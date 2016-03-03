/*
 * PROGRAM ID   : Ums.c
 * PROGRAM NAME : Ums 변수 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Ums 변수 모듈
 * HISTORY      : 2013.11.07 강석. 최초작성
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

char          sHostIp              [20];               /* UMS 서버 IP        */
int           iHostPort                ;               /* UMS 서버 PORT      */
int           iMaxSendCnt              ;               /* UMS 최대 전송건수  */

time_t        oldTime      = (time_t) 0;      /* UMS0120000 파일 변경시간    */

/*----------------------------------------------------------------------------*/

char          cLogFlag             [MAX_LOGFLAG];      /* LOG 출력 플래그    */

UMS_CFG       stSender             [MAX_SENDER];       /* 발송자 정보        */
int           iSenderCnt                       ;

UMS_CFG       stRecver             [MAX_RECVER];       /* 수신자 정보        */
int           iRecverCnt                       ;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int UmsTest()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * CFG 환경파일을 읽는다.
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
			LOG(_FL_, 0, 0, "ERROR : UMS 환경파일 미정의");
			return -1;
		}
		strcpy(sFileName, p);

		/* 처리할 파일을 오픈한다. */
		fp = fopen(sFileName, "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
			return -1;
		}

		iSenderCnt = 0;
		iRecverCnt = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (flag) LOG(_FL_, 0, 0, "> [%s] [%s]", sServiceid, strLine);

			/* 한라인을 구분자로 쪼갠다. */
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

		/* 처리된 파일을 닫는다. */
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
		 * DAT 파일에 메시지를 쓴다.
		 */

		FILE* fp;
		char* p;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
			return -1;
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		if (flag)
		{
			if (flag)
			{
				/* 폴더를 확인한다. 없으면 만든다. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (flag) LOG(_FL_, 0, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					mkdir(sPath, 0750);
				}
			}

			if (flag)
			{
				/* DAT 파일을 확인하여 없으면 생성한다. */
				if (access(sFileName, W_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					fp = fopen(sFileName, "w");
					fclose(fp);
				}
			}


			/* DAT 파일을 오픈한다. */
			fp = fopen(sFileName, "a");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
				return -1;
			}
		}

		fprintf(fp, "%14.14s%14.14s%-160.160s\n"
			, TimeGetDateTime()
			, ""
			, "[pfep03][UMS0120000]장애! 장애! 장애!");

		fflush(fp);

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		exit(0);
	}

	if (!flag)
	{
		/* DAT 파일의 갱신을 확인한다. */
		char sFileName[128];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
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
				/* 파일의 변경이 없다. */
				return 0;
			}

			if (flag) LOG(_FL_, 0, 0, "DAT파일 변경 : st.st_mtime=[%d], oldTime=[%d]"
				, st.st_mtime, oldTime);

			oldTime = st.st_mtime;
		}

		return oldTime;
	}

	if (flag)
	{
		/*
		 * DAT 파일에서 메시지를 읽는다.
		 */

		FILE* fp;
		char* p;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
			return -1;
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		/* DAT 파일을 오픈한다. */
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
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

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		exit(0);
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int UmsWrite(char* sMsg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * DAT 파일에 메시지를 쓴다.
		 */

		FILE* fp;
		char sFileName[128];
		char strLine[MAX_LINE_SIZE];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
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
				/* 폴더를 확인한다. 없으면 만든다. */
				char sPath[128];
				strcpy(sPath, sFileName);

				char* p = rindex(sPath, '/');
				*p = 0x00;
				if (!flag) LOG(_FL_, 0, 0, "[sPath:%s]", sPath);

				if (access(sPath, R_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					mkdir(sPath, 0750);
				}
			}

			if (flag)
			{
				/* DAT 파일을 확인하여 없으면 생성한다. */
				if (access(sFileName, W_OK) < 0)
				{
					/* 폴더가 없으면 생성한다. */
					fp = fopen(sFileName, "w");
					fclose(fp);
				}
			}


			/* DAT 파일을 오픈한다. */
			fp = fopen(sFileName, "a");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
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

		/* 처리된 파일을 닫는다. */
		fclose(fp);
	}

	return strlen(sMsg);
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int UmsIsChange()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 9, 0, "UMS Change check..." , "");

	if (flag)
	{
		/* DAT 파일의 갱신을 확인한다. */
		char sFileName[128];

		char* p = CfgGetMst("ib.path.file.ums.dat");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
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
				/* 파일의 변경이 없다. */
				return 0;
			}

			if (flag) LOG(_FL_, 0, 0, "DAT파일 [sFileName:%s] [st.st_mtime:%d] [oldTime:%d]"
				, sFileName, st.st_mtime, oldTime);

			oldTime = st.st_mtime;
		}

		return oldTime;
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int UmsGetCfg()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * CFG 환경파일을 읽는다.
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
			LOG(_FL_, 0, 0, "ERROR : UMS 환경파일 미정의");
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);

		/* 처리할 파일을 오픈한다. */
		fp = fopen(sFileName, "r");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
			if (flag) exit(-1);
			return -1;            /* TODO DATE.2013.11.07 */
		}

		iSenderCnt = 0;
		iRecverCnt = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "> [%s] [%s]", sServiceid, strLine);

			/* 한라인을 구분자로 쪼갠다. */
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

		/* 처리된 파일을 닫는다. */
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
 * 내용 :
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
		 * DAT 파일에서 메시지를 읽는다.
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
			LOG(_FL_, 0, 0, "ERROR : UMS DAT파일 미정의");
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}
		strcpy(sFileName, p);
		UtilSetYYYYMMDD(sFileName);
		if (!flag) LOG(_FL_, 0, 0, "[sFileName:%s]", sFileName);

		/* DAT 파일을 오픈한다. */
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}
		
		sMessageTemp[0] = 0x00;
		
		memcpy(sCurrentTime, TimeGetDateTime() + 8, 2);
		
		if (!flag) LOG(_FL_, 0, 0, "[현재시간:%s]", sCurrentTime);

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
		    	
		    	if (!flag) LOG(_FL_, 0, 0, "[파일읽기:%s]", strLine);
		    		
		    	/* 
                 *  등록시간이 체크되어있는 건중 시간만 추출한다
                 *  한시간동안 5건 이상 발송 중지
                 */
			    if (strncmp(pDat->sWriteTm , "              ", 14) != 0)
			    {
			    	/* 등록시간 추출 */
			        memcpy(sTemp, pDat->sWriteTm + 8 , 2);
			        
			        if (!flag) LOG(_FL_, 0, 0, "[등록시간:%s]", sTemp);
			        
			        /* 동일시간일경우 */
			        if (strncmp(sCurrentTime , sTemp, 2) == 0 ) {
			        	
			        	if (strncmp(pDat->sSendStat, "_READ_", 6) == 0 ) { 
			                iTempCnt++; /* 전송건수 */
                        }
			            
			        	/* 중복 체크 */
		                if ( strncmp(pDat->sSendTm, "              ", 14) == 0 ) {
		        	        if (strncmp(sMessageTemp , pDat->sMessage, 160) == 0) {
			        		    iDupchk = 1; /* 중복 */
			        	    } else {
			        	    	memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
			        	    }
			        	    			        	    
		                } else {
		                	memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
		                }
		                
			        } else {
			        	iTempCnt = 1;  /* 새로운 시간 시작 */
                        
			            memcpy(sMessageTemp, pDat->sMessage , sizeof(sMessageTemp));
			            iDupchk = 0; /* 중복 아님 */
			        }
		        }
		        
		        if ( strncmp(pDat->sSendTm, "              ", 14) == 0 && iTempCnt > iMaxSendCnt ) {
		        	if (flag) LOG(_FL_, 0, 0, "(%s시) %d건 > 기준 %d건 => SKIP, MSG_READ[%s]"
		        		          , sCurrentTime, iTempCnt, iMaxSendCnt, pDat->sMessage);
		        		
		        	memcpy(pDat->sSendTm, TimeGetDateTime(), 14);
                    memcpy(pDat->sSendStat, "_PASS_", 6);
			    	pDat->sLF[0] = 0x00;
                
			    	fseek(fp, pos, SEEK_SET);
			    	fputs(strLine, fp);
                
			    	strcpy(sSendMsg, "PASS");
			    	break;
		        	 
		        }
		        
		        if (!flag) LOG(_FL_, 0, 0, "[중복체크]=%d", iDupchk);
		        	
		        if ( strncmp(pDat->sSendTm, "              ", 14) == 0 && iDupchk ) {
		        	if (flag) LOG(_FL_, 0, 0, "(%s시)중복 메시지 => SKIP, MSG_READ[%s]"
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
		    if (!flag) LOG(_FL_, 0, 0, "[  END]비교시작");
			
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

		/* 처리된 파일을 닫는다. */
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
 * 내용 :
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
		 * iconv를 이용한 변환
		 * UTF-8 <- EUC-KR 변환
		 */

		iconv_t cd = iconv_open("UTF-8", "EUC-KR");   /* to <- from */
		if (cd == (iconv_t)(-1))
		{
			LOG(_FL_, 0, 0, "ERROR: iconv_open error..");
			if (flag) exit(-1);
			return NULL;            /* TODO DATE.2013.11.07 */
		}

		/* 작업저장공간을 만들고 클리어 한다. */
		char inBuf[MAX_LEN_TRAN];
		char outBuf[MAX_LEN_TRAN];
		memset(inBuf, 0x00, MAX_LEN_TRAN);
		memset(outBuf, 0x00, MAX_LEN_TRAN);

		/* 변환 소스를 복사한다. */
		strcpy(inBuf, str);

		/* 변환 처리를 위한 기초변수를 세팅한다. */
		char* in  = inBuf;
		char* out = outBuf;
		size_t inSize  = strlen(inBuf);
		size_t outSize = sizeof(outBuf);

		if (!flag) LOG(_FL_, 0, 0, "BEFORE [inSize:%d] [outSize:%d]", inSize, outSize);

		/* 변환 처리를 한다. */
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

			/* 변환 처리를 한 값을 얻는다. */
			sprintf(str, "%-*.*s", strLen, strLen, outBuf);
			
			if (!flag) LOG(_FL_, 0, 0, "[%d] [%d:%s]", strLen, strlen(str), str);
		}

		iconv_close(cd);
	}
	
	return str;
}


