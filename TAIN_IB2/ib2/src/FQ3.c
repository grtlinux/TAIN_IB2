/*
 * PROGRAM ID   : FQ3.c
 * PROGRAM NAME : 파일큐에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 파일큐에 사용하는 함수들을 정의한다.
 * HISTORY      : 2013.10.16 강석. 최초작성
 *
 * COMMENT      : N 개의 writer 와 1개의 reader 인 경우를 적용함.
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

FQ3SEQ*    pSeq             = (FQ3SEQ*) &sFQDatData;
FQ3DAT*    pDat             = (FQ3DAT*) &sFQDatData;

FQ3        stFq                                    ;

/*============================================================================*/
/*
 * 내용 : 파일큐2의 객체를 오픈한다.
 *
 * typedef struct _FILE_QUEUE3_
 * {
 *     char    sSvcid        [ 10+1]; // Service ID
 *     char    sFepid        [  5+1]; // FEPID
 *     char    sApno         [  2+1]; // AP서버 번호
 *
 *     char    chSR                 ; // 송수신구분
 *     char    chWR                 ; // 쓰기읽기구분
 *
 *     int     iWSeq                ; // write sequence
 *     int     iRSeq                ; // read sequence
 *
 *     long    lRPos                ; // read position
 *
 *     FILE*   fpSeq                ; // seq file
 *     FILE*   fpDat                ; // data file
 * } FQ3;
 *
 * typedef struct _FILE_QUEUE3_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ3SEQ;
 *
 * typedef struct _FILE_QUEUE3_DAT_
 * {
 *     char    sSeq           [ 8];   // 전문순번
 *     char    sRecvr         [10];   // 처음 공백이고, 읽은 놈이 기록함
 *     char    sLen           [ 5];   // sData의 길이이고 '\n'까지 포함
 *
 *     char    sData          [ 1];   // 전송하고자하는 데이터
 * } FQ3DAT;
 *
 *
 */

/*============================================================================*/
/*
 * 내용 : FQ3를 사용할 준비를 한다. 폴더생성 및 파일생성
 */
