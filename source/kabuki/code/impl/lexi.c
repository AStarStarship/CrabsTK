/*$T \Sources/lexi.c GC 1.150 2011-09-22 20:52:18 */


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

#include "error.h"
#include "config.h"
#include "lexi.h"
#include "tools.h"
#include "config.h"
#include "in.h"
#include "debug.h"
#include "os.h"

/*$4
 ***********************************************************************************************************************
 ***********************************************************************************************************************
 */

/* Current global pointer to parse a file */
char				*g;

/* To remember parsing line number */
int					gi_NumLine;

/* List of all tokens */
char				*name_of_token[] =
{
	"TOKEN_NOT_USED",
	"TOKEN_SPACE",
	"TOKEN_EOL",
	"TOKEN_CCMT",
	"TOKEN_CPPCMT",
	"TOKEN_PP",
	"TOKEN_WORD",
	"TOKEN_CST",
	"TOKEN_SPECWORD",
	"TOKEN_NOT_USED",
	"TOKEN_AUTOOP",
	"TOKEN_DOUBLECOLON",
	"TOKEN_POINTER",
	"TOKEN_AUTOSIGN",
	"TOKEN_COMPARE",
	"TOKEN_OPER",
	"TOKEN_LINK",
	"TOKEN_STRINGCST",
	"TOKEN_CHARCST",
	"TOKEN_NUMBER",
	"TOKEN_ELLIPSIS",
	"TOKEN_ADDRESS",
	"TOKEN_LESS",
	"TOKEN_GREAT",
	"TOKEN_NOT_USED",
	"TOKEN_NOT_USED",
	"TOKEN_NOT_USED",
	"TOKEN_NOT_USED",
	"TOKEN_NOT_USED",
	"TOKEN_BLANK_LINE",
	"TOKEN_STAR",
	"TOKEN_SEMICOL",
	"TOKEN_LPAREN",
	"TOKEN_RPAREN",
	"TOKEN_LARRAY",
	"TOKEN_RARRAY",
	"TOKEN_LBRACE",
	"TOKEN_RBRACE",
	"TOKEN_QUESTION",
	"TOKEN_COLON",
	"TOKEN_COMMA",
	"TOKEN_POINT",
	"TOKEN_SIGN",
	"TOKEN_AFFECT",
	"TOKEN_UNCHANGE",
	"TOKEN_BREAKLINE",
	"TOKEN_OR"
};

