##--------------------------------------------------------
#
#  Makefile
#
##--------------------------------------------------------

HOME_DIR= /hw01/ibridge/ib2

BIN_DIR =${HOME_DIR}/bin
CFG_DIR =${HOME_DIR}/cfg
DAT_DIR =${HOME_DIR}/dat
INC_DIR =${HOME_DIR}/inc
LIB_DIR =${HOME_DIR}/lib
OBJ_DIR =${HOME_DIR}/obj
SHL_DIR =${HOME_DIR}/shl
SRC_DIR =${HOME_DIR}/src
TMP_DIR =${HOME_DIR}/tmp

##--------------------------------------------------------

IBCOM_A    = ${LIB_DIR}/libIBcom.a

##--------------------------------------------------------

H_LIB      =-L${LIB_DIR} -lHLibs
COC_LIB    =-L${LIB_DIR} -lCocLibs
IBCOM_LIB  =-L${LIB_DIR} -lIBcom

#LIBS       =${H_LIB} ${COC_LIB}
LIBS       =${IBCOM_LIB}

INCLUDE    =-I. -I${INC_DIR}

XCC_LIB    =-m64 -D_LINUX -D_REENTRANT \
		   -I/hw01/ibridge/lib/XecureConnect2.6/include \
		   -L/hw01/ibridge/lib/XecureConnect2.6/lib \
		   -lxcc260

##--------------------------------------------------------

PC =proc
MV =mv
RM =rm -f
RL =ranlib
LS =ls -al

# Linux
CC =gcc -g -D_REENTRANT
AR =ar -ru
# UNIX
#CC =cc -g -q64
#AR =ar -X64 -ru

TAR_DAY=`/bin/date +%Y%m%d_%H%M`
TODAY=`/bin/date +%Y%m%d`

##--------------------------------------------------------

all : tar clear obj arch exe test

##--------------------------------------------------------

tar :
	@echo "[44m< $@ >[0m"
	${RM} bak/src_${TODAY}*
	tar -cvf bak/src_${TAR_DAY}.tar *.c
	tar -rvf bak/src_${TAR_DAY}.tar Makefile
	gzip bak/src_${TAR_DAY}.tar

clear :
	@echo "[44m< $@ >[0m"
	${RM} ${IBCOM_A}
	${RM} core.*

arch :
	@echo "[44m< $@ >[0m"
	${RL} ${IBCOM_A}
	${LS} ${IBCOM_A}

##--------------------------------------------------------

obj :           \
	Var.o       \
	Util.o      \
	Flck.o      \
	Time.o      \
	Sig.o       \
	Log.o       \
	Err.o       \
	Cfg.o       \
	MCfg.o      \
	Mmap.o      \
	FQ2.o       \
	FQ3.o       \
	Seq.o       \
	Sinf.o      \
	Sock.o      \
	SSock.o     \
	CSock.o     \
	Exec.o      \
	Passwd.o    \
	Rand.o      \
	MSock.o     \
	Poll.o      \
	Line.o      \
	X25.o       \
	Ums.o       \
	IBnet.o     \
	HWSeed.o    \


test :          \
	Var         \
	Util        \
	Flck        \
	Time        \
	Sig         \
	Log         \
	Err         \
	Cfg         \
	MCfg        \
	Mmap        \
	FQ2         \
	FQ3         \
	Seq         \
	Sinf        \
	Sock        \
	SSock       \
	CSock       \
	Exec        \
	Passwd      \
	Rand        \
	MSock       \
	Poll        \
	Line        \
	X25         \
	Biz         \
	Ums         \
	IBnet       \
	HWSeed      \
	KISeed      \




#   bfscom      \
#   bircom      \
#   biscom      \
#   bfrcom      \
#   bfscom2     \
#   bircom2     \
#   biscom2     \
#   bfrcom2     \

