/*
 * PROGRAM ID   : Cfg.c
 * PROGRAM NAME : 환경파일 처리 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : 환경파일 처리를 다루는 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 * TODO DATE.20130726 : FEPID에 해당하는 모든 항목을 출력하도록 하는 함수를 만든다.
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   USER_NAME          "ibridge"

#define   MAX_LINE_SIZE      1024
#define   MAX_ITEM             20
#define   FEPID_SIZE            5
#define   SERVICE_SIZE         10
#define   LOG_LEVEL_DEFAULT  "DEBUG"

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

CFG_MST      stCfgMst           [MAX_CFG_MST];   /* Master 정보 저장 */
int          iMaxCfgMst                      ;


CFG_BIZ      stCfgBiz           [MAX_CFG_BIZ];   /* biz.properties 정보 저장 */
int          iMaxCfgBiz                      ;

CFG_OFA      stCfgOfa           [MAX_CFG_OFA];   /* ofacom0000.cfg 정보 저장 */
int          iMaxCfgOfa                      ;


CFG_OFS      stCfgOfs           [MAX_CFG_OFS];   /* ofscom0000.cfg 정보 저장 */
int          iMaxCfgOfs                      ;

CFG_OIR      stCfgOir           [MAX_CFG_OIR];   /* oircom0000.cfg 정보 저장 */
int          iMaxCfgOir                      ;

CFG_OXALINE  stCfgOxaLine   [MAX_CFG_OXALINE];   /* oxa_line.cfg 정보 저장 */
int          iMaxCfgOxaLine                  ;

CFG_BXA      stCfgBxa           [MAX_CFG_BXA];   /* bxacom0000.cfg 정보 저장 */
int          iMaxCfgBxa                      ;

CFG_OIS      stCfgOis           [MAX_CFG_OIS];   /* oiscom0000.cfg 정보 저장 */
int          iMaxCfgOis                      ;

CFG_OFR      stCfgOfr           [MAX_CFG_OFR];   /* ofrcom0000.cfg 정보 저장 */
int          iMaxCfgOfr                      ;


CFG_BXATR    stCfgBxaTr       [MAX_CFG_BXATR];   /* bxa_trlst.cfg 정보 저장 */
int          iMaxCfgBxaTr                    ;


CFG_OXA      stCfgOxa           [MAX_CFG_OXA];   /* oxaline2.cfg 정보 저장  */
int          iMaxCfgOxa                      ;

CFG_OXAPOLL  stCfgOxaPoll   [MAX_CFG_OXAPOLL];   /* oxapoll2.cfg 정보 저장  */
int          iMaxCfgOxaPoll                  ;



CFG_OFR2     stCfgOfr2         [MAX_CFG_OFR2];   /* ofrcom2.cfg 정보 저장 */
int          iMaxCfgOfr2                     ;

CFG_OFS2     stCfgOfs2         [MAX_CFG_OFS2];   /* ofscom2.cfg 정보 저장 */
int          iMaxCfgOfs2                     ;

CFG_OIR2     stCfgOir2         [MAX_CFG_OIR2];   /* oircom2.cfg 정보 저장 */
int          iMaxCfgOir2                     ;

CFG_OIS2     stCfgOis2         [MAX_CFG_OIS2];   /* oiscom2.cfg 정보 저장 */
int          iMaxCfgOis2                     ;

CFG_POLL     stCfgPoll         [MAX_CFG_POLL];   /* oiapoll2.cfg 정보 저장  */
int          iMaxCfgPoll                     ;

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/
char  sHomePath          [128];
char  sPath              [MAX_LINE_SIZE];

char* str_aia_host       = "/hw01/ibridge/ib2/cfg/aia_host.cfg";
char* str_baa_trlist     = "/hw01/ibridge/ib2/cfg/baa_trlist.cfg";
char* str_biz_properties = "/hw01/ibridge/ib2/cfg/biz.properties";
char* str_biz_service    = "/hw01/ibridge/ib2/cfg/biz.service";
char* str_bxacom0000     = "/hw01/ibridge/ib2/cfg/bxacom0000.cfg";
char* str_ofacom0000     = "/hw01/ibridge/ib2/cfg/ofacom0000.cfg";
char* str_ofrcom0000     = "/hw01/ibridge/ib2/cfg/ofrcom0000.cfg";
char* str_ofscom0000     = "/hw01/ibridge/ib2/cfg/ofscom0000.cfg";
char* str_oia_msg        = "/hw01/ibridge/ib2/cfg/oia_msg.cfg";
char* str_oiacom0000     = "/hw01/ibridge/ib2/cfg/oiacom0000.cfg";
char* str_oircom0000     = "/hw01/ibridge/ib2/cfg/oircom0000.cfg";
char* str_oiscom0000     = "/hw01/ibridge/ib2/cfg/oiscom0000.cfg";
char* str_oxa_access     = "/hw01/ibridge/ib2/cfg/oxa_access.cfg";
char* str_oxa_conv       = "/hw01/ibridge/ib2/cfg/oxa_conv.cfg";
char* str_oxa_fdrw       = "/hw01/ibridge/ib2/cfg/oxa_fdrw.cfg";
char* str_oxa_line       = "/hw01/ibridge/ib2/cfg/oxa_line.cfg";
char* str_oxa_poll       = "/hw01/ibridge/ib2/cfg/oxa_poll.cfg";
char* str_oxa_reer       = "/hw01/ibridge/ib2/cfg/oxa_reer.cfg";
char* str_oxa_rout       = "/hw01/ibridge/ib2/cfg/oxa_rout.cfg";
char* str_sms_sender     = "/hw01/ibridge/ib2/cfg/sms_sender.cfg";

