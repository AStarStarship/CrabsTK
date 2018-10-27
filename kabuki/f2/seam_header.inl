#include <pch.h>

#include "ttest.h"
#if SEAM < SEAM_N
#define TEST_BEGIN \
  if (!_::TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define PRINT(item) _::Print(item)
#define PRINT_DATE(date) _::PrintDate()
#define PRINTF(format, ...) _::Printf(format, __VA_ARGS__)
#define PAUSE(message) _::Pause(message)
#define PAUSEF(format, ...) _::Pausef(format, __VA_ARGS__)
#define PRINT_HEADING(message) _::PrintHeading(message)
#define PRINT_LINE(c) _::PrintLine(c)
#define CHECK(condition) \
  if (!_::Test(condition)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define COMPARE(a, b) \
  if (!_::Test(a, b)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define ASSERT(condition) \
  if (!_::Test(condition)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define AVOW(a, b) \
  if (!_::Test(a, b)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define TEST_END Print("\n\nDone testing ", __FUNCTION__)
#else
#define TEST_BEGIN
#define PRINT(item)
#define PRINTF(format, ...)
#define PAUSE(message)
#define PAUSEF(format, ...)
#define PRINT_HEADING(message)
#define PRINT_LINE(c)
#define CHECK(condition)
#define COMPARE(a, b)
#define ASSERT(condition)
#define AVOW(a, b)
#define TEST_END
#endif
