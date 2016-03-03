/*
 * PROGRAM ID   : Mmap.c
 * PROGRAM NAME : mmap ó���� ���� ����ϴ� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : mmap ó���� ���� ����ϴ� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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
 * ���� : Mmap���� ������ �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* MmapCreate(const char* sFileName, const int iSize)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	char* pMap;
	
	if (flag)
	{
		if (access(sFileName, R_OK) < 0)
		{
			/*
			 * file not exist....  ������ ����.
			 */
			if (iSize == 0) {
				/*
				 * ���ϵ� ���� ����� 0 �̸� NULL�� �����Ѵ�.
				 */
				return NULL;
			}
			
			FILE* fp = fopen(sFileName, "w+");
			/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
			chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);

			if (flag)
			{
				/*
				 * �ʱ��ڷḦ �����Ѵ�.
				 */
				fseek(fp, 0, SEEK_SET);
				int i;
				for (i=0; i < iSize; i++)
				{
					fwrite(".", 1, 1, fp);
				}
			}

			pMap = mmap(0, iSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileno(fp), 0);

			fclose(fp);
		}
		else
		{
			/*
			 * file exist... ������ �����Ѵ�.
			 */
			
			/*
			 * get the file size.
			 */
			struct stat fstat;
			
			if (lstat(sFileName, &fstat) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : lstat error..");
				return NULL;
			}
			
			/*
			 * compare the size and the file size.
			 */
			if (iSize != 0 && iSize != fstat.st_size)
			{
				LOG(_FL_, 0, 0, "ERROR : file size error.");
				return NULL;
			}
			
			/*
			 * mmap �����͸� ��´�.
			 */
			FILE* fp = fopen(sFileName, "r+");
			pMap = mmap(0, fstat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileno(fp), 0);
			fclose(fp);
		}
	}
	
	return pMap;
}

/*============================================================================*/
/*
 * ���� : Mmap�� �����Ѵ�.
 */
/*----------------------------------------------------------------------------*/
char* MmapOpen(const char* sFileName, const int iSize)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	char* pMap;
	
	if (flag)
	{
		if (access(sFileName, R_OK) < 0)
		{
			/*
			 * file not exist....  ������ ����.
			 */
			return NULL;
		}
		else
		{
			/*
			 * file exist... ������ �����Ѵ�.
			 */
			
			/*
			 * get the file size.
			 */
			struct stat fstat;
			
			if (lstat(sFileName, &fstat) < 0)
			{
				LOG(_FL_, 0, 0, "ERROR : lstat error..");
				return NULL;
			}
			
			/*
			 * compare the size and the file size.
			 */
			if (iSize != 0 && iSize != fstat.st_size)
			{
				LOG(_FL_, 0, 0, "ERROR : file size error.");
				return NULL;
			}
			
			/*
			 * mmap �����͸� ��´�.
			 */
			FILE* fp = fopen(sFileName, "r+");
			pMap = mmap(0, fstat.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fileno(fp), 0);
			fclose(fp);
		}
	}
	
	return pMap;
}

/*============================================================================*/
/*
 * ���� : Mmap -> File : sync�� �����.
 */
/*----------------------------------------------------------------------------*/
BOOL MmapSync(void* pMap, int iSize)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		/*
		 * MMAP sync�� �����.
		 */
		msync(pMap, iSize, MS_SYNC);
	}
	
	return TRUE;
}

/*============================================================================*/
/*
 * ���� : �������� ���� �����Ѵ�.
 *
 *    msync(pTest, iShmSize, MS_SYNC);
 *        MS_SYNC : Mem -> File ����ȭ�ϸ� ��ĥ������ wait
 *        MS_ASYNC : Mem -> File ����ȭ�ϸ� no wait
 *        MS_INVALIDATE : Mem <- File
 */
/*----------------------------------------------------------------------------*/
void MmapTest()
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (!flag)
	{
		int iShmSize = 100;
		char* pTest = NULL;
		
		if (flag)
		{
			/*
			 * MMAP�� �����Ѵ�.
			 */
			char* strFileName = "/hw01/ibridge/ib2/MMAP/FLOCKTEST";

			char strBuf[100+1];
			memset(strBuf, '.', iShmSize);

			FILE* fp = fopen(strFileName, "w+");
			fseek(fp, 0, SEEK_SET);
			fwrite(strBuf, 1, iShmSize, fp);

			pTest = mmap(0, iShmSize, PROT_READ|PROT_WRITE, MAP_SHARED, fileno(fp), 0);

			fclose(fp);

			/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
			chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);
		}
		
		/*
		 * MMAP ���� �����Ѵ�.
		 */
		LOG(_FL_, 0, 0, "Before [%.100s]", pTest);
		memset(pTest, '0', iShmSize);
		LOG(_FL_, 0, 0, "After  [%.100s]", pTest);

		if (!flag)
		{		
			/*
			 * MMAP sync�� �����.
			 */
			msync(pTest, iShmSize, MS_SYNC);
		}
	}
	
	if (flag)
	{
		char* sFileName = "/hw01/ibridge/ib2/MMAP/TEST";
		int iSize = 1024;
		
		if (flag)
		{
			char* pMap = MmapOpen(sFileName, iSize);
			
			strncpy(pMap+100, "Hello, world........", 20);
			
			MmapSync(pMap, iSize);
			
			LOG(_FL_, 0, 0, "[%*.*s]", iSize, iSize, pMap);
		}
	}
}
/*
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
*/
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
		MmapTest();
	}

}

#endif
