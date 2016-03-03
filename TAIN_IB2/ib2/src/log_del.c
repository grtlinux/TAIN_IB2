/*
 * PROGRAM ID   : log_del.c
 * PROGRAM NAME : log_del 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : log_del 모듈
 * HISTORY      : 2013.11.14 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.11.14 강석. 최초작업"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.11.14 강석. 최초작업"

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
 *        char            sObjPath        [128+1];   // 대상폴더
 *        int             iExpNo                 ;   // Expire Number
 *        char            sPattern        [128+1];   // 파일검색 패턴
 *    } LOG_DEL;
 */
/*----------------------------------------------------------------------------*/

time_t       tmComp                           ;       /* 비교대상 시간 sec    */
char         sComp                       [128];       /* 비교대상 시간 문자열 */

int          iExpNo                           ;        /*     Expire Number   */

LOG_DEL      stLogDel            [MAX_LOG_DEL];          /* Log Del 정보 저장 */
int          iMaxLogDel                       ;


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
int GetCfg()
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

		char* p = CfgGetMst("ib.cfg.file.logdel");
		if (p == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : log_del 환경파일 미정의");
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
		
		iMaxLogDel = 0;

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

		/* 처리된 파일을 닫는다. */
		fclose(fp);
		
		if (flag)
		{
			/*
			 * 얻은 리스트 정보를 출력해 본다.
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
 * 내용 :
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
 * 내용 :
 *     참조함수 : asctime, ctime, gmtime, localtime, mktime
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

		tmCur =time(NULL);                     /* 현재 시각을 초단위로 얻는다. */
		localtime_r(&tmCur, &t);  /* 초단위의 시간을 분리하여 구조체에 넣는다. */
		if (!flag) PrintTm(tmCur, &t);

		tmCur = tmCur / (24 * 60 * 60) * (24 * 60 * 60);
		localtime_r(&tmCur, &t);  /* 초단위의 시간을 분리하여 구조체에 넣는다. */
		if (!flag) PrintTm(tmCur, &t);
	}

	if (flag)
	{
		time_t tmCur;
		struct tm t;

		tmCur =time(NULL);                     /* 현재 시각을 초단위로 얻는다. */
		localtime_r(&tmCur, &t);  /* 초단위의 시간을 분리하여 구조체에 넣는다. */
		if (!flag) PrintTm(tmCur, &t);

		t.tm_hour = 0;
		t.tm_min = 0;
		t.tm_sec = 0;

		tmCur = mktime(&t);
		localtime_r(&tmCur, &t);  /* 초단위의 시간을 분리하여 구조체에 넣는다. */
		if (!flag) PrintTm(tmCur, &t);

		tmCur = tmCur - (iExpNo * 24 * 60 * 60);
		localtime_r(&tmCur, &t);  /* 초단위의 시간을 분리하여 구조체에 넣는다. */
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
			if (flag) LOG(_FL_, 0, 0, "[%04d/%02d/%02d] 이전 폴더는 삭제합니다."
				, t.tm_year + 1900
				, t.tm_mon + 1
				, t.tm_mday);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int DelFolder(char* sPath)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 해당 폴더 내에 있는 파일을 삭제한다.
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

			/* 관련없는 폴더는 걸러낸다. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* sub folder에 대한 정보를 얻는다. */
			sprintf(sFile, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sFile, &st);

			if (flag) remove(sFile);
			if (flag) LOG(_FL_, 0, 0, "파일 삭제 [%s]", sFile);
		}

		closedir(pDp);
	}

	if (flag)
	{
		/*
		 * 해당 폴더를 삭제한다.
		 */

		if (flag) remove(sPath);
		if (flag) LOG(_FL_, 0, 0, "[%s] 폴더 삭제", sPath);
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int JobProc(char* sPath)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 폴더명을 이용한 폴더 삭제
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

			/* 관련없는 폴더는 걸러낸다. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* 비교일자 이후이면 넘어간다. */
			if (strcmp(sComp, pDirEnt->d_name) <= 0)
				continue;

			/* sub folder에 대한 정보를 얻는다. */
			sprintf(sSubPath, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sSubPath, &st);

			if (!flag) LOG(_FL_, 0, 0, "[%s:%d] 삭제 시작", sSubPath, st.st_mtime);

			DelFolder(sSubPath);

			if (!flag) break;
		}

		closedir(pDp);
	}

	if (!flag)
	{
		/*
		 * 폴더의 시간을 이용한 폴더 삭제
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

			/* 관련없는 폴더는 걸러낸다. */
			if (!strcmp(pDirEnt->d_name, ".") || !strcmp(pDirEnt->d_name, ".."))
				continue;

			/* sub folder에 대한 정보를 얻는다. */
			sprintf(sSubPath, "%s/%s", sPath, pDirEnt->d_name);
			lstat(sSubPath, &st);

			if (!flag) LOG(_FL_, 0, 0, "[%s:%d] %s"
				, sSubPath
				, st.st_mtime
				, (st.st_mtime < tmComp ? "삭제 시작" : "임시보관")
				);

			if (st.st_mtime < tmComp)
			{
				if (!flag) LOG(_FL_, 0, 0, "[%s:%d] 삭제 시작", sSubPath, st.st_mtime);

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
 * 내용 :
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
			/* Expire Time을 구한다. */
			if (flag) GetCompTime(stLogDel[i].iExpNo);

			/* Expire Time에 해당하는 폴더를 처리한다. */
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
		if (!flag && argc == 1 && strcmp(argv[0], "log_del") == 0)
		{
			printf("log_del 실행하지 말아 주세요.\n");
			exit(0);
		}

		argc = 2;
		argv[1] = "LOGDEL0000";
	}

	if (flag)
	{
		/*
		 * 기본 처리
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
		 * 프로세스에서 사용하는 서비스명과 FEPID를 얻는다.
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

	if (!flag)
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
