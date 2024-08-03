#include "util.h"

namespace perfx {

template<typename T, size_t S>
class Queue1P1S: NonCopyable {
public:
  Queue1P1S() = default;
  template<typename U>
  bool push(U&& u) {
    if (size_.load(std::memory_order_relaxed) >= data_.size())
      return false;
    data_[p_off_] = std::forward<U>(u);
    p_off_ = (p_off_ + 1) % data_.size();
    size_.fetch_add(1, std::memory_order_release);
    return true;
  }

  std::optional<T> pop() {
    if (size_.load(std::memory_order_acquire) == 0)
      return std::nullopt;
    std::optional<T> res = std::move(data_[c_off_]);
    c_off_ = (c_off_ + 1) % data_.size();
    size_.fetch_sub(1, std::memory_order_relaxed);
    return res;
  }

private:
  std::atomic<size_t> size_{0};
  size_t c_off_{0};
  size_t p_off_{0};
  std::array<T, S> data_;
};

}
