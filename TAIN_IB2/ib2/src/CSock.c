/*
 * PROGRAM ID   : CSock.c
 * PROGRAM NAME : Client Socket ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Client Socket ���� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : initialize socket information
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
 * ���� : initialize socket information
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
		 * �������̺��� �ʱ�ȭ�Ѵ�.
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
 * ���� : initialize socket information
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
		 * ������ ����� socket�� ����Ѵ�.
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
 * ���� : initialize socket information
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
		 * �ش� socket�� ã�� �����Ѵ�.
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
 * ���� : initialize socket information
 */
/*----------------------------------------------------------------------------*/
BOOL CSockFDCLR_All()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �ش� socket�� ã�� �����Ѵ�.
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
 * ���� : initialize socket information
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
		 * select�� �� fd set�� �����.
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
 * ���� : initialize socket information
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
		 * chType�� �ش��ϴ� socket�� ������ ���Ѵ�.
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
 * ���� : initialize socket information
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
		 * maxFds ���� ���Ѵ�.
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
 * ���� : initialize socket information
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
				if (flag) LOG(_FL_, 0, 0, "Socket ����");
				/*
				 * socket�� �����Ѵ�.
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
				 * address ������ �����Ѵ�.
				 */
				struct sockaddr_in sAddr;

				memset(&sAddr, 0x00, sizeof(sAddr));
				sAddr.sin_family      = AF_INET;
				sAddr.sin_addr.s_addr = inet_addr(strHostIp);
				sAddr.sin_port        = htons(iPort);

				int i;
				for (i=0; i < 24 * 60 * 6; i++)
				{
					if (flag) LOG(_FL_, 0, 0, "Connect() ȣ��");
					/*
					 * ������ �ش� �ּҷ� ����
					 * ������ �ʵǸ� �������� �õ��Ѵ�.
					 */
					int ret = connect(cSock, (struct sockaddr*) &sAddr, sizeof(sAddr));
					if (ret < 0)
					{
						/*
						 * DATE.2013.11.18 : 100�� ���� �õ��� ������ ���� ������
						 *     ���μ����� ���� �Ѵ�. �׷��� ps_man �������μ�����
						 *     ������� Ȯ���Ͽ� �� ���α׷��� �� �����Ѵ�.
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
				 * fd set�� sSock�� �����Ѵ�.
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
 * ���� : initialize socket information
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
		 * ������ socket�� ó���Ѵ�.
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
					 * �ڷ����ó���� �ϴ� �ڵ鷯�� �����Ѵ�.
					 */
					handler(sockTable[i].socket);
				}
			}
		}
	}
}

/*============================================================================*/
/*
 * ���� : initialize socket information
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
			 * �������� ���̺κ��� �д´�.
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
			 * �������� ���̺κ��� ������ �ڷ�κ��� �д´�.
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
		 * ���ŵ����͸� ����Ѵ�.
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
 * ���� : initialize socket information
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
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : �޽����� ����մϴ�....");
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : �޽����� ����մϴ�....");
		LOG(_FL_, 0, 0,  "SIGNAL(ALRM)-2 : �޽����� ����մϴ�....");
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
 * ���� : initialize socket information
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
		 * ������ socket�� ó���Ѵ�.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &wFds))
				{
					/*
					 * �ڷ����ó���� �ϴ� �ڵ鷯�� �����Ѵ�.
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
 * ���� : initialize socket information
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
 * ���� : initialize socket information
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
		 * ������ socket�� ó���Ѵ�.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &rFds))
				{
					/*
					 * �ڷ����ó���� �ϴ� �ڵ鷯�� �����Ѵ�.
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
 * ���� : initialize socket information
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
		 * ������ socket�� ó���Ѵ�.
		 */

		int i;
		for (i = 0; i < MAX_SOCKET; i++)
		{
			if (sockTable[i].chType == 'C')
			{
				if (FD_ISSET(sockTable[i].socket, &rFds))
				{
					/*
					 * �ڷ����ó���� �ϴ� �ڵ鷯�� �����Ѵ�.
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
		 * SOCKET_TABLE sockTable[MAX_SOCKET] �׽�Ʈ
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
		 * �� �ý��ۿ��� ������ �� �ִ� socket�� �ִ밪�� ���Ѵ�.
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
