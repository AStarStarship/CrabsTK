/*$T \Sources/config.c GC 1.150 2011-09-22 20:52:17 */


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

#define CONFIG_GLOB /* generate the global instances of items within config.h (instead of defining them as external */
#include "config.h"
#include "in.h"
#include "error.h"
#include "tools.h"
#include "os.h"

/*$2
 -----------------------------------------------------------------------------------------------------------------------
    Consts
 -----------------------------------------------------------------------------------------------------------------------
 */

#define TYPE_OPT_BOOL		1
#define TYPE_OPT_INT		3
#define TYPE_OPT_DIR		4
#define TYPE_OPT_DIREX		5
#define TYPE_OPT_FILE		6
#define TYPE_OPT_FILEEX		7
#define TYPE_OPT_CTGFXCMT	8
#define TYPE_OPT_CTGFXCMT1	9
#define TYPE_OPT_CHAR		10
#define TYPE_OPT_REPLACE	11
#define TYPE_OPT_DEPEND		12
#define TYPE_OPT_EXTTOKEN	13
#define TYPE_OPT_INSERT		14
#define TYPE_OPT_FIXME_CMT	15
#define TYPE_OPT_CFGFILE	16

#define MAX_DEF_PARAMS		1000
char		*gargv[MAX_DEF_PARAMS];

tdst_Config Config;
tdst_Config Config1;

char		*gz_LstFileName = "gc.lst";
char		*gz_CfgFileName = "gc.cfg";
char		*gz_FixmeComment = "/* */";

/*
 -----------------------------------------------------------------------------------------------------------------------
    All options
 -----------------------------------------------------------------------------------------------------------------------
 */
