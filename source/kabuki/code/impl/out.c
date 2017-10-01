/*$T \Sources/out.c GC 1.150 2011-09-22 20:52:18 */


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

#ifdef _WIN32
#include <sys/stat.h>
#include <io.h>
#else
#include <sys/stat.h>
#endif
#include "config.h"
#include "in.h"
#include "lexi.h"
#include "error.h"
#include "tools.h"
#include "os.h"

/*$4
 ***********************************************************************************************************************
 ***********************************************************************************************************************
 */

int gi_NumLines = 0;
int gi_NumChars = 0;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void WriteToken(FILE *h, FileDes *pfile, token *pcur)
{
	/*~~~~~~~~~~~~~~~~*/
	int		i, jj;
	int		col, col1;
	int		len;
	char	*pz, *plast;
	/*~~~~~~~~~~~~~~~~*/

	Tool_ComputeCol(pfile);
	while(pcur)
	{
		/* Unix style for EOL */
		if(Config.UnixEOL)
		{
			/* remove ALL CR characters from token */
			pz = strchr(pcur->pc_Value, '\r');

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

			/*
			 * while(pz) { if(pz[1] == '\n') { memmove(pz, pz + 1, strlen(pz) - 1);
			 * pz[strlen(pz) - 1] = 0;
			 * } pz = strchr(pz + 1, '\r');
			 * }
			 */
		}

		/* Dos style */
		else
		{
			len = strlen(pcur->pc_Value);
			pz = strchr(pcur->pc_Value, '\n');
			while(pz)
			{
				if(pz[-1] != '\r')
				{
					plast = pcur->pc_Value;
					pcur->pc_Value = (char *) __realloc__(pcur->pc_Value, sizeof(char) * (len + 2));
					pz = pcur->pc_Value + (pz - plast);
					memmove(pz + 1, pz, strlen(pz) + 1);
					*pz = '\r';
					pz++;
					len++;
				}

				pz = strchr(pz + 1, '\n');
			}
		}

		/* Remove trailing spaces */
		len = strlen(pcur->pc_Value);
		if(pcur->ForceEOLAfter)
		{
			while(pcur->pc_Value[len - 1] == ' ')
			{
				/* leave trailing space if preceded by a '\\' */
				if(len > 1 && pcur->pc_Value[len - 2] == '\\') break;
				pcur->pc_Value[len - 1] = '\0';
				len--;
			}
		}

		/* Write token */
		for(i = 0; i < pcur->AddSpaceBefore; i++)
		{
			fputc(' ', h);
		}

		fwrite(pcur->pc_Value, 1, strlen(pcur->pc_Value), h);
		gi_NumChars += strlen(pcur->pc_Value);

		/* Write EOL after the token */
		if(pcur->pst_Next && pcur->pst_Next->NoIndent) pcur->ForceEOLAfter = (char) pcur->WasEOLAfter;
		if(pcur->pst_Next && !pcur->pst_Next->i_ID) pcur->ForceEOLAfter = (char) Config.EndBlanks;

		for(i = 0; i < pcur->ForceEOLAfter; i++)
		{
			if(!Config.UnixEOL) fputc('\r', h);
			fputc('\n', h);
			gi_NumLines++;
		}

		/* Write spaces after the token */
		if(pcur->pst_Next && pcur->pst_Next->NoIndent) pcur->ForceSpaceAfter = (char) pcur->WasSpaceAfter;

		if(Config.OutTab && (pcur->ForceSpaceAfter > 1))
		{
			if(pcur->ForceSpaceAfter && pcur->pst_Next)
			{
				col = (char) pcur->Col;
				if(!pcur->ForceEOLAfter)
					col = pcur->Col + strlen(pcur->pc_Value);
				else
					col = 0;
				col1 = Tool_ToTab(col);
				if(col1 == col) col1 += Config.TabSize;
				if(col1 - col <= pcur->ForceSpaceAfter)
				{
					if(pcur->ForceSpaceAfter == 3) pcur->ForceSpaceAfter = 3;
					fputc('\t', h);
					gi_NumChars++;
					jj = pcur->ForceSpaceAfter;
					jj -= (col1 - col);
					pcur->ForceSpaceAfter = (char) jj;
					while(pcur->ForceSpaceAfter / Config.TabSize)
					{
						fputc('\t', h);
						gi_NumChars++;
						jj = pcur->ForceSpaceAfter;
						jj -= Config.TabSize;
						pcur->ForceSpaceAfter = (char) jj;
					}
				}

				for(i = 0; i < pcur->ForceSpaceAfter; i++)
				{
					fputc(' ', h);
					gi_NumChars++;
				}
			}
		}
		else
		{
			for(i = 0; i < pcur->ForceSpaceAfter; i++)
			{
				fputc(' ', h);
				gi_NumChars++;
			}
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void WriteFile(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	asz_Temp[_MAX_PATH];
	char	name[_MAX_PATH];
	char	name1[_MAX_PATH];
	char	*pz_Temp;
	FILE	*h;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.CanOut) return;
	if(access(pfile->psz_FileName, 2) && !Config.OutTest)
	{
		if(Config.ReadOnly)
		{
		}
		else
		{
			Warning("Source file is read only, can't write", NULL);
			return;
		}
	}

	/* Save current file with .bak extension */
	strcpy(asz_Temp, pfile->psz_FileName);
	strcat(asz_Temp, ".bak");
	if(access(asz_Temp, 2)) chmod(asz_Temp, S_IWRITE | S_IREAD);
	unlink(asz_Temp);
	if(Config.CanBak)
	{
		if(rename(pfile->psz_FileName, asz_Temp))
		{
			if(access(pfile->psz_FileName, 2)) chmod(pfile->psz_FileName, S_IWRITE | S_IREAD);
			if(rename(pfile->psz_FileName, asz_Temp)) Warning("Can't rename file", pfile->psz_FileName);
			return;
		}

		unlink(pfile->psz_FileName);
	}

	/* Open file to write */
	strcpy(name, pfile->psz_FileName);
	if(Config.OutTest)
	{
		pz_Temp = strrchr(name, '.');
		*name1 = 0;
		if(pz_Temp) strcpy(name1, pz_Temp);
		*pz_Temp = 0;
		strcat(name, "_test");
		strcat(name, name1);
	}
	else
	{
		chmod(name, S_IWRITE);
	}

	h = fopen(name, "wb");
	if(!h)
	{
		Fatal("Can't write to file", name);
	}

	/* Parse all tokens */
	WriteToken(h, pfile, pfile->pst_RootToken);

	/* Close file */
	fflush(h);
	fclose(h);
}
