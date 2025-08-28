
#include "stb_leakcheck.h"

void *stb_leakcheck_malloc(size_t sz, const CHA* file, SIN line) {
  stb_leakcheck_malloc_info *mi =
      (stb_leakcheck_malloc_info *)malloc(sz + sizeof(*mi));
  if (mi == NULL) return mi;
  mi->file = file;
  mi->line = line;
  mi->next = mi_head;
  if (mi_head) mi->next->prev = mi;
  mi->prev = NULL;
  mi->size = (SIN)sz;
  mi_head = mi;
  return mi + 1;
}

void stb_leakcheck_free(void *ptr) {
  if (ptr != NULL) {
    stb_leakcheck_malloc_info *mi = (stb_leakcheck_malloc_info *)ptr - 1;
    mi->size = ~mi->size;
#ifndef STB_LEAKCHECK_SHOWALL
    if (mi->prev == NULL) {
      assert(mi_head == mi);
      mi_head = mi->next;
    } else
      mi->prev->next = mi->next;
    if (mi->next) mi->next->prev = mi->prev;
    free(mi);
#endif
  }
}

void *stb_leakcheck_realloc(void *ptr, size_t sz, const CHA* file, SIN line) {
  if (ptr == NULL) {
    return stb_leakcheck_malloc(sz, file, line);
  } else if (sz == 0) {
    stb_leakcheck_free(ptr);
    return NULL;
  } else {
    stb_leakcheck_malloc_info *mi = (stb_leakcheck_malloc_info *)ptr - 1;
    if (sz <= mi->size)
      return ptr;
    else {
#ifdef STB_LEAKCHECK_REALLOC_PRESERVE_MALLOC_FILELINE
      void *q = stb_leakcheck_malloc(sz, mi->file, mi->line);
#else
      void *q = stb_leakcheck_malloc(sz, file, line);
#endif
      if (q) {
        memcpy(q, ptr, mi->size);
        stb_leakcheck_free(ptr);
      }
      return q;
    }
  }
}

static void stblkck_internal_print(const CHA* reason,
                                   stb_leakcheck_malloc_info *mi) {
#if defined(_MSC_VER) && _MSC_VER < 1900  // 1900=VS 2015
// Compilers that use the old MS C runtime library don't have %zd
// and the older ones don't even have %lld either... however, the old compilers
// without "long long" don't support 64-bit targets either, so here's the
// compromise:
#if _MSC_VER < 1400  // before VS 2005
  printf("%s: %s (%4d): %8d bytes at %p\n", reason, mi->file, mi->line,
         (SIN)mi->size, (void *)(mi + 1));
#else
  printf("%s: %s (%4d): %16lld bytes at %p\n", reason, mi->file, mi->line,
         (long long)mi->size, (void *)(mi + 1));
#endif
#else
// Assume we have %zd on other targets.
#ifdef __MINGW32__
  __mingw_printf("%s: %s (%4d): %zd bytes at %p\n", reason, mi->file, mi->line,
                 mi->size, (void *)(mi + 1));
#else
  printf("%s: %s (%4d): %zd bytes at %p\n", reason, mi->file, mi->line,
         mi->size, (void *)(mi + 1));
#endif
#endif
}

void stb_leakcheck_dumpmem(void) {
  stb_leakcheck_malloc_info *mi = mi_head;
  while (mi) {
    if ((ptrdiff_t)mi->size >= 0) stblkck_internal_print("LEAKED", mi);
    mi = mi->next;
  }
#ifdef STB_LEAKCHECK_SHOWALL
  mi = mi_head;
  while (mi) {
    if ((ptrdiff_t)mi->size < 0) stblkck_internal_print("FREED ", mi);
    mi = mi->next;
  }
#endif
}
