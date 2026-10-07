// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_CONTIGUOUS_STACK_H
#define CRABS_TOOLKIT_UPDATE_CONTIGUOUS_STACK_H

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <type_traits>
#include <utility>

namespace CT {

/*
A contiguous POD stack that begins in object-local memory and grows to one heap
allocation. Checkpoints are counts, so rollback is exactly a sequence of pops.
*/
template<typename T, std::size_t StackTotal = 32>
class ContiguousStack {
  static_assert(std::is_trivially_copyable_v<T>);
  static_assert(StackTotal > 0);

 public:
  using Checkpoint = std::size_t;

  ContiguousStack() = default;

  ContiguousStack(const ContiguousStack&) = delete;
  ContiguousStack& operator=(const ContiguousStack&) = delete;

  ContiguousStack(ContiguousStack&& other) noexcept {
    MoveFrom(std::move(other));
  }

  ContiguousStack& operator=(ContiguousStack&& other) noexcept {
    if (this != &other) {
      Release();
      MoveFrom(std::move(other));
    }
    return *this;
  }

  ~ContiguousStack() { Release(); }

  std::size_t count() const { return count_; }
  std::size_t total() const { return heap_ ? heap_total_ : StackTotal; }
  bool empty() const { return count_ == 0; }
  bool is_dynamic() const { return heap_ != nullptr; }

  T* Begin() { return heap_ ? heap_ : stack_; }
  const T* Begin() const { return heap_ ? heap_ : stack_; }
  T* End() { return Begin() + count_; }
  const T* End() const { return Begin() + count_; }

  Checkpoint Mark() const { return count_; }

  bool Reserve(std::size_t requested_total) {
    if (requested_total <= total()) return true;
    if (requested_total > std::numeric_limits<std::size_t>::max() / sizeof(T))
      return false;

    T* growth = static_cast<T*>(std::malloc(requested_total * sizeof(T)));
    if (!growth) return false;
    std::memcpy(growth, Begin(), count_ * sizeof(T));
    std::free(heap_);
    heap_ = growth;
    heap_total_ = requested_total;
    return true;
  }

  bool Push(const T& item) {
    if (count_ == total()) {
      std::size_t current_total = total();
      std::size_t growth = current_total >
                                   std::numeric_limits<std::size_t>::max() / 2
                               ? current_total
                               : current_total * 2;
      if (growth == current_total || !Reserve(growth)) return false;
    }
    Begin()[count_++] = item;
    return true;
  }

  bool Pop(T& item) {
    if (count_ == 0) return false;
    item = Begin()[--count_];
    return true;
  }

  bool Pop() {
    if (count_ == 0) return false;
    --count_;
    return true;
  }

  T* Peek() { return count_ ? Begin() + count_ - 1 : nullptr; }
  const T* Peek() const { return count_ ? Begin() + count_ - 1 : nullptr; }

  bool Rollback(Checkpoint checkpoint) {
    if (checkpoint > count_) return false;
    while (count_ > checkpoint) Pop();
    return true;
  }

  void Clear() { count_ = 0; }

  T& operator[](std::size_t index) { return Begin()[index]; }
  const T& operator[](std::size_t index) const { return Begin()[index]; }

 private:
  void Release() {
    std::free(heap_);
    heap_ = nullptr;
    heap_total_ = 0;
    count_ = 0;
  }

  void MoveFrom(ContiguousStack&& other) {
    count_ = other.count_;
    if (other.heap_) {
      heap_ = other.heap_;
      heap_total_ = other.heap_total_;
      other.heap_ = nullptr;
      other.heap_total_ = 0;
    } else {
      std::memcpy(stack_, other.stack_, count_ * sizeof(T));
    }
    other.count_ = 0;
  }

  alignas(T) T stack_[StackTotal]{};
  T* heap_ = nullptr;
  std::size_t heap_total_ = 0;
  std::size_t count_ = 0;
};

}  // namespace CT
#endif
