#!/bin/ksh

######################################################################
# DATE.2013.10.30 : È¯°æÁ¤º¸ (PORT:41501)

CURR=`date "+%Y.%m.%d %H:%M:%S"`
TODAY=`date "+%Y%m%d"`
Time=`/bin/date`

FILEPATH=/hw02/ibridge/LOG2/${TODAY}
COMMAND=
FILENAME=
TAILCOUNT=10

#cat ${FILEPATH}/${FILENAME}
#exit 0

######################################################################
# DATE.2013.12.06 : FUNCTION

#---------------------------------------------------------------------
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXRLTC5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXACTC5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAHDC5201 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKSN5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXASHC5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXASSN5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKFT5101 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKFT5201 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKFT5301 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKFT5401 | grep 20131230 | grep -v grep | tail -n 3
# tail -n 100 /hw02/ibridge/LOG2/20131230/BXAKFT5501 | grep 20131230 | grep -v grep | tail -n 3

func_00_connect()
{
	printf "${Time}\n"

	FILEPATH=/hw02/ibridge/LOG2/${TODAY}

	FILENAME=BXASHC5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "01 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAHDC5201
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "02 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXASSN5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "03 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAKSN5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "04 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXACTC5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "05 ${FILENAME} C ${FILETAIL}"



	FILENAME=BXRLTC5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "06 ${FILENAME} S ${FILETAIL}"



	FILENAME=BXAKFT5101
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "07 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAKFT5201
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "08 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAKFT5301
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "09 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAKFT5401
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "10 ${FILENAME} C ${FILETAIL}"

	FILENAME=BXAKFT5501
	FILETAIL=`tail -n 100 ${FILEPATH}/${FILENAME} | grep ${TODAY} | grep -v grep | tail -n 1`
	echo "11 ${FILENAME} C ${FILETAIL}"
}

#---------------------------------------------------------------------
#
func_00_rfile()
{
	FILEPATH=/hw03/ibridge/RECV/${TODAY}

	# echo -n "Please input the key : "
	read FILENAME
	FILENAME=`echo ${FILENAME} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`

	if [ -f ${FILEPATH}/${FILENAME} ];
	then
		cat ${FILEPATH}/${FILENAME}
	else
		echo "# ERROR : Do not know the filename : [${FILENAME}]"
	fi;
}

#---------------------------------------------------------------------
#
func_00_sfile()
{
	FILEPATH=/hw03/ibridge/SEND/${TODAY}

	# echo -n "Please input the key : "
	read FILENAME
	FILENAME=`echo ${FILENAME} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`

	if [ -f ${FILEPATH}/${FILENAME} ];
	then
		cat ${FILEPATH}/${FILENAME}
	else
		echo "# ERROR : Do not know the filename : [${FILENAME}]"
	fi;
}

#---------------------------------------------------------------------
#
func_00()
{
	FILEPATH=/hw02/ibridge/LOG2/${TODAY}

	# echo -n "Please input the key : "
	read COMMAND
	COMMAND=`echo ${COMMAND} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`
	
	case ${COMMAND} in
		BAT_ALL )
			echo "[RECVFILES]"
			/bin/ls -ltR /hw03/ibridge/RECV/${TODAY}

			echo "[SENDFILES]"
			/bin/ls -ltR /hw03/ibridge/SEND/${TODAY}

			echo "[COUNT]"
			/hw01/ibridge/ib2/shl/ibseq.sh

			echo "[CONNECT]"
			func_00_connect
			;;
		RECVFILES )
			/bin/ls -ltR /hw03/ibridge/RECV/${TODAY}
			;;
		SENDFILES )
			/bin/ls -ltR /hw03/ibridge/SEND/${TODAY}
			;;
		RFILE )
			func_00_rfile
			;;
		SFILE )
			func_00_sfile
			;;
		COUNT )
			/hw01/ibridge/ib2/shl/ibseq.sh
			;;
		CONNECT )
			func_00_connect
			;;
		* )
			echo "# ERROR : Do not know the filename : [${COMMAND}]"
			;;
	esac
}

#---------------------------------------------------------------------
#
func_01()
{
	INFOS="                     \
		SHC51½ÅÇÑÄ«µå_¼Û¼ö½Å..  \
		HDC52Çö´ëÄ«µå_¼Û¼ö½Å..  \
		KFT51±Ý°á¿ø_ÀÏ°ýÀü¼Û..  \
		KFT52±Ý°á¿ø_ÀÏ°ýÀü¼Û..  \
		KFT53±Ý°á¿ø_CMS_ÇÑÈ­..  \
		KFT54±Ý°á¿ø_CMS_ÇÁ·ç..  \
		KFT55±Ý°á¿ø_Áö·Î......  \
		"
	echo "[41m${INFOS}[0m"

	for INFO in ${INFOS}
	do
		FEPID=`expr substr ${INFO} 1 5`
		FEPNAME=`expr substr ${INFO} 6 17`
		
		/bin/echo ${INFO} ${FEPID} ${FEPNAME}
	done
}

#---------------------------------------------------------------------
#
func_02()
{
	LINE01=" ABCDEFG "
	LINE02=" 1234567 "
	LINE03=" AAAAAAA "
	
	LINE01="${LINE01} BAT00001"
	LINE02="${LINE02} BAT00002"

	echo ${LINE01};	
	echo ${LINE02};	
	echo ${LINE03};	
}

######################################################################
# DATE.2013.10.30 : MAIN

if [ $# -eq 0 ];
then
	func_00
else
	case ${1} in
		00 ) func_00 ;;
		01 ) func_01 ;;
		02 ) func_02 ;;
		03 ) func_03 ;;
		*    )
			echo "# NO ARGUMENT"
			;;
	esac;
fi;


