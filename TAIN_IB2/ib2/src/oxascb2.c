/*
 * PROGRAM ID   : oxascb2.c
 * PROGRAM NAME : oxascb2 ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : oxascb2 ���� ���
 * HISTORY      : 2013.11.19 ����. �����ۼ�
 *
 */

#include <Common.h>

#include <xmdevice.h>
#include <xmclient.h>
#include <xmdefine.h>
#include <xmdebug.h>

/*============================================================================*/
/*
#define   VERSION                  "2013.11.19.001: �����۾�"
 */
/*----------------------------------------------------------------------------*/
#define   VERSION                  "2013.11.19.001: �����۾�"

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
 *  * SC���� (504 bytes) - len (4) + header (57) + data (443)
 *  typedef struct _SCB_DATA_
 *  {
 *      char sMsgLen       [  4];  // ��������
 *      char sTrCode       [  9];  // transaction code
 *      char sSystemType   [  3];  // "STK"
 *      char sServiceType  [  3];  // "DDD":�������, "GGG":��������
 *      char sCompanyCode  [ 10];  // ��ü��ȣ:���ǻ��ȣ(7)+SPACE(3)
 *      char sSeqNo        [  7];  // �����Ϸù�ȣ
 *      char sMsgType      [  4];  // ���������ڵ�
 *      char sJobCode      [  3];  // ���������ڵ�
 *      char sSrFlag       [  1];  // �ۼ��ű��� (����:B ����:E)
 *      char sReplyCode    [  3];  // �����ڵ�
 *      char sTransDateTime[ 14];  // �ۼ����Ͻ� (YYYYMMDDhhmmss)
 *      char sData         [443];  // data
 *  } SCB_DATA;
 *
 */
/*----------------------------------------------------------------------------*/
/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define            MAX_DATA_LINE   (10*1024)

#define            SEND_FQ         "OAS"           /* �۽��� ���� FQ */
#define            RECV_FQ         "OAR"           /* ������ ���� FQ */

#define            KEY__           3
#define            MSG__           4

/*----------------------------------------------------------------------------*/

char               sDataLine       [MAX_DATA_LINE];   /* ó���� �ڷ� */
char               sSendLine       [MAX_DATA_LINE];   /* �۽ſ� �ڷ� */
char               sRecvLine       [MAX_DATA_LINE];   /* ���ſ� �ڷ� */

int                iDataLen        ;
int                iSendLen        ;
int                iRecvLen        ;

BIZ_DAT*           pBizDat         = (BIZ_DAT*) &sDataLine;

FEP_BIZ_DAT*       pSndFepBizDat   = (FEP_BIZ_DAT*) &sSendLine;
FEP_BIZ_DAT*       pRcvFepBizDat   = (FEP_BIZ_DAT*) &sRecvLine;

SCB_DATA*          pSndScbLine     = (SCB_DATA*) &sSendLine;
SCB_DATA*          pRcvScbLine     = (SCB_DATA*) &sRecvLine;

/*----------------------------------------------------------------------------*/

int                iForkFlag       ;           /* fork ó�� flag */
int                iProcessAlive = 0    ;
int                iProcessDiff  = 0    ;

char               sGUID           [100];      /* Global User ID */

