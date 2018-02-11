/*$T \Sources/in.c GC 1.150 2011-09-22 20:52:17 */


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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#ifndef __APPLE__
#include <malloc.h>
#endif
#include <ctype.h>

#ifdef _WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>	/* for stat() */
#include <dirent.h>
#endif
#include "tools.h"
#include "config.h"
#include "in.h"
#include "error.h"
#include "lexi.h"
#include "os.h"

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

#define GRAN_FILES	10
FileDes gpst_Files[1000];
char	gazstar[10 * 1024];
int		gazstarin = 0;
int		gi_NumFiles = 0;
int		gi_MaxFiles = 0;
char	*gpsz_RecurseDirs[MAX_DEF_FILESDIRS];
int		gi_NumRecurseDirs = 0;
char	*gpsz_ScanFiles[MAX_DEF_FILESDIRS];
int		gi_NumScanFiles = 0;
char	*gpsz_ExcludeDirs[MAX_DEF_FILESDIRS];
int		gi_NumExcludeDirs = 0;
char	*gpsz_ExcludeFiles[MAX_DEF_FILESDIRS];
int		gi_NumExcludeFiles = 0;
char	*gpsz_IncludeDirs[MAX_DEF_FILESDIRS];
int		gi_NumIncludesDirs = 0;

/*
 =======================================================================================================================
    Find if the given name is found within the table
 =======================================================================================================================
 */
char c_IsPresent(char *name, char **table, int table_entries)
{
	/*~~*/
	int i;
	/*~~*/

	for(i = 0; i < table_entries; i++)
	{
		if(!strcasecmp(name, table[i])) return 1;
	}

	return 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Replace(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	char	*temp;
	char	*memo;
	int		i, cpt;
	char	*p;
	/*~~~~~~~~~~~*/

	temp = (char *) __malloc__(strlen(pfile->pc_Buffer) * 10);
	strcpy(temp, pfile->pc_Buffer);
	memo = temp;
	p = pfile->pc_Buffer;
	while(*p)
	{
recom:
		if(ReplaceFirst[*p])
		{
			if(p != pfile->pc_Buffer && Lisword(p[-1])) goto zap;

			i = 0;
			for(cpt = 0; cpt < gi_NumReplace; cpt++)
			{
				while(gast_Replace[cpt].mpsz_Name1[i])
				{
					if(gast_Replace[cpt].mpsz_Name1[i] != p[i]) goto error;
					i++;
				}

				if(Lisword(p[i])) goto error;
				strcpy(temp, gast_Replace[cpt].mpsz_Name2);
				temp += strlen(gast_Replace[cpt].mpsz_Name2);
				p += i;
				goto recom;
error:
				i = 0;
			}
		}

		/* Copy buffer */
zap:
		*temp++ = *p++;
	}

	*temp = 0;
	free(pfile->pc_Buffer);
	pfile->pc_Buffer = memo;
}

/*
 =======================================================================================================================
    OpenFile : Try to open the file with name at the given index. If the filepart was NOT star open using the MOpenFile
    routine. Otherwise find all files in the directory and add file names to the gazstar block and then add this file
    name to the end of the file inforation table. Returns 0: No files 1: 1 file opened and read 2: Multiple files added
    to table
 =======================================================================================================================
 */
char OpenFile(int _i_Num)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~*/
#ifdef _WIN32
	/*
	 * intptr_t handle;
	 */
	long				handle;
	struct _finddata_t	t;
	FileDes				*pdes;
	/*~~~~~~~~~~~~~~~~~~~~~~~*/

	pdes = &gpst_Files[_i_Num];
	if(!strrchr(pdes->psz_FileName, '*')) return MOpenFile(_i_Num);
	handle = _findfirst(pdes->psz_FileName, &t);
	if(handle != -1) return 0;
	do
	{
		if(gi_NumFiles == 1000) Fatal("Too many input files", 0);
		strcpy(gazstar + gazstarin, t.name);
		gpst_Files[gi_NumFiles++].psz_FileName = gazstar + gazstarin;
		gazstarin += strlen(t.name) + 1;
	} while(_findnext(handle, &t) != -1);
	_findclose(handle);
	return 2;
#else
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	DIR				*dir;
	struct dirent	*d;
	FileDes			*pdes;
	char			dirname[_MAX_PATH];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pdes = &gpst_Files[_i_Num];
	if(!strrchr(pdes->psz_FileName, '*')) return MOpenFile(_i_Num);
	strcpy(dirname, pdes->psz_FileName);
	dirname[strlen(dirname) - 1] = '\0';	/* Kill the '*' */
	dir = opendir(dirname);
	if(!dir) return 0;

	for(d = readdir(dir); d != NULL; d = readdir(dir))
	{
		if(gi_NumFiles == 1000) Fatal("Too many input files", 0);
		strcpy(gazstar + gazstarin, d->d_name);
		gpst_Files[gi_NumFiles++].psz_FileName = gazstar + gazstarin;
		gazstarin += strlen(d->d_name) + 1;
	}

	return 2;
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		struct dirent	*d = readdir(dir);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		while(d)
		{
			if(gi_NumFiles == 1000) Fatal("Too many input files", 0);
			strcpy(gazstar + gazstarin, d->d_name);
			gpst_Files[gi_NumFiles++].psz_FileName = gazstar + gazstarin;
			gazstarin += strlen(d->d_name) + 1;
			d = readdir(dir);
		}
	}

	return 2;
