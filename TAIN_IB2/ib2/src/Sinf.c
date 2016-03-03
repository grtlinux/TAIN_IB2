/*
 * PROGRAM ID   : Sinf.c
 * PROGRAM NAME : 서버정보에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 서버정보에 사용하는 함수들을 정의한다.
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   MAX_LINE_SIZE      1024
#define   MAX_ITEM           20

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
char* SinfGetHostNo( char* strHostName )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	static char strHostNo[10+1];
	strHostNo[0] = 0x00;
	
	if (flag)
	{
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		
		/* 처리할 파일을 오픈한다. */
		FILE* fp;
		fp = fopen(CfgGetMaster("ib.path.file.system.inf"), "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, CfgGetMaster("ib.path.file.system.inf"));
			return NULL;
		}
		
		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;
	
			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;
			
			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arrItem);
				
				if (strcmp(strHostName, UtilTrim(arrItem[1])) == 0)
				{
					sprintf(strHostNo, "%.10s", UtilTrim(arrItem[2]));
					
					for (i=0; i < MAX_ITEM && arrItem[i]; i++)
					{
						if (!flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
					}
				}
			}
		}
		
		/* 처리된 파일을 닫는다. */
		fclose(fp);
	}
	
	return strHostNo;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

#ifdef _EXE

/*============================================================================*/
/*
 * 위에서 정의한 함수를 테스트 한다.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	sProgName = argv[0];
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		char strHostName[100];
		
		int ret = gethostname(strHostName, sizeof(strHostName));

		LOG(_FL_, 0, 0, "> %-15s = [%s]", strHostName , SinfGetHostNo(strHostName));

		LOG(_FL_, 0, 0, "> %-15s = [%s]", "local"   , SinfGetHostNo("local"   ));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "dbizap01", SinfGetHostNo("dbizap01"));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "dfep03"  , SinfGetHostNo("dfep03"  ));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "pbizap01", SinfGetHostNo("pbizap01"));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "pbizap02", SinfGetHostNo("pbizap02"));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "pfep03"  , SinfGetHostNo("pfep03"  ));
		LOG(_FL_, 0, 0, "> %-15s = [%s]", "pfep04"  , SinfGetHostNo("pfep04"  ));
	}
}

#endif
