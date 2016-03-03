#!/bin/ksh
# ----------------------------------------------------------------------
# 기본변수 세팅

HOSTNAME=`hostname`
DATE=`date "+%Y%m%d"`
TIME=`date "+%H%M%S"`
PROCESS=bxacom2

case ${HOSTNAME} in
	"dbizap01"   ) SVRNM=AP;;
	"pbizap01"   ) SVRNM=AP;;
	"pbizap02"   ) SVRNM=AP;;
	"dfep03"     ) SVRNM=FEP;;
	"pfep03"     ) SVRNM=FEP;;
	*            ) SVRNM=NOTHING;;
esac

DAT1_PATH=/hw03/ibridge/DAT
SEQ1_PATH=/hw03/ibridge/SEQ
LOG1_PATH=/hw02/ibridge/

DAT2_PATH=/hw03/ibridge/DAT2
SEQ2_PATH=/hw03/ibridge/SEQ2
LOG2_PATH=/hw02/ibridge/LOG2

# ----------------------------------------------------------------------
# 기본변수 출력

echo "--------- 기본변수 출력 ------------------------------------------"
echo "HOSTNAME=[${HOSTNAME}]"
echo "PROCESS =[${PROCESS}]"
echo "SVRNM   =[${SVRNM}]"
echo "DATE    =[${DATE}]"
echo "TIME    =[${TIME}]"
date
echo "------------------------------------------------------------"
echo ""

#########################################################################

# ----------------------------------------------------------------------
# 함수 : 프로세스 종료
func_kill()
{
	echo "* KILL THE PROCESS"

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | awk '{ print $2 }'`
	
	for PS in ${PSS}
	do
		echo "    종료 : kill -9 ${PS}"
		kill -9 ${PS}
	done;
}

# ----------------------------------------------------------------------
# 함수 : 사용방법
func_usage()
{
	echo "USAGE : ${0} [ run | start | begin | beg | kill | stop | end | finish ]"
	echo "    실행 : run, start, begin, beg"
	echo "    종료 : kill, stop, end, finish"
	echo ""
}

#########################################################################

# ----------------------------------------------------------------------
# Main Process

if [ $# -gt 0 ];
then
	# --- 대문자 변환
	CMD=`echo ${1} | tr 'a-z' 'A-Z'`
	
	case ${CMD} in
		"RUN" | "START" | "BEGIN" | "BEG" ) 
			func_run
			func_mkdir
			;;
		"KILL" | "STOP" | "END" | "FINISH" ) 
			func_kill
			;;
		*       )
			func_usage
			;;
	esac
else
	func_usage
fi
