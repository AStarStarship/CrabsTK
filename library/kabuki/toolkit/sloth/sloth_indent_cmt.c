/*$T \Sources/indent_cmt.c GC 1.150 2011-09-22 20:52:17 */


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

#include "config.h"
#include "in.h"
#include "grammar.h"
#include "lexi.h"
#include "error.h"
#include "tools.h"
#include "indent.h"
#include "debug.h"
#include "os.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ParseOptions(FileDes *pfile)
{
	/*~~~~~~~~~~~~*/
	token	*pcur;
	char	*pc_cmt;
	/*~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT)
		{
			if((pcur->pc_Value[2] == CMTMARK) && (pcur->pc_Value[3] == 'O'))
			{
				pcur->OptionCmt = 1;
				pc_cmt = strdup(pcur->pc_Value + 4);
				pc_cmt[strlen(pc_cmt) - 1] = ' ';
				pc_cmt[strlen(pc_cmt) - 2] = ' ';
				Options(1, &pc_cmt);
				free(pc_cmt);
			}

			if((pcur->pc_Value[2] == CMTMARK) && (pcur->pc_Value[3] == 'U'))
			{
				pcur->UnAlignCmt = 1;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_RemoveEmptyCmt(FileDes *pfile, token *pcur)
{
	/*~~~~~~~~~~~*/
	char	*p;
	token	*pprev;
	/*~~~~~~~~~~~*/

	if(FixedComment(pcur)) return;
	if(pcur->NoIndent) return;
	p = pcur->pc_Value;

	/* Delete comment if len is too short */
	p += 2;
	while(_isspace(*p)) p++;
	if(p[0] != '*' || p[1] != '/') return;
	if(!pcur->EmptyCmt)
	{
		/* Delete comment */
		if(pcur->WasEOLAfter)
		{
			pprev = PrevToken(pcur);
			if(pprev) pprev->WasEOLAfter = pcur->WasEOLAfter;
		}

		Tool_UnlinkToken(pfile, pcur);
	}
	else
	{
		if(WasFirstOnLine(pcur) && pcur->pst_Next && (!pcur->pst_Next->WasSpaceAfter))
		{
			pcur->ForceSpaceAfter = (char) ((pcur->StmtLevel * Config.TabSize) - 4);
			pcur->StmtLevel = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SplitCmtToIndent(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pmem;
	char	*p;
	char	memoeol;
	/*~~~~~~~~~~~~~~~~~*/

	Tool_ComputeCol(pfile);
	memoeol = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT)
		{
			if(Config.CToCpp) pcur->CppComment = 1;

			p = pcur->pc_Value;
			if(FixedComment(pcur) && !pcur->CppComment) continue;

			pmem = pcur;
			memoeol = pcur->ForceEOLAfter;
			for(;;)
			{
				p = pcur->pc_Value;
				while(*p && *p != '\n') p++;
				if(!*p) break;
				p++;
				if(!*p) break;
				pcur->SplitCmt = 1;
				Tool_InsertTokenAfter(pfile, pcur, TOKEN_CCMT);
				pcur->pst_Next->pc_Value = (char *) __malloc__(strlen(pcur->pc_Value) + 1);
				pcur->pst_Next->InPP = pcur->InPP;
				pcur->pst_Next->SplitCmt = 0;
				pcur->pst_Next->CppComment = pcur->CppComment;
				if(LastOnLine(pmem) && !FirstOnLine(pmem)) pcur->pst_Next->Splitted = 1;
				strcpy(pcur->pst_Next->pc_Value, p);

				/* Delete '\n' and replace with a ForceEOLAfter */
				p[-1] = 0;
				pcur->ForceEOLAfter = 1;
				pcur->ForceSpaceAfter = 0;
				pcur = NextToken(pcur);
			}

			if(!memoeol && pcur->CppComment && pcur->WasEOLAfter && Config.CmtKeepCpp) memoeol = 1;
			pcur->ForceEOLAfter = memoeol;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ConvertCToCpp(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	int		len;
	char	*p;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID != TOKEN_CCMT) continue;
		if(pcur->NoIndent) continue;
		if(!pcur->CppComment) continue;

		/* C++ comment with a \n that has been removed */
		if(FixedComment(pcur))
		{
			if(!pcur->ForceEOLAfter) pcur->ForceEOLAfter = 1;
			continue;
		}

		if(!pcur->ForceEOLAfter) continue;
		if(pcur->WizardCmt || Config.CmtKeepCpp)
		{
			len = strlen(pcur->pc_Value);
			if(pcur->pc_Value[len - 1] == '/' && pcur->pc_Value[len - 2] == '*')
			{
				pcur->pc_Value[len - 1] = ' ';
				pcur->pc_Value[len - 2] = ' ';
			}

			if(pcur->pc_Value[0] == '/' && pcur->pc_Value[1] == '*')
			{
				pcur->pc_Value[1] = '/';
			}
			else
			{
				p = (char *) __malloc__(strlen(pcur->pc_Value) + 3);
				p[0] = '/';
				p[1] = '/';
				strcpy(p + 2, pcur->pc_Value);
				free(pcur->pc_Value);
				pcur->pc_Value = p;
			}

			if(pcur->Splitted)
			{
				pcur->pc_Value[2] = '/';
			}

			if(Config.CppToCKeepEOL && !Config.CmtKeepCpp)
			{
				if(pcur->pc_Value[strlen(pcur->pc_Value) - 1] == Config.CharSplit)
					pcur->pc_Value[strlen(pcur->pc_Value) - 1] = 0;
			}

			if(!Config.SpaceCmtCpp && pcur->CppComment && pcur->pc_Value[2] == ' ')
			{
				len = strlen(pcur->pc_Value);
				memmove(pcur->pc_Value + 2, pcur->pc_Value + 3, len - 3);
				pcur->pc_Value[len - 1] = 0;
			}
		}
	}
}

/*$4
 ***********************************************************************************************************************
 ***********************************************************************************************************************
 */

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_CmtBase(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~*/

	/* Test all empty comments */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT)
		{
			if(pcur->pc_Value[2] == '*' && pcur->pc_Value[3] == '/') pcur->EmptyCmt = 1;
		}
	}

	/* UnSplit all comments */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if((pcur->i_ID == TOKEN_CCMT) && (!pcur->EmptyCmt))
		{
			Tool_UnSplitCmt(pcur);
		}
	}

	/* Remove empty comments */
	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->i_ID == TOKEN_CCMT)
		{
			pnext = pcur->pst_Next;
			Indent_RemoveEmptyCmt(pfile, pcur);
			pcur = pnext;
			continue;
		}

		pcur = NextToken(pcur);
	}

	/* Adjust spacing around comments */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT && !pcur->InParen)
		{
			/* Comment is the last on a line - don't add a blank line afterwards... */
			if(WasLastOnLine(pcur))
			{
				pcur->ForceEOLAfter = 1;
			}

			/* Comment is the first thing on a line - add a blank line afterwards... */
			if(WasFirstOnLine(pcur))
			{
				pcur->ForceEOLAfter = (char) (1 + Config.EOLAfterCmt);
			}

			/*
			 * If a comment part of a block of single line comments, close up any ;
			 * spaces in between them...
			 */
			if(WasFirstOnLine(pcur))
			{
				pnext = PrevToken(pcur);
				if(pnext)
				{
					if(pnext->i_ID == TOKEN_CCMT)
					{
						if(WasFirstOnLine(pnext))
						{
							pnext->ForceEOLAfter = 1;
						}
					}
				}
			}

			/*
			 * If a comment is on its own - and there's some code on the ;
			 * previous line - add a blank line...
			 */
			if(WasFirstOnLine(pcur))
			{
				pnext = PrevToken(pcur);
				if(pnext)
				{
					if(pnext->i_ID != TOKEN_CCMT)
					{
						pnext->ForceEOLAfter = (char) (1 + Config.EOLBeforeCmt);
					}
				}
			}
		}
	}

	/* Concat all first line comments */
	if(Config.ConcatFirstLineCmt)
	{
		for(pcur = pfile->pst_RootToken; pcur;)
		{
			if(pcur->NoIndent)
			{
				pcur = NextToken(pcur);
				continue;
			}

			if((pcur->i_ID == TOKEN_CCMT) && (!pcur->EmptyCmt) && (!pcur->doxygen))
			{
				if(FixedComment(pcur)) goto zap;
				pnext = pcur->pst_Next;
				if(pnext->i_ID == TOKEN_CCMT)
				{
					if(FixedComment(pcur)) goto zap;
					if(FixedComment(pcur->pst_Next)) goto zap;
					if(pcur->CmtLevel) goto zap;
					if(pcur->pst_Next->CmtLevel) goto zap;
					if(pcur->pst_Next->doxygen) goto zap;
					if(!FirstOnLine(pcur))
					{
						if(!pcur->pst_Prev || pcur->pst_Prev->i_ID != TOKEN_LBRACE) goto zap;
						if(!WasFirstOnLine(pcur->pst_Prev)) goto zap;
					}

					if(!LastOnLine(pcur)) goto zap;
					if(!FirstOnLine(pcur->pst_Next) || !LastOnLine(pcur->pst_Next)) goto zap;
					if(pcur->pc_Value[2] == CMTMARK) goto zap;
					if(pcur->pst_Next->pc_Value[2] == CMTMARK) goto zap;
					Tool_ConcatCmt(pfile, pcur);
					Tool_UnSplitCmt(pcur);
					continue;
				}
			}

zap:
			pcur = NextToken(pcur);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AddEmptyCmt(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pprev, *plast;
	struct token_	*pnext;
	int				cpt;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		/* Add empty comment at the beginning of file : $T */
		if(!pcur->pst_Prev)
		{
			if(Config.TagFile)
			{
				if((pcur->i_ID != TOKEN_CCMT) || (pcur->i_SubID != TOKEN_W_CMT_GC_TAG))
				{
					pcur = Tool_AddEmpty(pfile, pcur, "/*$T */\n", 0, TOKEN_W_CMT_GC_TAG);
					continue;
				}
			}
			else if((pcur->i_ID == TOKEN_CCMT) && (pcur->i_SubID == TOKEN_W_CMT_GC_TAG))
			{
				pcur = Tool_UnlinkToken(pfile, pcur);
			}
		}

		/* Add file mark at the beginning of file */
		cpt = 0;
		if
		(
			(!pcur->pst_Prev)
		||	((pcur->pst_Prev->i_ID == TOKEN_CCMT) && (pcur->pst_Prev->i_SubID == TOKEN_W_CMT_GC_TAG))
		)
		{
			pprev = pcur->pst_Prev;
			while(pprev)
			{
				if(!pprev->OptionCmt && !pprev->UnAlignCmt) cpt++;
				pprev = pprev->pst_Prev;
			}

			if(cpt <= 1)
			{
				if(pcur->i_SubID != TOKEN_W_CMT_GC_TAG)
				{
					if((pcur->i_ID != TOKEN_CCMT) || (pcur->i_SubID != TOKEN_W_CMT_FILE))
					{
						if(Config.FileCmt)
						{
							if(Config.FileCmtStyle == 0)
								pcur = Tool_AddEmpty(pfile, pcur, "/*$6*/\n", 6, TOKEN_W_CMT_FILE);
							else
								pcur = Tool_AddEmpty(pfile, pcur, "/*$I0*/\n", 6, TOKEN_W_CMT_FILE);
							continue;
						}
					}
				}
			}
		}

		/* Add empty comments after uncommented functions parameters that aren't void */
		if(Config.TrailingCmtStyle && pcur->IsFctDefName)
		{
			pcur = NextToken(pcur);
			plast = Tool_ToRelationNext(pcur);
			if((plast && (plast != pcur->pst_Next)) && (strcmp(plast->pst_Prev->pc_Value, "void") != 0))
			{
				while(pcur != plast)
				{
					if(pcur->i_ID == TOKEN_COMMA) pcur = Tool_AddEmptyCmtAfterIfMissing(pfile, pcur);
					pcur = NextToken(pcur);
				}

				pcur = Tool_AddEmptyCmtAfterIfMissing(pfile, pcur);
			}
		}

		/* Add empty comment before uncommented functions */
		if(!pcur->InClass || Config.AddCmtFctClass)
		{
			if(Config.AddCmtFct && pcur->InFctDef && !pcur->AlignBody)
			{
				pprev = PrevToken(pcur);
				if(!pprev || (pprev && pcur->InFctDef && !pprev->InFctDef))
				{
					if
					(
						!pprev
					||	((pprev->i_ID != TOKEN_CCMT) && (pprev->i_ID != TOKEN_CPPCMT))
					||	(pcur->pst_Prev->i_SubID == TOKEN_W_CMT_GC_TAG)
					||	(pcur->pst_Prev->i_SubID == TOKEN_W_CMT_FILE)
					||	(pcur->pst_Prev->i_SubID == TOKEN_W_CMT_OPTION)
					||	(pprev->OptionCmt)
					||	(pprev->UnAlignCmt)
					||	(pprev->NoIndent)
					||	(!FirstOnLine(pprev))
					||	((pprev->CmtLevel != 0) && (pprev->CmtLevel != Config.AutoSepFctDef))
					)
					{
						pcur = Tool_AddEmpty(pfile, pcur, gz_FixmeComment, Config.AutoSepFctDef, TOKEN_W_CMT_FUNC);
						continue;
					}
				}
			}
		}

		/* Add comment before class access */
		if((pcur->i_SubID == TOKEN_W_DECL) && (pcur->i_SubSubID == TOKEN_WW_CLASSACCESS) && (pcur->InClass))
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_COLON && Config.AddCmtClassAccess && pcur->StmtLevel == 0)
			{
				pprev = PrevToken(pcur);
				while(pprev && pprev->In_PP_If) pprev = pprev->pst_Prev;
				if
				(
					!pprev
				||	((pprev->i_ID != TOKEN_CCMT) && (pprev->i_ID != TOKEN_CPPCMT))
				||	(pcur->pst_Prev->i_SubID == TOKEN_W_CMT_GC_TAG)
				||	(pcur->pst_Prev->i_SubID == TOKEN_W_CMT_FILE)
				||	(pprev->OptionCmt)
				||	(pprev->UnAlignCmt)
				||	(pprev->NoIndent)
				||	(!FirstOnLine(pprev))
				||	((pprev->CmtLevel != 0) && (pprev->CmtLevel != Config.AutoSepClassAccess))
				)
				{
					if(!pcur->InWizardCmt)
					{
						pprev = Tool_AddEmpty
							(
								pfile,
								pprev->pst_Next,
								gz_FixmeComment,
								Config.AutoSepClassAccess,
								TOKEN_W_CMT_CLASS
							);
						pprev->CmtAccess = 1;
						continue;
					}
				}
			}
		}

		continue;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
char Indent_CharByLevel(int _i_Level)
{
	/*~~~~~~~~*/
	char	sep;
	/*~~~~~~~~*/

	switch(_i_Level)
	{
	case 1:		sep = (char) Config.CharCmt1; break;
	case 2:		sep = (char) Config.CharCmt2; break;
	case 3:		sep = (char) Config.CharCmt3; break;
	case 4:		sep = (char) Config.CharCmt4; break;
	case 5:		sep = (char) Config.CharCmt5; break;
	default:	sep = (char) Config.CharCmt6;
	}

	return sep;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void ChangeTrailingCmtToStyle2(token *pcur)
{
	if(pcur->Splitted) return;
	if(pcur->pc_Value[3] != '<')
	{
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
		/* Insert a '/<' plus the 0 so add 3 */
		char	*p = malloc(strlen(pcur->pc_Value) + 3);
		/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

		strcpy(p, "///<");
		strcat(p, &pcur->pc_Value[2]);
		free(pcur->pc_Value);
		pcur->pc_Value = p;
	}
	else
	{
		pcur->pc_Value[1] = '/';
		pcur->pc_Value[2] = '/';
	}

	if(strstr(&pcur->pc_Value[strlen(pcur->pc_Value) - 2], "*/")) pcur->pc_Value[strlen(pcur->pc_Value) - 2] = 0;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_CmtFctDefParms(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		while(pcur->InFctDef || pcur->InFctDecl)
		{
			if(((pcur->i_ID == TOKEN_CCMT) || (pcur->i_ID == TOKEN_CPPCMT)) && (Config.TrailingCmtStyle == 2))
			{
				ChangeTrailingCmtToStyle2(pcur);
			}

			pcur = NextToken(pcur);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_FixupTrailingComments(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~*/
	token	*pcur;
	int		AddLessThan;
	/*~~~~~~~~~~~~~~~~*/

	AddLessThan = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(((pcur->i_ID == TOKEN_CCMT) || (pcur->i_ID == TOKEN_CPPCMT)) && (Config.TrailingCmtStyle == 2))
		{
			if((pcur->pc_Value[2] == ' ') && (pcur->pc_Value[3] == '<'))
			{
				ChangeTrailingCmtToStyle2(pcur);
				AddLessThan = pcur->SplitCmt;
			}
			else if(AddLessThan)
			{
				ChangeTrailingCmtToStyle2(pcur);
				AddLessThan = pcur->SplitCmt && pcur->Splitted;
			}
		}
		else
		{
			AddLessThan = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_CmtCommentFirstLine(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pnext1;
	char	sep;
	int		i_FileInsert, i_Size, level;
	FILE	*file;
	char	*pc_Buf;
	char	ccan;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	sep = 0;
	level = 0;
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->CmtIndent) continue;
		if(pcur->i_ID != TOKEN_CCMT) continue;
		if(!FirstOnLine(pcur)) continue;

		if((pcur->pc_Value[2] == CMTMARK) && (pcur->pc_Value[3] == 'I'))
		{
			if(gi_NumFileInsert)
			{
				sscanf(pcur->pc_Value + 4, "%d", &i_FileInsert);
				if(i_FileInsert >= gi_NumFileInsert) i_FileInsert = gi_NumFileInsert - 1;
				file = fopen(gapsz_FileInsert[0], "rt");
				if(file)
				{
					rewind(file);
					fseek(file, 0, SEEK_END);
					i_Size = ftell(file);
					i_Size += 10;
					pc_Buf = (char *) __malloc__(i_Size);
					rewind(file);
					i_Size = fread(pc_Buf + 6, 1, i_Size, file);
					fclose(file);
					pc_Buf[0] = '/';
					pc_Buf[1] = '*';
					pc_Buf[2] = CMTMARK;
					pc_Buf[3] = 'I';
					pc_Buf[4] = pcur->pc_Value[4];
					pc_Buf[5] = ' ';
					pc_Buf[i_Size + 6] = ' ';
					pc_Buf[i_Size + 7] = '*';
					pc_Buf[i_Size + 8] = '/';
					pc_Buf[i_Size + 9] = 0;
					free(pcur->pc_Value);
					pcur->pc_Value = pc_Buf;
					continue;
				}
			}
		}

		if((pcur->pc_Value[2] == CMTMARK) && (pcur->pc_Value[3] == 'T'))
		{
			pcur->CmtIndent = 1;
			Tool_AddTag(pfile, pcur);
			continue;
		}

		/* File comment */
		if(pcur->CmtLevel == 6)
		{
			pcur->CmtIndent = 1;
			Tool_SplitCmtFct(pcur, Config.CharCmt6);
			Tool_ForceEmptyLineAfter(pcur);
			continue;
		}

		/* User request cmt */
		if(Tool_pLevelComment(pcur))
		{
			sep = Indent_CharByLevel(pcur->pc_Value[3] - '0');
			if(pcur->pc_Value[4] != '-')
			{
				pcur->CmtLevel = (char) (pcur->pc_Value[3] - '0');
				pcur->CmtIndent = 1;
				pcur->StmtLevel = pcur->pst_Next->StmtLevel;
				pcur->IndentLevel = pcur->pst_Next->IndentLevel;
				Tool_SplitCmtFct(pcur, sep);
				Tool_InCmt(pcur);
				Tool_ForceEOLBefore(pcur, 1);
				Tool_ForceEmptyLineBefore(pcur);
				Tool_ForceEOLAfter(pcur, 1);
				Tool_ForceEmptyLineAfter(pcur);
				continue;
			}
			else
			{
				Tool_InsertSingleSep(pfile, pcur, sep);
				continue;
			}
		}

		/* Struct */
		pnext = Tool_NextValid(pcur);
		pnext1 = Tool_NextValid(pnext);
		if(Config.AutoSepStruct)
		{
			if
			(
				(!pcur->OptionCmt)
			&&	(!pcur->UnAlignCmt)
			&&	(!pcur->CmtLevel)
			&&	(!pcur->StmtLevel)
			&&	(
					((pnext->i_SubSubID == TOKEN_WW_TYPEDEF) && (pnext1->i_SubSubID == TOKEN_WW_STRUCT))
				||	(pnext->i_SubSubID == TOKEN_WW_STRUCT)
				)
			)
			{
				pcur->CmtIndent = 1;
				pcur->CmtLevel = 1;
				pcur->StmtLevel = pcur->pst_Next->StmtLevel;
				pcur->IndentLevel = pcur->pst_Next->IndentLevel;
				Tool_SplitCmtFct(pcur, Indent_CharByLevel(Config.AutoSepStruct));
				Tool_ForceEOLBefore(pcur, 1);
				Tool_ForceEOLAfter(pcur, 1);
				Tool_ForceEmptyLineBefore(pcur);
				continue;
			}
		}

		/* Class */
		pnext = pcur->pst_Next;
		while(pnext->IsType) pnext = pnext->pst_Next;
		if(pnext && pnext->i_ID == TOKEN_LESS)
		{
			pnext = Tool_ToRelationNext(pnext);
			if(pnext) pnext = pnext->pst_Next;
		}

		if(Config.AutoSepClass && pnext)
		{
			if
			(
				(!pcur->OptionCmt)
			&&	(!pcur->UnAlignCmt)
			&&	(!pcur->CmtLevel)
			&&	(!pcur->StmtLevel)
			&&	(!pcur->InClass && pnext->ClassDecl)
			)
			{
				pcur->CmtIndent = 1;
				pcur->CmtLevel = 1;
				pcur->StmtLevel = pcur->pst_Next->StmtLevel;
				pcur->IndentLevel = pcur->pst_Next->IndentLevel;
				Tool_SplitCmtFct(pcur, Indent_CharByLevel(Config.AutoSepClass));
				Tool_ForceEOLBefore(pcur, 1);
				Tool_ForceEOLAfter(pcur, 1);
				Tool_ForceEmptyLineBefore(pcur);
				continue;
			}
		}

		/* Function cmt */
		pnext = NextToken(pcur);
		ccan = 0;
		if(pnext && (!pcur->OptionCmt) && (!pcur->UnAlignCmt) && (!pnext->EmptyCmtBefore))
		{
			if((pnext->InFctDef) && (Config.AutoSepFctDef) && !pnext->AlignBody)
			{
				level = Config.AutoSepFctDef;
				sep = Indent_CharByLevel(Config.AutoSepFctDef);
				ccan = 1;
			}

			if((pnext->InFctDecl) && (Config.AutoSepFctDecl) && (!pnext->InFct))
			{
				level = Config.AutoSepFctDecl;
				sep = Indent_CharByLevel(Config.AutoSepFctDecl);
				ccan = 1;
			}

			if(pnext->In_PP_Define && (Tool_NextValid(pnext)->MacroName) && (Config.AutoSepMacro))
			{
				level = Config.AutoSepMacro;
				sep = Indent_CharByLevel(Config.AutoSepMacro);
				ccan = 1;
			}

			if
			(
				(
					(pnext->i_SubID == TOKEN_W_DECL)
				&&	(pnext->i_SubSubID == TOKEN_WW_CLASSACCESS)
				&&	(Config.AutoSepClassAccess)
				)
			||	(pcur->CmtAccess)
			)
			{
				level = Config.AutoSepClassAccess;
				sep = Indent_CharByLevel(Config.AutoSepClassAccess);
				ccan = 1;
			}

			if(pcur->WizardCmt) ccan = 0;

			if(ccan)
			{
				pcur->CmtIndent = 1;
				pcur->StmtLevel = pcur->pst_Next->StmtLevel;
				pcur->IndentLevel = pcur->pst_Next->IndentLevel;
				Tool_SplitCmtFct(pcur, sep);
				pcur->CmtLevel = (char) level;
				Tool_InCmt(pcur);
				Tool_ForceEOLBefore(pcur, 1);
				Tool_ForceEOLAfter(pcur, 1);
				Tool_ForceEmptyLineBefore(pcur);
				pcur = NextToken(pcur);

				/* skip to the end of the function */
				while(pcur->InFctDef || pcur->InFctDecl) pcur = NextToken(pcur);
				continue;
			}
		}

		if(pcur->doxygen)
		{
			Tool_SplitCmtFct(pcur, 0);
		}

		/* Split comment without mark */
		pnext = NextToken(pcur);
		if
		(
			(((int) strlen(pcur->pc_Value) - 6) > Config.LineLenCmtFirstLine)
		||	(((int) strlen(pcur->pc_Value) + pcur->Col) > Config.LineLenCmt)
		||	(ForceSplitComment(pcur))
		||	(pcur->NeedSplit)	/* || (Config.JavaDoc) */
		)
		{
			pcur->CmtIndent = 1;
			Tool_SplitCmtFirstLine(pcur);
			continue;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_CmtCommentAddDeclStmt(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *ptmp, *pnext;
	int		i_Len;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.CmtDeclStmt) return;
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->StmtLevel > Config.CmtDeclMaxLevel) return;
		if
		(
			(pcur->BegStmtDecl)
		&&	(pcur->InFct)
		&&	(pcur->pst_Prev)
		&&	(
				(
					!pcur->pst_Prev->EndStmtDecl
				||	pcur->pst_Prev->StmtLevel == 0
				||	(pcur->InClass && pcur->pst_Prev->StmtLevel == 1)
				)
			)
		)
		{
			/* Compute max len */
			i_Len = Config.LineLenCmtSepDecl;
			if(Config.CmtSepDeclAutoLen)
			{
				ptmp = pcur;
				i_Len = 0;
				while(ptmp && !ptmp->EndStmtDecl)
				{
					if(ptmp->Col > i_Len) i_Len = ptmp->Col;
					ptmp = NextToken(ptmp);
					if(ptmp && ptmp->i_ID == TOKEN_LBRACE)
					{
						ptmp = Tool_ToRelationNext(ptmp);
						ptmp = NextToken(ptmp);
					}
				}

				if(ptmp && (ptmp->Col) > i_Len) i_Len = ptmp->Col;
				i_Len++;
				i_Len += Config.CmtSepDeclAutoLenAdd;
			}

			/* Insert separator before */
			ptmp = PrevToken(pcur);
			while(ptmp->i_ID == TOKEN_CCMT) ptmp = PrevToken(ptmp);
			if(!pcur->InPP && ptmp->InPP)
			{
				if(!ptmp->In_PP_EndIf && !ptmp->In_PP_Else)
				{
					while(ptmp->InPP) ptmp = PrevToken(ptmp);
				}
			}

			/* A separator before ? */
			if(Config.CmtSepDeclBefore)
			{
				Tool_InsertSepAfter(pfile, ptmp, pcur, Config.CharCmt1, i_Len);
				Tool_ForceEmptyLineBefore(ptmp->pst_Next);
				ptmp->pst_Next->BeginDecl = 0;
				ptmp->pst_Next->ForceEOLAfter = 1;
				ptmp->pst_Next->CmtIndent = 1;
				ptmp->pst_Next->InDeclStmt = 1;
				if(!ptmp->pst_Next->pst_Next->InPP) ptmp->pst_Next->InPP = 0;
			}

			/* Insert separator after */
			ptmp = pcur;
			while(ptmp && !ptmp->EndStmtDecl)
			{
				if(ptmp->ForceEOLAfter >= 1) ptmp->InDeclStmt = 1;
				ptmp = NextToken(ptmp);
				if(ptmp && ptmp->i_ID == TOKEN_LBRACE)
				{
					ptmp = Tool_ToRelationNext(ptmp);
					ptmp = NextToken(ptmp);
				}
			}

			if(ptmp)
			{
				if(ptmp->NoIndent)
				{
					ptmp = NextToken(ptmp);
					if(ptmp->i_ID == TOKEN_CCMT)
					{
						ptmp = NextToken(ptmp);
						if(!ptmp->NoIndent)
						{
							ptmp = PrevToken(ptmp);
							goto ok;
						}
					}

					pcur = ptmp;
					continue;
				}

ok:
				while(!ptmp->ForceEOLAfter) ptmp = NextToken(ptmp);
				pnext = Tool_NextValid(ptmp);
				if((pnext->i_ID == TOKEN_PP) && (pnext->i_SubID == TOKEN_PP_ENDIF)) ptmp = pnext;
				Tool_InsertSepAfter(pfile, ptmp, pcur, Config.CharCmt1, i_Len);
				ptmp->ForceEOLAfter = 1;
				ptmp->pst_Next->EndDecl = 0;
				ptmp->pst_Next->CmtIndent = 1;
				ptmp->pst_Next->InPP = 0;
				Tool_ForceEOLAfter(ptmp->pst_Next, 1);
				Tool_ForceEmptyLineAfter(ptmp->pst_Next);
				pcur = ptmp;
				continue;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_CommentLastLine(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*ptmp;
	token	*ptmp1, *ptmp2;
	token	*pdebi, *pdeb, *pnewend;
	int		mincol, max1, maxcol;
	int		mode, nextcol;
	char	cmt2;
	char	*pz;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pdebi = pfile->pst_RootToken;
recom:
	Tool_ComputeCol(pfile);
	pdeb = pdebi;
	pdebi = NULL;
	pnewend = NULL;
	maxcol = 0;
	mincol = 0;
	ptmp = NULL;
	for(pcur = pdeb; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if((pcur->i_ID == TOKEN_CCMT) && !FirstOnLine(pcur) && LastOnLine(pcur))
		{
			maxcol = 0;
			for(mode = 0; mode < 2; mode++)
			{
				/* Get common column */
				ptmp = pcur;
				while(ptmp && (ptmp->CmtLevel == 0) && (!ptmp->UnAlignCmt))
				{
					ptmp1 = ptmp;
					if((ptmp->i_ID == TOKEN_CCMT) && !FirstOnLine(ptmp) && LastOnLine(ptmp) && !ptmp->NoIndent)
					{
						ptmp2 = Tool_PrevValid(ptmp);
						if((ptmp2 && ptmp2->In_PP_EndIf) || (ptmp2 && ptmp2->In_PP_Else))
						{
							ptmp = ptmp->pst_Next;
							continue;
						}

						cmt2 = 0;
						while(ptmp->pst_Prev && (ptmp->pst_Prev->i_ID == TOKEN_CCMT) && !LastOnLine(ptmp->pst_Prev))
						{
							cmt2 = 1;
							ptmp = ptmp->pst_Prev;
						}

						if(ptmp == pnewend)
						{
							if(mode == 1) goto recom;
							break;
						}

						/* See if this is the end of the alignment set. */
						if
						(
							((pcur->InFctDef && !ptmp->InFctDef) || (!pcur->InFctDef && ptmp->InFctDef))
						&&	(
								(pcur->InFct && !ptmp->InFct)
							||	(!pcur->InFct && ptmp->InFct)
							||	(pcur->InParen && !ptmp->InParen)
							||	(!pcur->InParen && ptmp->InParen)
							)
						)
						{
							pnewend = ptmp2;
							pdeb = pdebi = ptmp;
							if(mode == 1) goto recom;
							break;
						}

						if(cmt2) ptmp->NoCmt2 = 1;
						if(mode == 0)
						{
							nextcol = ptmp->Col;
							if(PrevToken(ptmp)) nextcol -= PrevToken(ptmp)->ForceSpaceAfter;
							if(nextcol >= maxcol)
							{
								max1 = nextcol;
								max1 = Tool_ToTab(max1);
								if(max1 <= nextcol) max1 += Config.TabSize;
								if(mincol && (max1 - mincol > Config.MaxAlignCmt))
								{
									pnewend = ptmp;
									pdebi = ptmp;
									break;
								}

								maxcol = max1;
								if(!mincol || (maxcol < mincol)) mincol = maxcol;
							}
							else
							{
								max1 = nextcol;
								max1 = Tool_ToTab(max1);
								if(max1 <= nextcol) max1 += Config.TabSize;
								if(maxcol && ((maxcol - max1) > Config.MaxAlignCmt))
								{
									pnewend = ptmp;
									pdebi = ptmp;
									break;
								}

								if(!mincol || (max1 < mincol)) mincol = maxcol;
							}
						}
						else
						{
							nextcol = ptmp->Col;
							if(PrevToken(ptmp)) nextcol -= PrevToken(ptmp)->ForceSpaceAfter;
							if(nextcol < maxcol)
							{
								Tool_ForceSpaceBefore(ptmp, maxcol - nextcol);
							}
						}
					}

					ptmp = NextToken(ptmp1);
				}
			}

			pcur = ptmp;
		}
	}

	if(pdebi)
	{
		goto recom;
	}

	/* Split last line comments if too long */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if((pcur->i_ID == TOKEN_CCMT) && !FirstOnLine(pcur) && LastOnLine(pcur))
		{
			pcur->CmtIndent = 1;
			pz = strchr(pcur->pc_Value, '\n');
			if(pz) Tool_UnSplitCmt(pcur);
			if((pcur->NeedSplit) || (pcur->Col + (int) strlen(pcur->pc_Value) > Config.LineLenCode + 3))
			{
				Tool_SplitEndLineCmt(pcur);
				pcur->NeedSplit = 0;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SeparateComments(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		/* Comment in a middle of a line */
		if(pcur->i_ID == TOKEN_CCMT)
		{
			if(!FirstOnLine(pcur) && !LastOnLine(pcur) && !pcur->NoCmt2)
			{
				if(pcur->pst_Prev) pcur->pst_Prev->ForceSpaceAfter = 1;

				pnext = pcur->pst_Next;
				if(pnext && pnext->i_ID != TOKEN_COMMA)
				{
					pcur->ForceSpaceAfter = 1;
				}
			}
		}

		/* 2 following comments first on line */
		if(pcur->i_ID == TOKEN_CCMT && FirstOnLine(pcur) && !pcur->OptionCmt && !pcur->UnAlignCmt)
		{
			pnext = NextToken(pcur);
			if(pnext && pnext->i_ID == TOKEN_CCMT && FirstOnLine(pnext) && !pcur->WizardCmt)
			{
				if(pcur->CppComment && (Config.CmtKeepCpp || Config.NoCmtIndent || !Config.ConcatFirstLineCmt))
					continue;
				if(Config.FirstLineBlank) Tool_ForceEmptyLineAfter(pcur);
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_EmptyLinesSep(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	token	*pprev;
	int		ibefore, iafter;
	/*~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT && pcur->pc_Value[2] == CMTMARK && isdigit(pcur->pc_Value[3]))
		{
			ibefore = iafter = -1;
			switch(pcur->CmtLevel)
			{
			case 1: ibefore = Config.EOLBeforeCmt1; iafter = Config.EOLAfterCmt1; break;
			case 2: ibefore = Config.EOLBeforeCmt2; iafter = Config.EOLAfterCmt2; break;
			case 3: ibefore = Config.EOLBeforeCmt3; iafter = Config.EOLAfterCmt3; break;
			case 4: ibefore = Config.EOLBeforeCmt4; iafter = Config.EOLAfterCmt4; break;
			case 5: ibefore = Config.EOLBeforeCmt5; iafter = Config.EOLAfterCmt5; break;
			case 6: ibefore = Config.EOLBeforeCmt6; iafter = Config.EOLAfterCmt6; break;
			}

			if(ibefore != -1)
			{
				Tool_ForceExactEOLBefore(pcur, ibefore + 1);
				Tool_ForceExactEOLAfter(pcur, iafter + 1);
			}
		}

		if(pcur->i_ID == TOKEN_CCMT && pcur->CmtLevel == Config.AutoSepFctDef)
		{
			pnext = Tool_NextValid(pcur);
			if(!pnext) continue;
			if(pnext->InFctDef)
			{
				if
				(
					FirstOnLine(pcur)
				&&	pcur->CppComment
				&&	(Config.CmtKeepCpp || Config.NoCmtIndent || !Config.ConcatFirstLineCmt)
				)
				{
					pprev = pcur;
					while(pprev->pst_Prev)
					{
						if
						(
							pprev->pst_Prev->CppComment
						&&	pprev->pst_Prev->WasEOLAfter == 1
						&&	FirstOnLine(pprev->pst_Prev)
						) pprev = pprev->pst_Prev;
						else
							break;
					}

					Tool_ForceEOLBefore(pprev, Config.EolBeforeFctCmt + 1);
				}
				else
				{
					/* do not split trailing comment on pre-processor line */
					for(pprev = pcur; pprev->pst_Prev && pprev->i_ID == TOKEN_CCMT; pprev = pprev->pst_Prev);
					if(pprev->i_ID != TOKEN_PP) Tool_ForceEOLBefore(pcur, Config.EolBeforeFctCmt + 1);
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_Cmt(FileDes *pfile)
{
	Indent_AddEmptyCmt(pfile);
	Indent_CmtCommentFirstLine(pfile);
	Indent_CmtCommentAddDeclStmt(pfile);
	Indent_CommentLastLine(pfile);
	Indent_SeparateComments(pfile);
}
