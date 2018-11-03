#include "ctest.h"
#define TEST_BEGIN \
  if (!_::TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define TEST_END Print("\n\nDone testing ", __FUNCTION__)
#define PRINT(item)
#define PRINTF(format, ...)
#define PAUSE(message)
#define PAUSEF(format, ...)
#define PRINT_HEADING(message)
#define PRINT_LINE(c)
#define PRINT_TIME(date)
#define PRINT_TYPE(type, value)
#define PRINT_SOCKET(begin, end_or_size)
#define SOCKET_SAVE(begin, end)
#define ASSERT(condition)
#define CHECK(condition)
#define COMPARE(a, b)
#define AVOW(a, b)
