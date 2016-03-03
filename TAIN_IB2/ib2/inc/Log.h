/*
 * PROGRAM ID   : Log.h
 * PROGRAM NAME : �α�ó�� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : �α�ó�� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
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

#define  LOG_LEVEL_DEBUG        0    /* Biz ���� ��û�ϴ� ��� ���� ��� */
#define  LOG_LEVEL_INFO         5    /* Biz �� ����~������ ����Ѵ�      */
#define  LOG_LEVEL_WARNING      8    /* Biz �� ���~������ ����Ѵ�      */
#define  LOG_LEVEL_ERROR        9    /* Biz �� ������ ����Ѵ�           */

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


