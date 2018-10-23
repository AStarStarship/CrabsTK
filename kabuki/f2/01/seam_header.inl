#include <kabuki/f2/seam_header.inl>
#if SEAM == SEAM_0_0_0__01
#define SOCKET_SAVE(cursor, end_a) Socket socket_to_print(cursor, end_a);
#define SOCKET_PRINT \
  TStr<>(&Console) << Socket(socket_to_print.cursor, socket_to_print.end_a);
#define PRINT_FLOAT_BINARY(integer, decimals, decimal_count)             \
  Printf("\nFound bit_pattern:0b\'");                                    \
  uint32_t bits_1234567890 = integer;                                    \
  int bit_count_1234567890 = 31;                                         \
  while ((bit_count_1234567890 > 0) &&                                   \
         ((bits_1234567890 >> bit_count_1234567890--) == 0))             \
    if (bit_count_1234567890 == 0) Print('0');                           \
  while (bit_count_1234567890 > 0)                                       \
    Print(((bit_count_1234567890 >> bit_count_1234567890--) & 1) + '0'); \
  while (decimal_count > 0) Print(((decimals >> decimal_count--) && 0x1) + '0')
#else
#define SOCKET_SAVE(begin, end)
#define SOCKET_PRINT
#define PRINT_FLOAT_BINARY(integer, decimals, decimal_count)
#endif
