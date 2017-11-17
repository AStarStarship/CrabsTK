/*$T \Sources/in.h GC 1.150 2011-09-22 20:52:17 */


/*$6
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */


/*$F
    *****************************************************************************************
    GC GreatCode
    Original code by Christophe Beaudet
    e-mail: cbeaudet@club-internet.fr
    *****************************************************************************************
 */
#ifndef __IN_H__
#define __IN_H__

#include "lexi.h"

/*
 -----------------------------------------------------------------------------------------------------------------------
    An open file
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct FileDes_
{
	char			*psz_FileName;
	char			*pc_Buffer;
	struct token_	*pst_RootToken;
	struct token_	*pst_LastToken;
} FileDes;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

extern FileDes	gpst_Files[1000];
extern int		gi_NumFiles;
extern int		gi_MaxFiles;
extern char		*gpsz_RecurseDirs[MAX_DEF_FILESDIRS];
extern int		gi_NumRecurseDirs;
extern char		*gpsz_ScanFiles[MAX_DEF_FILESDIRS];
extern int		gi_NumScanFiles;
extern char		*gpsz_ExcludeDirs[MAX_DEF_FILESDIRS];
extern int		gi_NumExcludeDirs;
extern char		*gpsz_ExcludeFiles[MAX_DEF_FILESDIRS];
extern int		gi_NumExcludeFiles;
extern char		*gpsz_IncludeDirs[MAX_DEF_FILESDIRS];
extern int		gi_NumIncludesDirs;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

extern char OpenFile(int);
extern char MOpenFile(int);
extern void CloseFile(int);
extern void CreateFile(char *);
extern void CreateFilesInDir(char *);
#endif /* __IN_H__ */