/* List of all tokens */
Lexi_tdst_Token		gast_List[] =
{
	{ "if", TOKEN_W_IF, 0 },
	{ "else", TOKEN_W_ELSE, 0 },
	{ "while", TOKEN_W_WHILE, 0 },
	{ "do", TOKEN_W_DO, 0 },
	{ "try", TOKEN_W_TRY, 0 },
	{ "catch", TOKEN_W_CATCH, 0 },
	{ "for", TOKEN_W_FOR, 0 },
	{ "return", TOKEN_W_RETURN, 0 },
	{ "break", TOKEN_W_BREAK, 0 },
	{ "continue", TOKEN_W_CONTINUE, 0 },
	{ "goto", TOKEN_W_GOTO, 0 },
	{ "sizeof", TOKEN_W_SIZEOF, 0 },
	{ "default", TOKEN_W_DEFAULT, 0 },
	{ "case", TOKEN_W_CASE, 0 },
	{ "switch", TOKEN_W_SWITCH, 0 },
	{ "_asm", TOKEN_W_ASM, 0 },
	{ "__asm", TOKEN_W_ASM, 0 },
	{ "new", TOKEN_W_NEW, 0 },
	{ "delete", TOKEN_W_DELETE, 0 },
	{ "struct", TOKEN_W_DECL, TOKEN_WW_STRUCT },
	{ "enum", TOKEN_W_DECL, TOKEN_WW_ENUM },
	{ "union", TOKEN_W_DECL, TOKEN_WW_UNION },
	{ "class", TOKEN_W_DECL, TOKEN_WW_CLASS },
	{ "template", TOKEN_W_DECL, TOKEN_WW_TEMPLATE },
	{ "typedef", TOKEN_W_DECL, TOKEN_WW_TYPEDEF },
	{ "double", TOKEN_W_DECL, 0 },
	{ "void", TOKEN_W_DECL, 0 },
	{ "short", TOKEN_W_DECL, 0 },
	{ "char", TOKEN_W_DECL, 0 },
	{ "volatile", TOKEN_W_DECL, 0 },
	{ "virtual", TOKEN_W_DECL, 0 },
	{ "static", TOKEN_W_DECL, TOKEN_WW_STATIC },
	{ "unsigned", TOKEN_W_DECL, 0 },
	{ "signed", TOKEN_W_DECL, 0 },
	{ "const_cast", TOKEN_W_DECL, TOKEN_WW_SCAST },
	{ "static_cast", TOKEN_W_DECL, TOKEN_WW_SCAST },
	{ "dynamic_cast", TOKEN_W_DECL, TOKEN_WW_SCAST },
	{ "operator", TOKEN_W_DECL, TOKEN_WW_OPERATOR },
	{ "const", TOKEN_W_DECL, TOKEN_WW_CONST },
	{ "register", TOKEN_W_DECL, 0 },
	{ "int", TOKEN_W_DECL, 0 },
	{ "inline", TOKEN_W_DECL, 0 },
	{ "float", TOKEN_W_DECL, 0 },
	{ "friend", TOKEN_W_DECL, 0 },
	{ "long", TOKEN_W_DECL, 0 },
	{ "extern", TOKEN_W_DECL, TOKEN_WW_EXTERN },
	{ "namespace", TOKEN_W_DECL, TOKEN_WW_NAMESPACE },
	{ "private", TOKEN_W_DECL, TOKEN_WW_CLASSACCESS },
	{ "protected", TOKEN_W_DECL, TOKEN_WW_CLASSACCESS },
	{ "public", TOKEN_W_DECL, TOKEN_WW_CLASSACCESS },
	{ "throws", TOKEN_W_DECL, TOKEN_WW_THROWS },
	{ "throw", TOKEN_W_DECL, TOKEN_WW_THROW },
	{ "__inline", TOKEN_W_DECL, 0 },
	{ "__fastcall", TOKEN_W_DECL, 0 },
	{ "__declspec", TOKEN_W_DECL, 0 },
	{ "#if", TOKEN_PP_IF, 0 },
	{ "#endif", TOKEN_PP_ENDIF, 0 },
	{ "#else", TOKEN_PP_ELSE, 0 },
	{ "#elif", TOKEN_PP_ELIF, 0 },
	{ "#undef", TOKEN_PP_UNDEF, 0 },
	{ "#define", TOKEN_PP_DEFINE, 0 },
	{ "#pragma", TOKEN_PP_PRAGMA, 0 },
	{ "#error", TOKEN_PP_ERROR, 0 },
	{ "#ifdef", TOKEN_PP_IFDEF, 0 },
	{ "#ifndef", TOKEN_PP_IFNDEF, 0 },
	{ "#include", TOKEN_PP_INCLUDE, 0 },
	{ "#import", TOKEN_PP_INCLUDE, 0 },
	{ "#line", TOKEN_PP_LINE, 0 },
	{ "single_word", TOKEN_W_SINGLEWORD, 0 },
	{ "single_word_0", TOKEN_W_SINGLEWORD0, 0 },
	{ "BEGIN_MESSAGE_MAP", TOKEN_W_MFC_BMESSAGEMAP, 0 },
	{ "END_MESSAGE_MAP", TOKEN_W_MFC_EMESSAGEMAP, 0 },
};

