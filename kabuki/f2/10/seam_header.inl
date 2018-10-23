#include <kabuki/config.h>
#if SEAM == SEAM_0_0_0__12
#define PRINT_TYPE(type, value) Console<>().Out() << TypeValue(type, value);
#define WIPE MapWipe<UI, SI>(map);
#else
#define PRINT_TYPE(type, value)
#define WIPE(buffer, size)
#endif
