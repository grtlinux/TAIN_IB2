/* pximapper.h
 * todo : 1. DOC : DOC의 orgname의 제거방안(org list load후 DOC load ?
          2. DOC : docSub, bizSub -> SUB *sub
          3. SUB : value 제거
          4. ORG : lsub 기관으로 확장
          5. FIELD : value 제거
 */
#include <sys/shm.h>

#include <LibConstDefine.h>
#include <LibExtVarDefine.h>

#include "../lib/inc/h.info.h"

#define SOCK_INFO       "/ram/cfg/sock.cfg"
#define SYSTEM_INFO     "/ram/cfg/msock.cfg"
#define FIELD_INFO      "/ram/cfg/field.cfg"
#define COMP_INFO       "/ram/cfg/comp.cfg"
#define CMAP_INFO       "/ram/cfg/codemapping.cfg"
#define MSGANAL_INFO    "/ram/cfg/msganal.cfg"
#define MESSAGE_INFO    "/ram/cfg/msgorg.cfg"
#define MAPPING_INFO    "/ram/cfg/mapping.cfg"
#define ROUTING_INFO    "/ram/cfg/mrout.cfg"
#define REER_INFO       "/ram/cfg/mreer.cfg"
#define BUFFSZ         (1024*10)
#define MAXTHREADSZ     512
#define MAXTHREADRETRY  10

#define GETDD(a) (((DD*)(a)->dd)->type == 'R' || ((DD*)(a)->dd)->type == 'C' ? ((COMP*)(a)->dd)->dd : (DD*)(a)->dd)

/*----------------------------------------------------------------------------*/
struct SUB {
    int     offset;
    int     length;
    int     temp;
    char   *value;
    struct  SUB *next;
};
typedef struct SUB SUB;
struct SUBS {
    char   *key;
    SUB     subs;
    struct  SUBS *next;
};
typedef struct SUBS SUBS;
/*----------------------------------------------------------------------------*/
struct MSG_ANAL {
    char *name;
    SUB   keys;
    SUBS  subs;
    char  messageLoaded;
    char  mappingLoaded;
};
typedef struct MSG_ANAL MSG_ANAL;
/*----------------------------------------------------------------------------*/
struct REER_ACTION {
    char  *key;
    SUB    sub;
    struct REER_ACTION *next;
};
typedef struct REER_ACTION REER_ACTION;
struct REER {
    SUB    keys;
    void  *sock;    /* SOCK */
    REER_ACTION actions;
};
typedef struct REER REER;
/*----------------------------------------------------------------------------*/
struct ROUT {
    char   *val[3];
    void   *sock;   /* SOCK */
    struct  ROUT *next;
};
typedef struct ROUT ROUT;
struct ROUTS {
    SUB   keys[3];
    ROUT  routs;
};
typedef struct ROUTS ROUTS;
struct SOCK {
    char     *adapter;
    char     *socket;
    char     *rout;
    SUB       hsub;       /* header get */
    ROUTS    *routs;
    REER     *reer;
    MSG_ANAL *msgAnal;
    char     *ip;
    int       port;
    int       listenfd;
    int       fd;
    int       use;
    char      connectType;
    char      closeType;
    char      lenType;
    int       lenOffset;
    int       lenLength;
    int       lenPosition;
    char      lenTake;
  /*int       noOffset;
    int       noLength;*/
};
typedef struct SOCK SOCK;
/*----------------------------------------------------------------------------*/
struct DD {
    char   type;            /* Str, NUm, Array */
    char  *no;
    char  *name;
    int    length;
    int    scale;
    char  *desc;
};
typedef struct DD DD;

struct FIELD {
    void   *dd;             /* DD, COMP */
    struct  FIELD *next;
};
typedef struct FIELD FIELD;

struct CMAP {
    char   *no;
    char   *name;
    char   *desc;
    int     targetLength;
    int     sourceLength;
    int     size;
    char *(*map)[2];
};
typedef struct CMAP CMAP;
/*----------------------------------------------------------------------------*/
struct MAPPING {
    FIELD  *source;
    FIELD  *target;
    char    type;
    char   *value;
    int     offset;
    int     length;
    struct  MAPPING *next;
};
typedef struct MAPPING MAPPING;

struct COMP {
    char        type;      /* A:Array */
    DD         *dd;
    int         iRepeatCnt;
    FIELD      *countField;
    FIELD      *fieldList;
    MAPPING    *mappings;
};
typedef struct COMP COMP;

