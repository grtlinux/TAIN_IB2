/*
 * PROGRAM ID   : FQ2.c
 * PROGRAM NAME : ����ť�� ���Ǵ� �Լ��� ����
 * AUTHOR       : (��)ICA
 * COMMENT      : ����ť�� ����ϴ� �Լ����� �����Ѵ�.
 * HISTORY      : 2013.10.02 ����. �����ۼ�
 *
 * COMMENT      : N ���� writer �� 1���� reader �� ��츦 ������.
 *
			if (flag) LOG(_FL_, 0, 0, "AFTER [iLineNo=%d][stFq.iWSeq=%d] [stFq.iRSeq=%d]", iLineNo, stFq.iWSeq, stFq.iRSeq);
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define   MAX_LINE_SIZE      8192

/*============================================================================*/
/*
 * structure
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

char       sFQDatData               [MAX_LINE_SIZE];

int        iLineNo                                 ;

FQ2SEQ*    pSeq             = (FQ2SEQ*) &sFQDatData;
FQ2DAT*    pDat             = (FQ2DAT*) &sFQDatData;

FQ2        stFq                                    ;

/*============================================================================*/
/*
 * ���� : ����ť2�� ��ü�� �����Ѵ�.
 *
 * typedef struct _FILE_QUEUE2_
 * {
 *     char    sSvcid        [ 10+1]; // Service ID
 *     char    sFepid        [  5+1]; // FEPID
 *     char    sApno         [  2+1]; // AP���� ��ȣ
 *
 *     char    chSR                 ; // �ۼ��ű���
 *     char    chWR                 ; // �����бⱸ��
 *
 *     int     iWSeq                ; // write sequence
 *     int     iRSeq                ; // read sequence
 *
 *     long    lRPos                ; // read position
 *
 *     FILE*   fpSeq                ; // seq file
 *     FILE*   fpDat                ; // data file
 * } FQ2;
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ2SEQ;
 *
 * typedef struct _FILE_QUEUE2_DAT_
 * {
 *     char    sSeq           [ 8];   // ��������
 *     char    sRecvr         [10];   // ó�� �����̰�, ���� ���� �����
 *     char    sLen           [ 5];   // sData�� �����̰� '\n'���� ����
 *
 *     char    sData          [ 1];   // �����ϰ����ϴ� ������
 * } FQ2DAT;
 *
 *
 */

/*============================================================================*/
/*
 * ���� : FQ2�� ����� �غ� �Ѵ�. �������� �� ���ϻ���
 */
