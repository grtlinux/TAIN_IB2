/*
 * PROGRAM ID   : MSock.c
 * PROGRAM NAME : Multi Socket 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Multi Socket 관련 모듈
 * HISTORY      : 2013.09.26 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: 최초작업"
#define   VERSION                  "2013.10.05.001: 최초작업"
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
 *        int                   ssiNo       ;     // client에서 사용. 연결 서버번호
 *
 *        time_t                lastRecved  ;     // poll sec 저장용
 *        int                   iPollFlag   ;     // poll 상태
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

int          iLLen                      = 4;  /* LENLENGTH : LEN필드 길이 */
int          iLPls                      = 0;  /* LENMINUS  : LEN필드에 포함된 크기(LEN필드) */

int          iPollSendSec              = 20;  /* recv 없으면 Poll send 시간(초) */
int          iTimeOutSec               = 300; /* recv 없으면 세션close 시간(초) */
int          iTimeOutSock              = 0;

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/


/*============================================================================*/
/*
 * 내용 : ALARM 시스널 Handler
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
		if (!flag) LOG(_FL_, 9, 0,"SIGNAL(%d) : 프로그램을 종료합니다...", signo);
		if (!flag) exit(0);
	}
}

/*============================================================================*/
/*
 * 내용 : ALARM 시스널 세팅 함수
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
			 * alarm signal handler를 세팅한다.
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
 * 내용 :
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

    /* DATE.2014.05.07 : (JUN) alarm 추가 */
    iTimeOutSock = sock;  /* alarm sock   set   */
        
	while (nleft > 0) {
		
		errno = 0; /* errno 초기화 */

        MSockAlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set   */
        if (flag) LOG(_FL_, 0, 0, "[SEND] ALARM SET = [%d] sec", ALARM_SIGNAL_TIME);
        
		nwritten = send(sock, buf, nleft, 0);
		
		if (nwritten < 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
			/* DATE.2014.03.07 : (JUN) errno 출력 추가,  EAGAIN 조건추가 */
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
 * 내용 :
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

    /* DATE.2014.05.07 : (JUN) alarm 추가 */
    iTimeOutSock = sock;  /* alarm sock   set   */
    
	while (nleft > 0) {
		
		errno = 0; /* errno 초기화 */
		
		MSockAlarmSet(ALARM_SIGNAL_TIME);  /* alarm signal set   */
        if (flag) LOG(_FL_, 0, 0, "[RECV] ALARM SET = [%d] sec", ALARM_SIGNAL_TIME);

		nread = recv(sock, buf, nleft, 0);
        
		if (nread < 0) {
			MSockAlarmSet(0);  /* alarm signal clear */
			
			/* DATE.2014.03.07 : (JUN) errno 출력 추가 */
	        if (flag) LOG(_FL_, 9, 0, "ERROR : RECV RETURN=[%d], errno=[%d:%s] (fd:%d)", nread, errno, strerror(errno), sock);
	        	
            if (!flag) LOG(_FL_, 0, 0, "SIGSEGV signal 발생!! (fd:%s)", sock);  /* TEST 용도!! 주석 해제시 */

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
			
			/* DATE.2013.11.26 : (수정)상대편으로 부터 close(FIN_WAIT_1)을 받았다. */
			if (flag) LOG(_FL_, 9, 0, "ERROR : RECV RETURN 0 by close() (fd:%d)", sock);
			if (flag) return -1;
			
			/* 무시하고 처리 */
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
 * 내용 :
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
 * 내용 : LEN필드에 대한 처리가 필요
 *    0. LEN필드값 =  (          데이터필드) 길이  -> 4,0 : iLen, iPlus
 *    1. LEN필드값 =  (LEN필드 + 데이터필드) 길이  -> 4,4
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
			/* LEN필드를 읽는다. */
			ret = MSockRecv(sock, msg, iLLen);
			if (ret < 0)
				return -1;
            
            if (flag) LOG(_FL_, 0, 0, "[RECV] LENGTH=[%4.4s] Byte (fd:%d)", msg, sock);
		}

		if (flag)
		{
			/*
			 * DATE.2013.10.28
			 * 삼성네트웍 전문송신(OXSSSN0101) 이면 접속하자마자 'ACCEPTED...'(50)을
			 * 수신하게 된다. 그래서 0 를 리턴한다. 그러면 미처리하게 된다.
			 */
			if (memcmp(msg, "ACCE", 4) == 0)
			{
				/* 다음의 데이터 필드를 읽는다. */
				ret = MSockRecv(sock, msg + 4, 46);
				if (ret < 0)
					return -3;

				if (flag) LOG(_FL_, 0, 0, "OXSSSN0101_접속전문 : [msg=%s]", msg);

				return 0;
			}
		}

		if (flag)
		{
			/*
			 * TODO DATE.2013.10.04 : 길이필드 Validation
			 *
			 * 이부분에서 수신자료의 Validation 처리를 한다.
			 * 문자가 포함되어 있으면 에러. 숫자형 문자열
			 * 특정길이 이상의 값이면 에러발생. 0 or 9999 등
			 *
			 * Time Signal 처리 : 수신전문의 오류로 인해 수신 받지 못하고
			 * Blocking 된 경우 Time Signal을 이용하여 관련된 socket을
			 * close 하고 다음 처리를 하도록 한다.
			 */

			if (!flag) LOG(_FL_, 0, 0, "INFO : [msg=%*.*s]", iLLen, iLLen, msg);

			/*
			 * DATA길이 = LEN필드 값 - LEN필드에 포함된 크기
			 */
			lenData = UtilToIntN(msg, iLLen) - iLPls;
		}

		if (flag)
		{
			/* LEN필드 다음의 데이터 필드를 읽는다. */
			ret = MSockRecv(sock, msg + iLLen, lenData);
			if (ret < 0)
				return -3;
		}

		msg[lenData + iLLen] = 0x00;

		/* RETURN 값 : (LEN필드 + 데이터필드) 길이 */
		return (lenData + iLLen);
	}
}

#if defined __linux__
/*============================================================================*/
/*
 * 내용 : LEN필드에 대한 처리가 필요
 *    0. LEN필드값 =  (          데이터필드) 길이  -> 4,0 : iLen, iPlus
 *    1. LEN필드값 =  (LEN필드 + 데이터필드) 길이  -> 4,4
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
			/* LEN필드를 읽는다. */
			ret = MSockRecv(sock, msg, iLLen);
			if (ret < 0)
				return -1;
			else if (ret == 0)
				return 0;
		}

		if (flag)
		{
			/*
			 * TODO DATE.2013.10.04 : 길이필드 Validation
			 *
			 * 이부분에서 수신자료의 Validation 처리를 한다.
			 * 문자가 포함되어 있으면 에러. 숫자형 문자열
			 * 특정길이 이상의 값이면 에러발생. 0 or 9999 등
			 *
			 * Time Signal 처리 : 수신전문의 오류로 인해 수신 받지 못하고
			 * Blocking 된 경우 Time Signal을 이용하여 관련된 socket을
			 * close 하고 다음 처리를 하도록 한다.
			 */

			if (!flag) LOG(_FL_, 0, 0, "INFO : [msg=%*.*s]", iLLen, iLLen, msg);

			/*
			 * DATA길이 = LEN필드 값 - LEN필드에 포함된 크기
			 */
			lenData = UtilToIntN(msg, iLLen) - iLPls;
		}

		if (flag)
		{
			/* LEN필드 다음의 데이터 필드를 읽는다. */
			ret = MSockRecv(sock, msg + iLLen, lenData);
			if (ret < 0)
				return -3;
		}

		if (flag)
		{
			/*
			 * 암호화된 password를 복호화한다. 이유:암호화문장에 NULL문자가 있어 처리 곤란
			 * 받은데이터의
			 *     거래구분코드(msg + 50) 아니면 skip
			 *        1110     1111
			 *        1120     1121
			 *        1130     1131
			 *     비밀번호 (msg + 124 : 16자리)
			 */
			char* p = msg + 50;
			if (!flag) printf("-> 거래구분코드 찾기 [%-4.4s]\n", p);

			if (strncmp(p, "1110", 4) == 0 || strncmp(p, "1111", 4) == 0
			||  strncmp(p, "1120", 4) == 0 || strncmp(p, "1121", 4) == 0
			||  strncmp(p, "1130", 4) == 0 || strncmp(p, "1131", 4) == 0)
			{
				if (flag)
				{
					/* 암호문자열로 이동 */
					p = msg + 124;
					if (!flag)
					{
						char temp[100];
						int i;
						for(i = 0; i < 16; i++) {
							sprintf(temp + i * 5, "0x%02X ", p[i]);
						}
						printf("암호문 [%s]\n", temp);
					}

					/* 세션키를 선언한다. */
					unsigned char session_key[16] =
					{
						'1','1','1','1','1','1','1','1',
						'1','1','1','1','1','1','1','1'
					};

					/* 복호문자열 저장 변수 */
					char decPasswd[16];
					memset(decPasswd, 0x00, sizeof(decPasswd));

					/* 복호화 */
					HWSeed(session_key, 1, p, decPasswd, 2);

					/* 복호문자열을 원래위치로 옮긴다. */
					memcpy(p, decPasswd, sizeof(decPasswd));

					if (!flag) printf("-> 비밀번호 찾기 [%-16.16s]\n", p);
				}

				if (!flag)
				{
					/* 확인용 */
					unsigned char encPass[16] = {
						0x09, 0xA1, 0x22, 0x43, 0xAB, 0x31, 0x82, 0x3F, 0x46, 0x10, 0x26, 0x17, 0x6D, 0xF4, 0xEE, 0xAF
					};
					char temp[100];
					int i;
					for(i = 0; i < 16; i++) {
						sprintf(temp + i * 5, "0x%02X ", encPass[i]);
					}
					printf("암호문 [%s]\n", temp);
				}
			}
		}

		msg[lenData + iLLen] = 0x00;

		/* RETURN 값 : (LEN필드 + 데이터필드) 길이 */
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
 * 내용 : print socket information
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
 * 내용 : print socket information
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
 * 내용 : print socket information
 */
