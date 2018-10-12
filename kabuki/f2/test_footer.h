#pragma once
#if SEAM > 0 && SEAM < SEAM_N
#undef PRINT(item)
#undef PRINTF(x, ...)
#undef PAUSE(message)
#undef PAUSEF(message)
#undef TEST_BEGIN
#undef TEST1(item)
#undef TEST(a, b)
#undef ASSERT(condition)
#undef TEST_END
#endif
