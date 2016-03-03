/*
 * PROGRAM ID   : Biz.c
 * PROGRAM NAME : Biz ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Biz ���
 * HISTORY      : 2013.10.15 ����. �����ۼ�
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
 * ���� : �������� ���� �����Ѵ�.
 * 
 * OFSKBB0101    &   ��������  ����                         &   Y    &    V1       &  AP   & /hw01/ibridge/ib2/bin/ofscom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OFSKBB0102    &   ��������  ����                         &   N    &    V1       &  AP   & /hw01/ibridge/ib2/bin/ofscom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OFRKBB0101    &   ��������  ����                         &   Y    &    V1       &  AP   & /hw01/ibridge/ib2/bin/ofrcom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OISKBB0101    &   ��������  ����                         &   Y    &    V1       &  FEP  & /hw01/ibridge/ib2/bin/oiscom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OIRKBB0101    &   ��������  ����                         &   Y    &    V1       &  FEP  & /hw01/ibridge/ib2/bin/oircom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OXSKBB0101    &   ��������  ����                         &   Y    &    V1       &  FEP  & /hw01/ibridge/ib2/bin/oxscom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OXRKBB0101    &   ��������  ����                         &   Y    &    V1       &  FEP  & /hw01/ibridge/ib2/bin/oxrcom2     & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * OSAKBB0101    &   ��������  ����                         &   N    &    V1       &  FEP  & /hw01/ibridge/ib2/bin/osacom      & 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 * 
 * [OFSKBB0101]                                                                                                                     
 * 	DESCRIPTION      = ��������  ����                                                                                               
 * 	OBSERVATION_FLAG = Y                                                                                                            
 * 	#RUN_SCRIPT       = /hw01/ibridge/ram/src/xrun   OFSKBB0101                                                                     
 * 	RUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 OFSKBB0101                                                                      
 * 	RUNNING_SERVER   = AP                                                                                                           
 * 	RUNNING_PROCESS  = /hw01/ibridge/ib2/bin/ofscom2                                                                                
 *	RUNNING_TIME     = 0~00:25~23:55 | 1~00:25~23:55 | 2~00:25~23:55 | 3~00:25~23:55 | 4~00:25~23:55 | 5~00:25~23:55 | 6~00:25~23:55
 *
 *  DATE. 2013.10.22 ��� AP02�� �����ϱ����� BizTrans02 �߰�. by ������.
 *  biz.org  : FEP��  AP01�� biz.properties�� ����� ���� ����  BizTrans01( ) 
 *  biz2.org : AP02�� biz.properties�� ����� ���� ����.        BizTrans02( ) 
 */
/*----------------------------------------------------------------------------*/
int BizTrans01( )   
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "TRANS - 01...");

	if (flag)
	{
		char* sFileName = "/hw01/ibridge/ib2/cfg/biz.org";
		FILE* fp;
		char sLine[1024];
		char sFepid[20] = { 0x00 };
		
		fp = fopen(sFileName, "r");
		
		while (fgets(sLine, 1024, fp))
		{
			char* arr[20];
			
			UtilTrim(UtilDelComment(sLine));
			if (strcmp(sLine, "") == 0)
				continue;
			
			UtilStrSplit(sLine, '&', arr);

			if (flag)
			{
				UtilTrim(arr[0]);
				
				if (strncmp(sFepid, arr[0] + 3, 5) != 0)
				{
					printf("##################################################################################################\n");
					strcpy(sFepid, arr[0] + 3);
				}
				
				printf("[%s]\n", arr[0]);
				printf("\tDESCRIPTION      = %s\n", UtilTrim(arr[1]));
				printf("\tOBSERVATION_FLAG = %s\n", UtilTrim(arr[2]));
				
				if (memcmp("BXA", arr[0], 3) == 0)
				{
					/*
					 * ��ġ�ۼ��� �������α׷�
					 */
					if (!flag) printf("\tRUN_SCRIPT       = /hw01/ibridge/ib2/shl/bxacom2.sh %s\n", arr[0]);
					if (flag)  printf("\tRUN_SCRIPT       = %s %s\n", UtilTrim(arr[5]), arr[0]);
				}
				else
				{
					if (strcmp("V1", UtilTrim(arr[3])) == 0)
					{
						/*
						 * Ver 1 �������α׷�
						 */
						printf("\tRUN_SCRIPT       = /hw01/ibridge/shl/xrun %s\n", arr[0]);
						printf("\t#RUN_SCRIPT      = /hw01/ibridge/ib2/shl/ibrun2 %s\n", arr[0]);
					}
					else
					{
						/*
						 * Ver 2 �������α׷�
						 */
						printf("\t#RUN_SCRIPT      = /hw01/ibridge/shl/xrun %s\n", arr[0]);
						printf("\tRUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 %s\n", arr[0]);
					}
				}
				
				printf("\tRUNNING_SERVER   = %s\n", UtilTrim(arr[4]));
				printf("\tRUNNING_PROCESS  = %s\n", UtilTrim(arr[5]));
				printf("\tRUNNING_TIME     = %s\n", UtilTrim(arr[6]));
				printf("\n");
			}
		}
		
		fclose(fp);
		
	}
	
	return 0;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*----------------------------------------------------------------------------*/
