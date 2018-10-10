#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/10/header.h
#if SEAM == SEAM_0_0_01__10
=======
#if SEAM == SEAM_00_00_01__10
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/10/header.h
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
