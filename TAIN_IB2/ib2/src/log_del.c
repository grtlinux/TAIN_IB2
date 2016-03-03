/*
 * PROGRAM ID   : log_del.c
 * PROGRAM NAME : log_del ���
 * AUTHOR       : (��)ICA
 * COMMENT      : log_del ���
 * HISTORY      : 2013.11.14 ����. �����ۼ�
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.11.14 ����. �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.11.14 ����. �����۾�"

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   MAX_LINE_SIZE         8192

#define   MAX_ITEM                10

#define   MAX_LOG_DEL             20

/*============================================================================*/
/*
 * global variables
 *
 *    typedef struct
 *    {
 *        char            sObjPath        [128+1];   // �������
 *        int             iExpNo                 ;   // Expire Number
 *        char            sPattern        [128+1];   // ���ϰ˻� ����
 *    } LOG_DEL;
 */
/*----------------------------------------------------------------------------*/

time_t       tmComp                           ;       /* �񱳴�� �ð� sec    */
char         sComp                       [128];       /* �񱳴�� �ð� ���ڿ� */

int          iExpNo                           ;        /*     Expire Number   */

LOG_DEL      stLogDel            [MAX_LOG_DEL];          /* Log Del ���� ���� */
int          iMaxLogDel                       ;


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
int GetCfg()
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

		char* p = CfgGetMst("ib.cfg.file.logdel");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : log_del ȯ������ ������");
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
		
		iMaxLogDel = 0;

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
				if (strcmp("EXPIRE_NUMBER", UtilTrim(arr[0])) == 0)
				{
					if (!flag) LOG(_FL_, 0, 0, " [EXPIRE_NUMBER:%s]", UtilTrim(arr[1]));
					
					iExpNo = UtilToInt(UtilTrim(arr[1]));
				}
				else if (strcmp("OBJECT_FOLDER", UtilTrim(arr[0])) == 0)
				{
					if (!flag) LOG(_FL_, 0, 0, " [OBJECT_FOLDER:%s]", UtilTrim(arr[1]));
					
					UtilStrSplit(arr[1], '|', arr1);

					strcpy(stLogDel[iMaxLogDel].sObjPath, UtilTrim(arr1[0]));
					stLogDel[iMaxLogDel].iExpNo = UtilToInt(UtilTrim(arr1[1]));
					strcpy(stLogDel[iMaxLogDel].sPattern, UtilTrim(arr1[2]));
					
					iMaxLogDel ++;
				}
				else
				{
					if (!flag) LOG(_FL_, 0, 0, " NO PARAM [%s:%s]", UtilTrim(arr[0]), UtilTrim(arr[1]));
				}
			}
		}

		/* ó���� ������ �ݴ´�. */
		fclose(fp);
		
		if (flag)
		{
			/*
			 * ���� ����Ʈ ������ ����� ����.
			 */
			
			LOG(_FL_, 0, 0, " iExpNo = [%d]", iExpNo);

			int i;
			for (i=0; i < iMaxLogDel; i++)
			{
				LOG(_FL_, 0, 0, "%4d/%d)", (i+1), iMaxLogDel);
				LOG(_FL_, 0, 0, "\t stLogDel[%d].sObjPath = [%s]", i, stLogDel[i].sObjPath);
				LOG(_FL_, 0, 0, "\t stLogDel[%d].iExpNo   = [%d]", i, stLogDel[i].iExpNo  );
				LOG(_FL_, 0, 0, "\t stLogDel[%d].sPattern = [%s]", i, stLogDel[i].sPattern);
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
int PrintTm(time_t tm, struct tm* t)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		printf("UNIX TIME : %d sec\n", tm); /* from 1970/01/01 00:00:00 */
		printf("CURR YEAR : %04d\n", t->tm_year + 1900);
		printf("CURR MON  : %02d\n", t->tm_mon + 1);
		printf("CURR DAY  : %02d\n", t->tm_mday);
		printf("CURR HOUR : %02d\n", t->tm_hour);
		printf("CURR MIN  : %02d\n", t->tm_min);
		printf("CURR SEC  : %02d\n", t->tm_sec);
		printf("CURR WEEK : %d\n", t->tm_wday);
		printf("---------------------------\n");
	}
	
	return 0;
}

/*============================================================================*/
/*
 * ���� :
 *     �����Լ� : asctime, ctime, gmtime, localtime, mktime
 *                asctime_r, ctime_r, gmtime_r, localtime_r
 *                date(1), gettimeofday(2), time(2), utime(2), clock(3),
 *                difftime(3), strftime(3), strptime(3), timegm(3), tzset(3),
 *                time(7)
 */
