#pragma once

#include <queue>
#include <optional>
#include <array>

#include "util.h"

namespace perfx {

template<typename T, std::size_t S, typename LockType=std::mutex>
class LockQueue: NonCopyable {
public:
  LockQueue() = default;

  std::optional<T> pop() {
    std::lock_guard<LockType> lg(lock_);
    if (prod_off_ == cons_off_)
      return std::nullopt;
    std::optional<T> res(std::move(data_[cons_off_]));
    cons_off_ = (cons_off_ + 1) % S;
    return res;
  }

  template<typename U>
  bool push(U&& val) {
    std::lock_guard<LockType> lg(lock_);
    if ((prod_off_ + 1) % S == cons_off_)
      return false; // queue full
    data_[prod_off_] = std::forward<U>(val);
    prod_off_ = (prod_off_ + 1) % S;
    return true;
  }

private:
  size_t prod_off_{};
  size_t cons_off_{};
  LockType lock_;
  std::array<T, S> data_;
};

}
