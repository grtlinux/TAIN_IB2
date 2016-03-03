/*
 * PROGRAM ID   : ps_man.c
 * PROGRAM NAME : ps_man 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : ps_man 모듈
 *                프로세스 관리
 *                biz.properties 서비스정의 파일을 읽어 관련 프로세스들을 통제한다.
 *
 * HISTORY      : 2013.09.17 강석. 최초작성
 *              : 2013.09.18 강석. 항목 추가. RUNNING_SERVER=AP/FEP, RUNNING_TIME=00:00~00:00
 *              : 2013.10.29 강석. shell 프로그램도 처리 가능토록 수정
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.10.29 강석. shell 프로그램도 처리 가능토록 수정"

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   MAX_LINE_SIZE         8192

#define   MAX_ITEM                10

#define   MAX_BIZ_PROP          1000
#define   MAX_PROC_INFO         1000
#define   SERVICE_SIZE            10

#define   PROC_DIR                 "/proc"
#define   HOME_DIR                 "/hw01/ibridge"
#define   EXE_FILE                 "ibrun"

/*============================================================================*/
/*
 * global variables
 *
 *
 *    typedef struct _RUN_TIME_
 *    {
 *        char sBeg          [ 10];  // 시작시간 (HHMM)
 *        char sFin          [ 10];  // 종료시간 (HHMM)
 *    } RUN_TIME;
 *
 *     * biz.properties 정보 구조체
 *
 *    typedef struct _BIZ_PROP_
 *    {
 *        char sSvcId        [ 20];  // 서비스 ID
 *        char sDesc         [128];  // 설명 Description
 *        char sObsFlag      [  5];  // 관심 플래그
 *        char sRunScpt      [128];  // 실행 스크립트
 *        char sRunSvr       [ 20];  // 실행 서버 (AP/FEP)
 *        char sRunTime      [128];  // 실행 시간 (0~HH:MM~HH:MM)
 *
 *        RUN_TIME  rt       [  7];  // 요일별RunTime 0(일요일) 6(토요일)
 *    } BIZ_PROP;
 *
 *     * proc 정보 구조체
 *
 *    typedef struct _PROC_INFO_
 *    {
 *        char sSvcId        [ 20];  // 서비스 ID
 *        char sProcess      [128];  // 실행프로세스
 *        char sPid          [ 20];  // PID 문자열
 *        int  iPid               ;  // PID
 *    } PROC_INFO;
 *
 *
 */
/*----------------------------------------------------------------------------*/

char*    sWeekNm[]                   = { "일","월","화","수","목","금","토" };

char     sHostName           [ 50+1] = { 0x00, };    /* hostname */
char     sHostNo             [ 50+1] = { 0x00, };    /* host number */
char     sRunSvr             [ 50+1] = { 0x00, };    /* 실행 서버 */

char     sCurHHMM            [ 10+1] = { 0x00, };    /* 현재 시분 */

int      iWeekNo                                ;    /* 요일번호 0:일요일 */

/*----------------------------------------------------------------------------*/

BIZ_PROP     stBizProp          [MAX_BIZ_PROP];     /* BizProp 정보 저장 */
int          iMaxBizProp                      ;

