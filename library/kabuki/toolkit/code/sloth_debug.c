/*$T \Sources/debug.c GC 1.150 2011-09-22 20:52:17 */


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
#include <ctype.h>

#include "config.h"
#include "lexi.h"
#include "error.h"
#include "tools.h"
#include "grammar.h"

/*
 =======================================================================================================================
 =======================================================================================================================
 */

void print(char *msg)
{
	printf("%s\n", msg);
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void printTokens(FileDes *pFile)
{
	/*~~~~~~~~~~~~~~~*/
	token	*pCurToken;
	/*~~~~~~~~~~~~~~~*/

	printf("\n");

	for(pCurToken = pFile->pst_RootToken; pCurToken != NULL; pCurToken = NextToken(pCurToken))
	{
		printf("%s\n", pCurToken->pc_Value);
	}
}

/*
 =======================================================================================================================
 =======================================================================================================================
 */
void printTokensWithTypes(FileDes *pFile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	printf("\n");
	printf("T  D  N  E  vvvvvvvvvvvvvvv  vvvvvvvvvvvvvvvvvvv\n");

	for(pcur = pFile->pst_RootToken; pcur != NULL; pcur = NextToken(pcur))
	{
		printf
		(
			"%1d  %1d  %1d  %1d  %-15s  %s\n",
			pcur->InTemplate,
			pcur->DeclName,
			pcur->WasEOLAfter,
			pcur->ForceEOLAfter,
			name_of_token[pcur->i_ID],
			pcur->pc_Value
		);
	}
}
