/*
 * PROGRAM ID   : bxs.c
 * PROGRAM NAME : bxs ���
 * AUTHOR       : (��)ICA
 * COMMENT      : bxs ���
 * HISTORY      : 2013.08.04 ����. �����ۼ�
 *
 * �Ｚ��Ʈ�� ��ġ���� �۽�
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: �����۾�"

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define   RECONN_SEC               5
#define   MAX_DATA_LINE      8192000

#define   BLOCK_RECS             100
#define   MAX_MSG_DTL           2000

#define   MSGTYPE_LOGIN_REQ       10
#define   MSGTYPE_LOGIN_RES       20
#define   MSGTYPE_LOGOUT_REQ      30
#define   MSGTYPE_LOGOUT_RES      40
#define   MSGTYPE_FILE_REQ        50
#define   MSGTYPE_FILE_RES        60
#define   MSGTYPE_CHECK           70
#define   MSGTYPE_NONE            90        /* not defined. */

#define   FLAG_DANGBAL          TRUE        /* ��߹߻� */

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * ��ġ ���� ��� : �Ｚ��Ʈ��
 */
/*----------------------------------------------------------------------------*/

typedef MSG_SSN_HDR     MSG_HDR   ;
typedef MSG_SSN_LOGIN   MSG_LOGIN ;
typedef MSG_SSN_LOGOUT  MSG_LOGOUT;
typedef MSG_SSN_FILE    MSG_FILE  ;
typedef MSG_SSN_CHECK   MSG_CHECK ;
typedef MSG_SSN_LINE    MSG_LINE  ;

MSG_HDR   *             pHdr      ;
MSG_LINE  *             pLine     ;

MSG_LOGIN *             pLogin    ;
MSG_LOGOUT*             pLogout   ;
MSG_FILE  *             pFile     ;
MSG_CHECK *             pCheck    ;

BAT_INF   *             pInf      ;

/*----------------------------------------------------------------------------*/

int             iForkFlag       ;

int             iRSeq           ;                  /* read sequence number */

char            sBatInf         [MAX_DATA_LINE];   /* FEP ��ġ���� from FQ */

char            sFileInf        [MAX_DATA_LINE];   /* ��ġ��������         */

char            sSendLine       [MAX_DATA_LINE];   /* �۽����� */
int             iSendLen        ;

char            sRecvLine       [MAX_DATA_LINE];   /* �������� */
int             iRecvLen        ;

char            sBatHdr         [MAX_DATA_LINE];   /* ��ġ������ ����� */

char            sDataLine       [MAX_DATA_LINE];   /* ��ġ������ �����ͺ� */
int             iDataLen        ;

/*----------------------------------------------------------------------------*/

char            sUserid   [20+1];                  /* Userid          */
char            sPasswd   [20+1];                  /* Passwd          */
char            sFileId   [20+1];                  /* �ۼ��� ���ϸ�   */

char            sFileName [ 512];                  /* ���ϸ�          */

FILE*           fp              ;                  /* ��ġ���� fp     */

int             iRecLen         ;
int             iRecCnt         ;

int             iTrCodeSeq      ;                  /* TrCode Sequence : 01 ~ 99 */

char            sSequence [  50];
char            sSubject  [  50];


