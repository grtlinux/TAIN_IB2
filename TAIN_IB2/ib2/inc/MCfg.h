/*
 * PROGRAM ID   : MCfg.h
 * PROGRAM NAME : Mapper ȯ������ ó�� ��� ���
 * AUTHOR       : (��)ICA
 * COMMENT      : Mapper ȯ������ ó���� �ٷ�� ��� ���
 * HISTORY      : 2013.07.24 ����. �����ۼ�
 *
 */

#ifndef _MCFG_H_
#define _MCFG_H_

/*============================================================================*/
/*
 * define constant
 */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MCfgFld
 */
typedef struct _MCFG_FLD_
{
	char    sFldNo      [  6+1];
	char    sFldName    [ 50+1];
	char    sFldType    [  1+1];
	char    sFldLen     [  5+1];
	char    sScale      [  1+1];
	char    sComment    [200+1];
	int     iFldLen            ;
	int     iScale             ;
} MCFG_FLD;

#define   MAX_MCFG_FLD   1000000

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MCfgFldCd
 */
typedef struct _MCFG_FLDCD_
{
	char    sFldNo      [  6+1];
	char    sCodNo      [  6+1];
	char    sGrpNm      [  3+1];
} MCFG_FLDCD;

#define   MAX_MCFG_FLDCD   1000

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MCfgCode
 */
typedef struct _MCFG_CODE_
{
	char    sFldNo      [  6+1];
	char    sCode       [ 10+1];
	char    sName       [100+1];
} MCFG_CODE;

#define   MAX_MCFG_CODE   10000

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MCfgComp
 */
typedef struct _MCFG_COMP_
{
	char    sFldNo      [  6+1];
	char    sSeq        [  3+1];
	char    sSubFNo     [  6+1];
} MCFG_COMP;

#define   MAX_MCFG_COMP   10000

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MCfgSys
 */
typedef struct _MCFG_SYS_
{
	char    sHint       [ 10+1];
	char    sInfo       [100+1];
} MCFG_SYS;

#define   MAX_MCFG_SYS   500

/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MsgDesc
 */
typedef struct _MSG_DESC_
{
	char    sSrcMsgId   [ 20+1];   /* �ҽ� MSGID            */
	char    sSrcTrCode  [ 50+1];   /* �ҽ� TRCODE           */
	char    sSrcReqRes  [ 20+1];   /* �ҽ� ���� ��û/����   */
	char    sTgtMsgId   [ 20+1];   /* Ÿ�� MSGID            */
	char    sTgtTrCode  [ 50+1];   /* Ÿ�� TRCODE           */
	char    sGubunCd    [ 10+1];   /* �����ڵ�              */
	char    sDesc       [200+1];   /* ����                  */
} MSG_DESC;

#define   MAX_MSG_DESC       1

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MsgFld
 */
typedef struct _MSG_FLD_
{
	char    sFldType    [  1+1];
	char    sCmpFldNo   [ 13+1];
	char    sFldNo      [  6+1];
	char    sFldLen     [  5+1];
	char    sScale      [  2+1];
	char    sFldName    [ 50+1];
	char    sComment    [ 50+1];
	int     iOffset            ;
	int     iFldLen            ;
	int     iScale             ;
	char    sValue      [999+1];
} MSG_FLD;

#define   MAX_MSG_FLD      100

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MsgMap
 */
typedef struct _MSG_MAP_
{
	char    sTCmpFldNo  [ 13+1];
	char    sTFldNo     [  6+1];

	char    sSCmpFldNo  [ 13+1];
	char    sSFldNo     [  6+1];
	
	char    sSSetType   [  1+1];
	char    sSValue     [ 50+1];
	int     sSPartOff          ;
	int     sSPartLen          ;

} MSG_MAP;

#define   MAX_MSG_MAP      100


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*
 * ����ü : MsgFld List
 */
typedef struct _MSG_FLDLST_
{
	char    sMsgId      [ 10+1];
	char    sTrCode     [ 20+1];
	
	char    sMsgName    [100+1];
	char    sMsgSrFlag  [ 20+1];
	char    sMsgNum     [  2+1];

	char    sFldInfo    [ 4096];
	
} MSG_FLDLST;

#define   MAX_MSG_FLDLST  10000


/*----------------------------------------------------------------------------*/
/*
 * ����ü : MsgMap List
 */
typedef struct _MSG_MAPLST_
{
	char    sSMsgId     [ 10+1];
	char    sSTrCode    [ 20+1];

	char    sTMsgId     [ 10+1];
	char    sTTrCode    [ 20+1];

	char    sMapInfo    [ 4096];

} MSG_MAPLST;

#define   MAX_MSG_MAPLST  10000










/*============================================================================*/
/*
 * global variables
 */
/*----------------------------------------------------------------------------*/

extern MCFG_FLD      stMCfgFld          [MAX_MCFG_FLD];   /* field.cfg ���� ����     */
extern int           iMaxMCfgFld                      ;

extern MCFG_FLDCD    stMCfgFldCd      [MAX_MCFG_FLDCD];   /* fieldcode.cfg ���� ���� */
extern int           iMaxMCfgFldCd                    ;

extern MCFG_CODE     stMCfgCode        [MAX_MCFG_CODE];   /* code.cfg ���� ����      */
extern int           iMaxMCfgCode                     ;

extern MCFG_COMP     stMCfgComp        [MAX_MCFG_COMP];   /* comp.cfg ���� ����      */
extern int           iMaxMCfgComp                     ;

extern MCFG_SYS      stMCfgSys          [MAX_MCFG_SYS];   /* system.cfg ���� ����      */
extern int           iMaxMCfgSys                      ;


/*============================================================================*/
/*
 * decleration
 */
/*----------------------------------------------------------------------------*/

int MCfgFld()                                       ;
int MCfgFldCheckSorted()                            ;
int MCfgFldSearch(char* sFieldNo)                   ;
int MCfgFldCd()                                     ;
int MCfgFldCdCheckSorted()                          ;
int MCfgFldCdSearch(char* sFieldNo)                 ;
int MCfgCode()                                      ;
int MCfgCodeCheckSorted()                           ;
int MCfgCodeSearch(char* sFieldNo, char* sCode)     ;
int MCfgCodeSearch2(char* sFieldNo)                 ;
int MCfgComp()                                      ;
int MCfgCompCheckSorted()                           ;
int MCfgCompSearch(char* sFieldNo, char* sCode)     ;
int MCfgCompSearch2(char* sFieldNo)                 ;
int MCfgSystem()                                    ;
int MCfgMsgSearch(char* sFld)                       ;
int MCfgMsgCompFld(char* sCompFld)                  ;
int MCfgMsgFormatFld(char* sFormatFld)              ;
int MCfgMsgMappingFld(char* sMappingFld)            ;
int MCfgMsgX(char* sFepId, char* sMsgCd)            ;
int MCfgMsgI(char* sFepId, char* sMsgCd)            ;
int MCfgRout()                                      ;





#endif
