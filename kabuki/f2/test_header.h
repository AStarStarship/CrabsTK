#if SEAM > 0 && SEAM <= SEAM_N
#define PRINT(item) Print(item)
#define PRINTF(format, ...) Printf(format, __VA_ARGS__)
#define PAUSE(message) Pause(message)
#define PAUSEF(format, ...) Pausef(format, __VA_ARGS)
#define PRINT_HEADING(message) PrintHeading(message)
#define PRINT_LINE(c) PrintLine(c)
#define TEST_BEGIN \
  if (!TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define TEST(a, b) \
  if (!Test(a, b)) HandleAssert(__FUNCTION, __FILE__, __LINE__)
#define TEST1(a) \
  if (!Test(a)) HandleAssert(__FUNCTION, __FILE__, __LINE__)
#define TEST_END Print("\n\nDone testing ", __FUNCTION__)
#else
#define PRINT(item)
#define PRINTF(x, ...)
#define PAUSE(message)
#define PAUSEF(x, ...)
#define PRINT_HEADING(c)
#define PRINT_LINE(c)
#define TEST_BEGIN
#define TEST1(item)
#define TEST(a, b)
#define ASSERT(condition)
#define TEST_END
#endif
