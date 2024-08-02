#pragma once
#include <atomic>
#include <chrono>
#include <thread>

namespace perfx {

template<bool SLEEP = true>
class Spinlock {
public:
  Spinlock() = default;
  void lock() {
    unsigned i = 0;
    while (locked_.test_and_set(std::memory_order_acquire)) {
      if constexpr (SLEEP) {
        if ((++i & 7) == 0) {
          std::this_thread::sleep_for(std::chrono::nanoseconds(1));
        }
      }
    }
  }
  void unlock() {
    locked_.clear(std::memory_order_release);
  }
private:
  std::atomic_flag locked_ = ATOMIC_FLAG_INIT;
};

}
