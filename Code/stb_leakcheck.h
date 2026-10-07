// stb_leakcheck.h - v0.5 - quick & dirty malloc leak-checking - public domain
// LICENSE
//
//   See end of file.

#ifdef STB_LEAKCHECK_IMPLEMENTATION
#undef STB_LEAKCHECK_IMPLEMENTATION  // don't implement more than once

// if we've already included leakcheck before, undefine the macros
#ifdef malloc
#undef malloc
#undef free
#undef realloc
#endif

typedef struct malloc_info stb_leakcheck_malloc_info;

struct malloc_info {
  const CHA* file;
  ISN line;
  size_t size;
  stb_leakcheck_malloc_info *next, *prev;
};

static stb_leakcheck_malloc_info *mi_head;

void *stb_leakcheck_malloc(size_t sz, const CHA* file, ISN line);

void stb_leakcheck_free(void *ptr);

void *stb_leakcheck_realloc(void *ptr, size_t sz, const CHA* file, ISN line);

static void stblkck_internal_print(const CHA* reason,
                                   stb_leakcheck_malloc_info *mi);

void stb_leakcheck_dumpmem(void);
#endif  // STB_LEAKCHECK_IMPLEMENTATION

#ifndef INCLUDE_STB_LEAKCHECK_H
#define INCLUDE_STB_LEAKCHECK_H

#include <stdlib.h>  // we want to define the macros *after* stdlib to avoid a slew of errors

#define malloc(sz) stb_leakcheck_malloc(sz, __FILE__, __LINE__)
#define free(p) stb_leakcheck_free(p)
#define realloc(p, sz) stb_leakcheck_realloc(p, sz, __FILE__, __LINE__)

extern void *stb_leakcheck_malloc(size_t sz, const CHA* file, ISN line);
extern void *stb_leakcheck_realloc(void *ptr, size_t sz, const CHA* file,
                                   ISN line);
extern void stb_leakcheck_free(void *ptr);
extern void stb_leakcheck_dumpmem(void);

#endif
