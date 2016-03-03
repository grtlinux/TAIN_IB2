/*
 *  PROGRAM ID      : bxaktf1200.h
 *  PROGRAM NAME    : KFTC CMS 자료 송수신 MAC KEY HEADER FORMAT
 *  AUTHOR          : ICA
 *  HISTORY         : 2012.10.01 이현, 최초작성
 */

#ifndef _BXAKTF1200_H_
#define _BXAKTF1200_H_

#define  MAC_KEY1  "9715471621"
#define  MAC_KEY2  "5219287503"
#if 0
#define  MAC_KEY1  "7258911346" /*-  */
#define  MAC_KEY2  "6313998542" /*-  */
#endif

typedef struct _MAC_KEY_I_
{
    long lDataCnt;       /* 반복횟수: Call때 마다 1부터 1씩 증가 */
    char sDataVal[10+1]; /* Data:검증대상값 */
    char sIpData1[10+1]; /* 입력값1:반복회수 1이면 MacKey1, 그외에는 이전 Output1 */
    char sIpData2[10+1]; /* 입력값2:반복회수 1이면 MacKey2, 그외에는 이전 Output2 */
    char sMacKey1[10+1]; /* MacKey1         */
    char sMacKey2[10+1]; /* MacKey2         */
}   MAC_KEY_I;

typedef struct _MAC_KEY_O_
{
    char sOpData1[10+1]; /* Output1         */
    char sOpData2[10+1]; /* Output2         */
    char sMacValu[10+1]; /* MAC검증값       */
}   MAC_KEY_O;

#endif
