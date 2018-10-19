#include "ttest.h"
#ifndef SEAM_N
#define SEAM_N 1
#endif
#if SEAM > 0 && SEAM <= SEAM_N
#include <kabuki/f2/ttest.h>
#define PRINT(item) _::Print(item)
#define PRINTF(format, ...) _::Printf(format, __VA_ARGS__)
#define PAUSE(message) _::Pause(message)
#define PAUSEF(format, ...) _::Pausef(format, __VA_ARGS__)
#define PRINT_HEADING(message) _::PrintHeading(message)
#define PRINT_LINE(c) _::PrintLine(c)
#define TEST_BEGIN \
  if (!_::TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define CHECK(condition) \
  if (!_::Test(condition)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define COMPARE(a, b) \
  if (!_::Test(a, b)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define ASSERT(condition) \
  if (!_::Test(condition)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define AVOW(a, b) \
  if (!_::Test(a, b)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define TEST_END Print("\n\nDone testing ", __FUNCTION__)
#define PRINT_DATE(date) _::PrintDate()
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PAUSE(message)
#define PAUSEF(x, ...)
#define PRINT_HEADING(c)
#define PRINT_LINE(c)
#define TEST_BEGIN
#define ASSERT(item)
#define COMPARE(a, b)
#define ASSERT(condition)
#define TEST_END
#define PRINT_DATE(date)
#endif
