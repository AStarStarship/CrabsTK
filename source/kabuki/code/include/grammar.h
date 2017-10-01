/*$T \Sources/grammar.h GC 1.150 2011-09-22 20:52:17 */


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
#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__

extern void Grammar_InsideParen(FileDes *);
extern void Grammar_IsType(FileDes *);
extern void Grammar_IsTypeCorrect(FileDes *);	/* needed ?? */
extern void Grammar_FctDef(FileDes *);
extern void Grammar_FctParams(FileDes *);
extern void Grammar_FctCall(FileDes *);
extern void Grammar_BeginEndDecl(FileDes *);
extern void Grammar_DeclStmt(FileDes *);
extern void Grammar_LabelDecl(FileDes *);
extern void Grammar_UnaryBinary(FileDes *);
extern void Grammar_DefineCst(FileDes *);
extern void Grammar_InPP(FileDes *);
extern void Grammar_MoveDeclAffect(FileDes *);
extern void Grammar_MoveDecl(FileDes *);
extern void Grammar(FileDes *);
extern void Grammar_InTemplate(FileDes *);
extern void Grammar_InEnum(FileDes *);

extern void Depend(void);
#define MAX_INCLUDE 5000

/*
 -----------------------------------------------------------------------------------------------------------------------
    Definition of an include file
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct
{
	char	asz_Name[255];
	int		i_NumInc;
	int		i_DepDir;
	int		i_Dep;
	int		ai_IncludeBy[MAX_INCLUDE];
	int		i_NumIncludeBy;
} tdst_Include;
extern tdst_Include gast_Includes[MAX_INCLUDE];
extern int			gi_NumIncludes;
#endif /* __GRAMMAR_H__ */