#endif
}

/*
 =======================================================================================================================
    MOpenFileRec : Try to open the named file in the lowest child of the given path. i.e. recursivily decend to the
    lowest child directory and then find the first file with given name. (Not really sure why this is needed :-) )
 =======================================================================================================================
 */
FILE *MOpenFileRec(char *path, char *file)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifdef _WIN32
	struct _finddata_t	st_FileInfo;
	/*
	 * intptr_t handle;
	 */
	long				handle;
	char				asz_Temp[_MAX_PATH];
	FILE				*h;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	strcpy(asz_Temp, path);
	strcat(asz_Temp, "/*.*");
	handle = _findfirst(asz_Temp, &st_FileInfo);
	if(handle != -1)
	{
		do
		{
			if(st_FileInfo.attrib == _A_SUBDIR)
			{
				if(!strcasecmp(st_FileInfo.name, ".")) continue;
				if(!strcasecmp(st_FileInfo.name, "..")) continue;
				sprintf(asz_Temp, "%s/%s", path, st_FileInfo.name);
				h = MOpenFileRec(asz_Temp, file);
				if(h)
				{
					_findclose(handle);
					return h;
				}
			}
			else
			{
				if(!strcasecmp(file, st_FileInfo.name))
				{
					sprintf(asz_Temp, "%s/%s", path, st_FileInfo.name);
					h = fopen(asz_Temp, "rt");
					if(h)
					{
						_findclose(handle);
						return h;
					}
				}
			}
		} while(_findnext(handle, &st_FileInfo) != -1);
		_findclose(handle);
	}

	return NULL;
#else
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		DIR				*dir;
		struct dirent	*d;
		char			asz_Temp[_MAX_PATH];
		FILE			*h;
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		strcpy(asz_Temp, path);
		dir = opendir(asz_Temp);
		if(!dir)
		{
			for(d = readdir(dir); d != NULL; d = readdir(dir))
			{
				/*~~~~~~~~~~~~~~~~~*/
				struct stat stat_buf;
				/*~~~~~~~~~~~~~~~~~*/

				stat(d->d_name, &stat_buf);
				if(S_ISDIR(stat_buf.st_mode))
				{
					if(!strcasecmp(d->d_name, ".")) continue;
					if(!strcasecmp(d->d_name, "..")) continue;
					sprintf(asz_Temp, "%s/%s", path, d->d_name);
					h = MOpenFileRec(asz_Temp, file);
					if(h) return h;
				}
				else
				{
					if(!strcasecmp(file, d->d_name))
					{
						sprintf(asz_Temp, "%s/%s", path, d->d_name);
						h = fopen(asz_Temp, "rt");
						if(h) return h;
					}
				}
			}
		}
	}

	return NULL;
