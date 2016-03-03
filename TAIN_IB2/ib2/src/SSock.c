/*
 * PROGRAM ID   : SSock.c
 * PROGRAM NAME : Server Socket 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Server Socket 관련 모듈
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
#define   ALRM_WAIT          2

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

SOCKET_TABLE sockTable[MAX_SOCKET];

int      sSock;

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
BOOL SSockPrintSocketTable()
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

		printf("\n--------------------------------------------\n");
	}

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL SSockFDZERO()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "Socket Init" );

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
BOOL SSockFDSET(int socket, char ch)
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
BOOL SSockFDCLR(int socket)
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

	if (flag) SSockPrintSocketTable();

	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
fd_set SSockGetFdSet(char chType)
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
int SSockGetFdCount(char chType)
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
int SSockGetMaxFds(char chType)
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
BOOL SSockSocket(int iPort)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "Socket Server Init" );

	if (flag)
	{
		if (flag)
		{
			/*
			 * socket을 생성한다.
			 */
			sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if (sSock < 0)
			{
				LOG(_FL_, 0, 0, "ERROR: socket ....");
				exit(-1);
			}
		}

		if (flag)
		{
			/*
			 * socket option를 처리한다.
			 */
			int valOption = 1;

			setsockopt(sSock, SOL_SOCKET, SO_REUSEADDR, &valOption, sizeof(valOption));
		}

		if (flag)
		{
			/*
			 * address 정보를 세팅한다.
			 */
			struct sockaddr_in sAddr;

			memset(&sAddr, 0x00, sizeof(sAddr));
			sAddr.sin_family      = AF_INET;
			sAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			sAddr.sin_port        = htons(iPort);

			/* 소켓을 해당 주소로 연결*/
			int ret = bind(sSock, (struct sockaddr*) &sAddr, sizeof(sAddr));
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0, "ERROR : already bind the port %d", iPort);
				if (!flag) return -1;
				if (flag) LOG(_FL_, 0, 0, "\x1b[41m---------- process EXIT_ (%s) ----------\x1b[0m", sServiceid);
				if (flag) exit(-1);
			}
		}

		if (flag)
		{
			/*
			 * listening 처리를 한다.
			 */
			int ret = listen(sSock, 5);
			if (ret < 0)
			{
				LOG(_FL_, 0, 0, "ERROR: listen ....");
				exit(-1);
			}
			
			LOG(_FL_, 0, 0, "LISTEN : port %d (fd:%d)", iPort, sSock);
		}

		if (flag)
		{
			/*
			 * fd set에 sSock을 세팅한다.
			 */
			SSockFDSET(sSock, 'S');
		}
	}
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL SSockAccept()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "ACCEPT Wait...");

	if (flag)
	{
		int cnt;

		for (cnt=0; cnt < MAX_SOCKET; cnt++)
		{
			struct timeval tv;
			fd_set rFds = SSockGetFdSet('S');

			tv.tv_sec  = 0;
			tv.tv_usec = 100;

			if (!flag) LOG(_FL_, 0, 0, "STATUS : Before select....");
			int ret = select(sSock+1, &rFds, NULL, NULL, &tv);
			if (!flag) LOG(_FL_, 0, 0, "STATUS : After select.... ret = %d ", ret);

			if (ret < 0)
			{
				LOG(_FL_, 0, 0, "ERROR: select ....");
				exit(-1);
			}
			else if (ret == 0)
			{
				return FALSE;
			}

			if (FD_ISSET(sSock, &rFds))
			{
				/*
				 * client의 접속 발생
				 */
				struct sockaddr_in cAddr;
				unsigned int cLen = sizeof(cAddr);

				if (!flag) LOG(_FL_, 0, 0, "STATUS : Before accept....");
				int cSock = accept(sSock, (struct sockaddr*)&cAddr, &cLen);
				if (!flag) LOG(_FL_, 0, 0, "STATUS : After accept....cSock = %d ", cSock);

				if (cSock < 0)
				{
					LOG(_FL_, 0, 0, "ERROR: accept ....");
					exit(-1);
				}

				SSockFDSET(cSock, 'C');
				if (flag) SSockPrintSocketTable();
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
BOOL SSockAcceptWait()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * client의 접속 발생
		 */
		struct sockaddr_in cAddr;
		unsigned int cLen = sizeof(cAddr);

		if (!flag) LOG(_FL_, 0, 0, "STATUS : Before accept....");
		int cSock = accept(sSock, (struct sockaddr*)&cAddr, &cLen);
		if (!flag) LOG(_FL_, 0, 0, "STATUS : After accept....cSock = %d ", cSock);

		if (cSock < 0)
		{
			LOG(_FL_, 0, 0, "ERROR: accept ....(fd:%d)", cSock);
			exit(-1);
		}
		if (!flag) LOG(_FL_, 0, 0, "ACCEPT SOCK=(fd:%d)", cSock);

		SSockFDSET(cSock, 'C');
		if (flag) SSockPrintSocketTable();
	}

	return TRUE;
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
BOOL SSockReadable(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	fd_set rFds;
	int maxFds;

	if (flag)
	{
		/*
		 * select 를 실행한다.
		 */

		struct timeval tv;
		rFds = SSockGetFdSet('C');
		maxFds = SSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, &rFds, NULL, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0, "ERROR: select ....");
			exit(-1);
		}
		else if (ret == 0)
		{
			return FALSE;
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
					handler(sockTable[i].socket);
				}
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
int SSockReceiver(int cSock)
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
				if (flag) LOG(_FL_, 0, 0, "%d) ret = %d", cSock, ret);
				SSockFDCLR(cSock);
				if (flag) SSockPrintSocketTable();
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
				if (flag) LOG(_FL_, 0, 0, "%d) ret = %d", cSock, ret);
				SSockFDCLR(cSock);
				if (flag) SSockPrintSocketTable();
				return;
			}
		}

		strData[4 + iDataLen] = 0x00;

		/*
		 * 수신데이터를 출력한다.
		 */
		if (flag) LOG(_FL_, 0, 0,  "%d) [%s]\n", cSock, strData);
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
void SSockWritableAlarm(int signum)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		signal(SIGALRM, SSockWritableAlarm);
		alarm(ALRM_WAIT);
	}

	if (!flag)
	{
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
		LOG(_FL_, 0, 0, "SIGNAL(ALRM)-2 : 메시지를 출력합니다....");
	}

	if (flag)
	{
		char* msg = "SERVER) Hello, client.... This is socket test program...";

		char strData[1024];
		sprintf(strData, "%04d%s", strlen(msg), msg);

		SSockWritable(SSockSender);
	}
}

