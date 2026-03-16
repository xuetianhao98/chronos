//
// Created by Theo Tianhao Xue on 2026/3/16.
//

#include "threadpool.h"

#include <cstddef>
#include <functional>
#include <mutex>
#include <thread>
#include <utility>

namespace chronos {

ThreadPool::ThreadPool(std::size_t thread_count) {
  if (thread_count == 0) {
    thread_count = std::thread::hardware_concurrency();
    if (thread_count == 0) {
      thread_count = 1;
    }
  }

  workers_.reserve(thread_count);
  for (std::size_t i = 0; i < thread_count; ++i) {
    workers_.emplace_back(&ThreadPool::WorkerLoop, this);
  }
}

ThreadPool::~ThreadPool() {
  {
    std::lock_guard lock(mutex_);
    stop_ = true;
  }
  cv_.notify_all();

  for (auto& worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void ThreadPool::Submit(std::function<void()> task) {
  {
    std::lock_guard lock(mutex_);
    tasks_.push(std::move(task));
  }
  cv_.notify_one();
}

std::size_t ThreadPool::Size() const noexcept { return workers_.size(); }

void ThreadPool::WorkerLoop() {
  while (true) {
    std::function<void()> task;
    {
      std::unique_lock lock(mutex_);
      cv_.wait(lock, [this] { return stop_ || !tasks_.empty(); });

      // Drain remaining tasks before exiting.
      if (tasks_.empty() && stop_) {
        return;
      }

      task = std::move(tasks_.front());
      tasks_.pop();
    }
    task();
  }
}

}  // namespace chronos