/*----------------------------------------------------------------------------*/
int GetCompTime(int iExpNo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag)
	{
		time_t tmCur;
		struct tm t;

		tmCur =time(NULL);                     /* ���� �ð��� �ʴ����� ��´�. */
		localtime_r(&tmCur, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		if (!flag) PrintTm(tmCur, &t);

		tmCur = tmCur / (24 * 60 * 60) * (24 * 60 * 60);
		localtime_r(&tmCur, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		if (!flag) PrintTm(tmCur, &t);
	}

	if (flag)
	{
		time_t tmCur;
		struct tm t;

		tmCur =time(NULL);                     /* ���� �ð��� �ʴ����� ��´�. */
		localtime_r(&tmCur, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		if (!flag) PrintTm(tmCur, &t);

		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;

		tmCur = mktime(&t);
		localtime_r(&tmCur, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		if (!flag) PrintTm(tmCur, &t);

		tmCur = tmCur - (iExpNo * 24 * 60 * 60);
		localtime_r(&tmCur, &t);  /* �ʴ����� �ð��� �и��Ͽ� ����ü�� �ִ´�. */
		if (!flag) PrintTm(tmCur, &t);

		tmComp = tmCur;

		sprintf(sComp, "%04d%02d%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
	}

	if (flag)
	{
		struct tm t;

		localtime_r(&tmComp, &t);

		if (flag)
		{
			if (flag) LOG(_FL_, 0, 0, "[%04d/%02d/%02d] ���� ������ �����մϴ�."
				, t.tm_year + 1900
				, t.tm_mon + 1
				, t.tm_mday);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int DelFolder(char* sPath)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �ش� ���� ���� �ִ� ������ �����Ѵ�.
		 */

		DIR* pDp;
		struct dirent* pDirEnt;
		struct stat    st;
		char sFile[128];

		pDp = opendir(sPath);

		while ( TRUE )
		{
			pDirEnt = readdir(pDp);
			if (pDirEnt == NULL)
				break;

			/* ���þ��� ������ �ɷ�����. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* sub folder�� ���� ������ ��´�. */
			sprintf(sFile, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sFile, &st);

			if (flag) remove(sFile);
			if (flag) LOG(_FL_, 0, 0, "���� ���� [%s]", sFile);
		}

		closedir(pDp);
	}

	if (flag)
	{
		/*
		 * �ش� ������ �����Ѵ�.
		 */

		if (flag) remove(sPath);
		if (flag) LOG(_FL_, 0, 0, "[%s] ���� ����", sPath);
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int JobProc(char* sPath)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �������� �̿��� ���� ����
		 */
		
		DIR* pDp;
		struct dirent* pDirEnt;
		struct stat    st;
		char sSubPath[128];
		char sFile[128];

		pDp = opendir(sPath);

		while ( TRUE )
		{
			pDirEnt = readdir(pDp);
			if (pDirEnt == NULL)
				break;

			/* ���þ��� ������ �ɷ�����. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* ������ �����̸� �Ѿ��. */
			if (strcmp(sComp, pDirEnt->d_name) <= 0)
				continue;

			/* sub folder�� ���� ������ ��´�. */
			sprintf(sSubPath, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sSubPath, &st);

			if (!flag) LOG(_FL_, 0, 0, "[%s:%d] ���� ����", sSubPath, st.st_mtime);

			DelFolder(sSubPath);

			if (!flag) break;
		}

		closedir(pDp);
	}

	if (!flag)
	{
		/*
		 * ������ �ð��� �̿��� ���� ����
		 */
		
		DIR* pDp;
		struct dirent* pDirEnt;
		struct stat    st;
		char sSubPath[128];
		char sFile[128];

		pDp = opendir(sPath);

		while ( TRUE )
		{
			pDirEnt = readdir(pDp);
			if (pDirEnt == NULL)
				break;

			/* ���þ��� ������ �ɷ�����. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* sub folder�� ���� ������ ��´�. */
			sprintf(sSubPath, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sSubPath, &st);

			if (!flag) LOG(_FL_, 0, 0, "[%s:%d] %s"
				, sSubPath
				, st.st_mtime
				, (st.st_mtime < tmComp ? "���� ����" : "�ӽú���")
				);

			if (st.st_mtime < tmComp)
			{
				if (!flag) LOG(_FL_, 0, 0, "[%s:%d] ���� ����", sSubPath, st.st_mtime);

				DelFolder(sSubPath);
			}

			if (!flag) break;
		}

		closedir(pDp);
	}

	return 0;
}

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
		GetCfg();

		int i;
		for (i=0; i < iMaxLogDel; i++)
		{
			/* Expire Time�� ���Ѵ�. */
			if (flag) GetCompTime(stLogDel[i].iExpNo);

			/* Expire Time�� �ش��ϴ� ������ ó���Ѵ�. */
			if (flag) JobProc(stLogDel[i].sObjPath);
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

	if (flag)
	{
		/*
		 * �ӽ÷� �����.
		 */
		if (!flag && argc == 1 && strcmp(argv[0], "log_del") == 0)
		{
			printf("log_del �������� ���� �ּ���.\n");
			exit(0);
		}

		argc = 2;
		argv[1] = "LOGDEL0000";
	}

	if (flag)
	{
		/*
		 * �⺻ ó��
		 */
		sProgName = argv[0];

		if (argc < 2 || strcmp("LOGDEL0000", argv[1]) != 0)
		{
			/*
			 * USAGE
			 */
			printf("ERROR : Invalid argument... (Usage : %s LOGDEL0000)\n", sProgName);
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
		/* UtilToUpper(argv[1]); */
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

	if (!flag)
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

			if (flag) LOG(_FL_, 0, 0, "\x1b[44m-------------------- [ %s ] --------------------\x1b[0m"
				, TimeGetDateTime3());

			if (flag) LOG(_FL_, 0, 0, "LOGFILE = [%s]", strFile);
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
