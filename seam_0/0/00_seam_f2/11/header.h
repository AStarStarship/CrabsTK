#pragma once
#if SEAM == SEAM_0_0_0__11
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
