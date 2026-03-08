//
// Created by Theo Tianhao Xue on 2026/3/11.
//

#include <chrono>
#include <iostream>
#include <thread>

#include "chronometer_manager.h"

int main() {
    auto& mgr = chronos::ChronometerManager::Instance();

    // -- 1. Basic usage: Start -> Stop --
    std::cout << "=== Basic Start/Stop ===\n";
    uint64_t id1 = mgr.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    auto ms = mgr.Stop(id1, chronos::TickPrecision::kMilliseconds);
    if (ms) {
        std::cout << "Timer 1 elapsed: " << *ms << " ms\n";
    }

    // -- 2. Query elapsed time without stopping the timer --
    std::cout << "\n=== Elapsed without stopping ===\n";
    uint64_t id2 = mgr.Start();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    auto snap1 = mgr.Elapsed(id2, chronos::TickPrecision::kMilliseconds);
    if (snap1) {
        std::cout << "Timer 2 snapshot 1: " << *snap1 << " ms\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    auto snap2 = mgr.Elapsed(id2, chronos::TickPrecision::kMilliseconds);
    if (snap2) {
        std::cout << "Timer 2 snapshot 2: " << *snap2 << " ms\n";
    }

    auto total = mgr.Stop(id2, chronos::TickPrecision::kMilliseconds);
    if (total) {
        std::cout << "Timer 2 total:      " << *total << " ms\n";
    }

    // -- 3. Multiple concurrent timers --
    std::cout << "\n=== Multiple concurrent timers ===\n";
    uint64_t fast = mgr.Start();
    uint64_t slow = mgr.Start();

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    auto fast_ms = mgr.Stop(fast, chronos::TickPrecision::kMicroseconds);
    if (fast_ms) {
        std::cout << "Fast timer: " << *fast_ms << " us\n";
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(300));
    auto slow_ms = mgr.Stop(slow, chronos::TickPrecision::kMilliseconds);
    if (slow_ms) {
        std::cout << "Slow timer: " << *slow_ms << " ms\n";
    }

    // -- 4. Invalid ID handling --
    std::cout << "\n=== Invalid ID handling ===\n";
    auto bad = mgr.Stop(99999);
    if (!bad) {
        std::cout << "Stop(99999) returned nullopt — as expected.\n";
    }

    // -- 5. Different precisions for the same timer --
    std::cout << "\n=== Different precisions ===\n";
    uint64_t id3 = mgr.Start();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    auto ns = mgr.Elapsed(id3, chronos::TickPrecision::kNanoseconds);
    auto us = mgr.Elapsed(id3, chronos::TickPrecision::kMicroseconds);
    auto s  = mgr.Stop(id3,    chronos::TickPrecision::kSeconds);

    if (ns) std::cout << "Nanoseconds:  " << *ns << " ns\n";
    if (us) std::cout << "Microseconds: " << *us << " us\n";
    if (s)  std::cout << "Seconds:      " << *s  << " s\n";

    return 0;
}
