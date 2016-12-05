/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE OggVorbis SOFTWARE CODEC SOURCE CODE.   *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE OggVorbis SOURCE CODE IS (C) COPYRIGHT 1994-2002             *
 * by the Xiph.Org Foundation http://www.xiph.org/                  *
 *                                                                  *
 ********************************************************************

 function: #ifdef jail to whip a few platforms into the UNIX ideal.
 last mod: $Id: os_types.h 17712 2010-12-03 17:10:02Z xiphmont $

 ********************************************************************/
#ifndef _OS_TYPES_H
#define _OS_TYPES_H

/* make it easy on the folks that want to compile the libs with a
   different malloc than stdlib */
#define _ogg_malloc  malloc
#define _ogg_calloc  calloc
#define _ogg_realloc realloc
#define _ogg_free    free

#if defined(_WIN32) 

#  if defined(__CYGWIN__)
#    include <stdint.h>
     typedef int16_t int16_t;
     typedef uint16_t uint16_t;
     typedef int32_t int32_t;
     typedef uint32_t uint32_t;
     typedef int64_t int64_t;
     typedef uint64_t ogg_uint64_t;
#  elif defined(__MINGW32__)
#    include <sys/types.h>
     typedef short int16_t;
     typedef unsigned short uint16_t;
     typedef int int32_t;
     typedef unsigned int uint32_t;
     typedef long long int64_t;
     typedef unsigned long long ogg_uint64_t;
#  elif defined(__MWERKS__)
     typedef long long int64_t;
     typedef int int32_t;
     typedef unsigned int uint32_t;
     typedef short int16_t;
     typedef unsigned short uint16_t;
#  else
     /* MSVC/Borland */
     typedef __int64 int64_t;
     typedef __int32 int32_t;
     typedef unsigned __int32 uint32_t;
     typedef __int16 int16_t;
     typedef unsigned __int16 uint16_t;
#  endif

#elif defined(__MACOS__)

#  include <sys/types.h>
   typedef SInt16 int16_t;
   typedef UInt16 uint16_t;
   typedef SInt32 int32_t;
   typedef UInt32 uint32_t;
   typedef SInt64 int64_t;

#elif (defined(__APPLE__) && defined(__MACH__)) /* MacOS X Framework build */

#  include <inttypes.h>
   typedef int16_t int16_t;
   typedef uint16_t uint16_t;
   typedef int32_t int32_t;
   typedef uint32_t uint32_t;
   typedef int64_t int64_t;

#elif defined(__HAIKU__)

  /* Haiku */
#  include <sys/types.h>
   typedef short int16_t;
   typedef unsigned short uint16_t;
   typedef int int32_t;
   typedef unsigned int uint32_t;
   typedef long long int64_t;

#elif defined(__BEOS__)

   /* Be */
#  include <inttypes.h>
   typedef int16_t int16_t;
   typedef uint16_t uint16_t;
   typedef int32_t int32_t;
   typedef uint32_t uint32_t;
   typedef int64_t int64_t;

#elif defined (__EMX__)

   /* OS/2 GCC */
   typedef short int16_t;
   typedef unsigned short uint16_t;
   typedef int int32_t;
   typedef unsigned int uint32_t;
   typedef long long int64_t;

#elif defined (DJGPP)

   /* DJGPP */
   typedef short int16_t;
   typedef int int32_t;
   typedef unsigned int uint32_t;
   typedef long long int64_t;

#elif defined(R5900)

   /* PS2 EE */
   typedef long int64_t;
   typedef int int32_t;
   typedef unsigned uint32_t;
   typedef short int16_t;

#elif defined(__SYMBIAN32__)

   /* Symbian GCC */
   typedef signed short int16_t;
   typedef unsigned short uint16_t;
   typedef signed int int32_t;
   typedef unsigned int uint32_t;
   typedef long long int int64_t;

#elif defined(__TMS320C6X__)

   /* TI C64x compiler */
   typedef signed short int16_t;
   typedef unsigned short uint16_t;
   typedef signed int int32_t;
   typedef unsigned int uint32_t;
   typedef long long int int64_t;

#else

#  include <config_types.h>

#endif

#endif  /* _OS_TYPES_H */
