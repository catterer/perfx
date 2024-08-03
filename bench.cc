#include <thread>
#include <benchmark/benchmark.h>
#include "lock_queue.h"
#include "spinlock.h"
#include "queue_1p1c.h"

namespace perfx {

constexpr size_t kQueueSize = 1'000;

struct LockQueuePar {
  using QType = LockQueue<int, kQueueSize, std::mutex>;
  static QType queue;
};
LockQueuePar::QType LockQueuePar::queue;


template<bool SLEEP>
struct SpinlockQueuePar {
  using QType = LockQueue<int, kQueueSize, Spinlock<SLEEP>>;
  static QType queue;
};
template<bool SLEEP>
typename SpinlockQueuePar<SLEEP>::QType SpinlockQueuePar<SLEEP>::queue;


struct Queue1P1SPar {
  using QType = Queue1P1S<int, kQueueSize>;
  static QType queue;
};
Queue1P1SPar::QType Queue1P1SPar::queue;

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


template<typename T>
void BM_Queue1P1C(benchmark::State& state) {
  const bool producer = state.thread_index() & 1;
  auto N = state.range(0);
  for (auto _ : state) {
    for (auto i = 0; i < N; ++i) {
      if (producer)
        T::queue.push(i);
      else
        benchmark::DoNotOptimize(T::queue.pop());
    }
  }
  state.SetItemsProcessed(state.iterations() * N);
}



BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<true>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
//BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<false>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);

BENCHMARK_TEMPLATE(BM_Queue1P1C, LockQueuePar)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<true>)->Arg(1)->Threads(2);
//BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<false>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, Queue1P1SPar)->Arg(1)->Threads(2);


}

BENCHMARK_MAIN();

