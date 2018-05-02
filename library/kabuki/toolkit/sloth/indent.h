/*$T \Sources/indent.h GC 1.150 2011-09-22 20:52:17 */


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
#ifndef __INDENT_H__
#define __INDENT_H__

extern void Indent_SplitCmtToIndent(FileDes *);
extern void Indent_CmtBase(FileDes *);
extern void Indent_Cmt(FileDes *);
extern void Tool_InCmt(token *);
extern void Indent_PPBase(FileDes *);
extern void Indent_PP(FileDes *);
extern void Indent_SpacesBase(FileDes *);
extern void Indent_Spaces(FileDes *);
extern void Indent_EOLBase(FileDes *);
extern void Indent_EOL(FileDes *);
extern void Indent_EOLAddEmptyLines(FileDes *);
extern void Indent_EOLBetweenEmptyBraces(FileDes *);
extern void Indent_StmtIndentLevel(FileDes *);
extern void Indent_AlignTypeNamesGlobal(FileDes *);
extern void Indent_AlignBodyFctInlineClass(FileDes *);
extern void Indent_AlignBodyFct(FileDes *);
extern void Indent_AlignTypeNamesFunc(FileDes *);
extern void Indent_AlignParenToName(FileDes *);
extern void Indent_AlignDefine(FileDes *);
extern void Indent_RemoveEOL(FileDes *);
extern void Indent_Conform(FileDes *);
extern void Indent_PPBreak(FileDes *);
extern void Indent_Special(FileDes *);
extern void Indent_AlignEnum(FileDes *);
extern void Indent(FileDes *);
extern void Indent_ConvertCToCpp(FileDes *);
extern void Indent_EmptyLinesSep(FileDes *);
extern void Indent_ParseOptions(FileDes *);
extern void Indent_CommentLastLine(FileDes *);
extern void Indent_AlignOperSpecial(FileDes *);
extern void Indent_BraceStyle(FileDes *);
extern void Indent_EOLEmptyStmt(FileDes *);
extern void Indent_Switch(FileDes *);
extern void Indent_PPStyle(FileDes *);
extern void Indent_scast(FileDes *);
extern void Indent_Return(FileDes *);
extern void Indent_CmtCommentAddDeclStmt(FileDes *);
extern void Indent_InClassIndent(FileDes *);
extern void Indent_AlignTypeNamesCom(token *, token *);
extern void Indent_CmtFctDefParms(FileDes *pfile);
extern void Indent_FixupTrailingComments(FileDes *pfile);
extern void Indent_RetrospectiveElseAdjustment(FileDes *pfile);
extern void Indent_EOLAfterEveryCloseBrace(FileDes *pfile);
#endif /* __INDENT_H__ */
