#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/04/header.h
#if SEAM == SEAM_0_0_0__04
=======
#if SEAM == SEAM_00_00_00__04
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/04/header.h
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_DATE(date)                                                    \
  Printf("%i/%i/%i@%i:%i:%i", date.year + kSecondsPerEpoch, date.month + 1, \
         date.day, date.hour, date.minute, date.second);
#else
#define PRINTF(value, ...)
#define PRINT(item)
#define PRINT_DATE(date)
#endif