/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : FQ�� ������ �о� �����ͺ��� BAT_INF ����ü�� ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
void PrintBatInf()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatInf == 0x00)
			return;

		pInf = (BAT_INF*) &sBatInf;

		printf("---- BAT_INF* pInf --------------------------------\n");
		printf("[%s]\n", sBatInf);
		printf("\tpInf->sRecLen    = [%*.*s]\n", sizeof(pInf->sRecLen   ), sizeof(pInf->sRecLen   ), pInf->sRecLen   );
		printf("\tpInf->sRecCnt    = [%*.*s]\n", sizeof(pInf->sRecCnt   ), sizeof(pInf->sRecCnt   ), pInf->sRecCnt   );
		printf("\tpInf->sSR        = [%*.*s]\n", sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR       );
		printf("\tpInf->sOrgCode   = [%*.*s]\n", sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode  );
		printf("\tpInf->sTrCode    = [%*.*s]\n", sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode   );
		printf("\tpInf->sTrCodeLen = [%*.*s]\n", sizeof(pInf->sTrCodeLen), sizeof(pInf->sTrCodeLen), pInf->sTrCodeLen);
		printf("\tpInf->sDate      = [%*.*s]\n", sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate     );
		printf("\tpInf->sTime      = [%*.*s]\n", sizeof(pInf->sTime     ), sizeof(pInf->sTime     ), pInf->sTime     );
		printf("\tpInf->sTrCodeSeq = [%*.*s]\n", sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq);
		printf("\tpInf->sFiller    = [%*.*s]\n", sizeof(pInf->sFiller   ), sizeof(pInf->sFiller   ), pInf->sFiller   );
		printf("---------------------------------------------------\n");
	}

	if (flag)
	{
		printf("---- bxa_trlst.cfg --------------------------------\n");
		printf("\tSERVICE = [%s]\n", CfgGetBxaTr("SERVICE"));
		printf("\tFEPID   = [%s]\n", CfgGetBxaTr("FEPID"  ));
		printf("\tORGCD   = [%s]\n", CfgGetBxaTr("ORGCD"  ));
		printf("\tSNDRCV  = [%s]\n", CfgGetBxaTr("SNDRCV" ));
		printf("\tFILEID  = [%s]\n", CfgGetBxaTr("FILEID" ));
		printf("\tTRCODE  = [%s]\n", CfgGetBxaTr("TRCODE" ));
		printf("\tTRCDLEN = [%s]\n", CfgGetBxaTr("TRCDLEN"));
		printf("\tRECLEN  = [%s]\n", CfgGetBxaTr("RECLEN" ));
		printf("\tCOMMENT = [%s]\n", CfgGetBxaTr("COMMENT"));
		printf("---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * ���� : FQ�� ������ �д´�. sBatInfo ���� �����Ѵ�. �׸��� BAT_INF ���.
 */
/*----------------------------------------------------------------------------*/
int ReadBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		FQ2* pFq = FQ2Open("OAS", sServiceid);

		char* p = FQ2Read(pFq);
		if (p == NULL)
		{
			FQ2Close(pFq);
			return -1;
		}
		strcpy(sBatInf, p);

		FQ2Close(pFq);
	}

	if (flag)
	{
		/*
		 * ��ġ�������� sSR, sTrCode�� bxa_trlst.cfg ���� ������ �д´�.
		 */
		char sSR[10];
		char sTrCode[20];
		pInf = (BAT_INF*) &sBatInf;
		sprintf(sSR    , "%-*.*s", sizeof(pInf->sSR    ), sizeof(pInf->sSR    ), pInf->sSR    );
		sprintf(sTrCode, "%-*.*s", sizeof(pInf->sTrCode), sizeof(pInf->sTrCode), pInf->sTrCode);

		if (flag) printf("STATUS: CfgBxaTr('%s', '%s');\n", sSR, sTrCode);

		CfgBxaTr(sSR, sTrCode);
	}

	if (flag)
	{
		/*
		 * ���ϸ��� �����.
		 */
		pInf = (BAT_INF*) &sBatInf;
		sprintf(sFileName, CfgGetMaster("ib.path.file.bxscom")
			, sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR
			, sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode
			, sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode
			, sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate
			, sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq
			);
		UtilSetYYYYMMDD(sFileName);

		iRecLen = UtilToIntN(pInf->sRecLen, sizeof(pInf->sRecLen));
		iRecCnt = UtilToIntN(pInf->sRecCnt, sizeof(pInf->sRecCnt));

		/* ��ġ���� ������ ��� */
		if (flag) LOG(_FL_, 0, 0, "INF: BAT_FILE [RecLen=%d][RecCnt=%d][%s]"
			, iRecLen, iRecCnt, sFileName);
	}

	if (flag) PrintBatInf();

	return 0;
}

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/

