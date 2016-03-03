/*
 * PROGRAM ID   : Ums.c
 * PROGRAM NAME : Ums ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Ums ���� ���
 * HISTORY      : 2013.11.07 ����. �����ۼ�
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define MAX_ITEM           10
#define MAX_LINE_SIZE    8192

#define MAX_LEN_TRAN      512


#define MAX_LOGFLAG        10
#define MAX_SENDER          5
#define MAX_RECVER         20

#define LOGFLAGNO_WRITE     0
#define LOGFLAGNO_ISCHANGE  1
#define LOGFLAGNO_GETCFG    2
#define LOGFLAGNO_READ      3
#define LOGFLAGNO_SEND      4

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

char          sHostIp              [20];               /* UMS ���� IP        */
int           iHostPort                ;               /* UMS ���� PORT      */
int           iMaxSendCnt              ;               /* UMS �ִ� ���۰Ǽ�  */

time_t        oldTime      = (time_t) 0;      /* UMS0120000 ���� ����ð�    */

/*----------------------------------------------------------------------------*/

char          cLogFlag             [MAX_LOGFLAG];      /* LOG ��� �÷���    */

UMS_CFG       stSender             [MAX_SENDER];       /* �߼��� ����        */
int           iSenderCnt                       ;

UMS_CFG       stRecver             [MAX_RECVER];       /* ������ ����        */
int           iRecverCnt                       ;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int UmsTest()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * CFG ȯ�������� �д´�.
		 */

		FILE* fp;
		char* p;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];
		char* arr[MAX_ITEM];
		char* arr1[MAX_ITEM];
		int i;

		p = CfgGetMst("ib.cfg.file.ums");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS ȯ������ ������");
			return -1;
		}
		strcpy(sFileName, p);

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(sFileName, "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
			return -1;
		}

		iSenderCnt = 0;
		iRecverCnt = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (flag) LOG(_FL_, 0, 0, "> [%s] [%s]", sServiceid, strLine);

			/* �Ѷ����� �����ڷ� �ɰ���. */
			UtilStrSplit(strLine, '=', arr);

			if (flag)
			{
				if (strcmp("HOSTINFO", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(sHostIp , UtilTrim(arr1[0]));
					iHostPort =  UtilToInt(UtilTrim(arr1[1]));

					if (!flag) LOG(_FL_, 0, 0, " [HOSTINFO:%s:%d]", sHostIp, iHostPort);
				}
				else if (strcmp("LOGFLAG", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					for (i=0; arr1[i] && i < MAX_ITEM; i++)
					{
						UtilTrim(arr1[i]);

						cLogFlag[i] = arr1[i][0];
					}
					cLogFlag[i] = 0x00;

					if (!flag) LOG(_FL_, 0, 0, " [cLogFlag:%s]", cLogFlag);
				}
				else if (strcmp("SENDER", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(stSender[iSenderCnt].sUseYn , UtilTrim(arr1[0]));
					strcpy(stSender[iSenderCnt].sEmpNo , UtilTrim(arr1[1]));
					strcpy(stSender[iSenderCnt].sTmCode, UtilTrim(arr1[2]));
					strcpy(stSender[iSenderCnt].sName  , UtilTrim(arr1[3]));
					strcpy(stSender[iSenderCnt].sPhone , UtilTrim(arr1[4]));

					iSenderCnt ++;
				}
				else if (strcmp("RECVER", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(stRecver[iRecverCnt].sUseYn , UtilTrim(arr1[0]));
					strcpy(stRecver[iRecverCnt].sEmpNo , UtilTrim(arr1[1]));
					strcpy(stRecver[iRecverCnt].sTmCode, UtilTrim(arr1[2]));
					strcpy(stRecver[iRecverCnt].sName  , UtilTrim(arr1[3]));
					strcpy(stRecver[iRecverCnt].sPhone , UtilTrim(arr1[4]));

					iRecverCnt ++;
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		if (flag)
		{
			/* HOSTINFO */
			LOG(_FL_, 0, 0, "[HOSTINFO:%s:%d]", sHostIp, iHostPort);

			/* LOGFLAG */
			LOG(_FL_, 0, 0, "[cLogFlag:%s]", cLogFlag);

			/* SENDER */
			for (i=0; i < iSenderCnt; i++)
			{
				LOG(_FL_, 0, 0, "stSender(%d).sUseYn  = [%s]", i, stSender[i].sUseYn );
				LOG(_FL_, 0, 0, "stSender(%d).sEmpNo  = [%s]", i, stSender[i].sEmpNo );
				LOG(_FL_, 0, 0, "stSender(%d).sTmCode = [%s]", i, stSender[i].sTmCode);
				LOG(_FL_, 0, 0, "stSender(%d).sName   = [%s]", i, stSender[i].sName  );
				LOG(_FL_, 0, 0, "stSender(%d).sPhone  = [%s]", i, stSender[i].sPhone );
			}

			/* RECVER */
			for (i=0; i < iRecverCnt; i++)
			{
				LOG(_FL_, 0, 0, "stRecver(%d).sUseYn  = [%s]", i, stRecver[i].sUseYn );
				LOG(_FL_, 0, 0, "stRecver(%d).sEmpNo  = [%s]", i, stRecver[i].sEmpNo );
				LOG(_FL_, 0, 0, "stRecver(%d).sTmCode = [%s]", i, stRecver[i].sTmCode);
				LOG(_FL_, 0, 0, "stRecver(%d).sName   = [%s]", i, stRecver[i].sName  );
				LOG(_FL_, 0, 0, "stRecver(%d).sPhone  = [%s]", i, stRecver[i].sPhone );
			}
		}

		exit(0);
	}

	if (!flag)
	{
		/*
		 * DAT ���Ͽ� �޽����� ����.
		 */

		FILE* fp;
		char* p;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			return -1;
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		if (flag)
		{
			if (flag)
			{
				/* ������ Ȯ���Ѵ�. ������ �����. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (flag) LOG(_FL_, 0, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* ������ ������ �����Ѵ�. */
					mkdir(sPath, 0750);
				}
			}

			if (flag)
			{
				/* DAT ������ Ȯ���Ͽ� ������ �����Ѵ�. */
				if (access(sFileName, W_OK) < 0)
				{
					/* ������ ������ �����Ѵ�. */
					fp = fopen(sFileName, "w");
					fclose(fp);
				}
			}


			/* DAT ������ �����Ѵ�. */
			fp = fopen(sFileName, "a");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
				return -1;
			}
		}

		fprintf(fp, "%14.14s%14.14s%-160.160s\n"
			, TimeGetDateTime()
			, ""
			, "[pfep03][UMS0120000]���! ���! ���!");

		fflush(fp);

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		exit(0);
	}

	if (!flag)
	{
		/* DAT ������ ������ Ȯ���Ѵ�. */
		char sFileName[128];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			return -1;
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		if (flag)
		{
			struct stat st;
			memset((void*)&st, 0x00, sizeof(struct stat));

			if (lstat(sFileName, &st) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : file not found. [%s]", sFileName);
				return -1;
			}

			if (st.st_mtime == oldTime)
			{
				/* ������ ������ ����. */
				return 0;
			}

			if (flag) LOG(_FL_, 0, 0, "DAT���� ���� : st.st_mtime=[%d], oldTime=[%d]"
				, st.st_mtime, oldTime);

			oldTime = st.st_mtime;
		}

		return oldTime;
	}

	if (flag)
	{
		/*
		 * DAT ���Ͽ��� �޽����� �д´�.
		 */

		FILE* fp;
		char* p;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			return -1;
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		/* DAT ������ �����Ѵ�. */
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
			return -1;
		}

		while ( 1 )
		{
			strLine[0] = 0x00;

			long pos = ftell(fp);
			char* p = fgets(strLine, MAX_LINE_SIZE, fp);
			if (p == NULL)
				break;

			UMS_FILE* pDat = (UMS_FILE*) &strLine;
			if (strncmp(pDat->sSendTm, "              ", 14) == 0)
			{
				memcpy(pDat->sSendTm, TimeGetDateTime(), 14);
				memcpy(pDat->sSendStat, "_TEST_", 6);

				fseek(fp, pos, SEEK_SET);
				fputs(strLine, fp);

				pDat->sLF[0] = 0x00;
				if (flag) LOG(_FL_, 0, 0, "[MSG:%s]", pDat->sMessage);

				break;
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		exit(0);
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int UmsWrite(char* sMsg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * DAT ���Ͽ� �޽����� ����.
		 */

		FILE* fp;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (!flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		if (flag)
		{
			if (flag)
			{
				/* ������ Ȯ���Ѵ�. ������ �����. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (!flag) LOG(_FL_, 0, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* ������ ������ �����Ѵ�. */
					mkdir(sPath, 0750);
				}
			}

			if (flag)
			{
				/* DAT ������ Ȯ���Ͽ� ������ �����Ѵ�. */
				if (access(sFileName, W_OK) < 0)
				{
					/* ������ ������ �����Ѵ�. */
					fp = fopen(sFileName, "w");
					fclose(fp);
				}
			}


			/* DAT ������ �����Ѵ�. */
			fp = fopen(sFileName, "a");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
				if (flag) exit(-1);
				return -1;            /* TODO DATE.2013.11.07 */
			}
		}

		fprintf(fp, "%14.14s%14.14s%6.6s%-160.160s\n"
			, TimeGetDateTime()
			, ""
			, ""
			, sMsg);

		fflush(fp);

		/* ó���� ������ �ݴ´�. */
		fclose(fp);
	}

	return strlen(sMsg);
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int UmsIsChange()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 9, 0, "UMS Change check..." , "");

	if (flag)
	{
		/* DAT ������ ������ Ȯ���Ѵ�. */
		char sFileName[128];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (!flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		if (flag)
		{
			struct stat st;
			memset((void*)&st, 0x00, sizeof(struct stat));

			if (lstat(sFileName, &st) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : file not found. [%s]", sFileName);
				if (flag) exit(-1);
				return -1;            /* TODO DATE.2013.11.07 */
			}

			if (st.st_mtime == oldTime)
			{
				/* ������ ������ ����. */
				return 0;
			}

			if (flag) LOG(_FL_, 0, 0, "DAT���� [sFileName:%s] [st.st_mtime:%d] [oldTime:%d]"
				, sFileName, st.st_mtime, oldTime);

			oldTime = st.st_mtime;
		}

		return oldTime;
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int UmsGetCfg()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * CFG ȯ�������� �д´�.
		 */

		FILE* fp;
		char sFileName[128];

		char strLine[MAX_LINE_SIZE];
		char* arr[MAX_ITEM];
		char* arr1[MAX_ITEM];
		int i;

		char* p = CfgGetMst("ib.cfg.file.ums");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS ȯ������ ������");
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);

		/* ó���� ������ �����Ѵ�. */
		fp = fopen(sFileName, "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}

		iSenderCnt = 0;
		iRecverCnt = 0;

		/* �� ���ξ� ������ ó���� �Ѵ�. */
		while (1)
		{
			/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "> [%s] [%s]", sServiceid, strLine);

			/* �Ѷ����� �����ڷ� �ɰ���. */
			UtilStrSplit(strLine, '=', arr);

			if (flag)
			{
				if (strcmp("HOSTINFO", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(sHostIp , UtilTrim(arr1[0]));
					iHostPort =  UtilToInt(UtilTrim(arr1[1]));

					if (!flag) LOG(_FL_, 0, 0, " [HOSTINFO:%s:%d]", sHostIp, iHostPort);
				}
				if (strcmp("MAXSENDCNT", UtilTrim(arr[0])) == 0)
				{
					iMaxSendCnt =  UtilToInt(UtilTrim(arr[1]));

					if (!flag) LOG(_FL_, 0, 0, " [MAXSENDCNT:%d]", iMaxSendCnt);
				}
				else if (strcmp("LOGFLAG", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					for (i=0; arr1[i] && i < MAX_ITEM; i++)
					{
						UtilTrim(arr1[i]);

						cLogFlag[i] = arr1[i][0];
					}
					cLogFlag[i] = 0x00;

					if (!flag) LOG(_FL_, 0, 0, " [cLogFlag:%s]", cLogFlag);
				}
				else if (strcmp("SENDER", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(stSender[iSenderCnt].sUseYn , UtilTrim(arr1[0]));
					strcpy(stSender[iSenderCnt].sEmpNo , UtilTrim(arr1[1]));
					strcpy(stSender[iSenderCnt].sTmCode, UtilTrim(arr1[2]));
					strcpy(stSender[iSenderCnt].sName  , UtilTrim(arr1[3]));
					strcpy(stSender[iSenderCnt].sPhone , UtilTrim(arr1[4]));

					iSenderCnt ++;
				}
				else if (strcmp("RECVER", UtilTrim(arr[0])) == 0)
				{
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(stRecver[iRecverCnt].sUseYn , UtilTrim(arr1[0]));
					strcpy(stRecver[iRecverCnt].sEmpNo , UtilTrim(arr1[1]));
					strcpy(stRecver[iRecverCnt].sTmCode, UtilTrim(arr1[2]));
					strcpy(stRecver[iRecverCnt].sName  , UtilTrim(arr1[3]));
					strcpy(stRecver[iRecverCnt].sPhone , UtilTrim(arr1[4]));

					iRecverCnt ++;
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		if (!flag)
		{
			/* HOSTINFO */
			LOG(_FL_, 0, 0, "[HOSTINFO:%s:%d]", sHostIp, iHostPort);

			/* LOGFLAG */
			LOG(_FL_, 0, 0, "[cLogFlag:%s]", cLogFlag);

			/* SENDER */
			for (i=0; i < iSenderCnt; i++)
			{
				if (!flag)
				{
					LOG(_FL_, 0, 0, "stSender(%d).sUseYn  = [%s]", i, stSender[i].sUseYn );
					LOG(_FL_, 0, 0, "stSender(%d).sEmpNo  = [%s]", i, stSender[i].sEmpNo );
					LOG(_FL_, 0, 0, "stSender(%d).sTmCode = [%s]", i, stSender[i].sTmCode);
					LOG(_FL_, 0, 0, "stSender(%d).sName   = [%s]", i, stSender[i].sName  );
					LOG(_FL_, 0, 0, "stSender(%d).sPhone  = [%s]", i, stSender[i].sPhone );
				}

				if (flag) LOG(_FL_, 0, 0, "stSender(%d) [%s][%s][%s][%s][%s]", i
					, stSender[i].sUseYn
					, stSender[i].sEmpNo
					, stSender[i].sTmCode
					, stSender[i].sName
					, stSender[i].sPhone );
			}

			/* RECVER */
			for (i=0; i < iRecverCnt; i++)
			{
				if (!flag)
				{
					LOG(_FL_, 0, 0, "stRecver(%d).sUseYn  = [%s]", i, stRecver[i].sUseYn );
					LOG(_FL_, 0, 0, "stRecver(%d).sEmpNo  = [%s]", i, stRecver[i].sEmpNo );
					LOG(_FL_, 0, 0, "stRecver(%d).sTmCode = [%s]", i, stRecver[i].sTmCode);
					LOG(_FL_, 0, 0, "stRecver(%d).sName   = [%s]", i, stRecver[i].sName  );
					LOG(_FL_, 0, 0, "stRecver(%d).sPhone  = [%s]", i, stRecver[i].sPhone );
				}

				if (flag) LOG(_FL_, 0, 0, "stRecver(%d) [%s][%s][%s][%s][%s]", i
					, stRecver[i].sUseYn
					, stRecver[i].sEmpNo
					, stRecver[i].sTmCode
					, stRecver[i].sName
					, stRecver[i].sPhone );
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
char* UmsRead(char* sSendMsg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	
	if (flag) LOG(_FL_, 0, 0, "[UmsRead]=[%s]", sSendMsg);
		
	if (flag)
	{
		/*
		 * DAT ���Ͽ��� �޽����� �д´�.
		 */

		FILE* fp;
		char sMessageTemp[160+1];
		char sFileName[128];
		char sTemp    [2+1];
		char sCurrentTime[2+1];
		char strLine[MAX_LINE_SIZE];
		
		int iTempCnt = 0;
		int iDupchk  = 0;
		
		memset(sTemp     , 0x00, sizeof(sTemp)    );
		memset(sCurrentTime , 0x00, sizeof(sCurrentTime));

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT���� ������");
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (!flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		/* DAT ������ �����Ѵ�. */
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE ���� open error...[%s]", sFileName);
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}
		
		sMessageTemp[0] = 0x00;
		
		memcpy(sCurrentTime, TimeGetDateTime() + 8, 2);
		
		if (!flag) LOG(_FL_, 0, 0, "[����ð�:%s]", sCurrentTime);

		while ( 1 )
		{
			if (!flag) LOG(_FL_, 0, 0, "[while]");
				
			strLine[0] = 0x00;
			

			long pos = ftell(fp);
			char* p = fgets(strLine, MAX_LINE_SIZE, fp);
			if (p == NULL)
				break;

			UMS_FILE* pDat = (UMS_FILE*) &strLine;
			
            if (!flag) LOG(_FL_, 0, 0, "[iMaxSendCnt:%d]", iMaxSendCnt);
            	
			if (flag)
		    {
		    	
		    	if (!flag) LOG(_FL_, 0, 0, "[�����б�:%s]", strLine);
		    		
		    	/* 
                 *  ��Ͻð��� üũ�Ǿ��ִ� ���� �ð��� �����Ѵ�
                 *  �ѽð����� 5�� �̻� �߼� ����
                 */
			    if (strncmp(pDat->sWriteTm , "              ", 14) != 0)
			    {
			    	/* ��Ͻð� ���� */
			        memcpy(sTemp, pDat->sWriteTm + 8 , 2);
			        
			        if (!flag) LOG(_FL_, 0, 0, "[��Ͻð�:%s]", sTemp);
			        
			        /* ���Ͻð��ϰ�� */
			        if (strncmp(sCurrentTime , sTemp, 2) == 0 ) {
			        	
			        	if (strncmp(pDat->sSendStat, "_READ_", 6) == 0 ) { 
			                iTempCnt++; /* ���۰Ǽ� */
                        }
			            
			        	/* �ߺ� üũ */
		                if ( strncmp(pDat->sSendTm, "              ", 14) == 0 ) {
		        	        if (strncmp(sMessageTemp , pDat->sMessage, 160) == 0) {
			        		    iDupchk = 1; /* �ߺ� */
			        	    } else {
			        	    	memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
			        	    }
			        	    			        	    
		                } else {
		                	memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
		                }
		                
			        } else {
			        	iTempCnt = 1;  /* ���ο� �ð� ���� */
                        
			            memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
			            iDupchk = 0; /* �ߺ� �ƴ� */
			        }
		        }
		        
		        if ( strncmp(pDat->sSendTm, "              ", 14) == 0 && iTempCnt > iMaxSendCnt ) {
		        	if (flag) LOG(_FL_, 0, 0, "(%s��) %d�� > ���� %d�� => SKIP, MSG_READ[%s]"
		        		          , sCurrentTime, iTempCnt, iMaxSendCnt, pDat->sMessage);
		        		
		        	memcpy(pDat->sSendTm, TimeGetDateTime(), 14);
                    memcpy(pDat->sSendStat, "_PASS_", 6);
			    	pDat->sLF[0] = 0x00;
                
			    	fseek(fp, pos, SEEK_SET);
			    	fputs(strLine, fp);
                
			    	strcpy(sSendMsg, "PASS");
			    	break;
		        	 
		        }
		        
		        if (!flag) LOG(_FL_, 0, 0, "[�ߺ�üũ]=%d", iDupchk);
		        	
		        if ( strncmp(pDat->sSendTm, "              ", 14) == 0 && iDupchk ) {
		        	if (flag) LOG(_FL_, 0, 0, "(%s��)�ߺ� �޽��� => SKIP, MSG_READ[%s]"
		        		          , sCurrentTime, pDat->sMessage);
                    memcpy(pDat->sSendTm, TimeGetDateTime(), 14);
                    memcpy(pDat->sSendStat, "_PASS_", 6);
                    pDat->sLF[0] = 0x00;

                    fseek(fp, pos, SEEK_SET);
                    fputs(strLine, fp);

                    strcpy(sSendMsg, "PASS");
                    break;

		        }
		    }
		    if (!flag) LOG(_FL_, 0, 0, "[  END]�񱳽���");
			
			if (!flag) LOG(_FL_, 0, 0, "[START]..");
			if (strncmp(pDat->sSendTm, "              ", 14) == 0)
			{
				if (flag) LOG(_FL_, 0, 0, "MSG_READ[%s]", pDat->sMessage);
					
				memcpy(pDat->sSendTm, TimeGetDateTime(), 14);
				memcpy(pDat->sSendStat, "_READ_", 6);
				pDat->sLF[0] = 0x00;

				fseek(fp, pos, SEEK_SET);
				fputs(strLine, fp);

				strcpy(sSendMsg, pDat->sMessage);
				break;
			}
			
			if (!flag) LOG(_FL_, 0, 0, "[END]..");
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);

		if (strLine[0] == 0x00)
			return NULL;


		if (flag)
		{
			struct stat st;
			memset((void*)&st, 0x00, sizeof(struct stat));

			lstat(sFileName, &st);

			oldTime = st.st_mtime;
		}
	}

	return sSendMsg;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
char* UmsUTF8fromEUCKR(char* str, int strLen)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, " ##### UmsUTF8fromEUCKR[%d:%s]", strLen, str);
		
	if (flag)
	{
		/*
		 * iconv�� �̿��� ��ȯ
		 * UTF-8 <- EUC-KR ��ȯ
		 */

		iconv_t cd = iconv_open("UTF-8", "EUC-KR");   /* to <- from */
		if (cd == (iconv_t)(-1))
		{
			LOG(_FL_, 0, 0, "ERROR: iconv_open error..");
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}

		/* �۾���������� ����� Ŭ���� �Ѵ�. */
		char inBuf[MAX_LEN_TRAN];
		char outBuf[MAX_LEN_TRAN];
		memset(inBuf, 0x00, MAX_LEN_TRAN);
		memset(outBuf, 0x00, MAX_LEN_TRAN);

		/* ��ȯ �ҽ��� �����Ѵ�. */
		strcpy(inBuf, str);

		/* ��ȯ ó���� ���� ���ʺ����� �����Ѵ�. */
		char* in  = inBuf;
		char* out = outBuf;
		size_t inSize  = strlen(inBuf);
		size_t outSize = sizeof(outBuf);

		if (!flag) LOG(_FL_, 0, 0, "BEFORE [inSize:%d] [outSize:%d]", inSize, outSize);

		/* ��ȯ ó���� �Ѵ�. */
		if (iconv(cd, &in, &inSize, &out, &outSize) == -1)
		{
			LOG(_FL_, 0, 0, "ERROR: iconv error..");
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}
		else
		{
			if (!flag) LOG(_FL_, 0, 0, "[in:%x] [out:%x]", in, out);
			if (!flag) LOG(_FL_, 0, 0, "AFTER [inSize:%d] [outSize:%d]", inSize, outSize);

			int idx;
			for(idx = 0; !flag && idx < 300; idx++)
			{
				printf("%03d %c %x\t\t", idx, inBuf[idx], inBuf[idx]);
				printf("%03d %c %x\n", idx, outBuf[idx], outBuf[idx]);
			}

			/* ��ȯ ó���� �� ���� ��´�. */
			sprintf(str, "%-*.*s", strLen, strLen, outBuf);
			
			if (!flag) LOG(_FL_, 0, 0, "[%d] [%d:%s]", strLen, strlen(str), str);
		}

		iconv_close(cd);
	}
	
	return str;
}


/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int UmsSend(char* sendMsg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (strncmp(sendMsg , "PASS", 4) == 0)
	{
	    return 0;	
	}

	if (flag)
	{
		int sock;

		if (flag)
		{
			/* socket�� �����Ѵ�. */
			sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock < 0)
			{
				LOG(_FL_, 0, 0, "ERROR: socket ....");
				if (flag) exit(-1);
				return -1;            /* TODO DATE.2013.11.07 */
			}
		}

		if (flag)
		{
			/* address ������ �����Ѵ�. */
			struct sockaddr_in sAddr;

			memset(&sAddr, 0x00, sizeof(sAddr));
			sAddr.sin_family      = AF_INET;
			sAddr.sin_addr.s_addr = inet_addr(sHostIp);
			sAddr.sin_port        = htons(iHostPort);

			/*
			 * ������ �ش� �ּҷ� ����
			 * ������ �ʵǸ� �������� �õ��Ѵ�.
			 */
			int ret = connect(sock, (struct sockaddr*) &sAddr, sizeof(sAddr));
			if (ret < 0)
			{
				LOG(_FL_, 0, 0, "ERROR: connection error [%s:%d]", sHostIp, iHostPort);
				if (flag) sleep(2);
				if (flag) exit(-1);
				return -1;            /* TODO DATE.2013.11.07 */
			}
		}
		
		if (flag) LOG(_FL_, 0, 0, "������ [%d]��", iRecverCnt);

		if (flag)
		{
			/*
			 * RECVER�� �޽����� �۽��ϰ� ����� �޴� ó���� �ݺ��Ѵ�.
			 * ���� ��
			 *     "00" : ���强��
			 *     "10" : ���������� ����
			 *     "99" : �ý��� ����
			 */
			int i;
			for (i=0; i < iRecverCnt; i++)
			{
				if (strcasecmp("Y", stRecver[i].sUseYn) != 0)
					continue;

				char sendLine    [MAX_LINE_SIZE];
				char printLine    [MAX_LINE_SIZE];
				
				char tranRecver  [MAX_LEN_TRAN];
				char tranSender  [MAX_LEN_TRAN];
				char tranLine    [MAX_LEN_TRAN];
				
				strcpy(tranRecver, stRecver[i].sName);
				strcpy(tranSender, stSender[0].sName);
				strcpy(tranLine  , sendMsg);
				
					
				/* ��¸޽����޽����� �����. */
				sprintf(printLine, "%-8.8s%-3.3s%-3.3s%-7.7s%-9.9s%-20.20s%-11.11s%-9.9s%-5.5s%-20.20s%-11.11s%-160.160s"
					, TimeGetDateTime()
					, "SYS"
					, "012"
					, "SA00010"
					, stRecver[i].sEmpNo
					, tranRecver
					, stRecver[i].sPhone
					, stSender[0].sEmpNo
					, stSender[0].sTmCode
					, tranSender
					, stSender[0].sPhone
					, tranLine
					);

				/* SMS �۽� �޽����� �����. */
				sprintf(sendLine, "%-8.8s%-3.3s%-3.3s%-7.7s%-9.9s%-20.20s%-11.11s%-9.9s%-5.5s%-20.20s%-11.11s%-160.160s"
					, TimeGetDateTime()
					, "SYS"
					, "012"
					, "SA00010"
					, stRecver[i].sEmpNo
					, UmsUTF8fromEUCKR(tranRecver, 20)
					, stRecver[i].sPhone
					, stSender[0].sEmpNo
					, stSender[0].sTmCode
					, UmsUTF8fromEUCKR(tranSender, 20)
					, stSender[0].sPhone
					, UmsUTF8fromEUCKR(tranLine, 160)
					);

				if (!flag)
				{
					/* SMS �۽� �޽����� Ȯ���Ѵ�. */
					UMS_SEND* pSend = (UMS_SEND*) &sendLine;

					LOG(_FL_, 0, 0, "%2d) pSend->sWorkDay     = [%-8.8s]    ", i, pSend->sWorkDay    );
					LOG(_FL_, 0, 0, "%2d) pSend->sUmsSendType = [%-3.3s]    ", i, pSend->sUmsSendType);
					LOG(_FL_, 0, 0, "%2d) pSend->sChannelCode = [%-3.3s]    ", i, pSend->sChannelCode);
					LOG(_FL_, 0, 0, "%2d) pSend->sWorkCode    = [%-7.7s]    ", i, pSend->sWorkCode   );
					LOG(_FL_, 0, 0, "%2d) pSend->sToId        = [%-9.9s]    ", i, pSend->sToId       );
					LOG(_FL_, 0, 0, "%2d) pSend->sToName      = [%-20.20s]  ", i, pSend->sToName     );
					LOG(_FL_, 0, 0, "%2d) pSend->sToPhone     = [%-11.11s]  ", i, pSend->sToPhone    );
					LOG(_FL_, 0, 0, "%2d) pSend->sFromId      = [%-9.9s]    ", i, pSend->sFromId     );
					LOG(_FL_, 0, 0, "%2d) pSend->sFromDept    = [%-5.5s]    ", i, pSend->sFromDept   );
					LOG(_FL_, 0, 0, "%2d) pSend->sFromName    = [%-20.20s]  ", i, pSend->sFromName   );
					LOG(_FL_, 0, 0, "%2d) pSend->sFromPhone   = [%-11.11s]  ", i, pSend->sFromPhone  );
					LOG(_FL_, 0, 0, "%2d) pSend->sContent     = [%-160.160s]", i, pSend->sContent    );
				}

				if (flag) LOG(_FL_, 0, 0, "SEND : (%d) [%s->%s] [%d:%s] [%s:%d]"
					, i, stSender[0].sName, stRecver[i].sName
					, strlen(printLine), printLine, sHostIp, iHostPort);

				if (flag)
				{
					/* SMS �۽� �޽����� �۽��Ѵ�. */
					if (SockSend(sock, sendLine, strlen(sendLine)) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR: SockSend error [%s:%d]", sHostIp, iHostPort);

						if (flag) exit(-1);
						return -1;            /* TODO DATE.2013.11.07 */
					}
				}

				if (flag)
				{
					char retLine[10];

					/* SMS �۽ſ� ���� ��������� �޴´�. */
					if (SockRecv(sock, retLine, 2) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR: SockRecv error [%s:%d]", sHostIp, iHostPort);

						if (flag) exit(-1);
						return -1;            /* TODO DATE.2013.11.07 */
					}

					if (memcmp("00", retLine, 2) == 0)
					{
						if (flag) LOG(_FL_, 0, 0, "SUCCESS : UMS Send....OK!!!");
					}
					else
					{
						if (flag) LOG(_FL_, 0, 0, "ERROR : UMS Send error [ret code:%2.2s]....", retLine);

						close(sock);
						return -1;
					}
				}
			}
		}

		if (flag)
		{
			/* socket�� close �Ѵ�. */

			close(sock);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : SMS �޽����� �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int UmsMsgSend(char* sMsg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	int ret;
	char sHostName[100];
	char sSendMsg[100];

	if (flag)
	{
		/* WRITE */
		if (flag) LOG(_FL_, 9, 0, "UMS Write...[%s]" , sMsg);

		if (CfgMst() < 0)
		{
			if (flag) LOG(_FL_, 9, 0, "ERROR : get msg....");
			return -1;
		}
		
		ret = gethostname(sHostName, sizeof(sHostName));
		
		sprintf(sSendMsg, "[%s]%s", sHostName, sMsg);

		UmsWrite(sSendMsg);
	}
	
	if (flag)
	{
		/* READ AND SEND */
		if (flag) LOG(_FL_, 9, 0, "UMS Read & Send..." , "");

		if (CfgMst() < 0)
		{
			if (flag) LOG(_FL_, 9, 0, "ERROR : get msg....");
			exit(9);
		}

		/* UmsRead ���� */
		char sendMsg[MAX_LINE_SIZE];

		if (UmsIsChange() > 0)
		{
			UmsGetCfg();

			while (1)
			{
				if (UmsRead((char*)&sendMsg) == NULL)
					break;

				if (flag) UmsSend((char*)&sendMsg); /* Ums ���� */

				if (flag) sleep(1);
			}
		}
	}

	return 0;
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

	strcpy(sServiceid, "UMS0120000");

	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s %s >\x1b[0m", sProgName, sServiceid);

	/**********************************************************/

	if (!flag)
	{
		if (CfgMst() < 0)
		{
			printf("ERROR : get msg....\n");
			exit(9);
		}

		/* UmsIsChange Ȯ�� �׽�Ʈ */
		while ( 1 )
		{
			UmsTest();
			sleep(3);
		}

		exit(0);
	}

	if (!flag)
	{
		if (CfgMst() < 0)
		{
			printf("ERROR : get msg....\n");
			exit(9);
		}

		UmsTest();
		exit(0);
	}

	/**********************************************************/

	if (flag)
	{
		if (argc != 3)
		{
			printf("\nUSAGE : %s [ w (�޽���) | r (..) ]\n\n", argv[0]);
			exit(0);
		}

		if (strcasecmp(argv[1], "W") == 0)
		{
			/* WRITE */
			if (!flag) printf("\n> UMS Write...\n\n");
			if (flag) LOG(_FL_, 9, 0, "UMS Write..." , "");

			if (CfgMst() < 0)
			{
				printf("ERROR : get msg....\n");
				exit(9);
			}

			/* UmsWrite ���� */

			char* pMsg = argv[2];
			/* char* pMsg = "[pfep03][UMS0120000] �׽�Ʈ�� �޽����Դϴ�."; */
			/* char* pMsg = "[pfep03][UMS0120000] This msg is for test."; */
			
			UmsWrite(pMsg);
		}
		else if (strcasecmp(argv[1], "R") == 0)
		{
			/* READ AND SEND */
			if (!flag) printf("\n> UMS Read and Send...\n\n");
			if (flag) LOG(_FL_, 9, 0, "UMS Read & Send..." , "");

			if (CfgMst() < 0)
			{
				printf("ERROR : get msg....\n");
				exit(9);
			}

			/* UmsRead ���� */
			char sendMsg[MAX_LINE_SIZE];

			while (1)
			{
				if (UmsIsChange() > 0)
				{
					UmsGetCfg();

					while (1)
					{
						if (UmsRead((char*)&sendMsg) == NULL)
							break;

						UmsSend((char*)&sendMsg);

						if (flag) sleep(1);
					}
				}

				if (flag) sleep(10);
			}
		}
		else
		{
			printf("\nUSAGE : %s [ w | r ]\n\n", argv[0]);
		}
	}
}

#endif
