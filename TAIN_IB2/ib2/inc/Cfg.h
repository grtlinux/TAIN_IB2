/*
 * PROGRAM ID   : Cfg.h
 * PROGRAM NAME : 환경파일 처리 모듈 헤더
 * AUTHOR       : (주)ICA
 * COMMENT      : 환경파일 처리를 다루는 모듈 헤더
 * HISTORY      : 2013.07.24 강석. 최초작성
 *
 */

#ifndef _CFG_H_
#define _CFG_H_

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgMaster
 */
typedef struct _CFG_MST_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_MST;

#define   MAX_CFG_MST   100

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgBiz
 */
typedef struct _CFG_BIZ_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_BIZ;

#define   MAX_CFG_BIZ   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOfa
 */
typedef struct _CFG_OFA_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OFA;

#define   MAX_CFG_OFA   20

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOfs
 */
typedef struct _CFG_OFS_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OFS;

#define   MAX_CFG_OFS   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOir
 */
typedef struct _CFG_OIR_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OIR;

#define   MAX_CFG_OIR   10


/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOxaLine
 */
typedef struct _CFG_OXALINE_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OXALINE;

#define   MAX_CFG_OXALINE   20

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgBxa
 */
typedef struct _CFG_BXA_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_BXA;

#define   MAX_CFG_BXA   20

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOis
 */
typedef struct _CFG_OIS_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OIS;

#define   MAX_CFG_OIS   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOfr
 */
typedef struct _CFG_OFR_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OFR;

#define   MAX_CFG_OFR   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgBxaTr
 */
typedef struct _CFG_BXATR_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_BXATR;

#define   MAX_CFG_BXATR   20

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOxa
 */
typedef struct _CFG_OXA_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OXA;

#define   MAX_CFG_OXA   20

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOxaPoll
 */
typedef struct _CFG_OXAPOLL_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OXAPOLL;

#define   MAX_CFG_OXAPOLL   20





/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOfr
 */
typedef struct _CFG_OFR2_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OFR2;

#define   MAX_CFG_OFR2   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOfs
 */
typedef struct _CFG_OFS2_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OFS2;

#define   MAX_CFG_OFS2   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOir
 */
typedef struct _CFG_OIR2_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OIR2;

#define   MAX_CFG_OIR2   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgOis
 */
typedef struct _CFG_OIS2_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_OIS2;

#define   MAX_CFG_OIS2   10

/*----------------------------------------------------------------------------*/
/*
 * 구조체 : CfgPoll
 */
typedef struct _CFG_POLL_
{
	char    sKey      [128+1];
	char    sVal      [512+1];
} CFG_POLL;

#define   MAX_CFG_POLL   20




/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

extern CFG_MST      stCfgMst           [MAX_CFG_MST];   /* Master 정보 저장 */
extern int          iMaxCfgMst                      ;

/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

int CfgMst()                                  ;
char* CfgGetMst(char* sKey)                   ;
int CfgMaster()                               ;
char* CfgGetMaster(char* sKey)                ;
int CfgBiz()                                  ;
char* CfgGetBiz(char* sKey)                   ;
int CfgOfa()                                  ;
char* CfgGetOfa(char* sKey)                   ;
int CfgOfs()                                  ;
char* CfgGetOfs(char* sKey)                   ;
int CfgOir()                                  ;
char* CfgGetOir(char* sKey)                   ;
int CfgBxa()                                  ;
char* CfgGetBxa(char* sKey)                   ;
int CfgOis()                                  ;
char* CfgGetOis(char* sKey)                   ;
int CfgOfr()                                  ;
char* CfgGetOfr(char* sKey)                   ;
int CfgOxaLine()                              ;
char* CfgGetOxaLine(char* sKey)               ;
int CfgBxaTr(char* sSndRcv, char* sTrCode)    ;
char* CfgGetBxaTr(char* sKey)                 ;
int CfgOxa()                                  ;
char* CfgGetOxa(char* sKey)                   ;
int CfgOxaPoll()                              ;
char* CfgGetOxaPoll(char* sKey)               ;
int CfgOfr2()                                 ;
char* CfgGetOfr2(char* sKey)                  ;
int CfgOfs2()                                 ;
char* CfgGetOfs2(char* sKey)                  ;
int CfgOir2()                                 ;
char* CfgGetOir2(char* sKey)                  ;
int CfgOis2()                                 ;
char* CfgGetOis2(char* sKey)                  ;
int CfgPoll()                                 ;
char* CfgGetPoll(char* sKey)                  ;
int CfgAiaHost_OLD()                          ;
int CfgBizProperties_OLD()                    ;
int CfgBxaCom0000_OLD()                       ;
int CfgOxaLine_OLD()                          ;
int CfgOxaRout_OLD()                          ;
int CfgOxaPoll_OLD()                          ;




#endif