/*----------------------------------------------------------------------------*/
int MSockPrintSockItem(char* sHdr, SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag)
	{
		/*
		 * 출력한다.
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
		 * 출력한다.
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
 * 내용 : print socket information
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
 * 내용 :
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
		/* 배열의 방향 : 0(-) 1(+) */
		int direct = RandGetInt() % 2;

		/* 난수 idx를 얻는다. */
		int idx = RandGetInt() % maxCsi;

		int i;
		for (i=0; i < maxCsi && i < MAX_CLI_SOCK_INFO; i++)
		{
			/* accept / connect socket 모두 포함 */

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
 * 내용 : NOT USED
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
 * 내용 :
 *   서버 정보(LISTEN PORT)
 *       => NULL : port1, port2, port3, port4
 *
 *   클라이언트 정보(CONNECT IP, PORT)
 *       => ip:port1, port2; ip : port3, port4
 *
 *   통합 정보(LISTEN PORT, CONNECT IP-PORT)
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
			 * 입력정보를 분석하여 서버/클라이언트, 포트를 정한다.
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
				 * 처리된 내용을 출력한다.
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
			 * 파싱 결과를 정리하여 SOCK_INFO에 저장한다.
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
 * 내용 : 서버소켓을 생성한다. 이미 다른 프로세스가 포트바인드 되어 있으면 에러로 종료
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
		 * 서버소켓이 없거나, 서버소켓이 구성되어 있으면
		 * 이 함수는 실행하지 않는다.
		 */

		if (maxSsi == 0)
		{
			/* 서버 소켓이 없다. */
			if (!flag) LOG(_FL_, 9, 0, "STATUS : 서버소켓이 없다. #####");
			return 0;
		}

		if (ssi[0].sock != 0)
		{
			/* 이미 서버리스닝은 구성되었다. */
			if (!flag) LOG(_FL_, 9, 0, "STATUS : 이미 서버리스닝은 구성되었다 #####");
			return maxSsi;
		}
	}

	if (flag)
	{
		/*
		 * 서버소켓을 구성하기 위해 LISTEN 을 만든다.
		 * 만일 bind 에러이면 프로그램을 종료한다. 그리고 이미 listening 되어 있는
		 * 프로세스를 찾아 처리한다. 그리고 다시 프로그램을 실행한다.
		 */

		int i;
		for (i=0; i < maxSsi && i < MAX_SVR_SOCK_INFO; i++)
		{
			int sock;
			struct sockaddr_in addr;

			if (flag)
			{
				/*
				 * socket을 생성한다
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
				 * socket option을 처리한다.
				 */
				int valOption = 1;

				setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &valOption, sizeof(valOption));
			}

			if (flag)
			{
				/*
				 * address 정보를 세팅하고 bind 한다.
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
				 * listen 처리하면서 기다린다.
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
				 * 생성한 서버소켓 정보를 저장한다.
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
					/* 처리된 정보를 출력한다. */
					char sHdr[50];
					sprintf(sHdr, "LISTEN SSI(%d)", i);
					MSockPrintSockItem(sHdr, (SOCK_INFO*)&ssi[i]);
				}
			}

			if (!flag) LOG(_FL_, 9, 0, "STATUS : 서버LISTEN 구성 ->[LISTEN PORT:%d]"
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
 * 내용 : ACCEPT 소켓을 생성한다.
 *        클라이언트의 접속을 통해서 생성되면 이 항목들 갯수는 가변항으로 변한다.
 *        클라이언트가 접속할 때마다 증가한다. 세션이 종료되면 감소하고.
 *        accept socket의 갯수는 가변.
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
		 * 서버 Listening 소켓을 구성한다.
		 */

		int ret = MSockMakeServerSocket();
		if (ret < 0)
		{
			/* 서버소켓 구성에 에러가 발생함. */
			return -1;
		}
		else if (ret == 0)
		{
			/* 서버소켓이 없다. */
			return 0;
		}
	}

	if (flag)
	{
		/*
		 * Listening 소켓에 접속하는 accept socket을 만든다.
		 */

		struct timeval tv;
		fd_set rfds;
		int maxfds = 0;

		if (flag)
		{
			/* select blocking 시간을 세팅 */
			tv.tv_sec  = 0;
			if (flag) tv.tv_usec =   10 * 1000; /* 0.01sec */
			if (!flag) tv.tv_usec = 1000 * 1000; /* 1.00sec */
		}

		if (flag)
		{
			/* fd set을 만든다 */
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
			/* select를 실행한다. */
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
		/* FD_ISSET 출력 */
		int j;
		for (j=0; j < maxCsi ; j++)
		{
				if (flag) LOG(_FL_, 0, 0, "ACCEPT FD_ISSET STATS[%d/%d]=%d (fd:%d)", j, maxCsi, FD_ISSET(csi[j].sock, &rfds), csi[j].sock);
		}
#endif

		/* accept 처리한 값을 세팅한다. */
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
					 * ACCEPT Client Socket 생성
					 * 비어있는 SOCK_INFO를 찾는다.
					 */

					int k;
					for (k = 0; k < MAX_CLI_SOCK_INFO; k++)
					{
						if (csi[k].conntype != CONN_TYPE_CONNECT
						&&  csi[k].type == SOCK_TYPE_NOTCONNECT)
						{
							/* CONN_TYPE_CONNECT 타입이 아니고,
							   지금 사용중이 아니면
							   found the socket not used */
							break;
						}
					}

					if (k == MAX_CLI_SOCK_INFO)
					{
						/* 비어있는 소켓이 없다. */
						return -1;
					}

					/*
					 * ACCEPT한 클라이언트소켓 정보를 저장한다.
					 */

					csi[k].type     = SOCK_TYPE_CLIENT;
					csi[k].conntype = CONN_TYPE_ACCEPT;

					csi[k].sock = sock;
					csi[k].port = ntohs(addr.sin_port);
					strcpy(csi[k].ipaddr, inet_ntoa(addr.sin_addr));
					memcpy((void*)&csi[k].addr, (void*)&addr, sizeof(struct sockaddr_in));

					csi[k].ssiNo       = i;

					csi[k].lastRecved  = time(NULL);  /* 접속시간을 기록한다. */
					csi[k].iPollFlag   = FALSE;

					maxCsi = MAX(maxCsi, k+1);

					if (flag)
					{
						/* 처리된 정보를 출력한다. */
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
 * 내용 :
 *     정보 = ip:port1, port2; ip : port3, port4
 *     정의된 connect 정보의 갯수 만큼 세션을 만드려고 노력한다.
 *     정의된 갯수 이상의 소켓은 늘어나지 않는다.
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
					/* CONN_TYPE_CONNECT가 아니거나,
					   이미 생성된 소켓은 SKIP 한다. */
					continue;
				}
			}

			/*
			 * CONNECT 실행조건
			 * type != SOCK_TYPE_CLIENT
			 * AND
			 * conntype == CONN_TYPE_CONNECT
			 */

			if (flag)
			{
				/*
				 * socket을 생성한다
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
				 * address 정보를 세팅하고 bind 한다.
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
						 * CONNECT 실패시 SOCKET CLOSE
						 */
						ret = close(sock);
						
						if (ret < 0 ) {
							if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE 실패(fd:%d)", sock);
							return -1;
						} else {
						    if (!flag) LOG(_FL_, 0, 0, "SOCK CLOSE 성공(fd:%d)", sock);			
						}
					}
	
					iCntConnectFail ++;
					if (iCntConnectFail >= 100)
					{
						/* TODO DATE.2013.11.22 : connection fail이 100번이면 종료함. */
						if (flag) LOG(_FL_, 9, 0, "ERROR : CONNECTION_FAIL-%d AND EXIT [%s:%d]"
							, iCntConnectFail, csi[i].ipaddr, csi[i].port);

						/*UmsWrite("[SVCID][MSock] CONNECTION_FAIL AND EXIT");*/
						
						if (flag) LOG(_FL_, 9, 0, "\x1b[41m---------- process EXIT_ (%s) ----------\x1b[0m", sServiceid);
						exit(-1);
					}

					if (flag)
					{
						/* TODO DATE.2013.10.14 : connection 실패와 1초 delay 줌 */
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
				 * CONNECT한 클라이언트소켓 정보를 저장한다.
				 */

				csi[i].type     = SOCK_TYPE_CLIENT;
				/* csi[i].conntype = CONN_TYPE_CONNECT; */

				csi[i].sock = sock;
				/* csi[i].port = ntohs(addr.sin_port); */
				/* strcpy(csi[i].ipaddr, inet_ntoa(addr.sin_addr)); */
				memcpy((void*)&csi[i].addr, (void*)&addr, sizeof(struct sockaddr_in));

				csi[i].ssiNo       = -1;
				csi[i].lastRecved  = time(NULL);  /* 접속시간을 기록한다. */;
				csi[i].iPollFlag   = FALSE;

				if (flag)
				{
					/* 처리된 정보를 출력한다. */
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
 * 내용 :
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
					/* CONN_TYPE_CONNECT가 아니거나,
					   이미 생성된 소켓은 SKIP 한다. */
					continue;
				}
			}

			/*
			 * CONNECT 실행조건
			 * type != SOCK_TYPE_CLIENT
			 * AND
			 * conntype == CONN_TYPE_CONNECT
			 */

			if (flag)
			{
				/*
				 * socket을 생성한다
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
				 * address 정보를 세팅하고 bind 한다.
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
						 * CONNECT 실패시 SOCKET CLOSE
						 */
						ret = close(sock);
						
						if (ret < 0 ) {
							if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE 실패(fd:%d)", sock);
							return -1;
						} else {
						    if (flag) LOG(_FL_, 0, 0, "SOCK CLOSE 성공(fd:%d)", sock);			
						}
					}
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * CONNECT한 클라이언트소켓 정보를 저장한다.
				 */

				csi[i].type     = SOCK_TYPE_CLIENT;
				/* csi[i].conntype = CONN_TYPE_CONNECT; */

				csi[i].sock = sock;
				/* csi[i].port = ntohs(addr.sin_port); */
				/* strcpy(csi[i].ipaddr, inet_ntoa(addr.sin_addr)); */
				memcpy((void*)&csi[i].addr, (void*)&addr, sizeof(struct sockaddr_in));

				csi[i].ssiNo       = -1;
				csi[i].lastRecved  = time(NULL);  /* 접속시간을 기록한다. */;
				csi[i].iPollFlag   = FALSE;

				if (flag)
				{
					/* 처리된 정보를 출력한다. */
					char sHdr[50];
					sprintf(sHdr, "CONNECT CSI(%d)", i);
					MSockPrintSockItem(sHdr, (SOCK_INFO*)&csi[i]);
				}
			}

			if (flag)
			{
				/*
				 * MegaBox, MegaPac 연결을 위한 Init
				 */

				X25Init(csi[i].sock);
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 *         정보 = ip:port1, port2; ip : port3, port4
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
 * 내용 : socket에 관련된 정보를 close 한다.
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
		 * 소켓을 close 한다.
		 * 관련된 정보도 미사용으로 세팅한다.
		 */
		ret = close(si->sock);
		
		if (ret < 0 ) {
			if (flag) LOG(_FL_, 9, 0, "SOCK CLOSE (fd:%d)", si->sock);
			return -1;
		} else {
			si->type      = SOCK_TYPE_NOTCONNECT;
		    si->iPollFlag = FALSE;		
		    if (flag) LOG(_FL_, 0, 0, "SOCK CLOSE 성공(fd:%d)", si->sock);			
		}
		
		
	}
	
	if (!flag) /* 20140509 OXSKSN0201 오류 발생 주석 */
	{
		for (i=0; i < maxCsi ; i++)
		{
      		SOCK_INFO* msi = (SOCK_INFO*)&csi[i]; 
      
		    if ( msi->sock == si->sock) {
		    	if (!flag) LOG(_FL_, 0, 0, "maxCsi 에서 Close Sock[%d] 제거", si->sock);
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
 * 내용 : 모든 클라이언트 소켓을 확인하기 위해서 사용한다.
 *     accept / connect 소켓의 상태를 확인하면 TimeOut, PollSend 처리를 한다.
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
		 * 모든 socket을 체크 처리한다.
		 */
		int i;
		for (i = 0; i < maxCsi; i++)
		{
			if (csi[i].type == SOCK_TYPE_CLIENT ) {
			    if (flag && csi[i].conntype == CONN_TYPE_ACCEPT)
			    {
			    	/*
			    	 * accept socket 핸들러를 실행한다.
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
			    	 * connect socket 핸들러를 실행한다.
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
 * 내용 : 전문을 전송할 소켓을 선택하여 Send 처리를 한다.
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
		 * Random 선택된 socket을 이용한 send 처리한다.
		 */

		int idx = -1;

		if (flag)
		{
			/*
			 * 송신할 소켓을 뽑는다.
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
						 * accept socket 핸들러를 실행한다.
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
						 * connect socket 핸들러를 실행한다.
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
 * 내용 : select 함수를 사용하여 전문을 수신한 recv 된 소켓을 얻고
 *     그것에 대한 처리를 한다.
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
		 * 모든 socket의 recv를 select체크하고 처리한다.
		 */

		struct timeval tv;
		fd_set rfds;
		int maxfds = 0;

		if (flag)
		{
			/* select blocking 시간을 세팅 */
			tv.tv_sec  = 0;
			if (flag) tv.tv_usec =   10 * 1000; /* 0.01sec */
			if (!flag) tv.tv_usec = 1000 * 1000; /* 1.00sec */
		}


		if (flag)
		{
			/* fd set을 만든다 */
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
			/* select를 실행한다. */
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
        /* FD_ISSET 출력 */
		int j;
		for (j=0; j < maxCsi ; j++)
		{
				if (flag) LOG(_FL_, 0, 0, "[RECV] maxCsi[%d] FD_ISSET[%d] STATS[%d] (fd:%d)", maxCsi, j, FD_ISSET(csi[j].sock, &rfds), csi[j].sock);
		}
#endif
		/* rfds select 처리한 값을 세팅한다. */
		int i;
		for (i=0; i < maxCsi ; i++)
		{
			if (csi[i].type == SOCK_TYPE_CLIENT && FD_ISSET(csi[i].sock, &rfds))
			{
				if (flag) LOG(_FL_, 0, 0, "[RECV] FD_ISSET OK (fd:%d)", csi[i].sock);
				/*
				 * socket이 readable 한 상태이다. 읽을 자료가 있다.
				 */

				if (flag)
				{
					switch (csi[i].conntype)
					{
						case CONN_TYPE_ACCEPT:
							if (flag)
							{
								/*
								 * accept socket 핸들러를 실행한다.
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
								 * connect socket 핸들러를 실행한다.
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
					/* PRINT : socket recv 가 처리됨 */
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

char          sSendLine       [MAX_DATA_LINE];   /* 송신용 자료 */
char          sRecvLine       [MAX_DATA_LINE];   /* 수신용 자료 */

int           iSendLen        ;
int           iRecvLen        ;

BIZ_DAT*      pSndBizDat      = (BIZ_DAT*) &sSendLine;
BIZ_DAT*      pRcvBizDat      = (BIZ_DAT*) &sRecvLine;

FEP_BIZ_DAT*  pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*  pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

/*----------------------------------------------------------------------------*/

char          sFQLine         [MAX_DATA_LINE];   /* FQ 처리용 자료 */
int           iFQLen          ;

char          sDataLine       [MAX_DATA_LINE];   /* 내부 처리용 자료 */
int           iDataLen        ;

char          sGUID           [100];             /* Global User ID */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * 내용 : TimeOut과 PollSend 처리를 한다.
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
		 * [차이] = [현재시간(초)] - [최근 수신한 시간(초)]
		 */
		int diff = time(NULL) - csi->lastRecved;

		if (flag)
		{
			/*
			 * [ TIME_OUT ]
			 *
			 * TimeOut 시간이 초과 되었는지 확인한다.
			 * extern int iTimeOutSec  ;
			 *   [차이] > TimeOut(초) 이면 close, 아니면 지나간다.
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
			 * PollSend 시간이 되었는지 확인한다.
			 * extern int iPollSendSec  ;
			 *   [차이] > iPollSendSec(초) 이면
			 *   Poll 전문을 발생하고 iPollFlag = TRUE로 한다.
			 *
			 * Poll 송신은 CONNECT한 놈만 보내는 것으로 한다..
			 */

			if (diff >= iPollSendSec
			&&  csi->conntype   == CONN_TYPE_CONNECT
			&&  csi->iPollFlag  == FALSE)
			{
				csi->iPollFlag = TRUE;

				if (flag)
				{
					/* POLL REQ 생성 */
					strcpy(sSendLine, Poll2MsgSendReq());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL REQ 송신 */
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
					/* POLL REQ 출력 */
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
 * 내용 :
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
			 * 전문을 수신한다.
			 */

			iRecvLen = MSockMsgRecv(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* 수신 에러 */
				if (flag) LOG(_FL_, 9, 0, "[%s] [MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* 수신길이 0 -> 미처리 */
				if (flag) LOG(_FL_, 9, 0, "[%s] [MSG_RECV] Length Zero(0).(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return 0;
			}
			else
			{
				/* 정상 수신 */
				if (!flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
		}

		if (flag)
		{
			/*
			 * 전문 확인한다.
			 *   1. POLL요청수신 -> POLL응답송신 -> POLL clear
			 *   2. POLL응답수신 -> POLL clear
			 *   3. 일반전문 수신 -> 수신처리
			 */

			if (Poll2CmpRecvReq(sRecvLine) == 0)
			{
				/*
				 * 1. POLL요청수신 -> POLL응답송신 -> POLL clear
				 */

				if (flag)
				{
					/* 수신 POLL REQ 출력 */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- REQ [%d:%s] (%s:%d)"
						, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
				}

				if (flag)
				{
					/* POLL RES 생성 */
					strcpy(sSendLine, Poll2MsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES 송신 */
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
					/* 송신 POLL RES 출력 */
					if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_SEND] RES -> [%d:%s] (%s:%d)"
						, sFepid, iSendLen, sSendLine, csi->ipaddr, csi->port);
				}
			}
			else if (Poll2CmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL응답수신 -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- RES [%d:%s] (%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
			else
			{
				/*
				 * 3. 일반전문 수신 -> 수신처리( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] <- [%d:%s].(%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);

				if (flag)
				{
					/*
					 * GUID를 생성한다.
					 *
					 *  1. 발생주체(8)
					 *      거래가 처음 시작되는 곳에서 부여
					 *      통합단말과 HTS의 경우 MCI 핸들로 통일함
					 *      다른 채널 및 매매지원시스템의 경우
					 *      채널구분코드(3) 자유롭게 부여한 문자열(5)
					 *  2. 생성년월일시(20)
					 *      형식 : YYYYMMDDhhmmssuuuuuu
					 *      플랫폼에 따라 마이크로 단위초를 구하지 못할
					 *      경우에는 필리 단위로 표시함. 밀리 단위로
					 *      표시할 경우에는 마지막 3자리는 아래 일련번호 필드에 포함됨
					 *  3. 일련번호(4)
					 *      난수로 발생함. or 유일한 일련번호
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
					 * [ 대외기관 수신용 : oxr, oxa ]
					 * BIZ_HDR + data를 세팅한다.
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
					/* [ 내부 수신용 : oir, ofr ]
					 * FEP_HDR를 제거한다.
					 * BIZ_HDR + data를 구성한다.
					 */
					strcpy(sDataLine, (char*)&pRcvFepBizDat->stBiz);
					iDataLen = strlen(sDataLine);
				}

				if (flag)
				{
					/*
					 * FQ에 기록한다.
					WriteFQ();
					 */
				}
			}

			if (flag)
			{
				/*
				 * 클라이언트로부터 전문이 수신되었으므로
				 * lastRecved 시간을 현재시간으로 세팅한다.
				 * Poll 처리를 위해서 flag를 FALSE로 한다.
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
 * 내용 :
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
		 * 송신전문을 만들어 송신한다.
		 *   1. 4-0 형식 테스트 : SHC01
		 *   2. 4-4 형식 테스트 : TAT01
		 */

		if (flag)
		{
			/*
			 * 송신전문을 얻는다. ( ReadFQ )
			 * 없으면 그냥 return 한다.
			 */
			if (RandGetInt() % 200)
			{
				return 0;
			}
		}

		if (flag)
		{
			/* 4-0 형식 */
			char* sMsg = "0086Hello world !!!! How do you do?"
				" My name TEST .......한글..    12300000 0101     218765";

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
			/* 정상 송신 */
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
 * 위에서 정의한 함수를 테스트 한다.
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
			/* Server 소켓 정보 parsing */
			if (!flag) MSockParsingSockInfo(" null: 60002, 60003");
			if (flag) MSockParsingSockInfo(" null: 60002");
		}
		else
		{
			/* Client 소켓 정보 parsing */
			if (flag) MSockParsingSockInfo("127.0.0.1 : 60002, 60003");
			if (!flag) MSockParsingSockInfo("127.0.0.1 : 60002");
		}

		CfgMst();
		CfgPoll();

		/*-------------------- Proc Loop ---------------------*/

		while (TRUE)
		{
			/* server / accept / connect 소켓을 구성하고 소켓을 생성한다. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect 소켓을 체크한다. TimeOut, Poll 체크 */
			if (flag) MSockCheckHandler(CheckProc);

			/* 소켓의 send 처리를 다루는 handler */
			if (flag) MSockSendHandler(SendProc);

			/* 소켓의 recv 처리를 다루는 handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (!flag) sleep(5);
			if (flag) usleep(100 * 1000);
		}
	}
}

#endif
