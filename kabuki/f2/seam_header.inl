#include "ttest.h"
#ifndef SEAM_N
#define SEAM_N 1
#endif
#if SEAM > 0 && SEAM <= SEAM_N
#include <kabuki/f2/ttest.h>
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
#define DPRINT(item) _::Print(item)
#define DPRINT_DATE(date) _::PrintDate()
#define DPRINTF(format, ...) _::Printf(format, __VA_ARGS__)
#define DPAUSE(message) _::Pause(message)
#define DPAUSEF(format, ...) _::Pausef(format, __VA_ARGS__)
#define DPRINT_HEADING(message) _::PrintHeading(message)
#define DPRINT_LINE(c) _::PrintLine(c)
#define DTEST_BEGIN \
  if (!_::TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define DCHECK(condition) \
  if (!_::Test(condition)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define DCOMPARE(a, b) \
  if (!_::Test(a, b)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define DASSERT(condition) \
  if (!_::Test(condition)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define DAVOW(a, b) \
  if (!_::Test(a, b)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define DTEST_END Print("\n\nDone testing ", __FUNCTION__)
#else
#define PRINT(item) _::Print(item)
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
#define DPRINT(item)
#define DPRINT_DATE(date)
#define DPRINTF(x, ...)
#define DPAUSE(message)
#define DPAUSEF(x, ...)
#define DPRINT_HEADING(c)
#define DPRINT_LINE(c)
#define DTEST_BEGIN
#define DASSERT(item)
#define DCOMPARE(a, b)
#define DASSERT(condition)
#define DTEST_END
#endif
