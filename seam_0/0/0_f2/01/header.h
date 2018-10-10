#pragma once
<<<<<<< HEAD:seam_0/0/00_seam_f2/01/header.h
#if SEAM == SEAM_0_0_0__01
=======
#if SEAM == SEAM_00_00_00__01
>>>>>>> af98cdd86f8b7b5188063c203df0e9dd4e771336:seam_0/0/00_seam_f2/01/header.h
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED                                                   \
  sprintf_s(buffer, 24, "%u", value);                                   \
  *end = 0;                                                             \
  Printf("\n    Printed \"%s\" leaving value:\"%s\":%u", begin, buffer, \
         StringLength<>(buffer))
#define PRINT_BINARY \
  PrintIndent(4);    \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define PRINT_HEADER                   \
  for (int i = 0; i < 10; ++i) {       \
    *(cursor + i) = 'x';               \
  }                                    \
  *(cursor + 21) = 0;                  \
  Char* begin = cursor;                \
  Char buffer[256];                    \
  sprintf_s(buffer, 256, "%u", value); \
  Printf("Expecting %s:%u", buffer, StringLength<>(buffer));
#define BEGIN_ITOS_ALGORITHM \
  Print('\n');               \
  for (int i = 80; i > 0; --i) Print('-')
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED
#define PRINT_HEADER
#define PRINT_HEADING
#endif
