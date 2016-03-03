#!/bin/ksh
# ----------------------------------------------------------------------
# �⺻���� ����

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
# �⺻���� ���

echo "--------- �⺻���� ��� ------------------------------------------"
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
# �Լ� : ���μ��� ����
func_kill()
{
	echo "* KILL THE PROCESS"

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | awk '{ print $2 }'`
	
	for PS in ${PSS}
	do
		echo "    ���� : kill -9 ${PS}"
		kill -9 ${PS}
	done;
}

# ----------------------------------------------------------------------
# �Լ� : �����
func_usage()
{
	echo "USAGE : ${0} [ run | start | begin | beg | kill | stop | end | finish ]"
	echo "    ���� : run, start, begin, beg"
	echo "    ���� : kill, stop, end, finish"
	echo ""
}

#########################################################################

# ----------------------------------------------------------------------
# Main Process

if [ $# -gt 0 ];
then
	# --- �빮�� ��ȯ
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
