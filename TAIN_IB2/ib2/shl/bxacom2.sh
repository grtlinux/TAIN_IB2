#!/bin/ksh

###################################################################
# DATE 2013.10.28 : ��ġ�ۼ��� ���α׷� ����(shell)

TODAY=`/bin/date "+%Y%m%d"`
WAIT_TIME=60

LOGPATH=/hw02/ibridge/LOG2/${TODAY}

###################################################################
# DATE 2013.10.28 : ���� �ۼ�
#

#--------------------------------------------------------------
# ����ī��  �ϰ���ġ (�ۼ���)
Batch_BXASHC5101()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxsshc BXSSHC5101 >> ${LOGPATH}/BXASHC5101
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrshc BXRSHC5101 >> ${LOGPATH}/BXASHC5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ����ī��  �ϰ���ġ (�ۼ���)
Batch_BXAHDC5201()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxshdc BXSHDC5201 >> ${LOGPATH}/BXAHDC5201
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrhdc BXRHDC5201 >> ${LOGPATH}/BXAHDC5201
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ��Ƽī�� �ϰ���ġ����
Batch_BXACTC5101()
{
	while ( true )
	do

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrctc BXRCTC5101 >> ${LOGPATH}/BXACTC5101
		
		#/bin/sleep ${WAIT_TIME}
		/bin/sleep 5
	done
}

#--------------------------------------------------------------
# ���������� �ϰ�����
Batch_BXAKFT5101()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxskft1 BXSKFT5101 >> ${LOGPATH}/BXAKFT5101
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrkft1 BXRKFT5101 >> ${LOGPATH}/BXAKFT5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ���������� CMS
Batch_BXAKFT5201()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxskft2 BXSKFT5201 >> ${LOGPATH}/BXAKFT5201
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrkft2 BXRKFT5201 >> ${LOGPATH}/BXAKFT5201
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ���������� CMS(��ȭ����)
Batch_BXAKFT5301()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5301 >> ${LOGPATH}/BXAKFT5301
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5301 >> ${LOGPATH}/BXAKFT5301
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ���������� CMS(���絧��)
Batch_BXAKFT5401()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5401 >> ${LOGPATH}/BXAKFT5401
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5401 >> ${LOGPATH}/BXAKFT5401
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ���������� ����
Batch_BXAKFT5501()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5501 >> ${LOGPATH}/BXAKFT5501
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5501 >> ${LOGPATH}/BXAKFT5501
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# �Ｚ��Ʈ�� �ϰ���ġ
Batch_BXASSN5101()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxsssn BXSSSN5101 >> ${LOGPATH}/BXASSN5101
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrssn BXRSSN5101 >> ${LOGPATH}/BXASSN5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# KS-NET �ϰ���ġ
Batch_BXAKSN5101()
{
	while ( true )
	do
		# ��ġ �۽�
		/hw01/ibridge/ib2/bin/bxsksn BXSKSN5101 >> ${LOGPATH}/BXAKSN5101
		
		/bin/sleep 1

		# ��ġ ����
		/hw01/ibridge/ib2/bin/bxrksn BXRKSN5101 >> ${LOGPATH}/BXAKSN5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

###################################################################
###################################################################

if [ $# != 1 ];
then
	/bin/echo "USAGE : ${0} [ SVCID ]"
	exit 9
fi

case ${1} in
	"BXASHC5101" ) Batch_BXASHC5101;;
	"BXAHDC5201" ) Batch_BXAHDC5201;;
	"BXACTC5101" ) Batch_BXACTC5101;;
	"BXAKFT5101" ) Batch_BXAKFT5101;;
	"BXAKFT5201" ) Batch_BXAKFT5201;;
	"BXAKFT5301" ) Batch_BXAKFT5301;;
	"BXAKFT5401" ) Batch_BXAKFT5401;;
	"BXAKFT5501" ) Batch_BXAKFT5501;;
	"BXASSN5101" ) Batch_BXASSN5101;;
	"BXAKSN5101" ) Batch_BXAKSN5101;;
	* ) /bin/echo "[31m??? �𸣴� SVCID �Դϴ�.[0m"
		;;
esac