PROC_INFO    stProcInfo        [MAX_PROC_INFO];     /* proc 정보 저장 */
int          iMaxProcInfo                     ;


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
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
int FindProcPid(char* sSvcId)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int idx;

		for (idx=0; idx < iMaxProcInfo; idx++)
		{
			if (strcmp(sSvcId, stProcInfo[idx].sSvcId) == 0)
			{
				return idx;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int RunProcess(int idxBiz)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 9, 0, "\x1b[44m--------------- START [기동] (%s) ---------------\x1b[0m"
		, stBizProp[idxBiz].sSvcId);

	if (flag)
	{
		LOG(_FL_, 0, 0, "\t stBizProp[%d].sSvcId   = [%s]", idxBiz, stBizProp[idxBiz].sSvcId  );
		LOG(_FL_, 0, 0, "\t stBizProp[%d].sDesc    = [%s]", idxBiz, stBizProp[idxBiz].sDesc   );
		LOG(_FL_, 0, 0, "\t stBizProp[%d].sObsFlag = [%s]", idxBiz, stBizProp[idxBiz].sObsFlag);
		LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunScpt = [%s]", idxBiz, stBizProp[idxBiz].sRunScpt);
		LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunSvr  = [%s]", idxBiz, stBizProp[idxBiz].sRunSvr );
		/* LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunTime = [%s]", idxBiz, stBizProp[idxBiz].sRunTime); */
		LOG(_FL_, 0, 0, "\t stBizProp[%d].rt[%d] = [%s]~[%s]"
			, idxBiz, iWeekNo, stBizProp[idxBiz].rt[iWeekNo].sBeg, stBizProp[idxBiz].rt[iWeekNo].sFin);
	}

	if (flag)
	{
		if (flag)
		{
			/*
			 * fork job
			 */

			switch (fork())
			{
				case  0 :        /* child process */
					break;
				case -1 :
					LOG(_FL_, 0, 0, "ERROR : fork error...");
					return -1;
				default :        /* parent process */
					if (!flag) sleep(1);
					if (flag) usleep(100000);   /* microseconds */
					if (flag) LOG(_FL_, 9, 0, "\x1b[44m--------------- END   [기동] (%s) ---------------\x1b[0m"
						, stBizProp[idxBiz].sSvcId);
					return 0;
			}
		}

		if (!flag)
		{
			/*
			 * child process doing the execl job.
			 *    if (execl(arrItem[0], arrItem[1], NULL) < 0)
			 *    if (execl("/hw01/ibridge/shl/xrun", "OIRSCB0101", NULL) < 0)
			 *    if (execl("/hw01/ibridge/shl/xrun", "xrun", "OIRSCB0101", NULL) < 0)
			 *    if (execl("/hw01/ibridge/ib2/bin/ibrun", "ibrun", "OIRSCB0101", NULL) < 0)
			 *
			 */
			char* arrItem[MAX_ITEM];
			UtilStrSplit(stBizProp[idxBiz].sRunScpt, ' ', arrItem);

			LOG(_FL_, 0, 0, "RUNNING : process [%s] [%s]", arrItem[0], arrItem[1]);
			if (execl(arrItem[0], EXE_FILE, arrItem[1], NULL) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR (%d): execl('%s', '%s')..", errno, arrItem[0], arrItem[1]);
				exit(-1);
			}

			if (flag) exit(0);
		}

		if (flag)
		{
			/*
			 * DATE.2013.10.29 : execl -> execv 로 변경함 (이유: 이게 편함)
			 *
			 */
			char buf[128];
			char* argv[50];
			strcpy(buf, stBizProp[idxBiz].sRunScpt);
			UtilGetArgs(buf, argv);
			
			if (execv(argv[0], argv) < 0)
			{
				if (flag) LOG(_FL_, 9, 0, "ERROR (%d) : execv('%s')...", errno, stBizProp[idxBiz].sRunScpt);
				exit(-1);
			}

			if (flag) exit(0);
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int KillProcess(int idxPid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 9, 0, "\x1b[41m--------------- START [종료] (%s) ---------------\x1b[0m"
		, stProcInfo[idxPid].sSvcId);

	if (flag)
	{
		LOG(_FL_, 0, 0, "\t stProcInfo[%d].sSvcId   = [%s]", idxPid, stProcInfo[idxPid].sSvcId  );
		LOG(_FL_, 0, 0, "\t stProcInfo[%d].sProcess = [%s]", idxPid, stProcInfo[idxPid].sProcess);
		LOG(_FL_, 0, 0, "\t stProcInfo[%d].sPid     = [%s]", idxPid, stProcInfo[idxPid].sPid    );
		LOG(_FL_, 0, 0, "\t stProcInfo[%d].iPid     = [%d]", idxPid, stProcInfo[idxPid].iPid    );
	}

	if (flag)
	{
		/*
		 * child process에 signal을 보낸다. 즉, 종료시킨다.
		 *      SIGTERM (15)
		 */
		if (!flag) kill(stProcInfo[idxPid].iPid, SIGTERM);
		if (flag) kill(stProcInfo[idxPid].iPid, SIGKILL);

		if (flag) LOG(_FL_, 9, 0, "INFO : kill(%d, SIGTERM)....", stProcInfo[idxPid].iPid);
	}

	if (flag) LOG(_FL_, 9, 0, "\x1b[41m--------------- END   [종료] (%s) ---------------\x1b[0m"
		, stProcInfo[idxPid].sSvcId);

	return -1;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : child signal을 무시한다. defunct(좀비)를 막는다.
 */
/*----------------------------------------------------------------------------*/
int SubSignal()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SubSignal [START]...");

	if (flag)
	{
		/*
		 * child process 가 종료되면서 발생하는 시그널은 무시한다.
		 *     SIGCHLD (17)
		 */

		signal(SIGCHLD, SIG_IGN);
		if (!flag) LOG(_FL_, 0, 0, "INFO : signal(SIGCHLD, SIG_IGN)....");
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 서버의 정보를 얻는다.
 *
 *       TODO : DATA.2013.09.18
 *           0 : 개발
 *           1 : 테스트
 *           3 : 운영
 *
 *           1 : 업무계AP
 *           2 : 정보계AP
 *           3 : 뱅킹FEP
 *
 *           01 : #01
 *           02 : #02
 *           03 : #03
 *           04 : #04
 *
 *           ex) 0101(개발업무계#01), 3303(운영뱅킹FEP#03)
 */
/*----------------------------------------------------------------------------*/
int SubServerInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SubServerInfo [START]...");

	if (flag)
	{
		gethostname(sHostName, sizeof(sHostName));
		strcpy(sHostNo, SinfGetHostNo(sHostName));

		if (!strcmp(sHostNo, "03")
		||  !strcmp(sHostNo, "23")
		||  !strcmp(sHostNo, "24"))
		{
			strcpy(sRunSvr, "FEP");
		}
		else
		{
			strcpy(sRunSvr, "AP");
		}

		sprintf(sCurHHMM, "%5.5s", TimeGetTime2());

		iWeekNo = TimeGetWeek();

		if (flag)
		{
			LOG(_FL_, 0, 0, "* [sRunSvr=%s] [sHostName=%s] [sHostNo=%s] [iWeekNo=%d:%s] [sCurHHMM=%s]"
				, sRunSvr, sHostName, sHostNo, iWeekNo, sWeekNm[iWeekNo], sCurHHMM);

			/*
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "local"   , SinfGetHostNo("local"   ));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "dbizap01", SinfGetHostNo("dbizap01"));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "dfep03"  , SinfGetHostNo("dfep03"  ));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "pbizap01", SinfGetHostNo("pbizap01"));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "pbizap02", SinfGetHostNo("pbizap02"));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "pfep03"  , SinfGetHostNo("pfep03"  ));
			LOG(_FL_, 0, 0, "> %-15s = [%s]", "pfep04"  , SinfGetHostNo("pfep04"  ));
			*/
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : biz.properties
 */
/*----------------------------------------------------------------------------*/
int SubBizInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SubBizInfo [START]...");

	if (flag)
	{
		static time_t     oldTime                = (time_t) 0;
		static char       sBizFileName[128+1]    = { 0x00, };

		if (flag)
		{
			/*
			 * biz.properties 파일이 갱신이 되었는지를 확인한다.
			 */
			if (sBizFileName[0] == 0x00)
			{
				strcpy(sBizFileName, CfgGetMst("ib.cfg.file.biz"));
				if (flag) LOG(_FL_, 5, 0, "BIZ정보 파일 : [%s]", sBizFileName);
			}

			struct stat stStat;
			memset((void*)&stStat, 0x00, sizeof(struct stat));

			if (lstat(sBizFileName, &stStat) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : file not found. [%s]", sBizFileName);
				return -1;
			}

			if (stStat.st_mtime == oldTime)
			{
				/* 파일의 변경이 없다. */
				return 0;
			}
			if (flag) LOG(_FL_, 5, 0, "BIZ정보 변경 : stStat.st_mtime=[%d], oldTime=[%d]", stStat.st_mtime, oldTime);

			oldTime = stStat.st_mtime;
		}

		if (flag)
		{
			/*
			 * biz.properties 파일을 읽어 정보를 얻는다.
			 */

			FILE* fp;
			char strLine[MAX_LINE_SIZE];
			char* arrItem[MAX_ITEM];
			int i;
			int flgGrp = 0;

			/* 처리할 파일을 오픈한다. */
			fp = fopen(sBizFileName, "r+");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : file couldn't open. [%s]", sBizFileName);
				return -1;
			}

			iMaxBizProp = -1;

			/* 한 라인씩 읽으며 처리를 한다. */
			while (1)
			{
				/* strLine은 CR/LF를 포함한 문자열이다. */
				if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
					break;

				UtilTrim(UtilDelComment(strLine));
				if (strcmp(strLine, "") == 0)
					continue;

				if (!flag) LOG(_FL_, 0, 0, " strLine = [%s]", strLine);

				/* sSvcId */
				if (*strLine == '[')
				{
					iMaxBizProp ++;

					strncpy(stBizProp[iMaxBizProp].sSvcId, strLine+1, SERVICE_SIZE);

					/*
					 * default params
					 */

					strcpy(stBizProp[iMaxBizProp].sDesc   , "");
					strcpy(stBizProp[iMaxBizProp].sObsFlag, "N");
					strcpy(stBizProp[iMaxBizProp].sRunScpt, "");

					if (stBizProp[iMaxBizProp].sSvcId[1] == 'F')  /* OFS/OFR */
						strcpy(stBizProp[iMaxBizProp].sRunSvr , "AP");
					else
						strcpy(stBizProp[iMaxBizProp].sRunSvr , "FEP");

					strcpy(stBizProp[iMaxBizProp].sRunTime, "");

					int idx;
					for (idx=0; idx < 7; idx++)
					{
						strcpy(stBizProp[iMaxBizProp].rt[idx].sBeg, "00:05");
						strcpy(stBizProp[iMaxBizProp].rt[idx].sFin, "23:55");
					}

					continue;
				}

				if (!flag) LOG(_FL_, 0, 0, "%s> [%s]", stBizProp[iMaxBizProp].sSvcId, strLine);

				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '=', arrItem);

				/* 해당 환경파일 인자를 처리한다. */
				if (strcmp("DESCRIPTION", UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stBizProp[iMaxBizProp].sDesc, UtilTrim(arrItem[1]));
				}
				else if (strcmp("OBSERVATION_FLAG", UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stBizProp[iMaxBizProp].sObsFlag, UtilTrim(arrItem[1]));
				}
				else if (strcmp("RUN_SCRIPT", UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stBizProp[iMaxBizProp].sRunScpt, UtilTrim(arrItem[1]));
				}
				else if (strcmp("RUNNING_SERVER", UtilTrim(arrItem[0])) == 0)
				{
					strcpy(stBizProp[iMaxBizProp].sRunSvr, UtilTrim(arrItem[1]));
				}
				else if (strcmp("RUNNING_TIME", UtilTrim(arrItem[0])) == 0)
				{
					/*
					 * 관련 프로세스의 요일별 실행시간 설정. 기동/종료 시간(HH:MM)
					 */

					strcpy(stBizProp[iMaxBizProp].sRunTime, UtilTrim(arrItem[1]));

					char* arrWeek[MAX_ITEM];
					UtilStrSplit(arrItem[1], '|', arrWeek);

					int i;
					for (i=0; i < MAX_ITEM && arrWeek[i]; i++)
					{
						/*
						 * 요일번호별 기동/종료 시간 정보
						 */
						char* arr[MAX_ITEM];
						UtilStrSplit(arrWeek[i], '~', arr);

						int idx = UtilToInt(UtilTrim(arr[0])); /* 요일번호 */
						if (0 <= idx && idx <= 6)
						{
							strcpy(stBizProp[iMaxBizProp].rt[idx].sBeg, UtilTrim(arr[1]));
							strcpy(stBizProp[iMaxBizProp].rt[idx].sFin, UtilTrim(arr[2]));
						}
					}
				}
				else
				{
					if (!flag) LOG(_FL_, 0, 0, "WARNNING : wrong parameter. [%s]", UtilTrim(arrItem[0]));
				}
			}

			/* 처리된 파일을 닫는다. */
			fclose(fp);

			iMaxBizProp ++;
			if (iMaxBizProp == 0)
				return -1;

			if (!flag)
			{
				/*
				 * 얻은 리스트 정보를 출력해 본다.
				 */

				int i;
				for (i=0; i < iMaxBizProp; i++)
				{
					LOG(_FL_, 0, 0, "%4d/%d)", (i+1), iMaxBizProp);
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sSvcId   = [%s]", i, stBizProp[i].sSvcId  );
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sDesc    = [%s]", i, stBizProp[i].sDesc   );
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sObsFlag = [%s]", i, stBizProp[i].sObsFlag);
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunScpt = [%s]", i, stBizProp[i].sRunScpt);
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunSvr  = [%s]", i, stBizProp[i].sRunSvr );
					LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunTime = [%s]", i, stBizProp[i].sRunTime);

					int idx;   /* 요일번호 */
					for (idx=0; idx < 7; idx++)
					{
						if (!flag) LOG(_FL_, 0, 0, "\t\t stBizProp[%d].rt[%d].sBeg = [%s]", i, idx, stBizProp[i].rt[idx].sBeg);
						if (!flag) LOG(_FL_, 0, 0, "\t\t stBizProp[%d].rt[%d].sFin = [%s]", i, idx, stBizProp[i].rt[idx].sFin);

						if (flag) LOG(_FL_, 0, 0, "\t\t stBizProp[%d].rt[%d] = [%s]~[%s]"
							, i, idx, stBizProp[i].rt[idx].sBeg, stBizProp[i].rt[idx].sFin);
					}
				}

				LOG(_FL_, 0, 0, " iMaxBizProp = [%d]", iMaxBizProp);
			}
		}
	}

	return 0;
}

#if defined __linux__

/*============================================================================*/
/*
 * 내용 : for LINUX      /proc
 *
 *      #if defined __linux__
 *      #elif defined _AIX_TMP
 *      #elif defined _AIX
 *      #endif
 */
/*----------------------------------------------------------------------------*/
int SubProcInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SubProcInfo [START]...");

	if (!flag) LOG(_FL_, 0, 0, "##### LINUX : SubProcInfo() #####");

	if (flag)
	{
		DIR* pProc;
		struct dirent* pProcEnt;
		struct stat    stStat;
		char sPath[128];
		FILE* fp;
		char buf[128];
		char* arr[MAX_ITEM];
		int   arrCnt;
		
		pProc = opendir(PROC_DIR);

		iMaxProcInfo = 0;

		while ( TRUE )
		{
			pProcEnt = readdir(pProc);
			if (pProcEnt == NULL)
				break;

			if (!strcmp(pProcEnt->d_name, ".") || !strcmp(pProcEnt->d_name, ".."))
				continue;

			if (flag)
			{
				/*
				 * PID 폴더를 확인한다.
				 *  - lstat을 확인
				 *  - 폴더인지 확인
				 *  - UID 와 프로세스 UID와 같은지 확인
				 */
				sprintf(sPath, "%s/%s", PROC_DIR, pProcEnt->d_name);
				if (!flag) LOG(_FL_, 0, 0, "[%s]", sPath);

				if (lstat(sPath, &stStat) < 0)
					continue;

				if (S_ISDIR(stStat.st_mode) == 0)
					continue;

				if (getuid() != stStat.st_uid)
					continue;
			}

			if (flag)
			{
				/*
				 * cmdline을 확인한다. 그리고 cmdline을 읽는다.
				 */
				sprintf(sPath, "%s/%s/cmdline", PROC_DIR, pProcEnt->d_name);
				if (!flag) LOG(_FL_, 0, 0, "\t [%s]", sPath);

				fp = fopen(sPath, "r");
				if (fp == NULL)
					continue;

				memset(buf, 0x00, sizeof(buf));
				fgets(buf, sizeof(buf), fp);
				if (!flag) LOG(_FL_, 0, 0, "[sPath:%s] [buf:%s]", sPath, buf);

				fclose(fp);

				/*
				 * /proc/[PID]/cmdline의 내용을 정리한다. -> char* arr[];
				 */
				arrCnt = UtilGetCmdLine(buf, arr);

				if (!flag)
				{
					/* buf 출력 */
					int i;
					for (i=0; arr[i] && i < arrCnt; i++)
					{
						printf("\t\t (%d) [%s]\n", i, arr[i]);
					}
					printf("\t\t-----------------------------------\n");
				}
			}

			if (flag)
			{
				if (strcmp(arr[0], "/bin/ksh") == 0)
				{
					/*
					 * ksh shell 프로그램으로 실행
					 */
					char* p = strstr(arr[1], HOME_DIR);
					if (p != arr[1] || arrCnt != 3)
						continue;
	
					char* pid = pProcEnt->d_name;
					char* cmd = arr[1];
					char* svc = arr[2];
	
					if (!flag) LOG(_FL_, 0, 0, "[PID:%s][CMD:%s][SVC:%s]", pid, cmd, svc);
	
					strcpy(stProcInfo[iMaxProcInfo].sSvcId  , svc);
					strcpy(stProcInfo[iMaxProcInfo].sProcess, cmd);
					strcpy(stProcInfo[iMaxProcInfo].sPid    , pid);
	
					stProcInfo[iMaxProcInfo].iPid = UtilToInt(pid);
	
					iMaxProcInfo ++;
				}
				else
				{
					/*
					 * bin 프로그램으로 실행
					 */
					char* p = strstr(arr[0], HOME_DIR);
					if (p != buf || arrCnt != 2)
						continue;
	
					char* pid = pProcEnt->d_name;
					char* cmd = arr[0];
					char* svc = arr[1];
	
					if (!flag) LOG(_FL_, 0, 0, "[PID:%s][CMD:%s][SVC:%s]", pid, cmd, svc);
	
					strcpy(stProcInfo[iMaxProcInfo].sSvcId  , svc);
					strcpy(stProcInfo[iMaxProcInfo].sProcess, cmd);
					strcpy(stProcInfo[iMaxProcInfo].sPid    , pid);
	
					stProcInfo[iMaxProcInfo].iPid = UtilToInt(pid);
	
					iMaxProcInfo ++;
				}
			}
		}

		closedir(pProc);

		if (!flag)
		{
			/*
			 * 얻은 리스트 정보를 출력해 본다.
			 */

			int i;
			for (i=0; i < iMaxProcInfo; i++)
			{
				LOG(_FL_, 0, 0, "%4d/%d)", (i+1), iMaxProcInfo);
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sSvcId   = [%s]", i, stProcInfo[i].sSvcId  );
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sProcess = [%s]", i, stProcInfo[i].sProcess);
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sPid     = [%s]", i, stProcInfo[i].sPid    );
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].iPid     = [%d]", i, stProcInfo[i].iPid    );
			}

			LOG(_FL_, 0, 0, " iMaxProcInfo = [%d]", iMaxProcInfo);
		}
	}

	return 0;
}

#elif defined _AIX

/*============================================================================*/
/*
 * 내용 : for AIX           /proc
 *
 *      #if defined __linux__
 *      #elif defined _AIX_TMP
 *      #elif defined _AIX
 *      #endif
 */
/*----------------------------------------------------------------------------*/
int SubProcInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SubProcInfo [START]...");

	if (!flag) LOG(_FL_, 0, 0, "##### AIX : SubProcInfo() #####");

	if (flag)
	{
		DIR* pProc;
		struct dirent* pProcEnt;
		struct stat    stStat;
		char sPath[128];
		FILE* fp;
		char buf[MAX_LINE_SIZE];
		struct psinfo psInfo;

		pProc = opendir(PROC_DIR);

		iMaxProcInfo = 0;

		while ( TRUE )
		{
			pProcEnt = readdir(pProc);
			if (pProcEnt == NULL)
				break;

			if (!strcmp(pProcEnt->d_name, ".") || !strcmp(pProcEnt->d_name, ".."))
				continue;

			if (flag)
			{
				/*
				 * PID 폴더를 확인한다.
				 *  - lstat을 확인
				 *  - 폴더인지 확인
				 *  - UID 와 프로세스 UID와 같은지 확인
				 */
				sprintf(sPath, "%s/%s", PROC_DIR, pProcEnt->d_name);
				if (!flag) LOG(_FL_, 0, 0, "[%s]", sPath);

				if (lstat(sPath, &stStat) < 0)
					continue;

				if (S_ISDIR(stStat.st_mode) == 0)
					continue;

				if (getuid() != stStat.st_uid)
					continue;

				if (!flag) LOG(_FL_, 0, 0, "[%d] [%s]", stStat.st_uid, sPath);
			}

			if (flag)
			{
				/*
				 * psinfo을 확인한다. 그리고 psinfo을 읽는다.
				 */
				sprintf(sPath, "%s/%s/psinfo", PROC_DIR, pProcEnt->d_name);
				if (!flag) LOG(_FL_, 0, 0, "\t [%s]", sPath);

				fp = fopen(sPath, "r");
				if (fp == NULL)
					continue;

				int ret = fread((void*)&psInfo, 1, sizeof(struct psinfo), fp);

				fclose(fp);

				if (ret != sizeof(psInfo))
				{
					if (flag) LOG(_FL_, 9, 0, "ERROR : file read error. [ret=%d][size=%d] [%s] ", ret, sizeof(psInfo), sPath);
					continue;
				}
			}

			if (flag)
			{
				/* /hw01/ibridge 이 아니면 continue */
				char* p = NULL;
				p = strstr(psInfo.pr_psargs, HOME_DIR);
				if (p != psInfo.pr_psargs)
					continue;

				/* ps_man 이면 continue */
				p = strstr(psInfo.pr_psargs, "ps_man");
				if (p != NULL)
					continue;

				if (!flag) LOG(_FL_, 0, 0, "[%s]", psInfo.pr_psargs);

				char* arrItem[MAX_ITEM];
				UtilStrSplit(psInfo.pr_psargs, ' ', arrItem);

				char* pid = pProcEnt->d_name;
				char* cmd = UtilTrim(arrItem[0]);
				char* svc = UtilTrim(arrItem[1]);

				if (!flag) LOG(_FL_, 0, 0, "[PID:%s][CMD:%s][SVC:%s][iMaxProcInfo:%d]", pid, cmd, svc, iMaxProcInfo);

				strcpy(stProcInfo[iMaxProcInfo].sSvcId  , svc);
				strcpy(stProcInfo[iMaxProcInfo].sProcess, cmd);
				strcpy(stProcInfo[iMaxProcInfo].sPid    , pid);

				stProcInfo[iMaxProcInfo].iPid = UtilToInt(pid);

				iMaxProcInfo ++;
			}
		}

		closedir(pProc);

		if (!flag)
		{
			/*
			 * 얻은 리스트 정보를 출력해 본다.
			 */

			int i;
			for (i=0; i < iMaxProcInfo; i++)
			{
				LOG(_FL_, 0, 0, "%4d/%d)", (i+1), iMaxProcInfo);
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sSvcId   = [%s]", i, stProcInfo[i].sSvcId  );
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sProcess = [%s]", i, stProcInfo[i].sProcess);
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].sPid     = [%s]", i, stProcInfo[i].sPid    );
				LOG(_FL_, 0, 0, "\t stProcInfo[%d].iPid     = [%d]", i, stProcInfo[i].iPid    );
			}

			LOG(_FL_, 0, 0, " iMaxProcInfo = [%d]", iMaxProcInfo);
		}
	}

	return 0;
}

