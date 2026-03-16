//
// Created by Theo Tianhao Xue on 2026/3/12.
//

#ifndef CHRONOS_CHRONOTIMER_H_
#define CHRONOS_CHRONOTIMER_H_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "threadpool.h"

namespace chronos {

// ChronoTimer manages scheduled tasks and fires them at specified time points.
//
// Internally owns a ThreadPool. The scheduling worker loop runs as a task
// inside the pool, and each due callback is also dispatched to the pool for
// concurrent execution.
//
// Usage:
//   chronos::ChronoTimer timer;      // uses default pool size
//   timer.Start();
//   timer.Schedule(std::chrono::seconds(3), [] { std::puts("Hello!"); });
//   timer.Stop();
class ChronoTimer {
 public:
  using Clock = std::chrono::system_clock;
  using TimePoint = Clock::time_point;
  using Duration = Clock::duration;
  using TaskCallback = std::function<void()>;

  // Creates a ChronoTimer with an internal thread pool of |thread_count|
  // threads. If |thread_count| is 0, defaults to
  // std::thread::hardware_concurrency().  At least 2 threads are required
  // (one for the worker loop, the rest for callbacks).
  explicit ChronoTimer(std::size_t thread_count = 0);
  ~ChronoTimer();

  // Not copyable or movable.
  ChronoTimer(const ChronoTimer&) = delete;
  ChronoTimer& operator=(const ChronoTimer&) = delete;
  ChronoTimer(ChronoTimer&&) = delete;
  ChronoTimer& operator=(ChronoTimer&&) = delete;

  // Starts the background worker loop by submitting it to the thread pool.
  void Start();

  // Stops the background worker loop. Pending tasks that have not yet
  // fired are discarded.
  void Stop();

  // Schedules |callback| to be invoked after |delay| from now.
  void Schedule(Duration delay, TaskCallback callback);

  // Schedules |callback| to be invoked at the absolute |time_point|.
  void ScheduleAt(TimePoint time_point, TaskCallback callback);

  // Returns the number of pending tasks in the queue.
  std::size_t PendingCount() const;

 private:
  // A scheduled task entry.
  struct Task {
    TimePoint trigger_time;
    TaskCallback callback;

    // For the min-heap: the task with the earliest trigger time has the
    // highest priority (i.e., should be at the top of the queue).
    bool operator>(const Task& other) const { return trigger_time > other.trigger_time; }
  };

  // The background worker loop.
  void WorkerLoop();

  std::unique_ptr<ThreadPool> pool_;

  mutable std::mutex mutex_;
  std::condition_variable cv_;

  // Min-heap: earliest trigger time on top.
  std::priority_queue<Task, std::vector<Task>, std::greater<Task>> task_queue_;

  bool running_ = false;
  bool worker_done_ = false;
  std::condition_variable done_cv_;
};

}  // namespace chronos

#endif  // CHRONOS_CHRONOTIMER_H_
