/*
 * PROGRAM ID   : MSock.c
 * PROGRAM NAME : Multi Socket ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Multi Socket ���� ���
 * HISTORY      : 2013.09.26 ����. �����ۼ�
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
#define   VERSION                  "2013.10.05.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define      MAX_ITEM                    20
#define      ALARM_SIGNAL_TIME           30

/*============================================================================*/
/*
 * global variables
 *
 *    #define     SOCK_TYPE_NOTCONNECT     0
 *    #define     SOCK_TYPE_SERVER         1
 *    #define     SOCK_TYPE_CLIENT         2
 *
 *    #define     CONN_TYPE_NOTYPE         0
 *    #define     CONN_TYPE_ACCEPT         1
 *    #define     CONN_TYPE_CONNECT        2
 *
 *    typedef struct _SOCK_INFO_
 *    {
 *        int                   type        ;     // socket type
 *        int                   conntype    ;     // connect type
 *
 *        int                   sock        ;     // socket
 *        int                   port        ;     // port
 *        char                  ipaddr[20]  ;     // ip address
 *        struct sockaddr_in    addr        ;     // socket address
 *
 *        int                   ssiNo       ;     // client���� ���. ���� ������ȣ
 *
 *        time_t                lastRecved  ;     // poll sec �����
 *        int                   iPollFlag   ;     // poll ����
 *    } SOCK_INFO;
 *
 *    typedef struct _IPPORT_INFO_
 *    {
 *        char                  sIpAddr [20];     // ip address
 *        char                  sPort   [10];     // string port
 *        int                   iPort       ;     // integer port
 *    } IPPORT_INFO;
 *
 *
 *
 */
/*----------------------------------------------------------------------------*/

#define      MAX_IPPORT_INFO            100

IPPORT_INFO  info         [MAX_IPPORT_INFO];  /* ip-port information  */

int          maxInfo                       ;

/*----------------------------------------------------------------------------*/

#define      MAX_SVR_SOCK_INFO           10
#define      MAX_CLI_SOCK_INFO          100

SOCK_INFO    ssi        [MAX_SVR_SOCK_INFO];  /* server socket info */
SOCK_INFO    csi        [MAX_CLI_SOCK_INFO];  /* client socket info */

int          maxSsi                        ;
int          maxCsi                        ;

/*============================================================================*/
/*
 * extern variables
 *
#define      POLLSET_SEC                  0
#define      POLLSEND_SEC                30
#define      TIMEOUT_SEC                 60
 */
/*----------------------------------------------------------------------------*/

int          iLLen                      = 4;  /* LENLENGTH : LEN�ʵ� ���� */
int          iLPls                      = 0;  /* LENMINUS  : LEN�ʵ忡 ���Ե� ũ��(LEN�ʵ�) */

int          iPollSendSec              = 20;  /* recv ������ Poll send �ð�(��) */
int          iTimeOutSec               = 300; /* recv ������ ����close �ð�(��) */
int          iTimeOutSock              = 0;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/*============================================================================*/
/*
 * ���� : ALARM �ý��� Handler
 */
/*----------------------------------------------------------------------------*/
void MSockAlarmHandler(int signo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	char sHdr[50];
	
	if (!flag) LOG(_FL_, 0, 0,"SIGALRM TIME OUT=[%d]", ALARM_SIGNAL_TIME);

	if (flag)
	{
		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
      		SOCK_INFO* si = (SOCK_INFO*)&csi[i]; 
      
		    if ( iTimeOutSock == si->sock ) {
		    	sprintf(sHdr, "CLOSE(%d)", i);
		    	
		    	if (flag) LOG(_FL_, 0, 0,"ALARM TIME OUT=[%d]sec (fd:%d)", ALARM_SIGNAL_TIME, si->sock); 
		    	
		    	/* sock close */	
		    	MSockCloseSockInfo((SOCK_INFO*)&csi[i]);
		    }
		    
			/* MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[i]); */
		}
	}
	
					
	if (flag)
	{
		if (!flag) LOG(_FL_, 9, 0,"SIGNAL(%d) : ���α׷��� �����մϴ�...", signo);
		if (!flag) exit(0);
	}
}

/*============================================================================*/
/*
 * ���� : ALARM �ý��� ���� �Լ�
 */
/*----------------------------------------------------------------------------*/
void MSockAlarmSet(int sec)
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
			 
			signal(SIGALRM, MSockAlarmHandler);
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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int MSockSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	int nleft, nwritten, err;
	char *buf;

	buf = (char*) msg;
	nleft = len;

    /* DATE.2014.05.07 : (JUN) alarm �߰� */
    iTimeOutSock = sock;  /* alarm sock   set   */
        
	while (nleft > 0) {
		
		errno = 0; /* errno �ʱ�ȭ */

        MSockAlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set   */
        if (flag) LOG(_FL_, 0, 0, "[SEND] ALARM SET = [%d] sec", ALARM_SIGNAL_TIME);
        
		nwritten = send(sock, buf, nleft, 0);
		
		if (nwritten < 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
			/* DATE.2014.03.07 : (JUN) errno ��� �߰�,  EAGAIN �����߰� */
	        if (flag) LOG(_FL_, 9, 0, "ERROR : SEND RETURN=[%d], errno=[%d:%s]", nwritten, errno, strerror(errno));
	        	
			/* unix */
			if (errno == EINTR || errno == EAGAIN) {
				sleep(1);
				nwritten = 0;
			} else {
				return -2;
			}
		} else if (nwritten == 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
	        if (flag) LOG(_FL_, 9, 0, "ERROR : RECV RETURN 0 by close() (fd:%d)", sock);
			if (flag) return -1;
	    } 

		nleft -= nwritten;
		buf += nwritten;
	}
	
	MSockAlarmSet(0);                  /* alarm signal clear */
	
	if (flag) LOG(_FL_, 0, 0, "[SEND] RETURN = [%d] Byte", len);

	return len;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int MSockRecv(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int nleft, nread, err;
	char* buf;

	buf = (char*) msg;
	nleft = len;

    /* DATE.2014.05.07 : (JUN) alarm �߰� */
    iTimeOutSock = sock;  /* alarm sock   set   */
    
	while (nleft > 0) {
		
		errno = 0; /* errno �ʱ�ȭ */
		
		MSockAlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set   */
        if (flag) LOG(_FL_, 0, 0, "[RECV] ALARM SET = [%d] sec", ALARM_SIGNAL_TIME);

		nread = recv(sock, buf, nleft, 0);
        
		if (nread < 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
			
			/* DATE.2014.03.07 : (JUN) errno ��� �߰� */
	        if (flag) LOG(_FL_, 9, 0, "ERROR : RECV RETURN=[%d], errno=[%d:%s] (fd:%d)", nread, errno, strerror(errno), sock);
	        	
            if (!flag) LOG(_FL_, 0, 0, "SIGSEGV signal �߻�!! (fd:%s)", sock);  /* TEST �뵵!! �ּ� ������ */

			/* unix */
			if (errno == EINTR || errno == EAGAIN) {
				nread = 0;
			} else if (errno == EWOULDBLOCK) {
				return -3;
			} else {
				return -4;
			}
		} else if (nread == 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
			
			if (!flag) break;
			
			/* DATE.2013.11.26 : (����)��������� ���� close(FIN_WAIT_1)�� �޾Ҵ�. */
			if (flag) LOG(_FL_, 9, 0, "ERROR : RECV RETURN 0 by close() (fd:%d)", sock);
			if (flag) return -1;
			
			/* �����ϰ� ó�� */
			if (!flag) sleep(1);
			if (!flag) continue;
		}

		nleft -= nread;
		buf += nread;
	}

    MSockAlarmSet(0);  /* alarm signal clear */
    if (flag) LOG(_FL_, 0, 0, "[RECV] RETURN = [%d] Byte", len - nleft);

	return (len - nleft);
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int MSockMsgSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int ret;

		ret = MSockSend(sock, msg, len);
		if (ret < 0)
			return (-1);

		return ret;
	}
}

