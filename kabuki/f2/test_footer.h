#pragma once
#if SEAM > 0 && SEAM < SEAM_N
#undef PRINT(item)
#undef PRINTF(x, ...)
#undef PAUSE(message)
#undef PAUSEF(message)
#undef COMPARE_BEGIN
#undef COMPARE1(item)
#undef COMPARE(a, b)
#undef ASSERT(condition)
#undef COMPARE_END
#endif
