/*
 * PROGRAM ID   : oxameg2.c
 * PROGRAM NAME : oxameg2 ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : oxameg2 ���� ���
 * HISTORY      : 2013.10.02 ����. �����ۼ�
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * HISTORY VERSION
 */
/*----------------------------------------------------------------------------*/

#define       VERSION     "2013.09.11.001: �����۾�\n"              VERSION_002
#define       VERSION_002 "2013.10.07.002: �߰��۾�\n"              VERSION_003
#define       VERSION_003 "2014.05.19.003: ��ŷFEP�����۾�\n"

/*============================================================================*/
/*
 * global variables
 *
 *  typedef struct _FEP_HDR_50_
 *  {
 *      char    sLength     [ 4];  // ��������    : �������� Field�� ������ ������ Length
 *      char    sBsnCode    [ 6];  // �����ĺ��ڵ�: KBBS01
 *      char    sTrCode     [ 4];  // TR-CODE     : LINK,LIOK,DATA,DAOK,POLL,POOK,RJCT ��
 *      char    sResCode    [ 2];  // �����ڵ�    : '00'5. ��������ڵ�ǥ ����
 *      char    sSeq        [ 8];  // �����Ϸù�ȣ: '99999999' ? ����SEQ��ȣ
 *      char    sDataCnt    [ 2];  // Data�Ǽ�    : 01 ~ (2 KByte �̳�)
 *      char    sDateTime   [20];  // �ð�        : YYYYMMDDHHMMSSssssss
 *      char    sAckYn      [ 1];  // ACK����     : Data�� ���� ACK : ���(Y), �̻��(N)
 *      char    sFiller     [ 3];
 *  } FEP_HDR;
 *
 *  typedef struct _BIZ_HDR_100_
 *  {
 *      char    sDataLength [ 4];  // �� Length FIELD�� ������ ���� DATA SIZE
 *      char    sDataSeq    [ 8];  // �ش� Data�� �Ϸù�ȣ
 *      char    sGlobalId   [32];  // GUID
 *      char    sDateTime   [20];  // �ð�        : YYYYMMDDHHMMSSssssss
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
 *      char    sData    [ 1];  // variable length... ��ܱ�� �ۼ����ڷ�
 *  } FEP_BIZ_DAT;
 *
 */
/*----------------------------------------------------------------------------*/
/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define       SEND_FQ         "OAS"           /* �۽��� ���� FQ */
#define       RECV_FQ         "OAR"           /* ������ ���� FQ */

/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)

char          sDataLine       [MAX_DATA_LINE];   /* ���� ó���� �ڷ� */
char          sSendLine       [MAX_DATA_LINE];   /* �۽ſ� �ڷ� */
char          sRecvLine       [MAX_DATA_LINE];   /* ���ſ� �ڷ� */

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