/*============================================================================*/
/*
 * ���� : LEN�ʵ忡 ���� ó���� �ʿ�
 *    0. LEN�ʵ尪 =  (          �������ʵ�) ����  -> 4,0 : iLen, iPlus
 *    1. LEN�ʵ尪 =  (LEN�ʵ� + �������ʵ�) ����  -> 4,4
 */
/*----------------------------------------------------------------------------*/
int MSockMsgRecv(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int ret, lenData;

		if (flag)
		{
			/* LEN�ʵ带 �д´�. */
			ret = MSockRecv(sock, msg, iLLen);
			if (ret < 0)
				return -1;
            
            if (flag) LOG(_FL_, 0, 0, "[RECV] LENGTH=[%4.4s] Byte (fd:%d)", msg, sock);
		}

		if (flag)
		{
			/*
			 * DATE.2013.10.28
			 * �Ｚ��Ʈ�� �����۽�(OXSSSN0101) �̸� �������ڸ��� 'ACCEPTED...'(50)��
			 * �����ϰ� �ȴ�. �׷��� 0 �� �����Ѵ�. �׷��� ��ó���ϰ� �ȴ�.
			 */
			if (memcmp(msg, "ACCE", 4) == 0)
			{
				/* ������ ������ �ʵ带 �д´�. */
				ret = MSockRecv(sock, msg + 4, 46);
				if (ret < 0)
					return -3;

				if (flag) LOG(_FL_, 0, 0, "OXSSSN0101_�������� : [msg=%s]", msg);

				return 0;
			}
		}

		if (flag)
		{
			/*
			 * TODO DATE.2013.10.04 : �����ʵ� Validation
			 *
			 * �̺κп��� �����ڷ��� Validation ó���� �Ѵ�.
			 * ���ڰ� ���ԵǾ� ������ ����. ������ ���ڿ�
			 * Ư������ �̻��� ���̸� �����߻�. 0 or 9999 ��
			 *
			 * Time Signal ó�� : ���������� ������ ���� ���� ���� ���ϰ�
			 * Blocking �� ��� Time Signal�� �̿��Ͽ� ���õ� socket��
			 * close �ϰ� ���� ó���� �ϵ��� �Ѵ�.
			 */

			if (!flag) LOG(_FL_, 0, 0, "INFO : [msg=%*.*s]", iLLen, iLLen, msg);

			/*
			 * DATA���� = LEN�ʵ� �� - LEN�ʵ忡 ���Ե� ũ��
			 */
			lenData = UtilToIntN(msg, iLLen) - iLPls;
		}

		if (flag)
		{
			/* LEN�ʵ� ������ ������ �ʵ带 �д´�. */
			ret = MSockRecv(sock, msg + iLLen, lenData);
			if (ret < 0)
				return -3;
		}

		msg[lenData + iLLen] = 0x00;

		/* RETURN �� : (LEN�ʵ� + �������ʵ�) ���� */
		return (lenData + iLLen);
	}
}

#if defined __linux__
/*============================================================================*/
/*
 * ���� : LEN�ʵ忡 ���� ó���� �ʿ�
 *    0. LEN�ʵ尪 =  (          �������ʵ�) ����  -> 4,0 : iLen, iPlus
 *    1. LEN�ʵ尪 =  (LEN�ʵ� + �������ʵ�) ����  -> 4,4
 */
/*----------------------------------------------------------------------------*/
int MSockMsgRecv_DecPass(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int ret, lenData;

		if (flag)
		{
			/* LEN�ʵ带 �д´�. */
			ret = MSockRecv(sock, msg, iLLen);
			if (ret < 0)
				return -1;
			else if (ret == 0)
				return 0;
		}

		if (flag)
		{
			/*
			 * TODO DATE.2013.10.04 : �����ʵ� Validation
			 *
			 * �̺κп��� �����ڷ��� Validation ó���� �Ѵ�.
			 * ���ڰ� ���ԵǾ� ������ ����. ������ ���ڿ�
			 * Ư������ �̻��� ���̸� �����߻�. 0 or 9999 ��
			 *
			 * Time Signal ó�� : ���������� ������ ���� ���� ���� ���ϰ�
			 * Blocking �� ��� Time Signal�� �̿��Ͽ� ���õ� socket��
			 * close �ϰ� ���� ó���� �ϵ��� �Ѵ�.
			 */

			if (!flag) LOG(_FL_, 0, 0, "INFO : [msg=%*.*s]", iLLen, iLLen, msg);

			/*
			 * DATA���� = LEN�ʵ� �� - LEN�ʵ忡 ���Ե� ũ��
			 */
			lenData = UtilToIntN(msg, iLLen) - iLPls;
		}

		if (flag)
		{
			/* LEN�ʵ� ������ ������ �ʵ带 �д´�. */
			ret = MSockRecv(sock, msg + iLLen, lenData);
			if (ret < 0)
				return -3;
		}

		if (flag)
		{
			/*
			 * ��ȣȭ�� password�� ��ȣȭ�Ѵ�. ����:��ȣȭ���忡 NULL���ڰ� �־� ó�� ���
			 * ������������
			 *     �ŷ������ڵ�(msg + 50) �ƴϸ� skip
			 *        1110     1111
			 *        1120     1121
			 *        1130     1131
			 *     ��й�ȣ (msg + 124 : 16�ڸ�)
			 */
			char* p = msg + 50;
			if (!flag) printf("-> �ŷ������ڵ� ã�� [%-4.4s]\n", p);

			if (strncmp(p, "1110", 4) == 0 || strncmp(p, "1111", 4) == 0
			||  strncmp(p, "1120", 4) == 0 || strncmp(p, "1121", 4) == 0
			||  strncmp(p, "1130", 4) == 0 || strncmp(p, "1131", 4) == 0)
			{
				if (flag)
				{
					/* ��ȣ���ڿ��� �̵� */
					p = msg + 124;
					if (!flag)
					{
						char temp[100];
						int i;
						for(i = 0; i < 16; i++) {
							sprintf(temp + i * 5, "0x%02X ", p[i]);
						}
						printf("��ȣ�� [%s]\n", temp);
					}

					/* ����Ű�� �����Ѵ�. */
					unsigned char session_key[16] =
					{
						'1','1','1','1','1','1','1','1',
						'1','1','1','1','1','1','1','1'
					};

					/* ��ȣ���ڿ� ���� ���� */
					char decPasswd[16];
					memset(decPasswd, 0x00, sizeof(decPasswd));

					/* ��ȣȭ */
					HWSeed(session_key, 1, p, decPasswd, 2);

					/* ��ȣ���ڿ��� ������ġ�� �ű��. */
					memcpy(p, decPasswd, sizeof(decPasswd));

					if (!flag) printf("-> ��й�ȣ ã�� [%-16.16s]\n", p);
				}

				if (!flag)
				{
					/* Ȯ�ο� */
					unsigned char encPass[16] = {
						0x09, 0xA1, 0x22, 0x43, 0xAB, 0x31, 0x82, 0x3F, 0x46, 0x10, 0x26, 0x17, 0x6D, 0xF4, 0xEE, 0xAF
					};
					char temp[100];
					int i;
					for(i = 0; i < 16; i++) {
						sprintf(temp + i * 5, "0x%02X ", encPass[i]);
					}
					printf("��ȣ�� [%s]\n", temp);
				}
			}
		}

		msg[lenData + iLLen] = 0x00;

		/* RETURN �� : (LEN�ʵ� + �������ʵ�) ���� */
		return (lenData + iLLen);
	}
}
#endif
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : print socket information
 */