int                iInitFlag       ;
XM_CLIENT_CTX      ctx;

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

	if (flag) LOG(_FL_, 0, 0, "[%s] [%04d:%s] ", sFepid, iDataLen, sDataLine);

	return iDataLen;
}

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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int SEND(int sock, char* msg, int len)
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
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int RECV(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int nleft, nread, err;
	char* buf;

	buf = (char*) msg;
	nleft = len;

	while (nleft > 0) {
		nread = recv(sock, buf, nleft, 0);
		if (nread < 0) {
			/* unix */
			if (errno == EINTR)
				nread = 0;
			else if (errno == EWOULDBLOCK)
				return -3;
			else
				return -4;
		}
		else if (nread == 0)
			break;

		nleft -= nread;
		buf += nread;
	}

	return (len - nleft);
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int SocketMsgSend(int sock, char* msg, int len)
/*----------------------------------------------------------------------------*/
{
	int ret, pos = 0;
	unsigned char buf[1024*10];

	memcpy(buf, msg, len);

	ret = SEND(sock, buf, len);
	if (ret < 0)
		return (-1);

	return ret;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int SocketMsgRecv_BAK(int sock, char* msg)
/*----------------------------------------------------------------------------*/
{
	int ret, len;

	ret = RECV(sock, msg, 4);
	if (ret < 0)
		return -1;

	len = UtilToIntN(msg, 4);

	ret = RECV(sock, msg + 4, len);
	if (ret < 0)
		return -3;

	msg[len + 4] = 0x00;
	
	return (len + 4);
}

/*============================================================================*/
/*
 * ���� : ��ȣȭ
 */
/*----------------------------------------------------------------------------*/
int XM2_Encrypt()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * ��ȣȭ
		 */

		int ret;
		uint8   out[2048];
		uint16  outLen;
		uint8   outSend[8192];
		uint16  outSendLen;

		strcpy(out, sSendLine);
		outLen = iSendLen;

		if (!flag)
		{
			SCB_DATA* p = (SCB_DATA*)&out;
			printf("\t ENC.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t ENC.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t ENC.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t ENC.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t ENC.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t ENC.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t ENC.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t ENC.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t ENC.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t ENC.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t ENC.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t ENC.sData          = [%*.*s]\n", sizeof(p->sData         ), sizeof(p->sData         ), p->sData         );
		}

		if (flag)
		{
			/*
			 * sData �κ��� ��ȣȭ �Ѵ�.
			 */
			SCB_DATA* p = (SCB_DATA*)&out;

			ret = XMC2_Encrypt_Message(&ctx, outSend, &outSendLen, sizeof(outSend), p->sData, sizeof(p->sData));
			if (ret > 0)
			{
				if (flag) printf("[%s] SUCCESS : XMC2_Encrypt_Message (MSG) OK....[inLen=%d]\n", TimeGetTime2(), outLen);
			}
			else
			{
				if (flag) printf("[%s] FAIL : XMC2_Encrypt_Message (MSG) Fail [ret=%d]....\n", TimeGetTime2(), ret);
				return -1;
			}
		}

		if (flag)
		{
			/*
			 * outSend(��ȣȭ�� ����)�� �����Ѵ�. ��ȣȭ������ binary �ڵ��̴�.
			 */
			iSendLen = 4 + 57 + outSendLen;

			sprintf(sSendLine, "%04d%57.57s", iSendLen - 4, out + 4);

			memcpy(pSndScbLine->sData, outSend, outSendLen);
		}

		if (!flag)
		{
			SCB_DATA* p = pSndScbLine;
			printf("\t SEND.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t SEND.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t SEND.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t SEND.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t SEND.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t SEND.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t SEND.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t SEND.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t SEND.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t SEND.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t SEND.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t SEND.sData          = [��ȣȭ���ڿ�]\n");
		}
	}

	return 0;
}


/*============================================================================*/
/*
 * ���� : ��ȣȭ
 */
