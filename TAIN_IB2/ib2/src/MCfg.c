/*
 * PROGRAM ID   : Cfg.c
 * PROGRAM NAME : ȯ������ ó�� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : ȯ������ ó���� �ٷ�� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 * TODO DATE.20130726 : FEPID�� �ش��ϴ� ��� �׸��� ����ϵ��� �ϴ� �Լ��� �����.
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   USER_NAME          "ibridge"

#define   MAX_LINE_SIZE      8192
#define   MAX_ITEM            500
#define   FEPID_SIZE            5
#define   SERVICE_SIZE         10

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

MCFG_FLD      stMCfgFld          [MAX_MCFG_FLD];   /* field.cfg ���� ����     */
int           iMaxMCfgFld                      ;

MCFG_FLDCD    stMCfgFldCd      [MAX_MCFG_FLDCD];   /* fieldcode.cfg ���� ���� */
int           iMaxMCfgFldCd                    ;

MCFG_CODE     stMCfgCode        [MAX_MCFG_CODE];   /* code.cfg ���� ����      */
int           iMaxMCfgCode                     ;

MCFG_COMP     stMCfgComp        [MAX_MCFG_COMP];   /* comp.cfg ���� ����      */
int           iMaxMCfgComp                     ;

MCFG_SYS      stMCfgSys          [MAX_MCFG_SYS];   /* system.cfg ���� ����    */
int           iMaxMCfgSys                      ;

