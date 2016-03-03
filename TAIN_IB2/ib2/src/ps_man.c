/*
 * PROGRAM ID   : ps_man.c
 * PROGRAM NAME : ps_man ���
 * AUTHOR       : (��)ICA
 * COMMENT      : ps_man ���
 *                ���μ��� ����
 *                biz.properties �������� ������ �о� ���� ���μ������� �����Ѵ�.
 *
 * HISTORY      : 2013.09.17 ����. �����ۼ�
 *              : 2013.09.18 ����. �׸� �߰�. RUNNING_SERVER=AP/FEP, RUNNING_TIME=00:00~00:00
 *              : 2013.10.29 ����. shell ���α׷��� ó�� ������� ����
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.10.29 ����. shell ���α׷��� ó�� ������� ����"

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
 *        char sBeg          [ 10];  // ���۽ð� (HHMM)
 *        char sFin          [ 10];  // ����ð� (HHMM)
 *    } RUN_TIME;
 *
 *     * biz.properties ���� ����ü
 *
 *    typedef struct _BIZ_PROP_
 *    {
 *        char sSvcId        [ 20];  // ���� ID
 *        char sDesc         [128];  // ���� Description
 *        char sObsFlag      [  5];  // ���� �÷���
 *        char sRunScpt      [128];  // ���� ��ũ��Ʈ
 *        char sRunSvr       [ 20];  // ���� ���� (AP/FEP)
 *        char sRunTime      [128];  // ���� �ð� (0~HH:MM~HH:MM)
 *
 *        RUN_TIME  rt       [  7];  // ���Ϻ�RunTime 0(�Ͽ���) 6(�����)
 *    } BIZ_PROP;
 *
 *     * proc ���� ����ü
 *
 *    typedef struct _PROC_INFO_
 *    {
 *        char sSvcId        [ 20];  // ���� ID
 *        char sProcess      [128];  // �������μ���
 *        char sPid          [ 20];  // PID ���ڿ�
 *        int  iPid               ;  // PID
 *    } PROC_INFO;
 *
 *
 */
/*----------------------------------------------------------------------------*/

char*    sWeekNm[]                   = { "��","��","ȭ","��","��","��","��" };

char     sHostName           [ 50+1] = { 0x00, };    /* hostname */
char     sHostNo             [ 50+1] = { 0x00, };    /* host number */
char     sRunSvr             [ 50+1] = { 0x00, };    /* ���� ���� */

char     sCurHHMM            [ 10+1] = { 0x00, };    /* ���� �ú� */

int      iWeekNo                                ;    /* ���Ϲ�ȣ 0:�Ͽ��� */

/*----------------------------------------------------------------------------*/

BIZ_PROP     stBizProp          [MAX_BIZ_PROP];     /* BizProp ���� ���� */
int          iMaxBizProp                      ;

