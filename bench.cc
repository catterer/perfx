#include <thread>
#include <benchmark/benchmark.h>
#include "lock_queue.h"
#include "spinlock.h"

namespace perfx {

struct NonOpLock {
  void lock() {}
  void unlock() {}
};

struct LockQueuePar {
  using QType = LockQueue<int, 1'000, std::mutex>;
  static QType queue;
};
LockQueue<int, 1'000, std::mutex> LockQueuePar::queue;

template<bool SLEEP>
struct SpinlockQueuePar {
  using QType = LockQueue<int, 1'000, Spinlock<SLEEP>>;
  static QType queue;
};

template<bool SLEEP>
typename SpinlockQueuePar<SLEEP>::QType SpinlockQueuePar<SLEEP>::queue;

template<typename T>
void BM_LockQueue(benchmark::State& state) {
  auto N = state.range(0);
  for (auto _ : state) {
    for (auto i = 0; i < N; ++i) {
      T::queue.push(i);
      benchmark::DoNotOptimize(T::queue.pop());
    }
  }
  state.SetItemsProcessed(state.iterations() * N);
}


BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar)->Arg(1'000)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<true>)->Arg(1'000)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<false>)->Arg(1'000)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);

}

BENCHMARK_MAIN();

