/*
 * PROGRAM ID   : oxameg2.c
 * PROGRAM NAME : oxameg2 관련 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : oxameg2 관련 모듈
 * HISTORY      : 2013.10.02 강석. 최초작성
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

#define       SEND_FQ         "OAS"           /* 송신을 위한 FQ */
#define       RECV_FQ         "OAR"           /* 수신을 위한 FQ */

/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)

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

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int ReadFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "INFO : start ReadFQ().");

	if (flag)
	{
		*sDataLine = 0x00;

		if (flag)
		{
			/*
			 * FQ를 읽는다.
			FQ* pFq = FQOpen(SEND_FQ, sFepid);
			 */
			FQ2* pFq = FQ2Open(SEND_FQ, sServiceid);

			char* p = FQ2Read(pFq);

			FQ2Close(pFq);

			/*
			 * 읽은 자료를 sDataLine에 복사한다.
			 */
			if (p == NULL)
			{
				*sDataLine = 0x00;
				return 0;
			}
			strcpy(sDataLine, p);
			iDataLen = strlen(sDataLine);
		}
		
		if (flag)
		{
			/* DATE.2014.05.19 : 중복 체크 */
			static char sOldDataLine[MAX_DATA_LINE] = { 0x00, };
			
			if (strcmp(sOldDataLine, sDataLine) == 0)
			{
				/* 중복 */
				if (flag) LOG(_FL_, 8, 0, "\x1b[41m[WARNING!!]\x1b[0m [FQ_READ](중복)=[%04d:%s] ", iDataLen, sDataLine);

#if 0
				strcpy(sOldDataLine, sDataLine);
#else
				*sDataLine = 0x00;
				return 0;
#endif
			}
			else
			{
				if (flag) LOG(_FL_, 5, 0, "[FQ_READ]  -> [%04d:%s] ", iDataLen, sDataLine);
				strcpy(sOldDataLine, sDataLine);
			}
		}
	}

	return iDataLen;
}

/*============================================================================*/
/*
 * 내용 :
 */
