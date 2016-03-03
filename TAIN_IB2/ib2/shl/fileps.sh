#!/bin/ksh

HOSTNAME=`/bin/hostname`
DATE=`/bin/date "+%Y%m%d"`

DAT1_PATH=/hw03/ibridge/DAT
SEQ1_PATH=/hw03/ibridge/SEQ
LOG1_PATH=/hw02/ibridge/

DAT2_PATH=/hw03/ibridge/DAT2
SEQ2_PATH=/hw03/ibridge/SEQ2
LOG2_PATH=/hw02/ibridge/LOG2

case ${HOSTNAME} in
	"dbizap01"   ) SVRNM=AP;;
	"pbizap01"   ) SVRNM=AP;;
	"pbizap02"   ) SVRNM=AP;;
	"dfep03"     ) SVRNM=FEP;;
	"pfep03"     ) SVRNM=FEP;;
esac

/bin/echo "HOSTNAME=[${HOSTNAME}]"
/bin/echo "SVRNM   =[${SVRNM}]"
/bin/echo "DATE    =[${DATE}]"
/bin/date
/bin/echo "------------------------------------------------------------"
/bin/echo ""

#########################################################################

ps_netstat()
{
	#------------------------------------------------------
	# netstat -na -p
	
	PSS=`/bin/ps -ef | /bin/grep ibridge | /bin/grep ${FEPID} | /bin/grep -v vi | /bin/grep -v tail | /bin/grep -v grep | /bin/grep -v ibrun | /bin/grep -v ibend | /bin/awk '{ print $2 }'`
	
	KPS="${FEPID}"
	
	for PS in ${PSS}
	do
		KPS="${KPS}| ${PS}/"
	done;
	
	echo ${KPS}
	/bin/netstat -na -p | /bin/grep -E "${KPS}"
}