/*============================================================================*/
/*
 * 내용 : HOME/ib2/cfg/master.cfg : 마스터 환경파일로 다른 환경파일에 대한 정의를 한다.
 */
/*----------------------------------------------------------------------------*/
int CfgMst()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * HomePath 를 얻는다. [계정]/ib 폴더
		 */
		char* userName = USER_NAME;
		struct passwd* pPasswd = getpwnam(userName);
		if (pPasswd == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : not exist user...[%s]", userName);
			exit(-1);
		}

		if (!flag)
		{
			LOG(_FL_, 0, 0, "pw_name   = [%s]", pPasswd->pw_name  );
			LOG(_FL_, 0, 0, "pw_passwd = [%s]", pPasswd->pw_passwd);
			LOG(_FL_, 0, 0, "pw_uid    = [%d]", pPasswd->pw_uid   );
			LOG(_FL_, 0, 0, "pw_gid    = [%d]", pPasswd->pw_gid   );
			LOG(_FL_, 0, 0, "pw_gecos  = [%s]", pPasswd->pw_gecos );
			LOG(_FL_, 0, 0, "pw_dir    = [%s]", pPasswd->pw_dir   );
			LOG(_FL_, 0, 0, "pw_shell  = [%s]", pPasswd->pw_shell );
		}

		strcpy(sHomePath, pPasswd->pw_dir);
	}

	if (flag)
	{
		FILE* fp;
		char strLine[MAX_LINE_SIZE];
		char* arrItem[MAX_ITEM];

		/* 처리할 파일을 오픈한다. */
		sprintf(sPath, "%s/ib2/cfg/Master.cfg", sHomePath);
		fp = fopen(sPath, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, sPath);
			return -1;
		}

		iMaxCfgMst = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

			/* 한라인을 구분자로 쪼갠다. */
			UtilStrSplit(strLine, '=', arrItem);

			if (flag)
			{
				strcpy(stCfgMst[iMaxCfgMst].sKey, UtilTrim(arrItem[0]));
				strcpy(stCfgMst[iMaxCfgMst].sVal, UtilTrim(arrItem[1]));
				iMaxCfgMst ++;
			}

			if (!flag)
			{
				LOG(_FL_, 0, 0, "%s:%s:%d> [%s] [%s]=[%s]"
					, __FILE__, __FUNCTION__, __LINE__
					, sServiceid, UtilTrim(arrItem[0]), UtilTrim(arrItem[1]));
			}

			if (!flag)
			{
				int i;
				for (i=0; i < MAX_ITEM && arrItem[i]; i++)
				{
					LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (!flag)
		{
			int i;
			for (i=0; i < iMaxCfgMst; i++)
			{
				LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
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
char* CfgGetMst(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgMst; i++)
		{
			if (!flag)LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
			if (strcmp(stCfgMst[i].sKey, sKey) == 0)
			{
				return stCfgMst[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : HOME/ib2/cfg/master.cfg : 마스터 환경파일로 다른 환경파일에 대한 정의를 한다.
 */
/*----------------------------------------------------------------------------*/
int CfgMaster()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		CfgMst();

		return CfgBiz();
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetMaster(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		return CfgGetMst(sKey);
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : biz.properties : 그룹정보
 */
/*----------------------------------------------------------------------------*/
int CfgBiz()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.biz") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgBiz = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

			if (*strLine == '[')
			{
				if (strncmp(sServiceid, strLine+1, SERVICE_SIZE) == 0)
					swFlag = 1;
				else
					swFlag = 0;

				continue;
			}

			if (swFlag == 1)
			{
				if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, strLine);

				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '=', arrItem);

				if (flag)
				{
					strcpy(stCfgBiz[iMaxCfgBiz].sKey, UtilTrim(arrItem[0]));
					strcpy(stCfgBiz[iMaxCfgBiz].sVal, UtilTrim(arrItem[1]));
					iMaxCfgBiz ++;
				}

				if (!flag)
				{
					for (i=0; i < MAX_ITEM && arrItem[i]; i++)
					{
						if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
					}

				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (iMaxCfgBiz == 0)
			return -1;

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgBiz; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgBiz[i].sKey, stCfgBiz[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetBiz(%-17.17s) = [%s]\n"
					, i, stCfgBiz[i].sKey, stCfgBiz[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetBiz(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgBiz; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgBiz[i].sKey, stCfgBiz[i].sVal);
			if (strcmp(stCfgBiz[i].sKey, sKey) == 0)
			{
				return stCfgBiz[i].sVal;
			}
		}
	}

	/* DATE.2014.05.07 : (JUN) 로그레벨 추가에 따른 기본값 셋팅 */
	if (strcmp("LOG_LEVEL", sKey) == 0)
	{
		return LOG_LEVEL_DEFAULT;
 	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : ofacom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOfa()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag)LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ofa") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, sPath);
			return -1;
		}

		iMaxCfgOfa = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

			/* 한라인을 구분자로 쪼갠다. */
			UtilStrSplit(strLine, '=', arrItem);

			if (flag)
			{
				strcpy(stCfgOfa[iMaxCfgOfa].sKey, UtilTrim(arrItem[0]));
				strcpy(stCfgOfa[iMaxCfgOfa].sVal, UtilTrim(arrItem[1]));
				iMaxCfgOfa ++;
			}

		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOfa; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOfa[i].sKey, stCfgOfa[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOfa(%-17.17s) = [%s]\n"
					, i, stCfgOfa[i].sKey, stCfgOfa[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOfa(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOfa; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOfa[i].sKey, stCfgOfa[i].sVal);
			if (strcmp(stCfgOfa[i].sKey, sKey) == 0)
			{
				return stCfgOfa[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : ofscom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOfs()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ofs") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOfs = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOfs[0].sKey, "SERVICE");
					strcpy(stCfgOfs[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOfs[1].sKey, "PORT");
					strcpy(stCfgOfs[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOfs[2].sKey, "BEG_TIME");
					strcpy(stCfgOfs[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOfs[3].sKey, "END_TIME");
					strcpy(stCfgOfs[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOfs[4].sKey, "COMMENT");
					strcpy(stCfgOfs[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOfs = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOfs; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOfs[i].sKey, stCfgOfs[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOfs(%-17.17s) = [%s]\n"
					, i, stCfgOfs[i].sKey, stCfgOfs[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOfs(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOfs; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOfs[i].sKey, stCfgOfs[i].sVal);
			if (strcmp(stCfgOfs[i].sKey, sKey) == 0)
			{
				return stCfgOfs[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oircom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOir()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oir") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOir = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOir[0].sKey, "SERVICE");
					strcpy(stCfgOir[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOir[1].sKey, "PORT");
					strcpy(stCfgOir[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOir[2].sKey, "BEG_TIME");
					strcpy(stCfgOir[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOir[3].sKey, "END_TIME");
					strcpy(stCfgOir[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOir[4].sKey, "COMMENT");
					strcpy(stCfgOir[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOir = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다.*/
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOir; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOir[i].sKey, stCfgOir[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOir(%-17.17s) = [%s]\n"
					, i, stCfgOir[i].sKey, stCfgOir[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOir(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOir; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOir[i].sKey, stCfgOir[i].sVal);
			if (strcmp(stCfgOir[i].sKey, sKey) == 0)
			{
				return stCfgOir[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : bxacom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgBxa()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.bxa") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, sPath);
			return -1;
		}

		iMaxCfgBxa = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다.*/
				UtilStrSplit(strLine, '|', arrItem);

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgBxa[iMaxCfgBxa].sKey, UtilTrim(arrItem[1]));
					strcpy(stCfgBxa[iMaxCfgBxa].sVal, UtilTrim(arrItem[2]));
					iMaxCfgBxa ++;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgBxa; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgBxa[i].sKey, stCfgBxa[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetBxa(%-17.17s) = [%s]\n"
					, i, stCfgBxa[i].sKey, stCfgBxa[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetBxa(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgBxa; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgBxa[i].sKey, stCfgBxa[i].sVal);
			if (strcmp(stCfgBxa[i].sKey, sKey) == 0)
			{
				return stCfgBxa[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oiscom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOis()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ois") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOis = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOis[0].sKey, "SERVICE");
					strcpy(stCfgOis[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOis[1].sKey, "PORT");
					strcpy(stCfgOis[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOis[2].sKey, "STAGING");
					strcpy(stCfgOis[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOis[3].sKey, "SEND_TYPE");
					strcpy(stCfgOis[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOis[4].sKey, "SEND_RES");
					strcpy(stCfgOis[4].sVal, UtilTrim(arrItem[4]));

					strcpy(stCfgOis[5].sKey, "BEG_TIME");
					strcpy(stCfgOis[5].sVal, UtilTrim(arrItem[5]));

					strcpy(stCfgOis[6].sKey, "END_TIME");
					strcpy(stCfgOis[6].sVal, UtilTrim(arrItem[6]));

					strcpy(stCfgOis[7].sKey, "COMMENT");
					strcpy(stCfgOis[7].sVal, UtilTrim(arrItem[7]));

					iMaxCfgOis = 8;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOis; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOis[i].sKey, stCfgOis[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOis(%-17.17s) = [%s]\n"
					, i, stCfgOis[i].sKey, stCfgOis[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOis(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOis; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOis[i].sKey, stCfgOis[i].sVal);
			if (strcmp(stCfgOis[i].sKey, sKey) == 0)
			{
				return stCfgOis[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : ofrcom0000.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOfr()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ofr") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOfr = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOfr[0].sKey, "SERVICE");
					strcpy(stCfgOfr[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOfr[1].sKey, "PORT");
					strcpy(stCfgOfr[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOfr[2].sKey, "BEG_TIME");
					strcpy(stCfgOfr[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOfr[3].sKey, "END_TIME");
					strcpy(stCfgOfr[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOfr[4].sKey, "COMMENT");
					strcpy(stCfgOfr[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOfr = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOfr; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOfr[i].sKey, stCfgOfr[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOfr(%-17.17s) = [%s]\n"
					, i, stCfgOfr[i].sKey, stCfgOfr[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOfr(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOfr; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOfr[i].sKey, stCfgOfr[i].sVal);
			if (strcmp(stCfgOfr[i].sKey, sKey) == 0)
			{
				return stCfgOfr[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oxa_line.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOxaLine()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oxaline") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOxaLine = 0;

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

				if (!flag) LOG(_FL_, 0, 0, ": debug [%s]", strLine);

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					if (!flag) LOG(_FL_, 0, 0, ": debug [%s] found ", UtilTrim(arrItem[ 0]));

					strcpy(stCfgOxaLine[ 0].sKey, "SERVICE");
					strcpy(stCfgOxaLine[ 0].sVal, UtilTrim(arrItem[ 0]));

					strcpy(stCfgOxaLine[ 1].sKey, "CLISVR");
					strcpy(stCfgOxaLine[ 1].sVal, UtilTrim(arrItem[ 1]));

					strcpy(stCfgOxaLine[ 2].sKey, "HOSTIP");
					strcpy(stCfgOxaLine[ 2].sVal, UtilTrim(arrItem[ 2]));

					strcpy(stCfgOxaLine[ 3].sKey, "HOSTPORT");
					strcpy(stCfgOxaLine[ 3].sVal, UtilTrim(arrItem[ 3]));

					strcpy(stCfgOxaLine[ 4].sKey, "CLOSETYPE");
					strcpy(stCfgOxaLine[ 4].sVal, UtilTrim(arrItem[ 4]));

					strcpy(stCfgOxaLine[ 5].sKey, "LOOPTYPE");
					strcpy(stCfgOxaLine[ 5].sVal, UtilTrim(arrItem[ 5]));

					strcpy(stCfgOxaLine[ 6].sKey, "LENTYPE");
					strcpy(stCfgOxaLine[ 6].sVal, UtilTrim(arrItem[ 6]));

					strcpy(stCfgOxaLine[ 7].sKey, "LENOFFSET");
					strcpy(stCfgOxaLine[ 7].sVal, UtilTrim(arrItem[ 7]));

					strcpy(stCfgOxaLine[ 8].sKey, "LENLENGTH");
					strcpy(stCfgOxaLine[ 8].sVal, UtilTrim(arrItem[ 8]));

					strcpy(stCfgOxaLine[ 9].sKey, "LENMINUS");
					strcpy(stCfgOxaLine[ 9].sVal, UtilTrim(arrItem[ 9]));

					strcpy(stCfgOxaLine[10].sKey, "TRTYPE");
					strcpy(stCfgOxaLine[10].sVal, UtilTrim(arrItem[10]));

					strcpy(stCfgOxaLine[11].sKey, "TIMEOUT");
					strcpy(stCfgOxaLine[11].sVal, UtilTrim(arrItem[11]));

					strcpy(stCfgOxaLine[12].sKey, "SESSCOUNT");
					strcpy(stCfgOxaLine[12].sVal, UtilTrim(arrItem[12]));

					strcpy(stCfgOxaLine[13].sKey, "MAXSESSCOUNT");
					strcpy(stCfgOxaLine[13].sVal, UtilTrim(arrItem[13]));

					strcpy(stCfgOxaLine[14].sKey, "TRSAVETYPE");
					strcpy(stCfgOxaLine[14].sVal, UtilTrim(arrItem[14]));

					strcpy(stCfgOxaLine[15].sKey, "COMMENT");
					strcpy(stCfgOxaLine[15].sVal, UtilTrim(arrItem[15]));

					iMaxCfgOxaLine = 16;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOxaLine; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOxaLine[i].sKey, stCfgOxaLine[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOxaLine(%-17.17s) = [%s]\n"
					, i, stCfgOxaLine[i].sKey, stCfgOxaLine[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOxaLine(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOxaLine; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOxaLine[i].sKey, stCfgOxaLine[i].sVal);
			if (strcmp(stCfgOxaLine[i].sKey, sKey) == 0)
			{
				return stCfgOxaLine[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : bxa_trlst.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgBxaTr(char* sSndRcv, char* sTrCode)
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
		char keyTrCode[20];

		if (strlen(sTrCode) != 11)
		{
			/*
			 * sTrCode가 11 bytes가 아니면 나머지는 0으로 채운다.
			 */
			UtilFillCopy(keyTrCode, UtilTrim(sTrCode), 11, '0');
		}
		else
		{
			strcpy(keyTrCode, sTrCode);
		}

		if (flag)
		{
			/*
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.bxatrlst") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgBxaTr = 0;

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
				if (!flag) LOG(_FL_, 0, 0, ": debug [%s]", strLine);

				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arrItem);

				if (!flag) LOG(_FL_, 0, 0, ": debug [%s][%s][%s]", arrItem[0], arrItem[3], arrItem[5]);

				if (
					strcmp(sServiceid  , UtilTrim(arrItem[0])) == 0
					&& strcmp(sSndRcv  , UtilTrim(arrItem[3])) == 0
					&& strcmp(keyTrCode, UtilTrim(arrItem[5])) == 0
				)
				{
					if (!flag) LOG(_FL_, 0, 0, ": debug [%s] found ", UtilTrim(arrItem[ 0]));

					strcpy(stCfgBxaTr[ 0].sKey, "SERVICE");
					strcpy(stCfgBxaTr[ 0].sVal, UtilTrim(arrItem[ 0]));

					strcpy(stCfgBxaTr[ 1].sKey, "FEPID");
					strcpy(stCfgBxaTr[ 1].sVal, UtilTrim(arrItem[ 1]));

					strcpy(stCfgBxaTr[ 2].sKey, "ORGCD");
					strcpy(stCfgBxaTr[ 2].sVal, UtilTrim(arrItem[ 2]));

					strcpy(stCfgBxaTr[ 3].sKey, "SNDRCV");
					strcpy(stCfgBxaTr[ 3].sVal, UtilTrim(arrItem[ 3]));

					strcpy(stCfgBxaTr[ 4].sKey, "FILEID");
					strcpy(stCfgBxaTr[ 4].sVal, UtilTrim(arrItem[ 4]));

					strcpy(stCfgBxaTr[ 5].sKey, "TRCODE");
					strcpy(stCfgBxaTr[ 5].sVal, UtilTrim(arrItem[ 5]));

					strcpy(stCfgBxaTr[ 6].sKey, "TRCDLEN");
					strcpy(stCfgBxaTr[ 6].sVal, UtilTrim(arrItem[ 6]));

					strcpy(stCfgBxaTr[ 7].sKey, "RECLEN");
					strcpy(stCfgBxaTr[ 7].sVal, UtilTrim(arrItem[ 7]));

					strcpy(stCfgBxaTr[ 8].sKey, "COMMENT");
					strcpy(stCfgBxaTr[ 8].sVal, UtilTrim(arrItem[ 8]));

					iMaxCfgBxaTr = 9;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgBxaTr; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgBxaTr[i].sKey, stCfgBxaTr[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetBxaTr(%-17.17s) = [%s]\n"
					, i, stCfgBxaTr[i].sKey, stCfgBxaTr[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetBxaTr(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgBxaTr; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgBxaTr[i].sKey, stCfgBxaTr[i].sVal);
			if (strcmp(stCfgBxaTr[i].sKey, sKey) == 0)
			{
				return stCfgBxaTr[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oxaline2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOxa()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oxaline2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOxa = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, ": strLine [%s]", strLine);

			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arrItem);

				if (!flag) LOG(_FL_, 0, 0, ": [%s] [%s]", sServiceid, UtilTrim(arrItem[0]));

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					/*
					  OXASHC0101|C|10|58.150.242.242 :50246(1),50247(1)|4|0|신한카드 온라인TR 송수신
					  OXATAT0101|C|10|192.168.167.112:22104(1)         |4|4|씽크에이티 2채널인증 송수신
					*/
					strcpy(stCfgOxa[0].sKey, "SERVICEID" );strcpy(stCfgOxa[0].sVal, UtilTrim(arrItem[0]));
					strcpy(stCfgOxa[1].sKey, "CONN_TYPE" );strcpy(stCfgOxa[1].sVal, UtilTrim(arrItem[1]));
					strcpy(stCfgOxa[2].sKey, "CONN_TRY"  );strcpy(stCfgOxa[2].sVal, UtilTrim(arrItem[2]));
					strcpy(stCfgOxa[3].sKey, "CONN_INFO" );strcpy(stCfgOxa[3].sVal, UtilTrim(arrItem[3]));
					strcpy(stCfgOxa[4].sKey, "LENGTH_LEN");strcpy(stCfgOxa[4].sVal, UtilTrim(arrItem[4]));
					strcpy(stCfgOxa[5].sKey, "INC_LEN"   );strcpy(stCfgOxa[5].sVal, UtilTrim(arrItem[5]));
					strcpy(stCfgOxa[6].sKey, "TIME_OUT"  );strcpy(stCfgOxa[6].sVal, UtilTrim(arrItem[6]));
					strcpy(stCfgOxa[7].sKey, "POLL_TIME" );strcpy(stCfgOxa[7].sVal, UtilTrim(arrItem[7]));
					strcpy(stCfgOxa[8].sKey, "SVC_DESC"  );strcpy(stCfgOxa[8].sVal, UtilTrim(arrItem[8]));

					iMaxCfgOxa = 9;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOxa; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOxa[i].sKey, stCfgOxa[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOxa(%-17.17s) = [%s]\n"
					, i, stCfgOxa[i].sKey, stCfgOxa[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOxa(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOxa; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOxa[i].sKey, stCfgOxa[i].sVal);
			if (strcmp(stCfgOxa[i].sKey, sKey) == 0)
			{
				return stCfgOxa[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oxapoll2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOxaPoll()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oxapoll2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOxaPoll = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, ": strLine [%s]", strLine);

			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arrItem);

				if (!flag) LOG(_FL_, 0, 0, ": [%s] [%s]", sServiceid, UtilTrim(arrItem[0]));

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0 && strcmp("Y", UtilTrim(arrItem[1])) == 0)
				{
					/*
					  OXASHC0101 | Y | SEND_REQ | 60 : M,[0020HDRREQPOLL<MMDDHHMMSS>]         # 요청 송신
					  OXASHC0101 | Y | RECV_RES | 60 : T,0,14,0020HDRRESPOLL                  # 응답 수신
					  OXASHC0101 | Y | RECV_REQ | 60 : T,0,14,0020HDRREQPOLL                  # 요청 수신
					  OXASHC0101 | Y | SEND_RES | 60 : M,[0020HDRRESPOLL<MMDDHHMMSS>]         # 응답 송신
					*/
					strcpy(stCfgOxaPoll[iMaxCfgOxaPoll].sKey, UtilTrim(arrItem[2]));
					strcpy(stCfgOxaPoll[iMaxCfgOxaPoll].sVal, UtilTrim(arrItem[3]));

					iMaxCfgOxaPoll ++;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOxaPoll; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOxaPoll[i].sKey, stCfgOxaPoll[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOxaPoll(%-17.17s) = [%s]\n"
					, i, stCfgOxaPoll[i].sKey, stCfgOxaPoll[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOxaPoll(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOxaPoll; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOxaPoll[i].sKey, stCfgOxaPoll[i].sVal);
			if (strcmp(stCfgOxaPoll[i].sKey, sKey) == 0)
			{
				return stCfgOxaPoll[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : ofrcom2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOfr2()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ofr2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOfr2 = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOfr2[0].sKey, "SERVICE");
					strcpy(stCfgOfr2[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOfr2[1].sKey, "SVR_INFO");
					strcpy(stCfgOfr2[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOfr2[2].sKey, "BEG_TIME");
					strcpy(stCfgOfr2[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOfr2[3].sKey, "END_TIME");
					strcpy(stCfgOfr2[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOfr2[4].sKey, "COMMENT");
					strcpy(stCfgOfr2[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOfr2 = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOfr2; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOfr2[i].sKey, stCfgOfr2[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOfr2(%-17.17s) = [%s]\n"
					, i, stCfgOfr2[i].sKey, stCfgOfr2[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOfr2(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOfr2; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOfr2[i].sKey, stCfgOfr2[i].sVal);
			if (strcmp(stCfgOfr2[i].sKey, sKey) == 0)
			{
				return stCfgOfr2[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : ofscom2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOfs2()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ofs2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOfs2 = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOfs2[0].sKey, "SERVICE");
					strcpy(stCfgOfs2[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOfs2[1].sKey, "SVR_INFO");
					strcpy(stCfgOfs2[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOfs2[2].sKey, "BEG_TIME");
					strcpy(stCfgOfs2[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOfs2[3].sKey, "END_TIME");
					strcpy(stCfgOfs2[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOfs2[4].sKey, "COMMENT");
					strcpy(stCfgOfs2[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOfs2 = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOfs2; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOfs2[i].sKey, stCfgOfs2[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOfs2(%-17.17s) = [%s]\n"
					, i, stCfgOfs2[i].sKey, stCfgOfs2[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOfs2(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOfs2; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOfs2[i].sKey, stCfgOfs2[i].sVal);
			if (strcmp(stCfgOfs2[i].sKey, sKey) == 0)
			{
				return stCfgOfs2[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oircom2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOir2()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oir2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOir2 = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOir2[0].sKey, "SERVICE");
					strcpy(stCfgOir2[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOir2[1].sKey, "SVR_INFO");
					strcpy(stCfgOir2[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOir2[2].sKey, "BEG_TIME");
					strcpy(stCfgOir2[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOir2[3].sKey, "END_TIME");
					strcpy(stCfgOir2[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOir2[4].sKey, "COMMENT");
					strcpy(stCfgOir2[4].sVal, UtilTrim(arrItem[4]));

					iMaxCfgOir2 = 5;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다.*/
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOir2; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOir2[i].sKey, stCfgOir2[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOir2(%-17.17s) = [%s]\n"
					, i, stCfgOir2[i].sKey, stCfgOir2[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOir2(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOir2; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOir2[i].sKey, stCfgOir2[i].sVal);
			if (strcmp(stCfgOir2[i].sKey, sKey) == 0)
			{
				return stCfgOir2[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oiscom2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgOis2()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.ois2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgOis2 = 0;

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

				if (strcmp(sServiceid, UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stCfgOis2[0].sKey, "SERVICE");
					strcpy(stCfgOis2[0].sVal, UtilTrim(arrItem[0]));

					strcpy(stCfgOis2[1].sKey, "SVR_INFO");
					strcpy(stCfgOis2[1].sVal, UtilTrim(arrItem[1]));

					strcpy(stCfgOis2[2].sKey, "STAGING");
					strcpy(stCfgOis2[2].sVal, UtilTrim(arrItem[2]));

					strcpy(stCfgOis2[3].sKey, "SEND_TYPE");
					strcpy(stCfgOis2[3].sVal, UtilTrim(arrItem[3]));

					strcpy(stCfgOis2[4].sKey, "SEND_RES");
					strcpy(stCfgOis2[4].sVal, UtilTrim(arrItem[4]));

					strcpy(stCfgOis2[5].sKey, "BEG_TIME");
					strcpy(stCfgOis2[5].sVal, UtilTrim(arrItem[5]));

					strcpy(stCfgOis2[6].sKey, "END_TIME");
					strcpy(stCfgOis2[6].sVal, UtilTrim(arrItem[6]));

					strcpy(stCfgOis2[7].sKey, "COMMENT");
					strcpy(stCfgOis2[7].sVal, UtilTrim(arrItem[7]));

					iMaxCfgOis2 = 8;

					break;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgOis2; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgOis2[i].sKey, stCfgOis2[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetOis2(%-17.17s) = [%s]\n"
					, i, stCfgOis2[i].sKey, stCfgOis2[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetOis2(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgOis2; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgOis2[i].sKey, stCfgOis2[i].sVal);
			if (strcmp(stCfgOis2[i].sKey, sKey) == 0)
			{
				return stCfgOis2[i].sVal;
			}
		}
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : oiapoll2.cfg
 */
/*----------------------------------------------------------------------------*/
int CfgPoll()
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
			 * 관련파일정보 찾기
			 */
			for (i=0; i < iMaxCfgMst; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgMst[i].sKey, stCfgMst[i].sVal);
				if (strcmp(stCfgMst[i].sKey, "ib.cfg.file.oiapoll2") == 0)
				{
					break;
				}
			}

			if (i == iMaxCfgMst)
				return -1;
		}

		/* 처리할 파일을 오픈한다. */
		fp = fopen(stCfgMst[i].sVal, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, stCfgMst[i].sVal);
			return -1;
		}

		iMaxCfgPoll = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, ": strLine [%s]", strLine);

			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arrItem);

				if (!flag) LOG(_FL_, 0, 0, ": [%s] [%s]", sServiceid, UtilTrim(arrItem[0]));

				if (strcmp("DEFAULT", UtilTrim(arrItem[0])) == 0 && strcmp("Y", UtilTrim(arrItem[1])) == 0)
				{
					/*
					  OXASHC0101 | Y | SEND_REQ | 60 : M,[0020HDRREQPOLL<MMDDHHMMSS>]         # 요청 송신
					  OXASHC0101 | Y | RECV_RES | 60 : T,0,14,0020HDRRESPOLL                  # 응답 수신
					  OXASHC0101 | Y | RECV_REQ | 60 : T,0,14,0020HDRREQPOLL                  # 요청 수신
					  OXASHC0101 | Y | SEND_RES | 60 : M,[0020HDRRESPOLL<MMDDHHMMSS>]         # 응답 송신
					*/
					strcpy(stCfgPoll[iMaxCfgPoll].sKey, UtilTrim(arrItem[2]));
					strcpy(stCfgPoll[iMaxCfgPoll].sVal, UtilTrim(arrItem[3]));

					iMaxCfgPoll ++;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (flag)
		{
			int i;
			for (i=0; i < iMaxCfgPoll; i++)
			{
				if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]"
					, i, stCfgPoll[i].sKey, stCfgPoll[i].sVal);

				if (flag) printf("\t CFG(%02d) CfgGetPoll(%-17.17s) = [%s]\n"
					, i, stCfgPoll[i].sKey, stCfgPoll[i].sVal);
			}

			if (flag) printf("\t ------------------------------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
char* CfgGetPoll(char* sKey)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int i;
		for (i=0; i < iMaxCfgPoll; i++)
		{
			if (!flag) LOG(_FL_, 0, 0, "%d) [KEY:%s]=[VAL:%s]", i, stCfgPoll[i].sKey, stCfgPoll[i].sVal);
			if (strcmp(stCfgPoll[i].sKey, sKey) == 0)
			{
				return stCfgPoll[i].sVal;
			}
		}
	}

	return NULL;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/*============================================================================*/
/*
 * 내용 : aia_host.cfg : 모든정보
 */
/*----------------------------------------------------------------------------*/
int CfgAiaHost_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;

	/* 처리할 파일을 오픈한다. */
	fp = fopen(str_aia_host, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_aia_host);
		return -1;
	}

	/* 한 라인씩 읽으며 처리를 한다.  */
	while (1)
	{
		/* strLine은 CR/LF를 포함한 문자열이다. */
		if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
			break;

		UtilTrim(UtilDelComment(strLine));
		if (strcmp(strLine, "") == 0)
			continue;

		if (flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

		/* 한라인을 구분자로 쪼갠다. */
		UtilStrSplit(strLine, '|', arrItem);
		for (i=0; i < MAX_ITEM && arrItem[i]; i++)
		{
			if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
		}

	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

	return 0;
}

/*============================================================================*/
/*
 * 내용 : biz.properties : 그룹정보
 */
/*----------------------------------------------------------------------------*/
int CfgBizProperties_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;
	int swFlag = 0;

	/* 처리할 파일을 오픈한다. */
	fp = fopen(str_biz_properties, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_biz_properties);
		return -1;
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

		if (!flag) LOG(_FL_, 0, 0, "%s> [%s] [%s]", __FUNCTION__, sServiceid, strLine);

		if (*strLine == '[')
		{
			if (strncmp(sServiceid, strLine+1, SERVICE_SIZE) == 0)
				swFlag = 1;
			else
				swFlag = 0;
		}

		if (swFlag == 1)
		{
			if (flag) LOG(_FL_, 0, 0, "%s> [%s]", __FUNCTION__, strLine);

			/*( 한라인을 구분자로 쪼갠다. */
			UtilStrSplit(strLine, '=', arrItem);
			for (i=0; i < MAX_ITEM && arrItem[i]; i++)
			{
				if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
			}

		}
	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

	return 0;
}

/*============================================================================*/
/*
 * 내용 : bxacom0000.cfg : 라인정보
 */
/*----------------------------------------------------------------------------*/
int CfgBxaCom0000_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;

	/* 처리할 파일을 오픈한다. */
	fp = fopen(str_bxacom0000, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_bxacom0000);
		return -1;
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

			if (strcmp(sServiceid, arrItem[0]) == 0)
			{
				for (i=0; i < MAX_ITEM && arrItem[i]; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
				}

			}
		}
	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

	return 0;
}

/*============================================================================*/
/*
 * 내용 : oxa_line.cfg : 라인정보
 */
/*----------------------------------------------------------------------------*/
int CfgOxaLine_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;

	/* 처리할 파일을 오픈한다. */
	fp = fopen(str_oxa_line, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_oxa_line);
		return -1;
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

			if (strcmp(sServiceid, arrItem[0]) == 0)
			{
				for (i=0; i < MAX_ITEM && arrItem[i]; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
				}

			}
		}
	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

	return 0;
}

/*============================================================================*/
/*
 * 내용 : oxa_rout.cfg : 라인정보
 */
/*----------------------------------------------------------------------------*/
int CfgOxaRout_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;

	/* 처리할 파일을 오픈한다. */
	fp = fopen(str_oxa_rout, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_oxa_rout);
		return -1;
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

			if (strcmp(sServiceid, arrItem[0]) == 0)
			{
				for (i=0; i < MAX_ITEM && arrItem[i]; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
				}

			}
		}
	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

	return 0;
}

/*============================================================================*/
/*
 * 내용 : oxa_poll.cfg : 라인정보
 */
/*----------------------------------------------------------------------------*/
int CfgOxaPoll_OLD()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	FILE* fp;
	char strLine[MAX_LINE_SIZE];
	char* arrItem[MAX_ITEM];
	int i;

	/*  처리할 파일을 오픈한다.*/
	fp = fopen(str_oxa_poll, "r+");
	if (fp == NULL)
	{
		LOG(_FL_, 0, 0, "ERROR [%s] : FILE 정보 open error...[%s]", __FILE__, str_oxa_poll);
		return -1;
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

			if (strcmp(sServiceid, arrItem[0]) == 0)
			{
				for (i=0; i < MAX_ITEM && arrItem[i]; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "%s> (%d) [%s]", __FUNCTION__, i, UtilTrim(arrItem[i]));
				}

			}
		}
	}

	/* 처리된 파일을 닫는다. */
	fclose(fp);

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

	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (!flag)
	{
		strcpy(sServiceid, "OIRKBB0101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgAiaHost_OLD();
	}

	if (!flag)
	{
		strcpy(sServiceid, "OIRKBB0101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgBizProperties_OLD();
	}

	if (!flag)
	{
		strcpy(sServiceid, "BXRSSN5101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgBxaCom0000_OLD();
	}

	if (!flag)
	{
		strcpy(sServiceid, "OXRSSN0101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgOxaLine_OLD();
	}

	if (!flag)
	{
		strcpy(sServiceid, "OXRSSN0101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgOxaRout_OLD();
	}

	if (!flag)
	{
		strcpy(sServiceid, "OXRSSN0101");
		if (flag)  LOG(_FL_, 0, 0, "[%s]", sServiceid);

		CfgOxaPoll_OLD();
	}

	if (!flag)
	{
		char* userName = "ibridge";
		struct passwd* pPasswd = getpwnam(userName);
		if (pPasswd == NULL)
		{
			 LOG(_FL_, 0, 0, "ERROR : not exist user...[%s]", userName);
			exit(-1);
		}

		if (flag)
		{
			LOG(_FL_, 0, 0, "pw_name   = [%s]", pPasswd->pw_name  );
			LOG(_FL_, 0, 0, "pw_passwd = [%s]", pPasswd->pw_passwd);
			LOG(_FL_, 0, 0, "pw_uid    = [%d]", pPasswd->pw_uid   );
			LOG(_FL_, 0, 0, "pw_gid    = [%d]", pPasswd->pw_gid   );
			LOG(_FL_, 0, 0, "pw_gecos  = [%s]", pPasswd->pw_gecos );
			LOG(_FL_, 0, 0, "pw_dir    = [%s]", pPasswd->pw_dir   );
			LOG(_FL_, 0, 0, "pw_shell  = [%s]", pPasswd->pw_shell );
		}
	}

	if (!flag)
	{
		strcpy(sServiceid, "BXSSHC5101");

		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		/* if (CfgMst() < 0) */
		if (CfgMaster() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		CfgBxaTr("S", "HW130000000");

		if (flag)
		{
			LOG(_FL_, 0, 0, "SERVICE = [%s]", CfgGetBxaTr("SERVICE"));
			LOG(_FL_, 0, 0, "FEPID   = [%s]", CfgGetBxaTr("FEPID"  ));
			LOG(_FL_, 0, 0, "ORGCD   = [%s]", CfgGetBxaTr("ORGCD"  ));
			LOG(_FL_, 0, 0, "SNDRCV  = [%s]", CfgGetBxaTr("SNDRCV" ));
			LOG(_FL_, 0, 0, "FILEID  = [%s]", CfgGetBxaTr("FILEID" ));
			LOG(_FL_, 0, 0, "TRCODE  = [%s]", CfgGetBxaTr("TRCODE" ));
			LOG(_FL_, 0, 0, "TRCDLEN = [%s]", CfgGetBxaTr("TRCDLEN"));
			LOG(_FL_, 0, 0, "RECLEN  = [%s]", CfgGetBxaTr("RECLEN" ));
			LOG(_FL_, 0, 0, "COMMENT = [%s]", CfgGetBxaTr("COMMENT"));
		}

		CfgBxaTr("S", "HW140000000");

		if (flag)
		{
			LOG(_FL_, 0, 0, "SERVICE = [%s]", CfgGetBxaTr("SERVICE"));
			LOG(_FL_, 0, 0, "FEPID   = [%s]", CfgGetBxaTr("FEPID"  ));
			LOG(_FL_, 0, 0, "ORGCD   = [%s]", CfgGetBxaTr("ORGCD"  ));
			LOG(_FL_, 0, 0, "SNDRCV  = [%s]", CfgGetBxaTr("SNDRCV" ));
			LOG(_FL_, 0, 0, "FILEID  = [%s]", CfgGetBxaTr("FILEID" ));
			LOG(_FL_, 0, 0, "TRCODE  = [%s]", CfgGetBxaTr("TRCODE" ));
			LOG(_FL_, 0, 0, "TRCDLEN = [%s]", CfgGetBxaTr("TRCDLEN"));
			LOG(_FL_, 0, 0, "RECLEN  = [%s]", CfgGetBxaTr("RECLEN" ));
			LOG(_FL_, 0, 0, "COMMENT = [%s]", CfgGetBxaTr("COMMENT"));
		}

		printf(" %d / %d = %d\n", 10, 3, (int) 10/3 );
	}

	if (flag)
	{
		if (flag) strcpy(sServiceid, "OXASHC0101");
		if (flag) strcpy(sServiceid, "OXATAT0101");

		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		/* if (CfgMst() < 0) */
		if (CfgMaster() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		CfgOxa();

		if (flag)
		{
			LOG(_FL_, 0, 0, "[SERVICEID ] 서비스ID          = [%s]", CfgGetOxa("SERVICEID" ));
			LOG(_FL_, 0, 0, "[CONN_TYPE ] 접속형태          = [%s]", CfgGetOxa("CONN_TYPE" ));
			LOG(_FL_, 0, 0, "[CONN_TRY  ] 접속시도          = [%s]", CfgGetOxa("CONN_TRY"  ));
			LOG(_FL_, 0, 0, "[CONN_INFO ] 접속서버정보      = [%s]", CfgGetOxa("CONN_INFO" ));
			LOG(_FL_, 0, 0, "[LENGTH_LEN] 길이필드크기      = [%s]", CfgGetOxa("LENGTH_LEN"));
			LOG(_FL_, 0, 0, "[INC_LEN   ] 길이필드포함크기  = [%s]", CfgGetOxa("INC_LEN"   ));
			LOG(_FL_, 0, 0, "[SVC_DESC  ] 서비스 설명       = [%s]", CfgGetOxa("SVC_DESC"  ));
		}
	}
}

#endif

