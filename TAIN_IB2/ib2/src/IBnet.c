/*
 * PROGRAM ID   : IBnet.c
 * PROGRAM NAME : IBnet 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : IBnet 관련 모듈
 * HISTORY      : 2013.10.21 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * HISTORY VERSION
 */
/*----------------------------------------------------------------------------*/

#define       VERSION     "2013.09.11.001: 최초작업\n"              VERSION_002
#define       VERSION_002 "2013.10.07.002: 추가작업\n"              VERSION_003
#define       VERSION_003 "2014.05.19.003: 뱅킹FEP개선작업\n"

/*============================================================================*/
/*
 * global variables
 *
 *     typedef struct
 *     {
 *         char                  sSvcId    [ 20+1];   // Service ID
 *
 *         char                  sUseFlag  [  1+1];   // Use Flag
 *
 *         char                  sNetInfo  [200+1];   // Net Socket Info
 *         char                  sIpAddr   [ 20+1];   // ip address
 *         char                  sPort     [ 10+1];   // string port
 *         int                   iPort            ;   // integer port
 *
 *         char                  sProcType [  1+1];   // Process Type
 *
 *         char                  sTimeOut  [ 10+1];   // TimeOut sec
 *         char                  sPollTime [ 10+1];   // PollTime sec
 *
 *         int                   iTimeOut         ;   // TimeOut sec
 *         int                   iPollTime        ;   // PollTime sec
 *
 *         char                  sExeInfo  [200+1];   // Execute Info
 *
 *     } IBNET_CONF;
 *
 *     #define     IBNET_TYPE_NOTCONNECT          0
 *     #define     IBNET_TYPE_SERVER              1
 *     #define     IBNET_TYPE_ACCEPT              2
 *     #define     IBNET_TYPE_CONNECT             3
 *
 *     typedef struct
 *     {
 *         int                   type             ;   // socket type
 *
 *         int                   sock             ;   // socket
 *         int                   lport            ;   // listen port
 *         int                   port             ;   // port
 *         char                  ipaddr    [ 20+1];   // ip address
 *         struct sockaddr_in    addr             ;   // socket address
 *
 *         int                   iTimeOut         ;   // TimeOut sec
 *         int                   iPollTime        ;   // PollTime sec
 *
 *         char                  sExeInfo  [200+1];   // Execute Info
 *         int                   pid              ;   // PID
 *
 *     } IBNET_SOCK_INFO;
 *
 *
 */
/*----------------------------------------------------------------------------*/

#define           MAX_ITEM               20
#define           MAX_LINE_SIZE   (10*1024)

/*----------------------------------------------------------------------------*/

#define           MAX_IBNET_CONF        100

IBNET_CONF        conf     [MAX_IBNET_CONF];  /* IBnet Conf Information  */

int               maxConf                  ;

/*----------------------------------------------------------------------------*/

#define           MAX_SERVER_SOCK       100
#define           MAX_ACCEPT_SOCK       100
#define           MAX_CONNECT_SOCK      100

IBNET_SOCK_INFO   ssi    [MAX_SERVER_SOCK ];  /* server socket info */
IBNET_SOCK_INFO   asi    [MAX_ACCEPT_SOCK ];  /* accept socket info */
IBNET_SOCK_INFO   csi    [MAX_CONNECT_SOCK];  /* connect socket info */

int               maxSsi                = 0;
int               maxAsi                = 0;
int               maxCsi                = 0;

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
int IBnetSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int nleft, nwritten, err;
	char *buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nwritten = send(sock, buf, nleft, 0);
		if (nwritten < 0) {
			/* unix */
			if (errno == EINTR)
				nwritten = 0;
			else
				return -2;
		}

		nleft -= nwritten;
		buf += nwritten;
	}

	return len;
}

/*============================================================================*/
/*
 * 내용 :
 *      TODO DATE.2013.10.21 : alarm signal 처리 필요
 */
/*----------------------------------------------------------------------------*/
int IBnetRecv(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int nleft, nread, err;
	char* buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nread = recv(sock, buf, nleft, 0);
		if (nread < 0) {
			if (flag) printf("@@@@@@@@@ ERROR : recv return -1\n");

			/* unix */
			if (errno == EINTR)
				nread = 0;
			else if (errno == EWOULDBLOCK)
				return -3;
			else
				return -4;
		}
		else if (nread == 0) {
			if (!flag) printf("@@@@@@@@@ ERROR : recv return 0\n");
			if (!flag) break;
			if (flag) return -1;
		}

		nleft -= nread;
		buf += nread;
	}

	return (len - nleft);
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetLineSend(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int len = strlen(msg);

		int ret = IBnetSend(sock, msg, len);
		if (ret < 0)
			return (-1);

		return ret;
	}
}

/*============================================================================*/
/*
 * 내용 :
 *      TODO DATE.2013.10.21 : 수정필요.
 */
