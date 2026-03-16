//
// Created by Theo Tianhao Xue on 2026/3/12.
//
#include "chronotimer.h"

#include <cstddef>
#include <algorithm>
#include <utility>

namespace chronos {

ChronoTimer::ChronoTimer(std::size_t thread_count) {
  // Ensure at least 2 threads: one for the worker loop, the rest for
  // executing due callbacks concurrently.
  constexpr std::size_t kMinThreads = 2;
  if (thread_count < kMinThreads) {
    thread_count = std::max(kMinThreads,
                            static_cast<std::size_t>(
                                std::thread::hardware_concurrency()));
  }
  pool_ = std::make_unique<ThreadPool>(thread_count);
}

ChronoTimer::~ChronoTimer() { Stop(); }

void ChronoTimer::Start() {
  std::lock_guard lock(mutex_);
  if (running_) {
    return;
  }
  running_ = true;
  worker_done_ = false;
  pool_->Submit([this] { WorkerLoop(); });
}

void ChronoTimer::Stop() {
  {
    std::lock_guard lock(mutex_);
    if (!running_) {
      return;
    }
    running_ = false;
  }
  cv_.notify_all();

  // Wait until the worker loop has finished.
  std::unique_lock lock(mutex_);
  done_cv_.wait(lock, [this] { return worker_done_; });
}

void ChronoTimer::Schedule(Duration delay, TaskCallback callback) {
  ScheduleAt(Clock::now() + delay, std::move(callback));
}

void ChronoTimer::ScheduleAt(TimePoint time_point, TaskCallback callback) {
  {
    std::lock_guard lock(mutex_);
    task_queue_.push(Task{time_point, std::move(callback)});
  }
  cv_.notify_one();
}

std::size_t ChronoTimer::PendingCount() const {
  std::lock_guard lock(mutex_);
  return task_queue_.size();
}

void ChronoTimer::WorkerLoop() {
  std::unique_lock lock(mutex_);
  while (running_) {
    if (task_queue_.empty()) {
      // No tasks — sleep until notified.
      cv_.wait(lock, [this] { return !running_ || !task_queue_.empty(); });
      continue;
    }

    if (auto next_time = task_queue_.top().trigger_time;
        Clock::now() >= next_time) {
      // Task is due — dispatch it to the pool for async execution.
      TaskCallback cb = std::move(const_cast<Task&>(task_queue_.top()).callback);
      task_queue_.pop();

      pool_->Submit(std::move(cb));
    } else {
      // Sleep until the nearest trigger time or until a new task arrives.
      cv_.wait_until(lock, next_time, [this] {
        return !running_ ||
               (!task_queue_.empty() && task_queue_.top().trigger_time <= Clock::now());
      });
    }
  }

  worker_done_ = true;
  done_cv_.notify_all();
}

}  // namespace chronos