/*============================================================================*/
/*
 * 내용 :
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
			/* socket을 생성한다. */
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
			/* address 정보를 세팅한다. */
			struct sockaddr_in sAddr;

			memset(&sAddr, 0x00, sizeof(sAddr));
			sAddr.sin_family      = AF_INET;
			sAddr.sin_addr.s_addr = inet_addr(sHostIp);
			sAddr.sin_port        = htons(iHostPort);

			/*
			 * 소켓을 해당 주소로 연결
			 * 접속이 않되면 재접속을 시도한다.
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
		
		if (flag) LOG(_FL_, 0, 0, "수신인 [%d]명", iRecverCnt);

		if (flag)
		{
			/*
			 * RECVER에 메시지를 송신하고 결과를 받는 처리를 반복한다.
			 * 리턴 값
			 *     "00" : 저장성공
			 *     "10" : 전문데이터 오류
			 *     "99" : 시스템 오류
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
				
					
				/* 출력메시지메시지를 만든다. */
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

				/* SMS 송신 메시지를 만든다. */
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
					/* SMS 송신 메시지를 확인한다. */
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
					/* SMS 송신 메시지를 송신한다. */
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

					/* SMS 송신에 대한 결과응답을 받는다. */
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
			/* socket를 close 한다. */

			close(sock);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : SMS 메시지를 전송한다.
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

		/* UmsRead 시작 */
		char sendMsg[MAX_LINE_SIZE];

		if (UmsIsChange() > 0)
		{
			UmsGetCfg();

			while (1)
			{
				if (UmsRead((char*)&sendMsg) == NULL)
					break;

				if (flag) UmsSend((char*)&sendMsg); /* Ums 전송 */

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
 * 위에서 정의한 함수를 테스트 한다.
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

		/* UmsIsChange 확인 테스트 */
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
			printf("\nUSAGE : %s [ w (메시지) | r (..) ]\n\n", argv[0]);
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

			/* UmsWrite 시작 */

			char* pMsg = argv[2];
			/* char* pMsg = "[pfep03][UMS0120000] 테스트용 메시지입니다."; */
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

			/* UmsRead 시작 */
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
