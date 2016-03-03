#!/bin/ksh
# Ignore HUP, INT, QUIT now.
#trap "" 1 2 3 15

#-------------------------------------------------
DATETIME=`/bin/date "+%Y/%m/%d %H:%M:%S"`

USERID=${USER}
HOME_DIR=/hw01/ibridge/ib2

APP_BIN=${HOME_DIR}/bin
ARG_LEN=`/usr/bin/expr length "$1"`

EXE_FEPSTAT=${APP_BIN}/fepstat2

#-------------------------------------------------
Dummy()
{
	;
}

#-------------------------------------------------
LST_PS=/tmp/fepstat_ps.lst
LST_NETSTAT=/tmp/fepstat_netstat.lst

CreateFepFile()
{
	# /bin/ps -ef | /bin/grep ibridge | /bin/grep -E "ram/bin|ib2/bin" | /bin/grep -v /bin/grep | /bin/grep -v /bin/ps_man | /bin/awk '{print $2, $8, $9}'
	/bin/ps -ef | /bin/grep ${USERID} | /bin/grep -E "ram/bin|ib2/bin" | /bin/grep -v /bin/grep | /bin/grep -v /bin/ps_man | /bin/awk '{print $2, $8, $9}' > ${LST_PS}

	# netstat -na -t -p | /bin/awk '/^tcp/{print $4, $5, $6, $7}'
	/bin/netstat -na -t -p | /bin/awk '/^tcp/{print $4, $5, $6, $7}' > ${LST_NETSTAT}
}

#-------------------------------------------------
ECHO_TTL="FEPID AP _SvcName__ _PID__ LPort ___Local_IP:Port_____ <-> ___Remote_IP:Port____ ___State____ MBOX ___Comment____________________"
ECHO_BAR="----- -- ---------- ------ ----- --------------------- --- --------------------- ------------ ---- ------------------------------"

