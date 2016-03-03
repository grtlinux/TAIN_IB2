/* pximapper.h
 * todo : 1. DOC : DOC의 orgname의 제거방안(org list load후 DOC load ?
          2. DOC : docSub, bizSub -> SUB *sub
          3. SUB : value 제거
          4. ORG : lsub 기관으로 확장
          5. FIELD : value 제거
 */
#include <sys/shm.h>
#include "../lib/inc/h.info.h"

#define SYSTEM_INFO     "/IBRIDGE/ram/cfg/system.cfg"
#define FIELD_INFO      "/IBRIDGE/ram/cfg/field.cfg"
#define COMP_INFO       "/IBRIDGE/ram/cfg/comp.cfg"
#define MESSAGE_INFO    "/IBRIDGE/ram/cfg/message.cfg"
#define ROUTING_INFO    "/IBRIDGE/ram/cfg/routing.cfg"
#define ORGSZ           10
#define BUFFSZ         (9999)
#define MAXTHREADSZ     500
#define MAXTHREADRETRY  10

#define GETDD(a) (((DD*)(a)->dd)->type == 'A' ? ((COMP*)(a)->dd)->dd : (DD*)(a)->dd)

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
    char    orgname[10+1];  /* out org name */
    SUB     sub;
    struct  SUBS *next;
};
typedef struct SUBS SUBS;
/*----------------------------------------------------------------------------*/
struct ADDR {
    char    ip[15+1];
    int     port;
    int     fd;
    int     iUse;
    char    serverClientType;
    char    connectType;
};
typedef struct ADDR ADDR;
/*----------------------------------------------------------------------------*/
struct DOC {
    char    orgname[10+1];  /* out org name */
    SUB     sub;
    struct  DOC *next;
};
typedef struct DOC DOC;
struct ROUT {
    char   *code;
    ADDR   *addr;
    struct  ROUT *next;
};
typedef struct ROUT ROUT;
/*----------------------------------------------------------------------------*/
struct ORG {
    char    name[10+1];
    char   *desc;
    SUB     isub;       /* input org get  */
    SUB     osub;       /* output org get */
    SUB     lsub;       /* length get */
    SUB     fsub;       /* fd save get */
    SUB     nsub;       /* msg number get */
    SUB     hsub;       /* header get */
    long    recvCnt;    /* 수신건수 */
    long    sendCnt;    /* 송신건수 */
    DOC     doc;        /* 전문종별/거래구분 get */
    SUBS    ssubs;      /* sending rout code get */
    ROUT    rout;       /* routing info */
};
typedef struct ORG ORG;
/*----------------------------------------------------------------------------*/
struct SOCK {
    ADDR    addr;
    int     listenfd;
    ORG    *orgs[100];
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
    MAPPING    *mapping;
};
typedef struct COMP COMP;

