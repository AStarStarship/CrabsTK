#include "ttest.h"
#define TEST_BEGIN \
  if (!_::TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define TEST_END Print("\n\nDone testing ", __FUNCTION__)
#define PRINT(item) _::Print(item)
#define PRINT_DATE(date) _::PrintDate()
#define PRINTF(format, ...) _::Printf(format, __VA_ARGS__)
#define PAUSE(message) _::Pause(message)
#define PAUSEF(format, ...) _::Pausef(format, __VA_ARGS__)
#define PRINT_HEADING(message) _::PrintHeading(message, 5)
#define PRINT_LINE(c) _::PrintLine(c)
#define PRINT_TYPE(type, value) Console<>().Out() << TypeValue(type, value)
#define PRINT_SOCKET(begin, end_or_size) PrintSocket(begin, end_or_size)
#define SOCKET_SAVE(cursor, end_a) Socket socket_to_print(cursor, end_a)
#define CHECK(condition) \
  if (!_::Test(condition)) _::AssertWarn(__FUNCTION__, __FILE__, __LINE__)
#define COMPARE(a, b)                               \
  if (!_::Test(a, b)) {                             \
    _::Print("\n\nExpecting:");                     \
    _::Print(a);                                    \
    _::Print("\nFound    :");                       \
    _::Print(b);                                    \
    _::AssertWarn(__FUNCTION__, __FILE__, __LINE__) \
  }
#define ASSERT(condition) \
  if (!_::Test(condition)) _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__)
#define AVOW(a, b)                                    \
  if (!_::Test(a, b)) {                               \
    _::Print("\n\nExpecting:");                       \
    _::Print(a);                                      \
    _::Print("\nFound    :");                         \
    _::Print(b);                                      \
    _::ErrorFreeze(__FUNCTION__, __FILE__, __LINE__); \
  }