int           iRecLen                          ;

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * ���� : field.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgFld()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.field") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxMCfgFld = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

			if (flag)
			{
				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				strcpy(stMCfgFld[iMaxMCfgFld].sFldNo  , UtilTrim(arrItem[0]));
				strcpy(stMCfgFld[iMaxMCfgFld].sFldName, UtilTrim(arrItem[1]));
				strcpy(stMCfgFld[iMaxMCfgFld].sFldType, UtilTrim(arrItem[2]));
				strcpy(stMCfgFld[iMaxMCfgFld].sFldLen , UtilTrim(arrItem[3]));
				strcpy(stMCfgFld[iMaxMCfgFld].sScale  , UtilTrim(arrItem[4]));
				strcpy(stMCfgFld[iMaxMCfgFld].sComment, UtilTrim(arrItem[5]));

				stMCfgFld[iMaxMCfgFld].iFldLen = UtilToInt(stMCfgFld[iMaxMCfgFld].sFldLen);
				stMCfgFld[iMaxMCfgFld].iScale  = UtilToInt(stMCfgFld[iMaxMCfgFld].sScale );

				if (!flag)
				{
					printf("%06d> stMCfgFld[%d].sFldNo   = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sFldNo  );
					printf("%06d> stMCfgFld[%d].sFldName = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sFldName);
					printf("%06d> stMCfgFld[%d].sFldType = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sFldType);
					printf("%06d> stMCfgFld[%d].sFldLen  = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sFldLen );
					printf("%06d> stMCfgFld[%d].sScale   = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sScale  );
					printf("%06d> stMCfgFld[%d].iFldLen  = [%d]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].iFldLen );
					printf("%06d> stMCfgFld[%d].iScale   = [%d]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].iScale  );
					printf("%06d> stMCfgFld[%d].sComment = [%s]\n", iMaxMCfgFld, iMaxMCfgFld, stMCfgFld[iMaxMCfgFld].sComment);
				}
			}

			iMaxMCfgFld ++;
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}

	return 0;
}


/*============================================================================*/
/*
 * ���� : sorted�� Ȯ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgFldCheckSorted()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i = 1; i < iMaxMCfgFld; i++)
		{
			if (strcmp(stMCfgFld[i-1].sFldNo, stMCfgFld[i].sFldNo) >= 0)
			{
				printf("%06d) [%s] >= [%s]\n", i, stMCfgFld[i-1].sFldNo, stMCfgFld[i].sFldNo);
				if (!flag) break;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgFldSearch(char* sFieldNo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int iL, iM, iR;

		iL = 0;
		iR = iMaxMCfgFld - 1;

		int i;
		for (i=0; i < 20; i++)
		{
			iM = (iL + iR) / 2;

			if (strcmp(sFieldNo, stMCfgFld[iM].sFldNo) < 0)
			{
				/*
				 * ���� �۴�
				 */
				if (!flag) printf("STATUS : \t\t[sFieldNo:%s] < [%s]\n", sFieldNo, stMCfgFld[iM].sFldNo);
				iR = iM - 1;
			}
			else if (strcmp(stMCfgFld[iM].sFldNo, sFieldNo) < 0)
			{
				/*
				 * ���� ũ��
				 */
				if (!flag) printf("STATUS : [%s] < [sFieldNo:%s]\n", stMCfgFld[iM].sFldNo, sFieldNo);
				iL = iM + 1;
			}
			else
			{
				/*
				 * ã�Ҵ�.
				 */
				if (!flag) printf("SUCCESS : find...[%s]\n", stMCfgFld[iM].sFldNo);
				return iM;
			}

			if (iR < iL)
			{
				/*
				 * ã�� �ڷᰡ ����.
				 */
				if (!flag) printf("FAIL : not found...[iR:%d][iL:%d][%s][%s]\n", iR, iL, sFieldNo, stMCfgFld[iM].sFldNo);
				break;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * ���� : fieldcode.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgFldCd()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.fieldcode") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxMCfgFldCd = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);

			if (flag)
			{
				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				strcpy(stMCfgFldCd[iMaxMCfgFldCd].sFldNo, UtilTrim(arrItem[0]));
				strcpy(stMCfgFldCd[iMaxMCfgFldCd].sCodNo, UtilTrim(arrItem[1]));
				strcpy(stMCfgFldCd[iMaxMCfgFldCd].sGrpNm, UtilTrim(arrItem[2]));

				if (flag)
				{
					printf("%06d> stMCfgFld[%d].sFldNo = [%s]\n", iMaxMCfgFldCd, iMaxMCfgFldCd, stMCfgFldCd[iMaxMCfgFldCd].sFldNo);
					printf("%06d> stMCfgFld[%d].sCodNo = [%s]\n", iMaxMCfgFldCd, iMaxMCfgFldCd, stMCfgFldCd[iMaxMCfgFldCd].sCodNo);
					printf("%06d> stMCfgFld[%d].sGrpNm = [%s]\n", iMaxMCfgFldCd, iMaxMCfgFldCd, stMCfgFldCd[iMaxMCfgFldCd].sGrpNm);
				}
			}

			iMaxMCfgFldCd ++;
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}
}

/*============================================================================*/
/*
 * ���� : sorted�� Ȯ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgFldCdCheckSorted()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i = 1; i < iMaxMCfgFldCd; i++)
		{
			char str1[50];
			char str2[50];

			sprintf(str1, "%-6.6s.%-6.6s", stMCfgFldCd[i-1].sFldNo, stMCfgFldCd[i-1].sCodNo);
			sprintf(str2, "%-6.6s.%-6.6s", stMCfgFldCd[i  ].sFldNo, stMCfgFldCd[i  ].sCodNo);

			if (strcmp(str1, str2) >= 0)
			{
				printf("%06d) [%s] >= [%s]\n", i, str1, str2);
				if (!flag) break;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgFldCdSearch(char* sFieldNo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int iL, iM, iR;

		iL = 0;
		iR = iMaxMCfgFldCd - 1;

		int i;
		for (i=0; i < 20; i++)
		{
			iM = (iL + iR) / 2;

			if (strcmp(sFieldNo, stMCfgFldCd[iM].sFldNo) < 0)
			{
				/*
				 * ���� �۴�
				 */
				if (!flag) printf("STATUS : \t\t[sFieldNo:%s] < [%s]\n", sFieldNo, stMCfgFldCd[iM].sFldNo);
				iR = iM - 1;
			}
			else if (strcmp(stMCfgFldCd[iM].sFldNo, sFieldNo) < 0)
			{
				/*
				 * ���� ũ��
				 */
				if (!flag) printf("STATUS : [%s] < [sFieldNo:%s]\n", stMCfgFldCd[iM].sFldNo, sFieldNo);
				iL = iM + 1;
			}
			else
			{
				/*
				 * ã�Ҵ�.
				 */
				if (!flag) printf("SUCCESS : find...[%s]\n", stMCfgFldCd[iM].sFldNo);
				return iM;
			}

			if (iR < iL)
			{
				/*
				 * ã�� �ڷᰡ ����.
				 */
				if (!flag) printf("FAIL : not found...[iR:%d][iL:%d][%s][%s]\n", iR, iL, sFieldNo, stMCfgFldCd[iM].sFldNo);
				break;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * ���� : code.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgCode()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.code") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxMCfgCode = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);

			if (flag)
			{
				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				if (flag)
				{
					/*
					 * ù��° �׸��� �����. Search�� ����.
					 */
					strcpy(stMCfgCode[iMaxMCfgCode].sFldNo, UtilTrim(arrItem[0]));
					strcpy(stMCfgCode[iMaxMCfgCode].sCode , "");
					strcpy(stMCfgCode[iMaxMCfgCode].sName , "----------");

					if (!flag) {
						printf("%05d) [%s][%s]\t\t[%s]\n"
							, iMaxMCfgCode
							, stMCfgCode[iMaxMCfgCode].sFldNo
							, stMCfgCode[iMaxMCfgCode].sCode
							, stMCfgCode[iMaxMCfgCode].sName
						);
					}

					iMaxMCfgCode ++;
				}

				int idx;
				for (idx = 1; idx < MAX_ITEM; idx++)
				{
					/* ��ü�� NULL�̸� �������� */
					if (arrItem[idx] == NULL)
						break;

					/* Code�� Name�� ������ �������� */
					UtilTrim(arrItem[idx]);
					if (*arrItem[idx] == 0x00)
						break;


					if (flag)
					{
						char* item[2];
						UtilStrSplit(arrItem[idx], ':', item);

						strcpy(stMCfgCode[iMaxMCfgCode].sFldNo, UtilTrim(arrItem[0]));
						strcpy(stMCfgCode[iMaxMCfgCode].sCode , UtilTrim(item[0]));
						strcpy(stMCfgCode[iMaxMCfgCode].sName , UtilTrim(item[1]));

						if (!flag) {
							printf("%05d) [%s][%s]\t\t[%s]\n"
								, iMaxMCfgCode
								, stMCfgCode[iMaxMCfgCode].sFldNo
								, stMCfgCode[iMaxMCfgCode].sCode
								, stMCfgCode[iMaxMCfgCode].sName
							);
						}

						iMaxMCfgCode ++;
					}
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);
	}
}

/*============================================================================*/
/*
 * ���� : sorted�� Ȯ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCodeCheckSorted()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i = 1; i < iMaxMCfgCode; i++)
		{
			char str1[50];
			char str2[50];

			sprintf(str1, "%-6.6s.%-6.6s", stMCfgCode[i-1].sFldNo, stMCfgCode[i-1].sCode);
			sprintf(str2, "%-6.6s.%-6.6s", stMCfgCode[i  ].sFldNo, stMCfgCode[i  ].sCode);

			if (strcmp(str1, str2) >= 0)
			{
				printf("%06d) [%s] >= [%s]\n", i, str1, str2);
				if (!flag) break;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCodeSearch(char* sFieldNo, char* sCode)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char str1[100];
		char str2[100];
		int iL, iM, iR;

		sprintf(str1, "%-6.6s.%-20.20s", sFieldNo, sCode);

		iL = 0;
		iR = iMaxMCfgCode - 1;

		int i;
		for (i=0; i < 20; i++)
		{
			iM = (iL + iR) / 2;

			sprintf(str2, "%-6.6s.%-20.20s", stMCfgCode[iM].sFldNo, stMCfgCode[iM].sCode);

			if (strcmp(str1, str2) < 0)
			{
				/*
				 * ���� �۴�
				 */
				if (!flag) printf("STATUS : \t\t[str1:%s] < [%s]\n", str1, str2);
				iR = iM - 1;
			}
			else if (strcmp(str2, str1) < 0)
			{
				/*
				 * ���� ũ��
				 */
				if (!flag) printf("STATUS : [%s] < [str1:%s]\n", str2, str1);
				iL = iM + 1;
			}
			else
			{
				/*
				 * ã�Ҵ�.
				 */
				if (!flag) printf("SUCCESS : find...[%s]\n", str2);
				return iM;
			}

			if (iR < iL)
			{
				/*
				 * ã�� �ڷᰡ ����.
				 */
				if (!flag) printf("FAIL : not found...[iR:%d][iL:%d][%s][%s]\n", iR, iL, str1, str2);
				break;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCodeSearch2(char* sFieldNo)
/*----------------------------------------------------------------------------*/
{
	return MCfgCodeSearch(sFieldNo, "");
}

/*============================================================================*/
/*
 * ���� : comp.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgComp()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.comp") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxMCfgComp = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);

			if (flag)
			{
				char sFldNo[6+1];

				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				strcpy(sFldNo , UtilTrim(arrItem[0]));
				strcpy(strLine, UtilTrim(arrItem[1]));

				if (flag)
				{
					/*
					 * ù��° �׸��� �����. Search�� ����.
					 */
					strcpy(stMCfgComp[iMaxMCfgComp].sFldNo , sFldNo);
					strcpy(stMCfgComp[iMaxMCfgComp].sSeq   , "");
					strcpy(stMCfgComp[iMaxMCfgComp].sSubFNo, "------");

					if (!flag) {
						printf("%05d) [%s][%s]\t\t[%s]\n"
							, iMaxMCfgComp
							, stMCfgComp[iMaxMCfgComp].sFldNo
							, stMCfgComp[iMaxMCfgComp].sSeq
							, stMCfgComp[iMaxMCfgComp].sSubFNo
						);
					}

					iMaxMCfgComp ++;
				}

				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, ':', arrItem);

				int idx;
				for (idx = 0; idx < MAX_ITEM; idx++)
				{
					/* ��ü�� NULL�̸� �������� */
					if (arrItem[idx] == NULL)
						break;

					/* Code�� Name�� ������ �������� */
					UtilTrim(arrItem[idx]);
					if (*arrItem[idx] == 0x00)
						break;

					if (flag)
					{
						strcpy (stMCfgComp[iMaxMCfgComp].sFldNo , sFldNo);
						sprintf(stMCfgComp[iMaxMCfgComp].sSeq   , "%03d", idx+1);
						strcpy (stMCfgComp[iMaxMCfgComp].sSubFNo, arrItem[idx]);

						if (!flag) {
							printf("%05d) [%s][%s]\t\t[%s]\n"
								, iMaxMCfgComp
								, stMCfgComp[iMaxMCfgComp].sFldNo
								, stMCfgComp[iMaxMCfgComp].sSeq
								, stMCfgComp[iMaxMCfgComp].sSubFNo
							);
						}

						iMaxMCfgComp ++;
					}
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}
}

