/*
 * PROGRAM ID   : Mmap.c
 * PROGRAM NAME : mmap 처리를 위해 사용하는 모듈
 * AUTHOR       : (주)ICA
 * COMMENT      : mmap 처리를 위해 사용하는 모듈
 * HISTORY      : 2013.07.24 강석. 최초작성
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
 * 내용 : Mmap관련 파일을 생성한다.
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
			 * file not exist....  파일이 없다.
			 */
			if (iSize == 0) {
				/*
				 * 파일도 없고 사이즈도 0 이면 NULL을 리턴한다.
				 */
				return NULL;
			}
			
			FILE* fp = fopen(sFileName, "w+");
			/*chmod(strFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP | S_IROTH|S_IWOTH); */
			chmod(sFileName, S_IRUSR|S_IWUSR | S_IRGRP|S_IWGRP);

			if (flag)
			{
				/*
				 * 초기자료를 세팅한다.
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
			 * file exist... 파일이 존재한다.
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
			 * mmap 포인터를 얻는다.
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
 * 내용 : Mmap을 오픈한다.
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
			 * file not exist....  파일이 없다.
			 */
			return NULL;
		}
		else
		{
			/*
			 * file exist... 파일이 존재한다.
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
			 * mmap 포인터를 얻는다.
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
 * 내용 : Mmap -> File : sync를 맞춘다.
 */
/*----------------------------------------------------------------------------*/
BOOL MmapSync(void* pMap, int iSize)
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;
	
	if (flag)
	{
		/*
		 * MMAP sync를 맞춘다.
		 */
		msync(pMap, iSize, MS_SYNC);
	}
	
	return TRUE;
}

/*============================================================================*/
/*
 * 내용 : 순차적인 값을 리턴한다.
 *
 *    msync(pTest, iShmSize, MS_SYNC);
 *        MS_SYNC : Mem -> File 동기화하며 마칠때까지 wait
 *        MS_ASYNC : Mem -> File 동기화하며 no wait
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
			 * MMAP을 생성한다.
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
		 * MMAP 값을 세팅한다.
		 */
		LOG(_FL_, 0, 0, "Before [%.100s]", pTest);
		memset(pTest, '0', iShmSize);
		LOG(_FL_, 0, 0, "After  [%.100s]", pTest);

		if (!flag)
		{		
			/*
			 * MMAP sync를 맞춘다.
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
		MmapTest();
	}

}

#endif
