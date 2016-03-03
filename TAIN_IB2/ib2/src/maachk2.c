/*
 * PROGRAM ID   : oxrcom2.c
 * PROGRAM NAME : oxrcom2 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : oxrcom2 모듈
 *                대외기관 연결 모듈(수신)
 * HISTORY      : 2013.09.10 강석. 최초작성
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
 *  typedef struct _FEP_HDR_50_
 *  {
 *      char    sLength     [ 4];  // 전문길이    : 전문길이 Field를 제외한 나머지 Length
 *      char    sBsnCode    [ 6];  // 업무식별코드: KBBS01
 *      char    sTrCode     [ 4];  // TR-CODE     : LINK,LIOK,DATA,DAOK,POLL,POOK,RJCT 등
 *      char    sResCode    [ 2];  // 응답코드    : '00'5. 장애응답코드표 참조
 *      char    sSeq        [ 8];  // 전문일련번호: '99999999' ? 전문SEQ번호
 *      char    sDataCnt    [ 2];  // Data건수    : 01 ~ (2 KByte 이내)
 *      char    sDateTime   [20];  // 시각        : YYYYMMDDHHMMSSssssss
 *      char    sAckYn      [ 1];  // ACK구분     : Data에 대한 ACK : 사용(Y), 미사용(N)
 *      char    sFiller     [ 3];
 *  } FEP_HDR;
 *
 *  typedef struct _BIZ_HDR_100_
 *  {
 *      char    sDataLength [ 4];  // 본 Length FIELD를 제외한 개별 DATA SIZE
 *      char    sDataSeq    [ 8];  // 해당 Data의 일련번호
 *      char    sGlobalId   [32];  // GUID
 *      char    sDateTime   [20];  // 시각        : YYYYMMDDHHMMSSssssss
 *      char    sRespCode   [ 4];
 *      char    sMediaTp    [ 2];
 *      char    sMediaSeq   [ 1];
 *      char    sFiller     [29];
 *  } BIZ_HDR;
 *
 *  typedef struct _BIZ_DAT_
 *  {
 *      BIZ_HDR         stBiz;  // BIZ_HEADER (100)
 *
 *      char    sData    [ 1];  // variable length.
 *  } BIZ_DAT;
 *
 *  typedef struct _FEP_BIZ_DAT_
 *  {
 *      FEP_HDR         stFep;  // FEP_HEADER (50)
 *      BIZ_HDR         stBiz;  // BIZ_HEADER (100)
 *
 *      char    sData    [ 1];  // variable length... 대외기관 송수신자료
 *  } FEP_BIZ_DAT;
 *
 */
/*----------------------------------------------------------------------------*/
/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define       SEND_FQ         "OFS"           /* 송신을 위한 FQ */
#define       RECV_FQ         "OFR"           /* 수신을 위한 FQ */

#define       MAAPER          "MAACOM0000"    /* 매퍼           */

/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)
#define       POLL_TIME            10

char          sDataLine       [MAX_DATA_LINE];   /* 내부 처리용 자료 */
char          sSendLine       [MAX_DATA_LINE];   /* 송신용 자료 */
char          sRecvLine       [MAX_DATA_LINE];   /* 수신용 자료 */

int           iDataLen        ;
int           iSendLen        ;
int           iRecvLen        ;

BIZ_DAT*      pBizDat         = (BIZ_DAT*) &sDataLine;
BIZ_DAT*      pSndBizDat      = (BIZ_DAT*) &sSendLine;
BIZ_DAT*      pRcvBizDat      = (BIZ_DAT*) &sRecvLine;

FEP_BIZ_DAT*  pFepBizDat      = (FEP_BIZ_DAT*) &sDataLine;
FEP_BIZ_DAT*  pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*  pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

/*----------------------------------------------------------------------------*/

int           iForkFlag            ;      /* fork 처리 flag */
int           iProcessAlive = 0    ;
int           iProcessDiff  = 0    ;