/*----------------------------------------------------------------------------*/
char* MSockGetTypeStr(int iType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		switch (iType)
		{
			case SOCK_TYPE_NOTCONNECT : return "SOCK_TYPE_NOTCONNECT";
			case SOCK_TYPE_SERVER     : return "SOCK_TYPE_SERVER"    ;
			case SOCK_TYPE_CLIENT     : return "SOCK_TYPE_CLIENT"    ;
		}
	}

	return "???";
}

/*============================================================================*/
/*
 * ���� : print socket information
 */
/*----------------------------------------------------------------------------*/
char* MSockGetConnTypeStr(int iType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		switch (iType)
		{
			case CONN_TYPE_NOTYPE  : return "CONN_TYPE_NOTYPE" ;
			case CONN_TYPE_ACCEPT  : return "CONN_TYPE_ACCEPT" ;
			case CONN_TYPE_CONNECT : return "CONN_TYPE_CONNECT";
		}
	}

	return "???";
}

/*============================================================================*/
/*
 * ���� : print socket information
 */
/*----------------------------------------------------------------------------*/
int MSockPrintSockItem(char* sHdr, SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag)
	{
		/*
		 * ����Ѵ�.
		 */
		printf("%s < maxSsi > = [%d]\n", sHdr, maxSsi);
		printf("%s < maxCsi > = [%d]\n", sHdr, maxCsi);
		printf("%s type       = [%s]\n", sHdr, MSockGetTypeStr(si->type));
		printf("%s conntype   = [%s]\n", sHdr, MSockGetConnTypeStr(si->conntype));
		printf("%s sock       = [%d]\n", sHdr, si->sock);
		printf("%s port       = [%d]\n", sHdr, si->port);
		printf("%s ipaddr     = [%s]\n", sHdr, si->ipaddr);
		printf("%s ssiNo      = [%d]\n", sHdr, si->ssiNo);
		printf("%s lastRecved = [%d]\n", sHdr, si->lastRecved);
		printf("%s iPollFlag  = [%d]\n", sHdr, si->iPollFlag);
		printf("%s ---------------------------\n", sHdr);
		if (flag) LOG(_FL_, 0, 0, "---------------------------");
	}
	
	if (flag)
	{
		/*
		 * ����Ѵ�.
		 */
		if (flag) LOG(_FL_, 9, 0, "---------------------------------------------");
		if (flag) LOG(_FL_, 9, 0, "%s < maxSsi > = [%d]", sHdr, maxSsi);
		if (flag) LOG(_FL_, 9, 0, "%s < maxCsi > = [%d]", sHdr, maxCsi);
		if (flag) LOG(_FL_, 9, 0, "%s type       = [%s]", sHdr, MSockGetTypeStr(si->type));
		if (flag) LOG(_FL_, 9, 0, "%s conntype   = [%s]", sHdr, MSockGetConnTypeStr(si->conntype));
        if (flag) LOG(_FL_, 9, 0, "%s sock       = [%d]", sHdr, si->sock);
		if (flag) LOG(_FL_, 9, 0, "%s port       = [%d]", sHdr, si->port);
		if (flag) LOG(_FL_, 9, 0, "%s ipaddr     = [%s]", sHdr, si->ipaddr);
		if (flag) LOG(_FL_, 9, 0, "%s ssiNo      = [%d]", sHdr, si->ssiNo);
		if (flag) LOG(_FL_, 9, 0, "%s lastRecved = [%d]", sHdr, si->lastRecved);
		if (flag) LOG(_FL_, 9, 0, "%s iPollFlag  = [%d]", sHdr, si->iPollFlag);
		if (flag) LOG(_FL_, 9, 0, "=============================================");
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : print socket information
 */
/*----------------------------------------------------------------------------*/
int MSockPrintSocketInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) printf("========== SSI =================================================\n");

	if (flag)
	{
		int i;
		for (i=0; i < maxSsi; i++)
		{
			char sHdr[50];
			sprintf(sHdr, "SSI(%d)", i);

			MSockPrintSockItem(sHdr, (SOCK_INFO*)&ssi[i]);
		}
	}

	if (flag) printf("========== CSI =================================================\n");

	if (flag)
	{
		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
			char sHdr[50];
			sprintf(sHdr, "CSI(%d)", i);

			MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[i]);
		}
	}

	if (flag) printf("========== ### =================================================\n");

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int MSockGetRandomIndex()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockGetRandomIndex [START]...");

	if (flag)
	{
		if (maxCsi == 0)
			return -1;
	}

	if (flag)
	{
		/* �迭�� ���� : 0(-) 1(+) */
		int direct = RandGetInt() % 2;

		/* ���� idx�� ��´�. */
		int idx = RandGetInt() % maxCsi;

		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
			/* accept / connect socket ��� ���� */

			if (csi[idx].type == SOCK_TYPE_CLIENT)
			{
				return idx;
			}

			if (direct)
			{
				idx ++;
				idx = idx % maxCsi;
			}
			else
			{
				idx --;
				if (idx < 0) idx = maxCsi - 1;
			}
		}
	}

	return -1;
}

/*============================================================================*/
/*
 * ���� : NOT USED
 */
/*----------------------------------------------------------------------------*/
int MSockGetSendSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockGetSendSocket [START]...");

	if (flag)
	{
		int idx = MSockGetRandomIndex();
		if (idx < 0)
		{
			return idx;
		}

		if (!flag)
		{
			char sHdr[50];
			sprintf(sHdr, "\t RANDOM CSI(%d)", idx);

			MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[idx]);
		}

		return csi[idx].sock;
	}

	return -1;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� :
 *   ���� ����(LISTEN PORT)
 *       => NULL : port1, port2, port3, port4
 *
 *   Ŭ���̾�Ʈ ����(CONNECT IP, PORT)
 *       => ip:port1, port2; ip : port3, port4
 *
 *   ���� ����(LISTEN PORT, CONNECT IP-PORT)
 *       => NULL : port1, port2; ip:port1, port2; ip : port3, port4
 */
