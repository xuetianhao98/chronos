# Chronos

Chronos is a small C++ utility library for working with time-related tasks.
It is designed as a lightweight collection of tools built on top of `std::chrono`.

The project currently provides a **multi-thread friendly execution timer manager** that can start, track, and stop multiple timers using unique IDs.

The library is written in **C++20** and follows the **Google C++ Style Guide**.

---

# Features

Currently implemented:

* **ChronometerManager**

    * Start multiple timers using unique IDs
    * Stop timers and obtain elapsed time
    * Query elapsed time without stopping the timer
    * Thread-safe usage
    * Multiple time precisions supported

Supported time precisions:

* Nanoseconds
* Microseconds
* Milliseconds
* Seconds
* Minutes
* Hours

---

# Example

```cpp
#include "chronometer_manager.h"

using namespace chronos;

int main() {
  auto& mgr = ChronometerManager::Instance();

  uint64_t id = mgr.Start();

  // ... do some work ...

  auto elapsed = mgr.Stop(id, TickPrecision::kMilliseconds);

  if (elapsed) {
    printf("Elapsed: %lld ms\n", *elapsed);
  }
}
```

---

# API Overview

### Start a timer

```cpp
uint64_t Start();
```

Creates a new timer and returns a unique ID.

---

### Stop a timer

```cpp
std::optional<int64_t> Stop(uint64_t id,
                            TickPrecision precision = TickPrecision::kMilliseconds);
```

Stops the timer and returns the elapsed time.

Returns `std::nullopt` if the timer ID does not exist.

---

### Query elapsed time

```cpp
std::optional<int64_t> Elapsed(uint64_t id,
                               TickPrecision precision = TickPrecision::kMilliseconds) const;
```

Returns elapsed time without stopping the timer.

Returns `std::nullopt` if the timer ID does not exist.

---

# Thread Safety

`ChronometerManager` is thread-safe.

* Timer IDs are generated using `std::atomic`
* Timer storage is protected by `std::shared_mutex`
* Multiple readers (`Elapsed`) can run concurrently

---

# Limitations

Current limitations:

* Uses a **singleton design**, which may not fit every use case.
* Timers are stored in an `unordered_map`, which may introduce overhead if used for extremely high-frequency timing.
* No automatic cleanup for long-running timers.
* No RAII-style scoped timer yet.

Future versions may include additional utilities for time scheduling, timers, and task management.

---

# Requirements

* C++20 compatible compiler
* Standard library with `std::chrono`
* POSIX or Windows environments supported by the standard library

---

# License

MIT License.
