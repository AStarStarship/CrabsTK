#include <kabuki/config.h>
#if SEAM == SEAM_0_0_0__05
#define PRINT_DATE(date)                                                    \
  Printf("%i/%i/%i@%i:%i:%i", date.year + kSecondsPerEpoch, date.month + 1, \
         date.day, date.hour, date.minute, date.second);
#else
#define PRINT_DATE(date)
#endif