/* List of extended tokens */
Lexi_tdst_ExtToken	gast_ExtList[LEXI_C_MaxExtTokens];
int					gi_NumExtTokens = 0;	/* Number of extended tokens */

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int Token(void)
{
	/*~~~~~~~~~~~~~~~~~~~~~*/
	char	*p;
	char	next;
	int		seendot, seenexp;
	/*~~~~~~~~~~~~~~~~~~~~~*/

	/* Spaces (without \n') */
	if(Lisspace(*g))
	{
		while(Lisspace(*g)) g++;
		return TOKEN_SPACE;
	}

	/* C comment */
	if((*g == '/') && (g[1] == '*'))
	{
		g++;
l_ccmt:
		g++;
		while(*g && (*g != '*'))
		{
			if(*g == '\n') gi_NumLine++;
			g++;
		}

		if(!*g) return TOKEN_CCMT;
		if(!g[1]) return TOKEN_CCMT;
		if(g[1] != '/') goto l_ccmt;
		g += 2;
		return TOKEN_CCMT;
	}

	/* C++ comment */
	if((*g == '/') && (g[1] == '/'))
	{
		g += 2;
l_cppcmt:
		while(*g && *g != '\n') g++;

		p = g;
		while(_isspace(*p)) p++;
		if(p[0] == '/' && p[1] == '/' && p[2] == '/')
		{
			g = p;
			g += 3;
			goto l_cppcmt;
		}

		return TOKEN_CPPCMT;
	}

	/* PP */
	if(*g == '#')
	{
		g++;
		while(Lisspace(*g)) g++;
		while(Lisword(*g)) g++;
		return TOKEN_PP;
	}

	/* Word */
	if(isalpha(*g) || (*g == '_'))
	{
		if(*g != 'L' || (g[1] != '"') && (g[1] != '\''))
		{
			while(Lisword(*g)) g++;
			return TOKEN_WORD;
		}
	}

	/* Number */
	if(isdigit(*g) || ((*g == '.') && isdigit(g[1])))
	{
		seendot = 0;
		seenexp = 0;
		if((*g == '0') && ((g[1] == 'x') || (g[1] == 'X')))
		{
			g += 2;
			while(isxdigit(*g)) g++;
			if((*g == 'U') || (*g == 'u')) g++;
			if(*g == 'L' || *g == 'l') g++;
		}
		else
		{
			for(;;)
			{
				if(*g == '.')
				{
					if(seendot)
						break;
					else
						seendot++;
				}

				g++;
				if(!isdigit(*g) && (*g != '.'))
				{
					while(Lisspace(*g)) g++;
					if(((*g != 'E') && (*g != 'e')) || seenexp)
						break;
					else
					{
						seenexp++;
						seendot++;
						g++;
						while(Lisspace(*g)) g++;
						if((*g == '+') || (*g == '-')) g++;
					}
				}
			}

			while(Lisspace(*g)) g++;
			if((*g == 'F') || (*g == 'f') || (*g == 'i') || (*g == 'j'))
				g++;
			else
			{
				if((*g == 'U') || (*g == 'u')) g++;
				if(*g == 'L' || *g == 'l') g++;
				if(*g == 'L' || *g == 'l') g++;
			}
		}

		return TOKEN_NUMBER;
	}

	/* Others */
	next = g[1];
	switch(*g)
	{

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case 0:
		return 0;

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '\n':
		gi_NumLine++;
		RETURN1(TOKEN_EOL);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '*':
		if(next == '=') RETURN2(TOKEN_AUTOOP);
		RETURN1(TOKEN_STAR);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case ';':
		RETURN1(TOKEN_SEMICOL);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '(':
		RETURN1(TOKEN_LPAREN);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case ')':
		RETURN1(TOKEN_RPAREN);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '[':
		RETURN1(TOKEN_LARRAY);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case ']':
		RETURN1(TOKEN_RARRAY);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '{':
		RETURN1(TOKEN_LBRACE);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '}':
		RETURN1(TOKEN_RBRACE);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '?':
		RETURN1(TOKEN_QUESTION);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '\\':
		RETURN1(TOKEN_BREAKLINE);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case ':':
		if(next == ':') RETURN2(TOKEN_DOUBLECOLON);
		RETURN1(TOKEN_COLON);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case ',':
		RETURN1(TOKEN_COMMA);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '.':
		if((next == '.') && (g[2] == '.')) RETURN3(TOKEN_ELLIPSIS);
		RETURN1(TOKEN_POINT);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '-':
		if(next == '>') RETURN2(TOKEN_POINTER);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '+':
		if(next == *g) RETURN2(TOKEN_AUTOSIGN);
		if(next == '=') RETURN2(TOKEN_AUTOOP);
		RETURN1(TOKEN_SIGN);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '=':
		if(next == '=') RETURN2(TOKEN_COMPARE);
		RETURN1(TOKEN_AFFECT);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '>':
	case '<':
		if(next == *g)
		{
			if(g[2] == '=') RETURN3(TOKEN_AUTOOP);
			RETURN2(TOKEN_OPER);
		}

		if(next == '=') RETURN2(TOKEN_COMPARE);
		if(*g == '<') RETURN1(TOKEN_LESS);
		RETURN1(TOKEN_GREAT);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '&':
	case '|':
		if(next != *g && next != '=')
		{
			if(*g == '&')
			{
				RETURN1(TOKEN_ADDRESS);
			}
			else
			{
				RETURN1(TOKEN_OR);
			}
		}

		if(next == *g) RETURN2(TOKEN_LINK);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '%':
	case '^':
	case '/':
		if(next == '=') RETURN2(TOKEN_AUTOOP);
		RETURN1(TOKEN_OPER);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '!':
		if(next == '=') RETURN2(TOKEN_COMPARE);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '~':
		RETURN1(TOKEN_UNCHANGE);

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case 'L':
		g++;

	/*$2--------------------------------------------------------------------------------------------------------------*/

	case '\'':
	case '"':
		next = *g;
		g++;
		while(*g && (*g != next) && (*g != '\n'))
		{
			if(*g == '\\')
			{
				g++;
				if(*g == '\n') gi_NumLine++;
				if(*g == 0) break;
			}

			g++;
		}

		if(next == '"') RETURN1(TOKEN_STRINGCST);
		RETURN1(TOKEN_CHARCST);
	}

	/* Unknown token : Fatal error */
	RETURN1(TOKEN_UNKNOWCHAR);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
int IsReservedWord(FileDes *pfile, token *ptoken)
{
	/*~~~~~*/
	int i, j;
	/*~~~~~*/

	pfile = pfile;

	/* Normal token */
	for(i = 0; i < sizeof(gast_List) / sizeof(gast_List[0]); i++)
	{
		if(!strcmp(gast_List[i].name, ptoken->pc_Value)) return i;
	}

	/* Extended token */
	for(i = 0; i < gi_NumExtTokens; i++)
	{
		if(!strcmp(gast_ExtList[i].as, ptoken->pc_Value))
		{
			for(j = 0; j < sizeof(gast_List) / sizeof(gast_List[0]); j++)
			{
				if(!strcmp(gast_List[j].name, gast_ExtList[i].name)) return j;
			}

			return -1;
		}
	}

	return -1;
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void Lexi(FileDes *pfile)
{
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/
	token	*pst_Temp;
	char	mem, *pmem, *pz;
	int		i;
	int		ii, indice;
	int		col;
	char	*pz_Temp, *pz_Temp1;
	char	az_Tmp[2048];
	/*~~~~~~~~~~~~~~~~~~~~~~~~*/

	if(pfile->pc_Buffer == NULL) return;

	/* Global init for the file */
	g = pfile->pc_Buffer;
	gi_NumLine = 1;
	col = 0;

	/* Parse all the file */
	do
	{
		/* Allocate a new node */
		pst_Temp = pfile->pst_LastToken;
		pfile->pst_LastToken = (token *) __malloc__(sizeof(token));
		if(pfile->pst_LastToken == NULL)
		{
			Fatal("Not enough memory to process source file", pfile->psz_FileName);
		}

		memset(pfile->pst_LastToken, 0, sizeof(token));

		pfile->pst_LastToken->line = gi_NumLine;
		pfile->pst_LastToken->column = col;

		/* Link new token to list */
		pfile->pst_LastToken->pst_Prev = pst_Temp;
		pfile->pst_LastToken->pst_Next = NULL;
		if(pst_Temp)
		{
			pst_Temp->pst_Next = pfile->pst_LastToken;
		}

		/* If it's the first token, init the root node */
		if(pfile->pst_RootToken == NULL)
		{
			pfile->pst_RootToken = pfile->pst_LastToken;
		}

		/* Init token */
		pst_Temp = pfile->pst_LastToken;
Zap:
		pmem = g;
		pst_Temp->i_ID = Token();

		/* Initialize sub and subsub id's for comments */
		if((pst_Temp->i_ID == TOKEN_CCMT) || (pst_Temp->i_ID == TOKEN_CPPCMT))
		{
			pst_Temp->i_SubID = 0;
			pst_Temp->i_SubSubID = 0;
		}

		/* Special case for spaces and eol */
		if(pst_Temp->i_ID == TOKEN_SPACE)
		{
			if(pst_Temp->pst_Prev)
			{
				i = 0;
				while(isspace(pmem[i]))
				{
					if(*pmem == '\t')
					{
						pst_Temp->pst_Prev->WasSpaceAfter += (Config.TabSize - (col % Config.TabSize));
						col += (Config.TabSize - (col % Config.TabSize));
					}
					else
					{
						pst_Temp->pst_Prev->WasSpaceAfter++;
						col++;
					}

					i++;
				}
			}

			goto Zap;
		}

		if(pst_Temp->i_ID == TOKEN_EOL)
		{
			col = 0;

			if(pst_Temp->pst_Prev)
			{
				pst_Temp->pst_Prev->WasEOLAfter++;
				pst_Temp->pst_Prev->WasSpaceAfter = 0;
				goto Zap;
			}
		}

		/* Set string */
		mem = *g;
		*g = 0;
		pst_Temp->pc_Value = strdup(pmem);
		*g = mem;

		if(pst_Temp->i_ID == TOKEN_NUMBER)
		{
			pz = pst_Temp->pc_Value;
			while(*pz)
			{
				if(Lisspace(*pz))
					memmove(pz, pz + 1, strlen(pst_Temp->pc_Value) - (pz - pst_Temp->pc_Value));
				else
					pz++;
			}
		}

		/* Delete comment ? */
		if((pst_Temp->i_ID == TOKEN_CCMT) && DeleteComment(pst_Temp))
		{
			pmem[0] = ' ';
			pmem[1] = ' ';
			pmem[2] = ' ';
			pmem[3] = ' ';
			ii = 4;
			if(DeleteCommentEx(pst_Temp, 1))
			{
				pmem[4] = ' ';
				ii = 5;
			}

			while(pmem + ii < g - 2)
			{
				if(pmem[ii] == '\n')
				{
					col = 0;
					ii++;
					while(isspace(pmem[ii])) ii++;
					if(pmem[ii] == '*' && pmem + ii != g - 2) pmem[ii] = ' ';
				}

				ii++;
				col++;
			}

			g[-1] = ' ';
			g[-2] = ' ';
			g = pmem;
			free(pst_Temp->pc_Value);
			goto Zap;
		}

		col += strlen(pst_Temp->pc_Value);

		/* For a word, test if it's a reserved one */
		if((pst_Temp->i_ID == TOKEN_WORD) || (pst_Temp->i_ID == TOKEN_PP))
		{
			/* A # with a space after */
			if(pst_Temp->i_ID == TOKEN_PP && Lisspace(pst_Temp->pc_Value[1]))
			{
				pz_Temp1 = pst_Temp->pc_Value + 1;
				pz_Temp = az_Tmp + 1;
				az_Tmp[0] = '#';
				while(Lisspace(*pz_Temp1)) pz_Temp1++;
				while(*pz_Temp1) *(pz_Temp++) = *(pz_Temp1++);
				*pz_Temp = 0;
				free(pst_Temp->pc_Value);
				pst_Temp->pc_Value = strdup(az_Tmp);
			}

			indice = IsReservedWord(pfile, pst_Temp);
			if(indice == -1)
			{
			}
			else
			{
				pst_Temp->i_SubID = gast_List[indice].id;
				pst_Temp->i_SubSubID = gast_List[indice].subsubid;
			}

			if((pst_Temp->i_SubID) && (pst_Temp->i_SubID != TOKEN_W_DECL) && (pst_Temp->i_ID == TOKEN_WORD))
			{
				pst_Temp->i_ID = TOKEN_SPECWORD;
			}

			/* A wrong PP directive */
			if((pst_Temp->i_SubID == 0) && (pst_Temp->i_ID == TOKEN_PP)) pst_Temp->i_ID = TOKEN_WORD;

			/* #error */
			if((pst_Temp->i_SubID == TOKEN_PP_ERROR) && (pst_Temp->i_ID == TOKEN_PP))
			{
				pst_Temp = pfile->pst_LastToken;
				pfile->pst_LastToken = (token *) __malloc__(sizeof(token));
				memset(pfile->pst_LastToken, 0, sizeof(token));
				pfile->pst_LastToken->pst_Prev = pst_Temp;
				pfile->pst_LastToken->pst_Next = NULL;
				if(pst_Temp) pst_Temp->pst_Next = pfile->pst_LastToken;
				pst_Temp = pfile->pst_LastToken;

				pst_Temp->i_ID = TOKEN_STRINGCST;
				while(Lisspace(*g)) g++;
				pmem = g;
				while(*g && *g != '\n') g++;
				mem = *g;
				*g = 0;
				pst_Temp->pc_Value = strdup(pmem);
				*g = mem;
			}
		}
	} while(pfile->pst_LastToken->i_ID);

	/* Force the last dummy token to be a end of line */
	if(pfile->pst_LastToken->pst_Prev) pfile->pst_LastToken->pst_Prev->WasEOLAfter = 1;

	/* Free buffer of file */
	if(pfile->pc_Buffer) free(pfile->pc_Buffer);
}