struct MESSAGE {
    char   *io;
    char   *oo;
    char    type;
    int     timeout;
    char   *doc;
    char   *desc;
    FIELD *field;
    MAPPING    *mapping;
};
typedef struct MESSAGE MESSAGE;
/*----------------------------------------------------------------------------*/
struct ThreadParam {
    int     fd;
    SOCK   *sock;
};
typedef struct ThreadParam ThreadParam;
/*----------------------------------------------------------------------------*/
struct Context {
    char    messagetype;
    ORG    *iorg;
    ORG    *oorg;
    int     sndmessagelength;
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
extern void	 LOGPUT( char *, int, int, int, char *, ... );

extern int   Ins_BizTimer(char *, int);
extern int   Del_BizTimer(char *);


void showOrg(ORG *org) {
    DOC  *doc  = &org->doc;
    ROUT *rout = &org->rout;
    SUBS *subs = &org->ssubs;
    SUB  *sub  = NULL;
    printf("ORG Info --------------------------------------------\n");
    if (org) {
        printf("name    [%s]\n", org->name);
        printf("desc    [%s]\n", org->desc);
        if (org->isub.length) printf("isub    [%d,%d,%s]\n", org->isub.offset, org->isub.length, org->isub.value);
        printf("osub    [%d,%d]\n", org->osub.offset, org->osub.length);
        printf("doc     ");
        for (; doc; doc = doc->next) {
            printf("[%s", doc->orgname);
            for (sub = &doc->sub; sub; sub = sub->next) {
                printf(":%d,%d", sub->offset, sub->length);
            }
            printf("] ");
        }
        printf("\n");
        printf("ssubs   \n");
        for (; subs; subs = subs->next) {
            printf("        [%s]:", subs->orgname);
            for (sub = &subs->sub; sub; sub = sub->next) {
                printf("[%d,%d] ", sub->offset, sub->length);
            }
            printf("\n");
        }
        printf("rout    ");
        for (; rout; rout = rout->next) {
            printf("[%s:%c:%c:%s:%d] ", rout->code, rout->addr->serverClientType, rout->addr->connectType, rout->addr->ip, rout->addr->port);
        }
        printf("\n");
    }
}
void showSock(SOCK *sock) {
    int     i;
    ORG    *org;
    printf("SOCK Info ===========================================\n");
    if (sock) {
        printf("port    [%d]\n", sock->addr.port);
        for (i = 0; sock->orgs[i]; i++) {
            printf("org%d    [%s]\n", i, sock->orgs[i]->name);
            showOrg(sock->orgs[i]);
        }
    }
}
void showDD(DD *dd) {
    printf("DD Info[name:%s, desc:%s, type:%c, len:%d.%d]\n", dd->name, dd->desc, dd->type, dd->length, dd->scale); 
}
void showFieldList(FIELD *fieldList, int iLvl) {
    int i;
    for (; fieldList; fieldList = fieldList->next) {
        for (i = 0; i < iLvl; i++) printf("  ");
        printf("  field [%s:%s]\n", GETDD(fieldList)->no, GETDD(fieldList)->name);
        if (GETDD(fieldList)->type == 'A') {
            showFieldList(((COMP*)fieldList->dd)->fieldList, iLvl+1);
        }
    }
}
void showMessage(MESSAGE *msg) {
    FIELD   *field;
    MAPPING *mapping;
    printf("MESSAGE Info =====================================\n");
    if (msg) {
        printf("io[%s] oo[%s] doc[%s] tyep[%c] desc[%s]\n", msg->io, msg->oo, msg->doc, msg->type, msg->desc);
        for (field = msg->field; field; field = field->next) {
            printf("  field [%s:%s]\n", GETDD(field)->no, GETDD(field)->name);
            if (GETDD(field)->type == 'A') {
                showFieldList(((COMP*)field->dd)->fieldList, 1);
            }
        }
        for (mapping = msg->mapping; mapping; mapping = mapping->next) {
            printf("  mapping [%s]\n", GETDD(mapping->target)->name);
        }
    }
}

void showMapping(MAPPING *mapping) {
    FIELD   *field;
    printf("MAPPING Info =====================================\n");
    if (mapping) {
        for (; mapping; mapping = mapping->next) {
            printf("  %c:  [%s] <- [%s] %d, %d\n", mapping->type, GETDD(mapping->target)->name, mapping->source?GETDD(mapping->source)->name:"", mapping->offset, mapping->length);
            if (mapping->type == 'A') {
                showMapping(((COMP*)mapping->target->dd)->mapping);
            }
        }
    }
}

/*----------------------------------------------------------------------------*
 *  HEADER REPOSITORY Start
 *----------------------------------------------------------------------------*/
typedef struct {
    char cUse;
    char sKey   [ 7+1];
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

int  SetHeader(char *, char *, int);
int  GetHeader(char *, char *, int);
int  InitHeaderRepository();
void showHeaderRepository();

int SetHeader(char *psKey, char *psHeader, int len) {
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
            LOGPUT( (char*)__func__, __LINE__, 9, 0, "<SetHeader>Header Expired DateTime[%s] Key,Seq[%s] Header[%s]\n", pHD->sDateTm, pHD->sKey, pHD->sHeader);
            pHD->cUse = 0; gpHR->iCnt--;
        }

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

        memset (pHD, 0x00, HEADER_DATA_SIZE);
        strncpy(pHD->sKey,    key,      sizeof(pHD->sKey));
        strncpy(pHD->sHeader, psHeader, MIN(gpHR->iDataSize, len));
        memcpy (pHD->sDateTm, sTime,    10);
        pHD->cUse = 1; gpHR->iCnt++;
        return 0;
    }
    LOGPUT( (char*)__func__, __LINE__, 9, 0, "<SetHeader>Header Repository 자원부족 [%d / %d]\n", gpHR->iCnt, gpHR->iTotCnt);
    return -1;
}
int GetHeader(char *psKey, char *psHeader, int len) {
    HEADER_DATA *pHD = NULL;
    int i = 0;
    
    if (gpHR->iCnt <= 0) {
        LOGPUT( (char*)__func__, __LINE__, 9, 0, "<GetHeader>Header Repository has No Data\n");
        return -1;
    }
    for (i = 0; i < gpHR->iTotCnt; i++) {
        pHD = (HEADER_DATA *)(gpHR->pHD + HEADER_DATA_SIZE * i);
        if (pHD->cUse && memcmp(psKey, pHD->sKey, sizeof(pHD->sKey)) == 0) {
            memcpy(psHeader, pHD->sHeader, MIN(gpHR->iDataSize, len));
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
