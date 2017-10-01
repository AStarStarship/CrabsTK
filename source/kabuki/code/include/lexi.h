/*$T \Sources/lexi.h GC 1.150 2011-09-22 20:52:18 */


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
#ifndef __LEXI_H__
#define __LEXI_H__

#include "in.h"

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Consts
 -----------------------------------------------------------------------------------------------------------------------
 */

#define LEXI_C_MaxExtTokens 100 /* Max number of extended tokens */

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

typedef struct Lexi_tdst_ExtToken_
{
	char	*name;
	char	*as;
} Lexi_tdst_ExtToken;

typedef struct Lexi_tdst_Token_
{
	char	*name;
	int		id;
	int		subsubid;
} Lexi_tdst_Token;

extern Lexi_tdst_ExtToken	gast_ExtList[LEXI_C_MaxExtTokens];
extern int					gi_NumExtTokens;
extern char					*name_of_token[];

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Some macros...
 -----------------------------------------------------------------------------------------------------------------------
 */

#define Lisspace(c) (((c) == ' ') || ((c) == '\t'))
#define Lisword(c)	(isalnum(c) || ((c) == '_') || ((c) == '#'))
#define RETURN1(i)	{ g++; return(i); }
#define RETURN2(i)	{ g += 2; return(i); }
#define RETURN3(i)	{ g += 3; return(i); }

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    All tokens.
 -----------------------------------------------------------------------------------------------------------------------
 */

#define TOKEN_SPACE		1		/* ' ' \t */
#define TOKEN_EOL		2		/* \n */
#define TOKEN_CCMT		3		/* C comment */
#define TOKEN_CPPCMT	4		/* C++ comment */
#define TOKEN_PP		5		/* Preprocessor */
#define TOKEN_WORD		6		/* Word */
#define TOKEN_CST		7		/* Constant */
#define TOKEN_SPECWORD	8		/* Constant */

/* 9 */
#define TOKEN_AUTOOP		10	/* *= -= += >>= <<= &= |= /= %= ^= */
#define TOKEN_DOUBLECOLON	11	/* :: */
#define TOKEN_POINTER		12	/* -> */
#define TOKEN_AUTOSIGN		13	/* -- ++ */
#define TOKEN_COMPARE		14	/* == != >= <= */
#define TOKEN_OPER			15	/* >> << / % ^ */
#define TOKEN_LINK			16	/* && || */
#define TOKEN_STRINGCST		17	/* "..." */
#define TOKEN_CHARCST		18	/* '...' */
#define TOKEN_NUMBER		19	/* A number */
#define TOKEN_ELLIPSIS		20	/* ... */
#define TOKEN_ADDRESS		21	/* & */
#define TOKEN_LESS			22	/* < */
#define TOKEN_GREAT			23	/* > */

/*
 * 24 ;
 * 25 ;
 * 26 ;
 * 27 ;
 * 28
 */
#define TOKEN_BLANK_LINE	29	/* * */
#define TOKEN_STAR			30	/* * */
#define TOKEN_SEMICOL		31
#define TOKEN_LPAREN		32	/* ( */
#define TOKEN_RPAREN		33	/* ) */
#define TOKEN_LARRAY		34	/* [ */
#define TOKEN_RARRAY		35	/* ] */
#define TOKEN_LBRACE		36	/* { */
#define TOKEN_RBRACE		37	/* } */
#define TOKEN_QUESTION		38	/* ? */
#define TOKEN_COLON			39	/* : */
#define TOKEN_COMMA			40	/* , */
#define TOKEN_POINT			41	/* . */
#define TOKEN_SIGN			42	/* - + */
#define TOKEN_AFFECT		43	/* = */
#define TOKEN_UNCHANGE		44	/* ! ~ */
#define TOKEN_BREAKLINE		45	/* \ */
#define TOKEN_OR			46	/* | */

/* Sub ID */
#define TOKEN_W_IF				100
#define TOKEN_W_ELSE			101
#define TOKEN_W_WHILE			102
#define TOKEN_W_DO				103
#define TOKEN_W_FOR				104
#define TOKEN_W_RETURN			105
#define TOKEN_W_BREAK			106
#define TOKEN_W_GOTO			107
#define TOKEN_W_SIZEOF			108
#define TOKEN_W_DEFAULT			109
#define TOKEN_W_CASE			110
#define TOKEN_W_DECL			111
#define TOKEN_W_SWITCH			112
#define TOKEN_W_ASM				113
#define TOKEN_W_NEW				114
#define TOKEN_W_DELETE			115
#define TOKEN_W_SINGLEWORD		116
#define TOKEN_W_CONTINUE		117
#define TOKEN_W_SINGLEWORD0		118
#define TOKEN_W_CMT_GC_TAG		119
#define TOKEN_W_CMT_FILE		120
#define TOKEN_W_CMT_CLASS		121
#define TOKEN_W_CMT_FUNC		122
#define TOKEN_W_CMT_FUNC_PARAM	123
#define TOKEN_W_CMT_OPTION		124
#define TOKEN_W_TRY				125
#define TOKEN_W_CATCH			126

/* Subsub ID */
#define TOKEN_WW_STRUCT			150
#define TOKEN_WW_UNION			151
#define TOKEN_WW_TYPEDEF		152
#define TOKEN_WW_ENUM			153
#define TOKEN_WW_CLASS			154
#define TOKEN_WW_STATIC			155
#define TOKEN_WW_OPERATOR		156
#define TOKEN_WW_CONST			157
#define TOKEN_WW_CLASSACCESS	158
#define TOKEN_WW_TEMPLATE		159
#define TOKEN_WW_EXTERN			160
#define TOKEN_WW_SCAST			161
#define TOKEN_WW_THROWS			162
#define TOKEN_WW_THROW			163
#define TOKEN_WW_NAMESPACE		164

