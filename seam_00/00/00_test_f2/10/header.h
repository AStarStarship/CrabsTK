#if SEAM == SEAM_00_00_01__09
#ifndef PRINTF
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT(c) Print(c)
#define PRINT_HEADING \
  Print('\n');        \
  for (int i = 80; i > 0; --i) std::cout << '-'
#define PRINT_TYPE(type, value) Console<>().Out() << TypeValue(type, value);
#define WIPE ListWipe<UI, SI>(list);
#endif
#else
#define PRINTF(x, ...)
#define PRINT(c)
#define PRINT_HEADING
#define PRINT_TYPE(type, value)
#define WIPE(buffer, size)
#endif