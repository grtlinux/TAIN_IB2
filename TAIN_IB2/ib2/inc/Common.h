/*
 * PROGRAM ID   : Common.h
 * PROGRAM NAME : 공통 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : 공통적으로 사용하는 상수 및 변수 함수를 선언한다.
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 *   __FILE__, __FUNCTION__, __LINE__
 *
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*============================================================================*/
/*
 * include information
 */
/*----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <ctype.h>
#include <signal.h>
#include <time.h>
#include <pwd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <dirent.h>
#include <iconv.h>

#if defined _AIX
#include <procinfo.h>
#include <sys/procfs.h>
#endif

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define   BOOL          int
#define   TRUE          1
#define   FALSE         0

#ifndef MIN
	#define   MIN(x,y)      ((x) < (y) ? (x) : (y))
#endif

#ifndef MAX
	#define   MAX(x,y)      ((x) > (y) ? (x) : (y))
#endif

/*============================================================================*/
/*
 * include information
 */
/*----------------------------------------------------------------------------*/

#include <Structure.h>

#include <Var.h>
#include <Util.h>
#include <Flck.h>
#include <Time.h>
#include <Sig.h>
#include <Mmap.h>
#include <Passwd.h>

#include <Log.h>
#include <Err.h>
#include <Cfg.h>
#include <MCfg.h>
/* #include <FQ.h> */
#include <FQ2.h>
#include <FQ3.h>
#include <Seq.h>
#include <Sinf.h>
#include <SSock.h>
#include <CSock.h>
#include <MSock.h>
#include <IBnet.h>
#include <Poll.h>
#include <Line.h>
#include <X25.h>
#include <Biz.h>

#include <HWSeed.h>



#endif