/*----------------------------------------------------------------------------*/
int MSockParsingSockInfo(char* strInfo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockParsingSockInfo [START]...");

	if (flag)
	{
		if (flag)
		{
			/*
			 * �Է������� �м��Ͽ� ����/Ŭ���̾�Ʈ, ��Ʈ�� ���Ѵ�.
			 */
			char  strLine[1024];
			char* arr1[MAX_ITEM];
			char* arr2[MAX_ITEM];
			char* arr3[MAX_ITEM];
			int i, j, k;

			strcpy(strLine, strInfo);
			UtilStrSplit(strLine, ';', arr1);

			maxInfo = 0;

			for (i=0; arr1[i] && i < MAX_ITEM; i++)
			{
				UtilStrSplit(arr1[i], ':', arr2);

				UtilStrSplit(arr2[1], ',', arr3);

				for (k=0; arr3[k] && k < MAX_ITEM; k++)
				{
					strcpy(info[maxInfo].sIpAddr, UtilToUpper(UtilTrim(arr2[0])));
					strcpy(info[maxInfo].sPort  , UtilTrim(arr3[k]));
					info[maxInfo].iPort = UtilToInt(info[maxInfo].sPort);

					maxInfo ++;
				}
			}

			if (!flag)
			{
				/*
				 * ó���� ������ ����Ѵ�.
				 */
				int i;
				for (i=0; i < maxInfo; i++)
				{
					printf("\t info[%2d].sIpAddr = [%s]\n", i, info[i].sIpAddr);
					printf("\t info[%2d].sPort   = [%s]\n", i, info[i].sPort);
					printf("\t info[%2d].iPort   = [%d]\n", i, info[i].iPort);
					printf("\t ------------------------------\n");
				}
			}
		}

		if (flag)
		{
			/*
			 * �Ľ� ����� �����Ͽ� SOCK_INFO�� �����Ѵ�.
			 */
			maxSsi = 0;
			maxCsi = 0;

			int i;
			for (i=0; i < maxInfo; i++)
			{
				if (!flag)
				{
					if (flag) printf("\t info[%2d].sIpAddr = [%s]\n", i, info[i].sIpAddr);
					if (flag) printf("\t info[%2d].sPort   = [%s]\n", i, info[i].sPort);
					if (!flag) printf("\t info[%2d].iPort   = [%d]\n", i, info[i].iPort);
					printf("\n");
				}

				if (strcmp("NULL", info[i].sIpAddr) == 0)
				{
					/* SSI part */
					ssi[maxSsi].type     = SOCK_TYPE_SERVER;
					ssi[maxSsi].conntype = CONN_TYPE_NOTYPE;
					strcpy(ssi[maxSsi].ipaddr, info[i].sIpAddr);
					ssi[maxSsi].port = info[i].iPort;

					maxSsi ++;
				}
				else
				{
					/* CSI part, CONNECT socket */
					csi[maxCsi].type     = SOCK_TYPE_NOTCONNECT;
					csi[maxCsi].conntype = CONN_TYPE_CONNECT;
					strcpy(csi[maxCsi].ipaddr, info[i].sIpAddr);
					csi[maxCsi].port = info[i].iPort;

					maxCsi ++;
				}
			}
		}

		if (!flag)
		{
			MSockPrintSocketInfo();
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
 * ���� : ���������� �����Ѵ�. �̹� �ٸ� ���μ����� ��Ʈ���ε� �Ǿ� ������ ������ ����
 */
/*----------------------------------------------------------------------------*/
int MSockMakeServerSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockMakeServerSocket [START]...");

	if (flag)
	{
		/*
		 * ���������� ���ų�, ���������� �����Ǿ� ������
		 * �� �Լ��� �������� �ʴ´�.
		 */

		if (maxSsi == 0)
		{
			/* ���� ������ ����. */
			if (!flag) LOG(_FL_, 9, 0, "STATUS : ���������� ����. #####");
			return 0;
		}

		if (ssi[0].sock != 0)
		{
			/* �̹� ������������ �����Ǿ���. */
			if (!flag) LOG(_FL_, 9, 0, "STATUS : �̹� ������������ �����Ǿ��� #####");
			return maxSsi;
		}
	}

	if (flag)
	{
		/*
		 * ���������� �����ϱ� ���� LISTEN �� �����.
		 * ���� bind �����̸� ���α׷��� �����Ѵ�. �׸��� �̹� listening �Ǿ� �ִ�
		 * ���μ����� ã�� ó���Ѵ�. �׸��� �ٽ� ���α׷��� �����Ѵ�.
		 */

		int i;
		for (i=0; i < maxSsi && i < MAX_SVR_SOCK_INFO; i++)
		{
			int sock;
			struct sockaddr_in addr;

			if (flag)
			{
				/*
				 * socket�� �����Ѵ�
				 */
				sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (sock < 0)
				{
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * socket option�� ó���Ѵ�.
				 */
				int valOption = 1;

				setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &valOption, sizeof(valOption));
			}

			if (flag)
			{
				/*
				 * address ������ �����ϰ� bind �Ѵ�.
				 */
				memset((void*) &addr, 0x00, sizeof(struct sockaddr_in));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = htonl(INADDR_ANY);
				addr.sin_port        = htons(ssi[i].port);

				int ret = bind(sock, (struct sockaddr*) &addr, sizeof(addr));
				if (ret < 0)
				{
					if (flag) LOG(_FL_, 9, 0, "ERROR : already bind the port %d", ssi[i].port);
					if (!flag) return -1;
					if (flag) LOG(_FL_, 9, 0, "\x1b[41m---------- process EXIT_ (%s) ----------\x1b[0m", sServiceid);
					if (flag) exit(-1);
				}
			}

			if (flag)
			{
				/*
				 * listen ó���ϸ鼭 ��ٸ���.
				 */
				int ret = listen(sock, 5);
				if (ret < 0)
				{
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * ������ �������� ������ �����Ѵ�.
				 */
				/* ssi[i].type     = SOCK_TYPE_SERVER; */
				/* ssi[i].conntype = CONN_TYPE_NOTYPE; */

				ssi[i].sock = sock;
				/* ssi[i].port = ntohs(addr.sin_port); */
				strcpy(ssi[i].ipaddr, inet_ntoa(addr.sin_addr));
				memcpy((void*)&ssi[i].addr, (void*)&addr, sizeof(struct sockaddr_in));

				ssi[i].ssiNo       = -1;

				ssi[i].lastRecved  = 0;
				ssi[i].iPollFlag   = FALSE;

				if (flag)
				{
					/* ó���� ������ ����Ѵ�. */
					char sHdr[50];
					sprintf(sHdr, "LISTEN SSI(%d)", i);
					MSockPrintSockItem(sHdr, (SOCK_INFO*)&ssi[i]);
				}
			}

			if (!flag) LOG(_FL_, 9, 0, "STATUS : ����LISTEN ���� ->[LISTEN PORT:%d]"
				, ssi[i].port);
		}

		if (!flag)
		{
			MSockPrintSocketInfo();
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ACCEPT ������ �����Ѵ�.
 *        Ŭ���̾�Ʈ�� ������ ���ؼ� �����Ǹ� �� �׸�� ������ ���������� ���Ѵ�.
 *        Ŭ���̾�Ʈ�� ������ ������ �����Ѵ�. ������ ����Ǹ� �����ϰ�.
 *        accept socket�� ������ ����.
 */
/*----------------------------------------------------------------------------*/
int MSockAcceptSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockAcceptSocket [START]...");

	if (flag)
	{
		/*
		 * ���� Listening ������ �����Ѵ�.
		 */

		int ret = MSockMakeServerSocket();
		if (ret < 0)
		{
			/* �������� ������ ������ �߻���. */
			return -1;
		}
		else if (ret == 0)
		{
			/* ���������� ����. */
			return 0;
		}
	}

	if (flag)
	{
		/*
		 * Listening ���Ͽ� �����ϴ� accept socket�� �����.
		 */

		struct timeval tv;
		fd_set rfds;
		int maxfds = 0;

		if (flag)
		{
			/* select blocking �ð��� ���� */
			tv.tv_sec  = 0;
			if (flag) tv.tv_usec =   10 * 1000; /* 0.01sec */
			if (!flag) tv.tv_usec = 1000 * 1000; /* 1.00sec */
		}

		if (flag)
		{
			/* fd set�� ����� */
			FD_ZERO(&rfds);

			int i;
			for (i=0; i < maxSsi; i++)
			{
				FD_SET(ssi[i].sock, &rfds);
				maxfds = MAX(maxfds, ssi[i].sock);
			}
		}

		if (flag)
		{
			/* select�� �����Ѵ�. */
			int ret = select(maxfds+1, &rfds, NULL, NULL, &tv);
			if (ret < 0)
			{
				/* ERROR */
				return -1;
			}
			else if (ret == 0)
			{
				/* not event */
				return 0;
			}
		}
#if 0
		/* FD_ISSET ��� */
		int j;
		for (j=0; j < maxCsi ; j++)
		{
				if (flag) LOG(_FL_, 0, 0, "ACCEPT FD_ISSET STATS[%d/%d]=%d (fd:%d)", j, maxCsi, FD_ISSET(csi[j].sock, &rfds), csi[j].sock);
		}
#endif

		/* accept ó���� ���� �����Ѵ�. */
		int i;
		for (i=0; i < maxSsi; i++)
		{
			if (FD_ISSET(ssi[i].sock, &rfds))
			{
				struct sockaddr_in addr;
				unsigned int len = sizeof(addr);

				int sock = accept(ssi[i].sock, (struct sockaddr*)&addr, &len);
				if (sock < 0)
				{
					if (flag) LOG(_FL_, 9, 0, "ACCEPT SOCK ERROR=(fd:%d)", sock);
					/* ERROR */
					return -1;
				}
				
			    if (!flag) LOG(_FL_, 0, 0, "ACCEPT SOCK=(fd:%d)", sock);

				if (flag)
				{
					/*
					 * ACCEPT Client Socket ����
					 * ����ִ� SOCK_INFO�� ã�´�.
					 */

					int k;
					for (k = 0; k < MAX_CLI_SOCK_INFO; k++)
					{
						if (csi[k].conntype != CONN_TYPE_CONNECT
						&&  csi[k].type == SOCK_TYPE_NOTCONNECT)
						{
							/* CONN_TYPE_CONNECT Ÿ���� �ƴϰ�,
							   ���� ������� �ƴϸ�
							   found the socket not used */
							break;
						}
					}

					if (k == MAX_CLI_SOCK_INFO)
					{
						/* ����ִ� ������ ����. */
						return -1;
					}

					/*
					 * ACCEPT�� Ŭ���̾�Ʈ���� ������ �����Ѵ�.
					 */

					csi[k].type     = SOCK_TYPE_CLIENT;
					csi[k].conntype = CONN_TYPE_ACCEPT;

					csi[k].sock = sock;
					csi[k].port = ntohs(addr.sin_port);
					strcpy(csi[k].ipaddr, inet_ntoa(addr.sin_addr));
					memcpy((void*)&csi[k].addr, (void*)&addr, sizeof(struct sockaddr_in));

					csi[k].ssiNo       = i;

					csi[k].lastRecved  = time(NULL);  /* ���ӽð��� ����Ѵ�. */
					csi[k].iPollFlag   = FALSE;

					maxCsi = MAX(maxCsi, k+1);

					if (flag)
					{
						/* ó���� ������ ����Ѵ�. */
						char sHdr[50];
						sprintf(sHdr, "ACCEPT CSI(%d)", k);
						MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[k]);
					}
				}
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 *     ���� = ip:port1, port2; ip : port3, port4
 *     ���ǵ� connect ������ ���� ��ŭ ������ ������� ����Ѵ�.
 *     ���ǵ� ���� �̻��� ������ �þ�� �ʴ´�.
 */
/*----------------------------------------------------------------------------*/
int MSockConnectSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockConnectSocket [START]...");

	if (flag)
	{
		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
			int sock;
			struct sockaddr_in addr;

			if (flag)
			{
				if (csi[i].type == SOCK_TYPE_CLIENT
				||  csi[i].conntype != CONN_TYPE_CONNECT)
				{
					/* CONN_TYPE_CONNECT�� �ƴϰų�,
					   �̹� ������ ������ SKIP �Ѵ�. */
					continue;
				}
			}

			/*
			 * CONNECT ��������
			 * type != SOCK_TYPE_CLIENT
			 * AND
			 * conntype == CONN_TYPE_CONNECT
			 */

			if (flag)
			{
				/*
				 * socket�� �����Ѵ�
				 */
				sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (sock < 0)
				{
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * address ������ �����ϰ� bind �Ѵ�.
				 */
				memset((void*) &addr, 0x00, sizeof(struct sockaddr_in));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = inet_addr(csi[i].ipaddr);
				addr.sin_port        = htons(csi[i].port);

				static int iCntConnectFail = 0;

				int ret = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
				if (ret < 0)
				{
					if (flag)
					{
						/*
						 * CONNECT ���н� SOCKET CLOSE
						 */
						ret = close(sock);
						
						if (ret < 0 ) {
							if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE ����(fd:%d)", sock);
							return -1;
						} else {
						    if (!flag) LOG(_FL_, 0, 0, "SOCK CLOSE ����(fd:%d)", sock);			
						}
					}
	
					iCntConnectFail ++;
					if (iCntConnectFail >= 100)
					{
						/* TODO DATE.2013.11.22 : connection fail�� 100���̸� ������. */
						if (flag) LOG(_FL_, 9, 0, "ERROR : CONNECTION_FAIL-%d AND EXIT [%s:%d]"
							, iCntConnectFail, csi[i].ipaddr, csi[i].port);

						/*UmsWrite("[SVCID][MSock] CONNECTION_FAIL AND EXIT");*/
						
						if (flag) LOG(_FL_, 9, 0, "\x1b[41m---------- process EXIT_ (%s) ----------\x1b[0m", sServiceid);
						exit(-1);
					}

					if (flag)
					{
						/* TODO DATE.2013.10.14 : connection ���п� 1�� delay �� */
						if (flag) LOG(_FL_, 9, 0, "FAIL : CONNECTION_FAIL-%02d [%d:%s][%s:%d]"
							, iCntConnectFail, errno, strerror(errno), csi[i].ipaddr, csi[i].port);
						sleep(1);
					}

					return -1;
				}

				iCntConnectFail = 0;
			}

			if (flag)
			{
				/*
				 * CONNECT�� Ŭ���̾�Ʈ���� ������ �����Ѵ�.
				 */

				csi[i].type     = SOCK_TYPE_CLIENT;
				/* csi[i].conntype = CONN_TYPE_CONNECT; */

				csi[i].sock = sock;
				/* csi[i].port = ntohs(addr.sin_port); */
				/* strcpy(csi[i].ipaddr, inet_ntoa(addr.sin_addr)); */
				memcpy((void*)&csi[i].addr, (void*)&addr, sizeof(struct sockaddr_in));

				csi[i].ssiNo       = -1;
				csi[i].lastRecved  = time(NULL);  /* ���ӽð��� ����Ѵ�. */;
				csi[i].iPollFlag   = FALSE;

				if (flag)
				{
					/* ó���� ������ ����Ѵ�. */
					char sHdr[50];
					sprintf(sHdr, "CONNECT CSI(%d)", i);
					MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[i]);
				}
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
int MSockConnectSocketX25()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockConnectSocketX25 [START]...");

	if (flag)
	{
		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
			int sock;
			struct sockaddr_in addr;

			if (flag)
			{
				if (csi[i].type == SOCK_TYPE_CLIENT
				||  csi[i].conntype != CONN_TYPE_CONNECT)
				{
					/* CONN_TYPE_CONNECT�� �ƴϰų�,
					   �̹� ������ ������ SKIP �Ѵ�. */
					continue;
				}
			}

			/*
			 * CONNECT ��������
			 * type != SOCK_TYPE_CLIENT
			 * AND
			 * conntype == CONN_TYPE_CONNECT
			 */

			if (flag)
			{
				/*
				 * socket�� �����Ѵ�
				 */
				sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (sock < 0)
				{
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * address ������ �����ϰ� bind �Ѵ�.
				 */
				memset((void*) &addr, 0x00, sizeof(struct sockaddr_in));
				addr.sin_family      = AF_INET;
				addr.sin_addr.s_addr = inet_addr(csi[i].ipaddr);
				addr.sin_port        = htons(csi[i].port);

				int ret = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
				if (ret < 0)
				{
					if (flag) LOG(_FL_, 9, 0, "FAIL : CONNECTION_FAIL [%d:%s][%s:%d]"
							, errno, strerror(errno), csi[i].ipaddr, csi[i].port);
					if (flag)
					{
						/*
						 * CONNECT ���н� SOCKET CLOSE
						 */
						ret = close(sock);
						
						if (ret < 0 ) {
							if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE ����(fd:%d)", sock);
							return -1;
						} else {
						    if (flag) LOG(_FL_, 0, 0, "SOCK CLOSE ����(fd:%d)", sock);			
						}
					}
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * CONNECT�� Ŭ���̾�Ʈ���� ������ �����Ѵ�.
				 */

				csi[i].type     = SOCK_TYPE_CLIENT;
				/* csi[i].conntype = CONN_TYPE_CONNECT; */

				csi[i].sock = sock;
				/* csi[i].port = ntohs(addr.sin_port); */
				/* strcpy(csi[i].ipaddr, inet_ntoa(addr.sin_addr)); */
				memcpy((void*)&csi[i].addr, (void*)&addr, sizeof(struct sockaddr_in));

				csi[i].ssiNo       = -1;
				csi[i].lastRecved  = time(NULL);  /* ���ӽð��� ����Ѵ�. */;
				csi[i].iPollFlag   = FALSE;

				if (flag)
				{
					/* ó���� ������ ����Ѵ�. */
					char sHdr[50];
					sprintf(sHdr, "CONNECT CSI(%d)", i);
					MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[i]);
				}
			}

			if (flag)
			{
				/*
				 * MegaBox, MegaPac ������ ���� Init
				 */

				X25Init(csi[i].sock);
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 *         ���� = ip:port1, port2; ip : port3, port4
 */
/*----------------------------------------------------------------------------*/
int MSockMakeSockInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockMakeSockInfo [START]...");

	if (flag)
	{
		/*
		 * accept socket
		 */

		if (flag) MSockAcceptSocket();

		/*
		 * connect socket
		 */

		if (flag) MSockConnectSocket();
	}
}

/*============================================================================*/
/*
 * ���� : socket�� ���õ� ������ close �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MSockCloseSockInfo(SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

    int ret;
    int i=0;
    
	if (!flag) LOG(_FL_, 0, 0, "MSockCloseSockInfo [START]...");

	if (flag)
	{
		/*
		 * ������ close �Ѵ�.
		 * ���õ� ������ �̻������ �����Ѵ�.
		 */
		ret = close(si->sock);
		
		if (ret < 0 ) {
			if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE (fd:%d)", si->sock);
			return -1;
		} else {
			si->type      = SOCK_TYPE_NOTCONNECT;
		    si->iPollFlag = FALSE;		
		    if (flag) LOG(_FL_, 0, 0, "SOCK CLOSE ����(fd:%d)", si->sock);			
		}
		
		
	}
	
	if (!flag) /* 20140509 OXSKSN0201 ���� �߻� �ּ� */
	{
		for (i=0; i < maxCsi ; i++)
		{
      		SOCK_INFO* msi = (SOCK_INFO*)&csi[i]; 
      
		    if ( msi->sock == si->sock) {
		    	if (!flag) LOG(_FL_, 0, 0, "maxCsi ���� Close Sock[%d] ����", si->sock);
		    	maxCsi --;
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
 * ���� : ��� Ŭ���̾�Ʈ ������ Ȯ���ϱ� ���ؼ� ����Ѵ�.
 *     accept / connect ������ ���¸� Ȯ���ϸ� TimeOut, PollSend ó���� �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MSockCheckHandler(int (*handler) (SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockCheckHandler [START]...");

	if (flag)
	{
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		/*
		 * ��� socket�� üũ ó���Ѵ�.
		 */
		int i;
		for (i = 0; i < maxCsi; i++)
		{
			if (csi[i].type == SOCK_TYPE_CLIENT ) {
			    if (flag && csi[i].conntype == CONN_TYPE_ACCEPT)
			    {
			    	/*
			    	 * accept socket �ڵ鷯�� �����Ѵ�.
			    	 */
			    	if (handler((SOCK_INFO*)&csi[i]) < 0)
			    	{
			    		/* close */
			    		MSockCloseSockInfo((SOCK_INFO*)&csi[i]);
			    	}
			    }
			    else if (flag && csi[i].conntype == CONN_TYPE_CONNECT)
			    {
			    	/*
			    	 * connect socket �ڵ鷯�� �����Ѵ�.
			    	 */
			    	if (handler((SOCK_INFO*)&csi[i]) < 0)
			    	{
			    		/* close */
			    		MSockCloseSockInfo((SOCK_INFO*)&csi[i]);
			    	}
			    }
		    }
		}

	if (!flag) LOG(_FL_, 0, 0, "MSockCheckHandler [END]...");
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ������ ������ ������ �����Ͽ� Send ó���� �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MSockSendHandler(int (*handler) (SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockSendHandler [START]...");

	if (flag)
	{
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		/*
		 * Random ���õ� socket�� �̿��� send ó���Ѵ�.
		 */

		int idx = -1;

		if (flag)
		{
			/*
			 * �۽��� ������ �̴´�.
			 */

			idx = MSockGetRandomIndex();
			if (idx < 0)
			{
				return -1;
			}
		}

		if (flag)
		{
			switch (csi[idx].conntype)
			{
				case CONN_TYPE_ACCEPT:
					if (flag)
					{
						/*
						 * accept socket �ڵ鷯�� �����Ѵ�.
						 */
						if (handler((SOCK_INFO*)&csi[idx]) < 0)
						{
							/* close */
							MSockCloseSockInfo((SOCK_INFO*)&csi[idx]);
						}
					}
					break;

				case CONN_TYPE_CONNECT:
					if (flag)
					{
						/*
						 * connect socket �ڵ鷯�� �����Ѵ�.
						 */
						if (handler((SOCK_INFO*)&csi[idx]) < 0)
						{
							/* close */
							MSockCloseSockInfo((SOCK_INFO*)&csi[idx]);
						}
					}
					break;
			}
		}

	if (!flag) LOG(_FL_, 0, 0, "MSockSendHandler [END]...");
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : select �Լ��� ����Ͽ� ������ ������ recv �� ������ ���
 *     �װͿ� ���� ó���� �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int MSockRecvHandler(int (*handler) (SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "MSockRecvHandler [START]...");

	if (flag)
	{
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		/*
		 * ��� socket�� recv�� selectüũ�ϰ� ó���Ѵ�.
		 */

		struct timeval tv;
		fd_set rfds;
		int maxfds = 0;

		if (flag)
		{
			/* select blocking �ð��� ���� */
			tv.tv_sec  = 0;
			if (flag) tv.tv_usec =   10 * 1000; /* 0.01sec */
			if (!flag) tv.tv_usec = 1000 * 1000; /* 1.00sec */
		}


		if (flag)
		{
			/* fd set�� ����� */
			FD_ZERO(&rfds);

			int i;
			for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
			{
				if ( csi[i].type == SOCK_TYPE_CLIENT)
				{
					FD_SET(csi[i].sock, &rfds);
				    maxfds = MAX(maxfds, csi[i].sock);
			    }
			}
		}


		if (flag)
		{
			/* select�� �����Ѵ�. */
			int ret = select(maxfds+1, &rfds, NULL, NULL, &tv);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 9, 0, "ERROR select=[%d]", ret);
				/* select ERROR */
				return -1;
			}
			else if (ret == 0)
			{
				/* not event *//* Time Out */
				return 0;
			}
		}
		
#if 1
        /* FD_ISSET ��� */
		int j;
		for (j=0; j < maxCsi ; j++)
		{
				if (flag) LOG(_FL_, 0, 0, "[RECV] maxCsi[%d] FD_ISSET[%d] STATS[%d] (fd:%d)", maxCsi, j, FD_ISSET(csi[j].sock, &rfds), csi[j].sock);
		}
#endif
		/* rfds select ó���� ���� �����Ѵ�. */
		int i;
		for (i=0; i < maxCsi ; i++)
		{
			if (csi[i].type == SOCK_TYPE_CLIENT && FD_ISSET(csi[i].sock, &rfds))
			{
				if (flag) LOG(_FL_, 0, 0, "[RECV] FD_ISSET OK (fd:%d)", csi[i].sock);
				/*
				 * socket�� readable �� �����̴�. ���� �ڷᰡ �ִ�.
				 */

				if (flag)
				{
					switch (csi[i].conntype)
					{
						case CONN_TYPE_ACCEPT:
							if (flag)
							{
								/*
								 * accept socket �ڵ鷯�� �����Ѵ�.
								 */
								if (handler((SOCK_INFO*)&csi[i]) < 0)
								{
									/* close */
									MSockCloseSockInfo((SOCK_INFO*)&csi[i]);
								}
							}
							break;

						case CONN_TYPE_CONNECT:
							if (flag)
							{
								/*
								 * connect socket �ڵ鷯�� �����Ѵ�.
								 */
								if (handler((SOCK_INFO*)&csi[i]) < 0)
								{
									/* close */
									MSockCloseSockInfo((SOCK_INFO*)&csi[i]);
								}
							}
							break;
					}
				}

				if (flag)
				{
					/* PRINT : socket recv �� ó���� */
				}
			}
		}

	if (!flag) LOG(_FL_, 0, 0, "MSockRecvHandler [END]...");
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
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)

char          sSendLine       [MAX_DATA_LINE];   /* �۽ſ� �ڷ� */
char          sRecvLine       [MAX_DATA_LINE];   /* ���ſ� �ڷ� */

int           iSendLen        ;
int           iRecvLen        ;

BIZ_DAT*      pSndBizDat      = (BIZ_DAT*) &sSendLine;
BIZ_DAT*      pRcvBizDat      = (BIZ_DAT*) &sRecvLine;

FEP_BIZ_DAT*  pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*  pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

/*----------------------------------------------------------------------------*/

char          sFQLine         [MAX_DATA_LINE];   /* FQ ó���� �ڷ� */
int           iFQLen          ;

char          sDataLine       [MAX_DATA_LINE];   /* ���� ó���� �ڷ� */
int           iDataLen        ;

char          sGUID           [100];             /* Global User ID */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : TimeOut�� PollSend ó���� �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int CheckProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "CheckProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (CheckProc) ");

		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		/*
		 * [����] = [����ð�(��)] - [�ֱ� ������ �ð�(��)]
		 */
		int diff = time(NULL) - csi->lastRecved;

		if (flag)
		{
			/*
			 * [ TIME_OUT ]
			 *
			 * TimeOut �ð��� �ʰ� �Ǿ����� Ȯ���Ѵ�.
			 * extern int iTimeOutSec  ;
			 *   [����] > TimeOut(��) �̸� close, �ƴϸ� ��������.
			 */

			if (diff > iTimeOutSec)
			{
				if (flag) LOG(_FL_, 9, 0, "[%s] [TIME_OUT_CLOSE] SESSION CLOSE....(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (flag)
		{
			/*
			 * [ POLL_SEND ]
			 *
			 * PollSend �ð��� �Ǿ����� Ȯ���Ѵ�.
			 * extern int iPollSendSec  ;
			 *   [����] > iPollSendSec(��) �̸�
			 *   Poll ������ �߻��ϰ� iPollFlag = TRUE�� �Ѵ�.
			 *
			 * Poll �۽��� CONNECT�� �� ������ ������ �Ѵ�..
			 */

			if (diff >= iPollSendSec
			&&  csi->conntype   == CONN_TYPE_CONNECT
			&&  csi->iPollFlag  == FALSE)
			{
				csi->iPollFlag = TRUE;

				if (flag)
				{
					/* POLL REQ ���� */
					strcpy(sSendLine, Poll2MsgSendReq());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL REQ �۽� */
					int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 9, 0, "[%s] [POLL_SEND] SESSION CLOSE BY SEND ERROR..(%s:%d)"
							, sFepid, csi->ipaddr, csi->port);
						return -1;
					}
				}

				if (flag)
				{
					/* POLL REQ ��� */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_SEND:%d] REQ -> [%d:%s] (%s:%d)"
						, sFepid, diff, iSendLen, sSendLine, csi->ipaddr, csi->port);
				}
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
int RecvProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "RecvProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (RecvProc) ");

		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		if (flag)
		{
			/*
			 * ������ �����Ѵ�.
			 */

			iRecvLen = MSockMsgRecv(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* ���� ���� */
				if (flag) LOG(_FL_, 9, 0, "[%s] [MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* ���ű��� 0 -> ��ó�� */
				if (flag) LOG(_FL_, 9, 0, "[%s] [MSG_RECV] Length Zero(0).(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return 0;
			}
			else
			{
				/* ���� ���� */
				if (!flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
		}

		if (flag)
		{
			/*
			 * ���� Ȯ���Ѵ�.
			 *   1. POLL��û���� -> POLL����۽� -> POLL clear
			 *   2. POLL������� -> POLL clear
			 *   3. �Ϲ����� ���� -> ����ó��
			 */

			if (Poll2CmpRecvReq(sRecvLine) == 0)
			{
				/*
				 * 1. POLL��û���� -> POLL����۽� -> POLL clear
				 */

				if (flag)
				{
					/* ���� POLL REQ ��� */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- REQ [%d:%s] (%s:%d)"
						, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
				}

				if (flag)
				{
					/* POLL RES ���� */
					strcpy(sSendLine, Poll2MsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES �۽� */
					int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 9, 0, "[%s] [POLL_SEND] SESSION CLOSE BY SEND POLL-RES ERROR..(%s:%d)"
							, sFepid, csi->ipaddr, csi->port);
						return -1;
					}
				}

				if (flag)
				{
					/* �۽� POLL RES ��� */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_SEND] RES -> [%d:%s] (%s:%d)"
						, sFepid, iSendLen, sSendLine, csi->ipaddr, csi->port);
				}
			}
			else if (Poll2CmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL������� -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- RES [%d:%s] (%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
			else
			{
				/*
				 * 3. �Ϲ����� ���� -> ����ó��( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] <- [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);

				if (flag)
				{
					/*
					 * GUID�� �����Ѵ�.
					 *
					 *  1. �߻���ü(8)
					 *      �ŷ��� ó�� ���۵Ǵ� ������ �ο�
					 *      ���մܸ��� HTS�� ��� MCI �ڵ�� ������
					 *      �ٸ� ä�� �� �Ÿ������ý����� ���
					 *      ä�α����ڵ�(3) �����Ӱ� �ο��� ���ڿ�(5)
					 *  2. ��������Ͻ�(20)
					 *      ���� : YYYYMMDDhhmmssuuuuuu
					 *      �÷����� ���� ����ũ�� �����ʸ� ������ ����
					 *      ��쿡�� �ʸ� ������ ǥ����. �и� ������
					 *      ǥ���� ��쿡�� ������ 3�ڸ��� �Ʒ� �Ϸù�ȣ �ʵ忡 ���Ե�
					 *  3. �Ϸù�ȣ(4)
					 *      ������ �߻���. or ������ �Ϸù�ȣ
					 *
					 */
					sprintf(sGUID, "%-8.8s%-20.20s%04d"
						, sServiceid
						, TimeGetDateTime()
						, SeqGetNo(sFepid) % 10000);
				}

				if (flag)
				{
					/*
					 * [ ��ܱ�� ���ſ� : oxr, oxa ]
					 * BIZ_HDR + data�� �����Ѵ�.
					 */
					sprintf(sDataLine, "%04d%08d%-32.32s%-20.20s%04d%02d%01d%29.29s%s"
						, sizeof(BIZ_HDR) + iRecvLen - 4   /* sDataLength [ 4] */
						, 0                                /* sDataSeq    [ 8] */
						, sGUID                            /* sGlobalId   [32] */
						, TimeGetDateTime()                /* sDateTime   [20] */
						, 0                                /* sRespCode   [ 4] */
						, 1                                /* sMediaTp    [ 2] */
						, 1                                /* sMediaSeq   [ 1] */
						, ""                               /* sFiller     [29] */
						, sRecvLine);

					iDataLen = strlen(sDataLine);
				}

				if (!flag)
				{
					/* [ ���� ���ſ� : oir, ofr ]
					 * FEP_HDR�� �����Ѵ�.
					 * BIZ_HDR + data�� �����Ѵ�.
					 */
					strcpy(sDataLine, (char*)&pRcvFepBizDat->stBiz);
					iDataLen = strlen(sDataLine);
				}

				if (flag)
				{
					/*
					 * FQ�� ����Ѵ�.
					WriteFQ();
					 */
				}
			}

			if (flag)
			{
				/*
				 * Ŭ���̾�Ʈ�κ��� ������ ���ŵǾ����Ƿ�
				 * lastRecved �ð��� ����ð����� �����Ѵ�.
				 * Poll ó���� ���ؼ� flag�� FALSE�� �Ѵ�.
				 */
				csi->lastRecved = time(NULL);
				csi->iPollFlag = FALSE;
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
int SendProc(SOCK_INFO* csi)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SendProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (SendProc) ");

		MSockPrintSockItem(sHdr, csi);
	}

	if (flag)
	{
		/*
		 * �۽������� ����� �۽��Ѵ�.
		 *   1. 4-0 ���� �׽�Ʈ : SHC01
		 *   2. 4-4 ���� �׽�Ʈ : TAT01
		 */

		if (flag)
		{
			/*
			 * �۽������� ��´�. ( ReadFQ )
			 * ������ �׳� return �Ѵ�.
			 */
			if (RandGetInt() % 200)
			{
				return 0;
			}
		}

		if (flag)
		{
			/* 4-0 ���� */
			char* sMsg = "0086Hello world !!!! How do you do?"
				" My name TEST .......�ѱ�..    12300000 0101     218765";

			strcpy(sSendLine, sMsg);
			iSendLen = strlen(sSendLine);
		}

		if (flag)
		{
			int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 9, 0, "[%s] [MSG_SEND] SESSION CLOSE BY SEND REQ ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (flag)
		{
			/* ���� �۽� */
			if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_SEND] -> [%d:%s].(%s:%d)"
				, sFepid, iSendLen, sSendLine, csi->ipaddr, csi->port);
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	sProgName = argv[0];

	if (flag) printf("\x1b[44m< %s >\x1b[0m\n", sProgName);

	if (!flag)
	{
		struct sockaddr_in addr;

		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = inet_addr("172.30.225.53");
		addr.sin_port        = htons(1234);

		printf("IP = [%s]\n", inet_ntoa(addr.sin_addr));
		printf("PORT = [%d]\n", ntohs(addr.sin_port));

		addr.sin_family      = AF_INET;
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_port        = htons(4321);

		printf("IP = [%s]\n", inet_ntoa(addr.sin_addr));
		printf("PORT = [%d]\n", ntohs(addr.sin_port));
	}

	if (!flag)
	{
		if (flag) MSockParsingSockInfo(" null: 6002, 6003  , 6004   ;127.0.0.1 : 5002, 5003 , 5002 , 5001;  null : 6001; 127.0.0.1 : 7001,   7002,   7003    ");

		if (flag) MSockMakeSockInfo();
	}

	if (flag && argc == 2)
	{
		if (toupper(argv[1][0]) == 'S')
		{
			/* Server ���� ���� parsing */
			if (!flag) MSockParsingSockInfo(" null: 60002, 60003");
			if (flag) MSockParsingSockInfo(" null: 60002");
		}
		else
		{
			/* Client ���� ���� parsing */
			if (flag) MSockParsingSockInfo("127.0.0.1 : 60002, 60003");
			if (!flag) MSockParsingSockInfo("127.0.0.1 : 60002");
		}

		CfgMst();
		CfgPoll();

		/*-------------------- Proc Loop ---------------------*/

		while (TRUE)
		{
			/* server / accept / connect ������ �����ϰ� ������ �����Ѵ�. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect ������ üũ�Ѵ�. TimeOut, Poll üũ */
			if (flag) MSockCheckHandler(CheckProc);

			/* ������ send ó���� �ٷ�� handler */
			if (flag) MSockSendHandler(SendProc);

			/* ������ recv ó���� �ٷ�� handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (!flag) sleep(5);
			if (flag) usleep(100 * 1000);
		}
	}
}

#endif
