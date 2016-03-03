#!/bin/ksh

# ----------------------------------------------------------------------
# 기본변수 세팅

HOSTNAME=`hostname`
DATE=`date "+%Y%m%d"`
TIME=`date "+%H%M%S"`
PROCESS=ps_man

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

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | grep -v ".sh" | awk '{ print $2 }'`
	
	for PS in ${PSS}
	do
		echo "    종료 : kill -9 ${PS}"
		kill -9 ${PS}
	done;
}

# ----------------------------------------------------------------------
# 함수 : 프로세스 실행. 이미 실행되어 있으면 종료(kill)하고 다시 실행한다.
func_run()
{
	func_kill

	echo "* RUN THE PROCESS"
	/hw01/ibridge/ib2/bin/${PROCESS}

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | grep -v ".sh" | awk '{ print $2 }'`
	echo "    실행 : ${PROCESS}  : PID=${PSS}"
}

# ----------------------------------------------------------------------
# 함수 : FEP에서 RECV/SEND에 해당하는 폴더를 만든다.
func_mkdir()
{
	echo "MAKE THE RECV/SEND FOLDER"
	
	if [ "FEP" = "${SVRNM}" ]
	then
		# ----- CREATE RECV FOLDER
		FOLDER=/hw03/ibridge/RECV/${DATE}
		if [ ! -d ${FOLDER} ]
		then
			mkdir ${FOLDER}
		fi

		# ----- CREATE SEND FOLDER
		FOLDER=/hw03/ibridge/SEND/${DATE}
		if [ ! -d ${FOLDER} ]
		then
			mkdir ${FOLDER}
		fi
	fi
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
