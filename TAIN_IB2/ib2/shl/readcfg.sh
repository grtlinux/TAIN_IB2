#!/bin/ksh

######################################################################
# DATE.2013.10.30 : È¯°æÁ¤º¸ (PORT:41501)

CURR=`date "+%Y.%m.%d %H:%M:%S"`
TODAY=`date "+%Y%m%d"`

FILEPATH=/hw01/ibridge/ib2/cfg
FILENAME=biz.properties
#cat ${FILEPATH}/${FILENAME}
#exit 0

######################################################################
# DATE.2013.12.06 : FUNCTION

#---------------------------------------------------------------------
#
func_00()
{
	FILEPATH=/hw01/ibridge/ib2/cfg
	
	# echo -n "Please input the key : "
	read FILENAME
	
	FILENAME=`echo ${FILENAME} | tr -d '\r'`
	
	case ${FILENAME} in
		Master.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		biz.properties )
			cat ${FILEPATH}/${FILENAME}
			;;
		bxa_trlst.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		bxacom0000.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		ibnetd.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		ofrcom2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		ofscom2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		oircom2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		oiscom2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		oxaline2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		oxapoll2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		oiapoll2.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		ums.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		fep.properties )
			FILEPATH=/hw01/ibridge/vela/banca/server/fep/cfg
			cat ${FILEPATH}/${FILENAME}
			;;
		ap.properties )
			FILEPATH=/hw01/ibridge/vela/banca/server/ap/cfg
			cat ${FILEPATH}/${FILENAME}
			;;
		bat.properties )
			FILEPATH=/hw01/ibridge/vela/banca/server/bat/cfg
			cat ${FILEPATH}/${FILENAME}
			;;
		org_tel.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		* )
			echo "# ERROR : Do not know the filename : [${FILENAME}]"
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


