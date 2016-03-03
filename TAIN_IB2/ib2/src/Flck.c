/*
 * PROGRAM ID   : Flck.c
 * PROGRAM NAME : 파일잠금에 사용되는 함수들 정의
 * AUTHOR       : (주)ICA
 * COMMENT      : 파일잠금에 사용하는 함수들을 정의한다.
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
 * 내용 : fcntl 함수를 호출한다.
 */
/*----------------------------------------------------------------------------*/
int FlckFcntl( FLCK_FCNTL* flck )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret = 0;

	if (flag) {

		struct flock stFlock;

		stFlock.l_type   = flck->l_type   ;
		stFlock.l_whence = flck->l_whence ;
		stFlock.l_start  = flck->l_start  ;
		stFlock.l_len    = flck->l_len    ;

		/*
		 * On error, -1 is returned, and <errno> is set appropriately;
		 */
		ret = fcntl(flck->fd, flck->cmd, &stFlock);
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : 파일 잠금을 실행한다.
 */
/*----------------------------------------------------------------------------*/
int FlckLock( int fd, int start, int len )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret = 0;

	if (flag) {

		FLCK_FCNTL flck;

		flck.fd        = fd       ;
		flck.cmd       = F_SETLK  ;
		flck.l_type    = F_WRLCK  ;
		flck.l_whence  = SEEK_SET ;
		flck.l_start   = start    ;
		flck.l_len     = len      ;

		ret = FlckFcntl( &flck );
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : 파일 잠금을 실행한다. 계속 될 때까지
 */
/*----------------------------------------------------------------------------*/
int FlckLockWait( int fd, int start, int len )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int i;
	int ret = 0;

	if (flag) {

		for (i=0; i < 1000; i++)
		{
			FLCK_FCNTL flck;

			flck.fd        = fd       ;
			flck.cmd       = F_SETLKW ;
			flck.l_type    = F_WRLCK  ;
			flck.l_whence  = SEEK_SET ;
			flck.l_start   = start    ;
			flck.l_len     = len      ;

			ret = FlckFcntl( &flck );
			if (ret == 0)
				break;

			if (errno != EINTR)
				return -59;
			
			if (flag) LOG(_FL_, 0, 0, "Try to lock....[%05d]", i);
			
			/*usleep(1000000);*/
			sleep(1);
		}
	}

	return ret;
}

/*============================================================================*/
/*
 * 내용 : 파일 잠금을 해제한다.
 */
/*----------------------------------------------------------------------------*/
int FlckUnlock( int fd )
/*----------------------------------------------------------------------------*/
{
	BOOL flag = TRUE;

	int ret = 0;

	if (flag) {

		FLCK_FCNTL flck;

		flck.fd        = fd       ;
		flck.cmd       = F_SETLK  ;
		flck.l_type    = F_UNLCK  ;
		flck.l_whence  = 0        ;
		flck.l_start   = 0        ;
		flck.l_len     = 0        ;

		ret = FlckFcntl( &flck );
	}

	return ret;
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
		/* file lock test */
		FILE* fp = NULL;
		int ret;
		int i;

		fp = fopen("/hw01/ibridge/ib2/src/FLOCKTEST", "w+");

		ret = FlckLockWait(fileno(fp), 0, 10);
		LOG(_FL_, 0, 0, "%s> ret=%d", __FILE__, ret);
		if (ret < 0) {
			LOG(_FL_, 0, 0, "File couldn't be locked....");
			exit(-1);
		}
		
		for (i=0; i< 10; i++) {
			LOG(_FL_, 0, 0, "job-%03d...", i);
			sleep(1);
		}
		
		ret = FlckUnlock(fileno(fp));
		LOG(_FL_, 0, 0, "%s> ret=%d", __FILE__, ret);
		
		fclose(fp);
	}
}

#endif