/*----------------------------------------------------------------------------*/
int FQ3Init(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * 관련폴더를 만든다.
		 * 그냥 폴더생성명령 mkdir을 실행한다. 이미 있으면 return -1, 아니면
		 * 생성되는데 폴더의 확인을 하기 위함이다.
		 */
		char* p;
		char sFileName[128];

		if (flag)
		{
			/* SEQ 폴더 */
			strcpy(sFileName, CfgGetMaster("ib.path.file.fq.seq"));
			UtilSetYYYYMMDD(sFileName);
			p = rindex(sFileName, '/');
			*p = 0x00;

			mkdir(sFileName, 0777);
		}

		if (flag)
		{
			/* DAT 폴더 */
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
			 * 서비스명에서 관련 FEPID, AP번호를 얻는다.
			 */
			sprintf(sFepid, "%-5.5s", sServiceid + 3);
			sprintf(sApNum, "%-2.2s", sServiceid + 8);
		}

		if (flag)
		{
			/*
			 * SEQ 파일을 확인하여 생성하거나 초기화한다.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.seq"), sFqName, sFepid, sApNum);
			UtilSetYYYYMMDD(sFileName);
			if (flag) printf("\t FQ3Init SEQ => [%s]\n", sFileName);

			if (access(sFileName, W_OK) < 0)
			{
				/*
				 * 파일이 없으면 파일 생성과 초기화를 실행한다.
				 */

				FILE* fp = fopen(sFileName, "w+");

				if (flag)
				{
					/*
					 * SEQ 초기화 한다.
					 */
					FQ3SEQ stSeq;
					
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
					 * 권한(permission)을 세팅한다. 0660
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
			 * DAT 파일을 확인하여 생성한다.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.dat"), sFqName, sFepid, sApNum);
			UtilSetYYYYMMDD(sFileName);
			if (flag) printf("\t FQ3Init DAT => [%s]\n", sFileName);

			if (access(sFileName, W_OK) < 0)
			{
				/*
				 * 파일이 없으면 파일 생성과 초기화를 실행한다.
				 */

				FILE* fp = fopen(sFileName, "w+");

				if (flag)
				{
					/*
					 * 권한(permission)을 세팅한다. 0660
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
 * 내용 : 파일큐2의 객체를 오픈한다.
 *
 * typedef struct _FILE_QUEUE3_DAT_
 * {
 *     char    sSeq           [ 8];   // 전문순번
 *     char    sRecvr         [10];   // 처음 공백이고, 읽은 놈이 기록함
 *     char    sLen           [ 5];   // sData의 길이이고 '\n'까지 포함
 *
 *     char    sData          [ 1];   // 전송하고자하는 데이터
 * } FQ3DAT;
 *
 */
/*----------------------------------------------------------------------------*/
char* FQ3fgets(char* str, int maxlen, FILE* fp)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
    if (!flag) LOG(_FL_, 0, 0, "str[%s]/maxlen[%d]", str, maxlen);
    	
	if (flag)
	{
		int ret;
		int len;
		FQ3DAT* p = (FQ3DAT*) str;
		
		/*
		 * 레코드의 정보를 읽는다.
		 */
		ret = fread(p->sSeq, 1, sizeof(FQ3DAT) - 1, fp);
		if (ret != sizeof(FQ3DAT) - 1)
		{
			/* 읽을 자료가 없다. */
			return NULL;
		}
		
#if 0	
		if (flag) LOG(_FL_, 0, 0, "FQ3DAT p->sSeq  [%.8s]", p->sSeq  );
		if (flag) LOG(_FL_, 0, 0, "FQ3DAT p->sRecvr[%.10s]", p->sRecvr);
		if (flag) LOG(_FL_, 0, 0, "FQ3DAT p->sLen  [%.5s]", p->sLen  );
		if (flag) LOG(_FL_, 0, 0, "FQ3DAT p->sData [%.1s]", p->sData );
			
		if (!flag) sleep(1);
#endif
		
		/*
		 * 레코드의 길이를 구한다.
		 */
		len = UtilToIntN(p->sLen, sizeof(p->sLen));
		
		if (!flag) LOG(_FL_, 0, 0, "레코드 길이[%d]", len);
			
		/*
		 * 레코드의 자료를 읽는다.
		 */
		ret = fread(p->sData, 1, len, fp);
		
		if (!flag) LOG(_FL_, 0, 0, "레코드 자료읽기[%d]", ret);
			
		if (!flag) LOG(_FL_, 0, 0, "개행 여부[%x]", p->sData[len-1]);
			
		if (ret != len)
		{
			/* 읽으려는 길이와 읽은 길이가 다르면 자료의 오류로 판단 */
			if (flag) LOG(_FL_, 0, 0, "ERROR : WRONG RECORD LEN [ret:%d] != [len:%d]", ret, len);
			
			return NULL;
		}
		else if (p->sData[len-1] != 0x0A)
		{
			/* 레코드 자료 : 읽은 자료의 레코드 구분자 오류 */
			if (flag) LOG(_FL_, 0, 0, "ERROR : RECORD SEP [0x%02X]", p->sData[len-1]);
			
			return NULL;
		}
		
		p->sData[len] = 0x00;
	}
	
	return str;
}

/*============================================================================*/
/*
 * 내용 : 파일큐2의 객체를 오픈한다.
 */
/*----------------------------------------------------------------------------*/
FQ3* FQ3Open(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	/* FQ3 구조체 메모리를 할당한다. */
	FQ3* pFq = malloc(sizeof(FQ3));

	if (flag)
	{
		char sFileName[128];

		if (flag)
		{
			/*
			 * FQ 구조체를 채운다.
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
			 * SEQ 파일을 오픈한다.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.seq"), sFqName, pFq->sFepid, pFq->sApno);
			UtilSetYYYYMMDD(sFileName);
			pFq->fpSeq = fopen(sFileName, "r+");
			if (pFq->fpSeq == NULL)
			{
				pFq->fpSeq = fopen(sFileName, "w+");
				if (pFq->fpSeq == NULL)
				{
					return NULL;
				}
			}

			if (flag)
			{
				/*
				 * SEQ 크기가 0이면 초기화 한다.
				 */
				struct stat file_stat;
				fstat(fileno(pFq->fpSeq), &file_stat);

				if (!flag) LOG(_FL_, 0, 0,  "SEQ file size = %d", file_stat.st_size);

				if (file_stat.st_size == 0)
				{
					/* SEQ 파일을 초기화 값으로 채운다. */
					FQ3SEQ stSeq;
					
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
				 * 권한(permission)을 세팅한다. 0660
				 */
				/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
				chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
			}
		}

		if (flag)
		{
			/*
			 * DAT 파일을 오픈한다.
			 */
			sprintf(sFileName, CfgGetMaster("ib.path.file.fq.dat"), sFqName, pFq->sFepid, pFq->sApno);
			UtilSetYYYYMMDD(sFileName);
			pFq->fpDat = fopen(sFileName, "r+");
			if (pFq->fpDat == NULL)
			{
				pFq->fpDat = fopen(sFileName, "w+");
				if (pFq->fpDat == NULL)
				{
					return NULL;
				}
			}

			if (flag)
			{
				/*
				 * 권한(permission)을 세팅한다. 0660
				 */
				/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
				chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
			}
		}

		/*===========================================================*/

		if (flag)
		{
			/*
			 * SEQ 자료를 읽어 라인수의 값으로 정리한다.
			 */
			fseek(pFq->fpSeq, 0, SEEK_SET);
			fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpSeq);

			pFq->iWSeq = UtilToIntN(pSeq->sWSeq, sizeof(pSeq->sWSeq));
			pFq->iRSeq = UtilToIntN(pSeq->sRSeq, sizeof(pSeq->sRSeq));
		}

		if (flag)
		{
			/*
			 * DAT 자료를 읽어 라인수를 구한다.
			 * 그리고 iWSeq, iRSeq, lRPos 값을 보정한다.
			 */
			fseek(pFq->fpDat, 0, SEEK_SET);

			iLineNo = 0;
			if (iLineNo == pFq->iRSeq)
			{
				pFq->lRPos = 0;
			}

            if (!flag) LOG(_FL_, 0, 0, "FQ3fgets 호출[%s]", sFQDatData);
            	
			while (FQ3fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpDat))
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
			 * 보정작업 결과를 저장한다.
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
 * 내용 : 파일큐2에 객체를 닫는다.
 */
/*----------------------------------------------------------------------------*/
int FQ3Close(FQ3* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		fclose(pFq->fpSeq);
		fclose(pFq->fpDat);

		free(pFq);
		
		pFq = NULL;
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 출력한다.
 */
/*----------------------------------------------------------------------------*/
int FQ3Print(FQ3* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		if (flag)
		{
			if (flag) printf("\t---------- FQ3Print START ------------\n");
			if (flag) printf("\t pFq         = [%x]\n", pFq);
			if (flag) printf("\t pFq->sSvcid = [%s]\n", pFq->sSvcid);
			if (flag) printf("\t pFq->sFepid = [%s]\n", pFq->sFepid);
			if (flag) printf("\t pFq->sApno  = [%s]\n", pFq->sApno );
			if (flag) printf("\t pFq->chSR   = [%c]\n", pFq->chSR  );
			if (flag) printf("\t pFq->chWR   = [%c]\n", pFq->chWR  );
			if (flag) printf("\t pFq->iWSeq  = [%d]\n", pFq->iWSeq );
			if (flag) printf("\t pFq->iRSeq  = [%d]\n", pFq->iRSeq );
			if (flag) printf("\t pFq->lRPos  = [%d]\n", pFq->lRPos );
			if (flag) printf("\t---------- FQ3Print END ------------\n");
		}
	}

	return 0;
}

/*============================================================================*/
/*
 * 내용 : 파일큐에 자료를 넣는다. 여러 프로세스가 사용하는 함수이므로
 *        사용에 대한 파일락이 필요하다.
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ3SEQ;
 *
 * typedef struct _FILE_QUEUE2_DAT_
 * {
 *     char    sSeq           [ 8];   // 전문순번
 *     char    sRecvr         [10];   // 처음 공백이고, 읽은 놈이 기록함
 *     char    sLen           [ 5];   // sData의 길이이고 '\n'까지 포함
 *
 *     char    sData          [ 1];   // 전송하고자하는 데이터
 * } FQ3DAT;
 *
 */
/*----------------------------------------------------------------------------*/
int FQ3Write(FQ3* pFq, char* str)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int len = strlen(str);
	
	if (flag)
	{
		pFq->iWSeq ++;
	}

	if (flag)
	{
		/*
		 * DAT를 write 한다.
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
		/*
		 * SEQ를 write 한다.
		 */
		if (!flag) printf("FQ3Write [iWSeq=%d]\n", pFq->iWSeq);

		fseek(pFq->fpSeq, 0, SEEK_SET);
		fprintf(pFq->fpSeq, "%08d", pFq->iWSeq);
		fflush(pFq->fpSeq);
	}

	return len;
}

/*============================================================================*/
/*
 * 내용 : 파일큐의 자료를 읽는다.
 *
 * typedef struct _FILE_QUEUE2_SEQ_
 * {
 *     char    sWSeq          [ 8];   // write sequence
 *     char    sRSeq          [ 8];   // read sequence
 *     long    lRPos              ;   // Reserved                         
 *     char    cLF                ;   // new line, line feed              
 * } FQ3SEQ;
 *
 * typedef struct _FILE_QUEUE2_DAT_
 * {
 *     char    sSeq           [ 8];   // 전문순번
 *     char    sRecvr         [10];   // 처음 공백이고, 읽은 놈이 기록함
 *     char    sLen           [ 5];   // sData의 길이이고 '\n'까지 포함
 *
 *     char    sData          [ 1];   // 전송하고자하는 데이터
 * } FQ3DAT;
 *
 */
/*----------------------------------------------------------------------------*/
char* FQ3Read(FQ3* pFq)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		/*
		 * DAT를 read 한다.
		 */
		long rPos = pFq->lRPos;

		if (flag)
		{
			/* 한건의 레코드를 읽는다. */
			fseek(pFq->fpDat, pFq->lRPos, SEEK_SET);
			
			if (!flag) LOG(_FL_, 0, 0, "FQ3fgets 호출[%s]", sFQDatData);

			if (FQ3fgets(sFQDatData, MAX_LINE_SIZE, pFq->fpDat) == NULL)
			{
				if (!flag) LOG(_FL_, 0, 0,  "READ NULL STRING > [iWSeq=%d][iRSeq=%d][lRPos=%d]"
					, pFq->iWSeq
					, pFq->iRSeq
					, pFq->lRPos);

				return NULL;
			}

			int len = strlen(sFQDatData);

			sFQDatData[len - 1] = 0x00;        /* new line 제거 */

			/* 현재의 파일 포인터를 얻는다. */
			if (!flag) pFq->lRPos = ftell(pFq->fpDat);
			if (flag) pFq->lRPos = pFq->lRPos + len;
		}

		if (flag)
		{
			/* Receiver를 기록한다. */
			if (!flag) printf("FQ3Read [sRecvr=%-10.10s]\n", pFq->sSvcid);
			
			fseek(pFq->fpDat, rPos + 8, SEEK_SET);
			fprintf(pFq->fpDat, "%-10.10s", pFq->sSvcid);
			fflush(pFq->fpDat);
		}
	}

	if (flag)
	{
		/* 읽은 레코드 건수를 증가시킨다. */
		pFq->iRSeq ++;
	}

	if (flag)
	{
		/*
		 * 증가시킨 R-SEQ를 write 한다.
		 */
		if (!flag) printf("FQ3Read [iRSeq=%d]\n", pFq->iRSeq);

		fseek(pFq->fpSeq, 8, SEEK_SET);
		fprintf(pFq->fpSeq, "%08d", pFq->iRSeq);
		fflush(pFq->fpSeq);
	}

	return (char*) pDat->sData;
}

/*============================================================================*/
/*
 * 내용 : 파일큐의 자료를 읽는다.
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
int FQ3DiffSeq(char* sFqName, char* sServiceid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		char sFileName[128];
		FILE* fp;
		char sLine[128];
		
		/*
		 * SEQ 파일을 오픈한다.
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
		 * SEQ 자료를 읽어 차이를 구한다.
		 */
		fseek(fp, 0, SEEK_SET);
		fgets(sLine, sizeof(sLine), fp);

		int ret = UtilToIntN(sLine, 8) - UtilToIntN(sLine + 8, 8);
		
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
 * 위에서 정의한 함수를 테스트 한다.
 */
/*----------------------------------------------------------------------------*/
main(int argc, char* argv[])
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	sProgName = argv[0];

	if (flag) LOG(_FL_, 0, 0, "\x1b[44m< %s >\x1b[0m", sProgName);

	/*
	 * master 환경파일을 읽는다.
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
		 * FQ3Init 테스트
		 */
		 
		FQ3Init("OFS", "OFSTMP0101");
		FQ3Init("OFR", "OFSTMP0101");
		FQ3Init("OAS", "OFSTMP0101");
		FQ3Init("OAR", "OFSTMP0101");
	}
	
	if (!flag)
	{
		/*
		 * FQ3Open / FQ3Close 테스트
		 */
		 
		FQ3* pFq = NULL;
		
		pFq = FQ3Open("OFS", "OFSTMP0101");
		FQ3Print(pFq);
		FQ3Close(pFq);

		pFq = FQ3Open("OFR", "OFRTMP0101");
		FQ3Print(pFq);
		FQ3Close(pFq);

		pFq = FQ3Open("OFS", "OFSTMP0101");
		FQ3Print(pFq);
		FQ3Close(pFq);

		pFq = FQ3Open("OFR", "OFRTMP0101");
		FQ3Print(pFq);
		FQ3Close(pFq);
	}
	
	if (!flag)
	{
		/*
		 * FQ3Write 테스트
		 */
		 
		char* str = 
			"01234567890123456789012345678901234567890123456789"
			"01234567890123456789012345678901234567890123456789";
		char buf[1024];
		
		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 100; i++)
		{
			sprintf(buf, "SUCCESS_%-*.*s", i, i, str);
			FQ3Write(pFq, buf);
		}

		FQ3Close(pFq);
	}
	
	/*=================================================================*/

	if (!flag && argc == 2)
	{
		/*
		 * FQ3Write 가변길이 테스트 - 1
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
		
		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 100; i++)
		{
			int len = 50 + (RandGetInt() % 50) * 10;
			sprintf(buf, "SUCCESS_%-*.*s", len, len, str);
			FQ3Write(pFq, buf);
		}

		FQ3Close(pFq);
	}
	
	if (!flag && argc == 1)
	{
		/*
		 * FQ3Read 가변길이 테스트 - 1
		 */
		 
		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		int i;
		for (i=0; i < 1000000; i++)
		{
			char* str = FQ3Read(pFq);
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

		FQ3Close(pFq);
	}
	
	/*=================================================================*/

	if (!flag && argc == 2)
	{
		/*
		 * FQ3Write 가변길이 테스트 - 2
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
			
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");
			FQ3Write(pFq, buf);
			FQ3Close(pFq);
		}

	}
	
	if (!flag && argc == 1)
	{
		/*
		 * FQ3Read 가변길이 테스트 - 2
		 */

		int i;
		for (i=0; i < 1000000; i++)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");
			char* str = FQ3Read(pFq);
			FQ3Close(pFq);
			
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
		 * 다건을 한번에 기록하고 또한 다건 이상을 한번에 읽는다.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		FQ3Write(pFq, str);
		LOG(_FL_, 0, 0,  "WRITE");

		FQ3Close(pFq);
	}

	if (!flag && argc == 2)
	{
		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		char* str = FQ3Read(pFq);
		if (str == NULL)
		{
			LOG(_FL_, 0, 0,  "NO DATA");
		}
		else
		{
			LOG(_FL_, 0, 0,  "> (%d) %s", strlen(str), str);
		}

		FQ3Close(pFq);
	}

	if (!flag)
	{
		/*
		 * 단건을 입력하여 읽어본다.
		 */
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		FQ3Write(pFq, str);

		str = FQ3Read(pFq);
		LOG(_FL_, 0, 0,  "READ> (%d)[%s]", strlen(str), str);

		FQ3Close(pFq);
	}

	if (!flag)
	{
		/*
		 * 여러번 Open/Close 하며 쓰는 동시에 읽는다.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ3Init("OFS", "OFSTMP0101");

		for (i=0; i < 10; i++)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			FQ3Write(pFq, str);

			FQ3Close(pFq);
		}

		for (i=0; i < 10; i++)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			FQ3Write(pFq, str);

			str = FQ3Read(pFq);
			LOG(_FL_, 0, 0,  "> (%d)[%s]", strlen(str), str);

			FQ3Close(pFq);
		}
	}

	if (!flag)
	{
		/*
		 * 한번 Open하여 다건을 쓰는 동시에 읽는다.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

		for (i=0; i < 100; i++)
		{
			FQ3Write(pFq, str);

			str = FQ3Read(pFq);
			LOG(_FL_, 0, 0,  "> (%d)[%s]", strlen(str), str);
		}

		FQ3Close(pFq);
	}

	if (!flag)
	{
		/*
		 * 다건을 한번에 기록하고 또한 다건 이상을 한번에 읽는다.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		if (flag)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			for (i=0; i < 1000; i++)
			{
				FQ3Write(pFq, str);
			}

			FQ3Close(pFq);
		}

		if (flag)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			for (i=0; i < 1500; i++)
			{
				str = FQ3Read(pFq);
				if (str == NULL)
					break;

				LOG(_FL_, 0, 0,  "> (%d) %s", strlen(str), str);
			}

			FQ3Close(pFq);
		}
	}


	if (!flag)
	{
		/*
		 * 한번 Open하여 다건을 쓰는 동시에 읽는다.
		 */
		int i;
		char* str = "SUCCESS_0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";

		for (i=0; i < 10000; i++)
		{
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			FQ3Write(pFq, str);

			LOG(_FL_, 0, 0,  "%d) [%d:%s]", i, strlen(str), str);

			FQ3Close(pFq);
		}
	}

	/*=================================================================*/
	
	if (flag)
	{
		if (argc != 2)
		{
			printf("USAGE : %s [ w | r ]\n", argv[0]);
			exit(0);
		}
		
		if (strcmp(argv[1], "w") == 0)
		{
			/* WriteFQ */
			
			char* str = "SUCCESS_0123456789012345678\n   \n   901234567890\n  \n   1234567890123456789012345678\n    90123456789012\n\n     345678901234567\n890123456789";
	
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			FQ3Write(pFq, str);

			if (flag) printf("WriteFQ [%d:%s]\n", strlen(str), str);

			FQ3Close(pFq);
		}
		else
		{
			/* ReadFQ */
			
			FQ3* pFq = FQ3Open("OFS", "OFSTMP0101");

			char* str = FQ3Read(pFq);
			if (str == NULL)
			{
				if (flag) printf("No Message...\n");
			}
			else
			{
				if (flag) printf("ReadFQ  [%d:%s]\n", strlen(str), str);
			}

			FQ3Close(pFq);
			
		}
	}
}

#endif