/*----------------------------------------------------------------------------*/
int XM2_Decrypt()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * ��ȣȭ
		 */

		int ret;
		uint8   in[2048];
		uint16  inLen;
		uint8   inRecv[8192];
		uint16  inRecvLen;

		memcpy(inRecv, sRecvLine, iRecvLen);
		inRecvLen = iRecvLen;
		
		if (!flag)
		{
			SCB_DATA* p = (SCB_DATA*)&inRecv;
			printf("\t RECV.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t RECV.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t RECV.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t RECV.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t RECV.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t RECV.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t RECV.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t RECV.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t RECV.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t RECV.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t RECV.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t RECV.sData          = [��ȣȭ���ڿ�]\n");
		}

		if (flag)
		{
			/*
			 * inRecv.sData(��ȣȭ�� ����)�� in(��ȣȭ�� ����)���� �����.
			 */
			SCB_DATA* p = (SCB_DATA*)&inRecv;
			uint16 pLen = inRecvLen - 4 - 57;

			ret = XMC2_Decrypt_Message(&ctx, in, &inLen, sizeof(in), (uint8*)&p->sData, pLen);
			if (ret > 0)
			{
				if (flag) printf("[%s] SUCCESS : XMC2_Decrypt_Message (MSG) OK....[inLen=%d]\n", TimeGetTime2(), inLen);
			}
			else
			{
				if (flag) printf("[%s] FAIL : XMC2_Decrypt_Message (MSG) Fail [ret=%d]....\n", TimeGetTime2(), ret);
				return -4;
			}
		}

		if (flag)
		{
			/*
			 * ��ȣȭ ������ �����.
			 */
			iRecvLen = 4 + 57 + inLen;

			sprintf(sRecvLine, "%04d%57.57s%-443.443s", iRecvLen - 4, inRecv + 4, in);
		}

		if (!flag)
		{
			SCB_DATA* p = pRcvScbLine;
			printf("\t DEC.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t DEC.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t DEC.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t DEC.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t DEC.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t DEC.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t DEC.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t DEC.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t DEC.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t DEC.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t DEC.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			printf("\t DEC.sData          = [%*.*s]\n", sizeof(p->sData         ), sizeof(p->sData         ), p->sData         );
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int XM2_HandShake(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "[SEND] KEY �۽�" );

	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY�� �۽��� �����Ѵ�.
		 */

		int ret;
		uint8   out[2048];
		uint16  outLen;

		ret = XMC2_Encode_KeyInit_Token(&ctx, out, &outLen, sizeof(out));
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : XMC2_Encode_KeyInit_Token (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : XMC2_Encode_KeyInit_Token Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -1;
		}

        if (flag) LOG(_FL_, 0, 0, "STATUS : (KEY) outLen = %d\n", outLen);
		fflush(stdout);

		iSendLen = 4 + 57 + outLen;

		sprintf(sSendLine, "%04d%9.9s%3.3s%3.3s%-10.10s%07d%4.4s%3.3s%1.1s%3.3s%-14.14s"
			, iSendLen - 4
			, ""
			, "STK"
			, ""
			, "0005789"
			, SeqGetNo(sFepid)  /* �����Ϸù�ȣ */
			, "0800"
			, "500"
			, "E"
			, ""
			, TimeGetDateTime());

		memcpy(pSndScbLine->sData, out, outLen);

		ret = SocketMsgSend(cSock, sSendLine, iSendLen);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : SocketMsgSend (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : SocketMsgSend Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -2;
		}

		if (flag)
		{
			SCB_DATA* p = pSndScbLine;
			printf("\t SEND.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t SEND.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t SEND.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t SEND.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t SEND.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t SEND.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t SEND.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t SEND.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t SEND.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t SEND.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t SEND.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			fflush(stdout);
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "[RECV] KEY ����" );
	
	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY�� ������ �����Ѵ�.
		 */

		int ret;
		uint8   in[2048];
		uint16  inLen;

		ret = MSockMsgRecv(cSock, sRecvLine);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : MSockMsgRecv (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : MSockMsgRecv (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -3;
		}


		if (flag)
		{
			SCB_DATA* p = pRcvScbLine;
			printf("\t RECV.sMsgLen        = [%*.*s]\n", sizeof(p->sMsgLen       ), sizeof(p->sMsgLen       ), p->sMsgLen       );
			printf("\t RECV.sTrCode        = [%*.*s]\n", sizeof(p->sTrCode       ), sizeof(p->sTrCode       ), p->sTrCode       );
			printf("\t RECV.sSystemType    = [%*.*s]\n", sizeof(p->sSystemType   ), sizeof(p->sSystemType   ), p->sSystemType   );
			printf("\t RECV.sServiceType   = [%*.*s]\n", sizeof(p->sServiceType  ), sizeof(p->sServiceType  ), p->sServiceType  );
			printf("\t RECV.sCompanyCode   = [%*.*s]\n", sizeof(p->sCompanyCode  ), sizeof(p->sCompanyCode  ), p->sCompanyCode  );
			printf("\t RECV.sSeqNo         = [%*.*s]\n", sizeof(p->sSeqNo        ), sizeof(p->sSeqNo        ), p->sSeqNo        );
			printf("\t RECV.sMsgType       = [%*.*s]\n", sizeof(p->sMsgType      ), sizeof(p->sMsgType      ), p->sMsgType      );
			printf("\t RECV.sJobCode       = [%*.*s]\n", sizeof(p->sJobCode      ), sizeof(p->sJobCode      ), p->sJobCode      );
			printf("\t RECV.sSrFlag        = [%*.*s]\n", sizeof(p->sSrFlag       ), sizeof(p->sSrFlag       ), p->sSrFlag       );
			printf("\t RECV.sReplyCode     = [%*.*s]\n", sizeof(p->sReplyCode    ), sizeof(p->sReplyCode    ), p->sReplyCode    );
			printf("\t RECV.sTransDateTime = [%*.*s]\n", sizeof(p->sTransDateTime), sizeof(p->sTransDateTime), p->sTransDateTime);
			fflush(stdout);
		}

		iRecvLen = ret;

		inLen = iRecvLen - 4 - 57;
		memcpy(in, pRcvScbLine->sData, inLen);

		ret = XMC2_Decode_KeyFinal_Token(&ctx, in, inLen);
		if (ret > 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS : XMC2_Decode_KeyFinal_Token (KEY) [ret=%d] OK...", ret);
			fflush(stdout);
		}
		else
		{
			if (flag) LOG(_FL_, 0, 0, "FAIL : XMC2_Decode_KeyFinal_Token Fail (KEY) [ret=%d] ...", ret);
			fflush(stdout);
			return -4;
		}
	}
	
	return 0;
}

/*============================================================================*/
/*
 * ���� :
 */
/*----------------------------------------------------------------------------*/
int XM2_Init(int cSock)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "XecureConnect Lib Init..");
	
	if (flag)
	{
		/*
		 * XecureConnect�� Lib�� �ʱ�ȭ �Ѵ�.
		 */

		int ret;
		char sFileName[200];
		strcpy(sFileName, "/hw01/ibridge/lib/XecureConnect2.6/test/test-cert.der");

		ret = XMC2_ClientLib_Init(&ctx, sFileName);
		if (ret > 0)
		{
			if (flag) printf("SUCCESS : XecureConnect Lib Init OK....\n");
			fflush(stdout);
		}
		else
		{
			if (flag) printf("FAIL : XecureConnect Lib Init Fail [ret=%d]....\n", ret);
			fflush(stdout);
			return -1;
		}
	}

    if (flag) LOG(_FL_, 0, 0, "XecureConnect HandShake..");
	
	if (flag)
	{
		/*
		 * XecureConnect HandShake : KEY �� �۽�/�����Ѵ�.
		 */
		 
		int iret;
		
		iret = XM2_HandShake(cSock);
		
		if ( iret < 0 )
		{
			if (flag) LOG(_FL_, 0, 0, "XecureConnect HandShake FAIL...");
			return -1;
		}
	}
	
	if (flag) LOG(_FL_, 0, 0, "SUCCESS : XecureConnect Init...");

	return 0;
}


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
		 * SCB01�� ���� ���
		 */

		if (iInitFlag == 0)
		{
			if (flag) LOG(_FL_, 0, 0, "SUCCESS INITIALIZE..");
			
			int ret = XM2_Init(csi->sock);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0, "XM2_Init FAIL..");
				if (flag) exit(-1);
			}
			
			iInitFlag = 1;
		}
		else
		{
			if (!flag) LOG(_FL_, 0, 0, "CONTINUE..........");
		}
	}

	if (!flag)
	{
		/*
		 * [����] = [����ð�(��)] - [�ֱ� ������ �ð�(��)]
		 */
		int diff = time(NULL) - csi->lastRecved;

		if (!flag)
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
					"[%s] [TIME_OUT_CLOSE:%d] SESSION CLOSE....(%s:%d)"
					, sFepid, iTimeOutSec, csi->ipaddr, csi->port);
				return -1;
			}
		}

		if (!flag)
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
					strcpy(sSendLine, PollMsgSendReq());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL REQ �۽� */
					int ret = MSockMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0, 
							"[%s] [POLL_SEND] SESSION CLOSE BY SEND ERROR..(%s:%d)"
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
				if (flag) LOG(_FL_, 0, 0, 
					"[%s] [MSG_RECV] SESSION CLOSE BY RECV ERROR..(%s:%d)"
					, sFepid, csi->ipaddr, csi->port);
				return -1;
			}
			else if (iRecvLen == 0)
			{
				/* ���ű��� 0 -> ��ó�� */
				if (flag) LOG(_FL_, 0, 0, "[%s] [MSG_RECV] Length Zero(0).(%s:%d)"
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

			if (PollCmpRecvReq(sRecvLine) == 0)
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
					strcpy(sSendLine, PollMsgSendRes());
					iSendLen = strlen(sSendLine);
				}

				if (flag)
				{
					/* POLL RES �۽� */
					int ret = SocketMsgSend(csi->sock, sSendLine, iSendLen);
					if (ret < 0)
					{
						if (flag) LOG(_FL_, 0, 0, 
							"[%s] [POLL_SEND] SESSION CLOSE BY SEND POLL-RES ERROR..(%s:%d)"
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
			else if (PollCmpRecvRes(sRecvLine) == 0)
			{
				/*
				 * 2. POLL������� -> POLL clear
				 */
				if (flag) LOG(_FL_, 0, 0, "[%s] [POLL_RECV] <- RES [%d:%s] (%s:%d)"
					, sFepid, iRecvLen, sRecvLine, csi->ipaddr, csi->port);
			}
			else
			{
				if (flag)
				{
					/* ������ �κ��� ��ȣȭó���� �Ѵ�. */
					XM2_Decrypt();
				}
				
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
		}
		
		if (flag) LOG(_FL_, 0, 0, "[HDR_����] -> [%04d:%s] ", iSendLen, sSendLine);

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
			/* DATA �κ��� ��ȣȭ �Ѵ�. */
			XM2_Encrypt();
		}

		if (flag)
		{
			int ret = SocketMsgSend(csi->sock, sSendLine, iSendLen);
			if (ret < 0)
			{
				if (flag) LOG(_FL_, 0, 0, 
					"[%s] [MSG_SEND] SESSION CLOSE BY SEND REQ ERROR..(%s:%d)"
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

	if (flag) LOG(_FL_, 0, 0, "[MainService]...strInfo[%s]", strInfo);
		
	SigSetSignal();
    
    CheckAlive(); /* ���μ��� �⵿ �ʱ�ȭ */

	if (flag)
	{
		if (flag)
		{
			/* ����� FQ�� �����Ѵ�. */
			if (flag) FQ2Init(SEND_FQ, sServiceid);
			if (flag) FQ2Init(RECV_FQ, sServiceid);
		}

		if (flag)
		{
			/* ���������� �Ľ��Ͽ� �����Ѵ�. */
			MSockParsingSockInfo(strInfo);
		}

		while (TRUE)
		{
			CheckAlive(); /* ���μ��� �⵿ �ʱ�ȭ */
			
			/* server / accept / connect ������ �����ϰ� ������ �����Ѵ�. */
			if (flag) MSockMakeSockInfo();

			/* accept / connect ������ üũ�Ѵ�. TimeOut, Poll üũ */
			if (flag) MSockCheckHandler(CheckProc);

			/* ������ send ó���� �ٷ�� handler */
			if (flag) MSockSendHandler(SendProc);

			/* ������ recv ó���� �ٷ�� handler */
			if (flag) MSockRecvHandler(RecvProc);

			if (flag) sleep(5);
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
			if (!flag) LOG(_FL_, 0, 0, "[%s]\n", strFile);

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