#endif
}

/*
 =======================================================================================================================
    MOpenFile : Open the file from table at given index (if cannot directly find it, try include driectories and "other
    direcotries" (Fail if cannot open file !) ... Allocate buffer big enough to hold the complete content of file and
    read the file into this buffer Do "magic replace" if requested Finally close file and return
 =======================================================================================================================
 */
char MOpenFile(int _i_Num)
{
	/*~~~~~~~~~~~~~~~~~~*/
	FILE	*h;
	long	len;
	FileDes *pdes;
	char	asz_Path[512];
	int		i;
	char	*pz;
	/*~~~~~~~~~~~~~~~~~~*/

	pdes = &gpst_Files[_i_Num];

	/* Open file to read */
	h = fopen(pdes->psz_FileName, "rt");
	if(!h)
	{
		for(i = 0; i < gi_NumIncludesDirs; i++)
		{
			sprintf(asz_Path, "%s/%s", gpsz_IncludeDirs[i], pdes->psz_FileName);
			h = fopen(asz_Path, "rt");
			if(h) goto ok;
		}

		if(Config.DepRec)
		{
			for(i = 0; i < gi_NumIncludesDirs; i++)
			{
				h = MOpenFileRec(gpsz_IncludeDirs[i], pdes->psz_FileName);
				if(h) goto ok;
			}
		}

		Warning("Unable to open source file", pdes->psz_FileName);
		return 1;
	}

	/* Get file length */
ok:
	fseek(h, 0L, SEEK_END);
	len = ftell(h);
	rewind(h);

	/* Do not process file wihtout datas */
	if(len == 0)
	{
		pdes->pc_Buffer = NULL;
		fclose(h);
		return 0;
	}

	/* Allocate buffer to read file */
	pdes->pc_Buffer = (char *) __malloc__(len + 1);
	if(pdes->pc_Buffer == NULL)
	{
		Fatal("Not enough memory to load source file", pdes->psz_FileName);
	}

	/* Read the file */
	len = fread(pdes->pc_Buffer, 1, len, h);
	pdes->pc_Buffer[len] = 0;
	if(Config.ReplaceOn) Replace(pdes);

	/* remove ALL CR characters from buffer (i.e. if reading DOS file on UNIX) */
	pz = strchr(pdes->pc_Buffer, '\r');

	if(pz != NULL)
	{
		/*~~~~~~~~~~~~~~~*/
		char	*from, *to;
		/*~~~~~~~~~~~~~~~*/

		for(from = &pz[1], to = pz; *from != '\0';)
		{
			if(*from == '\r')
				from++;
			else
				*to++ = *from++;
		}

		*to = '\0';
	}

	/* Close file */
	fclose(h);
	return 0;
}

/*
 =======================================================================================================================
    CloseFile : Free all tokens parsed from the file with given index (doesn't REALLY close any file :-) )
 =======================================================================================================================
 */
void CloseFile(int _i_Num)
{
	/*~~~~~~~~~~~~*/
	FileDes *pdes;
	token	*ptoken;
	token	*pnext;
	/*~~~~~~~~~~~~*/

	pdes = &gpst_Files[_i_Num];
	ptoken = pdes->pst_RootToken;
	while(ptoken)
	{
		if(ptoken->pc_Value) free(ptoken->pc_Value);
		pnext = ptoken->pst_Next;
		free(ptoken);
		ptoken = pnext;
	}
}

/*
 =======================================================================================================================
    Add filename to the list of files to be prrocessed (if it looks to be the RIGHT sort of file)
 =======================================================================================================================
 */
