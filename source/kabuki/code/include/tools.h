/*$T \Sources/tools.h GC 1.150 2011-09-22 20:52:18 */


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
#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "config.h"
#include "lexi.h"
extern char LastOnLineWithCmt(token *);

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

#define __malloc__(a)		malloc(sizeof(char) * (a))
#define __realloc__(a, b)	realloc(a, sizeof(char) * (b))
#define PrevToken(p)		(p != NULL ? p->pst_Prev : NULL)
#define NextToken(p)		(p != NULL ? p->pst_Next : NULL)
#define FirstOnLine(p)		(!(p->pst_Prev) || ((p->pst_Prev) && (p->pst_Prev->ForceEOLAfter)))
#define WasFirstOnLine(p)	(!(p->pst_Prev) || ((p->pst_Prev) && (p->pst_Prev->WasEOLAfter)))
#define LastOnLine(p)		(p->ForceEOLAfter)
#define WasLastOnLine(p)	(p->WasEOLAfter)
#define SpecWord(p, i)		(p->i_ID != TOKEN_SPECWORD ? 0 : (p->i_SubID == i ? 1 : 0))
#define WWord(p, i)			(p->i_ID != TOKEN_WORD ? 0 : (p->i_SubID == i ? 1 : 0))
#define IsAComment(p)		((p->i_ID == TOKEN_CCMT) || (p->i_ID == TOKEN_CPPCMT))
#define IsACommentInC(p)	(p->i_ID == TOKEN_CCMT)
#define IsACommentInCPP(p)	(p->i_ID == TOKEN_CPPCMT)

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

extern void		Tool_MoveTokenBefore(FileDes *, token *, token *);
extern void		Tool_MoveTokenAfter(FileDes *, token *, token *);
extern token	*Tool_UnlinkToken(FileDes *, token *);
extern token	*Tool_InsertTokenAfter(FileDes *, token *, int);
extern token	*Tool_InsertTokenBefore(FileDes *, token *, int);
extern void		Tool_ComputeCol(FileDes *);
extern int		Tool_LenTokens(token *, token *);
extern token	*Tool_ToBegLine(token *);
extern token	*Tool_ToEndLine(token *);
extern token	*Tool_PrevValid(token *);
extern token	*Tool_NextValid(token *);
extern token	*Tool_SearchToken(token *, int);
extern int		Tool_SearchFirst(token *, int, int);
extern token	*Tool_ToRelationNext(token *);
extern token	*Tool_ToRelationPrev(token *);
extern void		Tool_ForceEOLAfterComments(token *, int);
extern void		Tool_ForceEOLBefore(token *, int);
extern void		Tool_ForceEOLAfter(token *, int);
extern void		Tool_ForceExactEOLBefore(token *, int);
extern void		Tool_ForceExactEOLAfter(token *, int);
extern void		Tool_ForceSpaceAfter(token *, int);
extern void		Tool_ForceSpaceBefore(token *, int);
extern void		Tool_ForceEmptyLineAfter(token *);
extern void		Tool_ForceEmptyLineBefore(token *);
extern void		Tool_SetFlag(token *, token *, char *, char);
extern void		Tool_IncIndentLevel(token *, token *, int);
extern int		Tool_ToTab(int);
extern char		Tool_IsSpecialWord(char *, int *type, int *column);

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

#define CMTMARK '$'
#define EmptyComment(p) \
		((p->pc_Value[0] == '/') && (p->pc_Value[1] == '*') && (p->pc_Value[2] == '*') && (p->pc_Value[3] == '/'))
#define LevelCommentEx(pcur, offset)	((pcur->pc_Value[offset + 2] == CMTMARK) && (isdigit(pcur->pc_Value[offset + 3])))
#define ForceSplitCommentEx(pcur, offset) \
		((pcur->pc_Value[offset + 2] == CMTMARK) && (pcur->pc_Value[offset + 3] == 'S'))
#define DeleteCommentEx(pcur, offset)	((pcur->pc_Value[offset + 2] == CMTMARK) && (pcur->pc_Value[offset + 3] == 'D'))
#define ForceSplitComment(pcur)			(ForceSplitCommentEx(pcur, 0) || ForceSplitCommentEx(pcur, 1))
#define DeleteComment(pcur)				(DeleteCommentEx(pcur, 0) || DeleteCommentEx(pcur, 1))
#define _isspace(a)						(a == ' ' || a == '\n' || a == '\t')

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

extern void		Tool_SplitCmtFirstLine(token *);
extern void		Tool_UnSplitCmt(token *);
extern void		Tool_ConcatCmt(FileDes *, token *);
extern void		Tool_SplitCmtFct(token *, int);
extern void		Tool_SplitEndLineCmt(token *);
extern void		Tool_InsertSepAfter(FileDes *, token *, token *, int, int);
extern void		Tool_InsertSingleSep(FileDes *, token *, char);
extern void		Tool_AddTag(FileDes *, token *);
extern token	*Tool_AddEmpty(FileDes *, token *, char *, int, int);
extern token	*Tool_AddEmptyCmtAfterIfMissing(FileDes *pfile, token *pcur);
extern char		*Tool_pLevelComment(token *pcur);
extern int		FixedComment(token *pcur);

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

#define Tool_ComputeColLine(pcur) \
	for(;;) \
	{ \
		token	*pp = pcur; \
		int		__col__; \
		__col__ = ((pp)->StmtLevel * Config.TabSize) + ((pp)->IndentLevel * Config.TabSize); \
		if((pp)->pst_Prev) __col__ += (pp)->pst_Prev->AddSpaceAfter; \
		(pp)->Col = __col__; \
		while((pp) && !(pp)->ForceEOLAfter) \
		{ \
			__col__ += strlen(pp->pc_Value) + pp->ForceSpaceAfter; \
			pp = NextToken(pp); \
			if(pp) pp->Col = __col__; \
		} \
		break; \
	}
#endif /* __TOOLS_H__ */