/*============================================================================*/
/*
 * ���� : BAT_INF ����ü�� ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
void PrintBatHdr()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatHdr == 0x00)
			return;

		MSG_HDR* p = (MSG_HDR*) &sBatHdr;

		printf("---- MSG_HDR* p --------------------------------\n");
		printf("[%*.*s]\n", sizeof(MSG_HDR), sizeof(MSG_HDR), sBatHdr);
		printf("\tp->sTrCode   = [%*.*s]\n", sizeof(p->sTrCode)  , sizeof(p->sTrCode)  , p->sTrCode  );
		printf("\tp->sTranNum  = [%*.*s]\n", sizeof(p->sTranNum) , sizeof(p->sTranNum) , p->sTranNum );
		printf("\tp->sResCode  = [%*.*s]\n", sizeof(p->sResCode) , sizeof(p->sResCode) , p->sResCode );
		printf("\tp->sReserved = [%*.*s]\n", sizeof(p->sReserved), sizeof(p->sReserved), p->sReserved);
		printf("---------------------------------------------------\n");
	}
}

/*============================================================================*/
/*
 * ���� : sDataLine �ڷḦ �۽��Ѵ�.
 */
/*----------------------------------------------------------------------------*/
BOOL SendData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			iSendLen = strlen(sSendLine);

			/*
			 * �۽��Ѵ�.
			 */
			int ret = SockSend(cSock, sSendLine, iSendLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		/*
		 * �۽ŵ����͸� ����Ѵ�.
		 */
		if (flag)
		{
			LOG(_FL_, 0, 0, "SEND(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), sSendLine
				, sSendLine + sizeof(MSG_HDR));
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * ���� : ������ �ڷḦ sDataLine �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
BOOL RecvData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			iRecvLen = 150;

			/*
			 * 150����Ʈ ������ �д´�.
			 * HDR(40) + CMD(110)
			 */
			int ret = SockRecv(cSock, sRecvLine, iRecvLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		sRecvLine[iRecvLen] = 0x00;

		/*
		 * ���ŵ����͸� ����Ѵ�.
		 */
		if (flag)
		{
			LOG(_FL_, 0, 0, "RECV(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_HDR), sizeof(MSG_HDR), sRecvLine
				, sRecvLine + sizeof(MSG_HDR));
		}
	}

	return TRUE;
}

/*============================================================================*/
/*
 * ���� : �������� (���/Ÿ��)   0600/001 -> 0610/001
 */
/*----------------------------------------------------------------------------*/
int GetMsgType(char* sData)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		pHdr = (MSG_HDR*) sData;

		switch (UtilToIntN(pHdr->sTranNum, sizeof(pHdr->sTranNum)))
		{
			case   3: return MSGTYPE_LOGIN_REQ ;
			case  30: return MSGTYPE_LOGIN_RES ;
			case   7: return MSGTYPE_LOGOUT_REQ;
			case  70: return MSGTYPE_LOGOUT_RES;
			case 100: return MSGTYPE_FILE_REQ  ;
			case 110: return MSGTYPE_FILE_RES  ;
			case 130: return MSGTYPE_CHECK     ;
			default:
				if (flag) LOG(_FL_, 0, 0, "[GetMsgType]...not defined[%s] ", sRecvLine);
				return MSGTYPE_NONE;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * ���� : Recv LOGIN
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // �����ڵ�
 *     sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *     sReserved [   5];  // ����
 *     sUserid   [  20];  // VAN �����ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(�۽�), RD(����), RS(�۽�List), RR(����List)
 *     sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *     sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *     sFlag     [   1];  // ���Ž� E(�̼���),R(�����),A(�̼���+�����)
 *     sStTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sEdTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sYnChgPw  [   1];  // Y : change password
 *     sNewPass  [   8];  // new password
 *     sCommSize [   4];  // default : 2048
 *     sChgCode  [   1];  // Code��ȯ����
 *     sTransId  [  10];  // �ŷ�óID
 *     sFiller   [  11];  // ����
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatLogin(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%1.1s%-8.8s%-4.4s%1.1s%-10.10s%-11.11s"
			, ""                                 /* sTrCode   */
			, "030"                              /* sTranNum  */
			, ""                                 /* sResCode  */
			, ""                                 /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS"                           /* sUserid   */
			, "HANCMS"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                                 /* sSubject  */
			, "E"                                /* sFlag     */
			, ""                                 /* sStTime   */
			, ""                                 /* sEdTime   */
			, ""                                 /* sYnChgPw  */
			, ""                                 /* sNewPass  */
			, "2048"                             /* sCommSize */
			, "N"                                /* sChgCode  */
			, ""                                 /* sTransId  */
			, ""                                 /* sFiller   */
		);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : Send LOGIN
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // �����ڵ�
 *     sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *     sReserved [   5];  // ����
 *     sUserid   [  20];  // VAN �����ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(�۽�), RD(����), RS(�۽�List), RR(����List)
 *     sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *     sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *     sFlag     [   1];  // ���Ž� E(�̼���),R(�����),A(�̼���+�����)
 *     sStTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sEdTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sYnChgPw  [   1];  // Y : change password
 *     sNewPass  [   8];  // new password
 *     sCommSize [   4];  // default : 2048
 *     sChgCode  [   1];  // Code��ȯ����
 *     sTransId  [  10];  // �ŷ�óID
 *     sFiller   [  11];  // ����
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatLogin(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sStTime[20];
		char sEdTime[20];
		
		if (flag)
		{
			char sYear[10];
			sprintf(sYear, "%4.4s", TimeGetYear());
			
			if (strcmp(sYear, "2014") <= 0)
			{
				/* INFO : 20140102 : ���켮������ ���ϱ��� ��ġ������ �����޾� ó���� */
				sprintf(sStTime, "201401030000");
				sprintf(sEdTime, "201412312350");
			}
			else
			{
				/* 2015�� ���ĺ��� ���� �� */
				sprintf(sStTime, "%4.4s01010000", sYear);
				sprintf(sEdTime, "%4.4s12312350", sYear);
			}
		}

		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%1.1s%-8.8s%-4.4s%1.1s%-10.10s%-11.11s"
			, "         "                        /* sTrCode   */
			, "003"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, "HANCMS             "              /* sUserid   */
			, "HANCMS "                          /* sPasswd   */
			, "SD"                               /* sJobType  */
			, "          "                       /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "E"                                /* sFlag     */
			, sStTime                            /* sStTime   */
			, sEdTime                            /* sEdTime   */
			, " "                                /* sYnChgPw  */
			, "        "                         /* sNewPass  */
			, "2048"                             /* sCommSize */
			, "N"                                /* sChgCode  */
			, "          "                       /* sTransId  */
			, "           "                      /* sFiller   */
		);


		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * ���� : FILE INFO
 *
 *    sTrCode   [   9];  // Transaction Code
 *    sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *    sResCode  [   3];  // �����ڵ�
 *    sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *    sReserved [   5];  // ����
 *    sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *    sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *    sFileSize [  10];  // �۽��� ������ size
 *    sSendId   [  20];  // �۽��� ID
 *    sRecvId   [  20];  // ������ ID
 *    sFiller1  [   1];  // ����-1
 *    sFiller2  [  10];  // ����-2
 *    sYnLast   [   3];  // ���������� ���� NXT:�������� END:��������
 *    sTranType [   3];  // �������� : 'NEW'
 *    sFiller3  [   1];  // ����-3
 *    sFiller4  [  22];  // ����-4
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileInfo(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		int iFileSize = 0;

		if (flag)
		{
			struct stat fst;
			lstat(sFileName, &fst);
			iFileSize = fst.st_size;
		}

			iRecLen = UtilToIntN(pInf->sRecLen, sizeof(pInf->sRecLen));
			iRecCnt = UtilToIntN(pInf->sRecCnt, sizeof(pInf->sRecCnt));


		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-10.10s%-10.10s%010d%-20.20s%-20.20s%1.1s%-10.10s%-3.3s%-3.3s%1.1s%-22.22s"
			, "         "                        /* sTrCode   */
			, "100"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, iRecLen + 34                       /* sFileSize */
			, "HANCMS"                           /* sSendId   */
			, "HHFEDI"                           /* sRecvId   */
			/*, "SENDER"     */                  /* sSendId   */
			/*, "RECEIVER" */                    /* sRecvId   */
			, ""                                 /* sFiller1  */
			, ""                                 /* sFiller2  */
			, "NXT"                              /* sYnLast   */
			, "NEW"                              /* sTranType */
			, " "                                /* sFiller3  */
			, "                      "           /* sFiller4  */
		);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : DATA
 *
 *     sHdrType  [  10];   // �������
 *     sBizType  [   1];   // ��������
 *     sDatType  [   1];   // �ڷ�����
 *     sFileSize [  10];   // ����ũ��
 *     sCompSize [  10];   // ����������ũ��
 *     sSendId   [  10];   // �۽���ID
 *     sUpcheGb  [   2];   // ��ü����
 *     sInfoGb   [   3];   // ��������
 *     sFiller   [   1];   // ����
 *     sRecvCnt  [   3];   // �����ڼ�
 *     sRecvId   [  10];   // ������ID
 *     sRecSep   [   2];   // ���ڵ屸����
 *     sData     [   1];   // ������ �ڷ� MAX = 1985
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatFileData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sData[2048*100];
		char sLine[4096];
		int iRead;

		*sData = 0x00;
		
		fp = fopen(sFileName, "r");

		while (TRUE)
		{
			iRead = fread(sLine, 1, 1985, fp);
			if (iRead == 0)
				break;

			sLine[iRead] = 0x00;

			strcat(sData, sLine);
		}

		fclose(fp);


		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%-10.10s%1.1s%1.1s%010d%-10.10s%-10.10s%-2.2s%-3.3s%1.1s%-3.3s%-10.10s%-2.2s%s"
			, "<<NEHEAD>>"                       /* sHdrType  */
			, "1"                                /* sBizType  */
			, "N"                                /* sDatType  */
			, strlen(sData)                      /* sFileSize */  /* TODO : DATE.20130924 ���� �ʿ� */
			, ""                                 /* sCompSize */
			, ""                                 /* sSendId   */
			, ""                                 /* sUpcheGb  */
			, ""                                 /* sInfoGb   */
			, ""                                 /* sFiller   */
			, "1"                                /* sRecvCnt  */
			, ""                                 /* sRecvId   */
			, "LF"                               /* sRecSep   */
			, sData                              /* sData     */
		);

		if (flag) printf("DATA : [%s]\n", sSendLine);

		if (flag)
		{
			iSendLen = strlen(sSendLine);

			/*
			 * �۽��Ѵ�.
			 */
			int ret = SockSend(cSock, sSendLine, iSendLen);
			if (ret <= 0)
			{
				CSockFDCLR(cSock);
				return FALSE;
			}
		}

		/*
		 * �۽ŵ����͸� ����Ѵ�.
		 */
		if (!flag)
		{
			LOG(_FL_, 0, 0, "SEND(%d): HDR+DATA [%*.*s][%s]"
				, cSock, sizeof(MSG_LINE)-1, sizeof(MSG_LINE)-1, sSendLine
				, sSendLine + sizeof(MSG_LINE)-1);
		}

	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : CHECK
 */
/*----------------------------------------------------------------------------*/
int RecvBatCheck(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * ���� : Recv LOGOUT
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // �����ڵ�
 *     sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *     sReserved [   5];  // ����
 *     sUserid   [  20];  // VAN �����ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(�۽�), RD(����), RS(�۽�List), RR(����List)
 *     sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *     sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *     sFlag     [   1];  // ���Ž� E(�̼���),R(�����),A(�̼���+�����)
 *     sStTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sEdTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sFiller   [  35];  // ����
 *
 */
/*----------------------------------------------------------------------------*/
int RecvBatLogout(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%-35.35s"
			, ""                                 /* sTrCode   */
			, "070"                              /* sTranNum  */
			, ""                                 /* sResCode  */
			, ""                                 /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS"                           /* sUserid   */
			, "HANCMS"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "A"                                /* sFlag     */
			, ""                                 /* sStTime   */
			, ""                                 /* sEdTime   */
			, ""                                 /* sFiller   */
		);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : Send LOGOUT
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // �����ڵ�
 *     sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *     sReserved [   5];  // ����
 *     sUserid   [  20];  // VAN �����ID
 *     sPasswd   [   8];  // VAN Password
 *     sJobType  [   2];  // SD(�۽�), RD(����), RS(�۽�List), RR(����List)
 *     sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *     sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *     sFlag     [   1];  // ���Ž� E(�̼���),R(�����),A(�̼���+�����)
 *     sStTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sEdTime   [  12];  // ���Ž� YYYYMMDDhhmm
 *     sFiller   [  35];  // ����
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatLogout(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sStTime[20];
		char sEdTime[20];
		
		if (flag)
		{
			char sYear[10];
			sprintf(sYear, "%4.4s", TimeGetYear());
			
			if (strcmp(sYear, "2014") <= 0)
			{
				/* INFO : 20140102 : ���켮������ ���ϱ��� ��ġ������ �����޾� ó���� */
				sprintf(sStTime, "201401030000");
				sprintf(sEdTime, "201412312350");
			}
			else
			{
				/* 2015�� ���ĺ��� ���� �� */
				sprintf(sStTime, "%4.4s01010000", sYear);
				sprintf(sEdTime, "%4.4s12312350", sYear);
			}
		}

		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-20.20s%-8.8s%-2.2s%-10.10s%-10.10s%1.1s%-12.12s%-12.12s%-35.35s"
			, "         "                        /* sTrCode   */
			, "007"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, ""                                 /* sReserved */
			, "HANCMS             "              /* sUserid   */
			, "HANCMS "                          /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, "FS082004"                         /* sSubject  */
			, "E"                                /* sFlag     */
			, sStTime                            /* sStTime   */
			, sEdTime                            /* sEdTime   */
			, "                         "        /* sFiller   */
		);

		/*
		 * �۽��Ѵ�. �۽ŵ����ʹ� sDataLine
		 */
		if (SendData(cSock) == FALSE)
		{
			return -1;
		}

		/*
		 * �����Ѵ�. ���ŵ����ʹ� sDataLine
		 */
		if (RecvData(cSock) == FALSE)
		{
			return -1;
		}
	}

	return 0;
}

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*
 * ���� : initialize socket information
 */
