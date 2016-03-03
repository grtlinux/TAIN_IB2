#!/bin/ksh

# ----------------------------------------------------------------------
# �⺻���� ����

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

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | grep -v ".sh" | awk '{ print $2 }'`
	
	for PS in ${PSS}
	do
		echo "    ���� : kill -9 ${PS}"
		kill -9 ${PS}
	done;
}

# ----------------------------------------------------------------------
# �Լ� : ���μ��� ����. �̹� ����Ǿ� ������ ����(kill)�ϰ� �ٽ� �����Ѵ�.
func_run()
{
	func_kill

	echo "* RUN THE PROCESS"
	/hw01/ibridge/ib2/bin/${PROCESS}

	PSS=`ps -ef | grep ibridge | grep ${PROCESS} | grep -v vi | grep -v tail | grep -v grep | grep -v ibrun | grep -v ibend | grep -v ".sh" | awk '{ print $2 }'`
	echo "    ���� : ${PROCESS}  : PID=${PSS}"
}

# ----------------------------------------------------------------------
# �Լ� : FEP���� RECV/SEND�� �ش��ϴ� ������ �����.
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
