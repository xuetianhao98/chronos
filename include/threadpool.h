//
// Created by Theo Tianhao Xue on 2026/3/16.
//

#ifndef CHRONOS_THREADPOOL_H_
#define CHRONOS_THREADPOOL_H_

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace chronos {

// ThreadPool maintains a fixed-size pool of worker threads and dispatches
// submitted tasks (std::function<void()>) to them in FIFO order.
//
// Usage:
//   chronos::ThreadPool pool(4);
//   pool.Submit([]{ DoWork(); });
//   // pool is joined automatically on destruction.
class ThreadPool {
 public:
  // Creates a thread pool with |thread_count| worker threads.
  // If |thread_count| is 0, defaults to std::thread::hardware_concurrency().
  explicit ThreadPool(std::size_t thread_count = 0);

  // Signals all workers to finish remaining tasks and joins them.
  ~ThreadPool();

  // Not copyable or movable.
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  // Enqueues a task for execution. The task will be picked up by the next
  // available worker thread.
  void Submit(std::function<void()> task);

  // Returns the number of worker threads in the pool.
  std::size_t Size() const noexcept;

 private:
  // Entry point for each worker thread.
  void WorkerLoop();

  std::vector<std::thread> workers_;
  std::queue<std::function<void()>> tasks_;

  std::mutex mutex_;
  std::condition_variable cv_;
  bool stop_{false};
};

}  // namespace chronos

#endif  // CHRONOS_THREADPOOL_H_
