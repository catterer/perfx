#include <thread>
#include <atomic>
#include <iostream>
#include <vector>
#include <cstdlib>

std::atomic<int> globcnt = 0;
char arr[256];
char arr1[256];
char arr2[256];
int must_be_same = 0;
std::atomic<int> bad_count = 0;
std::atomic<int> num_iter = 0;

template<int ODD>
void run_thread() {
  static_assert(ODD == 0 || ODD == 1);
  for (;;) {
    if (globcnt.load(std::memory_order_acquire) % 2 == ODD) {
      while (rand() % 8 != 0) {}
      if (must_be_same != globcnt) {
        bad_count.fetch_add(1, std::memory_order_relaxed);
      }
      num_iter.fetch_add(1, std::memory_order_relaxed);
      ++must_be_same;
      globcnt.fetch_add(1, std::memory_order_release);
    }
  }
}

int main() {
  arr[0] = 1;
  arr[1] = 1;
  arr[2] = 1;
  std::vector<std::thread> threads;
  threads.emplace_back(std::thread(&run_thread<0>));
  threads.emplace_back(std::thread(&run_thread<1>));
  for (;;) {
    int last_bc = 0;
    if (last_bc != bad_count) {
      std::cout << bad_count << " " << num_iter << "\n";
      last_bc = bad_count;
    }
  }
  for (auto& t: threads)
    t.join();
  return 0;
}
