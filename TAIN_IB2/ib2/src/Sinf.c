/*
 * PROGRAM ID   : Sinf.c
 * PROGRAM NAME : ���������� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ���������� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : 
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
		
		/* ó���� ������ �����Ѵ�. */
		FILE* fp;
		fp = fopen(CfgGetMaster("ib.path.file.system.inf"), "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, CfgGetMaster("ib.path.file.system.inf"));
			return NULL;
		}
		
		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;
	
			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;
			
			if (flag)
			{
				/* �Ѷ����� �����ڷ� �ɰ���. */
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
		
		/* ó���� ������ �ݴ´�. */
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
