#if SEAM == SEAM_00_00_01__04
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
