/*
 * PROGRAM ID   : Exec.c
 * PROGRAM NAME : 프로그램 실행 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : 프로그램 실행 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : Shell 명령어를 실행한다. 실행이 끝날 때까지 기다리지 않는다.
 */
/*----------------------------------------------------------------------------*/
int ExecShellNoWait(const char* sShlCmd)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int pid;

	if (flag)
	{
		switch (pid=fork())
		{
			case  0:   /* child process */
				if (flag)
				{
					if (execl(sShlCmd, NULL) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR : execl('%s')...", sShlCmd);
					}
				}
				break;

			case -1:   /* error */
				LOG(_FL_, 0, 0, "ERROR : fork()...");
				return -1;

			default:   /* parent process */
				LOG(_FL_, 0, 0, "SUCCESS : execl('%s')...", sShlCmd);
				break;
		}

		if (!flag)
		{
			LOG(_FL_, 0, 0, "STATUS : before wait for finishing process.");

			int status;
			waitpid(pid, &status, 0);

			LOG(_FL_, 0, 0, "STATUS : after the wait");
		}
	}

	return pid;
}

/*============================================================================*/
/*
 * 내용 : Shell 명령어를 실행한다. 실행이 끝날 때까지 기다린다.
 */
/*----------------------------------------------------------------------------*/
int ExecShellWait(const char* sShlCmd)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int pid;

	if (flag)
	{
		switch (pid=fork())
		{
			case  0:   /* child process */
				if (flag)
				{
					if (execl(sShlCmd, NULL) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR : execl('%s')...", sShlCmd);
					}
				}
				break;

			case -1:   /* error */
				LOG(_FL_, 0, 0, "ERROR : fork()...");
				return -1;

			default:   /* parent process */
				LOG(_FL_, 0, 0, "SUCCESS : execl('%s')...", sShlCmd);
				break;
		}

		if (flag)
		{
			LOG(_FL_, 0, 0, "STATUS : before wait for finishing process.");

			int status;
			waitpid(pid, &status, 0);

			LOG(_FL_, 0, 0, "STATUS : after the wait");
		}
	}

	return pid;
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
void ExecSample( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sProgName = "/hw01/ibridge/ib2/shl/fepstat2.sh";
		int pid;
		int status;

		if (flag)
		{
			/* child signal을 무시한다. */
			signal(SIGCHLD, SIG_IGN);
		}
		
		if (flag)
		{
			
		}
		
		switch (pid=fork())
		{
			case  0:   /* child process */
				if (flag)
				{
					if (execl(sProgName, NULL) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR : execl('%s')...", sProgName);
						exit(0);
					}
				}
				break;

			case -1:   /* error */
				LOG(_FL_, 0, 0, "ERROR : fork()...");
				break;

			default:   /* parent process */
				sleep(2);
				LOG(_FL_, 0, 0, "SUCCESS : execl('%s')...", sProgName);
				break;
		}

		printf("I'm waiting for the process done...\n");

		waitpid(pid, &status, 0);

		printf("child process be done..\n");
	}
}

/*============================================================================*/
/*
 * 내용 : 
 */
/*----------------------------------------------------------------------------*/
void ExecSample01( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char* sProgName = "/hw01/ibridge/ib2/shl/fepstat2.sh  aaaa   bbbbb '  ccc   dddd   eee   '   fffff ";
		int pid;
		int status;

		if (flag)
		{
			/* child signal을 무시한다. */
			signal(SIGCHLD, SIG_IGN);
		}
		
		if (flag)
		{
			
		}
		
		switch (pid=fork())
		{
			case  0:   /* child process */
				if (flag)
				{
					char buf[256];
					char* argv[20];
					strcpy(buf, sProgName);
					UtilGetArgs(buf, argv);
					int i;
					for (i=0; argv[i]; i++)
					{
						printf("(%d) [%s]\n", i, argv[i]);
					}
					
					if (flag && execv(argv[0], argv) < 0)
					{
						LOG(_FL_, 0, 0, "ERROR : execl('%s')...", sProgName);
						exit(0);
					}
				}
				break;

			case -1:   /* error */
				LOG(_FL_, 0, 0, "ERROR : fork()...");
				break;

			default:   /* parent process */
				sleep(2);
				LOG(_FL_, 0, 0, "SUCCESS : execl('%s')...", sProgName);
				break;
		}

		printf("I'm waiting for the process done...\n");

		waitpid(pid, &status, 0);

		printf("child process be done..\n");
	}
}

