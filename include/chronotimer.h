//
// Created by theo on 2026/3/12.
//

#ifndef CHRONOS_CHRONOTIMER_H_
#define CHRONOS_CHRONOTIMER_H_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace chronos {

// ChronoTimer manages scheduled tasks and fires them at specified time points.
//
// Tasks are stored in a min-heap (priority queue) sorted by trigger time.
// A background worker thread sleeps until the next task is due, then
// executes it.
//
// Usage:
//   chronos::ChronoTimer timer;
//   timer.Start();
//   timer.Schedule(std::chrono::seconds(3), [] { std::puts("Hello!"); });
//   // ... after 3 seconds, "Hello!" is printed.
//   timer.Stop();
class ChronoTimer {
 public:
  using Clock = std::chrono::steady_clock;
  using TimePoint = Clock::time_point;
  using Duration = Clock::duration;
  using TaskCallback = std::function<void()>;

  ChronoTimer() = default;
  ~ChronoTimer();

  // Not copyable or movable.
  ChronoTimer(const ChronoTimer&) = delete;
  ChronoTimer& operator=(const ChronoTimer&) = delete;
  ChronoTimer(ChronoTimer&&) = delete;
  ChronoTimer& operator=(ChronoTimer&&) = delete;

  // Starts the background worker thread.
  // Must be called before scheduling tasks.
  void Start();

  // Stops the background worker thread. Pending tasks that have not yet
  // fired are discarded.
  void Stop();

  // Schedules |callback| to be invoked after |delay| from now.
  void Schedule(Duration delay, TaskCallback callback);

  // Schedules |callback| to be invoked at the absolute |time_point|.
  void ScheduleAt(TimePoint time_point, TaskCallback callback);

  // Returns the number of pending tasks in the queue.
  size_t PendingCount() const;

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

  mutable std::mutex mutex_;
  std::condition_variable cv_;

  // Min-heap: earliest trigger time on top.
  std::priority_queue<Task, std::vector<Task>, std::greater<Task>> task_queue_;

  std::thread worker_;
  bool running_ = false;
};

}  // namespace chronos

#endif  // CHRONOS_CHRONOTIMER_H_
