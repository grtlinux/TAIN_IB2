/*
 * PROGRAM ID   : Seq.c
 * PROGRAM NAME : Seq ���� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Seq ���� ���
 * HISTORY      : 2013.08.10 ����. �����ۼ�
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
 * ���� : Seq�� �ʱⰪ�� �����Ѵ�.
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
 * ���� : Seq�� �ʱⰪ�� �����Ѵ�.
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
			 * pSeq ���� �ʱ�ȭ�Ѵ�.
			 */
			pSeq = (SEQ_REC*) SeqInit();
		}

		if (flag)
		{
			/*
			 * idx�� �ش��ϴ� SEQ_REC�� FEPID�� sFepid�� ��� SeqNo�� �д´�.
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
			 * �ش� FEPID �� ã�´�.
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
			 * �ش� FEPID�� ������ ����ִ� �ڸ��� ã�´�.
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
 * ������ ������ �Լ��� �׽�Ʈ �Ѵ�.
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
