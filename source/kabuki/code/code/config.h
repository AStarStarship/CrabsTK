/*$T \Sources/config.h GC 1.150 2011-09-22 20:52:17 */


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
#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifdef CONFIG_GLOB
#define EXTERN
#else
#define EXTERN	extern
#endif

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Current version and revision
 -----------------------------------------------------------------------------------------------------------------------
 */

#define VERSION		1
#define REVISION	150

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Consts
 -----------------------------------------------------------------------------------------------------------------------
 */

#define MAX_DEF_FILESDIRS	2000	/* Max number of files/dirs */
#define MAX_DEF_CATEG		50		/* Max number of categories in fonction comment */
#define MAX_DEF_REPLACE		2000	/* Max number of categories in fonction comment */
#define MAX_DEF_FILEINSERT	10		/* Max number of file insertion */

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Config struct
 -----------------------------------------------------------------------------------------------------------------------
 */

typedef struct tdst_Config_
{
	int CanOut;					/* No generate output files */
	int Verbose;				/* Display infos during process */
	int CanBak;					/* No .bak file */
	int TagFile;				/* Put a GC! mark at the beginning of the file */
	int TabSize;				/* Number of spaces for a tabulation */
	int OutTab;					/* Generate tab in output file */
	int LineLenCode;			/* Max length of line for code */
	int LineLenCmt;
	int LineLenCmtSepDecl;		/* Max length for separators of decls */
	int LineLenCmtFirstLine;	/* Max length comment on first line */
	int LineLenSplitInit;		/* Max length of line for static init split */
	int FileCmt;				/* Add a file comment at the beginning */
	int CmtSepDeclBefore;		/* A comment before the statement */
	int CmtSepDeclAutoLen;		/* Compute len of separator after decl */
	int CmtSepDeclAutoLenAdd;	/* Add when auto len */
	int CmtSeparatorStar;		/* Add the star for separators */
	int CmtDeclStmt;			/* Surround decl statements with comments separators */
	int CmtFirstLineBreakFirst; /* EOL after first cmt mark */
	int CmtFirstLineBreakLast;	/* Idem last mark */
	int CmtFirstLineFillStar;	/* Fill cmt first line with '*' */
	int CmtKeepCpp;				/* Keep C++ comments */
	int ForceFctDefDeclSplit;	/* Force function defintions and declartions to split */
	int ForceBrace;				/* To force brace around statements */
	int ConcatIf;				/* Concat if, while, for */
	int MoveDeclAffect;			/* Move affect in decl */
	int MoveDeclToTop;			/* Move decl at the top of the statement */
	int KeepEmptyLinesCode;		/* Keep empty lines in code */
	int SpaceIf;				/* Space after if, while etc. */
	int SpaceFctCall;			/* Space after a function call */
	int ReplaceOn;				/* Replace ON */
	int CharFctCmt;				/* Separator for function comments */
	int CharDeclCmt;			/* Separator for decl statements */
	int CharCmt1;				/* Char for blocked comment (level 1) */
	int CharCmt2;
	int CharCmt3;
	int CharCmt4;
	int CharCmt5;
	int CharCmt6;
	int EOLBeforeCmt1;
	int EOLAfterCmt1;
	int EOLBeforeCmt2;
	int EOLAfterCmt2;
	int EOLBeforeCmt3;
	int EOLAfterCmt3;
	int EOLBeforeCmt4;
	int EOLAfterCmt4;
	int EOLBeforeCmt5;
	int EOLAfterCmt5;
	int EOLBeforeCmt6;
	int EOLAfterCmt6;
	int CharSplit;				/* The char that force a split */
	int Dependencies;			/* Test dependencies */
	int DependenciesAll;		/* Test dependencies */
	int AddCmtFct;				/* Add empty comments before functions */
	int AddCmtFctClass;			/* Idem inline class */
	int TrailingCmtStyle;		/* Trailing comment style */
	int AddCmtClassAccess;
	int SplitBoolBefore;
	int AutoSepFctDecl;
	int AutoSepFctDef;
	int AutoSepMacro;
	int AutoSepClassAccess;
	int AutoSepStruct;
	int AutoSepClass;
	int CmtDeclMaxLevel;
	int NoCmtIndent;
	int ConcatInlineClass;
	int ConcatSwitch;
	int LeftDecl;
	int WizardIndent;
	int BreakTemplate;
	int ConcatMacros;
	int ReturnAddParen;
	int CppToCKeepEOL;
	int ConcatEnum;
	int MaxAlignSpaces;
	int ConcatFirstLineCmt;
	int FirstLineBlank;
	int OutTest;
	int MaxAlignCmt;
	int RemoveEOLDecl;
	int RemoveConcatIf;
	int BraceStyle;
	int BraceStyleFct;
	int BraceStyleDecl;
	int BraceStyleClass;
	int PPToCode;
	int SpaceParen;
	int SpaceCast;
	int EOLAlone;
	int EndBlanks;
	int ReturnDelParen;
	int CmtSepBreak;
	int SpaceCastAfter;
	int SwitchStyle;
	int RemoveEmptyLinesCode;
	int PPStyle;
	int SpaceScopeDecl;
	int SpaceScopeUse;
	int SpaceCmtCpp;
	int AddVoid;
	int BreakFctType;
	int IncludeLin;
	int ConcatStrings;
	int EmptyFctBlanks;
	int AccessBlanks;
	int CatchBlanksBefore;
	int AccessBlanksBefore;
	int LabelBlanks;
	int EolBeforeFctCmt;
	int IndentClass;
	int UnixEOL;
	int SplitFctCall;
	int SplitFctDef;
	int SplitFctDecl;
	int SplitDecl;
	int SpaceFirstCall;
	int SpaceFirstFctDecl;
	int SpaceFirstFctDef;
	int SpaceLastCall;
	int SpaceInCall;
	int SpaceLastFctDecl;
	int SpaceLastFctDef;
	int AffectStyle;
	int AutoAffectStyle;
	int CToCpp;
	int SpaceReturn;
	int ConstructStyle;
	int SpaceFctDecl;
	int SpaceFctDef;
	int ConcatElse;
	int IndentExternC;
	int IndentNamespace;
	int ForSplitStyle;
	int IfSplitStyle;
	int DepRec;
	int FileCmtStyle;
	int AlignBL;
	int ReadOnly;
	int SwitchEol;
	int BreakDoWhile;
	int EOLBeforeCmt;
	int EOLAfterCmt;
	int ConcatIfAndElse;
	int EOLAfterEveryCloseBrace;
	int EOLBetweenEmptyBraces;
	int ConcatElseIf;
	int TryHarderToKeepEmptyLines;
	int StaticInit;
	int doxygen;
} tdst_Config;
extern tdst_Config	Config;
extern tdst_Config	Config1;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Categories for function comment
 -----------------------------------------------------------------------------------------------------------------------
 */

EXTERN struct
{
	char	*mpsz_Name;
	int		mi_Type;
} gast_CmtCateg[MAX_DEF_CATEG];
EXTERN int	gi_NumCmtCateg;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Categories for function comment
 -----------------------------------------------------------------------------------------------------------------------
 */

EXTERN char ReplaceFirst[256];
EXTERN struct
{
	char	*mpsz_Name1;
	char	*mpsz_Name2;
} gast_Replace[MAX_DEF_REPLACE];
EXTERN int	gi_NumReplace;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Global
 -----------------------------------------------------------------------------------------------------------------------
 */

EXTERN char *gapsz_FileInsert[MAX_DEF_FILEINSERT];
EXTERN int	gi_NumFileInsert;

extern char *gz_LstFileName;
extern char *gz_CfgFileName;
extern char *gz_FixmeComment;

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Prototypes
 -----------------------------------------------------------------------------------------------------------------------
 */

extern int	ConvertFile(char *);
extern void ConvertFileToArgv(FILE *, int *);
extern void Usage(void);
extern void Default(void);
extern void Options(int, char *[]);
extern void CheckConfig(void);

#ifdef CONFIG_GLOB
#define CONFIG_GLOB
#undef EXTERN
#endif
#endif /* __CONFIG_H__ */
