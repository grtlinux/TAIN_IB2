/*
 * PROGRAM ID   : Sock.c
 * PROGRAM NAME : ���ϰ��� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : ���� ó���� �ٷ�� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 * 
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define ALARM_SIGNAL_TIME  30

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * ���� : ALARM �ý��� Handler
 */
/*----------------------------------------------------------------------------*/
void SockAlarmHandler(int signo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "SIGNAL(%d) : ���α׷��� �����մϴ�...", signo);
		if (flag) exit(0);
	}
}

/*============================================================================*/
/*
 * ���� : ALARM �ý��� ���� �Լ�
 */
/*----------------------------------------------------------------------------*/
void SockAlarmSet(int sec)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (sec > 0)
		{
			/*
			 * alarm signal handler�� �����Ѵ�.
			 */
			 
			signal(SIGALRM, SockAlarmHandler);
			alarm(sec);
		}
		else
		{
			alarm(sec);
		}
	}
}

/*============================================================================*/
/*
 * ���� : int ret = send(cSock, sSendFepLine, iSendFepLen, 0);
 */
/*----------------------------------------------------------------------------*/
int SockSend(int sock, char* sData, int iDataLen)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		if (!flag) LOG(_FL_, 0, 0,  "SockSend...[%*.*s]", iDataLen, iDataLen, sData);
		/*
		 * ���ϴ� ���̸�ŭ socket�� �ڷḦ ����.
		 */
		char* p       = sData;
		int iWritten  = 0;
		int iRemain   = iDataLen;
		
		while (iRemain)
		{
			p += iWritten;
			
			iWritten = send(sock, p, iRemain, 0);
			if (iWritten == 0)
			{
				/* 0.1 wait */
				usleep(100000);
				continue;
			}
			else if (iWritten < 0)
			{
				if (errno == EINTR)
				{
					/* interrupt �߻��� ���� ó�� */
					iWritten = 0;
					continue;
				}
				
				/* ������ ���� */
				return -1;
			}
			
			iRemain -= iWritten;
		}
	}
	
	return 1;
}

/*============================================================================*/
/*
 * ���� : ret = recv(cSock, strData + 4, iDataLen, 0);
 */
/*----------------------------------------------------------------------------*/
int SockRecv(int sock, char* sData, int iDataLen)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		/*
		 * ���ϴ� ���̸�ŭ socket���� �ڷḦ �д´�.
		 */
		char* p       = sData;
		int iReaded   = 0;
		int iRemain   = iDataLen;
		
		SockAlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set */

		while (iRemain)
		{
			p += iReaded;
			
			iReaded = recv(sock, p, iRemain, 0);
			
			if (iReaded == 0)
			{
				/* 0.1 wait */
				usleep(100000);
				continue;
			}
			else if (iReaded < 0)
			{
				if (errno == EINTR)
				{
					/* interrupt �߻��� ���� ó�� */
					iReaded = 0;
					continue;
				}
				
				/* ������ ���� */
				return -1;
			}
			
			iRemain -= iReaded;
		}

		SockAlarmSet(0);   /* alarm signal clear */

		if (!flag) LOG(_FL_, 0, 0,  "SockRecv...[%*.*s]", iDataLen, iDataLen, sData);
	}
	
	return 1;
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
	int flag = 1;
	
	sProgName = argv[0];
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
		char* buf = "1234567890abcdefghujklmnopqrstuvwxyz";
		char str[10];
		
		sprintf(str, "%5.5s", buf + 10);
		
		printf("[%s]\n", str);
		
		
		if (strcmp(str, "abcde") == 0)
		{
			printf("OK\n");
		}
		else
		{
			printf("Fail\n");
		}
	}
}

#endif
