/*$T \Sources/grammar_pp.c GC 1.150 2011-09-22 20:52:17 */


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
#include <stdio.h>
#include <string.h>
#ifndef __APPLE__
#include <malloc.h>
#endif
#include "config.h"
#include "lexi.h"
#include "error.h"
#include "tools.h"
#include "grammar.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_InPP(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pnext;
	char	*psz_Tmp;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->i_ID == TOKEN_PP)
		{
			pend = pcur;
			while(pend->i_ID)
			{
				if(pend->WasEOLAfter && (pend->i_ID != TOKEN_BREAKLINE)) break;
				pend = NextToken(pend);
			}

			Tool_SetFlag(pcur, pend, &pcur->InPP, 1);

			/* Real PP directive */
			switch(pcur->i_SubID)
			{
			case TOKEN_PP_PRAGMA:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_Pragma, 1);
				break;

			case TOKEN_PP_ELSE:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_Else, 1);
				break;

			case TOKEN_PP_ENDIF:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_EndIf, 1);
				break;

			case TOKEN_PP_IF:
			case TOKEN_PP_IFDEF:
			case TOKEN_PP_IFNDEF:
			case TOKEN_PP_ELIF:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_If, 1);
				break;

			case TOKEN_PP_INCLUDE:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_Include, 1);
				pnext = Tool_NextValid(pcur);
				if(Config.IncludeLin && pnext->i_ID == TOKEN_STRINGCST)
				{
					psz_Tmp = pnext->pc_Value;
					while(*psz_Tmp)
					{
						if(*psz_Tmp == '\\') *psz_Tmp = '/';
						psz_Tmp++;
					}
				}
				break;

			case TOKEN_PP_DEFINE:
				Tool_SetFlag(pcur, pend, &pcur->In_PP_Define, 1);
				break;
			}

			pcur = pend;
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_DefineCst(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pnext;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->i_ID == TOKEN_PP && pcur->i_SubID == TOKEN_PP_DEFINE)
		{
			pend = Tool_NextValid(pcur);
			pend->DefineName = 1;

			/* Constant */
			if(NextToken(pend)->i_ID == TOKEN_LPAREN)
			{
				if(!pend->WasSpaceAfter)
				{
					pend->MacroName = 1;
					pend = NextToken(pend);
					pend = Tool_ToRelationNext(pend);
				}
			}

			/* Define constant */
			pnext = Tool_NextValid(pend);
			if(pnext->InPP && (pnext->i_ID != TOKEN_PP))
			{
				pnext->DefineCst = 1;
				while(pnext && pnext->InPP && (pnext->i_ID != TOKEN_PP))
				{
					pnext->DefineCst = 1;
					pnext = NextToken(pnext);
				}
			}

			pcur = pend;
		}

		pcur = NextToken(pcur);
	}
}
