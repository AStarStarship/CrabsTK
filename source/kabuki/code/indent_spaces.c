/*$T \Sources/indent_spaces.c GC 1.150 2011-09-22 20:52:18 */


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
void Indent_SpacesBase(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	token	*pend;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur && *pcur->pc_Value; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent)
		{
			pcur->ForceSpaceAfter = (char) pcur->WasSpaceAfter;
			continue;
		}

		if(pcur->IsFctDefName && Config.SpaceFirstFctDef)
		{
			pnext = Tool_SearchToken(pcur, TOKEN_LPAREN);
			goto comfp;
		}

		if(pcur->IsFctDefName && Config.SpaceLastFctDef)
		{
			pnext = Tool_SearchToken(pcur, TOKEN_LPAREN);
			pnext = Tool_ToRelationNext(pnext);
			goto comfp1;
		}

		if(pcur->IsFctDeclName && Config.SpaceFirstFctDecl)
		{
			pnext = Tool_SearchToken(pcur, TOKEN_LPAREN);
			goto comfp;
		}

		if(pcur->IsFctDeclName && Config.SpaceLastFctDecl)
		{
			pnext = Tool_SearchToken(pcur, TOKEN_LPAREN);
			pnext = Tool_ToRelationNext(pnext);
			goto comfp1;
		}

		if(pcur->IsFctCall && Config.SpaceFirstCall)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LPAREN)
			{
comfp:
				pend = Tool_NextValid(pnext);
				if(pend->i_ID != TOKEN_RPAREN) pnext->ForceSpaceAfter = 1;
			}
		}

		if(pcur->IsFctCall && Config.SpaceLastCall)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LPAREN)
			{
				pnext = Tool_ToRelationNext(pnext);
comfp1:
				pend = Tool_PrevValid(pnext);
				if(pend->i_ID != TOKEN_LPAREN) pend->ForceSpaceAfter = 1;
			}
		}

		if(pcur->i_ID == TOKEN_LPAREN && Config.SpaceParen && pcur->InParen <= Config.SpaceParen)
		{
			if(!pcur->IsCast)
			{
				pnext = Tool_ToRelationNext(pcur);
				if(pcur->pst_Next != pnext) Tool_ForceSpaceAfter(pcur, 1);
			}
		}

		if(pcur->i_ID == TOKEN_RPAREN && Config.SpaceParen && pcur->InParen <= Config.SpaceParen)
		{
			if(!pcur->IsCast)
			{
				pnext = Tool_ToRelationPrev(pcur);
				if(pcur->pst_Prev != pnext) Tool_ForceSpaceBefore(pcur, 1);
			}
		}

		if(Config.SpaceCast && pcur->i_ID == TOKEN_LPAREN && pcur->IsCast) Tool_ForceSpaceAfter(pcur, 1);
		if(Config.SpaceCast && pcur->i_ID == TOKEN_RPAREN && pcur->IsCast) Tool_ForceSpaceBefore(pcur, 1);

		/* Spaces between two words */
		if(Lisword(pcur->pc_Value[strlen(pcur->pc_Value) - 1]))
		{
			if((pcur->pst_Next) && (Lisword(*pcur->pst_Next->pc_Value))) Tool_ForceSpaceAfter(pcur, 1);
			if((pcur->pst_Next) && (pcur->pst_Next->i_ID == TOKEN_STRINGCST)) Tool_ForceSpaceAfter(pcur, 1);
		}

		/* Case */
		if(SpecWord(pcur, TOKEN_W_CASE))
		{
			Tool_ForceSpaceAfter(pcur, 1);
		}

		if(SpecWord(pcur, TOKEN_W_RETURN))
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID != TOKEN_SEMICOL && pnext->i_ID != TOKEN_LPAREN) Tool_ForceSpaceAfter(pcur, 1);
		}

		/* Type (. This is for example a function definition) */
		if(pcur->IsType && !pcur->IsFctDefName && !pcur->IsFctDeclName)
		{
			pnext = NextToken(pcur);
			if((pnext && pnext->i_ID == TOKEN_LPAREN) || (pcur->i_ID == TOKEN_STRINGCST))
			{
				pcur->ForceSpaceAfter = 1;
			}
		}

		/* Special keywords */
		if
		(
			Config.SpaceIf
		&&	(
				(SpecWord(pcur, TOKEN_W_IF))
			||	(SpecWord(pcur, TOKEN_W_WHILE))
			||	(SpecWord(pcur, TOKEN_W_FOR))
			||	(SpecWord(pcur, TOKEN_W_SWITCH))
			)
		)
		{
			Tool_ForceSpaceAfter(pcur, 1);
		}

		/* Function calls */
		if
		(
			Config.SpaceFctCall
		&&	(pcur->IsFctCall || (SpecWord(pcur, TOKEN_W_SIZEOF)) || (SpecWord(pcur, TOKEN_W_RETURN)))
		) Tool_ForceSpaceAfter(pcur, 1);

		/* Function decl */
		if(Config.SpaceFctDecl && (pcur->IsFctDeclName)) Tool_ForceSpaceAfter(pcur, 1);
		if(Config.SpaceFctDef && (pcur->IsFctDefName)) Tool_ForceSpaceAfter(pcur, 1);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_StmtIndentLevel(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext;
	int		level;
	/*~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(FirstOnLine(pcur))
		{
			/* Force paren with the same indent level */
			if(pcur->i_ID == TOKEN_LPAREN)
			{
				pnext = Tool_ToRelationNext(pcur);
				if(pnext) pnext->IndentLevel = pcur->IndentLevel;
			}

			level = (pcur->StmtLevel * Config.TabSize) + (pcur->IndentLevel * Config.TabSize);
			if(pcur->pst_Prev) level += pcur->pst_Prev->AddSpaceAfter;
			Tool_ForceSpaceBefore(pcur, level);
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_SpacesOper(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev, *pnext, *pend;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		switch(pcur->i_ID)
		{
		case TOKEN_AFFECT:
			pprev = Tool_PrevValid(pcur);
			if(!Config.AffectStyle) Tool_ForceSpaceBefore(pcur, 1);
			if(!pprev || pprev->i_SubSubID != TOKEN_WW_OPERATOR)
			{
				if(Config.AffectStyle != 2) Tool_ForceSpaceAfter(pcur, 1);
			}
			break;

		case TOKEN_AUTOOP:
			pprev = Tool_PrevValid(pcur);
			if(!Config.AutoAffectStyle) Tool_ForceSpaceBefore(pcur, 1);
			if(!pprev || pprev->i_SubSubID != TOKEN_WW_OPERATOR)
			{
				if(Config.AutoAffectStyle != 2) Tool_ForceSpaceAfter(pcur, 1);
			}
			break;

		case TOKEN_COMPARE:
		case TOKEN_OPER:
		case TOKEN_LINK:
		case TOKEN_OR:
		case TOKEN_QUESTION:
			pprev = Tool_PrevValid(pcur);
			Tool_ForceSpaceBefore(pcur, 1);
			if(!pprev || pprev->i_SubSubID != TOKEN_WW_OPERATOR)
			{
				Tool_ForceSpaceAfter(pcur, 1);
			}
			break;

		case TOKEN_DOUBLECOLON:
			if(Config.SpaceScopeDecl && pcur->StmtLevel == 0)
			{
				Tool_ForceSpaceBefore(pcur, 1);
				Tool_ForceSpaceAfter(pcur, 1);
				break;
			}

			if(Config.SpaceScopeUse && pcur->StmtLevel > 0)
			{
				Tool_ForceSpaceBefore(pcur, 1);
				Tool_ForceSpaceAfter(pcur, 1);
				break;
			}
			break;

		case TOKEN_COLON:
			pprev = Tool_PrevValid(pcur);
			if(!SpecWord(pprev, TOKEN_W_DEFAULT))
			{
				if(!pprev->LabelDecl)
				{
					if(!pcur->ColCase) Tool_ForceSpaceBefore(pcur, 1);
				}
			}

			Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_COMMA:
			Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_STAR:
			pprev = Tool_PrevValid(pcur);
			if(pprev && pprev->i_ID == TOKEN_STAR) break;
			if(pcur->UnaryOp) break;

		case TOKEN_SIGN:
			pnext = NextToken(pcur);
			if(pnext->i_ID == TOKEN_AUTOSIGN) Tool_ForceSpaceAfter(pcur, 1);
			goto addr;

		case TOKEN_ADDRESS:
			addr : pprev = Tool_PrevValid(pcur);
			if
			(
				(pprev)
			&&	(pprev->i_ID != TOKEN_LPAREN)
			&&	(pprev->i_ID != TOKEN_LARRAY)
			&&	(pprev->i_ID != TOKEN_UNCHANGE)
			) Tool_ForceSpaceBefore(pcur, 1);
			if(pcur->BinaryOp) Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_RARRAY:
			pnext = NextToken(pcur);
			if(pnext && pnext->i_ID == TOKEN_WORD) Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_LBRACE:
			Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_RBRACE:
			Tool_ForceSpaceBefore(pcur, 1);
			break;

		case TOKEN_SEMICOL:
			pnext = NextToken(pcur);
			if(pcur->InParen)
			{
				if(!pnext || (pnext->i_ID == TOKEN_RPAREN)) break;
			}

			if(pnext->i_ID == TOKEN_SEMICOL) break;
			Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_LESS:
			if(!pcur->IsType)	/* C++ template */
			{
				if(!pcur->InTemplate)
				{
					Tool_ForceSpaceAfter(pcur, 1);
					Tool_ForceSpaceBefore(pcur, 1);
				}
			}
			break;

		case TOKEN_GREAT:
			if(!pcur->IsType)	/* C++ template */
			{
				if(!pcur->InTemplate) Tool_ForceSpaceBefore(pcur, 1);
			}

			pnext = NextToken(pcur);
			if(pnext && pnext->i_ID != TOKEN_DOUBLECOLON && pnext->i_ID != TOKEN_LARRAY && pnext->i_ID != TOKEN_SEMICOL)
				Tool_ForceSpaceAfter(pcur, 1);
			break;

		case TOKEN_LPAREN:
			if(pcur->IsCast)
			{
				pprev = PrevToken(pcur);
				if(pprev->i_ID != TOKEN_LPAREN && pprev->i_ID != TOKEN_LARRAY && !pprev->UnaryOp)
				{
					Tool_ForceSpaceBefore(pcur, 1);
				}
			}
			break;

		case TOKEN_RPAREN:
			if(!Config.SpaceCastAfter && pcur->IsCast) break;
			pnext = NextToken(pcur);
			switch(pnext->i_ID)
			{
			case TOKEN_WORD:
			case TOKEN_LPAREN:
			case TOKEN_NUMBER:
			case TOKEN_SPECWORD:
			case TOKEN_STRINGCST:
			case TOKEN_CHARCST:
				Tool_ForceSpaceAfter(pcur, 1);
				break;
			}
			break;
		}
	}

	if(!Config.SpaceInCall)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if(pcur->NoIndent) continue;
			if(pcur->IsFctCall)
			{
				pnext = Tool_NextValid(pcur);
				if(pnext->i_ID == TOKEN_LPAREN)
				{
					pnext = Tool_NextValid(pnext);
					while(pnext->i_ID != TOKEN_RPAREN)
					{
						pend = Tool_ToRelationNext(pnext);
						if(pend) pnext = pend;
						if(pnext->i_ID == TOKEN_COMMA) pnext->ForceSpaceAfter = 0;
						pnext = Tool_NextValid(pnext);
					}

					pcur = pnext;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignTypeNamesCom(token *pdeb, token *pend)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pnext, *pprev, *pmemo, *pdebi, *pcur, *pnewend;
	int		maxcol, mode, max1, mincol;
	int		fctmode;
	int		nextcol;
	int		tt;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	pdebi = pdeb;

	/*
	 * 2 modes : First one (0) is to get max column. Second one (1) is to indent to
	 * the max column.
	 */
recom:
	pdeb = pdebi;
	pdebi = NULL;
	pnewend = NULL;
	fctmode = 0;
	maxcol = 0;
	mincol = 0;
	for(mode = 0; mode <= 1; mode++)
	{
		pnext = pdeb;
		while(pnext)
		{
			pcur = pnext;

			/* We don't want word of struct, enum, union, class to be align */
			if
			(
				(pnext->i_SubSubID == TOKEN_WW_UNION)
			||	(pnext->i_SubSubID == TOKEN_WW_STRUCT)
			||	(pnext->i_SubSubID == TOKEN_WW_ENUM)
			)
			{
				pnext = Tool_NextValid(pnext);
				if(pnext->i_ID == TOKEN_WORD) pnext = Tool_NextValid(pnext);
			}

			while(pnext->i_SubID == TOKEN_W_DECL || (pnext->IsType && !pnext->DeclName))
			{
				pnext = NextToken(pnext);
			}

			if(*pnext->pc_Value == ']')
			{
				pnext = pnext->pst_Prev;
				if(*pnext->pc_Value == '[') pnext = pnext->pst_Prev;
			}
			else
			{
				if(!pnext->DeclName && pnext->i_ID != TOKEN_AFFECT) break;
			}

			/* Zap function parameters */
			pmemo = pnext;
			if(pnext->IsFctDefName || pnext->IsFctDeclName)
			{
				pmemo = Tool_NextValid(pmemo);
				pmemo = Tool_ToRelationNext(pmemo);
			}

			/* Align star and address at the same level as the name */
			pprev = Tool_PrevValid(pnext);
			if(pprev && ((pprev->i_ID == TOKEN_STAR) || (pprev->i_ID == TOKEN_ADDRESS) || pprev->UnClassDes))
			{
				while((pprev->i_ID == TOKEN_STAR) || (pprev->i_ID == TOKEN_ADDRESS) || pprev->UnClassDes)
				{
					pprev = PrevToken(pprev);
				}

				pnext = NextToken(pprev);
			}

			if(Config.LeftDecl && ((pnext->i_ID == TOKEN_STAR) || (pnext->i_ID == TOKEN_ADDRESS)))
			{
				while((pnext->i_ID == TOKEN_STAR) || (pnext->i_ID == TOKEN_ADDRESS)) pnext = NextToken(pnext);
			}

			/* Pass to next decl if current one is invalid (no name to indent) */
			if
			(
				(!Config.LeftDecl && ((pnext->i_ID == TOKEN_STAR) || (pnext->i_ID == TOKEN_ADDRESS)))
			||	(pnext->i_ID == TOKEN_WORD)
			||	(pnext->UnClassDes)
			)
			{
				/* Get max col */
				if(mode == 0)
				{
					nextcol = pnext->Col;
					if(PrevToken(pnext)) nextcol -= PrevToken(pnext)->ForceSpaceAfter;
					if(nextcol >= maxcol)
					{
						max1 = nextcol;
						max1 = Tool_ToTab(max1);
						if(max1 <= nextcol) max1 += Config.TabSize;
						if(mincol && (max1 - mincol > Config.MaxAlignSpaces))
						{
							pnewend = pnext;
							pdebi = pcur;
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
						if(maxcol && ((maxcol - max1) > Config.MaxAlignSpaces))
						{
							pnewend = pnext;
							pdebi = pcur;
							break;
						}

						if(!mincol || (max1 < mincol)) mincol = maxcol;
					}
				}

				/* Indent to common col */
				else
				{
					nextcol = pnext->Col;
					if(PrevToken(pnext)) nextcol -= PrevToken(pnext)->ForceSpaceAfter;
					if(pnext == pnewend) break;
					if(nextcol < maxcol)
					{
						tt = maxcol - nextcol;
						if(tt > Config.MaxAlignSpaces)
						{
							while(tt > Config.MaxAlignSpaces)
							{
								tt -= Config.TabSize;
							}
						}

						if(tt <= 0) tt = 1;

						/* We don't want word of struct, enum, union, class to be align */
						if
						(
							pprev
						&&	(
								(pprev->i_SubSubID == TOKEN_WW_UNION)
							||	(pprev->i_SubSubID == TOKEN_WW_STRUCT)
							||	(pprev->i_SubSubID == TOKEN_WW_ENUM)
							)
						)
						{
							tt = 1;
						}

						Tool_ForceSpaceBefore(pnext, tt);
					}
				}
			}

			/* Get the next type to indent */
			pnext = pmemo;
			while(pnext && !pnext->BeginDecl && (pnext != pend))
			{
				if(pnext->IsFctDefName || pnext->IsFctDeclName)
				{
					pnext = Tool_NextValid(pnext);
					pnext = Tool_ToRelationNext(pnext);
				}

				pnext = NextToken(pnext);
			}

			if(pnext == pend) break;
		}

		/* To begin the mode 2 */
		pnext = pdeb;
	}

	if(pdebi)
	{
		goto recom;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignTypeNamesGlobal(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pnext;
	token	*pnext1, *ptmp, *pbeg;
	token	*pnewend;
	char	meetparen;
	char	meetfct;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Indent declarations */
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->BegStmtDecl)
		{
			if(!pcur->InFctDef || pcur->InClass)
			{
				pend = pcur;
bcle:
				pnewend = NULL;
				pbeg = pend;
				meetparen = 0;
				meetfct = 0;
				while(pend && !pend->EndStmtDecl)
				{
					if(pend->i_ID == TOKEN_LPAREN) meetparen = 1;
					if(pend->IsFctDefName && pcur->InClass) meetfct = 1;
					pend = NextToken(pend);
					if(pend && pend->UnAlignCmt) break;
				}

				if(!pend) break;

				/* An inline in a class */
				if(pend->i_ID == TOKEN_LBRACE)
				{
					if(!FirstOnLine(pend) && meetparen)
					{
						pnext = Tool_ToRelationNext(pend);
						pnext = Tool_NextValid(pnext);
						while(pnext && pnext->i_ID == TOKEN_SEMICOL) pnext = Tool_NextValid(pnext);
						if(pnext->BegStmtDecl)
						{
							pend = pnext;
							goto bcle;
						}
					}
					else if(meetfct)
					{
						pnewend = pend;
						pend = pbeg;
						goto zapend;
					}
				}

				/* A normal function, but with a following inline */
				if(pend->i_SubID == TOKEN_W_DECL && pend->i_SubSubID == TOKEN_WW_CONST) pend = Tool_NextValid(pend);
				if(pend->i_ID == TOKEN_AFFECT)
				{
					pnext1 = Tool_NextValid(pend);
					if((pnext1->i_ID == TOKEN_NUMBER) && (pnext1->pc_Value[0] == '0') && (pnext1->pc_Value[1] == 0))
						pend = Tool_NextValid(pnext1);
				}

				if(pend->i_ID == TOKEN_SEMICOL)
				{
					pnext = pend;
					do
					{
						if(pnext->EndAlign) break;
						if(pnext->CmtLevel) break;
						pnext = NextToken(pnext);
					} while(pnext && ((pnext->i_ID == TOKEN_CCMT) || (pnext->i_ID == TOKEN_CPPCMT)));

					if(pnext->BegStmtDecl && !pnext->EndAlign)
					{
						if(!pnext->InFctDef || pnext->InClass)
						{
							pend = pnext;
							goto bcle;
						}
					}
				}

zapend:
				if(pcur != pend)
				{
					ptmp = pcur;
					while(ptmp->pst_Prev && !ptmp->pst_Prev->ForceEOLAfter) ptmp = ptmp->pst_Prev;
					while(ptmp != pend)
					{
						Tool_ComputeColLine(ptmp);
						while(ptmp && !ptmp->ForceEOLAfter && ptmp != pend) ptmp = ptmp->pst_Next;
						if(!ptmp) break;
						if(ptmp == pend) break;
						ptmp = ptmp->pst_Next;
					}

					Indent_AlignTypeNamesCom(pcur, pend);
					pcur = pend;
				}

				if(pnewend) pcur = pnewend;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignBodyFct(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pcurbeg, *pcurend, *pnext, *pend, *pmemo;
	int		maxcol, mode;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Indent declarations */
	pcurend = NULL;
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->IsFctDefName && pcur->AlignBody)
		{
			pmemo = pcur;

			/* ( */
			pcur = Tool_NextValid(pcur);
			pcur = Tool_ToRelationNext(pcur);

			/* { */
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LBRACE)
			{
				pend = Tool_ToRelationNext(pnext);
				pnext->pst_Prev->ForceEOLAfter = 0;
				pnext->pst_Prev->ForceSpaceAfter = 1;
				while(pnext != pend)
				{
					pnext->StmtLevel = pmemo->StmtLevel;
					pnext->IndentLevel = pmemo->IndentLevel;
					pnext->ForceEOLAfter = 0;
					pnext = NextToken(pnext);
					pnext->NoBreak = 1;
				}

				Tool_ForceEOLAfter(pnext, 1);

				/* Delete empty line before */
				pmemo->UnSplitFctDef = 1;
				pmemo = Tool_ToBegLine(pmemo);
				pmemo = PrevToken(pmemo);
				if(pmemo->ForceEOLAfter > 1)
				{
					if(!Config.KeepEmptyLinesCode || pmemo->WasEOLAfter == 1) pmemo->ForceEOLAfter = 1;
				}
			}
		}
	}

	/* Align function body if inline */
	Tool_ComputeCol(pfile);
	maxcol = 0;
	mode = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pcurbeg = pcur;
recom:
		if(pcur->IsFctDefName && pcur->AlignBody)
		{
			pmemo = pcur;

			/* ( */
			pcur = Tool_NextValid(pcur);
			pcur = Tool_ToRelationNext(pcur);

			/* { */
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LBRACE)
			{
				pend = Tool_ToRelationNext(pnext);
				if(mode == 0) mode = 1;
				if(mode == 1)
				{
					if(pcur->Col >= maxcol)
					{
						maxcol = pcur->Col;
						maxcol = Tool_ToTab(maxcol);
						if(maxcol <= pcur->Col + 2) maxcol += Config.TabSize;
					}
				}
				else
				{
					pcur->ForceSpaceAfter = (char) (maxcol - pcur->Col - strlen(pcur->pc_Value));
				}

				pcur = NextToken(pend);
				if(pcur->i_ID == TOKEN_SEMICOL) pcur = NextToken(pcur);
				while(pcur && (pcur->IsType || pcur->i_ID == TOKEN_WORD) && !pcur->IsFctDefName)
					pcur = NextToken(pcur);
				goto recom;
			}
		}

		if(mode == 1)
		{
			pcurend = pcur;
			mode = 2;
			pcur = pcurbeg->pst_Prev;
		}
		else if(mode == 2)
		{
			mode = 0;
			pcur = pcurend;
			maxcol = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignBodyFctInlineClass(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pcurbeg, *pcurend, *pnext, *pend, *pmemo;
	int		maxcol, mode;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Fct body */
	Indent_AlignBodyFct(pfile);

	/* Indent declarations */
	pcurend = NULL;
	if(Config.ConcatInlineClass)
	{
		Tool_ComputeCol(pfile);
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if(pcur->IsFctDefName && pcur->InClass)
			{
				pmemo = pcur;

				/* ( */
				pcur = Tool_NextValid(pcur);
				pcur = Tool_ToRelationNext(pcur);

				/* { */
				pnext = Tool_NextValid(pcur);
				if(pnext->i_ID == TOKEN_LBRACE)
				{
					pend = Tool_ToRelationNext(pnext);
					if(pcur->Col + Tool_LenTokens(pnext, pend) < Config.LineLenCode)
					{
						pnext->pst_Prev->ForceEOLAfter = 0;
						pnext->pst_Prev->ForceSpaceAfter = 1;
						while(pnext != pend)
						{
							pnext->StmtLevel = pmemo->StmtLevel;
							pnext->IndentLevel = pmemo->IndentLevel;
							pnext->ForceEOLAfter = 0;
							pnext = NextToken(pnext);
							pnext->NoBreak = 1;
						}

						Tool_ForceEOLAfter(pnext, 1);

						/* Delete empty line before */
						pmemo->UnSplitFctDef = 1;
						pmemo = Tool_ToBegLine(pmemo);
						pmemo = PrevToken(pmemo);
						if(pmemo->ForceEOLAfter > 1)
						{
							if(!Config.KeepEmptyLinesCode || pmemo->WasEOLAfter == 1) pmemo->ForceEOLAfter = 1;
						}
					}
				}
			}
		}
	}

	/* Align function body if inline */
	Tool_ComputeCol(pfile);
	maxcol = 0;
	mode = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pcurbeg = pcur;
recom:
		if(pcur->IsFctDefName && pcur->InClass)
		{
			pmemo = pcur;

			/* ( */
			pcur = Tool_NextValid(pcur);
			pcur = Tool_ToRelationNext(pcur);

			/* { */
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LBRACE)
			{
				pend = Tool_ToRelationNext(pnext);
				if(pcur->Col + Tool_LenTokens(pnext, pend) < Config.LineLenCode)
				{
					if(mode == 0) mode = 1;
					if(mode == 1)
					{
						if(pcur->Col >= maxcol)
						{
							maxcol = pcur->Col;
							maxcol = Tool_ToTab(maxcol);
							if(maxcol <= pcur->Col + 2) maxcol += Config.TabSize;
						}
					}
					else
					{
						pcur->ForceSpaceAfter = (char) (maxcol - pcur->Col - strlen(pcur->pc_Value));
					}

					pcur = NextToken(pend);
					if(pcur->i_ID == TOKEN_SEMICOL) pcur = NextToken(pcur);
					while(pcur && (pcur->IsType || pcur->i_ID == TOKEN_WORD) && !pcur->IsFctDefName)
						pcur = NextToken(pcur);
					goto recom;
				}
			}
		}

		if(mode == 1)
		{
			pcurend = pcur;
			mode = 2;
			pcur = pcurbeg->pst_Prev;
		}
		else if(mode == 2)
		{
			mode = 0;
			pcur = pcurend;
			maxcol = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignTypeNamesFunc(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pword, *pnext;
	int		col;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Indent inside functions decl and defs */
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if((pcur->IsFctDeclName) || (pcur->IsFctDefName))
		{
			pword = pcur;

			/* Get ( */
			pcur = Tool_NextValid(pcur);
			if(FirstOnLine(pcur) && LastOnLine(pcur))
			{
				pend = Tool_ToRelationNext(pcur);
				pnext = Tool_NextValid(pcur);
				Indent_AlignTypeNamesCom(pnext, pend);

				/* Force indentation level to be the same as name for function decl/def. */
				col = pword->Col / Config.TabSize;
				col -= (pcur->IndentLevel + pcur->StmtLevel);
				Tool_IncIndentLevel(pcur, pend, col);
				pcur = pend;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignEnum(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pscan;
	int		ilen, imaxlen, imode;
	int		maxcol;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	imaxlen = 0;
	pscan = NULL;
	maxcol = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_SubSubID == TOKEN_WW_ENUM)
		{
			pcur = Tool_NextValid(pcur);
			while(pcur && pcur->i_ID != TOKEN_LBRACE) pcur = Tool_NextValid(pcur);
			if(!pcur) return;
			pcur = Tool_NextValid(pcur);
			for(imode = 0; imode < 2; imode++)
			{
				pscan = pcur;
				while(pscan->i_ID != TOKEN_RBRACE)
				{
					if(!imode)
					{
						ilen = strlen(pscan->pc_Value);
						if(ilen > imaxlen)
						{
							imaxlen = ilen;
							maxcol = pscan->Col + ilen;
							maxcol = Tool_ToTab(maxcol);
						}
					}
					else
					{
						if(pscan->pst_Next->i_ID == TOKEN_AFFECT)
						{
							Tool_ForceSpaceAfter(pscan, maxcol - (pscan->Col + strlen(pscan->pc_Value)));
						}
					}

					while(pscan && (pscan->i_ID != TOKEN_COMMA) && (pscan->i_ID != TOKEN_RBRACE))
						pscan = Tool_NextValid(pscan);
					if(pscan->i_ID == TOKEN_RBRACE) break;
					pscan = Tool_NextValid(pscan);
				}
			}

			pcur = pscan;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignParenToName(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pword;
	int		col;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Indent inside functions decl and defs */
	Tool_ComputeCol(pfile);
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Force indentation level to be the same as name. */
		if
		(
			(pcur->IsFctDeclName)
		||	(SpecWord(pcur, TOKEN_W_IF))
		||	(SpecWord(pcur, TOKEN_W_FOR))
		||	(SpecWord(pcur, TOKEN_W_WHILE))
		||	(SpecWord(pcur, TOKEN_W_SWITCH))
		||	((pcur->IsFctCall) && (pcur->Col < Tool_NextValid(pcur)->Col))
		)
		{
			pword = pcur;

			/* Get ( */
			pcur = Tool_NextValid(pcur);
			if(FirstOnLine(pcur) && LastOnLine(pcur))
			{
				pend = Tool_ToRelationNext(pcur);
				col = pword->Col / Config.TabSize;
				col -= (pcur->IndentLevel + pcur->StmtLevel);
				Tool_IncIndentLevel(pcur, pend, col);
				continue;
			}
		}

		/* For function definition, force indentlevel to be 0. */
		if(pcur->IsFctDefName)
		{
			pword = pcur;

			/* Get ( */
			pcur = Tool_NextValid(pcur);
			if(FirstOnLine(pcur) && LastOnLine(pcur))
			{
				col = pcur->IndentLevel;
				pend = Tool_ToRelationNext(pcur);
				Tool_IncIndentLevel(pcur, pend, -col);
				pcur = pend;
				continue;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_Spaces(FileDes *pfile)
{
	Indent_SpacesOper(pfile);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_AlignOperSpecial(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext, *pprev;
	token	*pend;
	/*~~~~~~~~~~~~~~~~~~~*/

	if(!Config.LeftDecl) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;

		if(pcur->IsCast && pcur->i_ID == TOKEN_LPAREN)
		{
			pend = Tool_ToRelationNext(pcur);
			while(pcur != pend)
			{
				if((pcur->i_ID == TOKEN_ADDRESS) || (pcur->i_ID == TOKEN_STAR)) pcur->pst_Prev->ForceSpaceAfter = 0;
				pcur = pcur->pst_Next;
			}

			continue;
		}

		if(pcur->i_ID == TOKEN_LESS)
		{
			pend = Tool_NextValid(pcur);
			if(pend->IsType)
			{
				pend = Tool_ToRelationNext(pcur);
				while(pcur != pend)
				{
					if((pcur->i_ID == TOKEN_ADDRESS) || (pcur->i_ID == TOKEN_STAR))
						pcur->pst_Prev->ForceSpaceAfter = 0;
					pcur = pcur->pst_Next;
				}
			}

			continue;
		}

		if(!pcur->IsType) continue;

		pnext = NULL;
		if(pcur->i_SubSubID == TOKEN_WW_CLASS)
			pcur = Tool_NextValid(pcur);
		else
		{
			while(pcur && pcur->IsType) pcur = pcur->pst_Next;
			pcur = pcur->pst_Prev;
			if(pcur->i_ID == TOKEN_DOUBLECOLON)
			{
				pprev = Tool_PrevValid(pcur);
				if(pprev->i_ID == TOKEN_WORD)
				{
					pnext = pcur;
					pcur = pcur->pst_Prev;
					pcur = pcur->pst_Prev;
				}
			}

			if(pcur) pcur = pcur->pst_Prev;
		}

		if(!pcur) break;
		pcur = Tool_NextValid(pcur);

		if((pcur->i_ID == TOKEN_ADDRESS) || (pcur->i_ID == TOKEN_STAR))
		{
			pcur->pst_Prev->ForceSpaceAfter = 0;
			while(pcur->i_ID == TOKEN_ADDRESS || pcur->i_ID == TOKEN_STAR) pcur = pcur->pst_Next;
			pcur->pst_Prev->ForceSpaceAfter++;
		}

		if(pnext) pcur = pnext;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_Switch(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend;
	token	*pnext;
	/*~~~~~~~~~~~~~~~~~*/

	if(Config.SwitchStyle == 0) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(SpecWord(pcur, TOKEN_W_SWITCH))
		{
			pcur = Tool_SearchToken(pcur, TOKEN_LBRACE);
			if((Config.SwitchStyle == 3) || (Config.SwitchStyle == 4))
			{
				pcur->pst_Prev->ForceEOLAfter = 0;
				pcur->pst_Prev->ForceSpaceAfter = 1;

				if(Config.SwitchStyle == 3)
				{
					continue;
				}
			}

			pend = Tool_ToRelationNext(pcur);
			if(Config.SwitchStyle == 5)
			{
				pcur->AddSpaceBefore = pend->AddSpaceBefore = (Config.TabSize + 1) / 2;
			}

			if(Config.SwitchStyle == 2)
			{
				pcur->StmtLevel++;
			}

			pnext = pcur->pst_Next;
			while(pnext != pend)
			{
				pnext->StmtLevel++;
				pnext = pnext->pst_Next;
			}

			if(Config.SwitchStyle == 2)
			{
				pnext->StmtLevel++;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_scast(FileDes *pfile)
{
	/*~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext;
	token	*pend;
	/*~~~~~~~~~~~*/

	pend = NULL;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if((pcur->i_ID == TOKEN_WORD) && (pcur->i_SubID == TOKEN_W_DECL) && (pcur->i_SubSubID == TOKEN_WW_SCAST))
		{
			pnext = pcur->pst_Next;
			if(pnext->i_ID == TOKEN_LESS)
			{
				pcur->ForceSpaceAfter = 0;
				if(Config.SpaceCast)
					pnext->ForceSpaceAfter = 1;
				else
					pnext->ForceSpaceAfter = 0;
				pend = Tool_ToRelationNext(pnext);
				while(pnext != pend)
				{
					pnext->IsType = 1;
					pnext = NextToken(pnext);
				}

				if(Config.SpaceCast)
					pend->pst_Prev->ForceSpaceAfter = 1;
				else
					pend->pst_Prev->ForceSpaceAfter = 0;
			}

			if(pend->pst_Next->i_ID == TOKEN_LPAREN) pend->ForceSpaceAfter = 0;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Indent_Return(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(SpecWord(pcur, TOKEN_W_RETURN))
		{
			pnext = NextToken(pcur);
			if(pnext->i_ID == TOKEN_LPAREN)
			{
				if(Config.SpaceReturn || (pcur->pst_Next && pcur->pst_Next->IsCast))
					pcur->ForceSpaceAfter = 1;
				else
					pcur->ForceSpaceAfter = 0;
			}
		}
	}
}
