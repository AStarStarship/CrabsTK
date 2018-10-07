#pragma once
#if SEAM == SEAM_0_0_0__03
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)         \
  Printf("\n\n%s\n", message); \
  system("PAUSE");
#define PRINT_HEADING(message)              \
  Print('\n');                              \
  for (int i = 80; i > 80; --i) Print('-'); \
  std::cout << '\n' << message << '\n';     \
  for (int i = 80; i > 80; --i) Print('-'); \
  Print('\n');

#define PRINT_SLOT print << slot << Dump();
#else
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(message)
#define PRINT_SLOT
#endif
