/*$T \Sources/grammar.c GC 1.150 2011-09-22 20:52:17 */


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
#include <ctype.h>

#include "config.h"
#include "lexi.h"
#include "error.h"
#include "tools.h"
#include "grammar.h"
#include "debug.h"
#include "os.h"

/*$4
 ***********************************************************************************************************************
 ***********************************************************************************************************************
 */

tdst_Include	gast_Includes[MAX_INCLUDE];
int				gi_NumIncludes = 0;

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_MoveCmt(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pprev, *pnext;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_CCMT)
		{
			/* Specword cmt { */
			pprev = Tool_PrevValid(pcur);
			pnext = Tool_NextValid(pcur);
			if(FirstOnLine(pcur) && (pnext->i_ID == TOKEN_LBRACE))
			{
				if(pprev)
				{
					if(pprev->i_ID == TOKEN_SPECWORD) Tool_MoveTokenBefore(pfile, pcur, pprev);
					if(pprev->i_ID == TOKEN_RPAREN)
					{
						pprev = Tool_ToRelationPrev(pprev);
						pprev = Tool_PrevValid(pprev);
						if(pprev->i_ID == TOKEN_SPECWORD) Tool_MoveTokenBefore(pfile, pcur, pprev);
					}
				}

				continue;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_CmtClassFunc(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev;
	int		cpt;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Set CmtLevel */
		if(pcur->i_ID == TOKEN_CCMT)
		{
			/* be sure subid and subsubid for all comments */
			pcur->i_SubID = 0;
			pcur->i_SubSubID = 0;

			/* Check for function and class comments */
			if(pcur->pst_Next)
			{
				if(!pcur->InFctDef && pcur->pst_Next->InFctDef)
				{
					pcur->i_SubID = TOKEN_W_CMT_FUNC;
				}

				if(!pcur->InClass && pcur->pst_Next->InClass)
				{
					pcur->i_SubID = TOKEN_W_CMT_CLASS;
				}
			}

			/* Check for function parameter comments */
			if(pcur->pst_Prev)
			{
				if(pcur->pst_Prev->InFctDef) pcur->i_SubID = TOKEN_W_CMT_FUNC_PARAM;
			}

			/* GC mark */
			if(!pcur->pst_Prev && pcur->pc_Value[2] == '$' && pcur->pc_Value[3] == 'T')
			{
				pcur->i_SubID = TOKEN_W_CMT_GC_TAG;
				continue;
			}

			/* File comment */
			if
			(
				(!pcur->pst_Prev)
			||	((pcur->pst_Prev->i_ID == TOKEN_CCMT) && (pcur->pst_Prev->i_SubID == TOKEN_W_CMT_GC_TAG))
			)
			{
				cpt = 0;
				pprev = pcur->pst_Prev;
				while(pprev)
				{
					if(!pprev->OptionCmt && !pprev->UnAlignCmt) cpt++;
					pprev = pprev->pst_Prev;
				}

				if(cpt <= 1)
				{
					if(!pcur->pst_Prev || (pcur->pst_Prev->i_SubID == TOKEN_W_CMT_GC_TAG))
						pcur->i_SubID = TOKEN_W_CMT_FILE;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_CmtLevel(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur;
	char			*pchar;
	/*~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Set CmtLevel */
		if(pcur->i_ID == TOKEN_CCMT)
		{
			pchar = Tool_pLevelComment(pcur);
			if(pchar)
			{
				pcur->CmtLevel = (char) (*pchar - '0');
			}

			/* Empty cmt ? */
			if(EmptyComment(pcur))
			{
				if(pcur->pst_Next) pcur->pst_Next->EmptyCmtBefore = 1;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_WhileForDo(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(SpecWord(pcur, TOKEN_W_DO))
		{
			pnext = Tool_NextValid(pcur);
			if(!pnext) return;
			pnext = Tool_ToRelationNext(pnext);
			if(!pnext) return;
			pnext = Tool_NextValid(pnext);
			if(!pnext) return;
			pnext->WhileForDo = 1;
		}
	}
}

/*
 =======================================================================================================================
    Replace all asm comment (;
    ...) by c++ comment. Force EOL at the same location.
 =======================================================================================================================
 */
void Grammar_CommentAsm(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pend, *pmemo, *pnext;
	int		i;
	char	*p;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pcur = pfile->pst_RootToken;
	while(pcur)
	{
		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(!SpecWord(pcur, TOKEN_W_ASM))
		{
			pcur = Tool_NextValid(pcur);
			continue;
		}

		pcur = Tool_NextValid(pcur);

		/* An asm statement block. */
		if(pcur->i_ID == TOKEN_LBRACE)
		{
			pend = Tool_ToRelationNext(pcur);

			/* Set InASM */
			pnext = pcur;
			while(pnext != pend)
			{
				pnext = NextToken(pnext);
				pnext->InASM = 1;
			}

			while(pcur != pend)
			{
				if(pcur->i_ID == TOKEN_SEMICOL)
				{
					pcur->i_ID = TOKEN_CCMT;
					free(pcur->pc_Value);
					pcur->pc_Value = strdup("/* ");

					/* Concat to parameters each following tokens */
					pmemo = pcur;
					while(pcur && pcur != pend && !pcur->WasEOLAfter)
					{
						pcur = NextToken(pcur);
						if(pcur)
						{
							p = pmemo->pc_Value;
							pmemo->pc_Value = (char *) __malloc__(strlen(p) + pmemo->WasSpaceAfter + strlen(pcur->pc_Value) + 1);
							strcpy(pmemo->pc_Value, p);
							for(i = 0; i < pmemo->WasSpaceAfter; i++) strcat(pmemo->pc_Value, " ");
							strcat(pmemo->pc_Value, pcur->pc_Value);
							pmemo->WasSpaceAfter = pcur->WasSpaceAfter;
							if(!pcur || pcur == pend || pcur->WasEOLAfter)
							{
								/* EOL at the end of new cpp comment */
								pmemo->WasEOLAfter = 1;
								if(pcur) pmemo->WasEOLAfter = pcur->WasEOLAfter;

								/* Unlink and quit */
								Tool_UnlinkToken(pfile, pcur);
								pcur = pmemo;
								break;
							}

							Tool_UnlinkToken(pfile, pcur);
							pcur = pmemo;
						}
					}

					/* Add end comment mark */
					p = pmemo->pc_Value;
					pmemo->pc_Value = (char *) __malloc__(strlen(p) + 4);
					strcpy(pmemo->pc_Value, p);
					strcat(pmemo->pc_Value, " */");
					free(p);
					if(pcur == pend) break;
				}

				if(!pcur) return;

				/* Force EOL */
				if(pcur->WasEOLAfter)
				{
					pcur->ForceEOLAfter = 1;
					if(Config.KeepEmptyLinesCode && pcur->WasEOLAfter > 1) pcur->ForceEOLAfter = 2;
				}

				pcur = Tool_NextValid(pcur);
			}
		}
		else
		{
			pnext = pcur;
			while(pcur && !pcur->WasEOLAfter && (pcur->i_ID != TOKEN_SEMICOL)) pcur = NextToken(pcur);
			if(!pcur) return;

			/* Set InASM */
			while(pnext != pcur->pst_Next)
			{
				pnext = NextToken(pnext);
				pnext->InASM = 1;
			}

			/* Force EOL */
			if(pcur && pcur->WasEOLAfter) pcur->ForceEOLAfter = 1;
			if(pcur->i_ID == TOKEN_SEMICOL)
			{
				pcur->i_ID = TOKEN_CCMT;
				free(pcur->pc_Value);
				pcur->pc_Value = strdup("/* ");

				/* Concat to parameters each following tokens */
				pmemo = pcur;
				while(pcur && !pcur->WasEOLAfter)
				{
					pcur = NextToken(pcur);
					if(pcur)
					{
						/*~~~~~~~*/
						char	*p;
						/*~~~~~~~*/

						p = pmemo->pc_Value;
						pmemo->pc_Value = (char *) __malloc__(strlen(p) + pmemo->WasSpaceAfter + strlen(pcur->pc_Value) + 1);
						strcpy(pmemo->pc_Value, p);
						for(i = 0; i < pmemo->WasSpaceAfter; i++) strcat(pmemo->pc_Value, " ");
						strcat(pmemo->pc_Value, pcur->pc_Value);
						pmemo->WasSpaceAfter = pcur->WasSpaceAfter;
						if(!pcur || pcur->WasEOLAfter)
						{
							/* EOL at the end of new cpp comment */
							pmemo->WasEOLAfter = 1;
							if(pcur)
							{
								pmemo->WasEOLAfter = pcur->WasEOLAfter;
							}

							/* Unlink and quit */
							Tool_UnlinkToken(pfile, pcur);
							pcur = pmemo;
							break;
						}

						Tool_UnlinkToken(pfile, pcur);
						pcur = pmemo;
					}
				}

				/* Add end comment mark */
				p = pmemo->pc_Value;
				pmemo->pc_Value = (char *) __malloc__(strlen(p) + 4);
				strcpy(pmemo->pc_Value, p);
				strcat(pmemo->pc_Value, " */");
				free(p);
			}
		}

		pcur = Tool_NextValid(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_DeleteBreakLine(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->i_ID == TOKEN_BREAKLINE)
		{
			if(pcur->In_PP_Include)
			{
				if(Config.IncludeLin) *pcur->pc_Value = '/';
			}
			else
			{
				/* Pass EOL after breakline to previous token */
				if(pcur->pst_Prev) pcur->pst_Prev->WasEOLAfter = pcur->WasEOLAfter;
				pcur = Tool_UnlinkToken(pfile, pcur);
				continue;
			}
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_ConvertCppToC(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pmemo;
	char	*buf;
	int		len;
	int		last;
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->i_ID == TOKEN_CPPCMT)
		{
			if(FixedComment(pcur))
			{
				pcur->CppComment = 1;
				pcur->WasEOLAfter = 1;
				pcur->i_ID = TOKEN_CCMT;
				pcur = NextToken(pcur);
				continue;
			}

			last = 0;
			if(!WasFirstOnLine(pcur))
				last = 1;
			else
				pcur->WasEOLAfter = 1;
			pcur->CppComment = 1;

			/* Convert first comment */
			len = strlen(pcur->pc_Value);
			buf = (char *) __malloc__(len + 4);
			strcpy(buf, pcur->pc_Value);
			buf[0] = '/';
			buf[1] = '*';
			buf[len] = ' ';
			buf[len + 1] = '*';
			buf[len + 2] = '/';
			buf[len + 3] = 0;
			len += 3;
			pcur->pc_Value = buf;

			/*
			 * All following comments are added to the C cmt, so this will convert several C++
			 * comments into one unique C comment. We care of a C++ comment that ends with a
			 * blank line.
			 */
			pmemo = pcur;

			/* Pass first C++ comment */
			if(Config.ConcatFirstLineCmt && pcur->WasEOLAfter <= 1)
			{
				pcur = NextToken(pcur);
				while(pcur && (pcur->i_ID == TOKEN_CPPCMT))
				{
					if(pcur->NoIndent) break;
					if(FixedComment(pcur)) break;
					if(last && WasFirstOnLine(pcur) && (pcur->pc_Value[2] != '/')) break;
					if(pcur->pc_Value[2] == '/') pcur->pc_Value[2] = ' ';
					if(pcur->WizardCmt) break;	/* Special C++ comment : No concat */

					buf = (char *) __malloc__(len + strlen(pcur->pc_Value) + 2);
					strcpy(buf, pmemo->pc_Value);

					/* Erase end mark */
					if(Config.CppToCKeepEOL && !Config.CmtKeepCpp)
					{
						if((buf[len - 3] != Config.CharSplit) && (buf[len - 4] != Config.CharSplit))
						{
							buf[len - 2] = (char) Config.CharSplit;
						}
					}
					else
						buf[len - 2] = '\n';
					buf[len - 1] = 0;
					strcat(buf, pcur->pc_Value + 2);
					len = strlen(buf);
					buf[len] = ' ';
					buf[len + 1] = '*';
					buf[len + 2] = '/';
					buf[len + 3] = 0;
					len += 3;

					/* Repercut EOL after in dest comment */
					if(pcur->WasEOLAfter > pmemo->WasEOLAfter)
					{
						pmemo->WasEOLAfter = pcur->WasEOLAfter;
					}

					Tool_UnlinkToken(pfile, pcur);
					pmemo->pc_Value = buf;

					/* Detect a blank lines after a C++ comment */
					pcur = pmemo;
					if(pcur->WasEOLAfter > 1)
					{
						pcur = NextToken(pcur);
						break;
					}

					pcur = NextToken(pcur);
				}
			}

			pmemo->i_ID = TOKEN_CCMT;
			continue;
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
token *Grammar_StmtLevel(FileDes *pfile, token *pcur, int StmtLevel)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int		count_braces;
	token	*pnext, *pend, *pmemo;
	char	fordo;
	char	fif;
	token	*pnn;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	count_braces = 0;
	fif = 0;

	/* Special case for do switch */
	fordo = 0;
	if(PrevToken(pcur) && SpecWord(PrevToken(pcur), TOKEN_W_DO))
	{
		fordo = 1;
		pnext = pcur;
		if(pcur->i_ID == TOKEN_CCMT) pnext = Tool_NextValid(pcur);
		if(pnext->i_ID != TOKEN_LBRACE) fordo = 2;
	}

	while(pcur)
	{
		pcur->StmtLevel = StmtLevel;
		switch(pcur->i_ID)
		{
		case TOKEN_LPAREN:
			pnext = Tool_ToRelationNext(pcur);
			if(!pnext) Syntaxe(pcur->line, pcur->column);
			while(pnext && pnext != pcur)
			{
				pcur->StmtLevel = StmtLevel;
				pcur = NextToken(pcur);
			}

			continue;

		case TOKEN_LBRACE:
			/* Extern "C" */
			pnext = Tool_PrevValid(pcur);
			if(pnext && !strcasecmp(pnext->pc_Value, "\"C\""))
			{
				if(Config.IndentExternC)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext->i_ID == TOKEN_LBRACE)
					{
						pend = Tool_ToRelationNext(pnext);
						if(!pend) break;
						while(pnext && pnext != pend)
						{
							pnext->IndentLevel = 1;
							pnext = NextToken(pnext);
						}
					}
				}

				pcur->StmtLevel = StmtLevel;
				pcur = NextToken(pcur);
				continue;
			}

			if(pnext && pnext->i_ID == TOKEN_WORD && pnext->i_SubID != TOKEN_W_DECL) pnext = Tool_PrevValid(pnext);

			if
			(
				pnext
			&&	pnext->i_ID == TOKEN_WORD
			&&	pnext->i_SubID == TOKEN_W_DECL
			&&	pnext->i_SubSubID == TOKEN_WW_NAMESPACE
			)
			{
				if(Config.IndentNamespace)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext && pnext->i_ID == TOKEN_WORD && pnext->i_SubID != TOKEN_W_DECL)
						pnext = Tool_NextValid(pnext);
					if(pnext->i_ID == TOKEN_LBRACE)
					{
						pend = Tool_ToRelationNext(pnext);
						if(!pend) break;
						while(pnext && pnext != pend)
						{
							pnext->IndentLevel = 1;
							pnext = NextToken(pnext);
						}
					}
				}

				pcur->StmtLevel = StmtLevel;
				pcur = NextToken(pcur);
				continue;
			}

			if(count_braces || !StmtLevel) StmtLevel++;
			pcur->StmtLevel = StmtLevel;
			pcur = NextToken(pcur);
			count_braces++;
			continue;

		case TOKEN_RBRACE:
			pnext = Tool_NextValid(pcur);
			pcur = NextToken(pcur);
			if(StmtLevel) StmtLevel--;
			if(count_braces) count_braces--;
			if((count_braces == 0) && StmtLevel)
			{
				/*
				 * If else ;
				 * if(SpecWord(pnext, TOKEN_W_ELSE) && forif) continue;
				 */
				if(!fordo)
					return pcur;
				else
					fordo = 0;
			}

			continue;

		/* Statement without braces detected ? */
		case TOKEN_SEMICOL:
			if((count_braces == 0) && StmtLevel)
			{
				if(!fordo)
					return pcur->pst_Next;
				else
				{
					/* Do While without brace */
					if(fordo == 2)
					{
						StmtLevel--;
						return pcur->pst_Next;
					}

					fordo = 0;
				}
			}
			break;

		case TOKEN_SPECWORD:
			pmemo = pcur;
			switch(pcur->i_SubID)
			{
			case TOKEN_W_IF:
				fif = 1;

			case TOKEN_W_CATCH:
			case TOKEN_W_FOR:
			case TOKEN_W_WHILE:
			case TOKEN_W_SWITCH:
				/* Get the end of the expression, and indent it to token */
				pnext = Tool_NextValid(pcur);

				/* Search matching ) */
				pend = Tool_ToRelationNext(pnext);
				if(!pend) break;

				/* Force expression to be at the same stmt level that the initial token */
				while(pnext && pnext != pend->pst_Next)
				{
					pnext->StmtLevel = pcur->StmtLevel;
					pnext = NextToken(pnext);
				}

				pcur = pend;
				pnext = pend;

			case TOKEN_W_TRY:
			case TOKEN_W_ELSE:
			case TOKEN_W_DO:
				/* Recurse call to retrieve the statement */
				pnext = Tool_NextValid(pcur);
				pcur = Grammar_StmtLevel(pfile, NextToken(pcur), StmtLevel + 1);

				/* Add brace if there's not */
				if(!pcur) return NULL;
				if(!pcur->NoIndent && Config.ForceBrace && (Tool_LenTokens(pnext, pcur->pst_Prev) > Config.ForceBrace))
				{
					if(pnext->i_ID != TOKEN_PP)
					{
						/* Don't add for else if */
						if(!SpecWord(pmemo, TOKEN_W_ELSE) || !SpecWord(pnext, TOKEN_W_IF))
						{
							if((pnext->i_ID != TOKEN_LBRACE) && (pnext->i_ID != TOKEN_SEMICOL))
							{
								/* If there's a comment before */
								pnn = pnext->pst_Prev;
								while(pnn->i_ID == TOKEN_CCMT) pnn = pnn->pst_Prev;
								pnn = pnn->pst_Next;

								Tool_InsertTokenBefore(pfile, pnn, TOKEN_LBRACE);
								pnn->pst_Prev->StmtLevel = pnext->StmtLevel;
								pnn->pst_Prev->WasEOLAfter = 0;

								/* If there's a end line comment, brace must be after */
								pnn = pcur->pst_Prev;
								while(!pnn->WasEOLAfter && pnn->pst_Next->i_ID == TOKEN_CCMT) pnn = pnn->pst_Next;

								Tool_InsertTokenAfter(pfile, pnn, TOKEN_RBRACE);
								pnn->pst_Next->StmtLevel = pnext->StmtLevel;
								pnn->pst_Next->WasEOLAfter = pnn->WasEOLAfter;
								pcur = pnn->pst_Next->pst_Next;
							}
						}
					}
				}

				/* If not open brace, that's the end of the statement */
				if((count_braces == 0) && StmtLevel)
				{
					if(!fif || !SpecWord(pcur, TOKEN_W_ELSE)) return pcur;
					fif = 0;
				}

				continue;
			}
			break;
		}

		pcur = NextToken(pcur);
	}

	return NULL;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_CorrectStmtLevel(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pmemo;
	char	spec, iOff;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Else if */
		if(SpecWord(pcur, TOKEN_W_ELSE))
		{
			pnext = Tool_NextValid(pcur);
			if(SpecWord(pnext, TOKEN_W_IF))
			{
				while(pnext->StmtLevel > pcur->StmtLevel)
				{
					pnext->StmtLevel--;
					pnext = NextToken(pnext);
				}
			}
		}

		/* Content of a PP directive */
		if(pcur->InPP && (pcur->i_ID != TOKEN_PP)) pcur->StmtLevel++;
		spec = 0;
		switch(pcur->i_ID)
		{
		case TOKEN_PP:
			if(!Config.PPToCode)
			{
				pcur->StmtLevel = 0;
				pnext = NextToken(pcur);
				iOff = (char) pnext->StmtLevel;
				while(pnext && pnext->InPP && pnext->i_ID != TOKEN_PP)
				{
					pnext->StmtLevel -= iOff;
					pnext = NextToken(pnext);
				}
			}
			break;

		case TOKEN_LBRACE:
			spec = 1;

		case TOKEN_RBRACE:
			pcur->StmtLevel--;
			break;

		case TOKEN_SPECWORD:
			switch(pcur->i_SubID)
			{
			case TOKEN_W_DEFAULT:	if(pcur->In_PP_Pragma){ pcur->i_ID = TOKEN_WORD; break; }
			case TOKEN_W_CASE:		pcur->StmtLevel--; spec = 1; break;
			}
			break;
		}

		/* Correct comments on first line before a special statement */
		if(spec)
		{
			pmemo = PrevToken(pcur);
			while(pmemo && pmemo->i_ID == TOKEN_CCMT)
			{
				if(WasFirstOnLine(pmemo)) pmemo->StmtLevel = pcur->StmtLevel;
				pmemo = PrevToken(pmemo);
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_AddVoid(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pmemo;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.AddVoid) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->IsFctDefName || pcur->IsFctDeclName)
		{
			pmemo = pcur;

			/* ( */
			pcur = Tool_NextValid(pcur);
			pnext = Tool_NextValid(pcur);
			if(pnext == Tool_ToRelationNext(pcur))
			{
				Tool_InsertTokenAfter(pfile, pcur, TOKEN_WORD);
				pcur->pst_Next->pc_Value = strdup("void");
				pcur->pst_Next->IsType = 1;
				pcur->pst_Next->InStmtDecl = 1;
				pcur->pst_Next->BeginDecl = 1;
			}

			pcur = pmemo;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_NoIndent(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	char	off, offs, offb;
	/*~~~~~~~~~~~~~~~~~~~~*/

	off = 0;
	offs = 0;
	offb = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* On/Off code ? */
		pcur->NoIndent = off;
		pcur->NoSplit = offs;
		pcur->AlignBody = offb;
		if(pcur->i_ID == TOKEN_CCMT)
		{
			if(pcur->pc_Value[2] == CMTMARK)
			{
				if(pcur->pc_Value[3] == 'o' && pcur->pc_Value[4] == 'n')
				{
					off = 0;
					pcur->NoIndent = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 'o' && pcur->pc_Value[4] == 'f' && pcur->pc_Value[4] == 'f')
				{
					off = 1;
					pcur->NoIndent = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 's' && pcur->pc_Value[4] == 'f')
				{
					offs = 1;
					pcur->NoSplit = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 's' && pcur->pc_Value[4] == 't')
				{
					offs = 0;
					pcur->NoSplit = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 'b' && pcur->pc_Value[4] == 't')
				{
					offb = 1;
					pcur->AlignBody = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 'b' && pcur->pc_Value[4] == 'f')
				{
					offb = 0;
					pcur->AlignBody = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}

				if(pcur->pc_Value[3] == 'n' && pcur->pc_Value[4] == 'a')
				{
					pcur->EndAlign = 1;
					pcur->i_SubSubID = TOKEN_W_CMT_OPTION;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_Class(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	token	*pmemo;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_SubSubID == TOKEN_WW_CLASS)
		{
			if(!pcur->InParen)
			{
				pmemo = pcur;
				while(pcur && (pcur->i_ID != TOKEN_SEMICOL) && (pcur->i_ID != TOKEN_LBRACE)) pcur = NextToken(pcur);
				if(pcur->i_ID == TOKEN_LBRACE)
				{
					pnext = Tool_ToRelationNext(pcur);
					Tool_SetFlag(pcur, pnext, &pcur->InClass, 1);
					pmemo->ClassDecl = 1;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_Cast(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pprev;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_LPAREN)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_WORD && pnext->i_SubID == TOKEN_W_DECL) goto ok;
			pnext = Tool_NextValid(pnext);
			if(pnext->i_ID == TOKEN_STAR || pnext->i_ID == TOKEN_ADDRESS)
			{
				while(pnext->i_ID == TOKEN_STAR || pnext->i_ID == TOKEN_ADDRESS) pnext = Tool_NextValid(pnext);
				if(pnext->i_ID == TOKEN_RPAREN) goto ok;
			}

			continue;
ok:
			pprev = Tool_PrevValid(pcur);
			if(!pprev) continue;
			if(pprev->IsFctDefName) continue;
			if(pprev->IsFctDeclName) continue;
			if(pprev->i_ID == TOKEN_SPECWORD && pprev->i_SubID == TOKEN_W_FOR) continue;
			if(pprev->i_ID == TOKEN_SPECWORD && pprev->i_SubID == TOKEN_W_SIZEOF) continue;
			pnext = Tool_NextValid(pnext);
			if(pnext->i_ID == TOKEN_LPAREN) continue;

			pnext = Tool_ToRelationNext(pcur);
			Tool_SetFlag(pcur, pnext, &pcur->IsCast, 1);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_ColCase(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(SpecWord(pcur, TOKEN_W_CASE))
		{
			while(pcur && pcur->i_ID != TOKEN_COLON) pcur = NextToken(pcur);
			if(pcur) pcur->ColCase = 1;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_ConcatStrings(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~*/

	if(!Config.ConcatStrings) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID == TOKEN_STRINGCST)
		{
			pnext = pcur->pst_Next;
			while(pnext && pnext->i_ID == TOKEN_STRINGCST)
			{
				pcur->pc_Value = (char *) __realloc__
					(
						pcur->pc_Value,
						strlen(pcur->pc_Value) + strlen(pnext->pc_Value) + 10
					);
				strcpy(pcur->pc_Value + strlen(pcur->pc_Value) - 1, pnext->pc_Value + 1);
				Tool_UnlinkToken(pfile, pnext);
				pnext = pcur->pst_Next;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void DeclIncludes(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	int		i, j;
	/*~~~~~~~~~~~~~~~~~~*/

	if(!Config.Dependencies) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_PP && pcur->i_SubID == TOKEN_PP_INCLUDE)
		{
			memset(&gast_Includes[gi_NumIncludes], 0, sizeof(tdst_Include));
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LESS) continue;
			strcpy(gast_Includes[gi_NumIncludes].asz_Name, pnext->pc_Value + 1);
			*strrchr(gast_Includes[gi_NumIncludes].asz_Name, '\"') = 0;
			for(i = 0; i < gi_NumIncludes; i++)
			{
				if(!strcmp(gast_Includes[i].asz_Name, gast_Includes[gi_NumIncludes].asz_Name))
				{
					gast_Includes[i].i_NumInc++;
					for(j = 0; j < gast_Includes[i].i_NumIncludeBy; j++)
					{
						if(gast_Includes[i].ai_IncludeBy[j] == (int) pfile) break;
					}

					if(j == gast_Includes[i].i_NumIncludeBy)
					{
						gast_Includes[i].ai_IncludeBy[j] = (int) pfile;
						gast_Includes[i].i_NumIncludeBy++;
					}

					goto recom;
				}
			}

			gast_Includes[gi_NumIncludes].i_NumInc = 1;
			gast_Includes[gi_NumIncludes].ai_IncludeBy[0] = (int) pfile;
			gast_Includes[gi_NumIncludes].i_NumIncludeBy = 1;
			gi_NumIncludes++;
recom: ;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Depend(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	int		i, j, iCpt, iMax, k;
	token	*pcur, *pcur1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.Dependencies) return;
	printf("- Includes files --------------------------------------------------------\n");
	j = 0;
	for(i = 1; i < gi_NumFiles + gi_NumIncludes; i++)
	{
		if(!gpst_Files[i].psz_FileName) j++;
	}

	for(i = 1; i < gi_NumFiles + gi_NumIncludes; i++)
	{
		if(gpst_Files[i].psz_FileName)
		{
			printf("(%6d) #include \"%s\"\n", gast_Includes[i - gi_NumFiles].i_NumInc, gpst_Files[i].psz_FileName);
		}
	}

	printf("******** Unresolved %d Total %d\n", j, gi_NumIncludes);
	if(Config.DependenciesAll)
		iMax = gi_NumFiles + gi_NumIncludes - 1;
	else
		iMax = 1;
	printf("- Scanning %3d ----------------------------------------------------------\n", iMax);
	for(i = 0; i < iMax; i++)
	{
		fflush(stdout);
		printf(".");
		fflush(stdout);
		for(pcur = gpst_Files[i].pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if
			(
				((pcur->i_ID == TOKEN_WORD) || (pcur->DefineCst))
			&&	(!(pcur->IsFctParamName) && !(pcur->IsFctDefName) && !(pcur->DefineName) && !(pcur->MacroName))
			)
			{
				for(j = 0; j < gi_NumFiles + gi_NumIncludes; j++)
				{
					if(j == i) continue;
					for(pcur1 = gpst_Files[j].pst_RootToken; pcur1; pcur1 = NextToken(pcur1))
					{
						if
						(
							(
								(pcur1->IsFctDeclName)
							||	(pcur1->DefineName)
							||	(pcur1->MacroName)
							||	(pcur1->DefineCst)
							||	((pcur1->i_ID == TOKEN_WORD) && (!pcur1->StmtLevel) && (pcur1->i_SubID != TOKEN_W_DECL))
							)
						&&	(!(pcur1->IsFctParamName) && !(pcur1->DefineCst) && !(pcur1->IsType))
						)
						{
							if(!(strcmp(pcur1->pc_Value, pcur->pc_Value)))
							{
								if(i == 0)
									gast_Includes[j - gi_NumFiles].i_DepDir++;
								else
									gast_Includes[j - gi_NumFiles].i_Dep++;
							}
						}
					}
				}
			}
		}
	}

	printf("\n");
	printf("- Direct dependencies ---------------------------------------------------\n");
	iCpt = 0;
	for(i = 1; i < gi_NumFiles + gi_NumIncludes; i++)
	{
		if(gpst_Files[i].psz_FileName && gast_Includes[i - gi_NumFiles].i_DepDir)
		{
			printf("(%6d) #include \"%s\"\n", gast_Includes[i - gi_NumFiles].i_DepDir, gpst_Files[i].psz_FileName);
			iCpt++;
		}
	}

	printf("******** Total %d\n", iCpt);
	if(Config.DependenciesAll)
	{
		printf("- No Direct dependencies ------------------------------------------------\n");
		iCpt = 0;
		for(i = 1; i < gi_NumFiles + gi_NumIncludes; i++)
		{
			if
			(
				gpst_Files[i].psz_FileName
			&&	gast_Includes[i - gi_NumFiles].i_Dep
			&&	!gast_Includes[i - gi_NumFiles].i_DepDir
			)
			{
				printf("(%6d) #include \"%s\"\n", gast_Includes[i - gi_NumFiles].i_Dep, gpst_Files[i].psz_FileName);
				iCpt++;
			}
		}

		printf("******** Total %d\n", iCpt);
	}

	printf("- No dependencies -------------------------------------------------------\n");
	iCpt = 0;
	for(i = 1; i < gi_NumFiles + gi_NumIncludes; i++)
	{
		if
		(
			gpst_Files[i].psz_FileName
		&&	!gast_Includes[i - gi_NumFiles].i_Dep
		&&	!gast_Includes[i - gi_NumFiles].i_DepDir
		)
		{
			printf("(%6d) #include \"%s\"\n", gast_Includes[i - gi_NumFiles].i_NumInc, gpst_Files[i].psz_FileName);
			for(k = 0; k < gast_Includes[i - gi_NumFiles].i_NumIncludeBy; k++)
			{
				printf
				(
					"            Included by \"%s\"",
					((FileDes *) gast_Includes[i - gi_NumFiles].ai_IncludeBy[k])->psz_FileName
				);
				printf("\n");
			}

			iCpt++;
		}
	}

	printf("******** Total %d\n", iCpt);
	printf("-------------------------------------------------------------------------\n");
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_VisualCPPCmt(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	int		i;
	char	*p1[] =
	{
		"//{{AFX_MSG",
		"//}}AFX_MSG",
		"//{{AFX_MSG_MAP",
		"//}}AFX_MSG_MAP",
		"//{{AFX_DATA",
		"//}}AFX_DATA",
		"//{{AFX_DATA_INIT",
		"//}}AFX_DATA_INIT",
		"//{{AFX_DATA_MAP",
		"//}}AFX_DATA_MAP",
		"//{{AFX_FIELD",
		"//}}AFX_FIELD",
		"//{{AFX_FIELD_INIT",
		"//}}AFX_FIELD_INIT",
		"//{{AFX_FIELD_MAP",
		"//}}AFX_FIELD_MAP",
		"//{{AFX_EVENT",
		"//}}AFX_EVENT",
		"//{{AFX_EVENT_MAP",
		"//}}AFX_EVENT_MAP",
		"//{{AFX_DISP",
		"//}}AFX_DISP",
		"//{{AFX_DISP_MAP",
		"//}}AFX_DISP_MAP",
		"//{{AFX_VIRTUAL",
		"//}}AFX_VIRTUAL",
		"//{{AFX_INSERT_LOCATION}}",
	};
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID != TOKEN_CPPCMT) continue;

		for(i = 0; i < sizeof(p1) / sizeof(char *); i++)
		{
			if(!strncmp(pcur->pc_Value, p1[i], strlen(p1[i])))
			{
				if((pcur->pc_Value[2] == '}') || (pcur->pc_Value[strlen(pcur->pc_Value) - 1] == '}'))
					pcur->WizardCmtEnd = 1;
				pcur->WizardCmt = 1;
				break;
			}
		}
	}

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(!pcur->WizardCmt) continue;
		while(pcur && !pcur->WizardCmtEnd)
		{
			if(pcur->i_ID == TOKEN_CPPCMT) pcur->WizardCmt = 1;
			if(!Config.WizardIndent) pcur->NoIndent = 1;
			pcur->InWizardCmt = 1;
			pcur = pcur->pst_Next;
		}

		if(pcur->WizardCmtEnd)
		{
			if(!Config.WizardIndent) pcur->NoIndent = 1;
			pcur->InWizardCmt = 1;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar(FileDes *pfile)
{
	/* Dependencies */
	DeclIncludes(pfile);

	/* Set no indent flag */
	Grammar_NoIndent(pfile);

	/* Set flags */
	Grammar_CommentAsm(pfile);
	Grammar_InPP(pfile);

	/* Delete or change some tokens */
	Grammar_DeleteBreakLine(pfile);
	Grammar_DefineCst(pfile);
	Grammar_VisualCPPCmt(pfile);
	Grammar_ConvertCppToC(pfile);
	Grammar_ConcatStrings(pfile);

	/* Set statement level */
	Grammar_CmtLevel(pfile);
	Grammar_StmtLevel(pfile, pfile->pst_RootToken, 0);
	Grammar_CorrectStmtLevel(pfile);

	/* Set flags */
	Grammar_InsideParen(pfile);
	Grammar_LabelDecl(pfile);
	Grammar_Class(pfile);
	Grammar_IsType(pfile);

	/*
	 * Grammar_IsTypeCorrect(pfile);
	 * ?? Needed ??
	 */
	Grammar_InTemplate(pfile);
	Grammar_InsideParen(pfile);
	Grammar_IsType(pfile);
	Grammar_FctDef(pfile);
	Grammar_FctParams(pfile);
	Grammar_FctCall(pfile);
	Grammar_Cast(pfile);
	Grammar_UnaryBinary(pfile);
	Grammar_ColCase(pfile);
	Grammar_InEnum(pfile);

	/* Delete or change some tokens */
	Grammar_AddVoid(pfile);
	Grammar_InsideParen(pfile);

	/* Decl */
	Grammar_BeginEndDecl(pfile);
	Grammar_DeclStmt(pfile);
	Grammar_MoveDeclAffect(pfile);
	Grammar_MoveDecl(pfile);
	Grammar_DeclStmt(pfile);
	Grammar_WhileForDo(pfile);
	Grammar_MoveCmt(pfile);

	/* Find class and function comments */
	Grammar_CmtClassFunc(pfile);
}
