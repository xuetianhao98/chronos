# Chronos

Chronos is a small C++ utility library for working with time-related tasks.
It is designed as a lightweight collection of tools built on top of `std::chrono`.

The library is written in **C++20** and follows the **Google C++ Style Guide**.

---

# Features

Currently implemented:

* **Chronometer**

    * Start multiple timers using unique IDs
    * Stop timers and obtain elapsed time
    * Query elapsed time without stopping the timer
    * Thread-safe usage
    * Multiple time precisions supported
    * Optional `RDTSC` timing on supported x86/x64 CPUs

* **ChronoTimer**

    * Schedule tasks to run after a delay or at a specific time
    * Background worker thread for task execution
    * Thread-safe task scheduling
    * Support for relative and absolute timing

Supported time precisions:

* Nanoseconds
* Microseconds
* Milliseconds
* Seconds
* Minutes
* Hours

---

# Example

## Chronometer Example

```cpp
#include "chronometer.h"

using namespace chronos;

int main() {
  auto& mgr = Chronometer::Instance();

  uint64_t id = mgr.Start();

  // ... do some work ...

  auto elapsed = mgr.Stop(id, TickPrecision::kMilliseconds);

  if (elapsed) {
    printf("Elapsed: %lld ms\n", *elapsed);
  }
}
```

## ChronoTimer Example

```cpp
#include "chronotimer.h"
#include <iostream>

using namespace chronos;

int main() {
  ChronoTimer timer;
  timer.Start();

  // Schedule a task after 2 seconds
  timer.Schedule(std::chrono::seconds(2), [] {
    std::cout << "Task 1 fired after 2 seconds!" << std::endl;
  });

  // Schedule a task at an absolute time point
  auto future_time = ChronoTimer::Clock::now() + std::chrono::seconds(5);
  timer.ScheduleAt(future_time, [] {
    std::cout << "Task 2 fired at absolute time!" << std::endl;
  });

  // Wait for tasks to complete
  std::this_thread::sleep_for(std::chrono::seconds(6));

  timer.Stop();
}
```

---

# API Overview

## Chronometer API

### Start a timer

```cpp
uint64_t Start(bool use_rdtsc = false);
```

Creates a new timer and returns a unique ID.
If `use_rdtsc` is enabled, `Chronometer` records a TSC timestamp on supported
x86/x64 CPUs and falls back to `steady_clock` otherwise.

---

### Stop a timer

```cpp
std::optional<int64_t> Stop(uint64_t id,
                            TickPrecision precision = TickPrecision::kMilliseconds,
                            bool use_rdtsc = false);
```

Stops the timer and returns the elapsed time.
When `use_rdtsc` is requested, `Chronometer` prefers the TSC path only when the
timer has TSC state and the platform exposes a reliable invariant TSC. In that
mode, the return value is the raw elapsed cycle count and `precision` is
ignored.

Returns `std::nullopt` if the timer ID does not exist.

---

### Query elapsed time

```cpp
std::optional<int64_t> Elapsed(uint64_t id,
                               TickPrecision precision = TickPrecision::kMilliseconds,
                               bool use_rdtsc = false) const;
```

Returns elapsed time without stopping the timer.
As with `Stop`, the implementation falls back to `steady_clock` if `RDTSC`
cannot be used safely. When `RDTSC` is used, the return value is the raw
elapsed cycle count and `precision` is ignored.

Returns `std::nullopt` if the timer ID does not exist.

---

## ChronoTimer API

### Start/Stop Timer

```cpp
void Start();
void Stop();
```

`Start()` launches the background worker thread. Must be called before scheduling tasks.
`Stop()` terminates the worker thread and clears all pending tasks.

---

### Schedule Task (Relative)

```cpp
void Schedule(Duration delay, TaskCallback callback);
```

Schedules `callback` to be invoked after `delay` from now.

---

### Schedule Task (Absolute)

```cpp
void ScheduleAt(TimePoint time_point, TaskCallback callback);
```

Schedules `callback` to be invoked at the absolute `time_point`.

---

### Pending Tasks Count

```cpp
size_t PendingCount() const;
```

Returns the number of tasks currently in the queue.

---

# Thread Safety

`Chronometer` and `ChronoTimer` are thread-safe.

* `Chronometer`:
    * Timer IDs are generated using `std::atomic`
    * Timer storage is protected by `std::shared_mutex`
    * Multiple readers (`Elapsed`) can run concurrently
* `ChronoTimer`:
    * Task queue access is protected by `std::mutex`
    * Condition variable used for efficient worker thread synchronization
    * `Schedule` and `ScheduleAt` can be called from any thread

---

# Limitations

Current limitations:

* `Chronometer` uses a **singleton design**, which may not fit every use case.
* Timers are stored in an `unordered_map`, which may introduce overhead if used for extremely high-frequency timing.
* No automatic cleanup for long-running timers in `Chronometer`.
* No RAII-style scoped timer yet.
* `RDTSC` support is only available on x86/x64 CPUs with an invariant TSC and a
  stable TSC; unsupported systems automatically use `steady_clock`.

---

# Requirements

* C++20 compatible compiler
* Standard library with `std::chrono`
* POSIX or Windows environments supported by the standard library

---

# License

MIT License.
