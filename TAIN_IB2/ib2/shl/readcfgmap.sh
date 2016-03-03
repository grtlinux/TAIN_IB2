#!/bin/ksh

######################################################################
# DATE.2013.10.30 : ȯ������ (PORT:41501)

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
	FILEPATH=/hw01/ibridge/ram/cfg/mapper
	
	# echo -n "Please input the key : "
	read FILENAME
	
	FILENAME=`echo ${FILENAME} | tr -d '\r'`
	
	case ${FILENAME} in
		code.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		comp.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		field.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		fieldcode.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		routing.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		system.cfg )
			cat ${FILEPATH}/${FILENAME}
			;;
		msg_*.cfg )
			if [ -f ${FILEPATH}/${FILENAME} ];
			then
				cat ${FILEPATH}/${FILENAME}
			else
				echo "# ERROR : Do not know the filename : [${FILENAME}]"
			fi
			;;
		sender.message )
			cat ${FILEPATH}/${FILENAME}
			;;
		sender.scenario )
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
		SHC51����ī��_�ۼ���..  \
		HDC52����ī��_�ۼ���..  \
		KFT51�ݰ��_�ϰ�����..  \
		KFT52�ݰ��_�ϰ�����..  \
		KFT53�ݰ��_CMS_��ȭ..  \
		KFT54�ݰ��_CMS_����..  \
		KFT55�ݰ��_����......  \
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


