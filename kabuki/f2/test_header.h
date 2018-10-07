#pragma once
#include <header.h>
#if SEAM > 0 && SEAM <= SEAM_N
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message)
#define PAUSEF(format, ...) Pausef(format, __VA_ARGS)
#define PAUSE(message) Pause(message)
#define PRINT_HEADING(message) PrintHeading(message)
#define PRINT_LINE(c) PrintLine(c)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PAUSE(message)
#define PAUSEF(x, ...)
#define PRINT_HEADING(c)
#define PRINT_LINE(c)
#endif