/*----------------------------------------------------------------------------*/
int IBnetLineRecv(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
	}
}

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
int IBnetCfg()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FILE* fp;
		char* sFileName;
		char strLine[MAX_LINE_SIZE];
		char* arr1[MAX_ITEM];
		char* arr2[MAX_ITEM];
		char* arr3[MAX_ITEM];
		int i;

		/* 처리할 파일을 오픈한다. */
		sFileName = CfgGetMst("ib.netd.cfg.file");
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			LOG(_FL_, 0, 0, "ERROR : FILE 정보 open error...[%s]", sFileName);
			return -1;
		}

		maxConf = 0;

		/* 한 라인씩 읽으며 처리를 한다. */
		while (1)
		{
			/* strLine은 CR/LF를 포함한 문자열이다. */
			if (fgets(strLine, MAX_LINE_SIZE, fp) == NULL)
				break;

			UtilTrim(UtilDelComment(strLine));
			if (strcmp(strLine, "") == 0)
				continue;

			if (!flag) LOG(_FL_, 0, 0, "[strLine:%s]", strLine);

			if (flag)
			{
				/* 한라인을 구분자로 쪼갠다. */
				UtilStrSplit(strLine, '|', arr1);

				if (strcmp("Y", UtilTrim(arr1[1])) == 0 && maxConf < MAX_IBNET_CONF)
				{
					strcpy(conf[maxConf].sSvcId    , UtilTrim(arr1[0]));
					strcpy(conf[maxConf].sUseFlag  , UtilTrim(arr1[1]));
					strcpy(conf[maxConf].sNetInfo  , UtilTrim(arr1[2]));

					UtilStrSplit(arr1[2], ':', arr2);

					strcpy(conf[maxConf].sIpAddr   , UtilTrim(arr2[0]));
					strcpy(conf[maxConf].sPort     , UtilTrim(arr2[1]));

					conf[maxConf].iPort = UtilToInt(conf[maxConf].sPort);

					strcpy(conf[maxConf].sProcType , UtilTrim(arr1[3]));

					strcpy(conf[maxConf].sTimeOut  , UtilTrim(arr1[4]));
					strcpy(conf[maxConf].sPollTime , UtilTrim(arr1[5]));

					conf[maxConf].iTimeOut  = UtilToInt(conf[maxConf].sTimeOut);
					conf[maxConf].iPollTime = UtilToInt(conf[maxConf].sPollTime);

					strcpy(conf[maxConf].sExeInfo  , UtilTrim(arr1[6]));

					maxConf ++;
				}
			}
		}

		/* 처리된 파일을 닫는다. */
		fclose(fp);

		if (!flag)
		{
			int i;
			for (i=0; i < maxConf; i++)
			{
				printf("\t IBNET_CONF[%d].sSvcId    = [%s]\n", i, conf[i].sSvcId   );
				printf("\t IBNET_CONF[%d].sUseFlag  = [%s]\n", i, conf[i].sUseFlag );
				printf("\t IBNET_CONF[%d].sNetInfo  = [%s]\n", i, conf[i].sNetInfo );
				printf("\t IBNET_CONF[%d].sIpAddr   = [%s]\n", i, conf[i].sIpAddr  );
				printf("\t IBNET_CONF[%d].sPort     = [%s]\n", i, conf[i].sPort    );
				printf("\t IBNET_CONF[%d].iPort     = [%d]\n", i, conf[i].iPort    );
				printf("\t IBNET_CONF[%d].sProcType = [%s]\n", i, conf[i].sProcType);
				printf("\t IBNET_CONF[%d].sTimeOut  = [%s]\n", i, conf[i].sTimeOut );
				printf("\t IBNET_CONF[%d].sPollTime = [%s]\n", i, conf[i].sPollTime);
				printf("\t IBNET_CONF[%d].iTimeOut  = [%d]\n", i, conf[i].iTimeOut );
				printf("\t IBNET_CONF[%d].iPollTime = [%d]\n", i, conf[i].iPollTime);
				printf("\t IBNET_CONF[%d].sExeInfo  = [%s]\n", i, conf[i].sExeInfo );
				printf("\t ------------------------------------\n");
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
 * 내용 : print socket information
 */
/*----------------------------------------------------------------------------*/
char* IBnetGetTypeStr(int iType)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		switch (iType)
		{
			case IBNET_TYPE_NOTCONNECT : return "IBNET_TYPE_NOTCONNECT";  /* 0 */
			case IBNET_TYPE_SERVER     : return "IBNET_TYPE_SERVER"    ;  /* 1 */
			case IBNET_TYPE_ACCEPT     : return "IBNET_TYPE_ACCEPT"    ;  /* 2 */
			case IBNET_TYPE_CONNECT    : return "IBNET_TYPE_CONNECT"   ;  /* 3 */
		}
	}

	return "???";
}