PROC_INFO    stProcInfo        [MAX_PROC_INFO];     /* proc ���� ���� */
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
 * ���� :
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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int RunProcess(int idxBiz)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 9, 0, "\x1b[44m--------------- START [�⵿] (%s) ---------------\x1b[0m"
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
					if (flag) LOG(_FL_, 9, 0, "\x1b[44m--------------- END   [�⵿] (%s) ---------------\x1b[0m"
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
			 * DATE.2013.10.29 : execl -> execv �� ������ (����: �̰� ����)
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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int KillProcess(int idxPid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 9, 0, "\x1b[41m--------------- START [����] (%s) ---------------\x1b[0m"
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
		 * child process�� signal�� ������. ��, �����Ų��.
		 *      SIGTERM (15)
		 */
		if (!flag) kill(stProcInfo[idxPid].iPid, SIGTERM);
		if (flag) kill(stProcInfo[idxPid].iPid, SIGKILL);

		if (flag) LOG(_FL_, 9, 0, "INFO : kill(%d, SIGTERM)....", stProcInfo[idxPid].iPid);
	}

	if (flag) LOG(_FL_, 9, 0, "\x1b[41m--------------- END   [����] (%s) ---------------\x1b[0m"
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
 * ���� : child signal�� �����Ѵ�. defunct(����)�� ���´�.
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
		 * child process �� ����Ǹ鼭 �߻��ϴ� �ñ׳��� �����Ѵ�.
		 *     SIGCHLD (17)
		 */

		signal(SIGCHLD, SIG_IGN);
		if (!flag) LOG(_FL_, 0, 0, "INFO : signal(SIGCHLD, SIG_IGN)....");
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ������ ������ ��´�.
 *
 *       TODO : DATA.2013.09.18
 *           0 : ����
 *           1 : �׽�Ʈ
 *           3 : �
 *
 *           1 : ������AP
 *           2 : ������AP
 *           3 : ��ŷFEP
 *
 *           01 : #01
 *           02 : #02
 *           03 : #03
 *           04 : #04
 *
 *           ex) 0101(���߾�����#01), 3303(���ŷFEP#03)
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
 * ���� : biz.properties
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
			 * biz.properties ������ ������ �Ǿ������� Ȯ���Ѵ�.
			 */
			if (sBizFileName[0] == 0x00)
			{
				strcpy(sBizFileName, CfgGetMst("ib.cfg.file.biz"));
				if (flag) LOG(_FL_, 5, 0, "BIZ���� ���� : [%s]", sBizFileName);
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
				/* ������ ������ ����. */
				return 0;
			}
			if (flag) LOG(_FL_, 5, 0, "BIZ���� ���� : stStat.st_mtime=[%d], oldTime=[%d]", stStat.st_mtime, oldTime);

			oldTime = stStat.st_mtime;
		}

		if (flag)
		{
			/*
			 * biz.properties ������ �о� ������ ��´�.
			 */

			FILE* fp;
			char strLine[MAX_LINE_SIZE];
			char* arrItem[MAX_ITEM];
			int i;
			int flgGrp = 0;

			/* ó���� ������ �����Ѵ�. */
			fp = fopen(sBizFileName, "r+");
			if (fp == NULL)
			{
				LOG(_FL_, 0, 0, "ERROR : file couldn't open. [%s]", sBizFileName);
				return -1;
			}

			iMaxBizProp = -1;

			/* �� ���ξ� ������ ó���� �Ѵ�. */
			while (1)
			{
				/* strLine�� CR/LF�� ������ ���ڿ��̴�. */
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

				/* �Ѷ����� �����ڷ� �ɰ���. */
				UtilStrSplit(strLine, '=', arrItem);

				/* �ش� ȯ������ ���ڸ� ó���Ѵ�. */
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
					 * ���� ���μ����� ���Ϻ� ����ð� ����. �⵿/���� �ð�(HH:MM)
					 */

					strcpy(stBizProp[iMaxBizProp].sRunTime, UtilTrim(arrItem[1]));

					char* arrWeek[MAX_ITEM];
					UtilStrSplit(arrItem[1], '|', arrWeek);

					int i;
					for (i=0; i < MAX_ITEM && arrWeek[i]; i++)
					{
						/*
						 * ���Ϲ�ȣ�� �⵿/���� �ð� ����
						 */
						char* arr[MAX_ITEM];
						UtilStrSplit(arrWeek[i], '~', arr);

						int idx = UtilToInt(UtilTrim(arr[0])); /* ���Ϲ�ȣ */
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

			/* ó���� ������ �ݴ´�. */
			fclose(fp);

			iMaxBizProp ++;
			if (iMaxBizProp == 0)
				return -1;

			if (!flag)
			{
				/*
				 * ���� ����Ʈ ������ ����� ����.
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

					int idx;   /* ���Ϲ�ȣ */
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
 * ���� : for LINUX      /proc
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
				 * PID ������ Ȯ���Ѵ�.
				 *  - lstat�� Ȯ��
				 *  - �������� Ȯ��
				 *  - UID �� ���μ��� UID�� ������ Ȯ��
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
				 * cmdline�� Ȯ���Ѵ�. �׸��� cmdline�� �д´�.
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
				 * /proc/[PID]/cmdline�� ������ �����Ѵ�. -> char* arr[];
				 */
				arrCnt = UtilGetCmdLine(buf, arr);

				if (!flag)
				{
					/* buf ��� */
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
					 * ksh shell ���α׷����� ����
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
					 * bin ���α׷����� ����
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
			 * ���� ����Ʈ ������ ����� ����.
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
 * ���� : for AIX           /proc
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
				 * PID ������ Ȯ���Ѵ�.
				 *  - lstat�� Ȯ��
				 *  - �������� Ȯ��
				 *  - UID �� ���μ��� UID�� ������ Ȯ��
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
				 * psinfo�� Ȯ���Ѵ�. �׸��� psinfo�� �д´�.
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
				/* /hw01/ibridge �� �ƴϸ� continue */
				char* p = NULL;
				p = strstr(psInfo.pr_psargs, HOME_DIR);
				if (p != psInfo.pr_psargs)
					continue;

				/* ps_man �̸� continue */
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
			 * ���� ����Ʈ ������ ����� ����.
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
 * ���� :
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
		 * biz.properties �������� �Ʒ��� ���ǿ� �´� ������ ã�´�.
		 *   1. sRunSvr(AP/FEP) �� �ش��ϴ°�?
		 *   2. sObsFlag(Y/N) �� �ش��ϴ°�? (Y) �������μ���
		 *   3. sRunTime �� �ش��ϴ°�?   Beg <= CurHHMM < Fin
		 *   4. �� ���ǿ� �´� ���� ���μ����� ����/���� �����ΰ�?
		 */

		int i;
		int idxPid;

		for (i=0; i < iMaxBizProp; i++)
		{
			/* �ش缭������ Ȯ���Ͽ� �ƴϸ� continue */
			if (strcmp(stBizProp[i].sRunSvr, sRunSvr) != 0)
				continue;

			/* ���ɴ�� ���񽺸� Ȯ���Ͽ� �ƴϸ� continue */
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
				 * ���� ���� ���μ����� ��ã�Ҵ�.   -> NOT FOUND
				 * sCurHHMM ���� �⵿/����ð� ���� �ȿ� ������ �����Ѵ�.
				 */
				if (strcmp(stBizProp[i].rt[iWeekNo].sBeg, sCurHHMM) <= 0
				&&  strcmp(sCurHHMM, stBizProp[i].rt[iWeekNo].sFin) <  0)
				{
					/* ���μ����� �����Ѵ�. */
					if (!flag) LOG(_FL_, 0, 0, "[�⵿] RunScpt = [%s]", stBizProp[i].sRunScpt);

					RunProcess(i);
				}
				else
				{
					if (!flag) LOG(_FL_, 0, 0, "[����] ���� [%s]", stBizProp[i].sSvcId);
				}
			}
			else
			{
				/*
				 * ���� ���� ���μ����� ã�Ҵ�.    -> FOUND !   OLLEH
				 * sCurHHMM ���� �⵿/����ð� ���� �ۿ� ������ �����Ѵ�.
				 */
				if (strcmp(sCurHHMM, stBizProp[i].rt[iWeekNo].sBeg) <  0
				||  strcmp(stBizProp[i].rt[iWeekNo].sFin, sCurHHMM) <= 0)
				{
					/* ���μ����� �����Ѵ�. */
					if (!flag) LOG(_FL_, 0, 0, "[����] RunScpt = [%s]", stBizProp[i].sRunScpt);

					KillProcess(idxPid);
				}
				else
				{
					iDiff=SigAliveSec(stBizProp[i].sSvcId);
					
					
                    /*
					 * 20�� ���� ����� ���°�� ���μ��� �����Ѵ�.
					 */
					if (strcmp("MAACOM0000", stBizProp[i].sSvcId) == 0) {
						if (iDiff > 15) {
							if (flag) LOG(_FL_, 9, 0, "���� ������ : %d��", iDiff);
                        }							
                        if (iDiff > iMaperTimeOut) {
							sprintf(sSMS, "����[%s] ������ (%d�� ���) -> ��⵿" , stBizProp[i].sSvcId, iMaperTimeOut);
							
                            if (flag) LOG(_FL_, 9, 0, "ERROR SMS���� : [%s] ", sSMS);
		                        	
					        if (UmsMsgSend(sSMS) < 0)
							{
								printf("ERROR : UMS SEND....\n");
							}
								
							/* ���μ����� �����Ѵ�. */
						    if (!flag) LOG(_FL_, 0, 0, "[����] TimeOut = [%d], RunScpt = [%s]", iTimeOut, stBizProp[i].sRunScpt);
							 
							KillProcess(idxPid);
							SigAliveWrite(stBizProp[i].sSvcId);
					    }
						
					}
					
					/*
					 * 180 �� ���� ����� ���°�� ���μ��� �����Ѵ�.
					 */
					if (iDiff > iTimeOut ) {
						
						sprintf(sSMS, "%s ������ (%d�� ���) -> ��⵿" , stBizProp[i].sSvcId, iTimeOut);
	                        	
				        if (UmsMsgSend(sSMS) < 0)
						{
							printf("ERROR : UMS SEND....\n");
						}
							
						 /* ���μ����� �����Ѵ�. */
					     if (!flag) LOG(_FL_, 0, 0, "[����] TimeOut = [%d], RunScpt = [%s]", iTimeOut, stBizProp[i].sRunScpt);
						 
						 KillProcess(idxPid);
						 SigAliveWrite(stBizProp[i].sSvcId);
					}
					
					if (!flag) LOG(_FL_, 0, 0, "[����] ���� [%s]", stBizProp[i].sSvcId);
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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int MainService()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �ñ׳� ó���� �Ѵ�.
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
				 * Server ������ �д´�.
				 */

				if (SubServerInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * biz.properties ������ �д´�.
				 */

				if (SubBizInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * /proc ������ �д´�.
				 */

				if (SubProcInfo() < 0)
					break;
			}

			if (flag)
			{
				/*
				 * ibridge ���� ���μ����� ����/���� ó���Ѵ�.
				 */

				if (SubDoProcess() < 0)
					break;
			}

			if (!flag) break;

			if (flag)
			{
				/* ���߿� �����ϰ� �ð�(��)�� �����Ͽ� ����ϱ� �ٶ�. */
				int waitTime = 10;
				if (!flag) printf("sleep(%d)\n", waitTime);
				sleep(waitTime);
			}
		}
	}

	if (flag)
	{
		printf("���α׷� ����...\n");
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �ӽ÷� �����.
		 */
		if (!flag && argc == 1 && strcmp(argv[0], "ps_man") == 0)
		{
			printf("ps_man���� �������� ���� �ּ���.\n");
			exit(0);
		}

		strcpy(sDummy, "PS_MANAGER");
		argc = 2;
		argv[1] = (char*)&sDummy;
	}

	if (flag)
	{
		/*
		 * �⺻ ó��
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
		 * ���μ������� ����ϴ� ���񽺸�� FEPID�� ��´�.
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
		 * master ȯ�������� �д´�. biz.properties ���ϵ� �д´�.
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

			printf("\n���α׷��� �����մϴ�.\n");
		}
	}

	if (flag)
	{
		/*
		 * fork�� �̿��Ͽ� child process �� �����Ѵ�.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * ǥ������� LOGó���Ѵ�.
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
				/* ���ʻ��� */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND ��� */
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