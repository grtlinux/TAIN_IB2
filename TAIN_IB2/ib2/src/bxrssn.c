/*
 * PROGRAM ID   : bxr.c
 * PROGRAM NAME : bxr ���
 * AUTHOR       : (��)ICA
 * COMMENT      : bxr ���
 * HISTORY      : 2013.08.04 ����. �����ۼ�
 *
 * �Ｚ��Ʈ�� ��ġ���� ����
 *
 *
 */

#include <Common.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.09.11.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.09.11.001: �����۾�."

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/
#define   RECONN_SEC               5
#define   MAX_DATA_LINE         8192

#define   BLOCK_RECS             100

#define   MSGTYPE_LOGIN_REQ       10
#define   MSGTYPE_LOGIN_RES       20
#define   MSGTYPE_LOGOUT_REQ      30
#define   MSGTYPE_LOGOUT_RES      40
#define   MSGTYPE_FILE_REQ        50
#define   MSGTYPE_FILE_RES        60
#define   MSGTYPE_CHECK           70
#define   MSGTYPE_NONE            90        /* not defined. */

#define   FLAG_DANGBAL         TRUE        /* ��߹߻�(TRUE, FALSE) */

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

MSG_HDR *               pHdr      ;
MSG_LINE*               pLine     ;

MSG_LOGIN *             pLogin    ;
MSG_LOGOUT*             pLogout   ;
MSG_FILE  *             pFile     ;
MSG_CHECK *             pCheck    ;

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

FILE*           fp              ;                  /* ��ġ���� fp     */

char            sFileName [ 512];
int             iFileSize       ;

char            sTrCode   [  50];