exe :           \
	fepstat2    \
	bfscom2     \
	bircom2     \
	biscom2     \
	bfrcom2     \
	bxrltc      \
	bxsshc      \
	bxrshc      \
	bxskft1     \
	bxrkft1     \
	bxskft2     \
	bxrkft2     \
	bxskft3     \
	bxrkft3     \
	bxsksn		\
	bxrksn		\
	bxrctc      \
	bxsssn      \
	bxrssn      \
	bxshdc      \
	bxrhdc      \
	maachk2		\
	ofscom2     \
	ofrcom2     \
	ofrkft2     \
	oiscom2     \
	oircom2     \
	oxscom2     \
	oxrcom2     \
	maachk2     \
	oxatat2     \
	oxameg2     \
	oxsmeg2     \
	oxrmeg2     \
	ps_man      \
	log_del     \
	log_sec     \
	log_zip     \
	ofrcho2     \
	ofscho2     \
	oircho2     \
	oischo2     \
	oxrcho2     \
	oxscho2     \
	oxakft2     \
	oxascb2     \
	oxrdhi2     \
	oxsdhi2     \
	oiscom3     \



##--------------------------------------------------------

ofscom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oircom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxscom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxatat3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxrcom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oiscom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofrcom3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofscom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oircom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oiscom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxscom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxrcom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofrcom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofrkft2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxatat2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxameg2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxsmeg2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxrmeg2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxrdhi2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxsdhi2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofscho2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oircho2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oischo2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

ofrcho2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxscho2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxrcho2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxakft2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bfscom :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bircom :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

biscom :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bfrcom :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bfscom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bircom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

biscom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bfrcom2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrltc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxsshc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrshc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxskft1 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrkft1 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxskft2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrkft2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxskft3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrkft3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxskft4 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxsksn :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@	 $(INCLUDE)	-lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrksn :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@	 $(INCLUDE)	-lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrctc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxsssn :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrssn :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxshdc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

bxrhdc :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

maachk2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@	 $(INCLUDE)	-lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

oxascb2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS} $(XCC_LIB)
	${MV} $@   ${BIN_DIR};

ps_man :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

log_del :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

log_sec :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS} -I${SEC_HOME}/include -L${SEC_HOME}/lib -lscpdb_agent
	${MV} $@   ${BIN_DIR};

log_zip :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

fepstat2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_DEBFLG=1 -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};



##--------------------------------------------------------

Var.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Var :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Util.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Util :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Flck.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Flck :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Time.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Time :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Sig.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Sig :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Mmap.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Mmap :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Log.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Log :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Err.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Err :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Cfg.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Cfg :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

MCfg.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

MCfg :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

FQ2.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

FQ2 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

FQ3.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

FQ3 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Seq.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Seq :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Sinf.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Sinf :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Sock.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Sock :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

SSock.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

SSock :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

CSock.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

CSock :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

MSock.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

MSock :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Exec.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Exec :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Passwd.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Passwd :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Rand.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Rand :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Poll.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Poll :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Line.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Line :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

X25.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

X25 :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Biz.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Biz :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

IBnet.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

IBnet :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

Ums.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

Ums :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

HWSeed.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl -DBIG_ENDIAN
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

HWSeed :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS} -DBIG_ENDIAN
	${MV} $@   ${BIN_DIR};

KISeed.o :
	@echo "[41m< $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl -DBIG_ENDIAN
	${AR} ${IBCOM_A} $@
	${MV} $@ ${OBJ_DIR};

KISeed :
	@echo "[41m< $@ >[0m"
	# ${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS} -DBIG_ENDIAN
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl -DBIG_ENDIAN
	${MV} $@   ${BIN_DIR};
##--------------------------------------------------------


##--------------------------------------------------------
# EXE

cli :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@    $(INCLUDE) -lnsl
	${MV} $@   ${BIN_DIR};

svr :
	@echo "[41m< $@ >[0m"
	${CC} -D_EXE $@.c -o $@    $(INCLUDE) -lnsl
	${MV} $@   ${BIN_DIR};

############################################################
