/*
 * PROGRAM ID   : Seq.c
 * PROGRAM NAME : Seq 변수 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : Seq 변수 모듈
 * HISTORY      : 2013.08.10 강석. 최초작성
 *
 */

#include <Common.h>

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/*
 * 내용 : Seq의 초기값을 세팅한다.
 */
/*----------------------------------------------------------------------------*/
char* SeqInit()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	if (flag)
	{
		CfgMst();
		if (!flag) LOG(_FL_, 0, 0, "seqno file = [%s]", CfgGetMst("ib.path.file.man.seqno"));

		char* sFileName = CfgGetMst("ib.path.file.man.seqno");
		int iSize       = sizeof(SEQ_REC) * SEQ_MAX_REC;

		SEQ_REC* pSeq = (SEQ_REC*) MmapOpen(sFileName, iSize);

		if (pSeq == NULL)
		{
			pSeq = (SEQ_REC*) MmapCreate(sFileName, iSize);
			
			int i;
			for (i=0; i < SEQ_MAX_REC; i++)
			{
				snprintf((char*)&pSeq[i], sizeof(SEQ_REC)+1, "%*.*s%*.*s%*.*s%*.*s\n"
					, sizeof(pSeq[i].sFlag ), sizeof(pSeq[i].sFlag ), "N"
					, sizeof(pSeq[i].sFepid), sizeof(pSeq[i].sFepid), ""
					, sizeof(pSeq[i].sSvcNo), sizeof(pSeq[i].sSvcNo), ""
					, sizeof(pSeq[i].sSeqNo), sizeof(pSeq[i].sSeqNo), "0");
			}
	
			MmapSync(pSeq, iSize);
		}
		
		return (char*) pSeq;
	}

	return NULL;
}

/*============================================================================*/
/*
 * 내용 : Seq의 초기값을 세팅한다.
 */
/*----------------------------------------------------------------------------*/
int SeqGetNo(char* sFepid)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	static SEQ_REC* pSeq = NULL;
	static int idx = 0;

	if (flag)
	{
		if (pSeq == NULL)
		{
			/*
			 * pSeq 값을 초기화한다.
			 */
			pSeq = (SEQ_REC*) SeqInit();
		}

		if (flag)
		{
			/*
			 * idx에 해당하는 SEQ_REC의 FEPID가 sFepid면 기냥 SeqNo를 읽는다.
			 */
			if (strncmp(pSeq[idx].sFepid, sFepid, sizeof(pSeq[idx].sFepid)) == 0)
			{
				int retSeqNo = UtilToIntN(pSeq[idx].sSeqNo, sizeof(pSeq[idx].sSeqNo)) + 1;

				char buf[100];
				
				sprintf(buf, "%*d", sizeof(pSeq[idx].sSeqNo), retSeqNo);
				if (!flag) LOG(_FL_, 0, 0,  ": debug[%s][%s]", sFepid, buf);

				memcpy((char*) pSeq[idx].sSeqNo, buf, sizeof(pSeq[idx].sSeqNo));

				MmapSync(pSeq, sizeof(SEQ_REC) * SEQ_MAX_REC);
				
				return retSeqNo;
			}
		}

		if (flag)
		{
			int i;

			/*
			 * 해당 FEPID 를 찾는다.
			 */
			for (i = 0; i < SEQ_MAX_REC && pSeq[i].sFlag[0] == 'Y'; i++)
			{
				if (strncmp(pSeq[i].sFepid, sFepid, sizeof(pSeq[i].sFepid)) == 0)
				{
					idx = i;

					int retSeqNo = UtilToIntN(pSeq[idx].sSeqNo, sizeof(pSeq[idx].sSeqNo)) + 1;

					char buf[100];
					
					sprintf(buf, "%*d", sizeof(pSeq[idx].sSeqNo), retSeqNo);
					if (!flag) LOG(_FL_, 0, 0,  ": debug[%s][%s]", sFepid, buf);

					memcpy((char*) pSeq[idx].sSeqNo, buf, sizeof(pSeq[idx].sSeqNo));

					MmapSync(pSeq, sizeof(SEQ_REC) * SEQ_MAX_REC);
				
					return retSeqNo;
				}
			}

			/*
			 * 해당 FEPID가 없으면 비어있는 자리를 찾는다.
			 */
			for (;i < SEQ_MAX_REC; i++)
			{
				if (pSeq[i].sFlag[0] == 'N')
				{
					idx = i;
					
					char buf[100];

					sprintf(buf, "%*.*s%*.*s%*.*s%*d\n"
						, sizeof(pSeq[idx].sFlag ), sizeof(pSeq[idx].sFlag ), "Y"
						, sizeof(pSeq[idx].sFepid), sizeof(pSeq[idx].sFepid), sFepid
						, sizeof(pSeq[idx].sSvcNo), sizeof(pSeq[idx].sSvcNo), "01"
						, sizeof(pSeq[idx].sSeqNo), 1);
						
					if (!flag) LOG(_FL_, 0, 0,  ": debug[%s][%s]", sFepid, buf);
					
					memcpy((char*) pSeq[idx].sFlag, buf, sizeof(SEQ_REC));

					MmapSync(pSeq, sizeof(SEQ_REC) * SEQ_MAX_REC);
					
					return 1;
				}
			}
		}
	}

	return -1;
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

	if (flag)
	{
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP01", SeqGetNo("TMP01"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP02", SeqGetNo("TMP02"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP01", SeqGetNo("TMP01"));

		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP03", SeqGetNo("TMP03"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP02", SeqGetNo("TMP02"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP01", SeqGetNo("TMP01"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP04", SeqGetNo("TMP04"));

		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP01", SeqGetNo("TMP01"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP05", SeqGetNo("TMP05"));
		LOG(_FL_, 0, 0, "SeqGetNo = [%s][%d]", "TMP02", SeqGetNo("TMP02"));

	}
}

#endif
