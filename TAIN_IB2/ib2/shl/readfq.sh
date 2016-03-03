#!/bin/ksh

######################################################################
# DATE.2013.10.30 : È¯°æÁ¤º¸ (PORT:41501)

CURR=`date "+%Y.%m.%d %H:%M:%S"`
TODAY=`date "+%Y%m%d"`

FILEPATH=/hw02/ibridge/LOG2/${TODAY}
COMMAND=
FILENAME=
COUNT=10

#cat ${FILEPATH}/${FILENAME}
#exit 0

######################################################################
# DATE.2013.12.06 : FUNCTION

#---------------------------------------------------------------------
#
func_00_dat()
{
	FILEPATH=/hw03/ibridge/DAT2/${TODAY}

	# echo -n "Please input the key : "
	read FILENAME
	FILENAME=`echo ${FILENAME} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`
	
	case ${FILENAME} in
		OFR??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OFS??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OAR??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OAS??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		* )
			echo "# ERROR : Do not know the filename : [${FILENAME}]"
			;;
	esac
}

#---------------------------------------------------------------------
#
func_00_seq()
{
	FILEPATH=/hw03/ibridge/SEQ2/${TODAY}

	# echo -n "Please input the key : "
	read FILENAME
	FILENAME=`echo ${FILENAME} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`
	
	case ${FILENAME} in
		OFR??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OFS??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OAR??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		OAS??????? )
			cat ${FILEPATH}/${FILENAME}
			;;
		* )
			echo "# ERROR : Do not know the filename : [${FILENAME}]"
			;;
	esac
}

#---------------------------------------------------------------------
#
func_00()
{
	FILEPATH=/hw03/ibridge/DAT2/${TODAY}

	# echo -n "Please input the key : "
	read COMMAND
	COMMAND=`echo ${COMMAND} | tr 'a-z' 'A-Z' | tr -d ' ' | tr -d '\r'`
	
	case ${COMMAND} in
		LIST )
			ls -al ${FILEPATH}
			;;
		DAT )
			func_00_dat
			;;
		SEQ )
			func_00_seq
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


