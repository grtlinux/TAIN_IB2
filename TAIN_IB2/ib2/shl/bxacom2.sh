#!/bin/ksh

###################################################################
# DATE 2013.10.28 : ¹èÄ¡¼Û¼ö½Å ÇÁ·Î±×·¥ ½ÇÇà(shell)

TODAY=`/bin/date "+%Y%m%d"`
WAIT_TIME=60

LOGPATH=/hw02/ibridge/LOG2/${TODAY}

###################################################################
# DATE 2013.10.28 : ÃÖÃÊ ÀÛ¼º
#

#--------------------------------------------------------------
# ½ÅÇÑÄ«µå  ÀÏ°ý¹èÄ¡ (¼Û¼ö½Å)
Batch_BXASHC5101()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxsshc BXSSHC5101 >> ${LOGPATH}/BXASHC5101
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrshc BXRSHC5101 >> ${LOGPATH}/BXASHC5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# Çö´ëÄ«µå  ÀÏ°ý¹èÄ¡ (¼Û¼ö½Å)
Batch_BXAHDC5201()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxshdc BXSHDC5201 >> ${LOGPATH}/BXAHDC5201
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrhdc BXRHDC5201 >> ${LOGPATH}/BXAHDC5201
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ½ÃÆ¼Ä«µå ÀÏ°ý¹èÄ¡¼ö½Å
Batch_BXACTC5101()
{
	while ( true )
	do

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrctc BXRCTC5101 >> ${LOGPATH}/BXACTC5101
		
		#/bin/sleep ${WAIT_TIME}
		/bin/sleep 5
	done
}

#--------------------------------------------------------------
# ±ÝÀ¶°áÁ¦¿ø ÀÏ°ýÀü¼Û
Batch_BXAKFT5101()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxskft1 BXSKFT5101 >> ${LOGPATH}/BXAKFT5101
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrkft1 BXRKFT5101 >> ${LOGPATH}/BXAKFT5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ±ÝÀ¶°áÁ¦¿ø CMS
Batch_BXAKFT5201()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxskft2 BXSKFT5201 >> ${LOGPATH}/BXAKFT5201
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrkft2 BXRKFT5201 >> ${LOGPATH}/BXAKFT5201
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ±ÝÀ¶°áÁ¦¿ø CMS(ÇÑÈ­Áõ±Ç)
Batch_BXAKFT5301()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5301 >> ${LOGPATH}/BXAKFT5301
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5301 >> ${LOGPATH}/BXAKFT5301
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ±ÝÀ¶°áÁ¦¿ø CMS(ÇÁ·çµ§¼È)
Batch_BXAKFT5401()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5401 >> ${LOGPATH}/BXAKFT5401
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5401 >> ${LOGPATH}/BXAKFT5401
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# ±ÝÀ¶°áÁ¦¿ø Áö·Î
Batch_BXAKFT5501()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxskft3 BXSKFT5501 >> ${LOGPATH}/BXAKFT5501
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrkft3 BXRKFT5501 >> ${LOGPATH}/BXAKFT5501
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# »ï¼º³×Æ®¿÷ ÀÏ°ý¹èÄ¡
Batch_BXASSN5101()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxsssn BXSSSN5101 >> ${LOGPATH}/BXASSN5101
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
		/hw01/ibridge/ib2/bin/bxrssn BXRSSN5101 >> ${LOGPATH}/BXASSN5101
		
		/bin/sleep ${WAIT_TIME}
	done
}

#--------------------------------------------------------------
# KS-NET ÀÏ°ý¹èÄ¡
Batch_BXAKSN5101()
{
	while ( true )
	do
		# ¹èÄ¡ ¼Û½Å
		/hw01/ibridge/ib2/bin/bxsksn BXSKSN5101 >> ${LOGPATH}/BXAKSN5101
		
		/bin/sleep 1

		# ¹èÄ¡ ¼ö½Å
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
	* ) /bin/echo "[31m??? ¸ð¸£´Â SVCID ÀÔ´Ï´Ù.[0m"
		;;
esac


