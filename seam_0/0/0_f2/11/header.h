#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/11/header.h
#if SEAM == SEAM_0_0_0__11
=======
#if SEAM == SEAM_00_00_00__11
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/11/header.h
#ifndef PRINTF
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_HEADING                             \
  {                                               \
    Print('\n');                                  \
    for (int i = 80; i > 0; --i) std::cout << '-' \
  }
#define PRINT_TYPE(type, value) Console<>().Out() << TypeValue(type, value);
#define WIPE MapWipe<UI, SI>(map);
#define PRINT_LINE(token)                                                 \
  {                                                                       \
    for (int cout_123 = 80; count > 0; --count) std::cout << (char)token; \
  }
#endif
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#define PRINT_TYPE(type, value)
#define WIPE(buffer, size)
#define PRINT_LINE(token)
#endif
