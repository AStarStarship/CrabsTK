/*$T \Sources/indent_eol.c GC 1.150 2011-09-22 20:52:17 */


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
void Indent_EOLBase(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pnext, *pprev, *pcur1, *pnext1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent && pcur->pst_Next && !pcur->pst_Next->NoIndent)
		{
			pcur->ForceEOLAfter = 1;
			pcur->WasEOLAfter = 1;
		}

		if(Config.BreakFctType && pcur->IsFctDefName)
		{
			pcur1 = pcur;
			pprev = Tool_PrevValid(pcur);
			if(pprev->i_ID == TOKEN_DOUBLECOLON && pprev->IsType)
			{
				pcur = pprev;
				pprev = Tool_PrevValid(pcur);
				if(pprev->i_ID == TOKEN_WORD) pcur = pprev;
			}

			Tool_ForceEOLBefore(pcur, 1);
			pcur = pcur1;
		}

		if(pcur->NoIndent)
		{
			pcur->ForceEOLAfter = (char) pcur->WasEOLAfter;
			continue;
		}

		if(pcur->InASM && pcur->WasEOLAfter) Tool_ForceEOLAfter(pcur, 1);
		if(Config.KeepEmptyLinesCode)
		{
			/* Empty line after an include */
			if(pcur->In_PP_Include)
			{
				pnext = NextToken(pcur);
				while(pnext && pnext->In_PP_Include && (pnext->i_ID != TOKEN_PP)) pnext = NextToken(pnext);
				if(pnext) pnext = PrevToken(pnext);
				if(pnext && pnext->WasEOLAfter > 1) Tool_ForceEmptyLineAfter(pnext);
			}

			/* Empty line after a define */
			if(pcur->In_PP_Define)
			{
				pnext = NextToken(pcur);
				while(pnext && pnext->In_PP_Define && (pnext->i_ID != TOKEN_PP)) pnext = NextToken(pnext);
				pnext = PrevToken(pnext);
				if(pnext && pnext->WasEOLAfter > 1) Tool_ForceEmptyLineAfter(pnext);
			}

			/* After a end line comment */
			if(pcur->i_ID == TOKEN_CCMT && !WasFirstOnLine(pcur) && WasLastOnLine(pcur))
			{
				if(pcur->WasEOLAfter > 1) Tool_ForceEmptyLineAfter(pcur);
			}
		}

		/* Force label to be alone on line */
		if(pcur->LabelDecl)
		{
			Tool_ForceEOLBefore(pcur, 1);
			pnext = Tool_NextValid(pcur);

			if(pcur->i_ID == TOKEN_WORD && pcur->i_SubID == TOKEN_W_DECL && pcur->i_SubSubID == TOKEN_WW_CLASSACCESS)
			{
				Tool_ForceEOLAfterComments(pnext, Config.AccessBlanks);
				pcur->pst_Prev->ForceEOLAfter = (char) Config.AccessBlanksBefore;
			}
			else
			{
				Tool_ForceEOLAfterComments(pnext, Config.LabelBlanks);
			}

			continue;
		}

		/* Word() that is not a function */
		if((pcur->i_ID == TOKEN_WORD) && (!pcur->IsType))
		{
			pnext = Tool_NextValid(pcur);
			pprev = Tool_PrevValid(pcur);
			if
			(
				(pnext)
			&&	(pnext->i_ID == TOKEN_LPAREN)
			&&	(!pcur->IsFctCall)
			&&	(!pcur->IsFctDeclName)
			&&	(!pcur->IsFctDefName)
			&&	(!pcur->In_PP_Pragma)
			&&	(!pcur->In_PP_If)
			&&	(!pcur->DefineName)
			&&	(!pcur->InParen)
			&&	(pprev && (pprev->i_ID != TOKEN_UNCHANGE))
			&&	(pprev && (pprev->i_ID != TOKEN_DOUBLECOLON))
			)
			{
				pnext1 = Tool_ToRelationNext(pnext);
				pnext1 = Tool_NextValid(pnext1);
				pnext = Tool_PrevValid(pcur);
				if((pnext1->i_ID != TOKEN_SEMICOL) && (pnext1->i_ID != TOKEN_AFFECT))
				{
					if
					(
						(pnext)
					&&	(pnext->i_ID == TOKEN_WORD)
					&&	(!pnext->In_PP_Pragma)
					&&	(!pnext->In_PP_If)
					&&	(!pnext->In_PP_Define)
					)
					{
						Tool_ForceSpaceAfter(pcur, 1);
					}
					else
					{
						pnext = Tool_NextValid(pcur);
						Tool_ForceEOLBefore(pcur, 1);
						pnext = Tool_ToRelationNext(pnext);
						Tool_ForceEOLAfterComments(pnext, 1);
					}
				}
			}
		}

		/* Template */
		if(Config.BreakTemplate && (pcur->i_SubSubID == TOKEN_WW_TEMPLATE))
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LESS)
			{
				pnext = Tool_ToRelationNext(pnext);
				Tool_ForceEOLAfterComments(pnext, 1);
			}
		}

		switch(pcur->i_ID)
		{
		/* EOL after semicol on end of instruction */
		case TOKEN_SEMICOL:
			if(!pcur->InParen)	/* Don't treat
								 * ;
								 * in for */
				Tool_ForceEOLAfterComments(pcur, 1);
			if(Config.KeepEmptyLinesCode && !pcur->InParen)
			{
				if(pcur->WasEOLAfter > 1) Tool_ForceEmptyLineAfter(pcur);
			}
			break;

		/* Force braces to be alone on line */
		case TOKEN_LBRACE:
			if(pcur->InitDecl)
			{
				if(PrevToken(pcur) && PrevToken(pcur)->i_ID != TOKEN_AFFECT)
				{
					pnext = Tool_ToRelationNext(pcur);
					if(pcur->Col + Tool_LenTokens(pcur, pnext) > Config.LineLenCode)
					{
						Tool_ForceEOLBefore(pcur, 1);
						Tool_ComputeCol(pfile);
					}

					pcur = pnext;
					break;
				}
			}

			Tool_ForceEOLBefore(pcur, 1);
			Tool_ForceEOLAfterComments(pcur, 1);
			break;

		case TOKEN_RBRACE:
			Tool_ForceEOLBefore(pcur, 1);
			pcur1 = pcur;

			/*
			 * } word;
			 * (for structure def for example)
			 */
recom:
			pnext = Tool_NextValid(pcur1);

			if((pnext->i_ID == TOKEN_WORD) || (pnext->i_ID == TOKEN_SEMICOL))
			{
				if(pnext->i_ID == TOKEN_WORD) pnext = Tool_NextValid(pnext);

				while(pnext->i_ID == TOKEN_LARRAY)
				{
					pnext = Tool_ToRelationNext(pnext);
					pnext = Tool_NextValid(pnext);
				}

				if(Tool_SearchFirst(pnext, TOKEN_SEMICOL, TOKEN_LBRACE) == TOKEN_SEMICOL)
				{
					pprev = Tool_PrevValid(pcur1);
					while(pprev && pprev->InPP) pprev = Tool_PrevValid(pprev);
					if((pnext->EndDecl) || (pprev && pprev->EndDecl) || (pprev && pprev->InEnum))
					{
						pnext = NextToken(pcur);
						if(pnext->i_ID != TOKEN_SEMICOL) Tool_ForceSpaceAfter(pcur, 1);
						break;
					}

					pprev = Tool_PrevValid(pcur1);
					while(pprev->i_ID == TOKEN_SEMICOL) pprev = Tool_PrevValid(pprev);
					if((pprev->i_ID == TOKEN_WORD) || (pprev->i_ID == TOKEN_RBRACE))
					{
						if(pprev->i_ID == TOKEN_WORD) pprev = Tool_PrevValid(pprev);

						if(pprev->i_ID == TOKEN_RBRACE)
						{
							pnext = pcur1 = pprev;
							goto recom;
						}
					}
				}
			}

			Tool_ForceEOLAfterComments(pcur, 1);
			break;

		/* Around spec word */
		case TOKEN_SPECWORD:
			switch(pcur->i_SubID)
			{
			case TOKEN_W_IF:
			case TOKEN_W_CATCH:
			case TOKEN_W_WHILE:
			case TOKEN_W_FOR:
			case TOKEN_W_SWITCH:
				pcur = Tool_SearchToken(pcur, TOKEN_LPAREN);
				pcur = Tool_ToRelationNext(pcur);
				Tool_ForceEOLAfterComments(pcur, 1);
				break;

			case TOKEN_W_DO:
				Tool_ForceEOLAfterComments(pcur, 1);
				break;

			case TOKEN_W_ELSE:
				pnext = Tool_NextValid(pcur);
				if(!SpecWord(pnext, TOKEN_W_IF) || !Config.ConcatElseIf)
				{
					Tool_ForceEOLAfterComments(pcur, 1);
				}

				if(SpecWord(pnext, TOKEN_W_IF) && Config.ConcatElseIf)
				{
					pcur->ForceEOLAfter = 0;
					pcur->ForceSpaceAfter = 1;
				}
				break;

			case TOKEN_W_CASE:
				pnext = Tool_SearchToken(pcur, TOKEN_COLON);
				if(pnext) Tool_ForceEOLAfterComments(pnext, 1);

				/* Before */
lcase:
				pnext = Tool_PrevValid(pcur);
				if(pnext && !pnext->ColCase && pnext->i_ID != TOKEN_LBRACE)
				{
					if(Config.SwitchEol == 0)
						Tool_ForceEOLAfterComments(pnext, 2);
					else if(Config.SwitchEol == 1)
						pnext->ForceEOLAfter = 1;
				}
				break;

			case TOKEN_W_DEFAULT:
				if(!pcur->InParen)
				{
					/* Warning(default:) */
					pnext = Tool_NextValid(pcur);
					Tool_ForceEOLAfterComments(pnext, 1);
					goto lcase;
				}
				break;
			}
			break;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int Indent_SplitCommonParen(FileDes *pfile, token *pcur, token *pend, int id, int oper)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*ptmp, *ptmp1;
	/*
	 * token *ptmp2;
	 */
	char	caninc;
	token	*ptmp3;
	/*~~~~~~~~~~~~~~~~~~*/

	caninc = 0;

	if(!oper)
	{
		ptmp = PrevToken(pcur);
		if(ptmp->IndentLevel == pcur->IndentLevel)
		{
			ptmp1 = Tool_PrevValid(pcur);
			if(ptmp1->i_ID != TOKEN_LINK)
			{
				Tool_IncIndentLevel(pcur, pend, 1);
				Tool_IncIndentLevel(pcur->pst_Next, pend->pst_Prev, 1);
			}
		}

		Tool_ForceEOLBefore(pcur, 1);
		Tool_ComputeColLine(pcur);

		Tool_ForceEOLAfterComments(pcur, 1);
		Tool_ComputeColLine(NextToken(Tool_ToEndLine(pcur)));

		Tool_ForceEOLBefore(pend, 1);
		Tool_ComputeColLine(pend);
	}

	ptmp = NextToken(pcur);
	while(ptmp && ptmp != pend)
	{
		if((ptmp->i_ID == TOKEN_LPAREN) || (ptmp->i_ID == TOKEN_LARRAY)) ptmp = Tool_ToRelationNext(ptmp);
		if(!ptmp) break;

		if
		(
			(ptmp->i_ID == id)
		||	oper
		&&	(
				((ptmp->i_ID == TOKEN_SIGN) && (ptmp->BinaryOp))
			||	((ptmp->i_ID == TOKEN_STAR) && (ptmp->BinaryOp))
			||	(ptmp->i_ID == TOKEN_OR)
			||	(ptmp->i_ID == TOKEN_OPER)
			)
		)
		{
			if(ptmp->InParen)
			{
				ptmp3 = ptmp->OpenParen;
				if(ptmp3->InTemplate && ptmp3->i_ID == TOKEN_LESS)
				{
					ptmp = Tool_ToRelationNext(ptmp3);
					ptmp3 = ptmp3->OpenParen;
					goto zapzap;
				}

				if(!LastOnLineWithCmt(ptmp3))
				{
					if(ptmp3 != pcur || Tool_ToRelationNext(ptmp3) != pend)
					{
						Indent_SplitCommonParen(pfile, ptmp3, Tool_ToRelationNext(ptmp3), id, 0);
						Tool_IncIndentLevel(Tool_ToRelationNext(ptmp3), pend, 1);
					}
				}
			}

			if(!ptmp->ForceEOLAfter)
			{
				Tool_ForceEOLAfterComments(ptmp, 1);
				Tool_ComputeColLine(NextToken(Tool_ToEndLine(ptmp)));
				caninc = 1;
			}
		}

zapzap:
		if(ptmp != pend) ptmp = NextToken(ptmp);
	}

	if(caninc && oper)
	{
		if(pcur->pst_Next->IndentLevel == pcur->IndentLevel) Tool_IncIndentLevel(pcur->pst_Next, pend, 1);
	}

	return caninc;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SplitTooLong(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev, *pend, *pnew, *pret = NULL, *ptmp, *ptmp1;
	token	*pline;
	int		iparen, imaxparen, col;
	char	ccan1;
	token	*a_Paren[200];
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pline = NULL;
	iparen = 0;
	ccan1 = 0;

	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur;)
	{
		if(pcur->NoBreak)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->NoSplit)
		{
			pcur = NextToken(pcur);
			continue;
		}

		/* Don't treat some tokens. */
		switch(pcur->i_ID)
		{
		case TOKEN_CPPCMT:
			if(pcur->pst_Prev != 0)
			{
				if(pcur->pst_Prev->i_ID != TOKEN_CPPCMT && pcur->pst_Prev->i_ID != TOKEN_CCMT) break;

				/* place CPP comment on separate lines */
				pcur->ForceEOLAfter = pcur->pst_Prev->ForceEOLAfter = 1;
			}

		case TOKEN_CCMT:
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->pst_Prev && pcur->pst_Prev->ForceEOLAfter)
		{
			pline = pcur;
			ccan1 = 1;
		}

		if((pcur->Col + (int) strlen(pcur->pc_Value) > Config.LineLenCode) || (pcur->ForceSplit))
		{
ForceBreak:
			if(pcur->i_ID == TOKEN_LINK)
			{
				pprev = pcur->pst_Prev;
				if(pprev->i_ID == TOKEN_CCMT)
				{
					col = pcur->Col - strlen(pprev->pc_Value);
					if(col < Config.LineLenCode)
					{
						pcur = NextToken(pcur);
						continue;
					}
				}
			}

			pnew = pcur;
			if(pcur->i_ID == TOKEN_RPAREN)
			{
				if(pcur->pst_Prev->i_ID == TOKEN_LPAREN)
				{
					pcur = NextToken(pcur);
					continue;
				}

				pnew = PrevToken(pcur);
			}

			if(pcur->i_ID == TOKEN_GREAT && pcur->InTemplate)
			{
				if(pcur->pst_Prev->i_ID == TOKEN_LESS)
				{
					pcur = NextToken(pcur);
					continue;
				}

				pnew = PrevToken(pcur);
			}

			if(pcur->i_ID == TOKEN_LPAREN)
			{
				if(pcur->pst_Next->i_ID == TOKEN_RPAREN)
				{
					pcur = NextToken(pcur);
					continue;
				}

				pnew = NextToken(pcur);
			}

			if(pcur->i_ID == TOKEN_LESS && pcur->InTemplate)
			{
				if(pcur->pst_Next->i_ID == TOKEN_GREAT)
				{
					pcur = NextToken(pcur);
					continue;
				}

				pnew = NextToken(pcur);
			}

			if(pnew->InParen)
			{
				iparen = 0;
				a_Paren[iparen++] = pnew->OpenParen;
				while(a_Paren[iparen - 1]->InParen)
				{
					a_Paren[iparen] = a_Paren[iparen - 1]->OpenParen;
					iparen++;
				}

				iparen--;
				imaxparen = iparen;
				while((iparen >= 0) && FirstOnLine(a_Paren[iparen]))
				{
					iparen--;
				}

				/*
				 * Check all main parenthesis. If we split a parenthese that is not the main, we
				 * must be sure that all main parenthesis must be alone on line.
				 */
				while(imaxparen != iparen)
				{
					if(!LastOnLineWithCmt(a_Paren[imaxparen]))
					{
						if(iparen != -1)
						{
							pprev = Tool_PrevValid(a_Paren[iparen]);
							if(pprev && pprev->i_ID == TOKEN_LINK)
							{
								pend = Tool_ToRelationNext(a_Paren[imaxparen]);
								Indent_SplitCommonParen(pfile, a_Paren[imaxparen], pend, TOKEN_LINK, 0);
								if(a_Paren[imaxparen]->IndentLevel == a_Paren[imaxparen]->pst_Next->IndentLevel)
									Tool_IncIndentLevel(a_Paren[imaxparen]->pst_Next, pend->pst_Prev, 1);
								pcur = a_Paren[imaxparen];
								iparen = -1;
								break;
							}
						}

						pend = Tool_ToRelationNext(a_Paren[imaxparen]);

						Tool_ForceEOLAfterComments(a_Paren[imaxparen], 1);

						if(a_Paren[imaxparen]->pst_Next->IndentLevel == a_Paren[imaxparen]->IndentLevel)
						{
							Tool_IncIndentLevel(a_Paren[imaxparen]->pst_Next, pend->pst_Prev, 1);
						}

						Tool_ForceEOLBefore(pend, 1);
						Tool_ComputeCol(pfile);
					}

					imaxparen--;
				}

				if(iparen >= 0)
				{
					pprev = Tool_PrevValid(a_Paren[iparen]);
					pend = Tool_ToRelationNext(a_Paren[iparen]);

					/* First try to split a whole expression at operators */
					if((pprev->IsFctCall) || (SpecWord(pprev, TOKEN_W_SIZEOF)))
					{
						ptmp1 = pprev->pst_Prev;
						if((ptmp1->i_ID == TOKEN_LINK) && (!ptmp1->ForceEOLAfter))
						{
							if(pline->i_ID == TOKEN_LPAREN)
							{
								ptmp1 = Tool_ToRelationNext(pline);
								Indent_SplitCommonParen(pfile, pline, ptmp1, TOKEN_LINK, 1);
								continue;
							}
						}

						if(ccan1 && pline)
						{
							ptmp1 = pcur;
							while(ptmp1 && ptmp1->i_ID != TOKEN_SEMICOL)
							{
								if(ptmp1 == pend) break;
								ptmp1 = NextToken(ptmp1);
							}

							if(ptmp1 && ptmp1 != pend)
							{
								Indent_SplitCommonParen(pfile, pline, ptmp1, TOKEN_LINK, 1);
								ccan1 = 0;
								continue;
							}
						}
					}

					/* Function pointer ? */
					if(pprev->i_ID == TOKEN_RPAREN)
					{
						ptmp1 = Tool_ToRelationPrev(pprev);
						ptmp1 = Tool_PrevValid(ptmp1);
						if(ptmp1 && ptmp1->IsType)
						{
							Tool_ForceEOLAfter(pprev, 1);
							goto func;
						}
					}

					/* Template */
					if(a_Paren[iparen]->i_ID == TOKEN_LESS && pcur->InTemplate)
					{
						Indent_SplitCommonParen(pfile, a_Paren[iparen], pend, TOKEN_COMMA, 0);
						pcur = a_Paren[iparen];
						continue;
					}

					/* Function. */
					if
					(
						(pprev->IsFctCall)
					||	(pprev->IsFctDeclName)
					||	(pprev->IsFctDefName)
					||	(SpecWord(pprev, TOKEN_W_SIZEOF))
					)
					{
func:
						Indent_SplitCommonParen(pfile, a_Paren[iparen], pend, TOKEN_COMMA, 0);
						pcur = a_Paren[iparen];
						continue;
					}

					/* For. */
					if(SpecWord(pprev, TOKEN_W_FOR))
					{
						Indent_SplitCommonParen(pfile, a_Paren[iparen], pend, TOKEN_SEMICOL, 0);
						pcur = a_Paren[iparen];
						continue;
					}

					/* If, While. */
					if
					(
						(SpecWord(pprev, TOKEN_W_IF))
					||	(SpecWord(pprev, TOKEN_W_WHILE))
					||	(SpecWord(pprev, TOKEN_W_RETURN))
					)
					{
						Indent_SplitCommonParen(pfile, a_Paren[iparen], pend, TOKEN_LINK, 0);
						pcur = a_Paren[iparen];
						continue;
					}

					/* Force EOL before expression */
					if(pprev->i_ID == TOKEN_AUTOOP || pprev->i_ID == TOKEN_AFFECT)
					{
						pprev = Tool_SearchToken(pcur, TOKEN_SEMICOL);
						if(pprev && Indent_SplitCommonParen(pfile, a_Paren[iparen], pprev, TOKEN_LINK, 1))
						{
							pcur = a_Paren[iparen];
							continue;
						}
					}

					Tool_ForceEOLBefore(a_Paren[iparen], 1);
					pprev = a_Paren[iparen]->pst_Prev;
					if(a_Paren[iparen]->IndentLevel == pprev->IndentLevel)
					{
						Tool_IncIndentLevel(a_Paren[iparen], Tool_ToEndLine(a_Paren[iparen]), 1);
					}

					Tool_ComputeColLine(a_Paren[iparen]);
					pcur = a_Paren[iparen];
					continue;
				}
			}

			/* Split a normal expression. */
			if(!FirstOnLine(pcur) && (!pcur->IsType))
			{
				/* Try to split first at opers */
				if(ccan1 && pline)
				{
					pend = pcur;
					while(pend && (pend->i_ID != TOKEN_SEMICOL) && (pend->i_ID != TOKEN_COMMA))
					{
						if(pend->i_ID == TOKEN_LPAREN)
						{
							pend = Tool_ToRelationNext(pend);
						}
						else if(pend->i_ID == TOKEN_RPAREN)
						{
							pend = PrevToken(pend);
							break;
						}

						pend = NextToken(pend);
					}

					if(pend)
					{
						if(!pline->IsFctCall)
						{
							if(!Indent_SplitCommonParen(pfile, pline, pend, TOKEN_LINK, 1))
								Indent_SplitCommonParen(pfile, pline, pend, 0, 1);
						}

						ccan1 = 0;
						continue;
					}
				}

				/* In some cases, don't break before */
				if((!pcur->IsFctDeclName) && (!pcur->IsFctDefName))
				{
					pnew = pcur;

					/* Is there an expression on line ? */
					ptmp = pcur;
					while(PrevToken(ptmp) && (!PrevToken(ptmp)->ForceEOLAfter))
					{
						if(ptmp->i_ID == TOKEN_RPAREN)
						{
							pnew = ptmp;
							goto paren;
						}

						ptmp = PrevToken(ptmp);
					}

					while
					(
						(pnew->i_ID != TOKEN_WORD)
					&&	(pnew->i_ID != TOKEN_STRINGCST)
					&&	(pnew->i_ID != TOKEN_CHARCST)
					)
					{
						/* Find an expression */
						if(pnew->i_ID == TOKEN_RPAREN)
						{
paren:
							if(pret != pnew)
							{
								pret = pnew;
								pcur = pnew;
								goto ForceBreak;
							}
							else
							{
								pret = NULL;
								pnew = NextToken(pnew);
								break;
							}
						}

						pnew = PrevToken(pnew);
						if(!pnew || pnew->ForceEOLAfter) break;
					}

					if(pnew && !pnew->ForceEOLAfter)
					{
						if(!FirstOnLine(pnew)) pcur = pnew;
					}

					pprev = pcur->pst_Prev;
					Tool_ForceEOLBefore(pcur, 1);
					Tool_IncIndentLevel(pcur, Tool_ToEndLine(pcur), 1);
					Tool_ComputeColLine(pcur);
				}
			}
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_EOLAddEmptyLines(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pprev;
	token	*pnext;
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		/* TT This code seems to never fire... Empty functions */
		if(pcur->i_ID == TOKEN_LBRACE && pcur->InFct && pcur->StmtLevel == 0 && !pcur->AlignBody)
		{
			pnext = pcur->pst_Next;
			if(pnext->i_ID == TOKEN_RBRACE)
			{
				pcur->ForceEOLAfter = (char) (Config.EmptyFctBlanks);
				if(Config.EmptyFctBlanks == 0)
				{
					pcur->ForceSpaceAfter = 1;
				}

				continue;	/* TT */
			}
		}

		/* Empty line before comment on first line */
		pprev = PrevToken(pcur);
		if
		(
			(pcur->i_ID == TOKEN_CCMT)
		&&	(FirstOnLine(pcur))
		&&	(LastOnLine(pcur))
		&&	(!pcur->InParen)
		&&	(!pcur->AddCmt)
		&&	(!pcur->OptionCmt)
		&&	(!pcur->UnAlignCmt)
		&&	(pprev && !pprev->AddCmt)
		&&	(!pcur->pst_Next || !pcur->pst_Next->EmptyCmtBefore)
		)
		{
			if(pprev && pprev->i_ID == TOKEN_CCMT && FirstOnLine(pprev) && LastOnLine(pprev))
			{
				if(Config.FirstLineBlank)
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

						Tool_ForceEmptyLineBefore(pprev);
					}
					else
					{
						Tool_ForceEmptyLineBefore(pcur);
					}
				}
			}
			else
			{
				Tool_ForceEmptyLineBefore(pcur);
			}
		}

		/*
		 * Empty line after } Note that there's now additional TT code to do this
		 * elsewhere...
		 */
		if(pcur->i_ID == TOKEN_RBRACE && pcur->InFct && LastOnLineWithCmt(pcur) && FirstOnLine(pcur))
		{
			pnext = Tool_NextValid(pcur);
			if((pnext->i_ID != TOKEN_SPECWORD) || (pnext->i_SubID != TOKEN_W_BREAK))
			{
				if(!pcur->pst_Next || !EmptyComment(pcur->pst_Next))
				{
					if(!LastOnLine(pcur))
					{
						while(!pcur->ForceEOLAfter) pcur = pcur->pst_Next;
					}

					Tool_ForceEmptyLineAfter(pcur);
				}
			}
		}

		if((pcur->BegStmtDecl) && (pcur->InFct) && ((!pcur->pst_Prev->EndStmtDecl || pcur->pst_Prev->StmtLevel == 0)))
		{
			pprev = pcur->pst_Prev;
			if(pprev->i_ID != TOKEN_CCMT && pprev->i_ID != TOKEN_LBRACE) Tool_ForceEmptyLineAfter(pcur->pst_Prev);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_EOLBetweenEmptyBraces(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext;
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Empty brace pairs */
		if(pcur->i_ID == TOKEN_LBRACE)
		{
			pnext = NextToken(pcur);	/* pcur->pst_Next;
										 * */
			if(pnext && (pnext->i_ID == TOKEN_RBRACE))
			{
				if(pcur->ForceEOLAfter)
				{
					pcur->ForceEOLAfter = (char) (Config.EmptyFctBlanks);	/* not + 1 any more TT */
					if(Config.EmptyFctBlanks == 0)
					{
						pcur->ForceSpaceAfter = 1;
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
void Indent_EOLAfterEveryCloseBrace(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext;
	/*~~~~~~~~~~~*/

	if(!Config.EOLAfterEveryCloseBrace) return;

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Empty brace pairs */
		if(pcur->i_ID == TOKEN_RBRACE)
		{
			pnext = NextToken(pcur);	/* pcur->pst_Next;
										 * */
			if(pnext)
			{
				if
				(
					(pnext->i_ID != TOKEN_RBRACE)
				&&	(pnext->i_ID != TOKEN_SEMICOL)
				&&	(!SpecWord(pnext, TOKEN_W_ELSE))
				&&	(!SpecWord(pnext, TOKEN_W_WHILE))
				)
				{
					if(pcur->WasEOLAfter > 0)
					{
						pcur->ForceEOLAfter = (char) (Config.EOLAfterEveryCloseBrace + 1);
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
void Indent_SplitStaticInitStruct(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pend, *pprev;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	Tool_ComputeCol(pfile);

	/* Struct static init */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
recom:
		pprev = Tool_PrevValid(pcur);
		if(pcur->i_ID == TOKEN_LBRACE && pprev && pprev->i_ID == TOKEN_AFFECT)
		{
			pend = Tool_ToRelationNext(pcur);
			pnext = Tool_NextValid(pend);

			/* First check if the whole init can be on a single line */
			if(pprev->Col + Tool_LenTokens(pprev, pnext) <= Config.LineLenCode)
			{
				/* Do nothing if there's a pp directive in init */
				pcur = pprev;
				while(pcur != pend)
				{
					if(pcur->InPP)
					{
						pcur = pnext;
						goto recom;
					}

					pcur = NextToken(pcur);
				}

				pcur = pprev;
				while(pcur != pend)
				{
					if(pcur->ForceEOLAfter) pcur->ForceEOLAfter = 0;
					pcur = NextToken(pcur);
				}

				pcur = pnext;
				continue;
			}

			/* Braces inside braces */
			while(pcur && pcur != pend)
			{
				pnext = Tool_NextValid(pcur);
				if(pnext->i_ID == TOKEN_LBRACE)
				{
					pcur = pnext;

					/* Concat lines */
					pnext = Tool_ToRelationNext(pcur);
					if(Tool_LenTokens(pcur, pnext) <= Config.LineLenCode)
					{
						pcur->ForceEOLAfter = 0;
						pnext->pst_Prev->ForceEOLAfter = 0;
					}

					pcur = pnext;
					Tool_ForceEOLAfterComments(pcur, 1);
				}
				else
				{
					/* A normal struct init */
					if(pcur->i_ID == TOKEN_COMMA) Tool_ForceEOLAfterComments(pcur, 1);
					if(pcur->i_ID == TOKEN_LPAREN) pcur = Tool_ToRelationNext(pcur);
					pcur = NextToken(pcur);
				}
			}

			pcur = pend;
		}
	}

	/* Enum */
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_SubSubID == TOKEN_WW_ENUM)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_WORD) pnext = Tool_NextValid(pnext);
			if(pnext->i_ID == TOKEN_LBRACE)
			{
				pend = Tool_ToRelationNext(pnext);
				pcur = pnext;
				while(pcur != pend)
				{
					if(pcur->i_ID == TOKEN_COMMA) Tool_ForceEOLAfterComments(pcur, 1);
					pcur = NextToken(pcur);
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ConcatEnum(FileDes *pfile, token *pcur)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pnext, *pend;
	token	*pnext1;
	int		len;
	/*~~~~~~~~~~~~~~~~~~*/

	pfile = pfile;
	if(!Config.ConcatEnum) return;

	pnext = Tool_NextValid(pcur);
	if(pnext->i_ID != TOKEN_LBRACE) pnext = Tool_NextValid(pnext);
	if(pnext->i_ID != TOKEN_LBRACE) return;

	pend = Tool_ToRelationNext(pnext);
	pnext1 = Tool_NextValid(pend);
	if(pnext1->i_ID != TOKEN_SEMICOL) pnext1 = Tool_NextValid(pnext1);

	len = Tool_LenTokens(pnext, pnext1);
	if(len + pnext->Col < Config.LineLenCode)
	{
		pnext = pcur;
		while(pnext != pnext1)
		{
			if(pnext->i_ID == TOKEN_AFFECT)
			{
				pnext->pst_Prev->ForceSpaceAfter = 1;
				pnext->ForceSpaceAfter = 1;
			}

			if(pnext->ForceEOLAfter) pnext->ForceSpaceAfter = 1;
			pnext->ForceEOLAfter = 0;
			pnext = pnext->pst_Next;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_ConcatSwitch(FileDes *pfile, token *pcur)
{
	/*~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pend, *ptmp;
	token	*pend1, *pprev;
	int		len, maxcol, mode;
	int		col1;
	/*~~~~~~~~~~~~~~~~~~~~~~*/

	pfile = pfile;
	if(!Config.ConcatSwitch) return;

	pcur = Tool_NextValid(pcur);
	pcur = Tool_ToRelationNext(pcur);
	pcur = Tool_NextValid(pcur);	/* { */
	pend = Tool_ToRelationNext(pcur);
	pcur = Tool_NextValid(pcur);

	/* All case can be concat ? */
	ptmp = pcur;
	while(ptmp && ptmp != pend)
	{
		if(ptmp->NoIndent) return;
		if
		(
			((ptmp->i_SubID == TOKEN_W_CASE) || (ptmp->i_SubID == TOKEN_W_DEFAULT))
		&&	ptmp->StmtLevel == pcur->StmtLevel
		)
		{
			while(ptmp->i_ID != TOKEN_COLON)
			{
				ptmp = Tool_NextValid(ptmp);
				if(ptmp->NoIndent) return;
			}

			pend1 = Tool_NextValid(ptmp);
			if(pend1->i_SubID == TOKEN_W_CASE) return;
			if(pend1->i_SubID == TOKEN_W_DEFAULT) return;

			col1 = Tool_ToTab(ptmp->Col) + Config.TabSize;
			ptmp = ptmp->pst_Next;
			pend1 = ptmp;
			while(pend1->StmtLevel != pcur->StmtLevel)
			{
				if(pend1->NoIndent) return;
				pend1 = pend1->pst_Next;
			}

			len = Tool_LenTokens(ptmp, pend1->pst_Prev);
			if(len + col1 > Config.LineLenCode) return;
		}

		ptmp = ptmp->pst_Next;
	}

	/* Yes, then concat */
	maxcol = -1;
	for(mode = 0; mode < 2; mode++)
	{
		ptmp = pcur;
		while(ptmp && ptmp != pend)
		{
			if
			(
				((ptmp->i_SubID == TOKEN_W_CASE) || (ptmp->i_SubID == TOKEN_W_DEFAULT))
			&&	ptmp->StmtLevel == pcur->StmtLevel
			)
			{
				pprev = Tool_PrevValid(ptmp);
				if(pprev->ForceEOLAfter > 1) pprev->ForceEOLAfter = 1;

				Tool_ComputeColLine(ptmp);
				while(ptmp->i_ID != TOKEN_COLON && ptmp != pend) ptmp = Tool_NextValid(ptmp);
				if(mode == 0)
				{
					if(Tool_NextValid(ptmp) != pend) ptmp->ForceEOLAfter = 0;
				}

				ptmp = ptmp->pst_Next;
				if(mode == 0)
				{
					if(ptmp->pst_Prev->Col + 1 >= maxcol)
					{
						maxcol = ptmp->pst_Prev->Col + 1;
						maxcol = Tool_ToTab(maxcol);
						if(maxcol <= ptmp->pst_Prev->Col + 1) maxcol += Config.TabSize;
					}
				}
				else
				{
					if(ptmp->Col < maxcol)
					{
						Tool_ForceSpaceBefore(ptmp, (maxcol - ptmp->Col + 1));
					}
				}

				pend1 = ptmp;
				while(pend1 && pend1 != pend && pend1->pst_Next->StmtLevel != pcur->StmtLevel)
				{
					if(mode == 0)
					{
						pend1->ForceEOLAfter = 0;
					}

					pend1 = pend1->pst_Next;
				}

				ptmp = pend1;
			}

			if(ptmp != pend) ptmp = ptmp->pst_Next;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_RemoveEOL(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev, *pnext, *pend, *pnext1, *ptmp;
	token	*pmemo, *pmemo1;
	char	ok;
	int		len, base;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	ok = 0;

	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		/* else */
		if(Config.ConcatElse)
		{
			if(SpecWord(pcur, TOKEN_W_ELSE))
			{
				pprev = Tool_PrevValid(pcur);
				if(pprev->i_ID == TOKEN_RBRACE)
				{
					pprev->ForceSpaceAfter = 1;
					pprev = PrevToken(pcur);
					pprev->ForceEOLAfter = 0;
					pprev->ForceSpaceAfter = 1;
				}
			}
		}

		/* Empty lines in decl stmt */
		if(Config.RemoveEOLDecl && pcur->InDeclStmt && pcur->ForceEOLAfter > 1) pcur->ForceEOLAfter = 1;

		/* switch */
		if(pcur->i_ID == TOKEN_SPECWORD && pcur->i_SubID == TOKEN_W_SWITCH) Indent_ConcatSwitch(pfile, pcur);

		/* switch */
		if(pcur->i_ID == TOKEN_WORD && pcur->i_SubID == TOKEN_W_DECL && pcur->i_SubSubID == TOKEN_WW_ENUM)
			Indent_ConcatEnum(pfile, pcur);

		/* Macro */
		if(pcur->MacroName && Config.ConcatMacros)
		{
			pcur = pcur->pst_Next;
			pcur = Tool_ToRelationNext(pcur);
			base = pcur->Col + strlen(pcur->pc_Value);
			pnext1 = pcur;
			pcur = Tool_NextValid(pcur);
			if(pcur->i_ID == TOKEN_LBRACE)
			{
				pend = Tool_ToRelationNext(pcur);
				if(pend)
				{
					len = Tool_LenTokens(pcur, pend);
					len += base;
					if(len < Config.LineLenCode)
					{
						pcur = pnext1;
						while(pcur != pend)
						{
							if(pcur->ForceEOLAfter) pcur->ForceSpaceAfter = 1;
							pcur->ForceEOLAfter = 0;
							pcur = pcur->pst_Next;
						}
					}
				}
			}
		}

		/* Function decl type */
		if(pcur->i_ID == TOKEN_LPAREN)
		{
			pprev = Tool_PrevValid(pcur);
			if(pprev && pprev->i_ID == TOKEN_RPAREN && pprev->ForceEOLAfter)
			{
				if((pprev->StmtLevel == 0) && (!pcur->ForceEOLAfter))
				{
					pprev->ForceSpaceAfter = 0;
					pprev->ForceEOLAfter = 0;
				}
			}
		}

		/* Strings */
		if(pcur->i_ID == TOKEN_STRINGCST)
		{
			pprev = Tool_PrevValid(pcur);
			if(pprev && pprev->i_ID == TOKEN_STRINGCST)
			{
				pcur->IndentLevel = pprev->IndentLevel;
				pcur->StmtLevel = pprev->StmtLevel;
			}
		}

		if(pcur->i_ID == TOKEN_STRINGCST && FirstOnLine(pcur))
		{
			pprev = Tool_PrevValid(pcur);
			if(pprev && pprev->i_ID == TOKEN_AFFECT)
			{
				pprev->ForceEOLAfter = 0;
				pprev->ForceSpaceAfter = 1;
			}
		}

		/*
		 * ;
		 * alone
		 */
		if(pcur->i_ID == TOKEN_SEMICOL && FirstOnLine(pcur))
		{
			pprev = pcur->pst_Prev;
			if(pprev)
			{
				switch(pprev->i_ID)
				{
				case TOKEN_STRINGCST:	pprev->ForceEOLAfter = 0; break;
				}
			}
		}

		/* ! ( for example, with ! and ( alone on line */
		if
		(
			FirstOnLine(pcur)
		&&	(pcur->i_ID != TOKEN_LPAREN)
		&&	(pcur->i_ID != TOKEN_RPAREN)
		&&	(pcur->i_ID != TOKEN_LBRACE)
		&&	(pcur->i_ID != TOKEN_RBRACE)
		)
		{
			pnext = NextToken(pcur);
			if(pnext && pnext->i_ID == TOKEN_LPAREN && FirstOnLine(pnext))
			{
				if((int) strlen(pcur->pc_Value) == 1)
				{
					pcur->ForceEOLAfter = 0;
					if(pnext->IndentLevel > pcur->IndentLevel)
					{
						pend = Tool_ToRelationNext(pnext);
						Tool_IncIndentLevel(pnext, pend, -1);
					}
				}
			}
		}

		/* While for Do */
		if(pcur->WhileForDo)
		{
			pnext = pcur;
			do
			{
				pnext = PrevToken(pnext);
				if(!pnext) Syntaxe(pcur->line, pcur->column);
				if(Config.BreakDoWhile)
					pnext->ForceEOLAfter = 1;
				else
					pnext->ForceEOLAfter = 0;
				pnext->ForceSpaceAfter = 1;
			} while(pnext->i_ID != TOKEN_RBRACE);
		}

		/* Concat If */
		if(SpecWord(pcur, TOKEN_W_IF) || SpecWord(pcur, TOKEN_W_WHILE) || SpecWord(pcur, TOKEN_W_FOR))
		{
			if(LastOnLine(pcur))
			{
				pnext = Tool_NextValid(pcur);

				if(!LastOnLine(pnext))
				{
					pend = Tool_ToRelationNext(pnext);
					if(!pend) Syntaxe(pnext->line, pnext->column);
					while((pnext != pend) && (pnext->i_ID != TOKEN_EOL)) pnext = NextToken(pnext);
					if(pnext->i_ID == TOKEN_RPAREN && LastOnLine(pnext)) pcur->ForceEOLAfter = 0;
				}
			}
		}

		/* Concat Else */
		if(SpecWord(pcur, TOKEN_W_ELSE))
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID != TOKEN_LBRACE)
			{
				if(Config.ConcatIfAndElse)
				{
					/*
					 * put if statements together if possible... ;
					 * should check line length...?
					 */
					pcur->ForceEOLAfter = 0;
				}
				else
				{
					/* previous "}"? */
					pnext = PrevToken(pcur);

					if(pcur->InConcatIf)
					/* if(pnext->i_ID == TOKEN_RBRACE) */
					{
						/* problem - put if statements together... */
						pcur->ForceEOLAfter = 0;
					}
					else
					{
						/* otherwise split them apart... */
						pnext = Tool_NextValid(pcur);
						if(!SpecWord(pnext, TOKEN_W_IF))
						{
							pcur->ForceEOLAfter = 1;
						}
					}
				}
			}
		}

		/* Concat If */
		if(Config.ConcatIf)
		{
			/* used to also require: "FirstOnLine(pcur) && ..." */
			if
			(
				FirstOnLine(pcur)
			&&	(SpecWord(pcur, TOKEN_W_IF) || SpecWord(pcur, TOKEN_W_WHILE) || SpecWord(pcur, TOKEN_W_FOR))
			)
			{
				pmemo = pcur;
				pnext = Tool_NextValid(pcur);
				pnext = Tool_ToRelationNext(pnext);
				pnext1 = Tool_NextValid(pnext);
				if(!pnext1) return;
				if(pnext1->i_ID == TOKEN_CCMT) continue;
				if(pnext1->i_ID == TOKEN_LBRACE) continue;
				if(pnext1->i_ID == TOKEN_PP) continue;

				ok = 1;

				while(pnext1->StmtLevel > pcur->StmtLevel)
				{
					if(pnext1->StmtLevel > pcur->StmtLevel + 1)
					{
						ok = 0;
						break;
					}

					pnext1 = pnext1->pst_Next;
				}

				if(ok)
				{
					pmemo1 = pnext1;
					len = Tool_LenTokens(pcur, pnext1->pst_Prev);
					if(len + pcur->Col < Config.LineLenCode)	/* should fire - but doesn't */
					{
						if(Config.ConcatIfAndElse || !SpecWord(pnext1, TOKEN_W_ELSE))	/* TT */
						{
							pnext->ForceEOLAfter = 0;
							pnext1 = NextToken(pnext);
							if(pnext1->i_ID != TOKEN_SEMICOL)
							{
								pnext->ForceSpaceAfter = 1;
							}

							Tool_SetFlag(pmemo, pmemo1->pst_Prev, &pmemo->InConcatIf, 1);

							/* Remove empty lines if prev stmt is a concat if */
							if(Config.RemoveConcatIf)
							{
								if(pmemo->pst_Prev->ForceEOLAfter > 1 && pmemo->pst_Prev->InConcatIf)
									pmemo->pst_Prev->ForceEOLAfter = 1;
							}
						}
					}
				}
			}
		}

		/* Other concat */
		ptmp = pcur;
		if(ptmp->pst_Prev && ptmp->pst_Prev->ForceEOLAfter)
		{
			if
			(
				(!SpecWord(pcur, TOKEN_W_IF))
			&&	(!SpecWord(pcur, TOKEN_W_CASE))
			&&	(!SpecWord(pcur, TOKEN_W_DEFAULT))
			&&	(!SpecWord(pcur, TOKEN_W_ELSE))
			&&	(!SpecWord(pcur, TOKEN_W_WHILE))
			&&	(!SpecWord(pcur, TOKEN_W_DO))
			&&	(!SpecWord(pcur, TOKEN_W_FOR))
			&&	(pcur->i_ID != TOKEN_LBRACE)
			&&	((pcur->i_ID != TOKEN_LPAREN) || (!LastOnLine(pcur)))
			&&	(pcur->i_ID != TOKEN_PP)
			&&	(pcur->i_ID != TOKEN_CCMT)
			&&	(pcur->StmtLevel)
			&&	(!pcur->IsFctCall)
			&&	(!pcur->IsType)
			)
			{
				if(pcur->NoConcat)
				{
					while(pcur->NoConcat) pcur = NextToken(pcur);
					pcur = pcur->pst_Prev;
					continue;
				}

				ptmp = pcur;
				while(ptmp && !ptmp->ForceEOLAfter) ptmp = ptmp->pst_Next;
				if
				(
					ptmp
				&&	ptmp->pst_Next
				&&	(
						(ptmp->pst_Next->StmtLevel + ptmp->pst_Next->IndentLevel) ==
							(pcur->StmtLevel + pcur->IndentLevel + 1)
					)
				)
				{
					pnext = ptmp;
					ptmp = ptmp->pst_Next;
					while(ptmp && !ptmp->ForceEOLAfter) ptmp = ptmp->pst_Next;
					if(ptmp && ptmp->pst_Next && ptmp->pst_Next->StmtLevel <= pcur->StmtLevel)
					{
						if(ptmp->pst_Next->IndentLevel == pcur->IndentLevel)
						{
							if((ptmp->i_ID != TOKEN_LPAREN) && (pnext->i_ID != TOKEN_SEMICOL))
							{
								pnext->ForceEOLAfter = 0;
								pnext->pst_Next->IndentLevel--;
								pcur = PrevToken(pcur);
								continue;
							}
						}
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
void Indent_RetrospectiveElseAdjustment(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pprev;
	/*~~~~~~~~~~~*/

	if(Config.ConcatElse)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			/* Concat "} else" */
			if(SpecWord(pcur, TOKEN_W_ELSE))
			{
				pprev = Tool_PrevValid(pcur);
				if(pprev)
				{
					if(pprev->i_ID == TOKEN_RBRACE)
					{
						if(FirstOnLine(pprev))
						{
							pprev->ForceEOLAfter = 0;
							pprev->ForceSpaceAfter = 1;
						}
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
void Indent_Conform(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pnext1, *pend, *pprev;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* ( \n line \n ) : Concat */
		if((pcur->i_ID == TOKEN_LPAREN) && (LastOnLine(pcur)))
		{
			pend = Tool_ToRelationNext(pcur);
			pnext = NextToken(pcur);
			while(pnext && !pnext->ForceEOLAfter && pnext != pend) pnext = NextToken(pnext);
			if(!pnext) continue;

			pnext1 = NextToken(pnext);
			if(pnext1 == pend)
			{
				/* If it is a parent for a function call ? If yes, put the name first on line */
				pprev = PrevToken(pcur);
				if(pprev->IsFctCall)
				{
					pprev->IndentLevel = 0;
					pprev->ForceEOLAfter = 0;
				}

				pcur->ForceEOLAfter = 0;
				pnext->ForceEOLAfter = 0;
				pcur = pnext1;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_BoolSplit(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	char	ccan;
	/*~~~~~~~~~~~~~~~~~~*/

	if(Config.SplitBoolBefore)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			ccan = 0;
			if(pcur->i_ID == TOKEN_LINK)
			{
				if(!pcur->ForceEOLAfter)
				{
					pnext = pcur->pst_Next;
					while(pnext->i_ID == TOKEN_CCMT)
					{
						if(pnext->ForceEOLAfter)
						{
							ccan = 1;
							break;
						}

						pnext = pnext->pst_Next;
					}
				}
				else
					ccan = 1;

				if(ccan)
				{
					if(pcur->pst_Next->i_ID == TOKEN_CCMT)
					{
						while(pcur->pst_Next->i_ID == TOKEN_CCMT)
						{
							Tool_MoveTokenBefore(pfile, pcur->pst_Next, pcur);
							if(pcur->pst_Prev->ForceEOLAfter) break;
						}
					}

					pcur->pst_Prev->ForceEOLAfter = 1;
					pcur->ForceEOLAfter = 0;
					pcur->IndentLevel--;
					if((int) strlen(pcur->pc_Value) >= (int) Config.TabSize)
						pcur->ForceSpaceAfter = 1;
					else
						pcur->ForceSpaceAfter = (char) (Config.TabSize - strlen(pcur->pc_Value));
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void BraceStyle(FileDes *pfile, token *pcur, token *pend, int style)
{
	/*~~~~~~~~~~~*/
	token	*pmem;
	token	*ptemp;
	/*~~~~~~~~~~~*/

	if(!style) return;

	pmem = pcur;
	while(!pmem->ForceEOLAfter)
	{
		if(pmem->pst_Next->i_ID != TOKEN_CCMT) return;
		pmem = NextToken(pmem);
	}

	if(style == 1)
	{
		pcur->StmtLevel++;
		pend->StmtLevel++;
		return;
	}

	if((style == 2) || (style == 3))
	{
		if(FirstOnLine(pcur) && pcur->pst_Prev)
		{
			/* New code - TT */
			ptemp = pcur->pst_Prev;

			while(ptemp && IsAComment(ptemp))
			{
				ptemp = ptemp->pst_Prev;
			}

			if(ptemp)
			{
				if((ptemp->i_ID == TOKEN_RPAREN) || (SpecWord(ptemp, TOKEN_W_ELSE)))
				{
					while(pcur->pst_Prev && IsAComment(pcur->pst_Prev))
					{
						pcur->StmtLevel++;
						Tool_MoveTokenBefore(pfile, pcur, pcur->pst_Prev);
					}

					if(pcur->pst_Prev)
					{
						pcur->pst_Prev->ForceSpaceAfter = 1;
						pcur->pst_Prev->ForceEOLAfter = 0;
					}

					pcur->ForceEOLAfter = 1;

					if(pcur->pst_Next && IsAComment(pcur->pst_Next))
					{
						/* have another blank line... */
						pcur->ForceEOLAfter = 2;
					}
				}
			}
		}

		if(style == 3) pend->StmtLevel++;
		return;
	}

	if(style == 4)
	{
		pcur->ForceEOLAfter = 0;
		pcur->ForceSpaceAfter = (char) (Config.TabSize - 1);
		return;
	}

	if(style == 5 && pcur->StmtLevel)
	{
		pend->AddSpaceBefore = pcur->AddSpaceBefore = (Config.TabSize + 1) / 2;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_InClassIndent(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pend;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID != TOKEN_LBRACE) continue;
		pnext = Tool_NextValid(pcur);
		if(!pnext) continue;
		pend = Tool_ToRelationNext(pcur);
		if(!pend) continue;
		if(pcur->InClass && pcur->StmtLevel == 0)
		{
			pcur = pcur->pst_Next;
			while(pcur != pend)
			{
				pcur->IndentLevel += Config.IndentClass;
				pcur = pcur->pst_Next;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_BraceStyle(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pend;
	token	*pprev;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(Config.BraceStyleFct == 0 && Config.BraceStyle == 0 && Config.BraceStyleDecl == 0 && Config.BraceStyleClass == 0)
		return;

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID != TOKEN_LBRACE) continue;
		pnext = Tool_NextValid(pcur);
		if(!pnext) continue;
		pend = Tool_ToRelationNext(pcur);
		if(!pend) continue;

		if(pcur->InClass && pcur->StmtLevel == 0)
		{
			BraceStyle(pfile, pcur, pend, Config.BraceStyleClass);
			continue;
		}

		if(pnext->StmtLevel == pcur->StmtLevel + 1 || pnext == pend)
		{
			if(pcur->InStmtDecl)
			{
				pprev = Tool_PrevValid(pcur);
				if(pprev && pprev->InFctDef)
					BraceStyle(pfile, pcur, pend, Config.BraceStyleFct);
				else
					BraceStyle(pfile, pcur, pend, Config.BraceStyleDecl);
			}
			else
				BraceStyle(pfile, pcur, pend, Config.BraceStyle);
			continue;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_EOLEmptyStmt(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev;
	/*~~~~~~~~~~~~~~~~~~*/

	if(!Config.EOLAlone) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_SEMICOL)
		{
			if(pcur->InParen) continue;
			pprev = Tool_PrevValid(pcur);
			if(pprev && pprev->i_ID == TOKEN_RPAREN)
			{
				pprev = Tool_ToRelationPrev(pprev);
				if(pprev)
				{
					pprev = Tool_PrevValid(pprev);
					if
					(
						SpecWord(pprev, TOKEN_W_IF)
					||	SpecWord(pprev, TOKEN_W_FOR)
					||	SpecWord(pprev, TOKEN_W_SWITCH)
					||	(SpecWord(pprev, TOKEN_W_WHILE) && !pprev->WhileForDo)
					)
					{
						pcur->pst_Prev->ForceEOLAfter = 1;
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
void Indent_MakeSplitStyleNormal(token *pcur, int style)
{
	/*~~~~~~~~~~~*/
	token	*pnext;
	token	*pend;
	int		len;
	int		column;
	/*~~~~~~~~~~~*/

	pnext = NextToken(pcur);

	if(pnext->i_ID != TOKEN_LPAREN) return;
	pend = Tool_ToRelationNext(pnext);

	pcur->ForceEOLAfter = 0;

	if((style == 3) || (style == 4))
	{
		pnext->ForceEOLAfter = 1;
	}
	else
	{
		pnext->ForceEOLAfter = 0;
	}

	pcur = pnext;

	Tool_ComputeColLine(Tool_ToBegLine(pcur));
	column = pcur->Col;
	column++;

	if(style == 3)
	{
		column -= Config.TabSize;
	}

	pnext = NextToken(pcur);

	len = 100000;

	while(pcur != pend)
	{
		Tool_ComputeColLine(Tool_ToBegLine(pcur));

		/* Split every parameters */
		if((pcur != pend->pst_Prev) || style == 4)
		{
			if(pcur->Col + (int) strlen(pcur->pc_Value) + len > Config.LineLenCode)
			{
				if(pcur != pend)
				{
					if(style == 3)
					{
					}
					else if(style == 4)
					{
						pcur->IndentLevel--;
					}
					else
					{
						pcur->AddSpaceAfter = column - ((pcur->IndentLevel) * Config.TabSize);
						pcur->AddSpaceAfter -= (pcur->StmtLevel) * Config.TabSize;

						if(Config.SpaceParen)
						{
							pcur->AddSpaceAfter++;
						}
					}

					if(pcur == pend->pst_Prev && style == 4) pcur->ForceEOLAfter = 0;
					pcur = pcur->pst_Next;
					pnext = pcur;
					continue;
				}
				else
				{
					if(style == 4) continue;
				}
				break;
			}
		}

		if(style == 4)
		{
			pcur->IndentLevel--;
		}

		if(!IsAComment(pcur))
		{
			/* Wrap on to the end of this line... */
			pcur->ForceEOLAfter = 0;

			/*
			 * pcur->pst_Next->IndentLevel = pcur->IndentLevel;
			 */
		}
		else
		{
			/* Put onto a new line... */
			pcur->AddSpaceAfter = column - ((pcur->IndentLevel) * Config.TabSize);
		}

		pcur = pnext;
		pnext = NextToken(pcur);
		Tool_ComputeColLine(Tool_ToBegLine(pcur));
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int putNewLine(int column, int style, int first_one, token *pcur)
{
	/*~~~~~~~~~~~*/
	token	*ptemp;
	/*~~~~~~~~~~~*/

	ptemp = NextToken(pcur);

	/* Deal with final bracket... */
	if(ptemp && ptemp->i_ID == TOKEN_RPAREN)
	{
		if(style == 5)
		{
			column -= Config.TabSize;

			/*
			 * pcur->IndentLevel++;
			 */
		}
		else
		{
			column += Config.TabSize;

			/*
			 * pcur->IndentLevel--;
			 */
		}
	}

	/*
	 * pcur->AddSpaceAfter = column - ((pcur->IndentLevel) * Config.TabSize);
	 * // pnext?
	 */
	if(style == 5)
	{
		pcur->AddSpaceAfter = 2 - ((pcur->IndentLevel) * Config.TabSize);		/* pnext? */
	}
	else
	{
		pcur->AddSpaceAfter = column - ((pcur->IndentLevel) * Config.TabSize);	/* pnext? */
		/*
		 * pcur->ForceSpaceAfter = 0;
		 */
	}

	if(style == 5)
	{
		if(first_one)
		{
			pcur->AddSpaceAfter -= Config.TabSize;	/* pnext? */
		}
	}

	return column;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_MakeSplitStyleWrapped(token *pcur, int style)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pnext;
	token	*pprev;
	token	*pend;
	int		len;
	/*
	 * int first_offset;
	 * ;
	 */
	int		column;
	int		first_one = 1;
	/*~~~~~~~~~~~~~~~~~~*/

	pnext = NextToken(pcur);

	if(pnext->i_ID != TOKEN_LPAREN) return;
	pend = Tool_ToRelationNext(pnext);

	pcur->ForceEOLAfter = 0;

	if(style == 5)
	{
		pnext->ForceEOLAfter = 1;
	}
	else
	{
		pnext->ForceEOLAfter = 0;
	}

	pcur = pnext;

	if(style == 1)
	{
		Tool_ComputeColLine(Tool_ToBegLine(pcur));
		column = pcur->Col;
		column++;
		column -= Config.TabSize * pcur->StmtLevel; /* Statement level */
	}
	else
	{
		pprev = Tool_ToBegLine(PrevToken(pcur));
		Tool_ComputeColLine(pprev);
		column = pprev->Col;
		column += Config.TabSize;
	}

	pnext = NextToken(pcur);

	len = 100000;

	while(pcur != pend)
	{
		len = 0;
		while(pnext != pend && !pnext->ForceEOLAfter)
		{
			len += strlen(pnext->pc_Value);

			if(pnext->ForceSpaceAfter > 1)
			{
				pnext->ForceSpaceAfter = 1;
			}

			pnext = NextToken(pnext);
		}

		len += strlen(pnext->pc_Value);

		/*
		 * if (len > 300) print("long len");
		 */
		Tool_ComputeColLine(Tool_ToBegLine(pcur));

		if(pcur == pend) break;

		if(pcur != pend->pst_Prev)
		{
			if(pcur->Col + (int) strlen(pcur->pc_Value) + len >= Config.LineLenCode)
			{
				column = putNewLine(column, style, first_one, pcur);

				if(Config.SpaceParen) pcur->AddSpaceAfter++;

				pcur = NextToken(pcur);
				pnext = pcur;
				continue;
			}
		}

		/*
		 * TT effort to remove serious bug... ...causing lines to wrap onto the end of
		 * commented lines... ...if previous line does not end in a comment...
		 */
		if(!IsAComment(pcur) && ((style != 5) || !first_one))
		{
			/* Wrap on to the end of this line... */
			pcur->ForceEOLAfter = 0;
		}
		else
		{
			column = putNewLine(column, style, first_one, pcur);
		}

		first_one = 0;

		pcur = pnext;
		pnext = NextToken(pcur);

		Tool_ComputeColLine(Tool_ToBegLine(pcur));
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_MakeSplitStyle(token *pcur, int style)
{
	if(!style) return;

	if(style == 1)
	{
		Indent_MakeSplitStyleWrapped(pcur, style);
		return;
	}

	if(style == 5)
	{
		Indent_MakeSplitStyleWrapped(pcur, style);
		return;
	}

	Indent_MakeSplitStyleNormal(pcur, style);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SplitStyle(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	Tool_ComputeCol(pfile);
	if(Config.SplitFctCall) goto ok;
	if(Config.SplitFctDef) goto ok;
	if(Config.SplitFctDecl) goto ok;
	if(Config.ForSplitStyle) goto ok;
	if(Config.IfSplitStyle) goto ok;
	return;

ok:
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(!pcur->ForceEOLAfter) continue;

		if(SpecWord(pcur, TOKEN_W_IF))
		{
			Indent_MakeSplitStyle(pcur, Config.IfSplitStyle);
			continue;
		}

		if(SpecWord(pcur, TOKEN_W_FOR))
		{
			Indent_MakeSplitStyle(pcur, Config.ForSplitStyle);
			continue;
		}

		if(pcur->IsFctCall && Config.SplitFctCall)
		{
			Indent_MakeSplitStyle(pcur, Config.SplitFctCall);
			continue;
		}

		if(pcur->IsFctDefName && Config.SplitFctDef)
		{
			Indent_MakeSplitStyle(pcur, Config.SplitFctDef);
			continue;
		}

		if(pcur->IsFctDeclName && Config.SplitFctDecl)
		{
			Indent_MakeSplitStyle(pcur, Config.SplitFctDecl);
			continue;
		}
	}

	Indent_AlignTypeNamesFunc(pfile);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SplitDecl(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	token	*pfirst;
	token	*pcur;
	token	*pnext;
	token	*pprev;
	token	*ptype1, *ptype2;
	token	*ppnext, *ppprev;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	if(!Config.SplitDecl) return;
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(!pcur->DeclName) continue;
		if(pcur->IsFctDeclName) continue;
		if(pcur->IsFctDefName) continue;
		if(pcur->InParen) continue;
		if(!pcur->InFct && !pcur->InClass) continue;	/* added by TT. A bit of a crude hack... */
		pfirst = pcur;

		if(pcur->DeclName)
		{
			ptype1 = NULL;
			pprev = pcur;
			ptype2 = pcur;
			while(pprev && PrevToken(pprev) && (PrevToken(pprev)->IsType) || IsAComment(PrevToken(pprev)))
			{
				pprev = PrevToken(pprev);
				ptype1 = pprev;
			}

			for(;;)
			{
				pcur->NoConcat = 1;
				pnext = pcur;
				while(pnext->i_ID != TOKEN_COMMA && pnext->i_ID != TOKEN_SEMICOL)
				{
					if(pnext->i_ID == TOKEN_LPAREN) pnext = Tool_ToRelationNext(pnext);
					if(pnext->i_ID == TOKEN_LARRAY) pnext = Tool_ToRelationNext(pnext);
					if(pnext->i_ID == TOKEN_LESS) pnext = Tool_ToRelationNext(pnext);
					if(pnext->i_ID == TOKEN_LBRACE) pnext = Tool_ToRelationNext(pnext);
					pnext = NextToken(pnext);
				}

				if(pnext->i_ID == TOKEN_SEMICOL) break;

				Tool_ForceEOLAfter(pnext, 1);
				pnext->NoConcat = 1;
				pcur = NextToken(pnext);

				if(Config.SplitDecl == 2 && ptype1)
				{
					pnext->i_ID = TOKEN_SEMICOL;
					free(pnext->pc_Value);
					pnext->pc_Value = strdup(";");

					pprev = ptype1;
					do
					{
						Tool_InsertTokenAfter(pfile, pnext, pprev->i_ID);
						ppnext = pnext->pst_Next->pst_Next;
						ppprev = pnext->pst_Next->pst_Prev;
						memcpy(pnext->pst_Next, pprev, sizeof(token));
						pnext->pst_Next->pst_Next = ppnext;
						pnext->pst_Next->pst_Prev = ppprev;
						pnext->pst_Next->pc_Value = strdup(pnext->pst_Next->pc_Value);
						pnext = NextToken(pnext);
						pprev = NextToken(pprev);
						while(pprev && pprev != ptype2 && (IsAComment(pprev) || pprev->i_ID == TOKEN_STAR))
							pprev = NextToken(pprev);
					} while(pprev != ptype2);

					if(!pnext->ForceSpaceAfter) pnext->ForceSpaceAfter = 1;
					pnext = NextToken(pnext);
				}

				pcur->NoConcat = 1;
				Tool_ComputeColLine(pcur);
				pcur->IndentLevel += (pfirst->Col - pcur->Col) / Config.TabSize;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_Constructor(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	token	*pref, *pend;
	token	*pend1;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		if
		(
			pcur->i_ID == TOKEN_COLON
		&&	!pcur->InFct
		&&	!pcur->DefineCst
		&&	!pcur->InPP
		&&	!pcur->InParen
		&&	!pcur->StmtLevel
		)
		{
			pref = Tool_PrevValid(pcur);
			if(pref && PrevToken(pcur) && !PrevToken(pcur)->LabelDecl)
			{
				if(Config.ConstructStyle == 1)
				{
					pcur->ForceEOLAfter = 0;
					pcur->ForceSpaceAfter = 1;
				}
				else
				{
					Tool_ForceEOLAfterComments(pcur, 1);
				}

				pnext = pcur;
				while(pnext && (pnext->i_ID != TOKEN_LBRACE))
				{
					if(Config.ConstructStyle != 2)
						pnext->IndentLevel = pref->IndentLevel + 1;
					else
						pnext->IndentLevel = pref->IndentLevel;
					pnext = NextToken(pnext);
					if(!pnext) break;

					/* Template */
					if(pnext->i_ID == TOKEN_LESS)
					{
						pnext->ForceSpaceAfter = 0;
						pend1 = Tool_ToRelationNext(pnext);
						while(pnext != pend1)
						{
							pnext->ForceEOLAfter = 0;
							pnext = NextToken(pnext);
						}

						pnext->pst_Prev->ForceSpaceAfter = 0;
						pnext = NextToken(pnext);
					}

					if(pnext->i_ID == TOKEN_LPAREN)
					{
						pend = Tool_ToRelationNext(pnext);
						while(pnext && pnext != pend)
						{
							pnext->ForceEOLAfter = 0;
							pnext = NextToken(pnext);
						}
					}

					if(pnext && pnext->i_ID == TOKEN_COMMA)
					{
						pnext->pst_Prev->ForceSpaceAfter = 0;
						if(Config.ConstructStyle == 1)
						{
							pnext->ForceSpaceAfter = 1;
							pnext->ForceEOLAfter = 0;
						}
						else
							Tool_ForceEOLAfterComments(pnext, 1);
					}
					else if(NextToken(pnext) && NextToken(pnext)->i_ID != TOKEN_LBRACE && pnext->i_ID != TOKEN_LBRACE)
					{
						pnext->ForceEOLAfter = 0;
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
void Indent_StaticInit(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~*/
	token	*peq;
	token	*pcur;
	token	*pcur1;
	token	*pend;
	int		col, col1;
	/*~~~~~~~~~~~~~~*/

	if(!Config.StaticInit) return;

	/* Here, everything has been splitted */
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->i_ID != TOKEN_AFFECT) continue;
		if(!pcur->InitDecl) continue;
		peq = pcur;
		pcur = Tool_NextValid(pcur);
		if(pcur->i_ID != TOKEN_LBRACE) continue;
		pend = Tool_ToRelationNext(pcur);

		switch(Config.StaticInit)
		{
		/* Restore as before */
		case 1:
			while(peq != pcur)
			{
				peq->ForceEOLAfter = (char) peq->WasEOLAfter;
				peq->ForceSpaceAfter = (char) peq->WasSpaceAfter;
				peq = NextToken(peq);
			}

			while(pcur != pend)
			{
				pcur->ForceEOLAfter = (char) pcur->WasEOLAfter;
				pcur->ForceSpaceAfter = (char) pcur->WasSpaceAfter;
				pcur = NextToken(pcur);
			}
			break;

		/* All on the same line */
		case 2:
			while(peq != pcur)
			{
				if(peq->ForceEOLAfter)
				{
					peq->ForceEOLAfter = 0;
					peq->ForceSpaceAfter = 1;
				}

				peq = NextToken(peq);
			}

			while(pcur != pend)
			{
				if(pcur->ForceEOLAfter)
				{
					pcur->ForceEOLAfter = 0;
					pcur->ForceSpaceAfter = 1;
				}

				pcur = NextToken(pcur);
			}
			break;

		/* The more that we can on the same line */
		case 3:
			pcur = Tool_NextValid(pcur);
			while(pcur != pend)
			{
				while(!pcur->ForceEOLAfter && pcur != pend) pcur = NextToken(pcur);
				if(pcur == pend) break;
				col = pcur->Col + strlen(pcur->pc_Value);
recom:
				pcur1 = Tool_NextValid(pcur);
				if(pcur1 == pend) break;
				col1 = 0;
				while(!pcur1->ForceEOLAfter && pcur1 != pend)
				{
					col1 += strlen(pcur1->pc_Value);
					col1 += pcur1->ForceSpaceAfter;
					pcur1 = NextToken(pcur1);
				}

				if(pcur1 == pend) break;
				col1 += strlen(pcur1->pc_Value);
				if(col + col1 + 1 < Config.LineLenSplitInit)
				{
					pcur->ForceEOLAfter = 0;
					pcur->ForceSpaceAfter = 1;
					col += col1 + 1;
					pcur = pcur1;
					goto recom;
				}

				pcur = pcur1;
			}
			break;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_EOL(FileDes *pfile)
{
	Indent_SplitStaticInitStruct(pfile);
	Indent_AlignBodyFctInlineClass(pfile);
	Indent_AlignTypeNamesGlobal(pfile);
	Indent_AlignBodyFctInlineClass(pfile);
	Indent_SplitTooLong(pfile);
	Indent_AlignTypeNamesFunc(pfile);
	Indent_PPBreak(pfile);
	Indent_AlignEnum(pfile);
	Indent_AlignParenToName(pfile);
	Indent_AlignDefine(pfile);
	Indent_BoolSplit(pfile);
	Indent_SplitStyle(pfile);
	Indent_SplitDecl(pfile);
	Indent_Constructor(pfile);
	Indent_StaticInit(pfile);
}
