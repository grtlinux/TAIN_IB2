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

HWSEED    = ${LIB_DIR}/libHWSeed.a

##--------------------------------------------------------

HWSEED_LIB =-L${LIB_DIR} -lHWSeed

LIBS       =${HWSEED_LIB}

INCLUDE	   =-I.

##--------------------------------------------------------

PC =proc
MV =mv
RM =rm -f
RL =ranlib
LS =ls -al

CC =gcc -g -D_REENTRANT
AR =ar -ru
#CC =cc -g -q64
#AR =ar -X64 -ru

##--------------------------------------------------------

all : clear obj arch test

##--------------------------------------------------------

clear :
	@echo "[44m< clear $@ >[0m"
	${RM} ${HWSEED}
	${RM} core.*

arch :
	@echo "[44m< arch $@ >[0m"
	${RL} ${HWSEED}
	${LS} ${HWSEED}

##--------------------------------------------------------

obj :           \
	Seedx.o     \
	encrypt.o   \

test :          \
	encrypt     \
	kang        \

##--------------------------------------------------------

Seedx.o :
	@echo "[41m< obj $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -DBIG_ENDIAN -lnsl
	${AR} ${HWSEED} $@
	${MV} $@ ${OBJ_DIR};

encrypt.o :
	@echo "[41m< obj $@ >[0m"
	${CC} -D_OBJ $*.c -fPIC -c $(INCLUDE) -lnsl
	${AR} ${HWSEED} $@
	${MV} $@ ${OBJ_DIR};

##--------------------------------------------------------

encrypt :
	@echo "[41m< test $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

kang :
	@echo "[41m< test $@ >[0m"
	${CC} -D_EXE $@.c -o $@  $(INCLUDE) -DBIG_ENDIAN -lnsl ${LIBS}
	${MV} $@   ${BIN_DIR};

############################################################