/*----------------------------------------------------------------------------*/
int FQ2Init(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * ���������� �����.
		 * �׳� ����������� mkdir�� �����Ѵ�. �̹� ������ return -1, �ƴϸ�
		 * �����Ǵµ� ������ Ȯ���� �ϱ� �����̴�.
		 */
		char* p;
		char sFileName[128];

		if (flag)
		{
			/* SEQ ���� */
			strcpy(sFileName, CfgGetMaster("ib.path.file.fq.seq"));
			UtilSetYYYYMMDD(sFileName);
			p = rindex(sFileName, '/');
			*p = 0x00;

			mkdir(sFileName, 0777);
		}

		if (flag)
		{
			/* DAT ���� */
			strcpy(sFileName, CfgGetMaster("ib.path.file.fq.dat"));
			UtilSetYYYYMMDD(sFileName);
			p = rindex(sFileName, '/');
			*p = 0x00;

			mkdir(sFileName, 0777);
		}
	}

	if (flag)
	{
		char sFileName[128];
		char sFepid[20];
		char sApNum[20];

		if (flag)
		{
			/*
			 * ���񽺸��� ���� FEPID, AP��ȣ�� ��´�.
			 */
			sprintf(sFepid, "%-5.5s", sServiceid + 3);
			sprintf(sApNum, "%-2.2s", sServiceid + 8);
		}

		if (flag)
		{
			/*
			 * SEQ ������ Ȯ���Ͽ� �����ϰų� �ʱ�ȭ�Ѵ�.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.seq"), sFqName, sFepid, sApNum);
			UtilSetYYYYMMDD(sFileName);
			if (flag) LOG(_FL_, 0, 0, "FQ2Init SEQ => [%s]", sFileName);

			if (access(sFileName, W_OK) < 0)
			{
				/*
				 * ������ ������ ���� ������ �ʱ�ȭ�� �����Ѵ�.
				 */

				FILE* fp = fopen(sFileName, "w+");

				if (flag)
				{
					/*
					 * SEQ �ʱ�ȭ �Ѵ�.
					 */
					FQ2SEQ stSeq;
					
					memset(stSeq.sWSeq, '0', sizeof(stSeq.sWSeq));
					memset(stSeq.sRSeq, '0', sizeof(stSeq.sRSeq));
					stSeq.lRPos = 0;
					stSeq.cLF = 0x0a;

					fseek(fp, 0, SEEK_SET);
					fwrite((char*)&stSeq, 1, 8+8+8+1, fp);
					fflush(fp);
				}

				if (flag)
				{
					/*
					 * ����(permission)�� �����Ѵ�. 0660
					 */
					/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
					chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
				}

				fclose(fp);
			}
		}

		if (flag)
		{
			/*
			 * DAT ������ Ȯ���Ͽ� �����Ѵ�.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.dat"), sFqName, sFepid, sApNum);
			UtilSetYYYYMMDD(sFileName);
			if (flag) LOG(_FL_, 0, 0, "FQ2Init DAT => [%s]", sFileName);

			if (access(sFileName, W_OK) < 0)
			{
				/*
				 * ������ ������ ���� ������ �ʱ�ȭ�� �����Ѵ�.
				 */

				FILE* fp = fopen(sFileName, "w+");

				if (flag)
				{
					/*
					 * ����(permission)�� �����Ѵ�. 0660
					 */
					/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
					chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
				}

				fclose(fp);
			}
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ����ť2�� ��ü�� �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
FQ2* FQ2Open(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	/* FQ2 ����ü �޸𸮸� �Ҵ��Ѵ�. */
	FQ2* pFq = malloc(sizeof(FQ2));

	if (flag)
	{
		char sFileName[128];

		if (flag)
		{
			/*
			 * FQ ����ü�� ä���.
			 */
			sprintf(pFq->sFqNm , "%-3.3s"  , sFqName);
			sprintf(pFq->sSvcid, "%-10.10s", sServiceid);
			sprintf(pFq->sFepid, "%-5.5s"  , sServiceid + 3);
			sprintf(pFq->sApno , "%-2.2s"  , sServiceid + 8);

			pFq->chSR  = 'S';
			pFq->chWR  = 'W';

			pFq->iWSeq = 0;
			pFq->iRSeq = 0;
			pFq->lRPos = 0;
		}

		if (flag)
		{
			/*
			 * SEQ ������ �����Ѵ�.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.seq"), sFqName, pFq->sFepid, pFq->sApno);
			UtilSetYYYYMMDD(sFileName);
			pFq->fpSeq = fopen(sFileName, "r+");
			if (pFq->fpSeq == NULL)
			{
				pFq->fpSeq = fopen(sFileName, "w+");
				if (pFq->fpSeq == NULL)
				{
					if (flag) LOG(_FL_, 9, 0, "\x1b[41m[ERROR] SEQ NULL!![%s] errno=[%d:%s] \x1b[0m" , sFileName, errno, strerror(errno));
					return NULL;
				}
			}

			if (flag)
			{
				/*
				 * SEQ ũ�Ⱑ 0�̸� �ʱ�ȭ �Ѵ�.
				 */
				struct stat file_stat;
				fstat(fileno(pFq->fpSeq), &file_stat);

				if (!flag) LOG(_FL_, 0, 0,  "SEQ file size = %d", file_stat.st_size);

				if (file_stat.st_size == 0)
				{
					/* SEQ ������ �ʱ�ȭ ������ ä���. */
					FQ2SEQ stSeq;
					
					memset(stSeq.sWSeq, '0', sizeof(stSeq.sWSeq));
					memset(stSeq.sRSeq, '0', sizeof(stSeq.sRSeq));
					stSeq.lRPos = 0;
					stSeq.cLF = 0x0a;

					fseek(pFq->fpSeq, 0, SEEK_SET);
					fwrite((char*)&stSeq, 1, 8+8+8+1, pFq->fpSeq);
					fflush(pFq->fpSeq);
				}
			}

			if (flag)
			{
				/*
				 * ����(permission)�� �����Ѵ�. 0660
				 */
				/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
				chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
			}
		}

		if (flag)
		{
			/*
			 * DAT ������ �����Ѵ�.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.dat"), sFqName, pFq->sFepid, pFq->sApno);
			UtilSetYYYYMMDD(sFileName);
			pFq->fpDat = fopen(sFileName, "r+");
			if (pFq->fpDat == NULL)
			{
				pFq->fpDat = fopen(sFileName, "w+");
				if (pFq->fpDat == NULL)
				{
					if (flag) LOG(_FL_, 9, 0, "\x1b[41m[ERROR] DAT NULL!![%s] errno=[%d:%s] \x1b[0m" , sFileName, errno, strerror(errno));
					return NULL;
				}
			}

			if (flag)
			{
				/*
				 * ����(permission)�� �����Ѵ�. 0660
				 */
				/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
				chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
			}
		}

		/*===========================================================*/

		if (flag)
		{
			/*
			 * SEQ �ڷḦ �о� ���μ��� ������ �����Ѵ�.
			 */
			fseek(pFq->fpSeq, 0, SEEK_SET);
			fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpSeq);

			pFq->iWSeq = UtilToIntN(pSeq->sWSeq, sizeof(pSeq->sWSeq));
			pFq->iRSeq = UtilToIntN(pSeq->sRSeq, sizeof(pSeq->sRSeq));
		}

		if (flag)
		{
			/*
			 * DAT �ڷḦ �о� ���μ��� ���Ѵ�.
			 * �׸��� iWSeq, iRSeq, lRPos ���� �����Ѵ�.
			 */
			fseek(pFq->fpDat, 0, SEEK_SET);

			iLineNo = 0;
			if (iLineNo == pFq->iRSeq)
			{
				pFq->lRPos = 0;
			}

			while (fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpDat))
			{
				iLineNo ++;
				if (iLineNo == pFq->iRSeq)
				{
					pFq->lRPos = ftell(pFq->fpDat);
				}
			}

			pFq->iWSeq = iLineNo;
			if (iLineNo < pFq->iRSeq)
			{
				pFq->iRSeq = iLineNo;
				pFq->lRPos = ftell(pFq->fpDat);
			}
		}

		if (flag)
		{
			/*
			 * �����۾� ����� �����Ѵ�.
			 */
			fseek(pFq->fpSeq, 0, SEEK_SET);
			fprintf(pFq->fpSeq, "%08d%08d", pFq->iWSeq, pFq->iRSeq);
			fflush(pFq->fpSeq);
		}
	}

	return pFq;
}