#endif

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SubDoProcess()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	char sSMS           [100];

	int iDiff = 0;
	
	int iTimeOut      = 180;
	int iMaperTimeOut =  25;
		
	if (!flag) LOG(_FL_, 0, 0, "SubDoProcess [START]...");

	if (flag)
	{
		/*
		 * biz.properties 정보에서 아래의 조건에 맞는 정보를 찾는다.
		 *   1. sRunSvr(AP/FEP) 에 해당하는가?
		 *   2. sObsFlag(Y/N) 에 해당하는가? (Y) 관찰프로세스
		 *   3. sRunTime 에 해당하는가?   Beg <= CurHHMM < Fin
		 *   4. 위 조건에 맞는 서비스 프로세스가 실행/종료 상태인가?
		 */

		int i;
		int idxPid;

		for (i=0; i < iMaxBizProp; i++)
		{
			/* 해당서버인지 확인하여 아니면 continue */
			if (strcmp(stBizProp[i].sRunSvr, sRunSvr) != 0)
				continue;

			/* 관심대상 서비스를 확인하여 아니면 continue */
			if (strcmp(stBizProp[i].sObsFlag, "Y") != 0)
				continue;

			if (!flag)
			{
				LOG(_FL_, 0, 0, "%4d/%d)", (i+1), iMaxBizProp);
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sSvcId   = [%s]", i, stBizProp[i].sSvcId  );
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sDesc    = [%s]", i, stBizProp[i].sDesc   );
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sObsFlag = [%s]", i, stBizProp[i].sObsFlag);
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunScpt = [%s]", i, stBizProp[i].sRunScpt);
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunSvr  = [%s]", i, stBizProp[i].sRunSvr );
				LOG(_FL_, 0, 0, "\t stBizProp[%d].sRunTime = [%s]", i, stBizProp[i].sRunTime);
				LOG(_FL_, 0, 0, "\t stBizProp[%d].rt[%d].sBeg = [%s]", i, iWeekNo, stBizProp[i].rt[iWeekNo].sBeg);
				LOG(_FL_, 0, 0, "\t stBizProp[%d].rt[%d].sFin = [%s]", i, iWeekNo, stBizProp[i].rt[iWeekNo].sFin);
			}

			idxPid = FindProcPid(stBizProp[i].sSvcId);
			if (idxPid < 0)
			{
				/*
				 * 실행 중인 프로세스를 못찾았다.   -> NOT FOUND
				 * sCurHHMM 값이 기동/종료시간 범위 안에 있으면 실행한다.
				 */
				if (strcmp(stBizProp[i].rt[iWeekNo].sBeg, sCurHHMM) <= 0
				&&  strcmp(sCurHHMM, stBizProp[i].rt[iWeekNo].sFin) <  0)
				{
					/* 프로세스를 실행한다. */
					if (!flag) LOG(_FL_, 0, 0, "[기동] RunScpt = [%s]", stBizProp[i].sRunScpt);

					RunProcess(i);
				}
				else
				{
					if (!flag) LOG(_FL_, 0, 0, "[종료] 상태 [%s]", stBizProp[i].sSvcId);
				}
			}
			else
			{
				/*
				 * 실행 중인 프로세스를 찾았다.    -> FOUND !   OLLEH
				 * sCurHHMM 값이 기동/종료시간 범위 밖에 있으면 종료한다.
				 */
				if (strcmp(sCurHHMM, stBizProp[i].rt[iWeekNo].sBeg) <  0
				||  strcmp(stBizProp[i].rt[iWeekNo].sFin, sCurHHMM) <= 0)
				{
					/* 프로세스를 종료한다. */
					if (!flag) LOG(_FL_, 0, 0, "[종료] RunScpt = [%s]", stBizProp[i].sRunScpt);

					KillProcess(idxPid);
				}
				else
				{
					iDiff=SigAliveSec(stBizProp[i].sSvcId);
					
					
                    /*
					 * 20초 동안 기록이 없는경우 프로세스 종료한다.
					 */
					if (strcmp("MAACOM0000", stBizProp[i].sSvcId) == 0) {
						if (iDiff > 15) {
							if (flag) LOG(_FL_, 9, 0, "매퍼 무응답 : %d초", iDiff);
                        }							
                        if (iDiff > iMaperTimeOut) {
							sprintf(sSMS, "매퍼[%s] 무응답 (%d초 경과) -> 재기동" , stBizProp[i].sSvcId, iMaperTimeOut);
							
                            if (flag) LOG(_FL_, 9, 0, "ERROR SMS전송 : [%s] ", sSMS);
		                        	
					        if (UmsMsgSend(sSMS) < 0)
							{
								printf("ERROR : UMS SEND....\n");
							}
								
							/* 프로세스를 종료한다. */
						    if (!flag) LOG(_FL_, 0, 0, "[종료] TimeOut = [%d], RunScpt = [%s]", iTimeOut, stBizProp[i].sRunScpt);
							 
							KillProcess(idxPid);
							SigAliveWrite(stBizProp[i].sSvcId);
					    }
						
					}
					
					/*
					 * 180 초 동안 기록이 없는경우 프로세스 종료한다.
					 */
					if (iDiff > iTimeOut ) {
						
						sprintf(sSMS, "%s 무응답 (%d초 경과) -> 재기동" , stBizProp[i].sSvcId, iTimeOut);
	                        	
				        if (UmsMsgSend(sSMS) < 0)
						{
							printf("ERROR : UMS SEND....\n");
						}
							
						 /* 프로세스를 종료한다. */
					     if (!flag) LOG(_FL_, 0, 0, "[종료] TimeOut = [%d], RunScpt = [%s]", iTimeOut, stBizProp[i].sRunScpt);
						 
						 KillProcess(idxPid);
						 SigAliveWrite(stBizProp[i].sSvcId);
					}
					
					if (!flag) LOG(_FL_, 0, 0, "[실행] 상태 [%s]", stBizProp[i].sSvcId);
				}
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

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int MainService()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 시그널 처리를 한다.
		 */

		if (SubSignal() < 0)
			return -1;
	}

	if (flag)
	{
		while ( TRUE )
		{
			if (flag)
			{
				/*
				 * Server 정보를 읽는다.
				 */

				if (SubServerInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * biz.properties 정보를 읽는다.
				 */

				if (SubBizInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * /proc 정보를 읽는다.
				 */

				if (SubProcInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * ibridge 관련 프로세스를 실행/종료 처리한다.
				 */

				if (SubDoProcess() < 0)
					break;
			}

			if (!flag) break;

			if (flag)
			{
				/* 나중에 적당하게 시간(초)를 조절하여 사용하기 바람. */
				int waitTime = 10;
				if (!flag) printf("sleep(%d)\n", waitTime);
				sleep(waitTime);
			}
		}
	}

	if (flag)
	{
		printf("프로그램 종료...\n");
	}

	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
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

	if (flag)
	{
		/*
		 * 임시로 사용함.
		 */
		if (!flag && argc == 1 && strcmp(argv[0], "ps_man") == 0)
		{
			printf("ps_man으로 실행하지 말아 주세요.\n");
			exit(0);
		}

		strcpy(sDummy, "PS_MANAGER");
		argc = 2;
		argv[1] = (char*)&sDummy;
	}

	if (flag)
	{
		/*
		 * 기본 처리
		 */
		sProgName = argv[0];

		if (argc < 2 || strcmp("PS_MANAGER", argv[1]) != 0)
		{
			/*
			 * USAGE
			 */
			printf("ERROR : Invalid argument... (Usage : %s PS_MANAGER)\n", sProgName);
			exit(0);
		}

		if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-ver") == 0)
		{
			/*
			 * VERSION
			 */
			printf("Program : [%s], Version : %s\n", sProgName, VERSION);
			exit(0);
		}
	}

	if (flag)
	{
		/*
		 * 프로세스에서 사용하는 서비스명과 FEPID를 얻는다.
		 */
		UtilToUpper(argv[1]);
		sprintf(sServiceid, "%-10.10s", argv[1]);

		if (flag) printf("\x1b[44m< %s %s >\x1b[0m\n", sProgName, sServiceid);
		if (flag) printf("Program : [%s], Version : %s\n", sProgName, VERSION);
		if (flag) printf("\t -------------------------------------------\n");
	}

	if (flag)
	{
		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		if (CfgMst() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		if (flag)
		{
			printf("\t CfgGetMaster(ib.version  )  [%s]\n", CfgGetMst("ib.version"  ));
			printf("\t CfgGetMaster(ib.serialkey)  [%s]\n", CfgGetMst("ib.serialkey"));
			printf("\t CfgGetMaster(ib.author   )  [%s]\n", CfgGetMst("ib.author"   ));
			printf("\t CfgGetMaster(ib.company  )  [%s]\n", CfgGetMst("ib.company"  ));
			printf("\t -------------------------------------------\n");

			printf("\n프로그램을 시작합니다.\n");
		}
	}

	if (flag)
	{
		/*
		 * fork를 이용하여 child process 를 실행한다.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * 표준출력을 LOG처리한다.
		 */
		if (flag)
		{
			char strPath[128];
			char strFile[128];

			sprintf(strPath, CfgGetMst("ib.path.log"));
			UtilSetYYYYMMDD(strPath);
			mkdir(strPath, 0777);

			sprintf(strFile, "%s/%s", strPath, sServiceid);

			if (access(strFile, R_OK) < 0)
			{
				/* 최초생성 */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND 모드 */
				freopen(strFile, "a+", stdout);
			}

			if (flag) LOG(_FL_, 9, 0, "\x1b[44m-------------------- [ %s ] --------------------\x1b[0m"
				, TimeGetDateTime3());

			if (flag) LOG(_FL_, 9, 0, "LOGFILE = [%s]", strFile);
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag)
	{
		MainService();
	}
}

#endif