//
// Created by Tianhao Xue on 3/8/2026.
//

#ifndef CHRONOS_CHRONOMETER_MANAGER_H
#define CHRONOS_CHRONOMETER_MANAGER_H

#include <atomic>
#include <chrono>
#include <cstdint>
#include <optional>
#include <shared_mutex>
#include <unordered_map>

namespace chronos {

// Tick precision used when reporting elapsed time.
enum class TickPrecision {
  kNanoseconds,
  kMicroseconds,
  kMilliseconds,
  kSeconds,
  kMinutes,
  kHours,
};

// ChronometerManager manages multiple timers identified by unique IDs
// and measures execution durations across threads.
//
// Usage:
//   auto& mgr = ChronometerManager::Instance();
//   uint64_t id = mgr.Start();
//   // ... work ...
//   auto ms = mgr.Stop(id, TickPrecision::kMilliseconds);
class ChronometerManager {
 public:
  // Returns the singleton instance.
  static ChronometerManager& Instance();

  // Not copyable or movable.
  ChronometerManager(const ChronometerManager&) = delete;
  ChronometerManager& operator=(const ChronometerManager&) = delete;
  ChronometerManager(ChronometerManager&&) = delete;
  ChronometerManager& operator=(ChronometerManager&&) = delete;

  // Starts a new timer and returns its unique id.
  uint64_t Start();

  // Stops the timer identified by |id| and returns the elapsed time
  // in the unit determined by |precision|.
  // Returns std::nullopt if the id is not found (e.g., already stopped
  // or never started).
  std::optional<int64_t> Stop(uint64_t id, TickPrecision precision = TickPrecision::kMilliseconds);

  // Returns the elapsed time for the timer identified by |id| without
  // stopping it.  The timer keeps running after this call.
  // Returns std::nullopt if the id is not found.
  std::optional<int64_t> Elapsed(uint64_t id, TickPrecision precision = TickPrecision::kMilliseconds) const;

 private:
  ChronometerManager() = default;
  ~ChronometerManager() = default;

  using Clock = std::chrono::steady_clock;
  using TimePoint = Clock::time_point;

  // Converts a duration to an integer tick count based on the given precision.
  static int64_t ConvertDuration(std::chrono::nanoseconds duration,
                                 TickPrecision precision) noexcept;

  std::atomic<uint64_t> next_id_{1};

  mutable std::shared_mutex mutex_;
  std::unordered_map<uint64_t, TimePoint> chronometers_;
};

}  // namespace chronos

#endif  // CHRONOS_CHRONOMETER_MANAGER_H