struct MESSAGE {
    char   *msgClas;
    char   *doc;
    char   *ref;
    char    type;
    char   *desc;
    FIELD  *fields;
};
typedef struct MESSAGE MESSAGE;
struct MESSAGE_MAPPING {
    MESSAGE *imsg;
    MESSAGE *omsg;
    int      timeout;
    char     rePrcs;
    MAPPING *mappings;
    char    *resMsgCd;
    struct MESSAGE_MAPPING *reqMsgMapping;
};
typedef struct MESSAGE_MAPPING MESSAGE_MAPPING;
/*----------------------------------------------------------------------------*/
struct ThreadParam {
    int     fd;
    SOCK   *sock;
};
typedef struct ThreadParam ThreadParam;
/*----------------------------------------------------------------------------*/
struct Context {
    SOCK *recvSock;
    SOCK *sendSock;
    ROUT *rout;
    int   sendLength;
};
typedef struct Context Context;
/*----------------------------------------------------------------------------*/

HTable *createHTable(int colCount, void (*freeHandler)(void *value));
void freeHTable(HTable *table);
Row *getRowByIndex(HTable *table, int index);
int getRowIndex(HTable *table, int colIndex, const char *key);
int getRowIndexWithSortedData(HTable *table, int colIndex, const char *key);
Row *getRow(HTable *table, int keyIndex, const char *key);
Row *getRowWithSortedData(HTable *table, int keyIndex, const char *key);
int getRowIndexWhere(HTable *table, const void *v1,...);
Row *getRowWhere(HTable *table, const void *v1,...);
int delRowIndex(HTable *table, int index);
int delRow(HTable *table, int colIndex, const void *key);
Row *addRow(HTable *table, Row *row);
Row *addRowV(HTable *table, const void *v1,...);
void *getColumnIndex(HTable *table, int colIndex, int rowIndex);
void *getColumn(HTable *table, int colIndex, int keyIndex, const char *key);
void *getColumnWithSortedData(HTable *table, int colIndex, int keyIndex, const char *key);
Row *insRow(HTable *table, int index, Row *row);
Row *insRowV(HTable *table, int index, const void *v1,...);
static int compareRow(const void *p1, const void *p2);
static int compareRowSearch(const void *key, const void *p);
int sortHTable(HTable *table, int colIndex);
Row *searchHTable(HTable *table, int colIndex, const char *key);
void showRow(HTable *table, Row *row);
void showHTable(HTable *table);

int info_fgets(FILE *fp, char **buff);
HInfo *createInfo(char *fname, char *key, int len, const int fields,...);
void freeInfo(HInfo *info);
HTable *getTable(HInfo *info, const void *v1,...);

extern void  LibGetCurrTime(char *str, int FormatFlag, int NullFlag);
extern void  LibInitProc(char *pProcName);
extern char *LibGetStrProperties(char *sName);
extern void  LibSetExitFunction(void *ptrExitFunc);
extern int   LibHexstr2Int(char *str, int Length);
extern int   LibAsc2Int(char *str, int Length);
extern void  LibHandleSignal();
extern void  LibExitProc();
extern void  SAFPUT( char *psMsg, int iMsgLen );

extern int   Ins_BizTimer( char *pMsg, int iMsgLen, char *psMsgN, int iMsgNLen, int iTime );
extern int   Del_BizTimer( char *psMsgN, int iMsgNLen );

void showSock(SOCK *sock) {
    int     i;
    ROUT   *rout;
    SUB    *sub;
    printf("SOCK Info ===========================================\n");
    if (sock) {
        printf("adapter-socket[%s-%s] ip[%s] port[%d]\n", sock->adapter, sock->socket, sock->ip, sock->port);
        printf("connect[%c] close[%c] len[%c:%d:%d:%d:%c]\n", sock->connectType, sock->closeType, sock->lenType, sock->lenOffset, sock->lenLength, sock->lenPosition, sock->lenTake);
        printf("MSG ANAL[%s]\n", sock->msgAnal->name);
        printf("ROUT[%s]\n", sock->rout);
        for (i = 0; i < 3 && sock->routs && sock->routs->keys[i].length; i++) {
            printf(" key%d[%d,%d]", i, sock->routs->keys[i].offset, sock->routs->keys[i].length);
        }
        printf("\n");
        for (rout = sock->routs ? &sock->routs->routs : NULL; rout && rout->sock; rout = rout->next) {
            printf(" [%s][%s][%s] [%s-%s]\n", rout->val[0], rout->val[1], rout->val[2], ((SOCK*)rout->sock)->adapter, ((SOCK*)rout->sock)->socket);
        }
    }
    printf("SOCK Info -------------------------------------------\n");
}
void showDD(DD *dd) {
    printf("DD Info[name:%s, desc:%s, type:%c, len:%d.%d]\n", dd->name, dd->desc, dd->type, dd->length, dd->scale); 
}
void showMessage(MESSAGE *msg) {
    FIELD   *fields;
    printf("MESSAGE Info =====================================\n");
    if (msg) {
        printf("clas[%s] doc[%s] tyep[%c] desc[%s]\n", msg->msgClas, msg->doc, msg->type, msg->desc);
        for (fields = msg->fields; fields; fields = fields->next) {
            printf("  field [%s:%s]\n", GETDD(fields)->no, GETDD(fields)->name);
        }
    }
}