int             iRecLen         ;                  /* ���ڵ��Ѷ����� ����  */
int             iRecCnt         ;                  /* ���ڵ��� ����        */

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
			, "000"                              /* sResCode  */
			, ""                                 /* sBizFld   */
			, ""                                 /* sReserved */
			, "2USERID"                          /* sUserid   */
			, "PASSWD"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, "          "                       /* sSequence */
			, "          "                       /* sSubject  */
			, " "                                /* sFlag     */
			, "            "                     /* sStTime   */
			, "            "                     /* sEdTime   */
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
			, CfgGetBxa("SENDER_ID")             /* sUserid   */
			, CfgGetBxa("SENDER_PWD")            /* sPasswd   */
			, "RD"                               /* sJobType  */
			, "          "                       /* sSequence */
			, "          "                       /* sSubject  */
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
		
		if (flag)
		{
			/*
			 * DATE.2013.10.29
			 * ���ŵ������� �����ڵ�(sResCode)�� Ȯ���Ѵ�.
			 */
			char sResCode[10];
			
			pHdr = (MSG_HDR*)&sRecvLine;
			sprintf(sResCode, "%*.*s", sizeof(pHdr->sResCode)
				, sizeof(pHdr->sResCode), pHdr->sResCode);
			
			if (strcmp("000", sResCode) != 0)
			{
				if (strcmp("001", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : �ý��� ��� = '%s'", sResCode);
				}
				else if (strcmp("002", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : USERID ���� = '%s'", sResCode);
				}
				else if (strcmp("003", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : PASSWORD ���� = '%s'", sResCode);
				}
				else if (strcmp("004", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : JOB TYPE ���� = '%s'", sResCode);
				}
				else if (strcmp("005", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ���ǿ� �´� �ڷ� ���� = '%s'", sResCode);
				}
				else if (strcmp("006", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ���� ���� ���� = '%s'", sResCode);
				}
				else if (strcmp("007", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ���� Bytes ���� = '%s'", sResCode);
				}
				else if (strcmp("008", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ���� ���� ���� = '%s'", sResCode);
				}
				else if (strcmp("009", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : PASSWORD Change ���� = '%s'", sResCode);
				}
				else if (strcmp("010", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ���� ���� ���� = '%s'", sResCode);
				}
				else if (strcmp("099", sResCode) == 0)
				{
					LOG(_FL_, 9, 0, "�����ڵ� : ��Ÿ ���� = '%s'", sResCode);
				}
				else
				{
					LOG(_FL_, 0, 0, "�����ڵ� : Hdr.sResCode = '%s'", sResCode);
				}

				return -1;
			}
		}
	}

	return 0;
}

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
int RecvBatFileInfo(int cSock)
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

		if (flag)
		{
			pFile = (MSG_FILE*) &sRecvLine;

			iFileSize = UtilToIntN(pFile->sFileSize, sizeof(pFile->sFileSize));
			sprintf(sTrCode, "%*.*s", sizeof(pFile->sSubject), sizeof(pFile->sSubject), pFile->sSubject);
			UtilTrim(sTrCode);

			sprintf(sSequence, "%-*.*s", sizeof(pFile->sSequence), sizeof(pFile->sSequence), pFile->sSequence);
			sprintf(sSubject, "%-*.*s", sizeof(pFile->sSubject), sizeof(pFile->sSubject), pFile->sSubject);
		}


		/*
		 * LOGIN ������ �����Ѵ�.
		 * TODO. �������� ���� ���Ź����ɷ� ������ �ؾ� ��.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%-10.10s%-10.10s%010d%-20.20s%-20.20s%1.1s%-10.10s%-3.3s%-3.3s%1.1s%-22.22s"
			, "         "                        /* sTrCode   */
			, "110"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, pFile->sSequence                   /* sSequence */
			, pFile->sSubject                    /* sSubject  */
			, iFileSize                          /* sFileSize */
			, pFile->sSendId                     /* sSendId   */
			, pFile->sRecvId                     /* sRecvId   */
			, " "                                /* sFiller1  */
			, "          "                       /* sFiller2  */
			, pFile->sYnLast                     /* sYnLast   */
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

		/*======================================================================*/

		/*
		 *  TODO DATE.2013.08.06 : BAT_INF�� �����Ͽ� ������ �״´�.
		 *
		 *    typedef struct _BAT_INF_
		 *    {
		 *        char    sRecLen     [ 4]; // ���ڵ� ����
		 *        char    sRecCnt     [ 7]; // ���ڵ� �Ǽ�
		 *        char    sSR         [ 1]; // �ۼ��� ����
		 *        char    sOrgCode    [ 3]; // ����ڵ�
		 *        char    sTrCode     [11]; // �ϰ����� TR Code
		 *        char    sTrCodeLen  [ 2]; // �ϰ����� TR Code ����
		 *        char    sDate       [ 8]; // ������������ YYYYMMDD
		 *        char    sTime       [ 9]; // �������۽ð� hhmmssSSS
		 *        char    sTrCodeSeq  [ 2]; // ���ں� TR Code ȸ��
		 *        char    sFiller     [53]; // ����
		 *    } BAT_INF;
		 */
		if (flag)
		{
			/*
			 * ������ ���� ��ġ ����
			 */
			char* sSR         = "R"           ;    /* S �۽�, R ����              */
			char* sOrgCode    = "SSN"         ;    /* ����ڵ�                    */
			char  sDateTime[50]               ;    /* ��¥ : YYYYMMDDhhmmssSSSSSS */

			char buf[20];
			sprintf(buf, "%-8.8s", sTrCode);
			UtilFillCopy(sTrCode, UtilTrim(buf), 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			/*
			 * ��ġ�������� sSR, sTrCode�� bxa_trlst.cfg ���� ������ �д´�.
			 */
			CfgBxaTr(sSR, sTrCode);

			/*
			 * ���ڵ��� ���̸� ���Ѵ�.
			 */
			iRecLen = UtilToInt(CfgGetBxaTr("RECLEN"));
			iRecCnt = 0;

			if (flag)
			{
				/*
				 * �۽ſ� ��ġ������ �����.
				 */

				for (iTrCodeSeq = 1; iTrCodeSeq < 100; iTrCodeSeq++)
				{

					BAT_INF* pInf = (BAT_INF*) &sBatInf;

					sprintf(sBatInf, "%04d%07d%1.1s%3.3s%-11.11s%02d%-8.8s%9.9s%02d%53.53s"
						, iRecLen
						, iRecCnt
						, sSR
						, sOrgCode
						, sTrCode
						, 8
						, sDateTime
						, sDateTime + 8
						, iTrCodeSeq
						, "");

					sprintf(sFileName, CfgGetMaster("ib.path.file.bxrcom")
						, sizeof(pInf->sSR       ), sizeof(pInf->sSR       ), pInf->sSR
						, sizeof(pInf->sOrgCode  ), sizeof(pInf->sOrgCode  ), pInf->sOrgCode
						, sizeof(pInf->sTrCode   ), sizeof(pInf->sTrCode   ), pInf->sTrCode
						, sizeof(pInf->sDate     ), sizeof(pInf->sDate     ), pInf->sDate
						, sizeof(pInf->sTrCodeSeq), sizeof(pInf->sTrCodeSeq), pInf->sTrCodeSeq
						);

					UtilSetYYYYMMDD(sFileName);

					if (access(sFileName, R_OK) < 0)
					{
						/* file not exist.... */
						break;
					}
				}
			}
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
int RecvBatFileData(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * ��ġ������ �����Ѵ�.
		 */
		fp = fopen(sFileName, "w+");
		/* chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
		chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
	}
	
	if (flag)
	{
		pLine = (MSG_LINE*) &sRecvLine;

		int iReaded = 0;

		if (flag) printf("STATUS : iReaded = %d, iFileSize = %d\n", iReaded, iFileSize);

		while (iReaded < iFileSize)
		{
			if (!flag)
			{
				iRecvLen = 63;

				/*
				 * data header�� �д´�. 63 bytes
				 */
				int ret = SockRecv(cSock, sRecvLine, iRecvLen);
				if (!flag) printf("HDR  [%*.*s]\n", 63, 63, sRecvLine);
				if (ret <= 0)
				{
					CSockFDCLR(cSock);
					return FALSE;
				}
			}

			if (flag)
			{
				/*
				 * data body�� ���̸� ���Ѵ�.
				 */
				/*iRecvLen = UtilToIntN(pLine->sFileSize, sizeof(pLine->sFileSize));*/
				if (flag)
					iRecvLen = 201;
				else
					iRecvLen = 200;
			}

			if (flag)
			{
				/*
				 * data body�� �д´�. ���̿� �ش��ϴ� ��ŭ �д´�.
				 */
				int ret = SockRecv(cSock, pLine->sData, iRecvLen);
				if (!flag) printf("BODY  [%*.*s]\n", iRecvLen, iRecvLen, pLine->sData);
				if (ret <= 0)
				{
					CSockFDCLR(cSock);
					return FALSE;
				}

				if (!flag)
				{
					/* new line ���� */
					iRecvLen --;
				}
				
				pLine->sData[iRecvLen] = 0x00;
			}

			if (flag)
			{
				/*
				 * ��ġ���Ͽ� �ڷḦ ����Ѵ�.
				 */

				fwrite(pLine->sData, 1, iRecvLen, fp);
			}

			if (flag)
			{
				/*
				 * ���ŵ����͸� ����Ѵ�.
				 */
				LOG(_FL_, 0, 0, "RECV(%d): HDR+DATA [%*.*s][%s]"
					, cSock, sizeof(MSG_LINE)-1, sizeof(MSG_LINE)-1, sRecvLine
					, sRecvLine + sizeof(MSG_LINE)-1);
			}

			iReaded += iRecvLen;
		}
	}

	if (flag)
	{
		/*
		 * ������ close�Ѵ�.
		 */
		fclose(fp);
	}

	if (flag)
	{
		/*
		 * ��ġ�������� ���ڵ� �Ǽ��� ���Ѵ�.
		 * ���μ��� ���ڵ� �Ǽ��� �Ѵ�.
		 */
		char buf[MAX_DATA_LINE];

		iRecCnt = 0;
		fp = fopen(sFileName, "r");
		if (flag) printf("%d > sFileName [%s]\n", iRecCnt, sFileName);

		while (fgets(buf, MAX_DATA_LINE, fp))
		{
			iRecCnt ++;
			if (flag) printf("%d >%s", iRecCnt, buf);
		}

		fclose(fp);
	}

	if (flag)
	{
		/*
		 * FQ�� �ִ´�.
		 * ��ġ���� ������ �����Ѵ�.
		 */
		if (flag)
		{
			char* sSR         = "R"           ;    /* S �۽�, R ����              */
			char* sOrgCode    = "SSN"         ;
			char* sTrCode     = sSubject      ;
			char  sDateTime[50]               ;

			char buf[20];
			sprintf(buf, "%-8.8s", sTrCode);
			UtilFillCopy(sTrCode, UtilTrim(buf), 11, '0');

			strcpy(sDateTime, TimeGetDateTime());

			sprintf(sBatInf, "%04d%07d%1.1s%3.3s%-11.11s%02d%-8.8s%9.9s%02d%53.53s"
				, iRecLen
				, iRecCnt
				, sSR
				, sOrgCode
				, sTrCode
				, 8
				, sDateTime
				, sDateTime + 8
				, iTrCodeSeq
				, "");

			if (flag) LOG(_FL_, 0, 0, "[RecvBatFileData] : sBatInf [%s]", sBatInf);

			WriteBatFQ();
		}
	}

	if (!flag)
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
/*
 * ���� : CHECK
 *
 *     sTrCode   [   9];  // Transaction Code
 *     sTranNum  [   3];  // ������ȣ (003,030,007,070,100,110,130)
 *     sResCode  [   3];  // �����ڵ�
 *     sBizFld   [  20];  // �����ʵ� (VAN���� Ȱ��)
 *     sReserved [   5];  // ����
 *     sSequence [  10];  // ���Ž� Ư�����ϸ� �����ϰ����� ��
 *     sSubject  [  10];  // ���Ž� Subject (�����̸� ��� ����)
 *     sFileSize [  10];  // �۽��� ������ size
 *     sYnTrans  [   1];  // ó������ : Y.�̻���� N.�̻�����
 *     sFiller   [  79];  // ����
 *
 */
/*----------------------------------------------------------------------------*/
int SendBatCheck(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{

		/*
		 * LOGIN ������ �����Ѵ�.
		 */
		sprintf(sSendLine, "%9.9s%3.3s%3.3s%20.20s%5.5s"
			"%10.10s%-10.10s%10d%1.1s%79.79s"
			, "         "                        /* sTrCode   */
			, "130"                              /* sTranNum  */
			, "000"                              /* sResCode  */
			, "                    "             /* sBizFld   */
			, "     "                            /* sReserved */
			, sSequence                          /* sSequence */
			, "FS082004  "                       /* sSubject  */
			, iFileSize                          /* sFileSize */
			, "Y"                                /* sYnTrans  */
			, "                                                                               " /* sFiller   */
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
			, "USERID"                           /* sUserid   */
			, "PASSWD"                           /* sPasswd   */
			, "SD"                               /* sJobType  */
			, ""                                 /* sSequence */
			, ""                                 /* sSubject  */
			, "E"                                /* sFlag     */
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
			, "         "                            /* sTrCode   */
			, "007"                                  /* sTranNum  */
			, "000"                                  /* sResCode  */
			, "                    "                 /* sBizFld   */
			, "     "                                /* sReserved */
			, CfgGetBxa("SENDER_ID")                 /* sUserid   */
			, CfgGetBxa("SENDER_PWD")                /* sPasswd   */
			, "SD"                                   /* sJobType  */
			, "          "                           /* sSequence */
			, "          "                           /* sSubject  */
			, "E"                                    /* sFlag     */
			, sStTime                                /* sStTime   */
			, sEdTime                                /* sEdTime   */
			, "                                   "  /* sFiller   */
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
int RecvBatFile(int cSock)
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
				if (flag) printf("STATUS : RecvBatFileInfo()..\n");

				/*
				 * ��������
				 */
				ret = RecvBatFileInfo(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : RecvBatFileData()..\n");

				/*
				 * DATA����
				 */
				ret = RecvBatFileData(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (flag)
			{
				if (flag) printf("STATUS : SendBatCheck()..\n");

				/*
				 * ����Ȯ��
				 */
				ret = SendBatCheck(cSock);
				if (ret < 0)
				{
					return ret;
				}
			}

			if (!flag)
			{
				if (flag) printf("STATUS : WriteBatFQ()..\n");

				/*
				 * FQ���� �۽��ڷḦ �д´�.
				 * �۽��ڷᰡ ������ *sDataLine = 0x00
				 */
				if (WriteBatFQ() < 0)
				{
					/*sleep(5 * 60); */
					sleep(1);
					break;
				}
			}

			if (flag) break;

			sleep(20);
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

		BAT_INF* pInf = (BAT_INF*) &sBatInf;

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
 * ���� : �������� ���� �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int WriteBatFQ()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (*sBatInf == 0x00)
			return;

		FQ2* pFq = FQ2Open("OAR", sServiceid);

		FQ2Write(pFq, sBatInf);

		FQ2Close(pFq);
	}

	if (flag) PrintBatInf();

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

			if (CSockGetFdCount('C') == 0)
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

			if (flag)
			{
				if (flag) LOG(_FL_, 0, 0, "��ġ���� ����");
				/*
				 * ��ġ������ �޴´�.
				 */
				if (CSockWriteRead(RecvBatFile) < 0)
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
				/*
				 * polling ó���� �Ѵ�. poll���� �۽Ÿ�.
				 */
			}

            if (!flag) sleep(60);
            if (flag) break;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "���α׷��� �����մϴ�.");
}

#endif
