#include <header.h>
#if SEAM == SEAM_0_0_0__01
#define PRINT_PRINTED                                                   \
  sprintf_s(buffer, 24, "%u", value);                                   \
  *end = 0;                                                             \
  Printf("\n    Printed \"%s\" leaving value:\"%s\":%u", begin, buffer, \
         StringLength<>(buffer))
#define PRINT_BINARY \
  PrintIndent(4);    \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define BEGIN_ITOS_ALGORITHM           \
  for (int i = 0; i < 10; ++i) {       \
    *(cursor + i) = 'x';               \
  }                                    \
  *(cursor + 21) = 0;                  \
  Char* begin = cursor;                \
  Char buffer[256];                    \
  sprintf_s(buffer, 256, "%u", value); \
  Printf("Expecting %s:%u", buffer, StringLength<>(buffer));
#else
#define PRINT_PRINTED
#define BEGIN_ITOS_ALGORITHM
#endif