/*============================================================================*/
/*
 * 내용 : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL SSockWritable(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (SSockGetFdCount('C') == 0)
		{
			return FALSE;
		}
	}

	fd_set wFds;
	int maxFds;

	if (flag)
	{
		/*
		 * select 를 실행한다.
		 */

		struct timeval tv;
		wFds = SSockGetFdSet('C');
		maxFds = SSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] Before select....", __FUNCTION__);
		int ret = select(maxFds+1, NULL, &wFds, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0, "ERROR: select ....");
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
int SSockSender(int cSock)
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
int SSockReadWrite(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	fd_set rFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		rFds = SSockGetFdSet('C');
		maxFds = SSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, &rFds, NULL, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0, "ERROR: select ....");
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
						SSockFDCLR(sockTable[i].socket);
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
int SSockWriteRead(int (*handler) (int))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	fd_set wFds;
	int maxFds;

	if (flag)
	{
		struct timeval tv;
		wFds = SSockGetFdSet('C');
		maxFds = SSockGetMaxFds('C');

		tv.tv_sec  = 0;
		tv.tv_usec = 100;

		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] Before select....maxFds=%d", __FUNCTION__, maxFds);
		int ret = select(maxFds+1, NULL, &wFds, NULL, &tv);
		if (!flag) LOG(_FL_, 0, 0, "STATUS : [%s] After select.... ret = %d, maxFds=%d ", __FUNCTION__, ret, maxFds);

		if (ret < 0)
		{
			LOG(_FL_, 0, 0, "ERROR: select ....");
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
						SSockFDCLR(sockTable[i].socket);
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
	
	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	if (flag)
	{
		SSockFDZERO();
		SSockSocket(1234);
		SSockPrintSocketTable();

		signal(SIGALRM, *SSockWritableAlarm);
		alarm(ALRM_WAIT);

		while (TRUE)
		{
			SSockAccept();

			SSockReadable(SSockReceiver);

			sleep(1);
		}
	}

	if (!flag)
	{
		/*
		 * SOCKET_TABLE sockTable[MAX_SOCKET] 테스트
		 */
		SSockFDZERO();
		SSockPrintSocketTable();

		SSockFDSET(4, 'S');
		SSockPrintSocketTable();

		SSockFDSET(3, 'C');
		SSockPrintSocketTable();

		SSockFDSET(5, 'C');
		SSockPrintSocketTable();

		SSockFDSET(9, 'C');
		SSockPrintSocketTable();

		SSockFDCLR(3);
		SSockPrintSocketTable();

		SSockFDCLR(9);
		SSockPrintSocketTable();

		SSockFDCLR(5);
		SSockPrintSocketTable();
	}

}

#endif
