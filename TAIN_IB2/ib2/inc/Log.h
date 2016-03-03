/*
 * PROGRAM ID   : Log.h
 * PROGRAM NAME : 로그처리 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : 로그처리 헤더
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#ifndef _LOG_H_
#define _LOG_H_

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

#define _LOGINFO_   NULL, __FILE__, __FUNCTION__, __LINE__, 9
#define _FL_        NULL, __FILE__, __FUNCTION__, __LINE__

#define  LOG_LEVEL_DEBUG        0    /* Biz 에서 요청하는 모든 것을 출력 */
#define  LOG_LEVEL_INFO         5    /* Biz 의 정보~에러만 출력한다      */
#define  LOG_LEVEL_WARNING      8    /* Biz 의 경고~에러만 출력한다      */
#define  LOG_LEVEL_ERROR        9    /* Biz 의 에러만 출력한다           */

/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

extern void (*LOG) (const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... );

void SETLOG( void (*func) (const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... ) );
void LOG_DEFAULT(const char* sLogFile, const char* sFile, const char* sFunc, int iLine, int iLevel, int iCode, const char* pMsg, ... );

#endif


