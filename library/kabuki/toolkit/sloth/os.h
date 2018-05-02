/*$T \Sources/os.h GC 1.150 2011-09-22 20:52:18 */

/*
 * os.h - operating system specific dependancies ;
 * directory separator character - backslash on Windows, forward slash on Unix
 */
#ifdef _WIN32
#define DIR_SEPARATOR		'\\'
#define DIR_SEPARATOR_STR	"\\"
#else
#define DIR_SEPARATOR		'/'
#define DIR_SEPARATOR_STR	"/"
#endif
#ifdef _WIN32

/* Microsoft wants _ infront of a lot of system functions and definitions */
#define S_IREAD		_S_IREAD
#define S_IWRITE	_S_IWRITE
#define S_IEXEC		_S_IEXEC

#define strdup		_strdup
#define chmod		_chmod
#define getcwd		_getcwd
#define access		_access
#define unlink		_unlink

/* this function name is named differently on unix and windows */
#define strcasecmp	_stricmp

#else
#endif
#ifndef _MAX_PATH
#ifdef PATH_MAX
#define _MAX_PATH	PATH_MAX
#else
#define _MAX_PATH	4096
#endif
#endif