/*============================================================================*/
/*
 * ���� : sorted�� Ȯ���Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCompCheckSorted()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i = 1; i < iMaxMCfgComp; i++)
		{
			char str1[50];
			char str2[50];

			sprintf(str1, "%-6.6s.%-3.3s", stMCfgComp[i-1].sFldNo, stMCfgComp[i-1].sSeq);
			sprintf(str2, "%-6.6s.%-3.3s", stMCfgComp[i  ].sFldNo, stMCfgComp[i  ].sSeq);

			if (strcmp(str1, str2) >= 0)
			{
				printf("%06d) [%s] >= [%s]\n", i, str1, str2);
				if (!flag) break;
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCompSearch(char* sFieldNo, char* sCode)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char str1[100];
		char str2[100];
		int iL, iM, iR;

		sprintf(str1, "%-6.6s.%-3.3s", sFieldNo, sCode);

		iL = 0;
		iR = iMaxMCfgComp - 1;

		int i;
		for (i=0; i < 20; i++)
		{
			iM = (iL + iR) / 2;

			sprintf(str2, "%-6.6s.%-3.3s", stMCfgComp[iM].sFldNo, stMCfgComp[iM].sSeq);

			if (strcmp(str1, str2) < 0)
			{
				/*
				 * ���� �۴�
				 */
				if (!flag) printf("STATUS : \t\t[str1:%s] < [%s]\n", str1, str2);
				iR = iM - 1;
			}
			else if (strcmp(str2, str1) < 0)
			{
				/*
				 * ���� ũ��
				 */
				if (!flag) printf("STATUS : [%s] < [str1:%s]\n", str2, str1);
				iL = iM + 1;
			}
			else
			{
				/*
				 * ã�Ҵ�.
				 */
				if (!flag) printf("SUCCESS : find...[%s]\n", str2);
				return iM;
			}

			if (iR < iL)
			{
				/*
				 * ã�� �ڷᰡ ����.
				 */
				if (!flag) printf("FAIL : not found...[iR:%d][iL:%d][%s][%s]\n", iR, iL, str1, str2);
				break;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * ���� : search�Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MCfgCompSearch2(char* sFieldNo)
/*----------------------------------------------------------------------------*/
{
	return MCfgCompSearch(sFieldNo, "");
}

/*============================================================================*/
/*
 * ���� : system.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgSys()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.system") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxMCfgSys = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);

			if (flag)
			{
				char sFldNo[10+1];
				char sType[10+1];

				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				strcpy(sFldNo , UtilTrim(arrItem[1]));
				strcpy(sType  , UtilTrim(arrItem[2]));
				strcpy(strLine, UtilTrim(arrItem[3]));

				if (*sType == 'T')
				{
					char* p = index(strLine, ':') + 1;
					
					strcpy (stMCfgSys[iMaxMCfgSys].sHint , sFldNo);
					strcpy (stMCfgSys[iMaxMCfgSys].sInfo , p);
					
					if (!flag)
					{
						printf("%05d) [%s][%s]\n"
							, iMaxMCfgSys
							, stMCfgSys[iMaxMCfgSys].sHint
							, stMCfgSys[iMaxMCfgSys].sInfo
						);
					}
					
					iMaxMCfgSys ++;
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		if (!flag)
		{
			int i;
			for (i=0; i < iMaxMCfgSys; i++)
			{
				printf("%05d) [%s][%s]\n"
					, i
					, stMCfgSys[i].sHint
					, stMCfgSys[i].sInfo
				);
			}
		}
	}
}

/*============================================================================*/
/*
 * ���� : system.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgSystem()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.system") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		/* iMaxMCfgFld = 0; */

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (flag) printf("%s> [%s]\n", __FUNCTION__, strLine);


			/* iMaxMCfgFld ++; */
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*============================================================================*/
/*
 * ���� : Search Field
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgSearch(char* sFld)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		
	}
	
	return 0;
}

/*============================================================================*/
/*
 * ���� : Print Format Fields
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgCompFld(char* sCompFld)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int idx = MCfgCompSearch2(sCompFld);
		
		for (idx++; idx < iMaxMCfgComp; idx++)
		{
			if (strcmp(sCompFld, stMCfgComp[idx].sFldNo) != 0)
				break;
			
			if (!flag)
			{
				printf("\t\t COMP [%s][%s][%s]\n"
					, stMCfgComp[idx].sFldNo
					, stMCfgComp[idx].sSeq
					, stMCfgComp[idx].sSubFNo
				);
			}
			
			int iFldIdx = MCfgFldSearch(stMCfgComp[idx].sSubFNo);
			if (iFldIdx < 0)
			{
				if (flag) printf("ERROR : �ʵ������� �����ϴ�.\n");
				continue;
			}
			
			if (flag)
			{
				printf("\t\t stMCfgFld[%05d] = [%s][%s][%s][%s][%4d][%3d][%d][ %s (%s) ]\n"
					, iFldIdx
					, stMCfgFld[iFldIdx].sFldNo
					, stMCfgFld[iFldIdx].sFldType
					, stMCfgFld[iFldIdx].sFldLen
					, stMCfgFld[iFldIdx].sScale
					, iRecLen
					, stMCfgFld[iFldIdx].iFldLen
					, stMCfgFld[iFldIdx].iScale
					, stMCfgFld[iFldIdx].sFldName
					, stMCfgFld[iFldIdx].sComment
				);
			}
			
			iRecLen += stMCfgFld[iFldIdx].iFldLen;
		}
	}
	
	return 0;
}

/*============================================================================*/
/*
 * ���� : Print Format Fields
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgFormatFld(char* sFormatFld)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* arrItem[MAX_ITEM];

		if (flag)
		{
			/* �ڷḦ �����ڷ� �ɰ���. */
			UtilStrSplit(sFormatFld, ':', arrItem);

			iRecLen = 0;
			
			int i;
			for (i=0; i < MAX_ITEM; i++)
			{
				if (arrItem[i] == NULL)
					break;

				UtilTrim(arrItem[i]);
				if (*arrItem[i] == 0x00)
					continue;

				if (*arrItem[i] == '(')
				{
					/*
					 * Array ���� (Array Comp, ���ʰǼ�, �Ǽ��ʵ�)
					 * 1. Array �������� �Ǽ��� ���Ѵ�.
					 * 2. �����ڷῡ�� �Ǽ��� ���Ѵ�.
					 */
					continue;
				}
				
				int iFldIdx = MCfgFldSearch(arrItem[i]);
				if (iFldIdx < 0)
				{
					if (flag) printf("ERROR : �ʵ������� �����ϴ�.\n");
					continue;
				}

				if (flag)
				{
					printf("\t stMCfgFld[%05d] = [%s][%s][%s][%s][%4d][%3d][%d][ %s (%s) ]\n"
						, iFldIdx
						, stMCfgFld[iFldIdx].sFldNo
						, stMCfgFld[iFldIdx].sFldType
						, stMCfgFld[iFldIdx].sFldLen
						, stMCfgFld[iFldIdx].sScale
						, iRecLen
						, stMCfgFld[iFldIdx].iFldLen
						, stMCfgFld[iFldIdx].iScale
						, stMCfgFld[iFldIdx].sFldName
						, stMCfgFld[iFldIdx].sComment
					);
				}
				
				if (flag)
				{
					if (stMCfgFld[iFldIdx].sFldType[0] == 'A')
					{
						/*
						 * Array �׸���� ����Ѵ�.
						 */
						MCfgMsgCompFld(stMCfgFld[iFldIdx].sFldNo);
					}
				}
				
				iRecLen += stMCfgFld[iFldIdx].iFldLen;
			}
			
			if (flag) printf("\t\t\t\t\t iRecLen = %4d\n", iRecLen);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : Print Mapping Fields
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgMappingFld(char* sMappingFld)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* arrItem[MAX_ITEM];
		char* arrFld[2];

		if (flag)
		{
			/* �ڷḦ �����ڷ� �ɰ���. */
			UtilStrSplit(sMappingFld, ':', arrItem);

			int i;
			for (i=0; i < MAX_ITEM; i++)
			{
				if (arrItem[i] == NULL)
					break;

				UtilDelBracket(arrItem[i]);
				if (*arrItem[i] == 0x00)
					continue;

				/* �ڷḦ �����ڷ� �ɰ���. */
				UtilStrSplit(arrItem[i], ';', arrFld);

				if (!flag) printf("\t [%s] <- [%s]\n", arrFld[0], arrFld[1]);

				if (*arrFld[1] == 'V')
				{
					if (flag) printf("\t [%s] <- [%s]\n", arrFld[0], arrFld[1]+1);
					
					continue;
				}
				
				char* p = index(arrFld[1], '.');
				if (p != NULL)
					arrFld[1] = p + 1;
				
				if (strlen(arrFld[1]) > 6)
				{
					char* arr[3];
					UtilStrSplit(arrFld[1], ',', arr);
					
					int idx = MCfgFldSearch(arr[0]);

					if (flag) printf("\t [%s] <- [%s][%s][%3d][%d]  [%s,%s] [ %s (%s) ]\n"
						, arrFld[0]
						, stMCfgFld[idx].sFldNo
						, stMCfgFld[idx].sFldType
						, stMCfgFld[idx].iFldLen
						, stMCfgFld[idx].iScale
						, arr[1]
						, arr[2]
						, stMCfgFld[idx].sFldName
						, stMCfgFld[idx].sComment
					);
				}
				else
				if (strlen(arrFld[1]) == 6)
				{
					int idx = MCfgFldSearch(arrFld[1]);
					
					if (flag) printf("\t [%s] <- [%s][%s][%3d][%d]  [ %s (%s) ]\n"
						, arrFld[0]
						, stMCfgFld[idx].sFldNo
						, stMCfgFld[idx].sFldType
						, stMCfgFld[idx].iFldLen
						, stMCfgFld[idx].iScale
						, stMCfgFld[idx].sFldName
						, stMCfgFld[idx].sComment
					);
				}
				else
				{
					printf("ERROR: wrong size...\n");
				}
				
				if (!flag)
				{
					int   arrIdx[2];
					
					arrIdx[0] = MCfgFldSearch(arrFld[0]);
					if (arrIdx[0] < 0)
					{
						if (flag) printf("ERROR : �ʵ������� �����ϴ�.[%s]\n", arrItem[0]);
					}
					
					arrIdx[1] = MCfgFldSearch(arrFld[1]);
					if (arrIdx[1] < 0)
					{
						if (flag) printf("ERROR : �ʵ������� �����ϴ�.[%s]\n", arrItem[1]);
					}
	
					if (flag)
					{
						printf("\t [%s][%s][%3d][%d] <- [%s][%s][%3d][%d]  [ %s (%s) ] <- [ %s (%s) ]\n"
							, stMCfgFld[arrIdx[0]].sFldNo
							, stMCfgFld[arrIdx[0]].sFldType
							, stMCfgFld[arrIdx[0]].iFldLen
							, stMCfgFld[arrIdx[0]].iScale
							, stMCfgFld[arrIdx[1]].sFldNo
							, stMCfgFld[arrIdx[1]].sFldType
							, stMCfgFld[arrIdx[1]].iFldLen
							, stMCfgFld[arrIdx[1]].iScale
							, stMCfgFld[arrIdx[0]].sFldName
							, stMCfgFld[arrIdx[0]].sComment
							, stMCfgFld[arrIdx[1]].sFldName
							, stMCfgFld[arrIdx[1]].sComment
						);
					}
				}
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : msg_X_FEPID.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgX(char* sFepId, char* sMsgCd)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sFileName[512];
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.msg_X") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		strcpy(sFileName, stCfgMst[i].sVal);
		UtilSetFEPID(sFileName, sFepId);
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
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

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);

			if (flag)
			{
				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '|', arrItem);

				if (strcmp(sMsgCd, UtilTrim(arrItem[3])) == 0)
				{
					UtilTrim(arrItem[4]);
					UtilTrim(arrItem[5]);
					switch (*arrItem[4])
					{
						case 'D':   /* Description */
							if (flag) printf("Description : [%s] [%s]\n", (*arrItem[5] == '1' ? "�䱸" : "����"), arrItem[5]);
							break;
						case 'F':   /* Format (�Է��ʵ�) */
							if (flag) printf("Format (�Է��ʵ�)  : [%s]\n", arrItem[5]);
							if (flag) MCfgMsgFormatFld(arrItem[5]);
							break;
						case 'M':   /* Mapping (����ʵ�) */
							if (flag) printf("Mapping (����ʵ�) : [%s]\n", arrItem[5]);
							if (flag) MCfgMsgMappingFld(arrItem[5]);
							break;
						default:
							break;
					}
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		if (flag) printf("----------------------------------------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * ���� : msg_I_FEPID.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgMsgI(char* sFepId, char* sMsgCd)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sFileName[512];
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.msg_I") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		strcpy(sFileName, stCfgMst[i].sVal);
		UtilSetFEPID(sFileName, sFepId);
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		/* iMaxMCfgFld = 0; */

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);


			/* iMaxMCfgFld ++; */
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}
}

/*============================================================================*/
/*
 * ���� : routing.cfg �б�
 */
/*----------------------------------------------------------------------------*/
int MCfgRout()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];
		int i;
		int swFlag = 0;

		if (flag)
		{
			/*
			 * ������������ ã��
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.mapper.cfg.file.routing") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE ���� open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		/* iMaxMCfgFld = 0; */

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);
			if (!flag) printf("%s> [%s]\n", __FUNCTION__, strLine);


			/* iMaxMCfgFld ++; */
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

	}
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

	/*========================================================================*/

	if (flag)
	{
		/*
		 * system.cfg �� ���� �ڷḦ �д´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		MCfgFld();
		MCfgFldCd();
		MCfgCode();
		MCfgComp();
		MCfgSys();

		if (!flag) MCfgFldCheckSorted();
		if (!flag) MCfgCodeCheckSorted();
		if (flag) MCfgFldCdCheckSorted();
		if (!flag) MCfgCompCheckSorted();
		
		if (!flag) MCfgMsgX("KFT02", "0200_010000");
		if (!flag) MCfgMsgX("KBB01", "0810_1110");

		exit(-1);
	}

	/*========================================================================*/

	if (!flag)
	{
		/*
		 * comp.cfg �� ���� �ڷḦ �д´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		if (flag) MCfgComp();

		if (flag) MCfgCompCheckSorted();

		if (flag)
		{
			int i;
			for (i=0; i < iMaxMCfgComp; i++)
			{
				char* sFldNo = stMCfgComp[i].sFldNo;
				char* sSeq   = stMCfgComp[i].sSeq;

				int retIdx = MCfgCompSearch(sFldNo, sSeq);

				if (retIdx >= 0)
				{
					/*
					 * ã�Ҵ�
					 */
					if (retIdx == i)
					{
						/*
						 * ��Ȯ�ϰ� ã�Ҵ�.
						 */
						if (!flag) printf("SUCCESS : [%s][%s]\n", sFldNo, sSeq);
					}
					else
					{
						/*
						 * ����Ȯ�ϰ� ã�Ҵ�. ����
						 */
						if (flag) printf("ERROR : [%s][%s]\n", sFldNo, sSeq);
					}
				}
				else
				{
					/*
					 * ��ã�Ҵ�.
					 */
					if (flag) printf("NOT FOUND : [%s][%s]\n", sFldNo, sSeq);
				}
			}

			if (flag)
			{
				char* sFldNo = "511381";

				int idx = MCfgCompSearch2(sFldNo);

				if (flag) printf("SEARCH : [%s] -> idx[%d]\n", sFldNo, idx);
			}
		}

		exit(-1);
	}

	/*========================================================================*/

	if (!flag)
	{
		/*
		 * code.cfg �� ���� �ڷḦ �д´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		if (flag) MCfgCode();

		if (flag) MCfgCodeCheckSorted();

		if (flag)
		{
			int i;
			for (i=0; i < iMaxMCfgCode; i++)
			{
				char* sFldNo = stMCfgCode[i].sFldNo;
				char* sCode  = stMCfgCode[i].sCode;

				int retIdx = MCfgCodeSearch(sFldNo, sCode);

				if (retIdx >= 0)
				{
					/*
					 * ã�Ҵ�
					 */
					if (retIdx == i)
					{
						/*
						 * ��Ȯ�ϰ� ã�Ҵ�.
						 */
						if (!flag) printf("SUCCESS : [%s][%s]\n", sFldNo, sCode);
					}
					else
					{
						/*
						 * ����Ȯ�ϰ� ã�Ҵ�. ����
						 */
						if (flag) printf("ERROR : [%s][%s]\n", sFldNo, sCode);
					}
				}
				else
				{
					/*
					 * ��ã�Ҵ�.
					 */
					if (flag) printf("NOT FOUND : [%s][%s]\n", sFldNo, sCode);
				}
			}

			if (flag)
			{
				char* sFldNo = "000156";

				int idx = MCfgCodeSearch2(sFldNo);

				if (flag) printf("SEARCH : [%s] -> idx[%d]\n", sFldNo, idx);
			}
		}

		exit(-1);
	}

	/*========================================================================*/

	if (!flag)
	{
		/*
		 * fieldcode.cfg �� ���� �ڷḦ �д´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		if (flag) MCfgFldCd();

		if (flag) MCfgFldCdCheckSorted();

		if (flag)
		{
			int i;
			for (i=0; i < iMaxMCfgFldCd; i++)
			{
				char* sFldNo = stMCfgFldCd[i].sFldNo;

				int retIdx = MCfgFldCdSearch(sFldNo);

				if (retIdx >= 0)
				{
					/*
					 * ã�Ҵ�
					 */
					if (retIdx == i)
					{
						/*
						 * ��Ȯ�ϰ� ã�Ҵ�.
						 */
						if (flag) printf("SUCCESS : [%s]\n", sFldNo);
					}
					else
					{
						/*
						 * ����Ȯ�ϰ� ã�Ҵ�. ����
						 */
						if (flag) printf("ERROR : [%s]\n", sFldNo);
					}
				}
				else
				{
					/*
					 * ��ã�Ҵ�.
					 */
					if (flag) printf("NOT FOUND : [%s]\n", sFldNo);
				}
			}
		}

		exit(-1);
	}

	/*========================================================================*/

	if (!flag)
	{
		/*
		 * field.cfg �� ���� �ڷḦ �д´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		MCfgFld();
		MCfgComp();
		MCfgCode();
		MCfgFldCd();
		MCfgRout();
		MCfgSystem();

		exit(-1);
	}

	if (!flag)
	{
		/*
		 * Mapper �ʵ带 �˻��Ѵ�.
		 * Binary Search �� ���ؼ� �ڷḦ FieldNo�� sort�� ���´�.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		MCfgFld();

		MCfgFldCheckSorted();

		int idx = MCfgFldSearch("100422");
		if (idx >= 0)
		{
			if (flag)
			{
				printf("%06d> stMCfgFld[%d].sFldNo   = [%s]\n", idx, idx, stMCfgFld[idx].sFldNo  );
				printf("%06d> stMCfgFld[%d].sFldName = [%s]\n", idx, idx, stMCfgFld[idx].sFldName);
				printf("%06d> stMCfgFld[%d].sFldType = [%s]\n", idx, idx, stMCfgFld[idx].sFldType);
				printf("%06d> stMCfgFld[%d].sFldLen  = [%s]\n", idx, idx, stMCfgFld[idx].sFldLen );
				printf("%06d> stMCfgFld[%d].sScale   = [%s]\n", idx, idx, stMCfgFld[idx].sScale  );
				printf("%06d> stMCfgFld[%d].iFldLen  = [%d]\n", idx, idx, stMCfgFld[idx].iFldLen );
				printf("%06d> stMCfgFld[%d].iScale   = [%d]\n", idx, idx, stMCfgFld[idx].iScale  );
				printf("%06d> stMCfgFld[%d].sComment = [%s]\n", idx, idx, stMCfgFld[idx].sComment);
			}
		}
	}

	if (!flag)
	{
		/*
		 * Mapper �ʵ� ��θ� �˻��غ���.
		 */

		strcpy(sServiceid, "MAACOM0000");

		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		MCfgFld();

		int i;
		for (i=0; i < iMaxMCfgFld; i++)
		{
			char* sFldNo = stMCfgFld[i].sFldNo;

			int retIdx = MCfgFldSearch(sFldNo);

			if (retIdx >= 0)
			{
				/*
				 * ã�Ҵ�
				 */
				if (retIdx == i)
				{
					/*
					 * ��Ȯ�ϰ� ã�Ҵ�.
					 */
					if (flag) printf("SUCCESS : [%s]\n", sFldNo);
				}
				else
				{
					/*
					 * ����Ȯ�ϰ� ã�Ҵ�. ����
					 */
					if (flag) printf("ERROR : [%s]\n", sFldNo);
				}
			}
			else
			{
				/*
				 * ��ã�Ҵ�.
				 */
				if (flag) printf("NOT FOUND : [%s]\n", sFldNo);
			}
		}
	}

}

#endif

