---
apply: always
---

# AI Coding Rules

## Google C++ Style Guide (C++20 Strict)

These rules define how AI must generate C++ code in this repository.

All code **must strictly follow the Google C++ Style Guide** and target **C++20**.

Primary goals:

1. Readability
2. Consistency
3. Maintainability
4. Modern C++ best practices

The AI must generate **production-quality C++ code**.

---

# 1 Language Standard

All generated code must compile with:

```
C++20
```

Prefer modern language features when appropriate.

Allowed examples:

```
constexpr
consteval
constinit
std::span
std::string_view
concepts
ranges
structured bindings
designated initializers
```

Do not use outdated constructs.

---

# 2 File Naming

Use **snake_case**.

Source files:

```
timer_manager.cc
order_book.cc
point_cloud_filter.cc
```

Header files:

```
timer_manager.h
order_book.h
point_cloud_filter.h
```

Google style prefers:

```
.cc
.h
```

---

# 3 Header File Structure

Every header must follow this structure:

```cpp
#ifndef PROJECT_MODULE_FILENAME_H_
#define PROJECT_MODULE_FILENAME_H_

#include <cstdint>
#include <string>

namespace project {
namespace module {

class Example {
 public:
  Example();
  ~Example();

 private:
  int value_;
};

}  // namespace module
}  // namespace project

#endif  // PROJECT_MODULE_FILENAME_H_
```

Rules:

* Use **include guards**
* Do not use `#pragma once`
* Prefer forward declarations when possible
* Avoid unnecessary includes

---

# 4 Include Order

Include order must follow this sequence:

```
1 related header
2 C system headers
3 C++ standard headers
4 third-party libraries
5 project headers
```

Example:

```cpp
#include "timer_manager.h"

#include <stdint.h>

#include <chrono>
#include <memory>
#include <vector>

#include "project/common/log.h"
```

Each include group must be separated by **one blank line**.

---

# 5 Naming Conventions

Follow **Google naming rules strictly**.

### Classes / Structs

```
UpperCamelCase
```

Example:

```
ChronometerManager
OrderBook
LaserFilter
```

---

### Functions

```
UpperCamelCase
```

Example:

```
StartTimer()
StopTimer()
ComputeChecksum()
```

---

### Variables

```
lower_snake_case
```

Example:

```
timer_id
elapsed_time
buffer_size
```

---

### Class Member Variables

Must end with `_`.

Example:

```
class Timer {
 private:
  int id_;
  bool running_;
};
```

---

### Constants

Use **kPrefix**.

Example:

```
constexpr int kMaxTimers = 1024;
constexpr double kTimeoutSeconds = 5.0;
```

---

### Macros

Use

```
ALL_CAPS_WITH_UNDERSCORES
```

Macros should be **avoided unless necessary**.

---

# 6 Formatting Rules

Formatting must follow **Google clang-format style**.

Indentation:

```
2 spaces
```

Maximum line length:

```
100 characters
```

Braces style:

```cpp
if (condition) {
  DoSomething();
}
```

Never:

```cpp
if (condition)
{
}
```

Pointer style:

```
int* ptr;
```

Not:

```
int *ptr;
```

---

# 7 Namespace Rules

All project code must be inside namespaces.

Example:

```cpp
namespace trading {
namespace engine {

class OrderBook {};

}  // namespace engine
}  // namespace trading
```

Avoid:

```
using namespace std;
```

Especially in header files.

---

# 8 Class Design

Access sections must appear in this order:

```
public
protected
private
```

Example:

```cpp
class Timer {
 public:
  Timer();
  void Start();

 private:
  int id_;
};
```

---

# 9 Constructors

Single-argument constructors must be `explicit`.

Example:

```cpp
explicit Timer(int id);
```

---

# 10 Special Member Functions

Follow **Rule of Zero** whenever possible.

Prefer:

```
default constructor
default destructor
default copy
default move
```

Define special member functions only when necessary.

---

# 11 Memory Management

Prefer RAII.

Allowed:

```
std::unique_ptr
std::shared_ptr (only when ownership is shared)
std::vector
std::string
std::array
```

Avoid:

```
raw new
delete
malloc
free
```

---

# 12 STL Usage

Prefer STL containers and algorithms.

Example:

Good:

```cpp
std::sort(values.begin(), values.end());
```

Avoid manual loops when standard algorithms exist.

---

# 13 Error Handling

Prefer:

```
std::optional
std::expected (C++23 style if available)
status objects
```

Avoid heavy use of exceptions in performance-critical code.

---

# 14 Thread Safety

Concurrent code must:

* protect shared data
* avoid data races
* use proper synchronization

Allowed primitives:

```
std::mutex
std::shared_mutex
std::atomic
std::condition_variable
```

Example:

```cpp
class TimerManager {
 public:
  int CreateTimer();

 private:
  std::mutex mutex_;
  std::unordered_map<int, Timer> timers_;
};
```

---

# 15 Singleton Pattern

Use **Meyers Singleton**.

Example:

```cpp
class TimerManager {
 public:
  static TimerManager& Instance() {
    static TimerManager instance;
    return instance;
  }

 private:
  TimerManager() = default;
};
```

---

# 16 Comments

Comments should explain **intent**, not obvious behavior.

Bad:

```cpp
i++;  // increment i
```

Good:

```cpp
// Move to the next buffer slot.
i++;
```

---

# 17 Documentation Comments

Class example:

```cpp
// ChronometerManager manages multiple timers and measures
// execution durations across threads.
class ChronometerManager {};
```

Function example:

```cpp
// Starts a timer and returns a unique timer id.
int StartTimer();
```

---

# 18 Source File Layout

Typical `.cc` file structure:

```
includes

namespace

constants

class implementations

helper functions
```

---

# 19 Testing

Unit tests must use **GoogleTest**.

Example:

```cpp
TEST(TimerTest, BasicStart) {
  Timer timer;
  timer.Start();
  EXPECT_TRUE(timer.Running());
}
```

---

# 20 AI Output Requirements

When generating C++ code, AI must ensure:

✔ code compiles with C++20
✔ Google naming conventions are followed
✔ headers have include guards
✔ no `using namespace std`
✔ no raw ownership pointers
✔ class member variables end with `_`
✔ indentation is 2 spaces
✔ line length ≤100
✔ code is readable and production quality

---

# 21 AI Generation Policy

The AI should:

* prefer clarity over clever tricks
* generate maintainable code
* use modern C++ features when appropriate
* avoid unnecessary abstractions
* avoid premature optimization

