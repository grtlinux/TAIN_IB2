/*
 * PROGRAM ID   : Exec.c
 * PROGRAM NAME : ���α׷� ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : ���α׷� ���� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : Shell ��ɾ �����Ѵ�. ������ ���� ������ ��ٸ��� �ʴ´�.
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
 * ���� : Shell ��ɾ �����Ѵ�. ������ ���� ������ ��ٸ���.
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
 * ���� : 
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
			/* child signal�� �����Ѵ�. */
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
 * ���� : 
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
			/* child signal�� �����Ѵ�. */
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
 * ���� : 
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
		
		if (!flag) bk = open("dummy", O_WRONLY | O_CREAT); /* ����� ���� ���� */
		if (flag) bk = open("/dev/null", O_WRONLY | O_CREAT); /* ����� ���� ���� */
		
		/*
		 * �� �� ���� �۾��� bk�� ������ fd�� �޾ƿ´�.
		 * �� ��Ȳ���� 4���� �ȴ�.
		 * �̷� ������ �ؾ� ������ �ٸ� ���� open �� �� fd�� ���� �ʰ� �ȴ�.
		 * bk�� stdout�� �ӽ� �������̴�.
		 */
		close(bk);
		
		dup2(1, bk);   /* stdout������ bk������ ���� (�ӽ�����) */
		dup2(fd, 1);   /* fd������ stdout������ ���� */
		close(fd);     /* fd�� �ݴ´�. */
		
		printf("Hello, world..!!!!\n");   /* ���Ͽ� ����. */
		
		dup2(bk, 1);   /* bk������ stdout������ ���� (����) */
		
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
		
		p = "STDOUT : �̰��� �׽�Ʈ������ ���Ǵ� ���α׷��Դϴ�.\n";
		write(fd0, p, strlen(p));
		
		p = "stdout : �̰��� �׽�Ʈ������ ���Ǵ� ���α׷��Դϴ�.";
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
