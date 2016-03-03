/*
 * PROGRAM ID   : Sock.c
 * PROGRAM NAME : 소켓관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : 소켓 처리를 다루는 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
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
 * 내용 : ALARM 시스널 Handler
 */
/*----------------------------------------------------------------------------*/
void SockAlarmHandler(int signo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "SIGNAL(%d) : 프로그램을 종료합니다...", signo);
		if (flag) exit(0);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수
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
			 * alarm signal handler를 세팅한다.
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
 * 내용 : int ret = send(cSock, sSendFepLine, iSendFepLen, 0);
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
		 * 원하는 길이만큼 socket에 자료를 쓴다.
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
					/* interrupt 발생에 대한 처리 */
					iWritten = 0;
					continue;
				}
				
				/* 세션의 종료 */
				return -1;
			}
			
			iRemain -= iWritten;
		}
	}
	
	return 1;
}

/*============================================================================*/
/*
 * 내용 : ret = recv(cSock, strData + 4, iDataLen, 0);
 */
/*----------------------------------------------------------------------------*/
int SockRecv(int sock, char* sData, int iDataLen)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		/*
		 * 원하는 길이만큼 socket에서 자료를 읽는다.
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
					/* interrupt 발생에 대한 처리 */
					iReaded = 0;
					continue;
				}
				
				/* 세션의 종료 */
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
 * 위에서 정의한 함수를 테스트 한다.
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
