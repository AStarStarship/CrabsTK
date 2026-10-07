// Copyright AStarship <https://astarship.net>.
#pragma once
#ifndef CRABS_TOOLKIT_UPDATE_UNDO_STACK_H
#define CRABS_TOOLKIT_UPDATE_UNDO_STACK_H

#include "ContiguousStack.h"

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace CT {

struct UndoRecord {
  using Function = void (*)(void*, std::uintptr_t, std::uintptr_t) noexcept;

  Function undo;
  void* target;
  std::uintptr_t value_lsb;
  std::uintptr_t value_msb;
};

/* Every rollback operation is represented by and executed from one stack pop. */
template<std::size_t StackTotal = 32>
class UndoStack {
 public:
  using Checkpoint = typename ContiguousStack<UndoRecord, StackTotal>::Checkpoint;

  Checkpoint Mark() const { return records_.Mark(); }
  std::size_t count() const { return records_.count(); }
  bool is_dynamic() const { return records_.is_dynamic(); }

  bool Push(const UndoRecord& record) {
    if (!record.undo || !record.target) return false;
    return records_.Push(record);
  }

  bool Undo() {
    UndoRecord record{};
    if (!records_.Pop(record)) return false;
    record.undo(record.target, record.value_lsb, record.value_msb);
    return true;
  }

  bool Rollback(Checkpoint checkpoint) {
    if (checkpoint > records_.count()) return false;
    while (records_.count() > checkpoint)
      if (!Undo()) return false;
    return true;
  }

  template<typename T>
  bool Assign(T& target, const T& value) {
    static_assert(std::is_trivially_copyable_v<T>);
    static_assert(sizeof(T) <= 2 * sizeof(std::uintptr_t));
    std::uintptr_t words[2]{};
    std::memcpy(words, &target, sizeof(T));
    UndoRecord record{&Restore<T>, &target, words[0], words[1]};
    if (!Push(record)) return false;
    target = value;
    return true;
  }

 private:
  template<typename T>
  static void Restore(void* target, std::uintptr_t value_lsb,
                      std::uintptr_t value_msb) noexcept {
    std::uintptr_t words[2] = {value_lsb, value_msb};
    std::memcpy(target, words, sizeof(T));
  }

  ContiguousStack<UndoRecord, StackTotal> records_;
};

}  // namespace CT
#endif
