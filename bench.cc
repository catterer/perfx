#include <thread>
#include <benchmark/benchmark.h>
#include "lock_queue.h"
#include "spinlock.h"
#include "queue_1p1c.h"

namespace perfx {

constexpr size_t kQueueSize = 1'000;

template<typename T = int>
struct LockQueuePar {
  using Type = T;
  using QType = LockQueue<T, kQueueSize, std::mutex>;
  static QType queue;
};
template<typename T>
typename LockQueuePar<T>::QType LockQueuePar<T>::queue;


template<typename T = int>
struct SpinlockQueuePar {
  using Type = T;
  using QType = LockQueue<T, kQueueSize, Spinlock>;
  static QType queue;
};
template<typename T>
typename SpinlockQueuePar<T>::QType SpinlockQueuePar<T>::queue;


template<typename T = int>
struct Queue1P1SPar {
  using Type = T;
  using QType = Queue1P1S<T, kQueueSize>;
  static QType queue;
};
template<typename T>
typename Queue1P1SPar<T>::QType Queue1P1SPar<T>::queue;

template<typename T>
void BM_LockQueue(benchmark::State& state) {
  auto N = state.range(0);
  for (auto _ : state) {
    for (auto i = 0; i < N; ++i) {
      T::queue.push(typename T::Type());
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
        T::queue.push(typename T::Type());
      else
        benchmark::DoNotOptimize(T::queue.pop());
    }
  }
  state.SetItemsProcessed(state.iterations() * N);
}



BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar<int>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<int>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);

BENCHMARK_TEMPLATE(BM_Queue1P1C, LockQueuePar<int>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<int>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, Queue1P1SPar<int>)->Arg(1)->Threads(2);

BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar<std::array<uint8_t, 32>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<std::array<uint8_t, 32>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);

BENCHMARK_TEMPLATE(BM_Queue1P1C, LockQueuePar<std::array<uint8_t, 32>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<std::array<uint8_t, 32>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, Queue1P1SPar<std::array<uint8_t, 32>>)->Arg(1)->Threads(2);

BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar<std::array<uint8_t, 256>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<std::array<uint8_t, 256>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);

BENCHMARK_TEMPLATE(BM_Queue1P1C, LockQueuePar<std::array<uint8_t, 256>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<std::array<uint8_t, 256>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, Queue1P1SPar<std::array<uint8_t, 256>>)->Arg(1)->Threads(2);

BENCHMARK_TEMPLATE(BM_LockQueue, LockQueuePar<std::array<uint8_t, 1024>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_LockQueue, SpinlockQueuePar<std::array<uint8_t, 1024>>)->Arg(1)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->Threads(16)->Threads(32)->Threads(64);
BENCHMARK_TEMPLATE(BM_Queue1P1C, LockQueuePar<std::array<uint8_t, 1024>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, SpinlockQueuePar<std::array<uint8_t, 1024>>)->Arg(1)->Threads(2);
BENCHMARK_TEMPLATE(BM_Queue1P1C, Queue1P1SPar<std::array<uint8_t, 1024>>)->Arg(1)->Threads(2);


}

BENCHMARK_MAIN();

