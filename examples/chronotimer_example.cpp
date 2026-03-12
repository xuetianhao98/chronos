#include <chrono>
#include <iostream>

#include "chronotimer.h"

int main() {
  chronos::ChronoTimer timer;
  timer.Start();

  timer.Schedule(std::chrono::seconds(2), [] {
    std::cout << "Task A fired after 2s\n";
  });

  timer.Schedule(std::chrono::milliseconds(500), [] {
    std::cout << "Task B fired after 500ms\n";
  });

  timer.Schedule(std::chrono::seconds(1), [] {
    std::cout << "Task C fired after 1s\n";
  });

  // Output order: B (500ms) → C (1s) → A (2s)
  std::this_thread::sleep_for(std::chrono::seconds(3));
  timer.Stop();
  return 0;
}