/*============================================================================*/
/*
 * ���� : ����ť2�� ��ü�� �ݴ´�.
 */
/*----------------------------------------------------------------------------*/
int FQ2Close(FQ2* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	int ret1, ret2;

	if (flag)
	{
		/* 20140531 ���� ��ȯ�� SIGSEGV �߻� */
		if (pFq == NULL) {
			return 0;
		}
		
		if (!flag) LOG(_FL_, 0, 0, "<DEBUG> CLOSE iRSeq[%d] iWSeq[%d]", pFq->iRSeq, pFq->iWSeq);
		
		ret1 = fclose(pFq->fpSeq);		
		if ( ret1 != 0 ) {			
		    if (flag) LOG(_FL_, 9, 0, "[ERROR] SEQ fclose = [%d]", ret1);
		}
		
		ret2 = fclose(pFq->fpDat);
		if ( ret1 != 0 ) {			
		    if (flag) LOG(_FL_, 9, 0, "[ERROR] DAT fclose = [%d]", ret2);
		}

		free(pFq);
		
		pFq = NULL;
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
int FQ2Print(FQ2* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			if (flag) printf("\t---------- FQ2Print START ------------\n");
			if (flag) printf("\t pFq         = [%x]\n", pFq);
			if (flag) printf("\t pFq->sSvcid = [%s]\n", pFq->sSvcid);
			if (flag) printf("\t pFq->sFepid = [%s]\n", pFq->sFepid);
			if (flag) printf("\t pFq->sApno  = [%s]\n", pFq->sApno );
			if (flag) printf("\t pFq->chSR   = [%c]\n", pFq->chSR  );
			if (flag) printf("\t pFq->chWR   = [%c]\n", pFq->chWR  );
			if (flag) printf("\t pFq->iWSeq  = [%d]\n", pFq->iWSeq );
			if (flag) printf("\t pFq->iRSeq  = [%d]\n", pFq->iRSeq );
			if (flag) printf("\t pFq->lRPos  = [%d]\n", pFq->lRPos );
			if (flag) printf("\t---------- FQ2Print END ------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * ���� : ����ť�� �ڷḦ �ִ´�. ���� ���μ����� ����ϴ� �Լ��̹Ƿ�
 *        ��뿡 ���� ���϶��� �ʿ��ϴ�.
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ2SEQ;
 *
 * typedef struct _FILE_QUEUE2_DAT_
 * {
 *     char    sSeq           [ 8];   // ��������
 *     char    sRecvr         [10];   // ó�� �����̰�, ���� ���� �����
 *     char    sLen           [ 5];   // sData�� �����̰� '\n'���� ����
 *
 *     char    sData          [ 1];   // �����ϰ����ϴ� ������
 * } FQ2DAT;
 *
 */
/*----------------------------------------------------------------------------*/
int FQ2Write(FQ2* pFq, char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	/* 20140820 SIGSEGV �߻����� ����*/
	if (pFq == NULL) {
		if (flag) LOG(_FL_, 0, 0, "<ERROR> FQ2Write FQ2 NULL!!! [%s] ", str);
		return -1;
	}
	
	int len = strlen(str);
	
	if (flag)
	{
		pFq->iWSeq ++;
	}
	
	if (flag)
	{
		if (!flag) LOG(_FL_, 0, 0, "<DEBUG> FQ2Write DAT ��� [%d] ", pFq->iWSeq);
		/*
		 * DAT�� write �Ѵ�.
		 */
		fseek(pFq->fpDat, 0, SEEK_END);

		fprintf(pFq->fpDat, "%08d%-10.10s%05d%s\n"
			, pFq->iWSeq
			, ""                /* pFq->sSvcid */
			, len + 1           /* include new line (0x0A) */
			, str);

		fflush(pFq->fpDat);
	}

	if (flag)
	{
		if (!flag) LOG(_FL_, 0, 0, "<DEBUG> FQ2Write SEQ ��� [%d] ", pFq->iWSeq);
		/*
		 * SEQ�� write �Ѵ�.
		 */
		if (!flag) printf("FQ2Write [iWSeq=%d]\n", pFq->iWSeq);

		fseek(pFq->fpSeq, 0, SEEK_SET);
		fprintf(pFq->fpSeq, "%08d", pFq->iWSeq);
		fflush(pFq->fpSeq);
	}
	
	if (flag) LOG(_FL_, 0, 0, "<DEBUG> FQ2Write RETURN=[%d] ", len);

	return len;
}

/*============================================================================*/
/*
 * ���� : ����ť�� �ڷḦ �д´�.
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ2SEQ;
 *
 * typedef struct _FILE_QUEUE2_DAT_
 * {
 *     char    sSeq           [ 8];   // ��������
 *     char    sRecvr         [10];   // ó�� �����̰�, ���� ���� �����
 *     char    sLen           [ 5];   // sData�� �����̰� '\n'���� ����
 *
 *     char    sData          [ 1];   // �����ϰ����ϴ� ������
 * } FQ2DAT;
 *
 */
/*----------------------------------------------------------------------------*/
char* FQ2Read(FQ2* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/* 20140531 ���� ��ȯ�� SIGSEGV �߻� */
		if (pFq == NULL) {
			if (flag) LOG(_FL_, 0, 0, "[ERROR] NULL");
			return NULL;;
		}
		/*
		 * DAT�� read �Ѵ�.
		 */
		long rPos = pFq->lRPos;
		
		if (flag)
		{
			/* �Ѱ��� ���ڵ带 �д´�. */
			fseek(pFq->fpDat, pFq->lRPos, SEEK_SET);
			
			if (fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpDat) == NULL)
			{
				if (!flag) LOG(_FL_, 0, 0,  "READ NULL STRING > [iWSeq=%d][iRSeq=%d][lRPos=%d]"
					, pFq->iWSeq
					, pFq->iRSeq
					, pFq->lRPos);

				return NULL;
			}

			int len = strlen(sFQDatData);

			sFQDatData[len - 1] = 0x00;        /* new line ���� */

			/* ������ ���� �����͸� ��´�. */
			if (!flag) pFq->lRPos = ftell(pFq->fpDat);
			if (flag) pFq->lRPos = pFq->lRPos + len;
		}
		
		if (flag)
		{
			/* Receiver�� ����Ѵ�. */
			if (!flag) printf("FQ2Read [sRecvr=%-10.10s]\n", pFq->sSvcid);
			
			fseek(pFq->fpDat, rPos + 8, SEEK_SET);
			fprintf(pFq->fpDat, "%-10.10s", pFq->sSvcid);
			fflush(pFq->fpDat);
		}
	}

	if (flag)
	{
		/* ���� ���ڵ� �Ǽ��� ������Ų��. */
		pFq->iRSeq ++;
	}

	if (flag)
	{
		/*
		 * ������Ų R-SEQ�� write �Ѵ�.
		 */
		if (!flag) printf("FQ2Read [iRSeq=%d]\n", pFq->iRSeq);

		fseek(pFq->fpSeq, 8, SEEK_SET);
		fprintf(pFq->fpSeq, "%08d", pFq->iRSeq);
		fflush(pFq->fpSeq);
	}

	return (char*) pDat->sData;
}

/*============================================================================*/
/*
 * ���� : ����ť�� �ڷḦ �д´�.
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ2SEQ;
 *
 */
/*----------------------------------------------------------------------------*/
int FQ2DiffSeq(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	int return_close;

	if (flag)
	{
		char sFileName[128];
		FILE* fp;
		char sLine[128];
		
		/*
		 * SEQ ������ �����Ѵ�.
		 */
		sprintf(sFileName, CfgGetMst("ib.path.file.fq.seq"), sFqName, sServiceid+3, sServiceid+8);
		UtilSetYYYYMMDD(sFileName);
		fp = fopen(sFileName, "r+");
		if (fp == NULL)
		{
			fp = fopen(sFileName, "w+");
			if (fp == NULL)
			{
				return 0;
			}
		}

		/*
		 * SEQ �ڷḦ �о� ���̸� ���Ѵ�.
		 */
		fseek(fp, 0, SEEK_SET);
		fgets(sLine, sizeof(sLine), fp);

		int ret = UtilToIntN(sLine, 8) - UtilToIntN(sLine + 8, 8);
		
		return_close = fclose(fp);
		
		if ( return_close != 0 ) {			
		    if (flag) LOG(_FL_, 9, 0, "[ERROR] fclose = [%d]", return_close);
		}
		
		return ret;
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

	sProgName = argv[0];

	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	/*
	 * master ȯ�������� �д´�.
	 */
	if (CfgMst() < 0)
	{
		printf("ERROR : get config....\n");
		exit(9);
	}

	/*=================================================================*/

	if (!flag)
	{
		/*
		 * FQ2Init �׽�Ʈ
		 */
		 
		FQ2Init("OFS", "OFSTMP0101");
		FQ2Init("OFR", "OFSTMP0101");
		FQ2Init("OAS", "OFSTMP0101");
		FQ2Init("OAR", "OFSTMP0101");
	}
	
	if (!flag)
	{
		/*
		 * FQ2Open / FQ2Close �׽�Ʈ
		 */
		 
		FQ2* pFq = NULL;
		
		pFq = FQ2Open("OFS", "OFSTMP0101");
		FQ2Print(pFq);
		FQ2Close(pFq);

		pFq = FQ2Open("OFR", "OFRTMP0101");
		FQ2Print(pFq);
		FQ2Close(pFq);

		pFq = FQ2Open("OFS", "OFSTMP0101");
		FQ2Print(pFq);
		FQ2Close(pFq);

		pFq = FQ2Open("OFR", "OFRTMP0101");
		FQ2Print(pFq);
		FQ2Close(pFq);
	}
	
	if (!flag)
	{
		/*
		 * FQ2Write �׽�Ʈ
		 */
		 
		char* str = 
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789";
		char buf[1024];
		
		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 100; i++)
		{
			sprintf(buf, "SUCCESS_%-*.*s", i, i, str);
			FQ2Write(pFq, buf);
		}

		FQ2Close(pFq);
	}
	
	/*=================================================================*/

	if (!flag && argc == 2)
	{
		/*
		 * FQ2Write �������� �׽�Ʈ - 1
		 */
		 
		char* str = 
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789";
		char buf[1024];
		
		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 100; i++)
		{
			int len = 50 + (RandGetInt() % 50) * 10;
			sprintf(buf, "SUCCESS_%-*.*s", len, len, str);
			FQ2Write(pFq, buf);
		}

		FQ2Close(pFq);
	}
	
	if (!flag && argc == 1)
	{
		/*
		 * FQ2Read �������� �׽�Ʈ - 1
		 */
		 
		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 1000000; i++)
		{
			char* str = FQ2Read(pFq);
			if (str == NULL)
			{
				printf("NO DATA\n");
			}
			else
			{
				printf("> (%3d) %s\n", strlen(str), str);
			}
			
			sleep(1);
		}

		FQ2Close(pFq);
	}
	
	/*=================================================================*/

	if (flag && argc == 2)
	{
		/*
		 * FQ2Write �������� �׽�Ʈ - 2
		 */
		 
		char* str = 
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789";
		char buf[1024];

		int i;
		for (i=0; i < 100; i++)
		{
			int len = 50 + (RandGetInt() % 50) * 10;
			sprintf(buf, "SUCCESS_%-*.*s", len, len, str);
			
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");
			FQ2Write(pFq, buf);
			FQ2Close(pFq);
		}

	}
	
	if (flag && argc == 1)
	{
		/*
		 * FQ2Read �������� �׽�Ʈ - 2
		 */

		int i;
		for (i=0; i < 1000000; i++)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");
			char* str = FQ2Read(pFq);
			FQ2Close(pFq);
			
			if (str == NULL)
			{
				printf("NO DATA\n");
			}
			else
			{
				printf("> (%3d) %s\n", strlen(str), str);
			}
			
			sleep(1);
		}
	}
	
	/*=================================================================*/
	
	if (!flag && argc == 1)
	{
		/*
		 * �ٰ��� �ѹ��� ����ϰ� ���� �ٰ� �̻��� �ѹ��� �д´�.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		FQ2Write(pFq, str);
		LOG(_FL_, 0, 0,  "WRITE");

		FQ2Close(pFq);
	}

	if (!flag && argc == 2)
	{
		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		char* str = FQ2Read(pFq);
		if (str == NULL)
		{
			LOG(_FL_, 0, 0,  "NO DATA");
		}
		else
		{
			LOG(_FL_, 0, 0,  "> (%d) %s", strlen(str), str);
		}

		FQ2Close(pFq);
	}

	if (!flag)
	{
		/*
		 * �ܰ��� �Է��Ͽ� �о��.
		 */
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		FQ2Write(pFq, str);

		str = FQ2Read(pFq);
		LOG(_FL_, 0, 0,  "READ> (%d)[%s]", strlen(str), str);

		FQ2Close(pFq);
	}

	if (!flag)
	{
		/*
		 * ������ Open/Close �ϸ� ���� ���ÿ� �д´�.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ2Init("OFS", "OFSTMP0101");

		for (i=0; i < 10; i++)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

			FQ2Write(pFq, str);

			FQ2Close(pFq);
		}

		for (i=0; i < 10; i++)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

			FQ2Write(pFq, str);

			str = FQ2Read(pFq);
			LOG(_FL_, 0, 0,  "> (%d)[%s]", strlen(str), str);

			FQ2Close(pFq);
		}
	}

	if (!flag)
	{
		/*
		 * �ѹ� Open�Ͽ� �ٰ��� ���� ���ÿ� �д´�.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

		for (i=0; i < 100; i++)
		{
			FQ2Write(pFq, str);

			str = FQ2Read(pFq);
			LOG(_FL_, 0, 0,  "> (%d)[%s]", strlen(str), str);
		}

		FQ2Close(pFq);
	}

	if (!flag)
	{
		/*
		 * �ٰ��� �ѹ��� ����ϰ� ���� �ٰ� �̻��� �ѹ��� �д´�.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		if (flag)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

			for (i=0; i < 1000; i++)
			{
				FQ2Write(pFq, str);
			}

			FQ2Close(pFq);
		}

		if (flag)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

			for (i=0; i < 1500; i++)
			{
				str = FQ2Read(pFq);
				if (str == NULL)
					break;

				LOG(_FL_, 0, 0,  "> (%d) %s", strlen(str), str);
			}

			FQ2Close(pFq);
		}
	}


	if (!flag)
	{
		/*
		 * �ѹ� Open�Ͽ� �ٰ��� ���� ���ÿ� �д´�.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		for (i=0; i < 10000; i++)
		{
			FQ2* pFq = FQ2Open("OFS", "OFSTMP0101");

			FQ2Write(pFq, str);

			LOG(_FL_, 0, 0,  "%d) [%d:%s]", i, strlen(str), str);

			FQ2Close(pFq);
		}
	}
}

#endif