void CreateFile(char *_psz_FileName)
{
	/*~~~~~~~~~~~~~~*/
	char	*psz_Temp;
	/*~~~~~~~~~~~~~~*/

	/* Is the file exclude ? */
	if(c_IsPresent(_psz_FileName, gpsz_ExcludeFiles, gi_NumExcludeFiles)) return;

	/* Is it a correct extension ? */
	psz_Temp = strrchr(_psz_FileName, '.');
	if(!psz_Temp) return;
	psz_Temp++;

	if
	(
		(strcasecmp(psz_Temp, "c"))
	&&	(strcasecmp(psz_Temp, "cpp"))
	&&	(strcasecmp(psz_Temp, "cxx"))
	&&	(strcasecmp(psz_Temp, "h"))
	&&	(strcasecmp(psz_Temp, "hxx"))
	&&	(strcasecmp(psz_Temp, "hpp"))
	&&	(strcasecmp(psz_Temp, "java"))
	&&	(strcasecmp(psz_Temp, "inc"))
	&&	(strcasecmp(psz_Temp, "cs"))
	) return;

	/* First init file in array */
	memset(&gpst_Files[gi_NumFiles], 0, sizeof(FileDes));
	gpst_Files[gi_NumFiles].psz_FileName = strdup(_psz_FileName);

	/* One more gloal file */
	gi_NumFiles++;
}

/*
 =======================================================================================================================
    CreateFilesInDir : Find all files in given directory adding them to the global list of files If recure is turned on
    AND a directory if found, decend into that directory and recurse.
 =======================================================================================================================
 */
void CreateFilesInDir(char *_psz_Path)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#ifdef _WIN32
	struct _finddata_t	st_FileInfo;
	/*
	 * intptr_t handle;
	 */
	long				handle;
	char				asz_Temp[_MAX_PATH];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	strcpy(asz_Temp, _psz_Path);
	strcat(asz_Temp, "/*.*");
	handle = _findfirst(asz_Temp, &st_FileInfo);
	if(handle != -1)
	{
		do
		{
			/* Compute real name and file/dir bigfile name */
			strcpy(asz_Temp, _psz_Path);
			strcat(asz_Temp, "/");
			strcat(asz_Temp, st_FileInfo.name);

			/* One dir has been detected. Recurse call except for "." and ".." */
			if(st_FileInfo.attrib & _A_SUBDIR)
			{
				if(!strcasecmp(st_FileInfo.name, ".")) continue;
				if(!strcasecmp(st_FileInfo.name, "..")) continue;
				if(!c_IsPresent(asz_Temp, gpsz_ExcludeDirs, gi_NumExcludeDirs)) CreateFilesInDir(asz_Temp);
			}

			/* One file has been detected. Add it to bigfile */
			else
			{
				CreateFile(asz_Temp);
			}
		} while(_findnext(handle, &st_FileInfo) != -1);
		_findclose(handle);
	}

#else
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char			asz_Temp[_MAX_PATH];
	DIR				*dir;
	struct dirent	*d;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	strcpy(asz_Temp, _psz_Path);
	dir = opendir(asz_Temp);
	if(dir != NULL)
	{
		for(d = readdir(dir); d != NULL; d = readdir(dir))
		{
			/*~~~~~~~~~~~~~~~~~*/
			struct stat stat_buf;
			/*~~~~~~~~~~~~~~~~~*/

			stat(d->d_name, &stat_buf);

			/* Compute real name and file/dir bigfile name */
			strcpy(asz_Temp, _psz_Path);
			strcat(asz_Temp, "/");
			strcat(asz_Temp, d->d_name);

			/* One dir has been detected. Recurse call except for "." and ".." */
			if(S_ISDIR(stat_buf.st_mode))
			{
				if(!strcasecmp(d->d_name, ".")) continue;
				if(!strcasecmp(d->d_name, "..")) continue;
				if(!c_IsPresent(asz_Temp, gpsz_ExcludeDirs, gi_NumExcludeDirs)) CreateFilesInDir(asz_Temp);
			}

			/* One file has been detected. Add it to bigfile */
			else
			{
				CreateFile(asz_Temp);
			}
		}
	}
#endif
}
