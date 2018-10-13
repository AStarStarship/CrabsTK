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
#define DPRINT(item)
#define DPRINTF(x, ...)
#define DPAUSE(message)
#define DPAUSEF(x, ...)
#define DPRINT_HEADING(c)
#define DPRINT_LINE(c)
#define DTEST_BEGIN
#define DTEST1(item)
#define DTEST(a, b)
#define DASSERT(condition)
#define DTEST_END
#else
#define RPRINT(item) Print(item)
#define RPRINTF(format, ...) Printf(format, __VA_ARGS__)
#define RPAUSE(message) Pause(message)
#define RPAUSEF(format, ...) Pausef(format, __VA_ARGS)
#define RPRINT_HEADING(message) PrintHeading(message)
#define RPRINT_LINE(c) PrintLine(c)
#define RTEST_BEGIN \
  if (!TestBegin(seam_log, seam_end, args)) return __FUNCTION__
#define RTEST(a, b) \
  if (!Test(a, b)) HandleAssert(__FUNCTION, __FILE__, __LINE__)
#define RTEST1(a) \
  if (!Test(a)) HandleAssert(__FUNCTION, __FILE__, __LINE__)
#define RTEST_END Print("\n\nDone testing ", __FUNCTION__)
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