struct
{
	char	*name;
	char	type;
	int		*add;
	int		valdefault;
	int		valmin;
	char	*help;
}
/*$off*/
gast_Options[] =
{
	{ "in_cfg-",							TYPE_OPT_CFGFILE,	NULL, 0, 0, "Read file.cfg to get options"},
	{ "dir-",								TYPE_OPT_DIR,		NULL, 0, 0, "Process all files of that directory (recursively)"},
	{ "direx-",								TYPE_OPT_DIREX,		NULL, 0, 0, "Exclude directory of process"},
	{ "file-",								TYPE_OPT_FILE,		NULL, 0, 0, "Process file"},
	{ "fileex-",							TYPE_OPT_FILEEX,	NULL, 0, 0, "Exclude file of process"},
	{ "file_insert-",						TYPE_OPT_INSERT,	NULL, 0, 0, "Add an include file for /*$I<num> */ comment. Order is important"},
	{ "file_end_eol-",						TYPE_OPT_INT,		&Config.EndBlanks, 1, 0, "Number of EOL after last token of file"},
	{ "output-",							TYPE_OPT_BOOL,		&Config.CanOut, 1, 0,  "Write result to file"},
	{ "output_test-",						TYPE_OPT_BOOL,		&Config.OutTest, 0, 0,	"Write result to file_test.???"},
	{ "overwrite_read_only-",				TYPE_OPT_BOOL,		&Config.ReadOnly, 0, 0, "Can process read only files (change status)"},
	{ "verbose-",							TYPE_OPT_BOOL,		&Config.Verbose, 1, 0,	"Display processing messages"},
	{ "bak-",								TYPE_OPT_BOOL,		&Config.CanBak, 0, 0, "Generate a backup file (inputfile.bak)"},
	{ "tab_size-",							TYPE_OPT_INT,		&Config.TabSize, 4, 0, "Number of blanks in an indentation level"},
	{ "tab_out-",							TYPE_OPT_BOOL,		&Config.OutTab, 1, 0, "Generate tabulations in input file instead of blanks"},
#ifdef _WIN32
	{ "eol_unix-",							TYPE_OPT_BOOL,		&Config.UnixEOL, 0, 0, "Generate unix style EOL"},
#else
	{ "eol_unix-",							TYPE_OPT_BOOL,		&Config.UnixEOL, 1, 0, "Generate unix style EOL"},
#endif
	{ "space_if-",							TYPE_OPT_BOOL,		&Config.SpaceIf, 0, 0, "Add a blank after if, while, for, do"},
	{ "space_return-",						TYPE_OPT_BOOL,		&Config.SpaceReturn, 0, 0, "Add a blank after return"},
	{ "space_fctcall-",						TYPE_OPT_BOOL,		&Config.SpaceFctCall, 0, 0, "Add a blank after a function call"},
	{ "space_fctcall_firstparam-",			TYPE_OPT_BOOL,		&Config.SpaceFirstCall, 0, 0, "Force a space before the first parameter of a function call"},
	{ "space_fctcall_lastparam-",			TYPE_OPT_BOOL,		&Config.SpaceLastCall, 0, 0, "Force a space after the last parameter of a function call"},
	{ "space_fctcall_inparam-",				TYPE_OPT_BOOL,		&Config.SpaceInCall, 1, 0, "Force a space after all the inside parameters of a function call"},
	{ "space_fctdecl_firstparam-",			TYPE_OPT_BOOL,		&Config.SpaceFirstFctDecl, 0, 0, "Force a space before the first parameter of a function decl"},
	{ "space_fctdecl_lastparam-",			TYPE_OPT_BOOL,		&Config.SpaceLastFctDecl, 0, 0, "Force a space after the last parameter of a function decl"},
	{ "space_fctdef_firstparam-",			TYPE_OPT_BOOL,		&Config.SpaceFirstFctDef, 0, 0, "Force a space before the first parameter of a function def"},
	{ "space_fctdef_lastparam-",			TYPE_OPT_BOOL,		&Config.SpaceLastFctDef, 0, 0, "Force a space after the last parameter of a function def"},
	{ "space_fctdecl-",						TYPE_OPT_BOOL,		&Config.SpaceFctDecl, 0, 0, "Add a blank after a function declaration name"},
	{ "space_fctdef-",						TYPE_OPT_BOOL,		&Config.SpaceFctDef, 0, 0, "Add a blank after a function definition name"},
	{ "space_paren-",						TYPE_OPT_INT,		&Config.SpaceParen, 0, 0, "Number of spaces after '(' and before ')' if nested level of parentheses is less than argument"},
	{ "space_cast-",						TYPE_OPT_BOOL,		&Config.SpaceCast, 0, 0, "Add spaces after '(' and before ')' for cast operators"},
	{ "space_cast_after-",					TYPE_OPT_BOOL,		&Config.SpaceCastAfter, 1, 0, "Force a space after a cast expression"},
	{ "space_scope_def-",					TYPE_OPT_BOOL,		&Config.SpaceScopeDecl, 0, 0, "Force a space before and after :: in declaration"},
	{ "space_scope_access-",				TYPE_OPT_BOOL,		&Config.SpaceScopeUse, 0, 0, "Force a space before and after :: when accessing"},
	{ "space_affect_style-",				TYPE_OPT_INT,		&Config.AffectStyle, 0, 0, "Style of spaces for affect operators"},
	{ "space_autoaffect_style-",			TYPE_OPT_INT,		&Config.AutoAffectStyle, 0, 0, "Style of spaces for auto-affect operators"},
	{ "code_eol_after_close_brace-",		TYPE_OPT_INT,		&Config.EOLAfterEveryCloseBrace, 0, 0, "Number of blank lines after every close brace - except ones followed by else, while, etc"},
	{ "code_len-",							TYPE_OPT_INT,		&Config.LineLenCode, 120, 8, "Maximum length of a line of code"},
	{ "code_keep_empty_lines-",				TYPE_OPT_BOOL,		&Config.KeepEmptyLinesCode, 1, 0, "Keep or remove empty lines (no more than one line)"},
	{ "code_keep_more_empty_lines-",		TYPE_OPT_BOOL,		&Config.TryHarderToKeepEmptyLines, 0, 0, "Try harder to keep existing empty lines"},
	{ "code_remove_empty_lines-",			TYPE_OPT_INT,		&Config.RemoveEmptyLinesCode, 2, 1, "Remove all excess empty lines to the number specified between lines"},
	{ "code_split_bool_before-",			TYPE_OPT_BOOL,		&Config.SplitBoolBefore, 1, 0, "Prefer to split long expression before boolean operators"},
	{ "code_split_fctcall_style-",			TYPE_OPT_INT,		&Config.SplitFctCall, 0, 0, "Split style of too long function calls"},
	{ "code_split_fctdef_style-",			TYPE_OPT_INT,		&Config.SplitFctDef, 0, 0, "Split style of too long function definitions"},
	{ "code_split_fctdecl_style-",			TYPE_OPT_INT,		&Config.SplitFctDecl, 0, 0, "Split style of too long function declarations"},
	{ "code_split_for_style-",				TYPE_OPT_INT,		&Config.ForSplitStyle, 0, 0, "Split style of too long for statements"},
	{ "code_split_if_style-",				TYPE_OPT_INT,		&Config.IfSplitStyle, 0, 0, "Split style of too long if statements"},
	{ "code_split_decl_style-",				TYPE_OPT_INT,		&Config.SplitDecl, 0, 0, "Split style of variable declarations"},
	{ "code_constructor_style-",			TYPE_OPT_INT,		&Config.ConstructStyle, 0, 0, "Split style of C++ constructors definition"},
	{ "code_decl_move_affect-",				TYPE_OPT_BOOL,		&Config.MoveDeclAffect, 0, 0, "Move affection in a declaration after the declaration bloc"},
	{ "code_decl_move_top-",				TYPE_OPT_BOOL,		&Config.MoveDeclToTop, 0, 0, "Move all declarations to the top of the statement"},
	{ "code_decl_access_to_type-",			TYPE_OPT_BOOL,		&Config.LeftDecl, 0, 0, "Move & and * just after the type when possible"},
	{ "code_decl_break_template-",			TYPE_OPT_BOOL,		&Config.BreakTemplate, 1, 0, "Force a break line after a template declaration"},
	{ "code_decl_add_void-",				TYPE_OPT_BOOL,		&Config.AddVoid, 0, 0, "Force a \"void\" in function declaration if nothing is specified"},
	{ "code_wizard_indent-",				TYPE_OPT_BOOL,		&Config.WizardIndent, 1, 0, "Indent code between devstudio appwizard special comments"},
	{ "code_force_return_paren-",			TYPE_OPT_BOOL,		&Config.ReturnAddParen, 0, 0, "Force enclose parenthesis in a return expression"},
	{ "code_remove_return_paren-",			TYPE_OPT_BOOL,		&Config.ReturnDelParen, 0, 0, "Remove all parentheses around a return parameter"},
	{ "code_align_max_blanks-",				TYPE_OPT_INT,		&Config.MaxAlignSpaces, 1000, 1, "The max number of blanks to add for a declaration alignment"},
	{ "code_def_fct_break_return_type-",	TYPE_OPT_BOOL,		&Config.BreakFctType, 0, 0, "Force a line break after the return type in a function definition"},
	{ "code_concat_strings-",				TYPE_OPT_BOOL,		&Config.ConcatStrings, 1, 0, "Concat adjacent string constants"},
	{ "code_empty_fct_blanks-",				TYPE_OPT_INT,		&Config.EmptyFctBlanks, 1, 0, "Number of blank lines in empty functions"},
	{ "catch_eol_before-",					TYPE_OPT_INT,		&Config.CatchBlanksBefore, 1, 0, "Number of eol before catch"},
	{ "code_class_access_eol_before-",		TYPE_OPT_INT,		&Config.AccessBlanksBefore, 1, 0, "Number of eol before class access"},
	{ "code_class_access_eol_after-",		TYPE_OPT_INT,		&Config.AccessBlanks, 1, 0, "Number of eol after class access"},
	{ "code_labels_eol_after-",				TYPE_OPT_INT,		&Config.LabelBlanks, 1, 0, "Number of eol after labels"},
	{ "stmt_force_brace-",					TYPE_OPT_INT,		&Config.ForceBrace, 100, 0, "Force a statement to be enclose with braces if its length exceed parameter"},
	{ "stmt_break_alone-",					TYPE_OPT_BOOL,		&Config.EOLAlone, 0, 0, "Force an empty statement to be alone on its line"},
	{ "stmt_break_dowhile-",				TYPE_OPT_BOOL,		&Config.BreakDoWhile, 0, 0, "Force a while of a do to be alone on its line"},
	{ "stmt_concat_if-",					TYPE_OPT_BOOL,		&Config.ConcatIf, 1, 0, "Concat if, while, for for single statements"},
	{ "stmt_concat_else_2_stmt-",			TYPE_OPT_BOOL,		&Config.ConcatElse, 0, 0, "Concat else after the } statement"},
	{ "stmt_concat_if_and_else-",			TYPE_OPT_BOOL,		&Config.ConcatIfAndElse, 0, 0, "Concat if ... else ... constructs onto two lines - if possible"},
	{ "stmt_concat_else_if-",				TYPE_OPT_BOOL,		&Config.ConcatElseIf, 1, 0, "Concat else and if into else if"},
	{ "stmt_concat_inline_class-",			TYPE_OPT_BOOL,		&Config.ConcatInlineClass, 0, 0, "Concat if possible inline function body inside a class"},
	{ "stmt_concat_switch-",				TYPE_OPT_BOOL,		&Config.ConcatSwitch, 1, 0, "Concat all cases of a switch if possible"},
	{ "stmt_concat_macros-",				TYPE_OPT_BOOL,		&Config.ConcatMacros, 1, 0, "Concat a macro statement if possible"},
	{ "stmt_concat_enum-",					TYPE_OPT_BOOL,		&Config.ConcatEnum, 1, 0, "Concat content of enum if possible"},
	{ "stmt_decl_remove_empty-",			TYPE_OPT_BOOL,		&Config.RemoveEOLDecl, 1, 0, "Remove empty lines in declaration statements"},
	{ "stmt_concat_if_remove_empty-",		TYPE_OPT_BOOL,		&Config.RemoveConcatIf, 1, 0, "Remove empty lines between concat if/while/for"},
	{ "stmt_brace_style_class-",			TYPE_OPT_INT,		&Config.BraceStyleClass, 0, 0, "Change indentation style of braces for class declaration"},
	{ "stmt_brace_style_fct-",				TYPE_OPT_INT,		&Config.BraceStyleFct, 0, 0, "Change indentation style of braces for function body"},
	{ "stmt_brace_style_decl-",				TYPE_OPT_INT,		&Config.BraceStyleDecl, 0, 0, "Change indentation style of braces for declarations (struct, enum)"},
	{ "stmt_brace_style-",					TYPE_OPT_INT,		&Config.BraceStyle, 0, 0, "Change indentation style of braces for all other statements (if, while...)"},
	{ "stmt_switch_style-",					TYPE_OPT_INT,		&Config.SwitchStyle, 0, 0, "Change indentation style of switch"},
	{ "stmt_switch_eol-",					TYPE_OPT_INT,		&Config.SwitchEol, 0, 0, "Change style of lines between cases"},
	{ "stmt_class_indent-",					TYPE_OPT_INT,		&Config.IndentClass, 0, 0, "Number of additional indentation levels in a class declaration"},
	{ "stmt_namespace_indent-",				TYPE_OPT_BOOL,		&Config.IndentNamespace, 0, 0, "Indent one level a namespace statement"},
	{ "stmt_extern_c_indent-",				TYPE_OPT_BOOL,		&Config.IndentExternC, 0, 0, "Indent one level an extern c statement"},
	{ "stmt_static_init_style-",			TYPE_OPT_INT,		&Config.StaticInit, 0, 0, "Style of static initializations"},
	{ "stmt_static_init_len-",				TYPE_OPT_INT,		&Config.LineLenSplitInit, 80, 0, "Max length of line when concat static init"},
	{ "pp_align_to_code-",					TYPE_OPT_BOOL,		&Config.PPToCode, 0, 0, "Align or not PP directive to the code just below"},
	{ "pp_style-",							TYPE_OPT_INT,		&Config.PPStyle, 0, 0, "Set indentation style of PP directives"},
	{ "pp_include_unix-",					TYPE_OPT_BOOL,		&Config.IncludeLin, 1, 0, "Change backslash to slash in include directives"},
	{ "pp_align_breakline-",				TYPE_OPT_BOOL,		&Config.AlignBL, 0, 0, "Align break lines in multi lines macros"},
	{ "cmt_fixme-",							TYPE_OPT_FIXME_CMT, NULL, 0, 0, "Specify the string for FIXME comment the default is \"/* FIXME: Comment */\""},
	{ "cmt_align_max_blanks-",				TYPE_OPT_INT,		&Config.MaxAlignCmt, 20, 1, "The max number of blanks to add for last line comments"},
	{ "cmt_dont_modify-",					TYPE_OPT_BOOL,		&Config.NoCmtIndent, 1, 0, "Never touched comments (like $F directive but for all comments)"},
	{ "cmt_add_gc_tag-",					TYPE_OPT_BOOL,		&Config.TagFile, 1, 0, "Add a /*$T */ mark at the beginning of file (if not already present)"},
	{ "cmt_add_file-",						TYPE_OPT_BOOL,		&Config.FileCmt, 1, 0, "Add a special comment at the beginning of file (if not already present)"},
	{ "cmt_add_file_style-",				TYPE_OPT_INT,		&Config.FileCmtStyle, 0, 0, "Special comment style at the beginning of file"},
	{ "cmt_add_fct_def-",					TYPE_OPT_BOOL,		&Config.AddCmtFct, 1, 0, "Add an empty comment before function definition (if not already present)"},
	{ "cmt_add_fct_def_class-",				TYPE_OPT_BOOL,		&Config.AddCmtFctClass, 1, 0, "Add an empty comment before inline function in class (if not already present)"},
	{ "cmt_trailing_style-",				TYPE_OPT_INT,		&Config.TrailingCmtStyle, 0, 0, "Trailing comment style"},
	{ "cmt_add_class_access-",				TYPE_OPT_BOOL,		&Config.AddCmtClassAccess, 1, 0, "Add an empty comment before class access (if not already present)"},
	{ "cmt_first_space_cpp-",				TYPE_OPT_BOOL,		&Config.SpaceCmtCpp, 1, 0, "Force a space after the opening comment delimiter '//'"},
	{ "cmt_keep_cpp-",						TYPE_OPT_BOOL,		&Config.CmtKeepCpp, 0, 0, "Keep C++ comments in code"},
	{ "cmt_force_fct_def_decl_split-",		TYPE_OPT_BOOL,		&Config.ForceFctDefDeclSplit, 0, 0, "Force function defintions and declartions to split"},
	{ "cmt_c2cpp-",							TYPE_OPT_BOOL,		&Config.CToCpp, 0, 0, "Convert all C comments to the C++ form"},
	{ "cmt_cpp2c_keep_eol-",				TYPE_OPT_BOOL,		&Config.CppToCKeepEOL, 1, 0, "When converting C++ comments to C, replace EOL with GC break line character)"},
	{ "cmt_fct_categ-",						TYPE_OPT_CTGFXCMT,	NULL, 0, 0, "Add a special keyword for function declaration comments"},
	{ "cmt_fct_categ_in-",					TYPE_OPT_CTGFXCMT1, NULL, 0, 0, "Add a special keyword for parameters description in a function declaration comment"},
	{ "cmt_decl-",							TYPE_OPT_BOOL,		&Config.CmtDeclStmt, 0, 0, "Add separators in local variable declaration (before and/or after)"},
	{ "cmt_decl_max_level-",				TYPE_OPT_INT,		&Config.CmtDeclMaxLevel, 100, 0, "-cmt_decl- option is valid for declaration in a statement level < that value"},
	{ "cmt_decl_before-",					TYPE_OPT_BOOL,		&Config.CmtSepDeclBefore, 1, 0, "Add a separator before local declarations"},
	{ "cmt_decl_len-",						TYPE_OPT_INT,		&Config.LineLenCmtSepDecl, 120, 0, "Length of separators in a local declaration"},
	{ "cmt_decl_auto_len-",					TYPE_OPT_BOOL,		&Config.CmtSepDeclAutoLen, 1, 0, "Compute length of the declaration separator depending of code"},
	{ "cmt_decl_auto_len_add-",				TYPE_OPT_INT,		&Config.CmtSepDeclAutoLenAdd, 0, 0, "Add additional blanks to that length"},
	{ "cmt_first_line_break_first-",		TYPE_OPT_BOOL,		&Config.CmtFirstLineBreakFirst, 1, 0, "Add an EOL after begin mark of a first line comment"},
	{ "cmt_first_line_break_last-",			TYPE_OPT_BOOL,		&Config.CmtFirstLineBreakLast, 1, 0, "Add an EOL before end mark of a first line comment"},
	{ "cmt_first_line_fill_star-",			TYPE_OPT_BOOL,		&Config.CmtFirstLineFillStar, 1, 0, "Add a star at the beginning of each lines of a first line comment" },
	{ "cmt_first_line_len-",				TYPE_OPT_INT,		&Config.LineLenCmtFirstLine, 80, 8, "Maximum length of a first line comment"},
	{ "cmt_first_line_concat-",				TYPE_OPT_BOOL,		&Config.ConcatFirstLineCmt, 1, 0, "Concat adjacent first line comments"},
	{ "cmt_first_line_blank-",				TYPE_OPT_BOOL,		&Config.FirstLineBlank, 1, 0, "Add an empty line between two adjacent first line comments"},
	{ "cmt_sep_len-",						TYPE_OPT_INT,		&Config.LineLenCmt, 120, 8, "Maximum length for separators"},
	{ "cmt_sep_fill_star-",					TYPE_OPT_BOOL,		&Config.CmtSeparatorStar, 0, 0, "Add a star at the beginning of all lines of a separator"},
	{ "cmt_sep_break-",						TYPE_OPT_BOOL,		&Config.CmtSepBreak, 1, 0, "Force /* and */ to be alone on their lines for separators"},
	{ "cmt_sep_char_1-",					TYPE_OPT_CHAR,		&Config.CharCmt1, (int) '~', 0, "Filling character for /*$1 */ separator"},
	{ "cmt_sep_char_2-",					TYPE_OPT_CHAR,		&Config.CharCmt2, (int) '-', 0, "Filling character for /*$2 */ separator"},
	{ "cmt_sep_char_3-",					TYPE_OPT_CHAR,		&Config.CharCmt3, (int) '=', 0, "Filling character for /*$3 */ separator"},
	{ "cmt_sep_char_4-",					TYPE_OPT_CHAR,		&Config.CharCmt4, (int) '*', 0, "Filling character for /*$4 */ separator"},
	{ "cmt_sep_char_5-",					TYPE_OPT_CHAR,		&Config.CharCmt5, (int) '#', 0, "Filling character for /*$5 */ separator"},
	{ "cmt_sep_char_6-",					TYPE_OPT_CHAR,		&Config.CharCmt6, (int) '+', 0, "Filling character for /*$6 */ separator"},
	{ "cmt_sep_char_split-",				TYPE_OPT_CHAR,		&Config.CharSplit, (int) ';', 0, "Change the GC break line character in comments"},
	{ "cmt_sep_eol_before-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt, 0, 0, "Number of blank lines before a single line comment"},
	{ "cmt_sep_eol_after-",					TYPE_OPT_INT,		&Config.EOLAfterCmt, 0, 0, "Number of blank lines after a single line comment"},
	{ "cmt_sep_eol_before_1-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt1, 1, 0, "Number of blank lines before a comment separator /*$1 ... */"},
	{ "cmt_sep_eol_after_1-",				TYPE_OPT_INT,		&Config.EOLAfterCmt1, 1, 0, "Number of blank lines after a comment separator /*$1 ... */"},
	{ "cmt_sep_eol_before_2-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt2, 1, 0,"Number of blank lines before a comment separator /*$2 ... */"},
	{ "cmt_sep_eol_after_2-",				TYPE_OPT_INT,		&Config.EOLAfterCmt2, 1, 0, "Number of blank lines after a comment separator /*$2 ... */"},
	{ "cmt_sep_eol_before_3-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt3, 1, 0, "Number of blank lines before a comment separator /*$3 ... */"},
	{ "cmt_sep_eol_after_3-",				TYPE_OPT_INT,		&Config.EOLAfterCmt3, 1, 0, "Number of blank lines after a comment separator /*$3 ... */"},
	{ "cmt_sep_eol_before_4-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt4, 1, 0, "Number of blank lines before a comment separator /*$4 ... */"},
	{ "cmt_sep_eol_after_4-",				TYPE_OPT_INT,		&Config.EOLAfterCmt4, 1, 0, "Number of blank lines after a comment separator /*$4 ... */"},
	{ "cmt_sep_eol_before_5-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt5, 1, 0, "Number of blank lines before a comment separator /*$5 ... */"},
	{ "cmt_sep_eol_after_5-",				TYPE_OPT_INT,		&Config.EOLAfterCmt5, 1, 0, "Number of blank lines after a comment separator /*$5 ... */"},
	{ "cmt_sep_eol_before_6-",				TYPE_OPT_INT,		&Config.EOLBeforeCmt6, 2, 0, "Number of blank lines before a comment separator /*$6 ... */"},
	{ "cmt_sep_eol_after_6-",				TYPE_OPT_INT,		&Config.EOLAfterCmt6, 2, 0, "Number of blank lines after a comment separator /*$6 ... */"},
	{ "cmt_sep_eol_before_fct_def-",		TYPE_OPT_INT,		&Config.EolBeforeFctCmt, 1, 0, "Number of blank lines before a function definition comment"},
	{ "cmt_sep_force_fct_proto-",			TYPE_OPT_INT,		&Config.AutoSepFctDecl, 0, 0, "Force a comment before a function prototype to be a separator. <num> is separator level, 0 for none"},
	{ "cmt_sep_force_fct_macro-",			TYPE_OPT_INT,		&Config.AutoSepMacro, 3, 0, "Force a comment before a macro to be a separator. <num> is separator level, 0 for none"},
	{ "cmt_sep_force_fct_def-",				TYPE_OPT_INT,		&Config.AutoSepFctDef, 3, 0, "Force a comment before a function to be a separator. <num> is separator level, 0 for none"},
	{ "cmt_sep_force_class_access-",		TYPE_OPT_INT,		&Config.AutoSepClassAccess, 2, 0, "Force a comment before a class access (public, private, protected) to be a separator. <num> is separator level, 0 for none"},
	{ "cmt_sep_force_struct-",				TYPE_OPT_INT,		&Config.AutoSepStruct, 2, 0, "Force a comment before a struct declaration. <num> is separator level, 0 for none"},
	{ "cmt_sep_force_class-",				TYPE_OPT_INT,		&Config.AutoSepClass, 3, 0,"Force a comment before a class declaration. <num> is separator level, 0 for none"},
	{ "cmt_doxygen-",						TYPE_OPT_BOOL,		&Config.doxygen, 1, 0,"Enable doxygen comments"},
	{ "token_ext-",							TYPE_OPT_EXTTOKEN,	NULL, 0, 0,"Force user keyword to be indent as C/C++ keyword"},
	{ "replace_on-",						TYPE_OPT_BOOL,		&Config.ReplaceOn, 1, 0, "Toggle replace mode on/off"},
	{ "replace-",							TYPE_OPT_REPLACE,	NULL, 0, 0, "Replace a word by another"},
	{ "dependencies-",						TYPE_OPT_BOOL,		&Config.Dependencies, 0, 0, "Activate process of dependencies"},
	{ "dependencies_all-",					TYPE_OPT_BOOL,		&Config.DependenciesAll, 0, 0, "Activate complex process of dependencies"},
	{ "dependencies_dir-",					TYPE_OPT_DEPEND,	NULL, 0, 0, "Add an include path for the processing of dependencies"},
	{ "dependencies_dir_rec-",				TYPE_OPT_BOOL,		&Config.DepRec, 0, 0, "Include path are recurs scan for the processing of dependencies"},
};

#define GAST_ENTRIES (sizeof(gast_Options) / sizeof(gast_Options[0]))

/*$on*/

/*
 =======================================================================================================================
    Usage: Print the Usage message
 =======================================================================================================================
 */
void Usage(void)
{
	/*~~~~~~~~~~~~~~*/
	int		i;
	char	asz[1028];
	char	asz1[512];
	int		len;
	/*~~~~~~~~~~~~~~*/

	printf("-------------------------------------------------------\n\n");
	printf("Syntax :\n");
	printf("[-no]-option-[parameter]  (current default value)\n");
	printf("-------------------------------------------------------\n");
	printf("No blank between optional [-no] and option\n");
	printf("No blank between option and its parameter\n");
	printf("Enclose the parameter with \"\" in case of blanks\n");
	printf("Example : GC -no-bak- -tab_size-8 -file-filename.c\n");
	printf("-------------------------------------------------------\n\n");
	for(i = 0; i < GAST_ENTRIES; i++)
	{
		if(gast_Options[i].type == TYPE_OPT_BOOL)
			sprintf(asz, "[-no]-%s", gast_Options[i].name);
		else
			sprintf(asz, "     -%s", gast_Options[i].name);

		switch(gast_Options[i].type)
		{
		case TYPE_OPT_BOOL:
			sprintf(asz1, "  (%s)", gast_Options[i].valdefault == 1 ? "true" : "false");
			strcat(asz, asz1);
			break;

		case TYPE_OPT_INT:
			sprintf(asz1, "<num>  (%d, min %d)", gast_Options[i].valdefault, gast_Options[i].valmin);
			strcat(asz, asz1);
			break;

		case TYPE_OPT_CHAR:
			sprintf(asz1, "<char>  (%c)", gast_Options[i].valdefault);
			strcat(asz, asz1);
			break;

		case TYPE_OPT_DIR:
		case TYPE_OPT_DIREX:
			strcat(asz, "<path>");
			break;

		case TYPE_OPT_CFGFILE:
		case TYPE_OPT_FILE:
		case TYPE_OPT_FILEEX:
			strcat(asz, "<filename>");
			break;

		case TYPE_OPT_REPLACE:
			strcat(asz, "<word to replace>$<replacement word>");
			break;

		case TYPE_OPT_CTGFXCMT:
		case TYPE_OPT_CTGFXCMT1:
			strcat(asz, "<word>");
			break;

		case TYPE_OPT_EXTTOKEN:
			strcat(asz, "<C/C++ keyword>$<user keyword>");
			break;

		case TYPE_OPT_INSERT:
			strcat(asz, "<path/file>");
			break;

		case TYPE_OPT_DEPEND:
			strcat(asz, "<path>");
			break;
		}

		if(gast_Options[i].help)
		{
			len = strlen(asz);
			while(len < 55)
			{
				strcat(asz, " ");
				len++;
			}

			strcat(asz, gast_Options[i].help);
		}

		printf("%s\n", asz);
	}

	printf("\n");
	printf("Configuration files (optional)\n");
	printf("-------------------------------\n\n");
	printf("GC.lst					You can specify a list of files to indent (one per line)\n");
	printf("                        Use -file-???? option syntax for each file\n");
	printf("GC.cfg			        You can specify a list of options (one per line)\n");

	printf("\n");
	printf("Special comments you can write in your code\n");
	printf("-------------------------------------------\n\n");
	printf("/*$<num> ... */         Insert separator number <num>\n");
	printf("                        The filling character must have been defined with a -cmt_sep_char_<num>- option\n");
	printf("/*$<num>- ... */        Insert a single line separator number <num>\n");
	printf("                        The filling character must have been defined with a -cmt_sep_char_<num>- option\n");
	printf("/*$T */                 Add GC mark (GC version, file, date, time) at the beginning of file\n");
	printf("/*$F ... */             Do not indent the comment\n");
	printf("/*$off*/                Do not indent code below\n");
	printf("/*$on*/                 Indent code below\n");
	printf("/*$I<num> */            Insert file number <num> in the comment\n");
	printf("                        The file must have been defined with the -file_insert- option\n");
	printf("/*$S ... */             Force a first line comment to have split form\n");
	printf("/*$O-option- */         To change a GC option for the current file\n");
	printf("/**/                    Before a function definition, GC will not add a comment separator\n");
	printf("/*<word>:$*/            Before a function definition, insert all parameters in the comment\n");
	printf("                        <word> must have been defined with -cmt_fct_categ_in- option\n");
	printf("/*... <word>: ... */    Special indentation if <word> has been defined with -cmt_fct_categ- option\n");
	printf("/*... <char> ... */     Force an EOL just after <char> if <char> has been defined with the -cmt_sep_char_split option\n");

	printf("\n");
}

/*
 =======================================================================================================================
    Not used ? void ForceOption(char *opt) { Options(1, &opt);
    } ;
    Default : set values to their defaults
 =======================================================================================================================
 */
void Default(void)
{
	/*~~*/
	int i;
	/*~~*/

	for(i = 0; i < GAST_ENTRIES; i++)
	{
		if(gast_Options[i].add != NULL)
		{
			*(gast_Options[i].add) = gast_Options[i].valdefault;
		}
	}

	gi_NumCmtCateg = 0;
	gi_NumReplace = 0;
	memset(ReplaceFirst, 0, sizeof(ReplaceFirst));
}

/*
 =======================================================================================================================
    CheckConfig : check that configured options are allowed, force some configuration is certain flags given.
 =======================================================================================================================
 */
void CheckConfig(void)
{
	if(Config.DependenciesAll) Config.Dependencies = 1;

	if(Config.CanBak && Config.OutTest)
	{
		Fatal("-bak- and -output_test- can't both be TRUE", 0);
	}

	if(Config.ReturnAddParen && Config.ReturnDelParen)
	{
		Fatal("-code_force_return_paren- and -code_remove_return_paren- can't both be TRUE", 0);
	}

	if(Config.CToCpp) Config.CmtKeepCpp = 1;
	if(Config.TrailingCmtStyle)
	{
		Config.ForceFctDefDeclSplit = 1;
		Config.SplitFctDef = 3;
	}

	if(Config.NoCmtIndent) Config.CmtKeepCpp = 0;
}

/*
 =======================================================================================================================
    ConvertFile : Set options from the content of the given file
 =======================================================================================================================
 */
int ConvertFile(char *name)
{
	/*~~~~~~~~~~~~~~~*/
	FILE	*h;
	int		num;
	int		result = 0;
	/*~~~~~~~~~~~~~~~*/

	h = fopen(name, "rb");
	if(h)
	{
		ConvertFileToArgv(h, &num);
		Options(num, gargv);
		while(num) free(gargv[--num]);
		fclose(h);
		printf("Using configuration in file: %s\n", name);
		result = 1;
	}

	return result;
}

/*
 =======================================================================================================================
    ConvertFileToArgv : build an argument list from the content of the given file
 =======================================================================================================================
 */
void ConvertFileToArgv(FILE *h, int *argc)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	char	*p, *ptmp, *pmem, need_end;
	long	len;
	char	*pparam, *psc, *ptmp1;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/* Get length of file */
	*argc = 0;
	fseek(h, 0, SEEK_END);
	len = ftell(h);
	rewind(h);
	if(!len) return;

	/* Alloc buffer and read file */
	p = (char *) __malloc__(len + 1);
	if(!p) Fatal("Not enough memory", NULL);
	fread(p, 1, len, h);
	p[len] = 0;

	/* Parse file */
	ptmp = p;
	ptmp1 = NULL;
	*argc = 0;
	need_end = 0;
	while(!need_end)
	{
		while(isspace(*ptmp)) ptmp++;

		/* Comments */
		if(*ptmp == '#')
		{
			while(*ptmp && *ptmp != '\n') ptmp++;
			continue;
		}

		pmem = ptmp;
		pparam = NULL;
		while(*ptmp && *ptmp != ' ' && *ptmp != '\n' && *ptmp != '\r')
		{
			if(*ptmp == '\"')
			{
				pparam = psc = (char *) __malloc__(2048);
				ptmp1 = ptmp + 1;
				while(*ptmp1 && *ptmp1 != '\"') *psc++ = *ptmp1++;
				*psc = 0;
				break;
			}

			ptmp++;
		}

		if(!*ptmp) need_end = 1;
		if(ptmp - pmem == 0) break;
		if(*argc == MAX_DEF_PARAMS) Fatal("Too many defined params", NULL);
		*ptmp = 0;

		if(pparam)
		{
			gargv[*argc] = (char *) __malloc__(strlen(pmem) + strlen(pparam) + 10);
			strcpy(gargv[*argc], pmem);
			strcat(gargv[*argc], pparam);
			free(pparam);
			ptmp = ptmp1 + 1;
		}
		else
		{
			gargv[*argc] = strdup(pmem);
			ptmp++;
		}

		printf("arg[%d] = %s\n", *argc, gargv[*argc]);
		(*argc)++;
	}

	free(p);
}

/*
 =======================================================================================================================
    Options : set configuration from the given option array
 =======================================================================================================================
 */
void Options(int argc, char *argv[])
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	int				i, j, num;
	unsigned char	res;
	char			*ptest, *p;
	char			asz_Temp[_MAX_PATH];
	char			asz_Path[_MAX_PATH];
	FILE			*h;
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	for(i = 0; i < argc; i++)
	{
		ptest = argv[i];
		if(ptest[0] == '/' && ptest[1] == '/') continue;
		if(*ptest != '-') Fatal("Unknown option", ptest);
		ptest++;
		res = 255;
		if((*ptest == 'n') && (ptest[1] == 'o') && (ptest[2] == '-'))
		{
			ptest += 3;
			res = 0;
		}

		for(j = 0; j < GAST_ENTRIES; j++)
		{
			if((strstr(ptest, gast_Options[j].name)) && (ptest[strlen(gast_Options[j].name) - 1] == '-'))
			{
				/* Some basic options */
				switch(gast_Options[j].type)
				{

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_INSERT:
					if(gi_NumFileInsert == MAX_DEF_FILEINSERT) Fatal("Too many file insertion", NULL);
					ptest += strlen(gast_Options[j].name);
					gapsz_FileInsert[gi_NumFileInsert++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_EXTTOKEN:
					if(gi_NumExtTokens == LEXI_C_MaxExtTokens) Fatal("Too many extended tokens", NULL);
					ptest += strlen(gast_Options[j].name);
					gast_ExtList[gi_NumExtTokens].name = strdup(ptest);
					p = gast_ExtList[gi_NumExtTokens].name;
					while(*p && *p != '$') p++;
					if(!*p) Fatal("Invalid extended token definition", NULL);
					*p = 0;
					p++;
					gast_ExtList[gi_NumExtTokens].as = strdup(p);
					p = gast_ExtList[gi_NumExtTokens++].as;
					while(*p && !isspace(*p)) p++;
					*p = 0;
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_CHAR:
					ptest += strlen(gast_Options[j].name);
					*(gast_Options[j].add) = *ptest;
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_BOOL:
					*(gast_Options[j].add) = res;
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_INT:
					ptest += strlen(gast_Options[j].name);
					if(res == 0) Fatal("Bad option", argv[i]);
					if(!*ptest) Fatal("Bad option", argv[i]);
					*(gast_Options[j].add) = atol(ptest);
					if(*(gast_Options[j].add) < gast_Options[j].valmin) Fatal("Parameter is incorrect", argv[i]);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_DIR:
					if(gi_NumScanFiles == MAX_DEF_FILESDIRS) Fatal("Too many directories", NULL);
					ptest += strlen(gast_Options[j].name);
					gpsz_RecurseDirs[gi_NumRecurseDirs++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_DIREX:
					if(gi_NumExcludeFiles == MAX_DEF_FILESDIRS) Fatal("Too many exclude directories", NULL);
					ptest += strlen(gast_Options[j].name);
					gpsz_ExcludeDirs[gi_NumExcludeDirs++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_DEPEND:
					ptest += strlen(gast_Options[j].name);
					gpsz_IncludeDirs[gi_NumIncludesDirs++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_FILE:
					if(gi_NumScanFiles == MAX_DEF_FILESDIRS) Fatal("Too many files", NULL);
					ptest += strlen(gast_Options[j].name);
					gpsz_ScanFiles[gi_NumScanFiles++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_CFGFILE:
					ptest += strlen(gast_Options[j].name);
					strcpy(gz_CfgFileName, ptest);
					if(strchr(gz_CfgFileName, DIR_SEPARATOR) == NULL)
					{
						strcpy(asz_Path, asz_Temp);
						strcat(asz_Path, "/");	/* really should be "DIR_SEPARATOR" .. but */
						strcat(asz_Path, gz_CfgFileName);
					}
					else
					{
						strcpy(asz_Path, gz_CfgFileName);
					}

					h = fopen(asz_Path, "rb");
					if(!h)
					{
						h = fopen(gz_CfgFileName, "rb");
						if(h) printf("Using configuration in file: %s\n", gz_CfgFileName);
					}
					else
						printf("Using configuration in file: %s\n", gz_CfgFileName);

					if(h)
					{
						ConvertFileToArgv(h, &num);
						Options(num, gargv);
						while(num) free(gargv[--num]);
						fclose(h);
					}
					else
						Warning("Can't find cfg file", asz_Path);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_FILEEX:
					if(gi_NumExcludeFiles == MAX_DEF_FILESDIRS) Fatal("Too many exclude files", NULL);
					ptest += strlen(gast_Options[j].name);
					gpsz_ExcludeFiles[gi_NumExcludeFiles++] = strdup(ptest);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_CTGFXCMT:
					if(gi_NumCmtCateg == MAX_DEF_CATEG) Fatal("Too many categories for function comment", NULL);
					ptest += strlen(gast_Options[j].name);
					gast_CmtCateg[gi_NumCmtCateg].mpsz_Name = strdup(ptest);
					gast_CmtCateg[gi_NumCmtCateg++].mi_Type = 1;
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_CTGFXCMT1:
					if(gi_NumCmtCateg == MAX_DEF_CATEG) Fatal("Too many categories for function comment", NULL);
					ptest += strlen(gast_Options[j].name);
					gast_CmtCateg[gi_NumCmtCateg].mpsz_Name = strdup(ptest);
					gast_CmtCateg[gi_NumCmtCateg++].mi_Type = 2;
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_REPLACE:
					if(gi_NumReplace == MAX_DEF_REPLACE) Fatal("Too many words for replacement", NULL);
					ptest += strlen(gast_Options[j].name);
					gast_Replace[gi_NumReplace].mpsz_Name1 = strdup(ptest);
					ReplaceFirst[*ptest] = 1;
					p = gast_Replace[gi_NumReplace].mpsz_Name1;
					while(*p && *p != '$') p++;
					if(!*p) Fatal("Invalid replace words definition", NULL);
					*p = 0;
					p++;
					gast_Replace[gi_NumReplace++].mpsz_Name2 = strdup(p);
					break;

				/*$2--------------------------------------------------------------------------------------------------*/

				case TYPE_OPT_FIXME_CMT:
					ptest += strlen(gast_Options[j].name);
					gz_FixmeComment = strdup(ptest);
					break;
				}
			}
		}

		if(i == GAST_ENTRIES) Fatal("Unknown option", ptest);

		ptest = argv[i];
	}
}
