/*$T \Sources/indent_pp.c GC 1.150 2011-09-22 20:52:18 */


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

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_PPBase(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_PP) Tool_ForceSpaceAfter(pcur, 1);
		if((pcur->DefineName) && !(pcur->MacroName)) Tool_ForceSpaceAfter(pcur, 1);
	}

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Force EOL before a PP token */
		if(pcur->i_ID == TOKEN_PP)
		{
			Tool_ForceEOLBefore(pcur, 1);
			if(!Config.PPToCode)
			{
				pcur->StmtLevel = 0;
				pcur->IndentLevel = 0;
			}
		}

		/* Force EOL for the last token of a PP */
		if(pcur->InPP)
		{
			while(pcur && pcur->pst_Next && pcur->pst_Next->InPP && pcur->pst_Next->i_ID != TOKEN_PP)
				pcur = NextToken(pcur);
			Tool_ForceEOLAfter(pcur, 1);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignDefine(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~*/
	token	*pcur, *ptmp;
	int		maxcol;
	int		mode;
	/*~~~~~~~~~~~~~~~~~*/

	ptmp = NULL;
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->DefineCst)
		{
			/* Zap if cst is alone on line */
			if(FirstOnLine(pcur))
			{
				while(pcur && pcur->DefineCst) pcur = NextToken(pcur);
				continue;
			}

			maxcol = 0;
			for(mode = 0; mode < 2; mode++)
			{
				/* Get common column */
				ptmp = pcur;
				while(ptmp && ptmp->StmtLevel == pcur->StmtLevel)
				{
					if(ptmp->DefineCst)
					{
						/* Don't align cause constant is first on line */
						if(FirstOnLine(ptmp))
						{
							while(ptmp && ptmp->DefineCst) ptmp = NextToken(ptmp);
							break;
						}

						if(mode == 0)
						{
							if(PrevToken(ptmp)) ptmp->Col -= PrevToken(ptmp)->ForceSpaceAfter;
							if(ptmp->Col >= maxcol)
							{
								maxcol = ptmp->Col;
								maxcol = Tool_ToTab(maxcol);
								if(maxcol <= ptmp->Col) maxcol += Config.TabSize;
							}
						}
						else
						{
							if(ptmp->Col < maxcol)
							{
								Tool_ForceSpaceBefore(ptmp, maxcol - ptmp->Col);
								Tool_ComputeColLine(Tool_ToBegLine(ptmp));
							}
						}

						/* Zap constant */
						while(ptmp && ptmp->DefineCst) ptmp = NextToken(ptmp);
					}

					ptmp = NextToken(ptmp);
				}
			}

			pcur = ptmp;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_PPBreak(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->MacroName)
		{
			pnext = pcur;
			while(NextToken(pnext) && NextToken(pnext)->InPP && NextToken(pnext)->i_ID != TOKEN_PP)
			{
				if(pnext->ForceEOLAfter)
				{
					pnext = Tool_NextValid(pcur);
					pnext = Tool_ToRelationNext(pnext);
					Tool_ForceEOLAfterComments(pnext, 1);
					break;
				}

				pnext = NextToken(pnext);
			}

			pcur = pnext;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_PPDivers(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *ptmp, *pend;
	int		ii, jj;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* #endif */
		if(pcur->In_PP_EndIf || pcur->In_PP_Else)
		{
			/* Comment next the endif */
			pcur->ForceSpaceAfter = 1;
		}

		/* #define */
		if(pcur->In_PP_Define && !pcur->MacroName)
		{
		}

		/* #if */
		if(pcur->In_PP_If)
		{
		}

		if(pcur->In_PP_Include)
		{
			/* Delete spaces in include path */
			if(pcur->i_ID == TOKEN_LESS)
			{
				pend = Tool_ToRelationNext(pcur);
				ptmp = pcur;
				while(ptmp != pend)
				{
					if(ptmp->i_ID == TOKEN_BREAKLINE)
					{
						ptmp->ForceSpaceAfter = 0;
						ptmp->pst_Prev->ForceSpaceAfter = 0;
					}

					if(ptmp->i_ID == TOKEN_OPER)
					{
						ptmp->ForceSpaceAfter = 0;
						ptmp->pst_Prev->ForceSpaceAfter = 0;
					}

					ptmp = ptmp->pst_Next;
				}

				ii = pcur->ForceSpaceAfter;
				pcur->ForceSpaceAfter = 0;
				pcur = Tool_ToRelationNext(pcur);
				if(!pcur) Syntaxe(pcur->line, pcur->column);
				pcur = pcur->pst_Prev;
				ii += pcur->ForceSpaceAfter;
				pcur->ForceSpaceAfter = 0;
				pcur = pcur->pst_Next;
				if(pcur->ForceSpaceAfter)
				{
					jj = ii;
					jj += pcur->ForceSpaceAfter;
					pcur->ForceSpaceAfter = (char) jj;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_PP(FileDes *pfile)
{
	Indent_PPDivers(pfile);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_PPStyle(FileDes *pfile)
{
	/*~~~~~~~~~~~~*/
	token	*pcur;
	int		i_Level;
	/*~~~~~~~~~~~~*/

	if(!Config.PPStyle) return;

	/* Detect PP level */
	i_Level = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pcur->PPLevel = i_Level;
		if(pcur->i_ID == TOKEN_PP)
		{
			if
			(
				(pcur->i_SubID == TOKEN_PP_IF)
			||	(pcur->i_SubID == TOKEN_PP_IFDEF)
			||	(pcur->i_SubID == TOKEN_PP_IFNDEF)
			)
			{
				i_Level++;
			}
			else if((pcur->i_SubID == TOKEN_PP_ELSE) || (pcur->i_SubID == TOKEN_PP_ELIF))
			{
				pcur->PPLevel = i_Level - 1;
			}
			else if(pcur->i_SubID == TOKEN_PP_ENDIF)
			{
				i_Level--;
				pcur->PPLevel = i_Level;
			}
		}
	}

	/* Apply level */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_PP)
		{
			if(Config.PPStyle == 1)
				pcur->StmtLevel += pcur->PPLevel;
			else if((Config.PPStyle == 2) && (pcur->PPLevel))
			{
				/*~~~~~~~*/
				char	*p;
				int		i;
				/*~~~~~~~*/

				p = (char *) __malloc__(strlen(pcur->pc_Value) + (pcur->PPLevel * Config.TabSize) + 100);
				p[0] = '#';
				for(i = 0; i < pcur->PPLevel * (Config.TabSize - 1); i++) p[i + 1] = ' ';
				p[i + 1] = 0;
				strcat(p, pcur->pc_Value + 1);
				free(pcur->pc_Value);
				pcur->pc_Value = p;
			}
		}
	}
}
