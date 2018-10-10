#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/03/header.h
#if SEAM == SEAM_0_0_0__03
=======
#if SEAM == SEAM_00_00_00__03
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/03/header.h
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
