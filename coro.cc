#ifdef __clang__
#       include <experimental/coroutine>
#else
#       include <coroutine>
#endif
#include <iostream>
#include <chrono>
#include <thread>

template <typename T>
struct generator {
  struct promise_type {
    T value_{};
    T return_value(T v) {
      std::cout << "RET " << v << "\n";
      return value_;
    }
    generator get_return_object() {
      using handle = std::coroutine_handle<promise_type>;
      std::cout << "get_return()\n";
      return generator(handle::from_promise(*this));
    }

    std::suspend_never initial_suspend() { std::cout << "INIT\n"; return {}; }
    std::suspend_never final_suspend() noexcept { std::cout << "FINAL\n"; return {}; }
    void unhandled_exception() {
      std::cout << "EXCEP\n";
    }
    std::suspend_always yield_value(T v) {
      value_ = v;
      return {};
    }
  };
  std::coroutine_handle<promise_type> h_;
};

generator<int> coro() {
  for (int i = 0;; ++i) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    co_yield i;
  }
}

int main(int argc, char** argv) {
  std::cout << "start\n";
  auto h = coro().h_;
  std::cout << "got handle\n";
  auto& promise = h.promise();
  std::cout << "got promise\n";
  for (int i = 0; i < 3; ++i) {
    std::cout << &promise << ":" << promise.value_ << ",\n";
    h();
  }
  std::cout << "\n";
  h.destroy();
  return 0;
}