/*============================================================================*/
/*
 * 내용 : print socket information
 */
/*----------------------------------------------------------------------------*/
int IBnetPrintSockItem(char* sHdr, IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 출력한다.
		 */
#if 0
		printf("%s [maxSsi=%d] [maxAsi=%d] [maxCsi=%d]\n", sHdr, maxSsi, maxAsi, maxCsi);

		printf("%s IBNET_SOCK_INFO.type      = [%s]\n", sHdr, IBnetGetTypeStr(si->type));
		printf("%s IBNET_SOCK_INFO.sock      = [%d]\n", sHdr, si->sock);
		printf("%s IBNET_SOCK_INFO.lport     = [%d]\n", sHdr, si->lport);
		printf("%s IBNET_SOCK_INFO.port      = [%d]\n", sHdr, si->port);
		printf("%s IBNET_SOCK_INFO.ipaddr    = [%s]\n", sHdr, si->ipaddr);
		printf("%s IBNET_SOCK_INFO.iTimeOut  = [%d]\n", sHdr, si->iTimeOut);
		printf("%s IBNET_SOCK_INFO.iPollTime = [%d]\n", sHdr, si->iPollTime);
		printf("%s IBNET_SOCK_INFO.sExeInfo  = [%s]\n", sHdr, si->sExeInfo);
		printf("%s ------------------------------------------------\n", sHdr);
#else


        LOG(_FL_, 0, 0, "%s [maxSsi=%d] [maxAsi=%d] [maxCsi=%d]", sHdr, maxSsi, maxAsi, maxCsi);

		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.type      = [%s]", sHdr, IBnetGetTypeStr(si->type));
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.sock      = [%d]", sHdr, si->sock);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.lport     = [%d]", sHdr, si->lport);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.port      = [%d]", sHdr, si->port);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.ipaddr    = [%s]", sHdr, si->ipaddr);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.iTimeOut  = [%d]", sHdr, si->iTimeOut);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.iPollTime = [%d]", sHdr, si->iPollTime);
		LOG(_FL_, 0, 0, "%s IBNET_SOCK_INFO.sExeInfo  = [%s]", sHdr, si->sExeInfo);
		LOG(_FL_, 0, 0, "%s ------------------------------------------------", sHdr);


#endif
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : print socket information
 */