/*----------------------------------------------------------------------------*/
int WriteFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "INFO : start WriteFQ().");

	if (flag)
	{
		if (*sDataLine == 0x00)
		{
			return 0;
		}

		if (flag)
		{
			/*
			 * 자료를 FQ에 기록한다.
			FQ* pFq = FQOpen(RECV_FQ, sFepid);
			 */
			FQ2* pFq = FQ2Open(RECV_FQ, sServiceid);

			FQ2Write(pFq, sDataLine);

			FQ2Close(pFq);
		}
	}

	if (flag) LOG(_FL_, 0, 0, "[FQ_WRITE] [%04d:%s] ", iDataLen, sDataLine);

	return iDataLen;
}

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
				if (flag) LOG(_FL_, 0, 0,
					"[TIME_OUT_CLOSE:%d] SESSION CLOSE....(%s:%d) (fd:%d)"
					, iTimeOutSec, csi->ipaddr, csi->port, csi->sock);
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
                         * (JUN) 2014-03-12 MegaBox / MegaPac Line Check 로직수정
			 */

			if (diff >= iPollSendSec
			&&  csi->conntype   == CONN_TYPE_CONNECT
			&&  csi->iPollFlag  == FALSE)
			{
				csi->iPollFlag = TRUE;

				if (flag)
				{
                    /* POLL FFEF 생성 */
					*sSendLine = 0x00;
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL FFEF 송신 출력 */
					if (flag) LOG(_FL_, 0, 0,"[POLL_SEND:%d] LINE CHECK -> (%s:%d) (fd:%d)"
						, diff, csi->ipaddr, csi->port, csi->sock);
				}
				

				if (flag)
				{
					/* POLL FFEFQ 송신 */
					int ret = X25MsgSendFFEF(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0,
							"[POLL_SEND] SESSION CLOSE BY SEND ERROR..(%s:%d)(fd:%d)"
							, csi->ipaddr, csi->port, csi->sock);
						return -1;
					}
				}

				if (flag)
				{
					/* POLL FFEF 송신 출력
					 * FFEF 송신후 에러가 발생하지 않을경우 정상으로 본다.
					 */
				    if (flag) LOG(_FL_, 0, 0,"[POLL_RECV:%d] LINE CHECK OK (%s:%d) (fd:%d)"
						, diff, csi->ipaddr, csi->port, csi->sock);
				}
				
				if (flag)
				{
					/*
					 * 송신 에러가아니므로
					 * lastRecved 시간을 현재시간으로 세팅한다.
					 * Poll 처리를 위해서 flag를 FALSE로 한다.
					 */
					csi->lastRecved = time(NULL);
					csi->iPollFlag = FALSE;
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

			iRecvLen = X25MsgRecvFFEF(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* 수신 에러 */
				if (flag) LOG(_FL_, 0, 0,
					"[MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* 수신길이 0 -> 미처리 */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] Length Zero(0).(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return 0;
			}
			else
			{
				/* 정상 수신 */
				if (!flag) LOG(_FL_, 0, 0, "[MSG_RECV] [%d:%s].(%s:%d) (fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
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

			if (!flag && PollCmpRecvReq(sRecvLine) == 0)
			{
				/*
				 * 1. POLL요청수신 -> POLL응답송신 -> POLL clear
				 */

				if (flag)
				{
					/* 수신 POLL REQ 출력 */
					if (flag) LOG(_FL_, 0, 0, "[POLL_RECV] <- REQ [%d:%s] (%s:%d)(fd:%d)"
						, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
				}

				if (flag)
				{
					/* POLL RES 생성 */
					strcpy(sSendLine, PollMsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES 송신 */
					int ret = X25MsgSendFFEF(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0,
							"[POLL_SEND] SESSION CLOSE BY SEND POLL-RES ERROR..(%s:%d)(fd:%d)"
							, csi->ipaddr, csi->port, csi->sock);
						return -1;
					}
				}

				if (flag)
				{
					/* 송신 POLL RES 출력 */
					if (flag) LOG(_FL_, 0, 0, "[POLL_SEND] RES -> [%d:%s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
				}
			}
			else if (!flag && PollCmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL응답수신 -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[POLL_RECV]    <- RES [%d:%s] (%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
			}
			else
			{
				/*
				 * 3. 일반전문 수신 -> 수신처리( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] <- \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);

                /* 처리여부 : SEND(X) ,RECV(O), SEND/RECV(O) */
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

                /* 처리여부 : SEND(X) ,RECV(O), SEND/RECV(O) */
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

                /* 처리여부 : SEND(X) ,RECV(O), SEND/RECV(O) */
				if (flag)
				{
					/*
					 * FQ에 기록한다.
					 */
					WriteFQ();
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
		/* 확인용 */
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (SendProc) ");
		MSockPrintSockItem(sHdr, csi);
	}

    /* 처리여부 : SEND(O) ,RECV(X), SEND/RECV(O) */
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
			 * FQ에서 전문을 읽는다.
			 */

			*sDataLine = 0x00;

			ReadFQ();

			if (*sDataLine == 0x00)
			{
				return 0;
			}
		}

		if (flag)
		{
			/*
			 * [ 대외기관 송신용 : oxr, oxa ]
			 * FQ에서 읽은 자료에서 BIZ_HDR를 제거한다.
			 * DATA 만 전송한다.
			 */
			strcpy(sSendLine, (char*)&pBizDat->sData);
			iSendLen = strlen(sSendLine);
			
			if (flag) LOG(_FL_, 0, 0, "[HDR_제거] -> [%04d:%s] ", iSendLen, sSendLine);
		}

		if (!flag)
		{
			/* [ 내부 송신용 : oir, ofr ]
			 * FEP_HDR를 추가한다.
			 * FEP_HDR + BIZ_HDR + data 전송한다.
			 */
			iSendLen = sizeof(FEP_HDR) + strlen(sDataLine);

			sprintf(sSendLine, "%04d%-6.6s%-4.4s%02d%08d%02d%-20.20s%1.1s%-3.3s%s"
				, iSendLen - 4
				, sFepid
				, TC_DATA
				, 0
				, SeqGetNo(sFepid)  /* 전문일련번호 */
				, 1
				, TimeGetDateTime()
				, "N"
				, ""
				, sDataLine);
		}

		if (flag)
		{
			int ret = X25MsgSendFFEF(csi->sock, sSendLine, iSendLen);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0,
					"[MSG_SEND] SESSION CLOSE BY SEND REQ ERROR..(%s:%d)(fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return -1;
			}
		}

		if (flag)
		{
			/* 정상 송신 */
			if (flag) LOG(_FL_, 0, 0, "[MSG_SEND] -> \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
				, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
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

	if (!flag) LOG(_FL_, 0, 0, "JUN........");

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
			if (flag) FQ2Init(SEND_FQ, sServiceid);
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
			/* MegaBox / MegaPac을 사용. connect후 negociation 처리 함수 */
			if (flag) MSockConnectSocketX25();

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

	if (flag) LOG(_FL_, 0, 0, "\x1b[41m-----%S ProcessExit -----\x1b[0m", sServiceid);

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

			CfgOxaPoll();
			CfgOxaLine();
			CfgOxa();

			iLLen        = UtilToInt(CfgGetOxa("LENGTH_LEN"));
			iLPls        = UtilToInt(CfgGetOxa("INC_LEN"   ));
			iTimeOutSec  = UtilToInt(CfgGetOxa("TIME_OUT"  ));
			iPollSendSec = UtilToInt(CfgGetOxa("POLL_TIME" ));

			printf("\t iLLen        = [%d]\n", iLLen);
			printf("\t iLPls        = [%d]\n", iLPls);
			printf("\t iTimeOutSec  = [%d]\n", iTimeOutSec);
			printf("\t iPollSendSec = [%d]\n", iPollSendSec);
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
			if (flag) LOG(_FL_, 0, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 0, 0,"LOG_FILE => [%s]", strFile );
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOxa("CONN_INFO"));  /* 접속정보 */
}

#endif