/*----------------------------------------------------------------------------*/
int SendBatFile(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret;

	if (flag)
	{
		if (flag)
		{
			if (FLAG_DANGBAL)
			{
				/*
				 * Login (���߻�)
				 */
				ret = SendBatLogin(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
			else
			{
				/*
				 * Login (Ÿ��߻�)
				 */
				ret = RecvBatLogin(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
		}

		while (TRUE)
		{
			if (flag)
			{
				if (flag) printf("STATUS : ReadBatFQ()..\n");

				/*
				 * FQ���� �۽��ڷḦ �д´�.
				 * �۽��ڷᰡ ������ *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
					if (flag) break;
					
					/*sleep(5 * 60); */
					sleep(10);
					continue;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : SendBatFileInfo()..\n");

				/*
				 * ��������
				 */
				ret = SendBatFileInfo(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : SendBatFileData()..\n");

				/*
				 * DATA�۽�
				 */
				ret = SendBatFileData(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : RecvBatCheck()..\n");

				/*
				 * ����Ȯ��
				 */
				ret = RecvBatCheck(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag) break;
			
			sleep(30);
		}

		if (flag)
		{
			if (FLAG_DANGBAL)
			{
				/*
				 * Logout (���߻�)
				 */
				ret = SendBatLogout(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
			else
			{
				/*
				 * Logout (Ÿ��߻�)
				 */
				ret = RecvBatLogout(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}
		}
	}

	if (flag)
	{
		/* ������ �����Ѵ�. */
		CSockFDCLR(cSock);

		if (flag) LOG(_FL_, 0, 0, "STATUS : close session");
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
			printf("Program : [%s], Version : %s\n", sProgName, VERSION);
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

		if (flag) printf("\x1b[44m< %s %s %s >\x1b[0m\n", sProgName, sServiceid, sFepid);
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

		CfgBxa();

		if (!flag)
		{
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.version"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.serialkey"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.author"));
			printf("CfgGetMaster [%s]\n", CfgGetMaster("ib.company"));
			printf("CfgGetBiz    [%s]\n", CfgGetBiz("RUN_SCRIPT"));

			printf("CfgGetBxa    [%s]\n", CfgGetBxa("ORG_DESC"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SND_HOST"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SND_PORT"));
			printf("Port         [%d]\n", UtilToInt(CfgGetBxa("SND_PORT")));

			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_ID"));
			printf("CfgGetBxa    [%s]\n", CfgGetBxa("SENDER_PWD"));

			strcpy(sUserid, CfgGetBxa("SENDER_ID" ));
			strcpy(sPasswd, GetPassword("KFT51", 1, "HCMSTEST", CfgGetBxa("SENDER_PWD"), "269" ));

			printf("CfgGetBxa    [%s]\n", sPasswd);
		}

		if (!flag)
		{
			char sOrg[100];
			char sEnc[100];
			char sDec[100];

			strcpy(sOrg, CfgGetBxa("SENDER_PWD"));

			strcpy(sEnc, GetPassword("KFT51", 1, "HCMSTEST", sOrg, "243" ));
			printf("OUTPUT ���ڵ���ȣ=[%s]\n", sEnc );

			/*
			 *  ��ȣȭ
			 */
			strcpy(sDec, GetPassword("KFT51", 2, "HCMSTEST", sEnc, "243" ));
			printf("OUTPUT ���ڵ���ȣ=[%s]\n", sDec );
		}
	}

	if (!flag && iForkFlag)
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
		 * ǥ������� LOGó���Ѵ�.
		 */
		if (flag)
		{
			char strPath[128];
			char strFile[128];

			sprintf(strPath, CfgGetMst("ib.path.log"));
			UtilSetYYYYMMDD(strPath);
			mkdir(strPath, 0777);

			if (flag) sprintf(strFile, "%s/%s", strPath, sServiceid);
			if (!flag) sprintf(strFile, "%s/%s", strPath, sFepid);   /* ERROR : �� ���μ��� ���� */
			if (flag) LOG(_FL_, 0, 0, "LOG FILE => [%s]\n", strFile);

			if (access(strFile, R_OK) < 0)
			{
				/* ���ʻ��� */
				freopen(strFile, "w+", stdout);
			}
			else
			{
				/* APPEND ��� */
				freopen(strFile, "a+", stdout);
			}

			if (flag) LOG(_FL_, 0, 0, "[%s]\n", strFile);
			if (flag) LOG(_FL_, 0, 0, "[%s]\n", TimeGetDateTime2());
			fflush(stdout);
		}

		if (!flag) exit(0);
	}

	/******************************************************************************/

    if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
    	
    SigSetSignal();

	if (FLAG_DANGBAL)
	{
		/*
		 * ���α׷��� Ŭ���̾�Ʈ�� ���
		 */
		while (TRUE)
		{
			if (!flag) { printf("#"); fflush(stdout); }

			if (flag && CSockGetFdCount('C') == 0)
			{
				if (flag) LOG(_FL_, 0, 0, "������ ���� �õ�");
				/*
				 * ������ ������ �õ��Ѵ�.
				 */

				CSockFDZERO();
				if (CSockSocket(CfgGetBxa("SND_HOST"), UtilToInt(CfgGetBxa("SND_PORT")), 1) == FALSE)
				{
					sleep(RECONN_SEC);
					continue;
				}

				if (flag) CSockPrintSocketTable();

				/*
				 * link ó���� �Ѵ�. link���� �۽�
				 */
			}

			if (!flag)
			{
				/*
				 * FQ���� �۽��ڷḦ �д´�.
				 * �۽��ڷᰡ ������ *sDataLine = 0x00
				 */
				if (ReadBatFQ() < 0)
				{
			        /* ������ �����Ѵ�. */
			        CSockFDCLR_All();
                    if (!flag) sleep(60);
                    if (!flag) continue;
                    if (flag) break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "��ġ���� �۽�");
				/*
				 * ��ġ������ ������.
				 */
				if (CSockWriteRead(SendBatFile) < 0)
				{
					if (flag) LOG(_FL_, 0, 0, "ERROR....");
					break;
				}
			}

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "POLL���� �۽�");
				/*
				 * polling ó���� �Ѵ�. poll���� �۽Ÿ�.
				 */
				CSockFDCLR_All();
			}

            if (!flag) sleep(60);
            if (flag) break;
		}
	}
}

#endif