void showMapping(MAPPING *mapping) {
    FIELD   *fields;
    printf("MAPPING Info =====================================\n");
    if (mapping) {
        for (; mapping; mapping = mapping->next) {
            printf("  %c:  [%s] <- [%s] %d, %d\n", mapping->type, GETDD(mapping->target)->name, mapping->source?GETDD(mapping->source)->name:"", mapping->offset, mapping->length);
        }
    }
}

/*----------------------------------------------------------------------------*
 *  HEADER REPOSITORY Start
 *----------------------------------------------------------------------------*/
typedef struct {
    char cUse;
    char sKey   [50+1];
    char sDateTm[10+1];
    char sHeader[ 1+1];
} HEADER_DATA;
#define HEADER_DATA_SIZE (sizeof(HEADER_DATA) + gpHR->iDataSize - 1)
typedef struct {
    int   iCnt;
    int   iDataSize;
    int   iTotCnt;
    int   iTimeOut;
    long  lLastSeq;
    char *pHD;
} HEADER_REPOSITORY;

HEADER_REPOSITORY *gpHR;

int  SetHeader(char *, char *, char *, char *, int, char *, int);
int  GetHeader(char *, char *, char *, char *, int, char *, int);
int  InitHeaderRepository();
void showHeaderRepository();

int SetHeader(char *adapter, char *socket, char *org, char *psKey, int keyLen, char *psHeader, int len) {
    HEADER_DATA *pHD = NULL;
    int          iClearFlag = 0, i;
    char         sTime[14+1] = {0x00,};
    char         key[7+1];
    
    if (gpHR->iCnt >= gpHR->iTotCnt) iClearFlag = 1;
    LibGetCurrTime(sTime, 0, 0);
    for (i = 0; i < gpHR->iTotCnt; i++) {
        pHD = (HEADER_DATA *)(gpHR->pHD + HEADER_DATA_SIZE * i);
        if (pHD->cUse) {
            if (!iClearFlag || LibAsc2Int(sTime, 10) - LibAsc2Int(pHD->sDateTm, 10) < gpHR->iTimeOut) continue;
            LOGPUT( (char*)__func__, __LINE__, 9, 0, "Header Expired DateTime[%s] Key,Seq[%s] Header[%s]\n", pHD->sDateTm, pHD->sKey, pHD->sHeader);
            pHD->cUse = 0; gpHR->iCnt--;
        }
#if 0
        gpHR->lLastSeq++;
        if (gpHR->lLastSeq == 10000000L) gpHR->lLastSeq = 0L;
        sprintf(key, "%07ldl", ++(gpHR->lLastSeq));
        if (memcmp(key+1, "000000", 6) == 0) {
            HEADER_DATA *hd;
            for (i = 0; i < gpHR->iTotCnt; i++) {
                hd = (HEADER_DATA *)(gpHR->pHD + HEADER_DATA_SIZE * i);
                if (hd->cUse && hd->sKey[0] == key[0]) {
                    hd->cUse = 0; gpHR->iCnt--;
                }
            }
        }
#endif
        memset (pHD, 0x00, HEADER_DATA_SIZE);
        sprintf(pHD->sKey, "%s%s%s%.*s", adapter, socket, org, keyLen, psKey);
        strncpy(pHD->sHeader, psHeader, MIN(gpHR->iDataSize, len));
        memcpy (pHD->sDateTm, sTime,    10);
        pHD->cUse = 1; gpHR->iCnt++;
        return 0;
    }
    LOGPUT( (char*)__func__, __LINE__, 9, 0, "Header Repository 자원부족 [%d / %d]\n", gpHR->iCnt, gpHR->iTotCnt);
    return -1;
}
int GetHeader(char *adapter, char *socket, char *org, char *psKey, int keyLen, char *psHeader, int len) {
    HEADER_DATA *pHD = NULL;
    char         sKey[50];
    int i = 0;
    
    if (gpHR->iCnt <= 0) {
        LOGPUT( (char*)__func__, __LINE__, 9, 0, "Header Repository has No Data\n");
        return -1;
    }
    sprintf(sKey, "%s%s%s%.*s", adapter, socket, org, keyLen, psKey);
    for (i = 0; i < gpHR->iTotCnt; i++) {
        pHD = (HEADER_DATA *)(gpHR->pHD + HEADER_DATA_SIZE * i);
        if (pHD->cUse && strcmp(sKey, pHD->sKey) == 0) {
            memcpy(psHeader, pHD->sHeader, MIN(gpHR->iDataSize, len));
            pHD->cUse = 0; gpHR->iCnt--;
            return 0;
        }
    }
    return -1;
}
int InitHeaderRepository() {
    int shmid;
    int iKey      = LibHexstr2Int(LibGetStrProperties("HEADER_REPOSITORY_SHMKEY"  ), 0);
    int iDataSize = LibAsc2Int   (LibGetStrProperties("HEADER_REPOSITORY_DATASIZE"), 0);
    int iTotCnt   = LibAsc2Int   (LibGetStrProperties("HEADER_REPOSITORY_TOTALCNT"), 0);
    int iTimeOut  = LibAsc2Int   (LibGetStrProperties("HEADER_REPOSITORY_TIMEOUT" ), 0);

    /* 공유메모리 배타적 생성 */
    if ((shmid = shmget(iKey, sizeof(HEADER_REPOSITORY) + (sizeof(HEADER_DATA) + iDataSize - 1) * iTotCnt, IPC_CREAT|IPC_EXCL|0666)) != -1) {
        if ((gpHR = (HEADER_REPOSITORY *)shmat(shmid, 0, 0)) == NULL) {
            printf(">> Shared memory attach Fail.\n");
            return -1;
        }
        gpHR->iDataSize = iDataSize;
        gpHR->iTotCnt   = iTotCnt;
        gpHR->iTimeOut  = iTimeOut;
        gpHR->pHD     = (char*)(gpHR + 1);
    /* 공유메모리 존재 */
    } else {
        printf(">> Shared memory segment exists.\n");

        if ((shmid = shmget(iKey, sizeof(HEADER_REPOSITORY) + sizeof(HEADER_DATA) * iTotCnt, 0)) == -1) {
            printf(">> Shared memory get Fail.\n");
            return -1;
        }
/*printf("%d\n",shmctl(shmid, IPC_RMID, 0));
   if (1)return -1;
*/
        if ((gpHR = (HEADER_REPOSITORY *)shmat(shmid, 0, 0)) == NULL) {
            printf(">> Shared memory attach Fail.\n");
            return -1;
        }
        showHeaderRepository(gpHR);
    }
    LOGPUT( (char*)__func__, __LINE__, 9, 0, "Header Respository is Initiated[%d:%d]\n", gpHR->iCnt, iTotCnt);
    printf("Header Respository is Initiated[%d:%d]\n", gpHR->iCnt, iTotCnt) ;

    return 0;
}
void showHeaderRepository() {
    HEADER_DATA *pHD = NULL;
    int  i, j;
    char sTime[14+1] = {0x00,};

    printf("  ========= HEAER REPOSITORY S ===========\n");
    LibGetCurrTime(sTime, 0, 0);
    for (i = 0, j = 0; gpHR->iTotCnt && i < gpHR->iTotCnt; i++) {
        pHD = (HEADER_DATA*)(gpHR->pHD + HEADER_DATA_SIZE * i);
        if (!pHD->cUse) continue;
        printf("  %4d:%4d:", j++, i);
        printf("[%s:%d][%s][%s]\n", pHD->sDateTm, LibAsc2Int(sTime, 10) - LibAsc2Int(pHD->sDateTm, 10), pHD->sKey, pHD->sHeader);
    }
    printf("  ========= HEAER REPOSITORY X ===========\n");
}
/*----------------------------------------------------------------------------*
 *  HEADER REPOSITORY End
 *----------------------------------------------------------------------------*/
