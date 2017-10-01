/*$T \Sources/grammar_decl.c GC 1.150 2011-09-22 20:52:17 */


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
#include "os.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_InsideParen(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *pnext;
	token	*ptmp;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Add () around a return expression */
	if(Config.ReturnAddParen)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if(pcur->i_ID == TOKEN_SPECWORD && pcur->i_SubID == TOKEN_W_RETURN)
			{
				pnext = Tool_NextValid(pcur);
				if((pnext->i_ID != TOKEN_LPAREN) && (pnext->i_ID != TOKEN_SEMICOL))
				{
					pend = pnext;
					while(pend && pend->i_ID != TOKEN_SEMICOL)
					{
						if(pend->i_ID == TOKEN_LPAREN) pend = Tool_ToRelationNext(pend);
						pend = Tool_NextValid(pend);
					}

					Tool_InsertTokenAfter(pfile, pcur, TOKEN_LPAREN);
					Tool_InsertTokenBefore(pfile, pend, TOKEN_RPAREN);
					pcur = pend;
				}
			}
		}
	}

	/* Remove () around a return expression */
	else if(Config.ReturnDelParen)
	{
		for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
		{
			if(pcur->i_ID == TOKEN_SPECWORD && pcur->i_SubID == TOKEN_W_RETURN)
			{
recom:
				pnext = Tool_NextValid(pcur);
				if(pnext->i_ID == TOKEN_LPAREN)
				{
					pend = Tool_ToRelationNext(pnext);
					if(Tool_NextValid(pend)->i_ID == TOKEN_SEMICOL)
					{
						Tool_UnlinkToken(pfile, pnext);
						Tool_UnlinkToken(pfile, pend);
						goto recom;
					}
				}
			}
		}
	}

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if((pcur->i_ID == TOKEN_LPAREN) || ((pcur->i_ID == TOKEN_LESS) && (pcur->InTemplate)))
		{
			pend = Tool_ToRelationNext(pcur);
			if(pend != pcur->pst_Next)
			{
				pnext = pcur->pst_Next;

				/* Set InParen flag */
				ptmp = pnext;
				while(ptmp != pend)
				{
					ptmp->InParen++;
					ptmp = ptmp->pst_Next;
				}

				/* Save address of open paren token */
				while(pnext != pend)
				{
					pnext->OpenParen = pcur;
					pnext = NextToken(pnext);
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
token *Grammar_Affect(token *pcur)
{
	if(!pcur) return NULL;
	if(pcur->i_ID == TOKEN_AFFECT)
	{
		while((pcur) && (pcur->i_ID != TOKEN_COMMA) && (pcur->i_ID != TOKEN_SEMICOL) && (pcur->i_ID != TOKEN_RPAREN))
		{
			if(pcur->i_ID == TOKEN_GREAT && pcur->InTemplate) break;
			if(pcur->i_ID == TOKEN_LPAREN) pcur = Tool_ToRelationNext(pcur);
			if(pcur->i_ID == TOKEN_LBRACE) pcur = Tool_ToRelationNext(pcur);
			pcur = NextToken(pcur);
		}
	}

	return pcur;
}

/*
 =======================================================================================================================
    Grammar_IsTypeCorrect : Is this function STILL needed??
 =======================================================================================================================
 */
void Grammar_IsTypeCorrect(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur;
	struct token_	*pprev, *pnext;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->InASM) continue;
		if(pcur->i_ID == TOKEN_CCMT) continue;

		if(pcur->i_ID == TOKEN_LESS || pcur->i_ID == TOKEN_GREAT)
		{
			pprev = Tool_PrevValid(pcur);
			if(pprev->i_ID == TOKEN_WORD && pprev->i_SubSubID == TOKEN_WW_OPERATOR)
			{
				pcur->i_ID = TOKEN_WORD;
				pcur->i_SubID = 0;
				pcur->i_SubSubID = 0;
			}
		}

		if(pcur->i_ID == TOKEN_STAR)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LPAREN)
			{
				pprev = Tool_PrevValid(pcur);
				while(pprev->IsType)
				{
					if(pprev->i_ID == TOKEN_WORD && pprev->i_SubSubID == TOKEN_WW_OPERATOR)
					{
						pcur->i_ID = TOKEN_WORD;
						pcur->i_SubID = 0;
						pcur->i_SubSubID = 0;
						break;
					}

					pprev = Tool_PrevValid(pprev);
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_IsType(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *plastfirsttype = NULL;
	struct token_	*pprev, *pnext, *pmemo, *pword, *ptmp, *ptmp1;
	struct token_	*ptmp2, *pprev1;
	struct token_	*pnn, *pnn1, *pnn2;
	char			twowords, lasttwowords;
	char			cD1, cret;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	twowords = 0;
	lasttwowords = 0;
	cD1 = 0;
	cret = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->InASM) continue;
		if(pcur->i_ID == TOKEN_CCMT) continue;
		cret = 0;	/* does this need resetting? */

		if(pcur->i_ID != TOKEN_WORD)
		{
			if(pcur->i_ID == TOKEN_ELLIPSIS)
			{
				pcur->IsType = 1;
				continue;
			}

			if((pcur->i_ID == TOKEN_LPAREN) || (pcur->i_ID == TOKEN_LARRAY))
			{
				pcur = Tool_ToRelationNext(pcur);
			}

			/* To avoid pbs with static init (with commas) */
			if(pcur->i_ID == TOKEN_AFFECT)
			{
				ptmp = Tool_NextValid(pcur);
				if(ptmp->i_ID == TOKEN_LBRACE) pcur = Tool_ToRelationNext(ptmp);

				/* Static affect */
				pmemo = pcur;
				pcur = Grammar_Affect(pcur);
				if(pcur != pmemo) continue;
			}

			if
			(
				(pcur->i_ID != TOKEN_COMMA)
			&&	(pcur->i_ID != TOKEN_SEMICOL)
			&&	(pcur->i_ID != TOKEN_RPAREN)
			&&	(pcur->i_ID != TOKEN_RARRAY)
			&&	(pcur->i_ID != TOKEN_COLON)		/* C++ constructor */
			&&	(pcur->i_ID != TOKEN_LBRACE)	/* C++ constructor */
			&&	(pcur->i_ID != TOKEN_LESS)		/* Template */
			)
			{
				if(pcur->i_ID == TOKEN_GREAT)
				{
					pprev = Tool_PrevValid(pcur);
					if(pprev->i_ID == TOKEN_LESS) continue;
				}

				goto Error;
			}

			continue;
		}

		if(pcur->i_ID == TOKEN_WORD && pcur->i_SubID == TOKEN_W_DECL && pcur->i_SubSubID == TOKEN_WW_THROW)
			goto Error;

		pprev = Tool_PrevValid(pcur);
		if(pprev)
		{
			/* Get a PP directive before ? */
			if(!pcur->InPP && pprev->InPP)
			{
				while(pprev && pprev->InPP) pprev = Tool_PrevValid(pprev);
			}

			/* Check token before */
			if
			(
				(pprev)
			&&	(pprev->i_ID != TOKEN_LBRACE)
			&&	(pprev->i_ID != TOKEN_RBRACE)
			&&	(pprev->i_ID != TOKEN_SEMICOL)
			&&	((pprev->i_ID != TOKEN_COMMA) || (!pprev->InParen)) /* In function decl */
			&&	(pprev->i_ID != TOKEN_RPAREN)						/* Macro */
			&&	(pprev->i_ID != TOKEN_LPAREN)						/* In function decl */
			&&	(pprev->i_ID != TOKEN_COLON)						/* Cause of label */
			&&	(pprev->i_ID != TOKEN_LESS)			/* In template */
			&&	(pprev->i_ID != TOKEN_GREAT)		/* In template */
			)
			{
				if(pprev->i_ID == TOKEN_STRINGCST)
				{
					pprev1 = Tool_PrevValid(pprev);
					if((pprev1->i_SubID == TOKEN_W_DECL) && (pprev1->i_SubSubID == TOKEN_WW_EXTERN)) goto ok1;
				}

				goto Error;
			}

ok1:
			/* Are we sure it's a label ? */
			if(pprev && pprev->i_ID == TOKEN_COLON)
			{
				pprev = PrevToken(pprev);
				if(pprev && !pprev->LabelDecl)
				{
					goto Error;
				}
			}
		}

		/* Check token after */
		pnext = Tool_NextValid(pcur);
		if(!pnext)
		{
			goto Error;
		}

		if(pnext)
		{
			if
			(
				(pnext->i_ID != TOKEN_WORD)
			&&	(pnext->i_ID != TOKEN_STAR)
			&&	(pnext->i_ID != TOKEN_ADDRESS)
			&&	(pnext->i_ID != TOKEN_DOUBLECOLON)	/* C++ */
			&&	(pnext->i_ID != TOKEN_COMMA)		/* In function decl */
			&&	(pnext->i_ID != TOKEN_LPAREN)		/* Fct proto */
			&&	(pnext->i_ID != TOKEN_RPAREN)		/* Fct params */
			&&	(pnext->i_ID != TOKEN_LESS)			/* C++ template */
			&&	(pnext->i_ID != TOKEN_GREAT)		/* C++ template */
			)
			{
				if(pnext->i_ID == TOKEN_UNCHANGE && pnext->InClass)
				{
					pcur->IsType = 1;
					pnext->IsType = 1;
					pnext->UnClassDes = 1;
					pcur = pnext->pst_Next;
					continue;
				}

				/* Struct, enum ? */
				if(pnext->i_ID == TOKEN_LBRACE)
				{
					if(pcur->i_SubSubID == TOKEN_WW_ENUM)
					{
						pprev = Tool_ToRelationNext(pnext);
						Tool_SetFlag(pcur, pprev, &pcur->IsType, 1);
						pcur = pprev;
						continue;
					}
				}

				if((pcur->i_SubID == TOKEN_W_DECL) && (pcur->i_SubSubID == TOKEN_WW_EXTERN))
				{
					pmemo = pcur;
					pword = pnext;
					pnext->IsType = 1;
					goto ok;
				}

				goto Error;
			}

			if(pnext && pnext->i_ID == TOKEN_DOUBLECOLON)
			{
				pprev = Tool_PrevValid(pcur);
				if(pprev && !pprev->IsType && pcur->StmtLevel)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext && pnext->i_ID == TOKEN_WORD)
					{
						pnext = Tool_NextValid(pnext);
						if(pnext && pnext->i_ID == TOKEN_WORD)
						{
							goto kkk;
						}
					}

					/* AA::AA<jjj> */
					if(pnext && pnext->i_ID == TOKEN_LESS && !pcur->InParen) goto kkk;

					goto Error;
				}
			}
		}

kkk:
		pmemo = pcur;

		/* Zap all words */
		pnext = Tool_NextValid(pcur);
		twowords = 0;
		if(pnext->i_ID == TOKEN_WORD)
		{
			twowords = 1;
			while(pnext && (pnext->i_ID == TOKEN_WORD)) pnext = Tool_NextValid(pnext);
		}

		/* A function ? */
		if(pnext->i_ID == TOKEN_LPAREN)
		{
			ptmp = Tool_ToRelationNext(pnext);

			/* Check if it's really a function */
			ptmp1 = Tool_NextValid(ptmp);

			/* Const after close paren */
			if(ptmp1->i_SubID == TOKEN_W_DECL && ptmp1->i_SubSubID == TOKEN_WW_CONST) ptmp1 = Tool_NextValid(ptmp1);

			/* Throws (Java) */
			if((ptmp1->i_SubID == TOKEN_W_DECL) && (ptmp1->i_SubSubID == TOKEN_WW_THROWS))
			{
				ptmp1 = Tool_NextValid(ptmp1);
				if(ptmp1->i_ID == TOKEN_LPAREN)
				{
					ptmp1 = Tool_ToRelationNext(ptmp1);
					ptmp1 = Tool_NextValid(ptmp1);
				}

				while(ptmp1->i_ID == TOKEN_WORD)
				{
					ptmp1 = Tool_NextValid(ptmp1);
					if(ptmp1->i_ID == TOKEN_COMMA)
						ptmp1 = Tool_NextValid(ptmp1);
					else
						break;
				}
			}

			if
			(
				(ptmp1->i_ID != TOKEN_SEMICOL)
			&&	(ptmp1->i_ID != TOKEN_LBRACE)
			&&	(ptmp1->i_ID != TOKEN_COLON)
			&&	(ptmp1->i_ID != TOKEN_LPAREN)
			)
			{
				if((ptmp1->i_ID == TOKEN_AFFECT) && (ptmp1->InClass))
				{
					ptmp2 = Tool_NextValid(ptmp1);
					if((ptmp2->i_ID == TOKEN_NUMBER) && (ptmp2->pc_Value[0] == '0') && (ptmp2->pc_Value[1] == 0))
						goto okvirtual;
				}

				pcur = ptmp;
				goto Error;
			}

okvirtual:
			if(ptmp1->i_ID == TOKEN_LPAREN) pnext = ptmp1;
		}

		if(pcur->i_SubSubID == TOKEN_WW_TEMPLATE)
		{
			pcur->IsType = 1;
			continue;
		}

		/* A template ? */
		if(pnext->i_ID == TOKEN_LESS)
		{
			ptmp1 = Tool_ToRelationNext(pnext);
			if(!ptmp1) goto Error;
			if(pnext->In_PP_Define && !ptmp1->In_PP_Define) goto Error;
			if(!pnext->In_PP_Define && ptmp1->In_PP_Define) goto Error;
			if(pnext->StmtLevel != ptmp1->StmtLevel) goto Error;

			pnext = Tool_NextValid(ptmp1);
			if(!pnext)
			{
				goto Error;
			}

			if(pnext->i_ID == TOKEN_WORD)
			{
				pnext = Tool_NextValid(pnext);
				if(pnext->i_ID == TOKEN_SEMICOL)
				{	/* what is happening here???? */
					pnext = Tool_PrevValid(pnext);
				}
			}
		}

recom2:
		/* A serie of pointers declaration */
		if(pnext->i_ID == TOKEN_STAR)
		{
			pprev = Tool_PrevValid(pnext);
			if(pprev->i_ID == TOKEN_WORD && pprev->i_SubSubID == TOKEN_WW_OPERATOR)
			{
				pnext->i_ID = TOKEN_WORD;
				pnext->i_SubID = 0;
				pnext->i_SubSubID = 0;
			}

			twowords = 1;
			while(pnext && pnext->i_ID == TOKEN_STAR) pnext = Tool_NextValid(pnext);
		}

		/* Reference decl */
		else if(pnext->i_ID == TOKEN_ADDRESS)
		{
			twowords = 1;
			pnext = Tool_NextValid(pnext);
		}
		else if(pnext->i_ID != TOKEN_DOUBLECOLON)
		{
			if(!cret) pnext = Tool_PrevValid(pnext);
			if(pnext->i_ID == TOKEN_GREAT)
			{
				pnext = Tool_ToRelationPrev(pnext);
				pnext = Tool_PrevValid(pnext);
			}
		}

		/* Const after * */
		if(pnext->i_SubID == TOKEN_W_DECL && pnext->i_SubSubID == TOKEN_WW_CONST) pnext = Tool_NextValid(pnext);

		/* Type *Word::Word */
		if(pnext->i_ID == TOKEN_WORD)
		{
			ptmp = Tool_NextValid(pnext);
			if(ptmp && (ptmp->i_ID == TOKEN_DOUBLECOLON)) pnext = ptmp;
		}

		/* C++ member function */
recom1:
		if(pnext->i_ID == TOKEN_DOUBLECOLON)
		{
			cD1 = 1;
			pnext = Tool_NextValid(pnext);
			if(pnext->i_ID == TOKEN_UNCHANGE)
			{
				pnext = Tool_NextValid(pnext);
				cD1 = 0;
			}

			if(pnext->i_ID != TOKEN_WORD)
			{
				goto Error;
			}
		}

		/* Special for decl */
		pword = pnext;
		pnn = pnn1 = pnext;
		if(pnn->i_ID == TOKEN_WORD)
		{
			while(pnn->i_ID == TOKEN_WORD)
			{
				if(pnn->i_SubSubID == TOKEN_WW_OPERATOR)
				{
					pnn->IsType = 1;
					pword = pnn;
					pnn1 = Tool_NextValid(pnn);
					if
					(
						(pnn1->i_ID == TOKEN_SPECWORD && pnn1->i_SubID == TOKEN_W_NEW)
					||	(pnn1->i_ID == TOKEN_SPECWORD && pnn1->i_SubID == TOKEN_W_DELETE)
					)
					{
						pnn1->i_ID = TOKEN_WORD;
						pnn1->i_SubID = 0;
						pnn1->i_SubSubID = 0;
						pnn2 = Tool_NextValid(pnn1);
						if(pnn2->i_ID == TOKEN_LARRAY) pnn1 = pnn2;
					}

					if(pnn1->i_ID == TOKEN_LARRAY)
					{
						pnn2 = Tool_NextValid(pnn1);
						if(pnn2->i_ID == TOKEN_RARRAY)
						{
							pnn1->i_ID = TOKEN_WORD;
							pnn1->i_SubID = 0;
							pnn1->i_SubSubID = 0;
							pnn2->i_ID = TOKEN_WORD;
							pnn2->i_SubID = 0;
							pnn2->i_SubSubID = 0;
							pnn1 = pnn2;
						}
					}
					else
					{
						pnn1->i_ID = TOKEN_WORD;
						pnn1->i_SubID = 0;
						pnn1->i_SubSubID = 0;
					}
					break;
				}

				pnn = Tool_NextValid(pnn);
			}

			pnext = Tool_NextValid(pnn1);
		}

		if
		(
			(pnext)
		&&	(pnext->i_ID != TOKEN_SEMICOL)
		&&	(pnext->i_ID != TOKEN_AFFECT)
		&&	(pnext->i_ID != TOKEN_LARRAY)
		&&	(pnext->i_ID != TOKEN_LPAREN)	/* Function proto */
		&&	(pnext->i_ID != TOKEN_RPAREN)	/* Function params */
		&&	(pnext->i_ID != TOKEN_LBRACE)	/* Function decl */
		&&	(pnext->i_ID != TOKEN_COMMA)	/* Functions params */
		)
		{
			/* Test bitfield */
			if(pnext->i_ID == TOKEN_COLON)
			{
				ptmp = Tool_NextValid(pnext);
				if(ptmp->i_ID == TOKEN_NUMBER) goto ok;
			}

			if(pnext->i_ID == TOKEN_DOUBLECOLON)
			{
				goto recom1;
			}

			/* For :: in a function return type */
			if((pnext->i_ID == TOKEN_WORD) && (cD1))
			{
				pword = Tool_NextValid(pword);
				pnext = Tool_NextValid(pnext);
				if(pnext->i_ID == TOKEN_DOUBLECOLON) goto recom1;
				goto ok;
			}

			if(pnext->i_ID == TOKEN_LESS)
			{
				pnext = Tool_ToRelationNext(pnext);
				pnext = Tool_NextValid(pnext);
				cret = 1;
				if(pnext) goto recom2;
			}

			if(cD1 && pnext->i_ID == TOKEN_STAR || pnext->i_ID == TOKEN_ADDRESS)
			{
				if(pnext->InParen)
				{
					goto recom2;
				}
			}

			goto Error;
		}

		/* For a function, force lparen to be passed to treat inside it */
ok:
		if(pnext->i_ID == TOKEN_LPAREN)
		{
			/* Declaration of a function */
			ptmp1 = Tool_ToRelationNext(pnext);
			ptmp1 = Tool_NextValid(ptmp1);
			if(ptmp1->i_ID == TOKEN_LPAREN) pnext = ptmp1;

			/*
			 * Special test. Function def in function must have twowords. this is not true,
			 * but that avoid to detect bas function definition instead of function calls.
			 */
			if((!twowords) && (pnext->StmtLevel))
			{
				if(!pnext->InClass || pnext->StmtLevel > 1)
				{
					goto Error;
				}
			}
		}

		/* Set IsType flag */
		if(pmemo == pword)
			Tool_SetFlag(pmemo, pword, &pmemo->IsType, 1);
		else
			Tool_SetFlag(pmemo, pword->pst_Prev, &pmemo->IsType, 1);
		pcur = pword;

		/* For a function, force lparen to be passed to treat inside it */
		if(pnext->i_ID == TOKEN_LPAREN)
		{
			plastfirsttype = pmemo;
			lasttwowords = twowords;
			pcur = pnext;
		}

		if(pcur->i_ID == TOKEN_AFFECT) pcur = pcur->pst_Prev;
		continue;
Error:
		/*
		 * Detect a token that is not a type and that is inside a paren. If yes, we must
		 * reset IsType for each token inside the same expression and zap the expression.
		 */
		if(!pcur->IsType && pcur->InParen)
		{
			ptmp = pcur->OpenParen;
			pmemo = Tool_ToRelationNext(ptmp);
			Tool_SetFlag(ptmp, pmemo, &ptmp->IsType, 0);

			/* Was it for a function ? */
			ptmp = pcur->OpenParen;
			ptmp = Tool_PrevValid(ptmp);
			if((ptmp) && (ptmp->i_ID == TOKEN_WORD) && (plastfirsttype) && (!lasttwowords))
			{
				Tool_SetFlag(plastfirsttype, pmemo, &plastfirsttype->IsType, 0);
			}

			pcur = pmemo;
			plastfirsttype = NULL;
			continue;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_FctDef(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pnext, *pend, *pmemo;
	struct token_	*pnext1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Typedef of a function */
		if(pcur->IsType)
		{
			pnext = Tool_PrevValid(pcur);
			if(pnext && (pnext->i_SubSubID == TOKEN_WW_TYPEDEF))
			{
				pcur = Tool_SearchToken(pcur, TOKEN_SEMICOL);
				continue;
			}
		}

		/* Destructor ?? */
		if(pcur->i_ID == TOKEN_UNCHANGE && pcur->StmtLevel == 0)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_WORD)
			{
				pnext1 = Tool_NextValid(pnext);
				if(pnext1->i_ID == TOKEN_LPAREN)
				{
					pcur->IsType = 1;
					pnext->IsFctDefName = 1;
					pnext = Tool_PrevValid(pcur);
					if(pnext->i_ID == TOKEN_WORD)
					{
						Tool_ForceSpaceAfter(pnext, 1);
						pnext->IsType = 1;
						pcur = pnext;
					}
				}
			}
		}

		/* Destructor */
		if(pcur->i_ID == TOKEN_UNCHANGE && pcur->InClass && pcur->StmtLevel == 1)
		{
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_WORD)
			{
				pnext1 = Tool_NextValid(pnext);
				if(pnext1->i_ID == TOKEN_LPAREN)
				{
					pnext->IsFctDeclName = 1;
					pcur->IsType = 1;
				}
			}
		}

		pmemo = pcur;
bcl:
		if(pcur->IsType)
		{
			/* Zap all types definition */
			while(pcur && pcur->IsType)
			{
				pcur = Tool_NextValid(pcur);
			}

			/* Special case for function decl without type */
			if(pcur->i_ID == TOKEN_LPAREN)
			{
				pcur = Tool_PrevValid(pcur);
			}

			if(pcur->i_ID == TOKEN_LESS)
			{
				pnext = Tool_ToRelationNext(pcur);
				if(pnext) pnext = Tool_NextValid(pnext);
				if(!pnext) continue;
				pcur = pnext;
				goto bcl;
			}

			/* Must be the name of the function */
			if(pcur->i_ID != TOKEN_WORD)
			{
				if(pcur->i_ID == TOKEN_GREAT)
				{
				}
				else
				{
					/* Operator */
					if(pcur->pst_Prev->i_SubSubID != TOKEN_WW_OPERATOR) continue;
				}
			}
			else
			{
				while(pcur->i_ID == TOKEN_WORD) pcur = Tool_NextValid(pcur);
				pcur = Tool_PrevValid(pcur);
			}

			/* Check for open paren */
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_LPAREN)
			{
				/* Get the token after close paren */
				pnext = Tool_ToRelationNext(pnext);
				pnext = Tool_NextValid(pnext);

				/* Const after declaration */
				if((pnext->i_SubID == TOKEN_W_DECL) && (pnext->i_SubSubID == TOKEN_WW_CONST))
				{
					pnext = Tool_NextValid(pnext);
				}

				/* C++ constructor */
				if(pnext->i_ID == TOKEN_COLON)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext->i_ID == TOKEN_WORD)
					{
						pnext = Tool_SearchToken(pnext, TOKEN_LBRACE);
						if(!pnext) continue;
					}
				}

				/* Throws */
				if
				(
					(pnext->i_SubID == TOKEN_W_DECL)
				&&	((pnext->i_SubSubID == TOKEN_WW_THROWS) || (pnext->i_SubSubID == TOKEN_WW_THROW))
				)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext->i_ID == TOKEN_LPAREN)
					{
						pnext = Tool_ToRelationNext(pnext);
						pnext = Tool_NextValid(pnext);
					}

					while(pnext->i_ID == TOKEN_WORD)
					{
						pnext = Tool_NextValid(pnext);
						if(pnext->i_ID == TOKEN_COMMA)
							pnext = Tool_NextValid(pnext);
						else
							break;
					}
				}

				/* Semicol = function declaration */
				if(pnext->i_ID == TOKEN_SEMICOL)
				{
					pcur->IsFctDeclName = 1;
					Tool_SetFlag(pmemo, pnext->pst_Prev, &pmemo->InFctDecl, 1);
					pcur = pnext;
				}

				if(pnext->i_ID == TOKEN_AFFECT)
				{
					pnext1 = Tool_NextValid(pnext);
					if((pnext1->i_ID == TOKEN_NUMBER) && (pnext1->pc_Value[0] == '0') && (pnext1->pc_Value[1] == 0))
					{
						pcur->IsFctDeclName = 1;
						Tool_SetFlag(pmemo, pnext->pst_Prev, &pmemo->InFctDecl, 1);
						pcur = pnext;
					}
				}

				/* Lbrace = function definition */
				if((pnext->i_ID == TOKEN_LBRACE) || (pnext->i_ID == TOKEN_COLON))
				{
					pcur->IsFctDefName = 1;
					Tool_SetFlag(pmemo, pnext->pst_Prev, &pmemo->InFctDef, 1);

					/* Search the open brace if not yet found */
					if(pnext->i_ID != TOKEN_LBRACE)
					{
						pnext = Tool_SearchToken(pnext, TOKEN_LBRACE);
					}

					/* Set InFct flag */
					pend = Tool_ToRelationNext(pnext);
					Tool_SetFlag(pnext, pend, &pnext->InFct, 1);
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_FctParams(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend;
	char	cmacro;
	/*~~~~~~~~~~~~~~~~~*/

	cmacro = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if((pcur->IsFctDefName) || (pcur->IsFctDeclName) || (pcur->MacroName))
		{
			cmacro = 0;
			if(pcur->MacroName) cmacro = 1;

			/* Get open paren */
			pcur = Tool_NextValid(pcur);
			pend = Tool_ToRelationNext(pcur);
			pend->EndListParam = 1;
			pcur = Tool_NextValid(pcur);

			/* Special case for a macro */
			if(cmacro)
			{
				while(pcur != pend)
				{
					pcur->IsFctParamName = 1;
					pcur = NextToken(pcur);
					if(pcur->i_ID == TOKEN_COMMA) pcur = NextToken(pcur);
				}

				continue;
			}

			/* The first token just after the open paren must be a type */
			if(!pcur->IsType) continue;
			while(pcur != pend)
			{
				if(pcur->i_ID == TOKEN_ELLIPSIS) pcur->IsFctParamName = 1;
				if((!pcur->IsType) && (pcur->i_ID == TOKEN_WORD)) pcur->IsFctParamName = 1;
				pcur = NextToken(pcur);
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_FctCall(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext;
	token	*pprev;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pnext = Tool_NextValid(pcur);
		if
		(
			(pcur->i_ID == TOKEN_WORD)
		&&	(pnext && (pnext->i_ID == TOKEN_LPAREN))
		&&	(!pcur->IsFctDeclName)
		&&	(!pcur->IsFctDefName)
		&&	(!pcur->MacroName)
		&&	(pcur->StmtLevel)
		&&	((pcur->InFct) || (pcur->In_PP_Define))
		)
		{
			pprev = Tool_PrevValid(pcur);
			if
			(
				pprev
			&&	(pprev->i_ID != TOKEN_PP)
			||	(
					(pprev->i_SubID != TOKEN_PP_IF)
				&&	(pprev->i_SubID != TOKEN_PP_ELIF)
				&&	(pprev->i_SubID != TOKEN_PP_IFDEF)
				&&	(pprev->i_SubID != TOKEN_PP_IFNDEF)
				)
			)
			{
				pcur->IsFctCall = 1;
			}
		}
	}
}

/*
 =======================================================================================================================
    Aim: For &, * or -, determine if it's a binary or unary operator.
 =======================================================================================================================
 */
void Grammar_UnaryBinary(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pprev, *pprev1, *pnext;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pprev = Tool_PrevValid(pcur);
		pnext = Tool_NextValid(pcur);
		switch(pcur->i_ID)
		{
		case TOKEN_AFFECT:
		case TOKEN_AUTOOP:
		case TOKEN_SIGN:
		case TOKEN_LBRACE:
		case TOKEN_LPAREN:
		case TOKEN_LARRAY:
		case TOKEN_COMMA:
		case TOKEN_SEMICOL:
		case TOKEN_LESS:
		case TOKEN_GREAT:
		case TOKEN_OPER:
		case TOKEN_LINK:
		case TOKEN_ADDRESS:
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_STAR || pnext->i_ID == TOKEN_ADDRESS || pnext->i_ID == TOKEN_SIGN)
				pnext->UnaryOp = 1;
			break;

		case TOKEN_STAR:
			pnext = Tool_NextValid(pcur);
			if(pnext->i_ID == TOKEN_STAR || pnext->i_ID == TOKEN_ADDRESS || pnext->i_ID == TOKEN_SIGN)
				pnext->UnaryOp = 1;
			pnext = Tool_PrevValid(pcur);
			if(pnext && (pnext->i_ID == TOKEN_POINT || pnext->i_ID == TOKEN_POINTER)) pcur->UnaryOp = 1;
			break;
		}
	}

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		pprev = Tool_PrevValid(pcur);
		pnext = Tool_NextValid(pcur);
		switch(pcur->i_ID)
		{
		case TOKEN_ADDRESS:
			if(pcur->UnaryOp) continue;

			/* ... *) & : It's a cast */
			if(pprev->i_ID == TOKEN_RPAREN)
			{
				pprev1 = Tool_PrevValid(pprev);
				if(pprev1->i_ID == TOKEN_STAR) break;
			}

		case TOKEN_STAR:
		case TOKEN_SIGN:
			if(pcur->UnaryOp) continue;
			if(pcur->IsType) break;
			if(pprev->IsCast) break;

			/* After an expression if, while etc... */
			if(pprev->i_ID == TOKEN_RPAREN)
			{
				pprev1 = Tool_ToRelationPrev(pprev);
				pprev1 = Tool_PrevValid(pprev1);
				if(SpecWord(pprev1, TOKEN_W_IF)) break;
				if(SpecWord(pprev1, TOKEN_W_WHILE)) break;
				if(SpecWord(pprev1, TOKEN_W_FOR)) break;
				if(pprev1->MacroName) break;
			}

			if(SpecWord(pprev, TOKEN_W_DO)) break;

			/* Just after a type declaration */
			if(pprev->i_ID == TOKEN_WORD && pprev->i_SubID == TOKEN_W_DECL) break;
			if(pnext->IsFctParamName) break;
			if
			(
				(
					(pprev->i_ID == TOKEN_WORD)
				||	(pprev->i_ID == TOKEN_RPAREN)
				||	(pprev->i_ID == TOKEN_RARRAY)
				||	(pprev->i_ID == TOKEN_NUMBER)
				||	(pprev->i_ID == TOKEN_STRINGCST)
				||	(pprev->i_ID == TOKEN_CHARCST)
				)
			&&	(
					(pnext->i_ID == TOKEN_WORD)
				||	(pnext->i_ID == TOKEN_LPAREN)
				||	(pnext->i_ID == TOKEN_NUMBER)
				||	(SpecWord(pnext, TOKEN_W_SIZEOF))
				||	(pnext->i_ID == TOKEN_STRINGCST)
				||	(pnext->i_ID == TOKEN_CHARCST)
				)
			) pcur->BinaryOp = 1;
			break;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_BeginEndDecl(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	struct token_	*pcur, *pmemo, *pnext, *pnext1;
	char			c_enum;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	c_enum = 0;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->IsType)
		{
			/* Begin decl */
			pcur->BeginDecl = 1;
			c_enum = 0;
			while(pcur->IsType)
			{
				if(pcur->i_SubSubID == TOKEN_WW_ENUM) c_enum = 1;
				pcur = NextToken(pcur);
				if(pcur->i_SubSubID == TOKEN_WW_ENUM) c_enum = 1;
			}

			/* Special case for function decl without type */
			if(pcur->i_ID == TOKEN_LPAREN) pcur = Tool_PrevValid(pcur);

			/* Decl name */
recom:
			if(pcur->i_ID == TOKEN_WORD)
			{
				pcur->DeclName = 1;

				/* A function */
				if(pcur->IsFctDefName || pcur->IsFctDeclName)
				{
					pnext = Tool_NextValid(pcur);
					pnext = Tool_ToRelationNext(pnext);
					pnext = Tool_NextValid(pnext);

					/* Decl with const */
					if((pnext->i_SubID == TOKEN_W_DECL) && (pnext->i_SubSubID == TOKEN_WW_CONST))
					{
						pnext = Tool_NextValid(pnext);
					}

					if(pnext->i_ID == TOKEN_AFFECT)
					{
						pnext1 = Tool_NextValid(pnext);
						if((pnext1->i_ID == TOKEN_NUMBER) && (pnext1->pc_Value[0] == '0') && (pnext1->pc_Value[1] == 0))
							pnext = Tool_NextValid(pnext1);
					}

					pnext->EndDecl = 1;
					continue;
				}

				if(c_enum)
				{
					pcur = Tool_NextValid(pcur);
					pcur->EndDecl = 1;
					continue;
				}

				pcur = Tool_NextValid(pcur);
			}

			/* Init decl */
			for(;;)
			{
				while
				(
					(pcur->i_ID == TOKEN_LPAREN)
				||	(pcur->i_ID == TOKEN_LARRAY)
				||	(pcur->i_ID == TOKEN_LESS && pcur->InTemplate)
				)
				{
					pcur = Tool_ToRelationNext(pcur);
					pcur = Tool_NextValid(pcur);
				}

				pmemo = pcur;
				if(pcur->pst_Prev->i_SubSubID != TOKEN_WW_OPERATOR)
				{
					pcur = Grammar_Affect(pcur);
					if(pcur && pmemo != pcur)
					{
						Tool_SetFlag(pmemo, pcur->pst_Prev, &pmemo->InitDecl, 1);
					}
				}

				/* End decl */
				while
				(
					(pcur)
				&&	(pcur->i_ID != TOKEN_SEMICOL)
				&&	(pcur->i_ID != TOKEN_COMMA)
				&&	(pcur->i_ID != TOKEN_RPAREN)
				)
				{
					if((pcur->i_ID == TOKEN_LBRACE) && !c_enum) break;
					if((pcur->i_ID == TOKEN_LPAREN) || (pcur->i_ID == TOKEN_LBRACE)) pcur = Tool_ToRelationNext(pcur);
					pcur = NextToken(pcur);
				}

				/* Multiple declaration */
				if(pcur && (pcur->i_ID == TOKEN_COMMA) && (!pcur->InFctDecl) && (!pcur->InFctDef))
				{
					pcur = Tool_NextValid(pcur);
					if(pcur->IsType)
					{
						pcur = Tool_PrevValid(pcur);
						break;
					}

					if(pcur->i_ID == TOKEN_WORD) goto recom;
				}
				else
					break;
			}

			if(!pcur) break;
			pcur->EndDecl = 1;
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_DeclStmt(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pmemo, *pnext;
	int		count;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->BeginDecl)
		{
			pmemo = pcur;
			pmemo->BegStmtDecl = 1;
			while(pmemo)
			{
				/* Search end decl */
				if(!pmemo->EndDecl)
				{
					pmemo = NextToken(pmemo);
					count = 1;
					while(pmemo && count)
					{
						if(pmemo->BeginDecl) count++;
						if(pmemo->EndDecl) count--;
						if(count) pmemo = NextToken(pmemo);
					}
				}

				if(!pmemo) break;
				if(pmemo->i_ID == TOKEN_LBRACE) goto forceend;

				/* C++ constructor */
				if(pmemo->i_ID == TOKEN_COLON)
				{
					pnext = pmemo;
					pnext = Tool_NextValid(pmemo);
					if(pnext->i_ID == TOKEN_WORD)
					{
						pmemo = Tool_SearchToken(pmemo, TOKEN_LBRACE);
						if(!pmemo) break;
						goto forceend;
					}
				}

				pnext = pmemo;
				if(pnext->CmtLevel) goto forceend;
				if(pnext->EndAlign) goto forceend;
				while(pnext->i_ID == TOKEN_CCMT)
				{
					if(pnext->CmtLevel) goto forceend;
					if(pnext->EndAlign) goto forceend;
					pnext = pnext->pst_Next;
				}

				pnext = NextToken(pnext);
				if(pnext->CmtLevel) goto forceend;
				if(pnext->EndAlign) goto forceend;
				while(pnext->i_ID == TOKEN_CCMT)
				{
					if(pnext->CmtLevel) goto forceend;
					if(pnext->EndAlign) goto forceend;
					pnext = pnext->pst_Next;
				}

				if(pnext->PubClassDecl) goto forceend;

				/* Get token after declaration */
				pnext = pmemo;
				pnext = Tool_NextValid(pnext);

				/* Don't care of label */
				while(pnext->LabelDecl)
				{
					pnext = Tool_NextValid(pnext);
					pnext = Tool_NextValid(pnext);
				}

				/* Don't care about some PP directives */
				if(pnext->InPP && !pcur->InPP)
				{
					pnext = Tool_NextValid(pnext);
					if(pnext->MacroName) goto forceend;
					while(pnext && pnext->InPP) pnext = NextToken(pnext);
				}

				if((pnext) && ((!pnext->BeginDecl) || (pnext->StmtLevel != pcur->StmtLevel) || (pnext->InFctDef)))
				{
forceend:
					pmemo->EndStmtDecl = 1;
					Tool_SetFlag(pcur, pmemo, &pcur->InStmtDecl, 1);
					pcur = pmemo;
					break;
				}

				pmemo = pnext;
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_LabelDecl(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pnext, *pprev;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		/* Public: etc... */
		if
		(
			(pcur->i_ID == TOKEN_WORD)
		&&	(pcur->i_SubID == TOKEN_W_DECL)
		&&	(NextToken(pcur))
		&&	(NextToken(pcur)->i_ID == TOKEN_COLON)
		)
		{
			pcur->LabelDecl = 1;
			pcur->PubClassDecl = 1;
			if(pcur->StmtLevel) pcur->StmtLevel--;
			pcur->IndentLevel = 0;
		}

		/* Normal label decl */
		else if(pcur->i_ID == TOKEN_WORD)
		{
			pnext = NextToken(pcur);
			pprev = Tool_PrevValid(pcur);
			if
			(
				pnext
			&&	(pnext->i_ID == TOKEN_COLON)
			&&	pprev
			&&	((pprev->i_ID == TOKEN_SEMICOL) || (pprev->i_ID == TOKEN_RBRACE) || (pprev->i_ID == TOKEN_LBRACE))
			)
			{
				if((WasFirstOnLine(pcur)) && (pcur->StmtLevel))
				{
					pcur->LabelDecl = 1;
					pcur->StmtLevel = pcur->IndentLevel = 0;
				}
			}
		}
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_MoveDeclAffect(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pmemo, *ptmp, *ptmp1, *pend, *pend1, *ptmp2;
	char	cmemo;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	cmemo = 0;
	pend1 = NULL;
	pcur = pfile->pst_RootToken;
	if(!Config.MoveDeclAffect) return;
	while(pcur)
	{
		if(pcur->NoIndent)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->InEnum)
		{
			pcur = NextToken(pcur);
			continue;
		}

		if(pcur->StmtLevel && !pcur->InFctDef && !pcur->InFctDecl && !pcur->InClass)
		{
			if(pcur->BeginDecl)
			{
				/* Search the last token of the decl block */
				pend = pcur;
				while(!pend->EndStmtDecl) pend = NextToken(pend);
				while(pend->i_ID == TOKEN_CCMT) pend = PrevToken(pend);

				/* Static decl */
				if((pcur->i_SubSubID == TOKEN_WW_STATIC) || (pcur->i_SubSubID == TOKEN_WW_CONST))
				{
					ptmp = pcur;
					goto zap;
				}

				/* Get the first decl name */
				ptmp = pcur;
				while(ptmp && !ptmp->DeclName)
				{
					if(ptmp->i_ID == TOKEN_ADDRESS) goto zap;
					ptmp = NextToken(ptmp);
				}

				cmemo = 0;
				if(!ptmp) break;
				if(ptmp->i_SubSubID == TOKEN_WW_OPERATOR) goto zap;
				if(ptmp->InEnum) goto zap;
				for(;;)
				{
					ptmp1 = Tool_NextValid(ptmp);
					if(ptmp1->i_ID == TOKEN_AFFECT)
					{
						/* Don't move affect with {} */
						ptmp2 = Tool_NextValid(ptmp1);
						if(ptmp2->i_ID != TOKEN_LBRACE)
						{
							Tool_InsertTokenAfter(pfile, pend, TOKEN_SEMICOL);
							Tool_InsertTokenAfter(pfile, pend, TOKEN_WORD);

							/* Copy variable name */
							cmemo = 1;
							pend = NextToken(pend);
							pend->pc_Value = strdup(ptmp->pc_Value);
							pend->StmtLevel = pcur->StmtLevel;
							pend->ForceEOLAfter = 0;

							/* Move everything until comma */
							while(ptmp1 && ptmp1->i_ID != TOKEN_COMMA && ptmp1->i_ID != TOKEN_SEMICOL)
							{
								pmemo = NextToken(ptmp1);
								if(ptmp1->i_ID == TOKEN_LPAREN) pend1 = Tool_ToRelationNext(ptmp1);
								Tool_MoveTokenAfter(pfile, ptmp1, pend);
								if(ptmp1->i_ID == TOKEN_LPAREN)
								{
									pend = ptmp1;
									ptmp1 = pmemo;
									while(ptmp1 != pend1)
									{
										pmemo = NextToken(ptmp1);
										Tool_MoveTokenAfter(pfile, ptmp1, pend);
										pend = ptmp1;
										ptmp1 = pmemo;
									}

									continue;
								}

								pend = ptmp1;
								ptmp1 = pmemo;
							}

							/* Insert a semicol */
							if(pend) pend = NextToken(pend);
							if(pend)
							{
								pend->pc_Value = strdup(";");
								pend->WasEOLAfter = pend->ForceEOLAfter = 1;
								Tool_ForceEOLAfter(pend, 1);
							}
						}
						else
						{
							ptmp = Tool_ToRelationNext(ptmp2);
						}
					}

					/* Get next declaration */
					ptmp = ptmp->pst_Next;
					while(!ptmp->DeclName && !ptmp->EndDecl)
					{
						if(ptmp->i_ID == TOKEN_ADDRESS) goto zap;
						ptmp = NextToken(ptmp);
					}

					if(!ptmp) break;
					if(ptmp->NoIndent) break;
					if(ptmp->InEnum) break;
recom:
					if(ptmp->EndDecl)
					{
						ptmp = Tool_NextValid(ptmp);
						if(ptmp && ptmp->BeginDecl)
						{
							/* Special case for static vars */
							if
							(
								(ptmp->i_SubID == TOKEN_W_DECL && ptmp->i_SubSubID == TOKEN_WW_STATIC)
							||	(ptmp->i_SubID == TOKEN_W_DECL && ptmp->i_SubSubID == TOKEN_WW_CONST)
							)
							{
zap:
								while(ptmp && !ptmp->EndDecl) ptmp = NextToken(ptmp);
								goto recom;
							}

							while(ptmp && !ptmp->DeclName)
							{
								if(ptmp->i_ID == TOKEN_ADDRESS) goto zap;
								ptmp = NextToken(ptmp);
							}

							if(!ptmp) break;
						}
						else
							break;
					}

					pcur = ptmp;
				}

				/* Force an empty line after block */
				if(cmemo && pend) Tool_ForceEmptyLineAfter(pend);
			}
		}

		pcur = NextToken(pcur);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_MoveDecl(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pnext, *pmemo;
	/*~~~~~~~~~~~~~~~~~~~*/

	pcur = pfile->pst_RootToken;
	if(!Config.MoveDeclToTop) return;
	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->NoIndent) continue;
		if(pcur->StmtLevel && pcur->InFct)
		{
			if(pcur->BegStmtDecl)
			{
				/* Get the beginning of the decl */
				pnext = pcur;
				while((pnext->StmtLevel >= pcur->StmtLevel) && (pnext->IndentLevel >= pcur->IndentLevel))
				{
					pnext = PrevToken(pnext);
					if(pnext->i_ID == TOKEN_RBRACE) pnext = Tool_ToRelationPrev(pnext);
				}

				if(pnext->i_ID == TOKEN_LBRACE)
				{
					/* Get the firstdecl of the statement */
					pnext = Tool_NextValid(pnext);
					if(pnext != pcur)
					{
						/* Go to the end of the block decl */
						if(pnext->BegStmtDecl)
						{
							while(pnext && (pnext != pcur) && (!pnext->EndStmtDecl) && (pnext->i_ID != TOKEN_RBRACE))
							{
								if(pnext->i_ID == TOKEN_AFFECT)
								{
									pnext = NextToken(pnext);
									if(!pnext) break;
									if(pnext == pcur) break;
									if(pnext->EndStmtDecl) break;
								}
								else
								{
									pnext = NextToken(pnext);
								}

								if(pnext->i_ID == TOKEN_LBRACE)
								{
									pnext = Tool_ToRelationNext(pnext);
									if(!pnext->EndStmtDecl) pnext = NextToken(pnext);
								}
							}
						}
						else
						{
							pnext = Tool_PrevValid(pnext);
						}

						/* Move the decl block */
						if(pnext != pcur)
						{
							pnext->EndStmtDecl = 0;
							pcur->BegStmtDecl = 0;
							do
							{
								pmemo = NextToken(pcur);
								Tool_MoveTokenAfter(pfile, pcur, pnext);
								pnext = NextToken(pnext);
								pcur = pmemo;
							} while(pcur && !pcur->EndStmtDecl);
							if(pcur) Tool_MoveTokenAfter(pfile, pcur, pnext);
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
void Grammar_InTemplate(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~*/
	token	*pcur;
	token	*pprev;
	token	*pnext, *pend;
	/*~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(!pcur->IsType) continue;
		if(WWord(pcur, TOKEN_W_DECL) && pcur->i_SubSubID == TOKEN_WW_OPERATOR)
		{
			pcur = NextToken(pcur);
			continue;
		}

		pnext = Tool_NextValid(pcur);
		if(pnext->i_ID != TOKEN_LESS) continue;
		pend = Tool_ToRelationNext(pnext);
		if(!pend) Syntaxe(pcur->line, pcur->column);
		while(pnext != pend)
		{
			pnext->InTemplate = 1;
			pnext = pnext->pst_Next;
		}

		pnext->InTemplate = 1;
	}

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID != TOKEN_LESS) continue;
		pprev = Tool_PrevValid(pcur);
		if(pprev->i_ID != TOKEN_WORD) continue;
		pnext = Tool_NextValid(pcur);
		if(pnext->IsType)
		{
			pend = Tool_ToRelationNext(pcur);
			if(!pend) continue;
			pnext = pcur;
			while(pnext != pend)
			{
				pnext->InTemplate = 1;
				pnext = pnext->pst_Next;
			}

			pnext->InTemplate = 1;
			continue;
		}

		pend = Tool_ToRelationNext(pcur);
		if(!pend) continue;
		pnext = pcur;
		pnext = Tool_NextValid(pnext);
		while(pnext != pend)
		{
			switch(pnext->i_ID)
			{
			case TOKEN_WORD:
			case TOKEN_NUMBER:
				break;

			default:
				goto lnext;
			}

			pnext = Tool_NextValid(pnext);
			if(pnext == pend) break;
			if(!pnext) goto lnext;
			if(pnext->i_ID != TOKEN_COMMA) goto lnext;
			pnext = Tool_NextValid(pnext);
		}

		pnext = pcur;
		while(pnext != pend)
		{
			pnext->InTemplate = 1;
			pnext = pnext->pst_Next;
		}

		pnext->InTemplate = 1;
lnext: ;
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Grammar_InEnum(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pcur, *pend, *ptmp;
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_SubSubID == TOKEN_WW_ENUM)
		{
			ptmp = pcur;
			pcur = Tool_NextValid(pcur);
			if(pcur->i_ID != TOKEN_LBRACE)
			{
				pcur = Tool_NextValid(pcur);
				if(pcur && pcur->i_ID != TOKEN_LBRACE) return;
			}

			pend = Tool_ToRelationNext(pcur);
			while(ptmp != pend)
			{
				ptmp->InEnum = 1;
				ptmp = NextToken(ptmp);
			}
		}
	}
}