/* PP */
#define TOKEN_PP_IF				200
#define TOKEN_PP_ELSE			201
#define TOKEN_PP_ELIF			202
#define TOKEN_PP_UNDEF			203
#define TOKEN_PP_DEFINE			204
#define TOKEN_PP_PRAGMA			205
#define TOKEN_PP_ERROR			206
#define TOKEN_PP_IFDEF			207
#define TOKEN_PP_IFNDEF			208
#define TOKEN_PP_INCLUDE		209
#define TOKEN_PP_ENDIF			210
#define TOKEN_PP_LINE			211
#define TOKEN_W_MFC_BMESSAGEMAP 300
#define TOKEN_W_MFC_EMESSAGEMAP 301
#define TOKEN_UNKNOWCHAR		- 1

/*
 -----------------------------------------------------------------------------------------------------------------------
    Structure to define a token.
 -----------------------------------------------------------------------------------------------------------------------
 */
typedef struct token_
{
	int				i_ID;			/* ID of token (TOKEN_???) */
	int				i_SubID;		/* Sub ID */
	int				i_SubSubID;

	/*
	 * int i_Type;
	 * /* BitField for type
	 */
	int				line;			/* line number - start of token */
	int				column;			/* column number - start of token */

	char			*pc_Value;		/* String of token */
	struct token_	*pst_Prev;		/* To link tokens */
	struct token_	*pst_Next;

	/* Decl */
	char			BinaryOp;
	char			UnaryOp;
	char			IsType;			/* Type in a declaration */
	char			IsFctDefName;	/* Name in a function definition */
	char			IsFctDeclName;	/* Name in a function declaration */
	char			IsFctParamName; /* Name of a param in a function def/decl */
	char			EndListParam;
	char			IsFctCall;		/* Function call */
	char			InFct;			/* In function body */
	char			InFctDef;		/* In function definition */
	char			InFctDecl;		/* In function declaration */
	char			InParen;		/* Token is inside a parenthesis expression */
	char			WhileForDo;		/* A while for a do */
	struct token_	*OpenParen;		/* If InParen, address of open paren */
	char			BegStmtDecl;	/* Begin of a statement decl */
	char			EndStmtDecl;	/* End of a statement decl */
	char			InStmtDecl;		/* Inside a statement decl */
	char			DeclName;		/* Name in a declaration */
	char			BeginDecl;		/* Begin of a declaration statement */
	char			EndDecl;		/* End of a declaration statement */
	char			InitDecl;		/* Init in a declaration */
	char			LabelDecl;		/* This is a label */
	char			InClass;		/* Inside a class definition */
	char			UnSplitFctDef;	/* True if body is just after def of a function */
	char			NoBreak;		/* No EOL after that item */
	char			ColCase;		/* A ':' for a case */
	char			ClassDecl;		/* A "class" declaration */
	char			ForceSplit;
	char			InEnum;
	char			InDeclStmt;
	char			InConcatIf;		/* Concat if expression */

	/* Cmt */
	char			CppComment;		/* Is it a C++ comment ? */
	char			CmtIndent;		/* Comment has already been processed */
	char			AddCmt;			/* Comment has been added */
	char			SplitCmt;		/* Comment has been split */
	char			CmtLevel;		/* Comment level */
	char			NeedSplit;		/* When split char is found on a comment */
	char			EmptyCmt;		/* An empty comment */
	char			OptionCmt;		/* An option cmt */
	char			UnAlignCmt;
	char			WizardCmt;		/* App Wizard comment */
	char			WizardCmtEnd;	/* App Wizard comment */
	char			InWizardCmt;
	char			Splitted;		/* End of line comment split cause too long */
	int				doxygen;		/* doxygen first characters */

	/* Level */
	int				StmtLevel;		/* Statement level */
	int				IndentLevel;	/* Indentation level */
	int				Col;			/* Column */

	/* Divers */
	char			InPP;			/* Token in PP */
	char			In_PP_Else;		/* In a Else directive */
	char			In_PP_EndIf;	/* In a EndIf directive */
	char			In_PP_Pragma;	/* In a pragma directive */
	char			In_PP_If;		/* In a if/elif directive */
	char			In_PP_Include;	/* In include directive */
	char			In_PP_Define;	/* In define directive */
	char			DefineName;		/* Name of the define constant */
	char			MacroName;		/* Name of a macro */
	char			DefineCst;		/* Define constant */
	char			InASM;			/* In asm statement */
	char			IsCast;			/* It's a cast */
	char			NoIndent;		/* Do not indent that token */
	char			NoSplit;		/* Do not split that token */
	char			EmptyCmtBefore; /* There's an empty comment beforetoken */
	char			CmtAccess;		/* A comment added before an access */
	char			NoCmt2;
	char			InTemplate;
	int				PPLevel;
	char			NoConcat;
	char			UnClassDes;
	char			PubClassDecl;
	char			EndAlign;		/* Mark end of align */
	char			AlignBody;		/* Force fct body to be concat/align */

	/* Input */
	int				WasSpaceAfter;	/* Number of spaces after token in input file */
	int				WasEOLAfter;	/* Number of EOL after token in input file */

	/* For output */
	char			ForceSpaceAfter;
	char			ForceEOLAfter;
	int				AddSpaceAfter;
	int				AddSpaceBefore;
} token;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------
 */

struct FileDes_;
extern void Lexi(struct FileDes_ *);
#endif /* __LEXI_H__ */
