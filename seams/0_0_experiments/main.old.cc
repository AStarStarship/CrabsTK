inline void* align_gcc(size_t align, size_t size, void*& ptr,
                       size_t& space) noexcept {
  // Begin GCC Implementation
  intptr_t int_ptr = reinterpret_cast<intptr_t>(ptr),
           signed_align = ((intptr_t)align),
           aligned = (int_ptr + signed_align - 1) & -signed_align,
           diff = aligned - int_ptr;
  if ((size + diff) > space) return nullptr;
  space -= diff;
  return reinterpret_cast<void*>(aligned);
  // End GCC Implementation
}

inline void* align_kabuki(size_t align, size_t size, void*& ptr,
                          size_t& space) noexcept {
  // Begin Kabuki Toolkit Implementation
  intptr_t int_ptr = reinterpret_cast<intptr_t>(ptr),
           offset = (-int_ptr) & (align - 1);
  if ((space -= offset) < size) {
    space += offset;
    return nullptr;
  }
  return reinterpret_cast<void*>(int_ptr + offset);
  // End Kabuki Toolkit Implementation
}

void* align_vcpp(size_t alignment, size_t size, void*& ptr, size_t& space) {
  // Begin Visual-C++ Implementation
  // try to carve out _Size bytes on boundary _Bound
  size_t offset =
      static_cast<size_t>(reinterpret_cast<uintptr_t>(ptr) & (alignment - 1));
  if (offset != 0) {
    offset = alignment - offset;  // number of bytes to skip
  }

  if (space < offset || space - offset < size) {
    return nullptr;
  }

  // enough room, update
  ptr = static_cast<char*>(ptr) + offset;
  space -= offset;
  return ptr;
  // End Visual-C++ Implementation
}

/*
  void* ptr = 0;
  size_t align = 64, space = 0, size = 0;

  align_gcc(64, size, ptr, space);

  align_kabuki(64, size, ptr, space);

  align_vcpp(64, size, ptr, space);*/