/*============================================================================*/
/*
 * 내용 : 
 *
 *    S_IRWXU  00700 user (file owner) has read, write and execute permission
 *    S_IRUSR  00400 user has read permission
 *    S_IWUSR  00200 user has write permission
 *    S_IXUSR  00100 user has execute permission
 *    S_IRWXG  00070 group has read, write and execute permission
 *    S_IRGRP  00040 group has read permission
 *    S_IWGRP  00020 group has write permission
 *    S_IXGRP  00010 group has execute permission
 *    S_IRWXO  00007 others have read, write and execute permission
 *    S_IROTH  00004 others have read permission
 *    S_IWOTH  00002 others have write permission
 *    S_IXOTH  00001 others have execute permission
 *    
 */
/*----------------------------------------------------------------------------*/
void ExecDupSample( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag)
	{
		int fd, bk;
		
		fd = open("zzz", O_WRONLY | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
		
		if (!flag) bk = open("dummy", O_WRONLY | O_CREAT); /* 백업용 더미 생성 */
		if (flag) bk = open("/dev/null", O_WRONLY | O_CREAT); /* 백업용 더미 생성 */
		
		/*
		 * 위 두 줄의 작업은 bk에 적당한 fd를 받아온다.
		 * 이 상황에서 4정도 된다.
		 * 이런 식으로 해야 앞으로 다른 파일 open 시 위 fd를 쓰지 않게 된다.
		 * bk는 stdout의 임시 보관소이다.
		 */
		close(bk);
		
		dup2(1, bk);   /* stdout정보을 bk정보에 복사 (임시저장) */
		dup2(fd, 1);   /* fd정보를 stdout정보에 복사 */
		close(fd);     /* fd를 닫는다. */
		
		printf("Hello, world..!!!!\n");   /* 파일에 쓴다. */
		
		dup2(bk, 1);   /* bk정보를 stdout정보에 복사 (원복) */
		
		printf("Good Luck!!!\n");
		
		if (!flag) remove("dummy");
	}
	
	if (flag)
	{
		int fd0, fd1;
		
		fd0 = dup(1);
		
		fd1 = open("zzz", O_WRONLY | O_CREAT, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
		
		dup2(fd1, 1);

		if (flag)
		{
			int i;
			for (i=0; i < 10; i++)
			{
				printf("(%03d) Hello world.....\n", i);
			}
		}
		
		dup2(fd0, 1);
		
		if (flag)
		{
			int i;
			for (i=0; i < 10; i++)
			{
				printf("(%03d) Hello world.....\n", i);
			}
		}
		
		char* p;
		
		p = "STDOUT : 이것은 테스트용으로 사용되는 프로그램입니다.\n";
		write(fd0, p, strlen(p));
		
		p = "stdout : 이것은 테스트용으로 사용되는 프로그램입니다.";
		fprintf(stdout, "> [%s]\n", p);
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
		ExecShellWait("/hw01/ibridge/ib2/shl/ftptest.sh");

		LOG(_FL_, 0, 0, "Main Program : %s finished..", sProgName);
	}

	if (!flag)
	{
		ExecShellNoWait("/hw01/ibridge/ib2/shl/ftptest.sh");

		LOG(_FL_, 0, 0, "Main Program : %s finished..", sProgName);
	}

	if (!flag)
	{
		ExecSample();
	}

	if (flag)
	{
		ExecSample01();
	}

	if (!flag)
	{
		ExecDupSample();
	}
}

#endif