PrintFepInfo()
{
	# ${EXE_FEPSTAT} KFT03 '±ÝÀ¶°áÁ¦¿ø ÀüÀÚ±ÝÀ¶           '      ; ${EXE_ECHO}
	# return;

	/bin/echo
	/bin/echo "½ÇÇà½Ã°£ : ${DATETIME}"
	/bin/echo
	/bin/echo "[44m##### ¿Â¶óÀÎ : ÀºÇà #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} KBB01 '±¹¹ÎÀºÇà ¿¬°è                 '      ; /bin/echo

	${EXE_FEPSTAT} SHB01 '½ÅÇÑÀºÇà Æß¹ðÅ·               '      ; /bin/echo

	${EXE_FEPSTAT} WRB01 '¿ì¸®ÀºÇà ÀÌÃ¼                 '      ; /bin/echo
	${EXE_FEPSTAT} WRB02 '¿ì¸®ÀºÇà ¼­ºñ½º               '      ; /bin/echo
	${EXE_FEPSTAT} WRB03 '¿ì¸®ÀºÇà ÀÚµ¿ÀÌÃ¼             '      ; /bin/echo
	${EXE_FEPSTAT} WRB04 '¿ì¸®ÀºÇà º¸Åë¿¹±Ý             '      ; /bin/echo
	${EXE_FEPSTAT} WRB05 '¿ì¸®ÀºÇà ÀÚ±ÝÁý±Ý             '      ; /bin/echo
	${EXE_FEPSTAT} WRB06 '¿ì¸®ÀºÇà RealTime             '      ; /bin/echo
	${EXE_FEPSTAT} WRB07 '¿ì¸®ÀºÇà ÅðÁ÷¿¬±Ý             '      ; /bin/echo
	${EXE_FEPSTAT} WRB08 '¿ì¸®ÀºÇà °¡»ó°èÁÂ             '      ; /bin/echo
	# ${EXE_FEPSTAT} WRB09 'X)¿ì¸®ÀºÇà °áÁ¦Á¤º¸(¸Å¸Å)     '      ; /bin/echo  # ¹Ì»ç¿ëÀ¸·Î »èÁ¦
	# ${EXE_FEPSTAT} WRB10 'X)¿ì¸®ÀºÇà °áÁ¦Á¤º¸(¼ö¼ö·á)   '      ; /bin/echo  # ¹Ì»ç¿ëÀ¸·Î »èÁ¦

	${EXE_FEPSTAT} TKB01 '´ë±¸ÀºÇà ÀÌÃ¼                 '      ; /bin/echo

	${EXE_FEPSTAT} EXB01 '¿ÜÈ¯ÀºÇà ¿¬°è                 '      ; /bin/echo
	${EXE_FEPSTAT} EXB02 '¿ÜÈ¯ÀºÇà È¯À²Á¤º¸¼ö½Å         '      ; /bin/echo

	${EXE_FEPSTAT} HNB01 'ÇÏ³ªÀºÇà ÀÌÃ¼                 '      ; /bin/echo
	${EXE_FEPSTAT} HNB02 'ÇÏ³ªÀºÇà ¿¬°è                 '      ; /bin/echo
	#${EXE_FEPSTAT} HNB03 'ÇÏ³ªÀºÇà ÁÖ¹®Áõ°Å±Ý           '      ; /bin/echo  #¹Ì»ç¿ëÀ¸·Î »èÁ¦

	${EXE_FEPSTAT} PSB01 'ºÎ»êÀºÇà ÀÌÃ¼(X.25)           '      ; /bin/echo

	${EXE_FEPSTAT} POS01 '¿ìÃ¼±¹   Á¦ÈÞ                 '      ; /bin/echo

	${EXE_FEPSTAT} NHB01 '³óÇù     ¿¹¼ö±Ý(¿¬°è)         '      ; /bin/echo
	${EXE_FEPSTAT} NHB02 '³óÇù     °¡»ó(ÀÌÃ¼)           '      ; /bin/echo
	${EXE_FEPSTAT} NHB03 '³óÇù     ¿¬°è(¿¹¼ö±Ý)         '      ; /bin/echo

	${EXE_FEPSTAT} GUB01 '±â¾÷ÀºÇà ¿¬°è(AP01,AP02)      '      ; /bin/echo

	${EXE_FEPSTAT} SCB01 'SCÁ¦ÀÏÀºÇà ÀÌÃ¼(Xecure.)      '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### ¿Â¶óÀÎ : Ä«µå #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} SHC01 '½ÅÇÑÄ«µå ¿Â¶óÀÎTR             '      ; /bin/echo
	${EXE_FEPSTAT} SHC02 '½ÅÇÑÄ«µå ¿Â¶óÀÎBL             '      ; /bin/echo
	${EXE_FEPSTAT} SHC03 '½ÅÇÑÄ«µå ½ÅÃ»                 '      ; /bin/echo
	${EXE_FEPSTAT} SHC04 '½ÅÇÑÄ«µå Æß¹ðÅ·               '      ; /bin/echo

	${EXE_FEPSTAT} HDC01 'Çö´ëÄ«µå ½ÂÀÎÀÔÃâ±Ý           '      ; /bin/echo
	${EXE_FEPSTAT} HDC02 'Çö´ëÄ«µå ¹ß±Þ/ºñ¹øµî·Ï        '      ; /bin/echo
	${EXE_FEPSTAT} HDC03 'Çö´ëÄ«µå BL                   '      ; /bin/echo
	${EXE_FEPSTAT} HDC04 'Çö´ëÄ«µå CMA½Å¿ëÄ«µå          '      ; /bin/echo

	${EXE_FEPSTAT} LTC01 '·Ôµ¥Ä«µå ½ÅÃ»/Á¢¼öÁøÇà        '      ; /bin/echo
	${EXE_FEPSTAT} LTC02 '·Ôµ¥Ä«µå BL                   '      ; /bin/echo

	${EXE_FEPSTAT} SSC01 '»ï¼ºÄ«µå CMA½Å¿ëÄ«µå          '      ; /bin/echo   # MegaBox
	# ${EXE_FEPSTAT} SSC51 '»ï¼ºÄ«µå ÀÏ°ý¹èÄ¡¼ö½Å         '      ; /bin/echo   # MegaBox

	${EXE_FEPSTAT} CTC01 '½ÃÆ¼Ä«µå Á¦ÈÞ                 '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### ¿Â¶óÀÎ : ±âÅ¸ #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} DHI01 '´ëÇÑ»ý¸í   ½Å¿ë´ëÃâ           '      ; /bin/echo
	${EXE_FEPSTAT} DHI02 '´ëÇÑ»ý¸í   ATMÀÔÃâ±Ý          '      ; /bin/echo
	${EXE_FEPSTAT} DHI03 '´ëÇÑ»ý¸í   ÀÚµ¿³³ÀÔ           '      ; /bin/echo

	${EXE_FEPSTAT} KIS01 '³ªÀÌ½º     ¿¬°è               '      ; /bin/echo
	${EXE_FEPSTAT} KIS02 '³ªÀÌ½º     ÈÞ´ëÆùÀÎÁõ         '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS02 'ÇÑ½ÅÆò     Á¶±â°æº¸Á¶È¸       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS03 'ÇÑ½ÅÆò     Á¶±â°æº¸µî·Ï       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS04 'ÇÑ½ÅÆò     CB SCOREÁ¶È¸       '      ; /bin/echo

	${EXE_FEPSTAT} KFT01 '±ÝÀ¶°áÁ¦¿ø Å¸ÇàÈ¯ (07~18)     '      ; /bin/echo
	${EXE_FEPSTAT} KFT02 '±ÝÀ¶°áÁ¦¿ø CD/ATM             '      ; /bin/echo
	${EXE_FEPSTAT} KFT03 '±ÝÀ¶°áÁ¦¿ø ÀüÀÚ±ÝÀ¶           '      ; /bin/echo
	${EXE_FEPSTAT} KFT04 '±ÝÀ¶°áÁ¦¿ø ´ë°í°´¼­ºñ½º(ARS)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT05 '±ÝÀ¶°áÁ¦¿ø ÀÎÅÍ³ÝÁö·Î         '      ; /bin/echo
	${EXE_FEPSTAT} KFT06 '±ÝÀ¶°áÁ¦¿ø ½Ç½Ã°£Áö·Î (09~23) '      ; /bin/echo
	${EXE_FEPSTAT} KFT07 '±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡¼­ºñ½º      '      ; /bin/echo
	${EXE_FEPSTAT} KFT08 '±ÝÀ¶°áÁ¦¿ø PG                 '      ; /bin/echo
	${EXE_FEPSTAT} KFT09 '±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡(ÇÑÈ­Áõ±Ç)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT10 '±ÝÀ¶°áÁ¦¿ø CMSºÎ°¡(ÇÁ·çµ§¼È)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT11 '±ÝÀ¶°áÁ¦¿ø ÀÎÅÍ³ÝÁö·Î-Å¸¹ß  '        ; /bin/echo
	${EXE_FEPSTAT} KFT12 '±ÝÀ¶°áÁ¦¿ø ½Ç½Ã°£Áö·Î-Å¸¹ß(09~23)'   ; /bin/echo
	${EXE_FEPSTAT} KFT13 '±ÝÀ¶°áÁ¦¿ø Æß¹ðÅ·(BC)'               ; /bin/echo

	# ${EXE_FEPSTAT} NCE01 'NICE       Á¡¿ÜCD             '      ; /bin/echo   # ±Ý°á¿ø CD/ATM»ç¿ë
	# ${EXE_FEPSTAT} NCE02 '½Å)NICE    ½Å¿ëÁ¤º¸Á¶È¸       '      ; /bin/echo   # ¿øº¹
	# ${EXE_FEPSTAT} NCE03 '½Å)NICE    Á¶±â°æº¸Á¶È¸       '      ; /bin/echo   # ¿øº¹
	# ${EXE_FEPSTAT} NCE04 '½Å)NICE    Á¶±â°æº¸µî·Ï       '      ; /bin/echo   # ¿øº¹
	# ${EXE_FEPSTAT} NCE05 '½Å)NICE    CB SCOREÁ¶È¸       '      ; /bin/echo   # ¿øº¹

	${EXE_FEPSTAT} HNT01 'ÇÑ³×Æ®     Á¡¿ÜCD             '      ; /bin/echo

	${EXE_FEPSTAT} GBK01 'ÈÑ¹Ì¸®, BGFÄ³½¬³Ý Á¡¿ÜCD      '      ; /bin/echo   # ÈÑ¹Ì¸®, BGFÄ³½¬³Ý
	#${EXE_FEPSTAT} GBK01 '°ÔÀÌÆ®¹ðÅ© Á¡¿ÜCD             '      ; /bin/echo

	${EXE_FEPSTAT} HYS01 'È¿¼º       Á¡¿ÜCD (080-920-2220) '   ; /bin/echo

	${EXE_FEPSTAT} CHO01 'Ã»È£       Á¡¿ÜCD             '      ; /bin/echo

	${EXE_FEPSTAT} KBK01 'KISBANK    Á¡¿ÜCD             '      ; /bin/echo

	${EXE_FEPSTAT} LOT01 '·Ôµ¥       Á¡¿ÜCD             '      ; /bin/echo

	${EXE_FEPSTAT} SSN01 '»ï¼º³×Æ®¿÷ Æß¹ðÅ·             '      ; /bin/echo

	${EXE_FEPSTAT} KSN01 'KS-NET     Æß¹ðÅ·             '      ; /bin/echo

	${EXE_FEPSTAT} KSN02 'KS-NET     Æß¹ðÅ·-ÀÚ±ÝÆÀ      '      ; /bin/echo

	${EXE_FEPSTAT} LGU01 'LGÀ¯ÇÃ·¯½º PG (ÇÊ¿ä½ÃÁ¢¼Ó)    '      ; /bin/echo

	${EXE_FEPSTAT} TAT01 '¾ÅÅ©¿¡ÀÌÆ¼ 2Ã¤³ÎÀÎÁõ          '      ; /bin/echo

	/bin/echo
	/bin/echo
#	/bin/echo "[44m##### ¹èÄ¡ #####[0m"
#	/bin/echo ${ECHO_TTL}
#	# /bin/echo ${ECHO_BAR}
#	/bin/echo

#	${EXE_FEPSTAT} SHC51 '½ÅÇÑÄ«µå ÀÏ°ý¹èÄ¡             '      ; /bin/echo
#
#	${EXE_FEPSTAT} HDC52 'Çö´ëÄ«µå ÀÏ°ý¹èÄ¡             '      ; /bin/echo
#
#	${EXE_FEPSTAT} LTC51 '·Ôµ¥Ä«µå ÀÏ°ý¹èÄ¡¼ö½Å         '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSC51 '»ï¼ºÄ«µå ÀÏ°ý¹èÄ¡¼ö½Å         '      ; /bin/echo   # MegaBox

#	${EXE_FEPSTAT} CTC51 '½ÃÆ¼Ä«µå ÀÏ°ý¹èÄ¡¼ö½Å         '      ; /bin/echo
#
#	${EXE_FEPSTAT} KFT51 '±ÝÀ¶°áÁ¦¿ø ÀÏ°ýÀü¼Û           '      ; /bin/echo
#	${EXE_FEPSTAT} KFT52 '±ÝÀ¶°áÁ¦¿ø CMS                '      ; /bin/echo
#	${EXE_FEPSTAT} KFT53 '½Å)±ÝÀ¶°áÁ¦¿ø CMS(ÇÑÈ­Áõ±Ç)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT54 '½Å)±ÝÀ¶°áÁ¦¿ø CMS(ÇÁ·çµ§¼È)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT55 '½Å)±ÝÀ¶°áÁ¦¿ø ÀÏ°ýÀü¼Û(Áö·Î)  '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSN51 '»ï¼º³×Æ®¿÷ ÀÏ°ý¹èÄ¡           '      ; /bin/echo
#
#	${EXE_FEPSTAT} KSN51 'KS-NET     ÀÏ°ý¹èÄ¡           '      ; /bin/echo


	#/bin/echo
	#/bin/echo
	#/bin/echo "[44m##### ¹æÄ« #####[0m"
	#/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	#/bin/echo

	# ${EXE_FEPSTAT} HWI01 '½Å)ÇÑÈ­»ý¸í ¿¬°è              '      ; /bin/echo
	# ${EXE_FEPSTAT} HWI51 '½Å)ÇÑÈ­»ý¸í ÀÏ°ý¹èÄ¡          '      ; /bin/echo

	# ${EXE_FEPSTAT} SHI01 '½Å)½ÅÇÑ»ý¸í ¿¬°è              '      ; /bin/echo
	# ${EXE_FEPSTAT} SHI51 '½Å)½ÅÇÑ»ý¸í ÀÏ°ý¹èÄ¡          '      ; /bin/echo

	# ${EXE_FEPSTAT} HNI01 '½Å)ÇÏ³ªHSBC»ý¸í ¿¬°è          '      ; /bin/echo
	# ${EXE_FEPSTAT} HNI51 '½Å)ÇÏ³ªHSBC»ý¸í ÀÏ°ý¹èÄ¡      '      ; /bin/echo

	# ${EXE_FEPSTAT} HKI01 '½Å)Èï±¹»ý¸í ¿¬°è              '      ; /bin/echo
	# ${EXE_FEPSTAT} HKI51 '½Å)Èï±¹»ý¸í ÀÏ°ý¹èÄ¡          '      ; /bin/echo

	# ${EXE_FEPSTAT} KFI01 '½Å)Ä«µðÇÁ»ý¸í ¿¬°è            '      ; /bin/echo
	# ${EXE_FEPSTAT} KFI51 '½Å)Ä«µðÇÁ»ý¸í ÀÏ°ý¹èÄ¡        '      ; /bin/echo

	# ${EXE_FEPSTAT} ALI01 '½Å)¾Ë¸®¾ÈÃ÷»ý¸í ¿¬°è          '      ; /bin/echo
	# ${EXE_FEPSTAT} ALI51 '½Å)¾Ë¸®¾ÈÃ÷»ý¸í ÀÏ°ý¹èÄ¡      '      ; /bin/echo

	# ${EXE_FEPSTAT} SSI01 '½Å)»ï¼º»ý¸í ¿¬°è              '      ; /bin/echo
	# ${EXE_FEPSTAT} SSI51 '½Å)»ï¼º»ý¸í ÀÏ°ý¹èÄ¡          '      ; /bin/echo
	#/bin/echo "[44m##### AP1 : ¸ÅÆÛ #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA
	#/bin/echo "[44m##### AP2 : ¸ÅÆÛ #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA


	/bin/echo
	/bin/echo "½ÇÇà½Ã°£ : ${DATETIME}"
	/bin/echo
}

#-------------------------------------------------
EXE_RM=/bin/rm

DeleteFepFile()
{
	${EXE_RM} -rf ${LST_PS}
	${EXE_RM} -rf ${LST_NETSTAT}
}

#-------------------------------------------------

FepStatLoop()
{
	/bin/echo ${FEPID}

	while ( true )
	do
		CreateFepFile

		# ${EXE_FEPSTAT} ${FEPID} '<<<<<< FEPID=${FEPID} »óÅÂ >>>>>>'      ; /bin/echo
		${EXE_FEPSTAT} ${FEPID} "<<<<<< FEPID=${FEPID} »óÅÂ >>>>>>"      ; /bin/echo

		DeleteFepFile

		DATETIME=`/bin/date "+%Y/%m/%d %H:%M:%S"`
		/bin/echo "----( ${DATETIME} )-------------------------------------------"
		/bin/sleep 5
	done;
}

#-------------------------------------------------
#-------------------------------------------------
#-------------------------------------------------
#-------------------------------------------------

if [ $# = 1 ];
then
	FEPID=`/bin/echo ${1} | tr 'a-z' 'A-Z' `

	FepStatLoop
else
	CreateFepFile

	PrintFepInfo

	DeleteFepFile
fi

#-------------------------------------------------