char          sGUID           [100];      /* Global User ID */

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
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
		/* 확인용 */
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
			 * [ TIME_OUT : SERVER / CLIENT ]
			 *
			 * TimeOut 시간이 초과 되었는지 확인한다.
			 * extern int iTimeOutSec  ;
			 *   [차이] > TimeOut(초) 이면 close, 아니면 지나간다.
			if (diff > iTimeOutSec && csi->conntype == CONN_TYPE_ACCEPT)
			 */

			if (diff > iTimeOutSec)
			{
				if (flag) LOG(_FL_, 9, 0,
					"[TIME_OUT_CLOSE:%d] SESSION CLOSE....(%s:%d)"
					, iTimeOutSec, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (flag)
		{
			/*
			 * [ POLL_SEND : CONNECT CLIENT ]
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
					strcpy(sSendLine, "1285M1.0<매퍼TEST>108001007_1->0800_1190I_KBB01   0600              00035000                                                                                    KBB0100411080010071HANWHA            0490      50500308    0045050030800400000IQ20140611154240000000000000002014061115424020140611154240              0000000002    1115424008001191  0S0Y0000                                                                0000000000005050030801 01       000000T320351037 0000000000000000000000000                                                                                                                         2014061100700000000                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ");
					iSendLen = strlen(sSendLine);
					
					if (flag) LOG(_FL_, 0, 0, "[POLL_MAKE] [%d]sec [%d:%s]", diff, iSendLen, sSendLine);    
				}

				if (flag)
				{
					/* POLL REQ 송신 */
					int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 9, 0,
							"[POLL_SEND] SESSION CLOSE BY SEND ERROR..(%s:%d)(fd:%d)"
							, csi->ipaddr, csi->port, csi->sock);
						return -1;
					}
				}

				if (flag)
				{
					/* POLL REQ 출력 */
					if (flag) LOG(_FL_, 0, 0, "[CHK_SEND] -> [%d:%s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
                    if (flag) LOG(_FL_, 5, 0, "[CHK_SEND] -> [%d:%50.50s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
				}
			}
			else
			{
				if (!flag) LOG(_FL_, 0, 0, "POLL 송신하지 않음");
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
		/* 확인용 */
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
				if (flag) LOG(_FL_, 9, 0,
					"[MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* 수신길이 0 -> 미처리 */
				if (flag) LOG(_FL_, 9, 0, "[MSG_RECV] Length Zero(0).(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return 0;
			}
			else
			{
				/* 정상 수신 */
				if (!flag) LOG(_FL_, 5, 0, "[MSG_RECV] [%d:%s].(%s:%d) (fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
			}
		}

		if (flag)
		{
			/*
			 * 매퍼 ALIVE CHECK
			 */
			SigAliveWrite(MAAPER);
			
			{
				/*
				 * 3. 일반전문 수신 -> 수신처리( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] <- \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
                if (flag) LOG(_FL_, 5, 0, "[MSG_RECV] <- \x1b[42m[%04d:%50.50s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
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
 * 내용 : /hw03/ibridge/DAT2/YYYYMMDD/PS_ALIVE 파일에 ALIVE를 기록한다.
 */
/*----------------------------------------------------------------------------*/
int CheckAlive()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "CheckAlive...");

	if (flag)
	{
		/* 프로세스 기동 초기화 */
		if (iProcessAlive == 0 ) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
		
		iProcessDiff = time(NULL) - iProcessAlive;
		
		/* 1분마다 Alive 기록 */
		if (iProcessDiff >= 60) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
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
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int MainService(char* strInfo)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

    SigSetSignal();
    
    if (flag) CheckAlive();
    
	if (!flag) LOG(_FL_, 0, 0, "SOCKET INFO [%s]", strInfo);

	if (flag)
	{
		if (flag)
		{
			/* 사용할 FQ를 선택한다. */
            /* 처리여부 : SEND(O) ,RECV(X), SEND/RECV(O) */
			if (!flag) FQ2Init(SEND_FQ, sServiceid);
            /* 처리여부 : SEND(X) ,RECV(O), SEND/RECV(O) */
			if (flag) FQ2Init(RECV_FQ, sServiceid);
		}

		if (flag)
		{
			/* 소켓정보를 파싱하여 정리한다. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			/* 프로세스 동작체크 */
			if (flag) CheckAlive();
				
			/* server / accept / connect 소켓을 구성하고 소켓을 생성한다. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect 소켓을 체크한다. TimeOut, Poll 체크 */
			if (flag) MSockCheckHandler(CheckProc);

			/* 소켓의 recv 처리를 다루는 handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (!flag) sleep(5);
			if (flag) usleep(20 * 1000); /* 0.02초 */
				
		}
	}

	if (flag) LOG(_FL_, 9, 0, "\x1b[41m-----%S ProcessExit -----\x1b[0m", sServiceid);

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
 * 위에서 정의한 함수를 테스트 한다.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

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
		if (isupper(argv[1][0]))
		{
			iForkFlag = 1;
		}
		else
		{
			iForkFlag = 0;
		}

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

	if (flag)
	{
		/*
		 * master 환경파일을 읽는다. biz.properties 파일도 읽는다.
		 */
		if (CfgMaster() < 0)
		{
			printf("ERROR : get config....\n");
			exit(9);
		}

		if (flag)
		{
			printf("\t CfgGetMaster(ib.version  )  [%s]\n", CfgGetMaster("ib.version"  ));
			printf("\t CfgGetMaster(ib.serialkey)  [%s]\n", CfgGetMaster("ib.serialkey"));
			printf("\t CfgGetMaster(ib.author   )  [%s]\n", CfgGetMaster("ib.author"   ));
			printf("\t CfgGetMaster(ib.company  )  [%s]\n", CfgGetMaster("ib.company"  ));
			printf("\t -------------------------------------------\n");

			CfgPoll(); /* Default Poll */
			CfgOfs2();
			iPollSendSec = POLL_TIME;
			
			iLLen = 4;
			iLPls = 0;
			printf("\t iLLen = [%d]\n", iLLen);
			printf("\t iLPls = [%d]\n", iLPls);
			printf("\t -------------------------------------------\n");

			printf("\n프로그램을 시작합니다.\n");
		}
	}

	if (flag && iForkFlag)
	{
		/*
		 * fork를 이용하여 child process 를 실행한다.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * 표준출력을 제거한다.
		 */
		if (!flag) freopen("/dev/null", "w", stdout);

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
			if (flag) LOG(_FL_, 9, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 9, 0,"LOG_FILE => [%s]", strFile );
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOfs2("SVR_INFO"));  /* 접속정보 */
}

#endif