fileps1()
{
	#------------------------------------------------------
	# Version 1

	FEPID=$1
	DESC=$2
	WARN=$3

	if [ "AP" = "${SVRNM}" ]
	then
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT1_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ1_PATH}/${DATE}/*${FEPID}*
		/bin/cat ${SEQ1_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG1_PATH}/${DATE}/*${FEPID}*
		
		#echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi
		
		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	else
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT1_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ1_PATH}/${DATE}/*${FEPID}* | /bin/grep -v RESHOST
		/bin/echo " => RECV : "`/bin/cat ${SEQ1_PATH}/${DATE}/OAR${FEPID}*` 
		/bin/echo " => SEND : "`/bin/cat ${SEQ1_PATH}/${DATE}/OAS${FEPID}*`
		
		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG1_PATH}/${DATE}/*${FEPID}*
		
		#echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi

		#echo "[32m--- NET (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps_netstat

		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	fi
}

fileps2()
{
	#------------------------------------------------------
	# Version 2

	FEPID=$1
	DESC=$2
	WARN=$3

	if [ "AP" = "${SVRNM}" ]
	then
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT1_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ1_PATH}/${DATE}/*${FEPID}*
		/bin/cat ${SEQ1_PATH}/${DATE}/*${FEPID}*

		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG1_PATH}/${DATE}/*${FEPID}*
		
		#/bin/echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi
		
		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	else
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT2_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ2_PATH}/${DATE}/*${FEPID}*
		/bin/echo " => RECV : "`/bin/cat ${SEQ2_PATH}/${DATE}/OAR${FEPID}*` 
		/bin/echo " => SEND : "`/bin/cat ${SEQ2_PATH}/${DATE}/OAS${FEPID}*`

		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG2_PATH}/${DATE}/*${FEPID}*
		
		#/bin/echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi

		#/bin/echo "[32m--- NET (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps_netstat

		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	fi
}

fileps3()
{
	#------------------------------------------------------
	# WEBÄ³½Ã

	FEPID=$1
	DESC=$2
	WARN=$3

	if [ "AP" = "${SVRNM}" ]
	then
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT1_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ1_PATH}/${DATE}/*${FEPID}*
		/bin/cat ${SEQ1_PATH}/${DATE}/*${FEPID}*

		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG1_PATH}/${DATE}/*${FEPID}*
		
		#/bin/echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi
		
		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	else
		/bin/echo "[44m--- ¡Ú DAT (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${DAT2_PATH}/${DATE}/*${FEPID}*
		
		/bin/echo "[32m--- SEQ (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${SEQ2_PATH}/${DATE}/*${FEPID}*
		/bin/echo " => RECV : "`/bin/cat ${SEQ2_PATH}/${DATE}/OAR${FEPID}*` 
		/bin/echo " => SEND : "`/bin/cat ${SEQ2_PATH}/${DATE}/OAS${FEPID}*`

		/bin/echo "[32m--- LOG (${FEPID} : ${DESC}) -----------------------------[0m"
		/bin/ls -al ${LOG2_PATH}/${DATE}/*${FEPID}*
		
		#/bin/echo "[32m--- PS  (${FEPID} : ${DESC}) -----------------------------[0m"
		#ps -ef | grep ibridge | grep ${FEPID} | grep -v grep | grep -v tail | grep -v vi
		
		/bin/echo "WARN : [31m${WARN}[0m"
		/bin/echo "============================================="
		/bin/echo ""
	fi
}

#########################################################################
# 1. ¿Â¶óÀÎ : ÀºÇà

fileps2 KBB01 "ÀºÇà:±¹¹ÎÀºÇà ¿¬°è"
fileps2 SHB01 "ÀºÇà:½ÅÇÑÀºÇà Æß¹ðÅ·"
fileps2 WRB01 "ÀºÇà:¿ì¸®ÀºÇà ÀÌÃ¼"
fileps2 WRB02 "ÀºÇà:¿ì¸®ÀºÇà ¼­ºñ½º"
fileps2 WRB03 "ÀºÇà:¿ì¸®ÀºÇà ÀÚµ¿ÀÌÃ¼"
fileps2 WRB04 "ÀºÇà:¿ì¸®ÀºÇà º¸Åë¿¹±Ý"
fileps2 WRB05 "ÀºÇà:¿ì¸®ÀºÇà ÀÚ±ÝÁý±Ý"
fileps2 WRB06 "ÀºÇà:¿ì¸®ÀºÇà ½Ç½Ã°£"
fileps2 WRB07 "ÀºÇà:¿ì¸®ÀºÇà ÅðÁ÷¿¬±Ý"
fileps2 WRB08 "ÀºÇà:¿ì¸®ÀºÇà °¡»ó°èÁÂ"
fileps2 TKB01 "ÀºÇà:´ë±¸ÀºÇà ÀÌÃ¼"
fileps2 EXB01 "ÀºÇà:¿ÜÈ¯ÀºÇà ¿¬°è"
fileps2 EXB02 "ÀºÇà:¿ÜÈ¯ÀºÇà È¯À²Á¤º¸¼ö½Å"
fileps2 HNB01 "ÀºÇà:ÇÏ³ªÀºÇà ¿¬°è"
fileps2 HNB02 "ÀºÇà:ÇÏ³ªÀºÇà ÁÖ¹®Áõ°Å±Ý"
#fileps2 HNB03 "ÀºÇà:ÇÏ³ªÀºÇà ÀÌÃ¼"
fileps2 PSB01 "ÀºÇà:ºÎ»êÀºÇà ÀÌÃ¼"
fileps2 POS01 "ÀºÇà:¿ì Ã¼ ±¹ Á¦ÈÞ"
fileps2 NHB01 "ÀºÇà:³ó    Çù ¿¬°è"
fileps2 NHB02 "ÀºÇà:³ó    Çù °¡»ó(ÀÌÃ¼)"
fileps2 NHB03 "ÀºÇà:³ó    Çù ¿¹¼ö±Ý"
fileps2 GUB01 "ÀºÇà:±â¾÷ÀºÇà ¿¬°è"
fileps2 SCB01 "ÀºÇà:SCÁ¦ÀÏÀºÇà ÀÌÃ¼"                       "¾ÏÈ£È­ Àû¿ë"

# 2. ¿Â¶óÀÎ : Ä«µå

fileps2 SHC01 "Ä«µå:½ÅÇÑÄ«µå ¿Â¶óÀÎTR"
fileps2 SHC02 "Ä«µå:½ÅÇÑÄ«µå ¿Â¶óÀÎBL"
fileps2 SHC03 "Ä«µå:½ÅÇÑÄ«µå ½ÅÃ»"
fileps2 SHC04 "Ä«µå:½ÅÇÑÄ«µå Æß¹ðÅ·"
fileps2 HDC01 "Ä«µå:Çö´ëÄ«µå ½ÂÀÎÀÔÃâ±Ý"
fileps2 HDC02 "Ä«µå:Çö´ëÄ«µå ¹ß±Þ/ºñ¹øµî·Ï"
fileps2 HDC03 "Ä«µå:Çö´ëÄ«µå BL"
fileps2 HDC04 "Ä«µå:Çö´ëÄ«µå CMA½Å¿ëÄ«µå"
fileps2 LTC01 "Ä«µå:·Ôµ¥Ä«µå ½ÅÃ»/Á¢¼ö½ÅÃ»"
fileps2 LTC02 "Ä«µå:·Ôµ¥Ä«µå BL"
fileps2 SSC01 "Ä«µå:»ï¼ºÄ«µå CMA½Å¿ëÄ«µå"
fileps2 CTC01 "Ä«µå:½ÃÆ¼Ä«µå Á¦ÈÞ"

# 3. ¿Â¶óÀÎ : ±âÅ¸

fileps2 DHI01 "±âÅ¸:´ëÇÑ  »ý¸í ½Å¿ë´ëÃâ"
fileps2 DHI02 "±âÅ¸:´ëÇÑ  »ý¸í ATMÀÔÇ®±Ý"
fileps2 DHI03 "±âÅ¸:´ëÇÑ  »ý¸í ÀÚµ¿³³ÀÔ"
fileps2 KIS01 "±âÅ¸:³ª  ÀÌ  ½º ¿¬°è"
fileps2 KFT01 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø Å¸ÇàÈ¯"                     "¿î¿µ½Ã°£(09:00~19:00)"
fileps2 KFT02 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CD/ATM"
fileps2 KFT03 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ÀüÀÚ±ÝÀ¶"
fileps2 KFT04 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ´ë°í°´¼­ºñ½º(ARS)"
fileps2 KFT05 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ÀÎÅÍ³ÝÁö·Î"
fileps2 KFT06 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ½Ç½Ã°£Áö·Î"                 "¿î¿µ½Ã°£(09:00~19:00)"
fileps2 KFT07 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡¼­ºñ½º"
fileps2 KFT08 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø PG"
fileps2 KFT09 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡¼­ºñ½º(ÇÑÈ­Áõ±Ç)"
fileps2 KFT10 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡¼­ºñ½º(ÇÁ·çµ§¼È)"
fileps2 KFT11 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ÀÎÅÍ³ÝÁö·Î(Å¸¹ß)"
fileps2 KFT12 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ½Ç½Ã°£Áö·Î(Å¸¹ß)"           "¿î¿µ½Ã°£(09:00~19:00)"
fileps2 HNT01 "±âÅ¸:ÇÑ  ³×  Æ® Á¡¿ÜCD"
fileps2 GBK01 "±âÅ¸:°ÔÀÌÅ©¹ðÅ© Á¡¿ÜCD"
fileps2 HYS01 "±âÅ¸:È¿      ¼º Á¡¿ÜCD"
fileps2 CHO01 "±âÅ¸:Ã»      È£ Á¡¿ÜCD"
fileps2 KBK01 "±âÅ¸:KIS   BANK Á¡¿ÜCD"
fileps2 LOT01 "±âÅ¸:·Ô      µ¥ Á¡¿ÜCD"
fileps2 SSN01 "±âÅ¸:»ï¼º³×Æ®¿÷ Æß¹ðÅ·"
fileps2 KSN01 "±âÅ¸:KS  -  NET Æß¹ðÅ·"
fileps2 LGU01 "±âÅ¸:LGÀ¯ÇÃ·¯½º PG"                         "ÇÊ¿ä½Ã¸¸ Á¢¼Ó"
fileps2 TAT01 "±âÅ¸:½ÌÅ©¿¡ÀÌÆ¼ 2Ã¤³Î ÀÎÁõ"

# 4. ¹èÄ¡

#fileps1 HDC51 "Ä«µå:Çö´ëÄ«µå ÀÏ°ý¹èÄ¡     (WEBÄ³½Ã)"       "WEBÄ³½Ã"
#fileps2 SSC51 "Ä«µå:»ï¼ºÄ«µå ÀÏ°ý¹èÄ¡¼ö½Å (WEBÄ³½Ã)"       "WEBÄ³½Ã"
#fileps1 NHC51 "Ä«µå:³óÇùÄ«µå ÀÏ°ý¹èÄ¡¼ö½Å (WEBÄ³½Ã)"       "WEBÄ³½Ã"
#fileps1 BCC51 "Ä«µå:B C Ä«µå ½ÂÀÎ¼ö½Å     (WEBÄ³½Ã)"       "WEBÄ³½Ã"
#fileps1 KBC51 "Ä«µå:±¹¹ÎÄ«µå ½ÂÀÎ¼ö½Å     (WEBÄ³½Ã)"       "WEBÄ³½Ã"
#fileps1 SSC52 "Ä«µå:»ï¼ºÄ«µå ¸ÅÀÔÁ¤º¸¼ö½Å (WEBÄ³½Ã)"       "WEBÄ³½Ã"

fileps2 HDC52 "Ä«µå:Çö´ëÄ«µå ÀÏ°ý¹èÄ¡"    
fileps2 CTC51 "Ä«µå:½ÃÆ¼Ä«µå ÀÏ°ý¹èÄ¡¼ö½Å"
fileps2 SHC51 "Ä«µå:½ÅÇÑÄ«µå ÀÏ°ý¹èÄ¡"
fileps2 LTC51 "Ä«µå:·Ôµ¥Ä«µå ÀÏ°ý¹èÄ¡¼ö½Å"
fileps2 SSN51 "±âÅ¸:»ï¼º³×Æ®¿÷ ÀÏ°ý¹èÄ¡"
fileps2 KSN51 "±âÅ¸:KS  -  NET ÀÏ°ý¹èÄ¡"

fileps2 KFT51 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ÀÏ°ýÀü¼Û"
fileps2 KFT52 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMS"
fileps2 KFT53 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMS(ÇÑÈ­Áõ±Ç)"
fileps2 KFT54 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø CMD(ÇÁ·çµ§¼È)"
fileps2 KFT55 "±âÅ¸:±ÝÀ¶°áÁ¦¿ø ÀÏ°ýÀü¼Û(Áö·Î)"


