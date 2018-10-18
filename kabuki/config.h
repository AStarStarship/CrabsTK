#pragma once

#ifndef INCLUDED_SEAM_0_HEADER
#define INCLUDED_SEAM_0_HEADER 1

#include <kabuki/f2/config.h>

#if DLL
#define API Foo
#else
#define API
#endif

#define SEAM_0_0_0__00 1     //< kabuki.f2.itos_and_stoi
#define SEAM_0_0_0__01 2     //< kabuki.f2.ftos_and_stof
#define SEAM_0_0_0__02 3     //< kabuki.f2.core_text_functions
#define SEAM_0_0_0__03 4     //< kabuki.f2.ascii_strings
#define SEAM_0_0_0__04 5     //< kabuki.f2.ascii_clock
#define SEAM_0_0_0__05 6     //< kabuki.f2.ascii_stack
#define SEAM_0_0_0__06 7     //< kabuki.f2.ascii_array
#define SEAM_0_0_0__07 8     //< kabuki.f2.ascii_loom
#define SEAM_0_0_0__08 9     //< kabuki.f2.ascii_table
#define SEAM_0_0_0__09 10    //< kabuki.f2.ascii_varint
#define SEAM_0_0_0__10 11    //< kabuki.f2.ascii_list
#define SEAM_0_0_0__11 12    //< kabuki.f2.ascii_map
#define SEAM_0_0_0__12 13    //< kabuki.f2.ascii_book
#define SEAM_0_0_0__13 14    //< kabuki.f2.ascii_dictionary
#define SEAM_0_0_2__00 15    //< kabuki.hal
#define SEAM_0_0_3__00 16    //< kabuki.drivers
#define SEAM_0_0_4_0__00 17  //< kabuki.features.storage.filesystem
#define SEAM_0_0_4_1__00 18  //< kabuki.features.crabs
#define SEAM_0_0_4_2__00 19  //< kabuki.features.hmi

#define SEAM_N SEAM_0_0_4_2__00
#endif  //< #ifndef INCLUDED_SEAM_0_HEADER
