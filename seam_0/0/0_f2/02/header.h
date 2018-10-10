#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/02/header.h
#if SEAM == SEAM_0_0_0__02
=======
#if SEAM == SEAM_00_00_00__02
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/02/header.h
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message) Pause(message)
#define PRINT_HEADING(message) PrintHeading(message)
#define PRINT_LINE(c) PrintLine(c)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PAUSE(message)
#define PRINT_HEADING(c)
#define PRINT_LINE(c)
#endif
