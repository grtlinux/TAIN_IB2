/*
 * PROGRAM ID   : Poll.h
 * PROGRAM NAME : Poll 변수 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : Poll 변수 헤더
 * HISTORY      : 2013.09.30 강석. 최초작성
 *
 */

#ifndef _POLL_H_
#define _POLL_H_

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
 * decleration
 */
/*----------------------------------------------------------------------------*/

char* PollSetMMDDHHMMSS(char* str)     ;
char* PollSetYYMMDDHHMMSS(char* str)   ;
char* PollSetYYYYMMDDHHMMSS(char* str) ;
int Poll2GetPollTime()                 ;
char* Poll2MsgSendReq()                ;
char* Poll2MsgSendRes()                ;
int Poll2CmpRecvReq(char* str)         ;
int Poll2CmpRecvRes(char* str)         ;
int PollGetPollTime()                  ;
char* PollMsgSendReq()                 ;
char* PollMsgSendRes()                 ;
int PollCmpRecvReq(char* str)          ;
int PollCmpRecvRes(char* str)          ;




#endif


