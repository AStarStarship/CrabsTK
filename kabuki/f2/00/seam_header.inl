#include "../seam_header.inl"
#if SEAM == SEAM_0_0_0__00
#define PRINT_PRINTED PrintPrinted<Char>(PuffItoSBegin<Char>())
#define BEGIN_ITOS_ALGORITHM                                          \
  static const char* ui_format = sizeof(UI) == 8 ? FORMAT_UI8 : "%u"; \
  PuffItoSBegin<Char>(cursor);                                        \
  for (int32_t i = 0; i < 10; ++i) *(cursor + i) = 'x';               \
  *(cursor + 21) = 0;                                                 \
  enum { kSize = 256 };                                               \
  char buffer[kSize];                                                 \
  sprintf_s(buffer, kSize, ui_format, value);                         \
  printf(" Expecting %s:%i ", buffer, StringLength<Char>(buffer))
#define PRINT_ARGS                                     \
  Printf("\nargs_count:%i args:%p", args_count, args); \
  for (int i = 0; i < args_count; ++i)                 \
  Printf("\n%i:\"%s\"", i, args[args_count])
#else
#define PRINT_PRINTED
#define BEGIN_ITOS_ALGORITHM
#define PRINT_ARGS
#endif
