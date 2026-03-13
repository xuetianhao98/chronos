//
// Created by Tianhao Xue on 3/8/2026.
//
#include <mutex>

#include "chronometer.h"

namespace chronos {

Chronometer& Chronometer::Instance() {
  static Chronometer instance;
  return instance;
}

uint64_t Chronometer::Start() {
  // Capture the time point as early as possible to minimize overhead.
  TimePoint now = Clock::now();
  uint64_t id = next_id_.fetch_add(1, std::memory_order_relaxed);

  {
    std::unique_lock lock(mutex_);
    chronometers_.emplace(id, now);
  }

  return id;
}

std::optional<int64_t> Chronometer::Stop(uint64_t id, TickPrecision precision) {
  // Capture the time point as early as possible to minimize overhead.
  TimePoint now = Clock::now();

  TimePoint start_time;
  {
    std::unique_lock lock(mutex_);
    auto it = chronometers_.find(id);
    if (it == chronometers_.end()) {
      return std::nullopt;
    }
    start_time = it->second;
    chronometers_.erase(it);
  }

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time);
  return ConvertDuration(duration, precision);
}

std::optional<int64_t> Chronometer::Elapsed(uint64_t id, TickPrecision precision) const {
  // Capture the time point as early as possible to minimize overhead.
  TimePoint now = Clock::now();

  TimePoint start_time;
  {
    std::shared_lock lock(mutex_);
    auto it = chronometers_.find(id);
    if (it == chronometers_.end()) {
      return std::nullopt;
    }
    start_time = it->second;
  }

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time);
  return ConvertDuration(duration, precision);
}

int64_t Chronometer::ConvertDuration(std::chrono::nanoseconds duration,
                                            TickPrecision precision) noexcept {
  namespace sc = std::chrono;
  switch (precision) {
    case TickPrecision::kNanoseconds:
      return duration.count();
    case TickPrecision::kMicroseconds:
      return sc::duration_cast<sc::microseconds>(duration).count();
    case TickPrecision::kMilliseconds:
      return sc::duration_cast<sc::milliseconds>(duration).count();
    case TickPrecision::kSeconds:
      return sc::duration_cast<sc::seconds>(duration).count();
    case TickPrecision::kMinutes:
      return sc::duration_cast<sc::minutes>(duration).count();
    case TickPrecision::kHours:
      return sc::duration_cast<sc::hours>(duration).count();
  }
  // Fallback — should never reach here.
  return sc::duration_cast<sc::milliseconds>(duration).count();
}

}  // namespace chronos