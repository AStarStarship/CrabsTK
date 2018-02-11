/*$T \Sources/indent_special.c GC 1.150 2011-09-22 20:52:18 */


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
void Indent_Special(FileDes *pfile)
{
	/*~~~~~~~~~~*/
	token	*pcur;
	/*~~~~~~~~~~*/

	for(pcur = pfile->pst_RootToken; pcur; pcur = NextToken(pcur))
	{
		if(pcur->i_ID == TOKEN_SPECWORD)
		{
			switch(pcur->i_SubID)
			{
			case TOKEN_W_SINGLEWORD:
			case TOKEN_W_SINGLEWORD0:
				if(!pcur->MacroName && !pcur->DefineName)
				{
					Tool_ForceEOLBefore(pcur, 1);
					Tool_ForceEOLAfterComments(pcur, 1);
					if(pcur->i_SubID == TOKEN_W_SINGLEWORD0) pcur->StmtLevel = pcur->IndentLevel = 0;
				}
				break;

			case TOKEN_W_MFC_BMESSAGEMAP:
				pcur = Tool_NextValid(pcur);
				pcur = Tool_ToRelationNext(pcur);
				Tool_ForceEOLAfterComments(pcur, 1);

				/* Commands */
				pcur = Tool_NextValid(pcur);
				while(pcur && !SpecWord(pcur, TOKEN_W_MFC_EMESSAGEMAP))
				{
					while(pcur && pcur->i_ID != TOKEN_WORD && !SpecWord(pcur, TOKEN_W_MFC_EMESSAGEMAP))
						pcur = Tool_NextValid(pcur);
					if(SpecWord(pcur, TOKEN_W_MFC_EMESSAGEMAP)) break;
					if(!pcur) return;
					if(pcur->pst_Next && pcur->pst_Next->i_ID != TOKEN_LPAREN)
					{
						pcur = Tool_NextValid(pcur);
						continue;
					}

					pcur->IndentLevel = 1;

					/* ( */
					pcur = Tool_NextValid(pcur);
					if(pcur)
					{
						pcur = Tool_ToRelationNext(pcur);
						if(pcur)
						{
							Tool_ForceEOLAfterComments(pcur, 1);
							pcur = Tool_NextValid(pcur);
						}
					}
				}

				/* End message map */
				pcur = Tool_NextValid(pcur);
				pcur = Tool_ToRelationNext(pcur);
				Tool_ForceEOLAfterComments(pcur, 1);
				break;
			}
		}
	}
}
