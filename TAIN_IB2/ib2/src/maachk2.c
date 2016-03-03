/*
 * PROGRAM ID   : oxrcom2.c
 * PROGRAM NAME : oxrcom2 ���
 * AUTHOR       : (��)ICA
 * COMMENT      : oxrcom2 ���
 *                ��ܱ�� ���� ���(����)
 * HISTORY      : 2013.09.10 ����. �����ۼ�
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

#define       SEND_FQ         "OFS"           /* �۽��� ���� FQ */
#define       RECV_FQ         "OFR"           /* ������ ���� FQ */

#define       MAAPER          "MAACOM0000"    /* ����           */

/*----------------------------------------------------------------------------*/

#define       MAX_DATA_LINE        (10*1024)
#define       POLL_TIME            10

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
					strcpy(sSendLine, "1285M1.0<����TEST>108001007_1->0800_1190I_KBB01   0600              00035000                                                                                    KBB0100411080010071HANWHA            0490      50500308    0045050030800400000IQ20140611154240000000000000002014061115424020140611154240              0000000002    1115424008001191  0S0Y0000                                                                0000000000005050030801 01       000000T320351037 0000000000000000000000000                                                                                                                         2014061100700000000                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    ");
					iSendLen = strlen(sSendLine);
					
					if (flag) LOG(_FL_, 0, 0, "[POLL_MAKE] [%d]sec [%d:%s]", diff, iSendLen, sSendLine);    
				}

				if (flag)
				{
					/* POLL REQ �۽� */
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
					/* POLL REQ ��� */
					if (flag) LOG(_FL_, 0, 0, "[CHK_SEND] -> [%d:%s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
                    if (flag) LOG(_FL_, 5, 0, "[CHK_SEND] -> [%d:%50.50s] (%s:%d)(fd:%d)"
						, iSendLen, sSendLine, csi->ipaddr, csi->port, csi->sock);
				}
			}
			else
			{
				if (!flag) LOG(_FL_, 0, 0, "POLL �۽����� ����");
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

			iRecvLen = MSockMsgRecv(csi->sock, sRecvLine);
			if (iRecvLen < 0)
			{
				/* ���� ���� */
				if (flag) LOG(_FL_, 9, 0,
					"[MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* ���ű��� 0 -> ��ó�� */
				if (flag) LOG(_FL_, 9, 0, "[MSG_RECV] Length Zero(0).(%s:%d) (fd:%d)"
					, csi->ipaddr, csi->port, csi->sock);
				return 0;
			}
			else
			{
				/* ���� ���� */
				if (!flag) LOG(_FL_, 5, 0, "[MSG_RECV] [%d:%s].(%s:%d) (fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
			}
		}

		if (flag)
		{
			/*
			 * ���� ALIVE CHECK
			 */
			SigAliveWrite(MAAPER);
			
			{
				/*
				 * 3. �Ϲ����� ���� -> ����ó��( WriteFQ )
				 */
				if (flag) LOG(_FL_, 0, 0, "[MSG_RECV] <- \x1b[42m[%04d:%s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
                if (flag) LOG(_FL_, 5, 0, "[MSG_RECV] <- \x1b[42m[%04d:%50.50s]\x1b[0m.(%s:%d)(fd:%d)"
					, iRecvLen, sRecvLine, csi->ipaddr, csi->port, csi->sock);
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
 * ���� : /hw03/ibridge/DAT2/YYYYMMDD/PS_ALIVE ���Ͽ� ALIVE�� ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int CheckAlive()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (!flag) LOG(_FL_, 0, 0, "CheckAlive...");

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
			if (!flag) FQ2Init(SEND_FQ, sServiceid);
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
			if (flag) MSockMakeSockInfo();

			/* accept / connect ������ üũ�Ѵ�. TimeOut, Poll üũ */
			if (flag) MSockCheckHandler(CheckProc);

			/* ������ recv ó���� �ٷ�� handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (!flag) sleep(5);
			if (flag) usleep(20 * 1000); /* 0.02�� */
				
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

			CfgPoll(); /* Default Poll */
			CfgOfs2();
			iPollSendSec = POLL_TIME;
			
			iLLen = 4;
			iLPls = 0;
			printf("\t iLLen = [%d]\n", iLLen);
			printf("\t iLPls = [%d]\n", iLPls);
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
			if (flag) LOG(_FL_, 9, 0, "\x1b[44m---------- process START (%s) ----------\x1b[0m" , sServiceid);
			if (flag) LOG(_FL_, 9, 0,"LOG_FILE => [%s]", strFile );
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

	if (flag) MainService(CfgGetOfs2("SVR_INFO"));  /* �������� */
}

#endif