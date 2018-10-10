#pragma once
#if SEAM == SEAM_0_0_0__00

#include <kabuki/f2/global.h>

namespace _ {

template <typename UI>
static void PrinttoSDebug(UI value) {
  enum { kSize = sizeof(UI) * 8 };

  const char* debug_table =
      sizeof(UI) == 8
          ? "\n    "
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "\n    "
            "6666555555555544444444443333333333222222222211111111110000000000"
            "\n    "
            "3210987654321098765432109876543210987654321098765432109876543210"
            "\n    "
            "|  |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |  |  |   |"
            "\n    "
            "2  1  1  1   1  1  1   1  1  1   1  0  0   0  0  0   0  0  0   0"
            "\n    "
            "0  9  8  7   6  5  4   3  2  1   0  9  8   7  6  5   4  3  2   1"
          : "\n    bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
            "\n    33222222222211111111110000000000"
            "\n    10987654321098765432109876543210"
            "\n    ||  |  |   |  |  |   |  |  |   |"
            "\n    |1  0  0   0  0  0   0  0  0   0"
            "\n    |0  9  8   7  6  5   4  3  2   1";

  PrintIndent(4);
  PrintBinary<UI>(value);
  Printf(debug_table);
}

static char* buffer_begin = 0;

template <typename UI>
static void PrintPrinted(char* cursor) {
  static const char* format = (sizeof(UI) == 8) ? FORMAT_UI8 : "%i";

  Printf("\n    Printed \"%s\" leaving value:\"%s\"", buffer_begin, cursor);
  char* start = cursor;
  while (*cursor++)
    ;
  Print(':');
  Printf(format, cursor - start);
}

}  // namespace _

#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PRINT_PRINTED PrintPrinted(buffer, buffer, value);

#define PRINT_BINARY \
  PrintIndent(4);    \
  PrintBinary(value);
#define PRINT_BINARY_TABLE PrintBinaryTable(value);
#define BEGIN_ITOS_ALGORITHM                            \
  buffer_begin = cursor;                                \
  for (int32_t i = 0; i < 10; ++i) *(cursor + i) = 'x'; \
  *(cursor + 21) = 0;                                   \
  char* buffer = cursor;                                \
  char buffer[256];                                     \
  sprintf_s(buffer, 256, "%u", value);                  \
  Printf("Expecting %s:%u", buffer, (uint)strlen(buffer));
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PRINT_PRINTED ;
#define BEGIN_ITOS_ALGORITHM
#endif