int BizTrans02( )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag) LOG(_FL_, 0, 0, "TRANS - 01...");

	if (flag)
	{
		char* sFileName = "/hw01/ibridge/ib2/cfg/biz2.org";
		FILE* fp;
		char sLine[1024];
		char sFepid[20] = { 0x00 };
		
		fp = fopen(sFileName, "r");
		
		while (fgets(sLine, 1024, fp))
		{
			char* arr[20];
			
			UtilTrim(UtilDelComment(sLine));
			if (strcmp(sLine, "") == 0)
				continue;
			
			UtilStrSplit(sLine, '&', arr);

			if (flag)
			{
				UtilTrim(arr[0]);
				
				if (strncmp(sFepid, arr[0] + 3, 5) != 0)
				{
					printf("##################################################################################################\n");
					strcpy(sFepid, arr[0] + 3);
				}
				
				printf("[%s]\n", arr[0]);
				printf("\tDESCRIPTION      = %s\n", UtilTrim(arr[1]));
				printf("\tOBSERVATION_FLAG = %s\n", UtilTrim(arr[2]));
				
				if (memcmp("BXA", arr[0], 3) == 0)
				{
					/*
					 * ��ġ�ۼ��� �������α׷�
					 */
					if (!flag) printf("\tRUN_SCRIPT       = /hw01/ibridge/ib2/shl/bxacom2.sh %s\n", arr[0]);
					if (flag)  printf("\tRUN_SCRIPT       = %s %s\n", UtilTrim(arr[5]), arr[0]);
				}
				else
				{
					if (strcmp("V1", UtilTrim(arr[3])) == 0)
					{
						/*
						 * Ver 1 �������α׷�
						 */
						printf("\tRUN_SCRIPT       = /hw01/ibridge/shl/xrun %s\n", arr[0]);
						printf("\t#RUN_SCRIPT      = /hw01/ibridge/ib2/shl/ibrun2 %s\n", arr[0]);
					}
					else
					{
						/*
						 * Ver 2 �������α׷�
						 */
						printf("\t#RUN_SCRIPT      = /hw01/ibridge/shl/xrun %s\n", arr[0]);
						printf("\tRUN_SCRIPT       = /hw01/ibridge/ib2/shl/ibrun2 %s\n", arr[0]);
					}
				}
				
				printf("\tRUNNING_SERVER   = %s\n", UtilTrim(arr[4]));
				printf("\tRUNNING_PROCESS  = %s\n", UtilTrim(arr[5]));
				printf("\tRUNNING_TIME     = %s\n", UtilTrim(arr[6]));
				printf("\n");
			}
		}
		
		fclose(fp);
		
	}
	
	return 0;
}

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
		if (argc != 2)
		{
			if (flag) printf("USAGE : %s [ t01 ]\n", argv[0]);
			
			exit(0);
		}
		
		if (strcmp(argv[1], "t01") == 0)
		{
			if (flag) BizTrans01();
		}
		else if (strcmp(argv[1], "t02") == 0)
		{
			if (flag) BizTrans02();
		}
		else
		{
			if (flag) printf("do nothing.....\n");
		}
	}
}

#endif
