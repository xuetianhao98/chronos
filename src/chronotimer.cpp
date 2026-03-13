//
// Created by theo on 2026/3/12.
//
#include "chronotimer.h"

namespace chronos {

ChronoTimer::~ChronoTimer() { Stop(); }

void ChronoTimer::Start() {
  std::lock_guard lock(mutex_);
  if (running_) {
    return;
  }
  running_ = true;
  worker_ = std::thread(&ChronoTimer::WorkerLoop, this);
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
  if (worker_.joinable()) {
    worker_.join();
  }
}

void ChronoTimer::Schedule(Duration delay, TaskCallback callback) {
  ScheduleAt(Clock::now() + delay, std::move(callback));
}

void ChronoTimer::ScheduleAt(TimePoint time_point, TaskCallback callback) {
  {
    std::lock_guard lock(mutex_);
    task_queue_.push(Task{time_point, std::move(callback)});
  }
  // Wake the worker so it can re-evaluate the earliest deadline.
  cv_.notify_one();
}

size_t ChronoTimer::PendingCount() const {
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

    if (auto next_time = task_queue_.top().trigger_time; Clock::now() >= next_time) {
      // Task is due — extract and execute.
      TaskCallback cb = std::move(const_cast<Task&>(task_queue_.top()).callback);
      task_queue_.pop();

      // Release the lock while executing the callback so that new tasks
      // can be scheduled concurrently.
      lock.unlock();
      cb();
      lock.lock();
    } else {
      // Sleep until the nearest trigger time or until a new task arrives.
      cv_.wait_until(lock, next_time, [this] {
        return !running_ ||
               (!task_queue_.empty() && task_queue_.top().trigger_time <= Clock::now());
      });
    }
  }
}

}  // namespace chronos