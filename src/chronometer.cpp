//
// Created by Theo Tianhao Xue on 3/8/2026.
//
#include <limits>
#include <mutex>

#if defined(__i386__) || defined(__x86_64__)
#include <cpuid.h>
#include <x86intrin.h>
#endif

#include "chronometer.h"

namespace chronos {
namespace {

bool HasInvariantTsc() noexcept {
#if defined(__i386__) || defined(__x86_64__)
  if (__get_cpuid_max(0x80000000, nullptr) < 0x80000007) {
    return false;
  }

  unsigned int eax = 0;
  unsigned int ebx = 0;
  unsigned int ecx = 0;
  unsigned int edx = 0;
  __cpuid(0x80000007, eax, ebx, ecx, edx);
  return (edx & (1u << 8)) != 0;
#else
  return false;
#endif
}

uint64_t ReadTscRaw() noexcept {
#if defined(__i386__) || defined(__x86_64__)
  _mm_lfence();
  const auto cycles = __rdtsc();
  _mm_lfence();
  return cycles;
#else
  return 0;
#endif
}

struct RdtscConfig {
  bool enabled = false;
};

const RdtscConfig& GetRdtscConfig() noexcept {
  static const RdtscConfig config{HasInvariantTsc()};
  return config;
}

}  // namespace

Chronometer& Chronometer::Instance() {
  static Chronometer instance;
  return instance;
}

uint64_t Chronometer::Start(bool use_rdtsc) {
  TimerState timer;
  if (use_rdtsc && CanUseRdtsc()) {
    timer.tsc_start = ReadTsc();
  }
  // Always keep a steady_clock anchor so we can fall back safely.
  timer.steady_start = Clock::now();
  uint64_t id = next_id_.fetch_add(1, std::memory_order_relaxed);

  {
    std::unique_lock lock(mutex_);
    chronometers_.emplace(id, timer);
  }

  return id;
}

std::optional<int64_t> Chronometer::Stop(uint64_t id,
                                         TickPrecision precision,
                                         bool use_rdtsc) {
  std::optional<TimerCycles> now_cycles;
  if (use_rdtsc && CanUseRdtsc()) {
    now_cycles = ReadTsc();
  }
  // Capture the steady_clock time as a fallback path.
  TimePoint now = Clock::now();

  TimerState timer;
  {
    std::unique_lock lock(mutex_);
    auto it = chronometers_.find(id);
    if (it == chronometers_.end()) {
      return std::nullopt;
    }
    timer = it->second;
    chronometers_.erase(it);
  }

  if (now_cycles && timer.tsc_start && *now_cycles >= *timer.tsc_start) {
    const auto cycles = *now_cycles - *timer.tsc_start;
    if (cycles <= static_cast<TimerCycles>(std::numeric_limits<int64_t>::max())) {
      return static_cast<int64_t>(cycles);
    }
  }

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - timer.steady_start);
  return ConvertDuration(duration, precision);
}

std::optional<int64_t> Chronometer::Elapsed(uint64_t id,
                                            TickPrecision precision,
                                            bool use_rdtsc) const {
  std::optional<TimerCycles> now_cycles;
  if (use_rdtsc && CanUseRdtsc()) {
    now_cycles = ReadTsc();
  }
  // Capture the steady_clock time as a fallback path.
  TimePoint now = Clock::now();

  TimerState timer;
  {
    std::shared_lock lock(mutex_);
    auto it = chronometers_.find(id);
    if (it == chronometers_.end()) {
      return std::nullopt;
    }
    timer = it->second;
  }

  if (now_cycles && timer.tsc_start && *now_cycles >= *timer.tsc_start) {
    const auto cycles = *now_cycles - *timer.tsc_start;
    if (cycles <= static_cast<TimerCycles>(std::numeric_limits<int64_t>::max())) {
      return static_cast<int64_t>(cycles);
    }
  }

  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(now - timer.steady_start);
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

bool Chronometer::CanUseRdtsc() noexcept {
  return GetRdtscConfig().enabled;
}

Chronometer::TimerCycles Chronometer::ReadTsc() noexcept {
  return ReadTscRaw();
}

}  // namespace chronos