int           iForkFlag            ;      /* fork ó�� flag */
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
 * ���� :
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
			 * FQ�� �д´�.
			FQ* pFq = FQOpen(SEND_FQ, sFepid);
			 */
			FQ2* pFq = FQ2Open(SEND_FQ, sServiceid);

			char* p = FQ2Read(pFq);

			FQ2Close(pFq);

			/*
			 * ���� �ڷḦ sDataLine�� �����Ѵ�.
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
			/* DATE.2014.05.19 : �ߺ� üũ */
			static char sOldDataLine[MAX_DATA_LINE] = { 0x00, };
			
			if (strcmp(sOldDataLine, sDataLine) == 0)
			{
				/* �ߺ� */
				if (flag) LOG(_FL_, 8, 0, "\x1b[41m[WARNING!!]\x1b[0m [FQ_READ](�ߺ�)=[%04d:%s] ", iDataLen, sDataLine);

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
 * ���� :
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
			 * �ڷḦ FQ�� ����Ѵ�.
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
		/* Ȯ�ο� */
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
			 * [ TIME_OUT : SERVER / CLIENT ]
			 *
			 * TimeOut �ð��� �ʰ� �Ǿ����� Ȯ���Ѵ�.
			 * extern int iTimeOutSec  ;
			 *   [����] > TimeOut(��) �̸� close, �ƴϸ� ��������.
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
			 * PollSend �ð��� �Ǿ����� Ȯ���Ѵ�.
			 * extern int iPollSendSec  ;
			 *   [����] > iPollSendSec(��) �̸�
			 *   Poll ������ �߻��ϰ� iPollFlag = TRUE�� �Ѵ�.
			 *
			 * Poll �۽��� CONNECT�� �� ������ ������ �Ѵ�..
                         * (JUN) 2014-03-12 MegaBox / MegaPac Line Check ��������
			 */

			if (diff >= iPollSendSec
			&&  csi->conntype   == CONN_TYPE_CONNECT
			&&  csi->iPollFlag  == FALSE)
			{
				csi->iPollFlag = TRUE;

				if (flag)
				{
                    /* POLL FFEF ���� */
					*sSendLine = 0x00;
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL FFEF �۽� ��� */
					if (flag) LOG(_FL_, 0, 0,"[POLL_SEND:%d] LINE CHECK -> (%s:%d) (fd:%d)"
						, diff, csi->ipaddr, csi->port, csi->sock);
				}
				

				if (flag)
				{
					/* POLL FFEFQ �۽� */
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
					/* POLL FFEF �۽� ���
					 * FFEF �۽��� ������ �߻����� ������� �������� ����.
					 */
				    if (flag) LOG(_FL_, 0, 0,"[POLL_RECV:%d] LINE CHECK OK (%s:%d) (fd:%d)"
						, diff, csi->ipaddr, csi->port, csi->sock);
				}
				
				if (flag)
				{
					/*
					 * �۽� �������ƴϹǷ�
					 * lastRecved �ð��� ����ð����� �����Ѵ�.
					 * Poll ó���� ���ؼ� flag�� FALSE�� �Ѵ�.
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
		/* Ȯ�ο� */
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

			iRecvLen = X25MsgRecvFFEF(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* ���� ���� */
				if (flag) LOG(_FL_, 0, 0,
					"[MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* ���ű��� 0 -> ��ó�� */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] Length Zero(0).(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return 0;
			}
			else
			{
				/* ���� ���� */
				if (!flag) LOG(_FL_, 0, 0, "[MSG_RECV] [%d:%s].(%s:%d) (fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
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

			if (!flag && PollCmpRecvReq(sRecvLine) == 0)
			{
				/*
				 * 1. POLL��û���� -> POLL����۽� -> POLL clear
				 */

				if (flag)
				{
					/* ���� POLL REQ ��� */
					if (flag) LOG(_FL_, 0, 0, "[POLL_RECV] <- REQ [%d:%s] (%s:%d)(fd:%d)"
						, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
				}

				if (flag)
				{
					/* POLL RES ���� */
					strcpy(sSendLine, PollMsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES �۽� */
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
					/* �۽� POLL RES ��� */
					if (flag) LOG(_FL_, 0, 0, "[POLL_SEND] RES -> [%d:%s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
				}
			}
			else if (!flag && PollCmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL������� -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[POLL_RECV]    <- RES [%d:%s] (%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
			}
			else
			{
				/*
				 * 3. �Ϲ����� ���� -> ����ó��( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] <- \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);

                /* ó������ : SEND(X) ,RECV(O), SEND/RECV(O) */
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

                /* ó������ : SEND(X) ,RECV(O), SEND/RECV(O) */
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

                /* ó������ : SEND(X) ,RECV(O), SEND/RECV(O) */
				if (flag)
				{
					/*
					 * FQ�� ����Ѵ�.
					 */
					WriteFQ();
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
		/* Ȯ�ο� */
		char sHdr[50];
		sprintf(sHdr, "\t -> CSI (SendProc) ");
		MSockPrintSockItem(sHdr, csi);
	}

    /* ó������ : SEND(O) ,RECV(X), SEND/RECV(O) */
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
			 * FQ���� ������ �д´�.
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
			 * [ ��ܱ�� �۽ſ� : oxr, oxa ]
			 * FQ���� ���� �ڷῡ�� BIZ_HDR�� �����Ѵ�.
			 * DATA �� �����Ѵ�.
			 */
			strcpy(sSendLine, (char*)&pBizDat->sData);
			iSendLen = strlen(sSendLine);
			
			if (flag) LOG(_FL_, 0, 0, "[HDR_����] -> [%04d:%s] ", iSendLen, sSendLine);
		}

		if (!flag)
		{
			/* [ ���� �۽ſ� : oir, ofr ]
			 * FEP_HDR�� �߰��Ѵ�.
			 * FEP_HDR + BIZ_HDR + data �����Ѵ�.
			 */
			iSendLen = sizeof(FEP_HDR) + strlen(sDataLine);

			sprintf(sSendLine, "%04d%-6.6s%-4.4s%02d%08d%02d%-20.20s%1.1s%-3.3s%s"
				, iSendLen - 4
				, sFepid
				, TC_DATA
				, 0
				, SeqGetNo(sFepid)  /* �����Ϸù�ȣ */
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
			/* ���� �۽� */
			if (flag) LOG(_FL_, 0, 0, "[MSG_SEND] -> \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
				, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : /hw03/ibridge/DAT2/YYYYMMDD/PS_ALIVE ���Ͽ� ALIVE�� ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int CheckAlive()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "JUN........");

	if (flag)
	{
		/* ���μ��� �⵿ �ʱ�ȭ */
		if (iProcessAlive == 0 ) {
			iProcessAlive = time(NULL);
			SigAliveWrite(sServiceid);
		}
		
		iProcessDiff = time(NULL) - iProcessAlive;
		
		/* 1�и��� Alive ��� */
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
 * ���� :
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
			/* ����� FQ�� �����Ѵ�. */
            /* ó������ : SEND(O) ,RECV(X), SEND/RECV(O) */
			if (flag) FQ2Init(SEND_FQ, sServiceid);
            /* ó������ : SEND(X) ,RECV(O), SEND/RECV(O) */
			if (flag) FQ2Init(RECV_FQ, sServiceid);
		}

		if (flag)
		{
			/* ���������� �Ľ��Ͽ� �����Ѵ�. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			/* ���μ��� ����üũ */
			if (flag) CheckAlive();
				
			/* server / accept / connect ������ �����ϰ� ������ �����Ѵ�. */
			/* MegaBox / MegaPac�� ���. connect�� negociation ó�� �Լ� */
			if (flag) MSockConnectSocketX25();

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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �⺻ ó��
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
		 * ���μ������� ����ϴ� ���񽺸�� FEPID�� ��´�.
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
		 * master ȯ�������� �д´�. biz.properties ���ϵ� �д´�.
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

			printf("\n���α׷��� �����մϴ�.\n");
		}
	}

	if (flag && iForkFlag)
	{
		/*
		 * fork�� �̿��Ͽ� child process �� �����Ѵ�.
		 */
		setpgrp();
		switch(fork())
		{
			case -1: printf("ERROR : fork error : [errno=%d]\n", errno); exit(9);
			case  0: break;    /* child process */
			default: exit(0);  /* parent process */
		}

		/*
		 * ǥ������� �����Ѵ�.
		 */
		if (!flag) freopen("/dev/null", "w", stdout);

		/*
		 * ǥ������� LOGó���Ѵ�.
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
				/* ���ʻ��� */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND ��� */
				freopen(strFile, "a+", stdout);
			}
			if (flag) LOG(_FL_, 0, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 0, 0,"LOG_FILE => [%s]", strFile );
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOxa("CONN_INFO"));  /* �������� */
}

#endif