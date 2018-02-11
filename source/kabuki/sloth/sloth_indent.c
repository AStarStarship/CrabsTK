/*$T \Sources/indent.c GC 1.150 2011-09-22 20:52:17 */


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

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignBreakLine(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~*/
	int		col;
	token	*pcur, *pdeb;
	/*~~~~~~~~~~~~~~~~~*/

	if(!Config.AlignBL) return;

	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_BREAKLINE)
		{
			pdeb = pcur;
			col = pcur->Col;
			while(pcur && pcur->InPP)
			{
				if(pcur->i_ID == TOKEN_BREAKLINE && pcur->Col > col) col = pcur->Col;
				if(!pcur->pst_Next) break;
				if((!pcur->pst_Next->InPP) || (pcur->pst_Next->i_ID == TOKEN_PP)) break;
				pcur = NextToken(pcur);
			}

			pcur = pdeb;
			while(pcur && pcur->InPP)
			{
				if(pcur->i_ID == TOKEN_BREAKLINE) Tool_ForceSpaceBefore(pcur, col - pcur->Col + 1);
				if(!pcur->pst_Next) break;
				if((!pcur->pst_Next->InPP) || (pcur->pst_Next->i_ID == TOKEN_PP)) break;
				pcur = NextToken(pcur);
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ForceBreakLine(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext;
	int		i;
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if((pcur) && (pcur->InPP) && (LastOnLine(pcur)) && (pcur->i_ID != TOKEN_BREAKLINE))
		{
			/* Don't put the last breakline */
			if(!pcur->pst_Next) break;
			if((!pcur->pst_Next->InPP) || (pcur->pst_Next->i_ID == TOKEN_PP)) continue;
			if(pcur->SplitCmt) continue;

			/* Add one breakline per line */
			for(i = 0; i < pcur->ForceEOLAfter; i++)
			{
				Tool_InsertTokenAfter(pfile, pcur, TOKEN_BREAKLINE);
				pnext = pcur->pst_Next;
				pcur->ForceSpaceAfter = 1;
				pnext->ForceEOLAfter = 1;
			}

			/* Reset EOL in PP */
			pcur->ForceEOLAfter = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ForceSplit(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *ptmp;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.CmtKeepCpp) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if((pcur->i_ID == TOKEN_LPAREN) && (pcur->InFctDecl || pcur->InFctDef))
		{
			pnext = Tool_ToRelationNext(pcur);
			if(!pnext) continue;
			ptmp = pcur;
			while(ptmp != pnext)
			{
				if(ptmp->i_ID == TOKEN_CCMT && ptmp->WasEOLAfter)
				{
					pcur->ForceSplit = 1;
					pcur = pnext;
					break;
				}

				ptmp = ptmp->pst_Next;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ForceSplitOfFuncDefDecl(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev;
	/*~~~~~~~~~~~~~~~~~~*/

	if(Config.ForceFctDefDeclSplit)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if((pcur->i_ID == TOKEN_LPAREN) && (pcur->InFctDecl || pcur->InFctDef))
			{
				/* If void parameters don't break */
				if(pcur->pst_Next && (strcmp(pcur->pst_Next->pc_Value, "void") != 0))
				{
					pprev = PrevToken(pcur);
					if(pprev->IsFctDefName)
					{
						pcur->ForceSplit = 1;
					}
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_RemoveExLines(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if((pcur->ForceEOLAfter > 1) && (pcur->ForceEOLAfter - 1 > Config.RemoveEmptyLinesCode))
		{
			pcur->ForceEOLAfter = (char) (Config.RemoveEmptyLinesCode + 1);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_KeepEmptyLines(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	if(!Config.TryHarderToKeepEmptyLines) return;
	if(!Config.KeepEmptyLinesCode) return;

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->WasEOLAfter > 1)
		{
			if(!IsAComment(pcur))
			{
				if(!IsAComment(NextToken(pcur)))
				{
					pcur->ForceEOLAfter = 2;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent(FileDes *pfile)
{
	/*~~*/
	int i;
	/*~~*/

	Indent_ParseOptions(pfile);
	Indent_ForceSplit(pfile);
	Indent_ForceSplitOfFuncDefDecl(pfile);
	Indent_CmtBase(pfile);
	Indent_EOLBase(pfile);
	Indent_SpacesBase(pfile);
	Indent_PPBase(pfile);		/* preprocessor - e.g. include lines... */
	Indent_Spaces(pfile);
	Indent_scast(pfile);
	Indent_Special(pfile);
	Indent_InClassIndent(pfile);
	Indent_EOL(pfile);
	Indent_Cmt(pfile);
	Indent_EOLAddEmptyLines(pfile);
	Indent_PPBase(pfile);		/* preprocessor - e.g. include lines... */
	Indent_PP(pfile);			/* preprocessor - e.g. include lines... */

	for(i = 0; i < 5; i++)
	{
		Indent_RemoveEOL(pfile);
		Indent_Conform(pfile);
	}

	Indent_EOLEmptyStmt(pfile);
	Indent_AlignDefine(pfile);
	Indent_CommentLastLine(pfile);
	Indent_EmptyLinesSep(pfile);
	Indent_AlignOperSpecial(pfile);
	Indent_BraceStyle(pfile);	/* Comment rearrangement wrapping disaster zone... */
	Indent_Switch(pfile);
	Indent_PPStyle(pfile);
	Indent_Return(pfile);
	Indent_CommentLastLine(pfile);

	/* Split comments on more than one token, for each line to be indent */
	Indent_SplitCmtToIndent(pfile);
	Indent_ConvertCToCpp(pfile);

	Indent_KeepEmptyLines(pfile);
	Indent_EOLBetweenEmptyBraces(pfile);
	Indent_EOLAfterEveryCloseBrace(pfile);

	/* Set spaces depending on statement and indent level */
	Indent_StmtIndentLevel(pfile);

	/* Add break lines in pp */
	Indent_ForceBreakLine(pfile);
	Indent_AlignBreakLine(pfile);

	/* Remove extra empty lines */
	Indent_RemoveExLines(pfile);

	/* Handle function definition parameters */
	Indent_CmtFctDefParms(pfile);
	Indent_FixupTrailingComments(pfile);
	Indent_RetrospectiveElseAdjustment(pfile);
}
