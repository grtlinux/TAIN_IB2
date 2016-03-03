/*
 *  PROGRAM ID      : bxa_config.h
 *  PROGRAM NAME    : 대외 배치 Config
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 */

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/time.h>
#if defined(_WIN32)
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
/*#include <sys/varargs.h>*/
#include <fcntl.h>
#include <ctype.h>

#ifndef _BXA_CONFIG_H_
#define _BXA_CONFIG_H_

#define  SPRINT           sprintf
#define  FPRINT           fprintf
#define  STNCPY(x, y)     strncpy( x, y, strlen(y) )
#define  STNCMP(x, y)     strncmp( x, y, strlen(y) )

#define  MEMSET(x, y)     memset( x, y, y == 0x00 ? sizeof(x) : sizeof(x)-1 )
#define  STINIT(x, y)     memset(&x, y, y == 0x00 ? sizeof(x) : sizeof(x)-1 )
#define  SETCPY(x, y, z)  memset( x, y, sizeof(x) );memcpy( x, z, strlen(z) )
#define  SETCAT(x, y, z)  memset( x, y, sizeof(x) );strcat( x, z );

#define  MAX_BUFF_SIZE    4200
#define  MAX_BLOCK_SEQ    100              /* 블럭당 최대 SEQ 갯수 */

#if defined(_WIN32)
#define  FILE_INFO_FILE  "./iib_ftp_client.cfg"
#define  RSLT_FILE_PATH  "./iib_ftp_client.log.%.8s"
#define  sleep(t)        Sleep(1000*(t))
#define  RTRIM(str)      { char *ps; for( ps = (str)+strlen(str)-1; ps >= (str) && *ps == ' '; ps-- ); ps[1] = 0x00; }
#else
#define  FILE_INFO_FILE  "/hw01/ibridge/ram/cfg/bxacom0000.cfg"
/* KANG 변경 : 2013.05.26 강석. 해당되는 폴더가 없어 재정의 하여 적용함. */
/* #define  RSLT_FILE_PATH  "/hw03/hwbiz/BK/WORK/BXACOM0000_%.8s" */
#define  RSLT_FILE_PATH  "/hw03/ibridge/DAT/BXACOM0000_%.8s"
#endif

#define  MAX_FILE_INFO    99
#define  MAX_OFTP_INFO    50
#define  MAX_MAKE_INFO    50

typedef struct
{
    char   sConType  [ 1+1];
    char   sSndHost  [15+1];
    char   sSndPort  [ 6+1];
    char   sLenType  [ 1+1];
    char   sLenOffset[ 1+1];
    char   sLenLength[ 1+1];
    char   sLenMinus [ 1+1];
    char   sBizCode  [ 3+1];
    char   sOrgCode  [10+1];
    char   sUserId   [20+1];
    char   sUserPw   [20+1];
    char   sIntrval  [ 3+1];

    char   sBizDate  [ 8+1];
    char   sChgTime  [ 6+1];

    int    iLenOffset;
    int    iLenLength;
    int    iLenMinus;
}   BAT_INFO;  BAT_INFO  gstBat;

#endif