/*----------------------------------------------------------------------------*/
int IBnetPrintSocketInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "========== SSI =======================================");

		int i;
		for (i=0; i < maxSsi; i++)
		{
			char sHdr[50];
			sprintf(sHdr, "SSI(%d)", i);

			IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&ssi[i]);
		}
	}

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "========== ASI =======================================");

		int i;
		for (i=0; i < maxAsi; i++)
		{
			char sHdr[50];
			sprintf(sHdr, "ASI(%d)", i);

			IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&asi[i]);
		}
	}

	if (flag)
	{
		if (flag) LOG(_FL_, 0, 0, "========== CSI =======================================");

		int i;
		for (i=0; i < maxCsi; i++)
		{
			char sHdr[50];
			sprintf(sHdr, "CSI(%d)", i);

			IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&csi[i]);
		}
	}

	if (flag) LOG(_FL_, 0, 0, "========== ### =======================================");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetSetSockInfo()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		maxSsi = 0;
		maxAsi = 0;
		maxCsi = 0;

		int i;
		for (i=0; i < maxConf; i++)
		{
			if (strcmp("NULL", conf[i].sIpAddr) == 0)
			{
				/*
				 * Server Socket Information
				 */

				ssi[maxSsi].type  = IBNET_TYPE_SERVER;
				ssi[maxSsi].sock  = -1;              /* socket ready state */
				ssi[maxSsi].lport = conf[i].iPort;
				ssi[maxSsi].port  = conf[i].iPort;
				strcpy(ssi[maxSsi].ipaddr, conf[i].sIpAddr);  /* NULL */

				memset((void*) &(ssi[maxSsi].addr), 0x00, sizeof(struct sockaddr_in));
				ssi[maxSsi].addr.sin_family      = AF_INET;
				ssi[maxSsi].addr.sin_addr.s_addr = htonl(INADDR_ANY);
				ssi[maxSsi].addr.sin_port        = htons(conf[i].iPort);

				ssi[maxSsi].iTimeOut  = conf[i].iTimeOut;
				ssi[maxSsi].iPollTime = conf[i].iPollTime;

				ssi[maxSsi].lastTime = time(NULL);

				strcpy(ssi[maxSsi].sExeInfo, conf[i].sExeInfo);

				maxSsi ++;
			}
			else
			{
				/*
				 * Connect Socket Information
				 */

				csi[maxCsi].type  = IBNET_TYPE_CONNECT;
				csi[maxCsi].sock  = -1;              /* socket ready state */
				csi[maxCsi].lport = conf[i].iPort;
				csi[maxCsi].port  = conf[i].iPort;
				strcpy(csi[maxCsi].ipaddr, conf[i].sIpAddr);  /* IP Address */

				memset((void*) &(csi[maxCsi].addr), 0x00, sizeof(struct sockaddr_in));
				csi[maxCsi].addr.sin_family      = AF_INET;
				csi[maxCsi].addr.sin_addr.s_addr = inet_addr(conf[i].sIpAddr);
				csi[maxCsi].addr.sin_port        = htons(conf[i].iPort);

				csi[maxCsi].iTimeOut  = conf[i].iTimeOut;
				csi[maxCsi].iPollTime = conf[i].iPollTime;

				csi[maxCsi].lastTime = time(NULL);

				strcpy(csi[maxCsi].sExeInfo, conf[i].sExeInfo);

				maxCsi ++;
			}
		}

		if (flag)
		{
			/*
			 * 설정된 소켓정보를 출력한다.
			 */

			IBnetPrintSocketInfo();
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
int IBnetCloseSocket(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetCloseSocket [START]...");

	if (flag)
	{
		if (flag)
		{
			/* SO_LINGER 옵션을 설정한다. */
			struct linger ling;
			
			ling.l_onoff = 1;    /* SO_LINGER 옵션을 적용한다. 1(TRUE), 0(FALSE) */
			ling.l_linger = 3;   /* 대기시간 (sec) */
			
			setsockopt(si->sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
		}

		close(si->sock);
		si->sock = -1;
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetCloseSocket [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 서버소켓을 생성한다. 이미 다른 프로세스가 포트바인드 되어 있으면 에러로 종료
 */
/*----------------------------------------------------------------------------*/
int IBnetServerSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetServerSocket [START]...");

	if (flag)
	{
		/*
		 * 서버소켓이 없거나, 서버소켓이 구성되어 있으면
		 * 이 함수는 실행하지 않는다.
		 */

		if (maxSsi == 0)
		{
			/* 서버 소켓이 없다. */
			if (!flag) LOG(_FL_, 0, 0, "STATUS : 서버소켓이 없다. #####");
			return 0;
		}

		if (ssi[0].sock > 0)
		{
			/* 이미 서버리스닝은 구성되었다. */
			if (!flag) LOG(_FL_, 0, 0, "STATUS : 이미 서버리스닝은 구성되었다 #####");
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
		for (i=0; i < maxSsi; i++)
		{
			if (flag)
			{
				/*
				 * socket을 생성한다
				 */
				ssi[i].sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (ssi[i].sock < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a server socket");
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * socket option을 처리한다.
				 */
				int valOption = 1;

				setsockopt(ssi[i].sock, SOL_SOCKET, SO_REUSEADDR, &valOption, sizeof(valOption));
			}

			if (flag)
			{
				/* SO_LINGER 옵션을 설정한다. */
				struct linger ling;
				
				ling.l_onoff = 1;    /* SO_LINGER 옵션을 적용한다. 1(TRUE), 0(FALSE) */
				ling.l_linger = 3;   /* 대기시간 (sec) */
				
				setsockopt(ssi[i].sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
			}

			if (flag)
			{
				/*
				 * address 정보를 세팅하고 bind 한다.
				 */
				int ret = bind(ssi[i].sock, (struct sockaddr*) &(ssi[i].addr), sizeof(ssi[i].addr));
				if (ret < 0)
				{
					IBnetCloseSocket(&ssi[i]);

					if (flag) LOG(_FL_, 0, 0, "ERROR : already bind the port %d", ssi[i].lport);
					if (flag) exit(-1);
				}
			}

			if (flag)
			{
				/*
				 * listen 처리하면서 기다린다.
				 */
				int ret = listen(ssi[i].sock, 5);
				if (ret < 0)
				{
					IBnetCloseSocket(&ssi[i]);

					if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a listen of the port %d", ssi[i].lport);
					if (flag) exit(-1);
				}
			}

			if (flag)
			{
				/*
				 * 생성된 서버소켓 정리
				 */

				if (flag)
				{
					/* 처리된 정보를 출력한다. */
					char sHdr[50];
					sprintf(sHdr, "SSI(%d)", i);
					IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&ssi[i]);
				}
			}

			if (flag) LOG(_FL_, 0, 0, "STATUS : 서버LISTEN 구성 ->[LISTEN PORT:%d]"
				, ssi[i].lport);
		}

		if (!flag)
		{
			/* 모든 소켓정보를 출력한다. */
			IBnetPrintSocketInfo();
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetServerSocket [END]...");

	return maxSsi;
}

/*============================================================================*/
/*
 * 내용 : ACCEPT 소켓을 생성한다.
 *        클라이언트의 접속을 통해서 생성되면 이 항목들 갯수는 가변항으로 변한다.
 *        클라이언트가 접속할 때마다 증가한다. 세션이 종료되면 감소하고.
 *        accept socket의 갯수는 가변.
 */
/*----------------------------------------------------------------------------*/
int IBnetAcceptSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetAcceptSocket [START]...");

	if (flag)
	{
		/*
		 * 서버 Listening 소켓을 구성한다.
		 */

		int ret = IBnetServerSocket();
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
				if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a accept selection...");
				return -1;
			}
			else if (ret == 0)
			{
				/* not event */
				return 0;
			}
		}

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
					/* ERROR */
					if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make an accept...");
					return -1;
				}

				if (flag)
				{
					/*
					 * ACCEPT Client Socket 생성
					 * 비어있는 SOCK_INFO를 찾는다.
					 */

					int k;
					for (k = 0; k < MAX_ACCEPT_SOCK; k++)
					{
						if (asi[k].sock == -1 || asi[k].type == IBNET_TYPE_NOTCONNECT)
						{
							/* CONN_TYPE_CONNECT 타입이 아니고,
							   지금 사용중이 아니면
							   found the socket not used */
							break;
						}
					}

					if (k == MAX_ACCEPT_SOCK)
					{
						/* 비어있는 소켓이 없다. */
						if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't find a empty socket...");
						return -1;
					}

					/*
					 * ACCEPT한 클라이언트소켓 정보를 저장한다.
					 */

					asi[k].type  = IBNET_TYPE_ACCEPT;
					asi[k].sock  = sock;
					asi[k].lport = ssi[i].lport;
					asi[k].port  = ntohs(addr.sin_port);
					strcpy(asi[k].ipaddr, inet_ntoa(addr.sin_addr));

					memcpy((void*)&(asi[k].addr), (void*)&addr, sizeof(struct sockaddr_in));

					asi[k].iTimeOut  = ssi[i].iTimeOut;
					asi[k].iPollTime = ssi[i].iPollTime;

					asi[k].lastTime = time(NULL);

					strcpy(asi[k].sExeInfo, ssi[i].sExeInfo);

					maxAsi = MAX(maxAsi, k+1);

					if (flag)
					{
						/* 처리된 정보를 출력한다. */
						char sHdr[50];
						sprintf(sHdr, "ACCEPT ASI(%d) (SO_LINGER=0)", k);
						IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&asi[k]);
					}
					
					if (flag)
					{
						/* SO_LINGER 옵션을 설정한다. */
						struct linger ling;
						
						ling.l_onoff = 1;    /* SO_LINGER 옵션을 적용한다. 1(TRUE), 0(FALSE) */
						ling.l_linger = 3;   /* 대기시간 (sec) */
						
						setsockopt(sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * Accept Socket을 확인한다.
			 */
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetAcceptSocket [END]...");

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
int IBnetConnectSocket()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetConnectSocket [START]...");

	if (flag)
	{
		int i;
		for (i=0; i < maxCsi; i++)
		{
			if (flag)
			{
				if (csi[i].sock != -1)
				{
					/*
					 * connect socket 값이 있거나
					 * 이미 생성된 소켓은 SKIP 한다.
					 */
					continue;
				}
			}

			/*
			 * CONNECT 실행조건
			 * sock == -1
			 */

			if (flag)
			{
				/*
				 * socket을 생성한다
				 */
				csi[i].sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				if (csi[i].sock < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a client socket");
					IBnetCloseSocket(&csi[i]);
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * address 정보를 세팅하고 bind 한다.
				 */

				int ret = connect(csi[i].sock, (struct sockaddr*) &(csi[i].addr), sizeof(struct sockaddr_in));
				if (ret < 0)
				{
					if (flag)
					{
						/* TODO DATE.2013.10.14 : connection 실패와 1초 delay 줌 */
						if (flag) LOG(_FL_, 0, 0, "ERROR : CONNECTION_FAIL [%s:%d]"
							, csi[i].ipaddr, csi[i].port);
						sleep(1);
					}

					IBnetCloseSocket(&csi[i]);
					return -1;
				}
			}

			if (flag)
			{
				/*
				 * CONNECT한 클라이언트소켓 정보를 처리
				 */

				if (flag)
				{
					/* 처리된 정보를 출력한다. */
					char sHdr[50];
					sprintf(sHdr, "CONNECT CSI(%d)", i);
					IBnetPrintSockItem(sHdr, (IBNET_SOCK_INFO*)&csi[i]);
				}
			}

			if (flag)
			{
				/*
				 * Connect Socket을 확인한다.
				 */
			}
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetConnectSocket [END]...");

	return 0;
}

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
int IBnetTimeOutHandler(int (*handler) (IBNET_SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetTimeOutHandler [START]...");

	if (flag)
	{
		/* handler가 없으면 이 로직은 skip 한다. */
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		if (flag)
		{
			/*
			 * connect socket 처리
			 */

			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (csi[i].sock != -1)
				{
					if (handler(&csi[i]) < 0)
					{
						IBnetCloseSocket(&csi[i]);
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * accept socket 처리
			 */

			int i;
			for (i=0; i < maxAsi; i++)
			{
				if (asi[i].sock != -1)
				{
					if (handler(&asi[i]) < 0)
					{
						IBnetCloseSocket(&asi[i]);
					}
				}
			}
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetTimeOutHandler [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetPollTimeHandler(int (*handler) (IBNET_SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetPollTimeHandler [START]...");

	if (flag)
	{
		/* handler가 없으면 이 로직은 skip 한다. */
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		if (flag)
		{
			/*
			 * connect socket 처리
			 */

			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (csi[i].sock != -1)
				{
					if (handler(&csi[i]) < 0)
					{
						IBnetCloseSocket(&csi[i]);
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * accept socket 처리
			 */

			int i;
			for (i=0; i < maxAsi; i++)
			{
				if (asi[i].sock != -1)
				{
					if (handler(&asi[i]) < 0)
					{
						IBnetCloseSocket(&asi[i]);
					}
				}
			}
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetPollTimeHandler [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetRecvHandler(int (*handler) (IBNET_SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetRecvHandler [START]...");

	if (flag)
	{
		/* handler가 없으면 이 로직은 skip 한다. */
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		/*
		 * 모든 socket의 recv를 select 처리한다.
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
			for (i=0; i < maxCsi; i++)
			{
				if (csi[i].sock != -1)
				{
					/* connect socket */
					FD_SET(csi[i].sock, &rfds);
					maxfds = MAX(maxfds, csi[i].sock);
				}
			}

			for (i=0; i < maxAsi; i++)
			{
				if (asi[i].sock != -1)
				{
					/* accept socket */
					FD_SET(asi[i].sock, &rfds);
					maxfds = MAX(maxfds, asi[i].sock);
				}
			}
		}

		if (flag)
		{
			/* select를 실행한다. */
			int ret = select(maxfds+1, &rfds, NULL, NULL, &tv);
			if (ret < 0)
			{
				/* ERROR */
				if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a recv selection...");
				return -1;
			}
			else if (ret == 0)
			{
				/* not event */
				return 0;
			}
		}

		if (flag)
		{
			/* rfds select 처리한 값을 세팅한다. */
			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (FD_ISSET(csi[i].sock, &rfds))
				{
					/*
					 * socket이 readable 한 상태이다. 읽을 자료가 있다.
					 */

					if (flag)
					{
						/*
						 * accept socket 핸들러를 실행한다.
						 */
						if (handler(&csi[i]) < 0)
						{
							/* close */
							IBnetCloseSocket(&csi[i]);
						}
					}

					if (flag)
					{
						/* PRINT : socket recv 가 처리됨 */
					}
				}
			}

			for (i=0; i < maxAsi; i++)
			{
				if (FD_ISSET(asi[i].sock, &rfds))
				{
					/*
					 * socket이 readable 한 상태이다. 읽을 자료가 있다.
					 */

					if (flag)
					{
						/*
						 * accept socket 핸들러를 실행한다.
						 */
						if (handler(&asi[i]) < 0)
						{
							/* close */
							IBnetCloseSocket(&asi[i]);
						}
					}

					if (flag)
					{
						/* PRINT : socket recv 가 처리됨 */
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * 뒤처리 한다.
			 */
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetRecvHandler [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetSendHandler(int (*handler) (IBNET_SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetSendHandler [START]...");

	if (flag)
	{
		/* handler가 없으면 이 로직은 skip 한다. */
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		/*
		 * 모든 socket의 recv를 select 처리한다.
		 */

		struct timeval tv;
		fd_set sfds;
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
			FD_ZERO(&sfds);

			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (csi[i].sock != -1)
				{
					/* connect socket */
					FD_SET(csi[i].sock, &sfds);
					maxfds = MAX(maxfds, csi[i].sock);
				}
			}

			for (i=0; i < maxAsi; i++)
			{
				if (asi[i].sock != -1)
				{
					/* accept socket */
					FD_SET(asi[i].sock, &sfds);
					maxfds = MAX(maxfds, asi[i].sock);
				}
			}
		}

		if (flag)
		{
			/* select를 실행한다. */
			int ret = select(maxfds+1, NULL, &sfds, NULL, &tv);
			if (ret < 0)
			{
				/* ERROR */
				if (flag) LOG(_FL_, 0, 0, "ERROR : couldn't make a send selection...");
				return -1;
			}
			else if (ret == 0)
			{
				/* not event */
				return 0;
			}
		}

		if (flag)
		{
			/* sfds select 처리한 값을 세팅한다. */
			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (FD_ISSET(csi[i].sock, &sfds))
				{
					/*
					 * socket이 readable 한 상태이다. 읽을 자료가 있다.
					 */

					if (flag)
					{
						/*
						 * accept socket 핸들러를 실행한다.
						 */
						if (handler(&csi[i]) < 0)
						{
							/* close */
							IBnetCloseSocket(&csi[i]);
						}
					}

					if (flag)
					{
						/* PRINT : socket recv 가 처리됨 */
					}
				}
			}

			for (i=0; i < maxAsi; i++)
			{
				if (FD_ISSET(asi[i].sock, &sfds))
				{
					/*
					 * socket이 readable 한 상태이다. 읽을 자료가 있다.
					 */

					if (flag)
					{
						/*
						 * accept socket 핸들러를 실행한다.
						 */
						if (handler(&asi[i]) < 0)
						{
							/* close */
							IBnetCloseSocket(&asi[i]);
						}
					}

					if (flag)
					{
						/* PRINT : socket recv 가 처리됨 */
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * 뒤처리 한다.
			 */
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetSendHandler [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int IBnetExecHandler(int (*handler) (IBNET_SOCK_INFO*))
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "IBnetExecHandler [START]...");

	if (flag)
	{
		/* handler가 없으면 이 로직은 skip 한다. */
		if (handler == NULL)
			return 0;
	}

	if (flag)
	{
		if (!flag)
		{
			/*
			 * connect socket 처리
			 */

			int i;
			for (i=0; i < maxCsi; i++)
			{
				if (csi[i].sock != -1)
				{
					if (handler(&csi[i]) < 0)
					{
						IBnetCloseSocket(&csi[i]);
					}
				}
			}
		}

		if (flag)
		{
			/*
			 * accept socket 처리
			 */

			int i;
			for (i=0; i < maxAsi; i++)
			{
				if (asi[i].sock != -1)
				{
					if (handler(&asi[i]) < 0)
					{
						IBnetCloseSocket(&asi[i]);
					}
				}
			}
		}
	}

	if (!flag) LOG(_FL_, 0, 0, "IBnetExecHandler [END]...");

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

char          sSendLine       [MAX_LINE_SIZE];   /* 송신용 자료 */
char          sRecvLine       [MAX_LINE_SIZE];   /* 수신용 자료 */

int           iSendLen        ;
int           iRecvLen        ;

BIZ_DAT*      pSndBizDat      = (BIZ_DAT*) &sSendLine;
BIZ_DAT*      pRcvBizDat      = (BIZ_DAT*) &sRecvLine;

FEP_BIZ_DAT*  pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*  pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

/*----------------------------------------------------------------------------*/

char          sFQLine         [MAX_LINE_SIZE];   /* FQ 처리용 자료 */
int           iFQLen          ;

char          sDataLine       [MAX_LINE_SIZE];   /* 내부 처리용 자료 */
int           iDataLen        ;

char          sGUID           [100];             /* Global User ID */

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
int TimeOutProc(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "TimeOutProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "-> SI (%s)", __FUNCTION__);
		IBnetPrintSockItem(sHdr, si);
	}

	if (flag)
	{
	}

	if (!flag) LOG(_FL_, 0, 0, "TimeOutProc [END]...");

	return 0;
}


/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int PollTimeProc(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "PollTimeProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "-> SI (%s)", __FUNCTION__);
		IBnetPrintSockItem(sHdr, si);
	}

	if (flag)
	{
	}

	if (!flag) LOG(_FL_, 0, 0, "PollTimeProc [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int RecvProc(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "RecvProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "-> SI (%s)", __FUNCTION__);
		IBnetPrintSockItem(sHdr, si);
	}

	if (flag)
	{
	}

	if (!flag) LOG(_FL_, 0, 0, "RecvProc [END]...");

	return 0;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int SendProc(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "SendProc [START]...");

	if (!flag)
	{
		char sHdr[50];
		sprintf(sHdr, "-> SI (%s)", __FUNCTION__);
		IBnetPrintSockItem(sHdr, si);
	}

	if (flag)
	{
	}

	if (!flag) LOG(_FL_, 0, 0, "SendProc [END]...");

	return 0;
}


/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int ExecProc(IBNET_SOCK_INFO* si)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag) LOG(_FL_, 0, 0, "ExecProc [START]...");

	if (flag)
	{
		char sHdr[50];
		sprintf(sHdr, "-> SI (%s)", __FUNCTION__);
		IBnetPrintSockItem(sHdr, si);
	}

	if (flag)
	{
		int pid;
		int status;

		if (!flag)
		{
			/* child signal을 무시한다. */
			signal(SIGCHLD, SIG_IGN);
		}
		
		switch (pid=fork())
		{
			case  0:   /* child process */
				if (flag)
				{
					char buf[512];
					char* argv[50];
					strcpy(buf, si->sExeInfo);
					UtilGetArgs(buf, argv);
					if (flag)
					{
						int i;
						for (i=0; argv[i]; i++)
						{
							printf("(%d) [%s]\n", i, argv[i]);
						}
					}

					/*
					 * [ Redirect ]
					 * 표준입출력 및 에러의 방향을 바꾼다.
					 * si->sock -> 0.stdin
					 * si->sock -> 1.stdout
					 * si->sock -> 2.stderr
					 */
					dup2(si->sock, 0);
					dup2(si->sock, 1);
					dup2(si->sock, 2);
					
					if (execv(argv[0], argv) < 0)
					{
						if (flag) LOG(_FL_, 0, 0, "ERROR : execl('%s')...", si->sExeInfo);
						exit(0);
					}
				}
				break;

			case -1:   /* error */
				if (flag) LOG(_FL_, 0, 0, "ERROR : fork()...");
				break;

			default:   /* parent process */
				if (!flag) sleep(2);
				if (flag) LOG(_FL_, 0, 0, "SUCCESS : execl('%s')...", si->sExeInfo);
				break;
		}

		waitpid(pid, &status, 0);
	}

	if (flag) LOG(_FL_, 0, 0, "ExecProc [END]..");

	return -1;
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
#if 0
	if (flag)
	{
		/*
		 * 임시로 사용함.
		 */
		if (!flag && argc == 1 && strcmp(argv[0], "IBnet") == 0)
		{
			printf("ps_man으로 실행하지 말아 주세요.\n");
			exit(0);
		}

		strcpy(sDummy, "IBNETD0000");
		argc = 2;
		argv[1] = (char*)&sDummy;
	}

	if (flag)
	{
		/*
		 * 기본 처리
		 */
		sProgName = argv[0];

		if (argc < 2 || strcmp("IBNETD0000", argv[1]) != 0)
		{
			/*
			 * USAGE
			 */
			printf("ERROR : Invalid argument... (Usage : %s IBNETD0000)\n", sProgName);
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
		UtilToUpper(argv[1]);
		sprintf(sServiceid, "%-10.10s", argv[1]);

		if (flag) printf("\x1b[44m< %s %s >\x1b[0m\n", sProgName, sServiceid);
		if (flag) printf("Program : [%s], Version : %s\n", sProgName, VERSION);
		if (flag) printf("\t -------------------------------------------\n");
	}
#else
	if (flag)
	{
		/*
		 * 기본 처리
		 */
		sProgName = argv[0];

		if (argc < 2)
		{
			/*
			 * USAGE
			 */
			printf("ERROR : Invalid argument... (Usage : %s SERVICE_NAME)\n", sProgName);
			exit(0);
		}

		if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "-ver") == 0)
		{
			/*
			 * VERSION
			 */
			printf("[%s] Program Version\n=============== \n%s", sProgName, VERSION);
			exit(0);
		}
	}

	if (flag)
	{
		/*
		 * 프로세스에서 사용하는 서비스명과 FEPID를 얻는다.
		 */

		UtilToUpper(argv[1]);
		sprintf(sServiceid, "%-10.10s", argv[1]);
		sprintf(sFepid, "%-5.5s", sServiceid + 3);
		sprintf(sSpace, "%1.1s", sServiceid + 1);

		if (flag) printf("\x1b[32m< %s %s %s (%s) >\x1b[0m\n"
			, sProgName, sServiceid, sFepid, sSpace);
		if (flag) printf("Program : [%s], Version : %s\n"
			, sProgName, VERSION);
		if (flag) printf("\t -------------------------------------------\n");
	}
#endif

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
	
	if (flag)
	{
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
			if (!flag) LOG(_FL_, 0, 0,"[%s]\n", strFile);

			if (access(strFile, W_OK) < 0)
			{
				/* 최초생성 */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND 모드 */
				freopen(strFile, "a+", stdout);
			}
			if (flag) LOG(_FL_, 0, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 0, 0,"LOG_FILE => [%s]", strFile );
		}
    }

	if (flag)
	{
		IBnetCfg();
		IBnetSetSockInfo();
		
		while ( TRUE )
		{
			IBnetAcceptSocket();          /* server socket and accept socket */
			IBnetConnectSocket();         /* client connection socket        */

			if (!flag)
			{
				/*
				 * 일반전문 처리용으로 사용
				 */

				IBnetTimeOutHandler (TimeOutProc);
				IBnetPollTimeHandler(PollTimeProc);
				IBnetRecvHandler    (RecvProc);
				IBnetSendHandler    (SendProc);
			}

			if (flag)
			{
				/*
				 * IBnet 용으로 사용
				 */

				IBnetExecHandler(ExecProc);
			}

			if (!flag) sleep(1);
			if (flag) usleep(100 * 1000);
		}
	}
}

#endif
