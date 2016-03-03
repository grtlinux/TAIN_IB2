/*
 * PROGRAM ID   : CSock.c
 * PROGRAM NAME : Client Socket 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Client Socket 관련 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   MAX_SOCKET        10
#define   ALRM_WAIT          5
#define   RECONN_SEC         5


/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

SOCKET_TABLE sockTable[MAX_SOCKET];

fd_set   rFds;
fd_set   wFds;
fd_set   eFds;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockPrintSocketTable()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag)
	{
		int i;

		for (i=0; i < MAX_SOCKET; i++)
		{
			printf("%02d [%c][%5d][%s]", i, sockTable[i].chType, sockTable[i].socket, sockTable[i].strInfo);
		}

		printf("\n--------------------------------------------\n");
	}

	if (flag)
	{
		int i;

		for (i=0; i < MAX_SOCKET; i++)
		{
			char sock[50+1];
			if (sockTable[i].socket < 0)
			{
				sprintf(sock, "      ");
			}
			else
			{
				sprintf(sock, "%d%c%4d", i, sockTable[i].chType, sockTable[i].socket);
			}

			printf("[%6.6s]", sock);
		}

		printf("\n");
		
		if (!flag) LOG(_FL_, 0, 0, "--------------------------------------------");
		fflush(stdout);
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockFDZERO()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FD_ZERO(&rFds);
		FD_ZERO(&wFds);
		FD_ZERO(&eFds);
	}

	if (flag)
	{
		/*
		 * 소켓테이블을 초기화한다.
		 */

		int i;

		for (i=0; i < MAX_SOCKET; i++)
		{
			sockTable[i].chType = '.';
			sockTable[i].socket = -1;
			sprintf(sockTable[i].strInfo, "no stream");
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockFDSET(int socket, char ch)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FD_SET(socket, &rFds);
		FD_SET(socket, &wFds);
		FD_SET(socket, &eFds);
	}

	if (flag)
	{
		/*
		 * 세션이 연결된 socket을 등록한다.
		 */

		int i;
		for (i=0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == '.')
			{
				switch (ch)
				{
					/*
					 * Server Socket
					 */
					case 'S':
						sockTable[i].chType = 'S';
						sockTable[i].socket = socket;
						sprintf(sockTable[i].strInfo, "SERVER SOCKET");
						break;

					/*
					 * Client Socket
					 */
					case 'C':
						sockTable[i].chType = 'C';
						sockTable[i].socket = socket;
						sprintf(sockTable[i].strInfo, "CLIENT SOCKET");
						break;

					default :
						break;
				}

				break;
			}
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockFDCLR(int socket)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FD_CLR(socket, &rFds);
		FD_CLR(socket, &wFds);
		FD_CLR(socket, &eFds);
	}

	if (flag)
	{
		/*
		 * 해당 socket을 찾아 제거한다.
		 */

		int i;
		for (i=0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].socket == socket)
			{
				sockTable[i].chType = '.';
				sockTable[i].socket = -1;
				sprintf(sockTable[i].strInfo, "no stream");

				close(socket);

				break;
			}
		}
	}

	if (flag) CSockPrintSocketTable();

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockFDCLR_All()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 해당 socket을 찾아 제거한다.
		 */

		int i;
		for (i=0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].socket != -1)
			{
				sockTable[i].chType = '.';
				sockTable[i].socket = -1;
				sprintf(sockTable[i].strInfo, "no stream");

				close(sockTable[i].socket);
			}
		}
	}

	if (flag) CSockPrintSocketTable();

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
fd_set CSockGetFdSet(char chType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static fd_set retFds;
	FD_ZERO(&retFds);

	if (flag)
	{
		/*
		 * select를 할 fd set를 만든다.
		 */

		int i;
		for (i=0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == chType)
			{
				FD_SET(sockTable[i].socket, &retFds);
				if (!flag) printf("%d) [%c][%d]\n", i, chType, sockTable[i].socket);
			}
		}
	}

	return retFds;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockGetFdCount(char chType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int count = 0;

	if (flag)
	{
		/*
		 * chType에 해당하는 socket의 갯수를 구한다.
		 */

		int i;
		for (i=0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == chType)
			{
				count ++;
			}
		}
	}

	return count;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockGetMaxFds(char chType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int maxFds = 0;

	if (flag)
	{
		/*
		 * maxFds 값을 구한다.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == chType)
			{
				maxFds = MAX(maxFds, sockTable[i].socket);
			}
		}
	}

	return maxFds;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockSocket(char* strHostIp, int iPort, int connCount)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int count;

		for (count=0; count < connCount; count++)
		{
			int cSock;

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "Socket 생성");
				/*
				 * socket을 생성한다.
				 */
				cSock = socket(AF_INET, SOCK_STREAM, 0);
				if (cSock < 0)
				{
					LOG(_FL_, 0, 0, "ERROR: socket ....");
					return FALSE;
				}
			}

			if (flag)
			{
				/*
				 * address 정보를 세팅한다.
				 */
				struct sockaddr_in sAddr;

				memset(&sAddr, 0x00, sizeof(sAddr));
				sAddr.sin_family      = AF_INET;
				sAddr.sin_addr.s_addr = inet_addr(strHostIp);
				sAddr.sin_port        = htons(iPort);

				int i;
				for (i=0; i < 24 * 60 * 6; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "Connect() 호출");
					/*
					 * 소켓을 해당 주소로 연결
					 * 접속이 않되면 재접속을 시도한다.
					 */
					int ret = connect(cSock, (struct sockaddr*) &sAddr, sizeof(sAddr));
					if (ret < 0)
					{
						/*
						 * DATE.2013.11.18 : 100번 접속 시도후 접속이 되지 않으면
						 *     프로세스를 종료 한다. 그러면 ps_man 관리프로세스가
						 *     종료됨을 확인하여 당 프로그램을 재 실행한다.
						 */
						if (i > 100)
						{
							LOG(_FL_, 0, 0,  "ERROR: CONNECTION FAIL AND EXIT..[%d]", i);
							exit(-1);
						}
						
						if (flag) LOG(_FL_, 0, 0, "FAIL : CONNECTION_FAIL-%02d [%d:%s][%s:%d]"
							, i, errno, strerror(errno), strHostIp, iPort);
						sleep(1);
						continue;
					}

					break;
				}
			}

			if (flag)
			{
				/*
				 * fd set에 sSock을 세팅한다.
				 */
				CSockFDSET(cSock, 'C');
			}
		}
	}
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockReadable(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (CSockGetFdCount('C') == 0)
		{
			return FALSE;
		}
	}

	fd_set rFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		rFds = CSockGetFdSet('C');
		maxFds = CSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, &rFds, NULL, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0,  "ERROR: select ....");
			exit(-1);
		}
		else if (ret == 0)
		{
			return TRUE;
		}
	}

	if (flag)
	{
		/*
		 * 가능한 socket을 처리한다.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (!flag) printf("%d) [%d]\n", i, sockTable[i].socket);
				
				if (FD_ISSET(sockTable[i].socket, &rFds))
				{
					/*
					 * 자료수신처리를 하는 핸들러를 실행한다.
					 */
					handler(sockTable[i].socket);
				}
			}
		}
	}
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockReceiver(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char strData[4096];
		int iDataLen;
		int ret;

		if (flag)
		{
			/*
			 * 전문에서 길이부분을 읽는다.
			 */
			ret = recv(cSock, strData, 4, 0);
			if (ret <= 0)
			{
				if (flag) LOG(_FL_, 0, 0,  "%d) ret = %d", cSock, ret);
				CSockFDCLR(cSock);
				if (flag) CSockPrintSocketTable();
				return;
			}
		}

		iDataLen = UtilToIntN( strData, 4 );

		if (flag)
		{
			/*
			 * 전문에서 길이부분을 제외한 자료부분을 읽는다.
			 */
			ret = recv(cSock, strData + 4, iDataLen, 0);
			if (ret <= 0)
			{
				if (flag) LOG(_FL_, 0, 0,  "%d) ret = %d", cSock, ret);
				CSockFDCLR(cSock);
				if (flag) CSockPrintSocketTable();
				return;
			}
		}

		strData[4 + iDataLen] = 0x00;

		/*
		 * 수신데이터를 출력한다.
		 */
		if (flag) LOG(_FL_, 0, 0,  "%d) [%s]", cSock, strData);
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
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
void CSockWritableAlarm(int signum)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, CSockWritableAlarm);
		alarm(ALRM_WAIT);
	}

	if (!flag)
	{
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
	}

	if (flag)
	{
		char* msg = "CLIENT) Hello, server.... This is socket test program.............";

		char strData[1024];
		sprintf(strData, "%04d%s", strlen(msg), msg);

		CSockWritable( CSockSender);
	}
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockWritable(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (CSockGetFdCount('C') == 0)
		{
			return FALSE;
		}
	}

	fd_set wFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		wFds = CSockGetFdSet('C');
		maxFds = CSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, NULL, &wFds, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0,  "ERROR: select ....");
			exit(-1);
		}
		else if (ret == 0)
		{
			return TRUE;
		}
	}

	if (flag)
	{
		/*
		 * 가능한 socket을 처리한다.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &wFds))
				{
					/*
					 * 자료수신처리를 하는 핸들러를 실행한다.
					 */
					handler(sockTable[i].socket);

					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockSender(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
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
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockReadWrite(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	fd_set rFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		rFds = CSockGetFdSet('C');
		maxFds = CSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, &rFds, NULL, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0,  "ERROR: select ....");
			exit(-1);
		}
		else if (ret == 0)
		{
			return -1;
		}
	}

	if (flag)
	{
		/*
		 * 가능한 socket을 처리한다.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &rFds))
				{
					/*
					 * 자료수신처리를 하는 핸들러를 실행한다.
					 */
					int ret = handler(sockTable[i].socket);
					if (ret < 0)
					{
						CSockFDCLR(sockTable[i].socket);
					}
					return ret;
				}
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int CSockWriteRead(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	fd_set wFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		wFds = CSockGetFdSet('C');
		maxFds = CSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, NULL, &wFds, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0,  "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0,  "ERROR: select ....");
			exit(-1);
		}
		else if (ret == 0)
		{
			return -1;
		}
	}

	if (flag)
	{
		/*
		 * 가능한 socket을 처리한다.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &rFds))
				{
					/*
					 * 자료수신처리를 하는 핸들러를 실행한다.
					 */
					int ret = handler(sockTable[i].socket);
					if (ret < 0)
					{
						CSockFDCLR(sockTable[i].socket);
					}
					return ret;
				}
			}
		}
	}

	return -1;
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
	
	if (flag) LOG(_FL_, 0, 0,  "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
		/*CSockFDZERO();*/

		while (TRUE)
		{
			if (CSockGetFdCount('C') == 0)
			{
				CSockFDZERO();
				if (CSockSocket("127.0.0.1", 1234, 3) == FALSE)
				{
					sleep(RECONN_SEC);
					continue;
				}

				if (flag) CSockPrintSocketTable();

				signal(SIGALRM, *CSockWritableAlarm);
				alarm(ALRM_WAIT);
			}

			CSockReadable(CSockReceiver);

			sleep(1);
		}
	}

	if (!flag)
	{
		/*
		 * SOCKET_TABLE sockTable[MAX_SOCKET] 테스트
		 */
		CSockFDZERO();
		CSockPrintSocketTable();

		CSockFDSET(3, 'C');
		CSockPrintSocketTable();

		CSockFDSET(5, 'C');
		CSockPrintSocketTable();

		CSockFDSET(9, 'C');
		CSockPrintSocketTable();

		CSockFDCLR(3);
		CSockPrintSocketTable();

		CSockFDCLR(9);
		CSockPrintSocketTable();

		CSockFDCLR(5);
		CSockPrintSocketTable();
	}

	if (!flag)
	{
		/*
		 * 각 시스템에서 생성할 수 있는 socket의 최대값을 구한다.
		 */

		int cSock;
		int i;
		for (i=0; i < 10000; i++)
		{
			int sock = socket(AF_INET, SOCK_STREAM, 0);
			if (sock < 0)
				break;

			cSock = sock;
		}
		LOG(_FL_, 0, 0,  "%d) cSock = %d", i, cSock);
	}
}

#endif
