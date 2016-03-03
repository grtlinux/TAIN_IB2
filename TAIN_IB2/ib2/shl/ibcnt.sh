#!/bin/ksh

# ----------------------------------------------------------------------
# ±âº»º¯¼ö ¼¼ÆÃ
TODAY=`/bin/date +%Y%m%d`
TIME=`/bin/date`
# ----------------------------------------------------------------------


cd /hw03/ibridge/SEQ2/${TODAY}


########################################################################
# ----------------------------------------------------------------------
# ÇÔ¼ö : ¼Û/¼ö½Å°Ç¼ö Ãâ·Â (°Ç¼ö Â÷ÀÌ³ª´Â ¿øÀÎ ÇÔ²² Ãâ·Â)
func_seqall()
{
	printf "\t\t${TIME}\n"
	printf "       ===========================================\n"
	printf "        FEPID ¼­ºñ½º¸í              RECV     SEND"
	#############################################################

	printf "\n[ KFT ]-------------------------------------------\n"

	INFOS="KFT01KFTC_Å¸ÇàÈ¯......\
				 KFT02KFTC_CD/ATM......\
				 KFT03KFTC_ÀüÀÚ±ÝÀ¶....\
				 KFT04KFTC_´ë°í°´¼­ºñ½º\
				 KFT05KFTC_ÀÎÅÍ³ÝÁö·Î..\
				 KFT06KFTC_½Ç½Ã°£Áö·Î..\
				 KFT07KFTC_CMSºÎ°¡.....\
				 KFT08KFTC_PG..........\
				 KFT09KFTC_CMSºÎ°¡_ÇÑÈ­\
				 KFT10KFTC_CMSºÎ°¡_ÇÁ·ç\
				 KFT11KFTC_ÀÎÅÍ³ÝÁö·ÎÅ¸\
				 KFT12KFTC_½Ç½Ã°£Áö·ÎÅ¸\
				 KFT13KFTC_BCÄ«µåÆß¹ðÅ·"

	NUM=1

	for INFO in ${INFOS}
	do

		FEPID=`/usr/bin/expr substr ${INFO} 1 5`
		FEPNAME=`/usr/bin/expr substr ${INFO} 6 19`


		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01

		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

		printf "   %2d" ${NUM};

		if [ ${RecvSeq} -ne ${SendSeq} ]; then
			case ${FEPID} in
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*°Ç:³»¿ëÀÛ¼º)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ ÀºÇà ]------------------------------------------\n"

	INFOS="WRB01¿ì¸®ÀºÇà_ÀÌÃ¼....\
				 WRB02¿ì¸®ÀºÇà_¼­ºñ½º..\
				 WRB03¿ì¸®ÀºÇà_ÀÚµ¿ÀÌÃ¼\
				 WRB04¿ì¸®ÀºÇà_º¸Åë¿¹±Ý\
				 WRB05¿ì¸®ÀºÇà_ÀÚ±ÝÁý±Ý\
				 WRB06¿ì¸®ÀºÇà_RealTIME\
				 WRB07¿ì¸®ÀºÇà_ÅðÁ÷¿¬±Ý\
				 WRB08¿ì¸®ÀºÇà_°¡»ó°èÁÂ\
				 SHB01½ÅÇÑÀºÇà_Æß¹ðÅ·..\
				 SCB01SCÀºÇà_ÀÌÃ¼......\
				 TKB01´ë±¸ÀºÇà_ÀÌÃ¼....\
				 PSB01ºÎ»êÀºÇà_ÀÌÃ¼....\
				 KBB01±¹¹ÎÀºÇà_¿¬°è....\
				 NHB01³óÇù_¿¬°è........\
				 NHB02³óÇù_°¡»ó........\
				 NHB03³óÇù_¿¹¼ö±Ý......\
				 EXB01¿ÜÈ¯ÀºÇà_¿¬°è....\
				 EXB02¿ÜÈ¯ÀºÇà_È¯À²....\
				 HNB01ÇÏ³ªÀºÇà_ÀÌÃ¼....\
				 HNB02ÇÏ³ªÀºÇà_¿¬°è....\
				 POS01¿ìÃ¼±¹_Á¦ÈÞ......\
				 GUB01±â¾÷ÀºÇà_¿¬°è...."

	NUM=1

	for INFO in ${INFOS}
	do

		FEPID=`/usr/bin/expr substr ${INFO} 1 5`
		FEPNAME=`/usr/bin/expr substr ${INFO} 6 19`


		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01

		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

		printf "   %2d" ${NUM};

		if [ ${RecvSeq} -ne ${SendSeq} ]; then
			case ${FEPID} in
				#"KBB01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (12°Ç:Àü¹®ÀÌ»ó)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"NHB02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1°Ç:°³½ÃÀü¹® ¹«ÀÀ´ä)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ Ä«µå ]------------------------------------------\n"

	INFOS="SHC01½ÅÇÑÄ«µå_¿Â¶óÀÎTR\
				 SHC02½ÅÇÑÄ«µå_BL......\
				 SHC03½ÅÇÑÄ«µå_½ÅÃ»....\
				 SHC04½ÅÇÑÄ«µå_Æß¹ðÅ·..\
				 HDC01Çö´ëÄ«µå_½ÂÀÎ....\
				 HDC02Çö´ëÄ«µå_¹ß±Þ....\
				 HDC03Çö´ëÄ«µå_BL......\
				 HDC04Çö´ëÄ«µå_CMA½Å¿ë.\
				 SSC01»ï¼ºÄ«µå_CMA.....\
				 LTC01·Ôµ¥Ä«µå_½ÅÃ»Á¢¼ö\
				 LTC02·Ôµ¥Ä«µå_BL......\
				 CTC01½ÃÆ¼Ä«µå........."

	NUM=1

	for INFO in ${INFOS}
	do

		FEPID=`/usr/bin/expr substr ${INFO} 1 5`
		FEPNAME=`/usr/bin/expr substr ${INFO} 6 19`


		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01

		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

		printf "   %2d" ${NUM};

		if [ ${RecvSeq} -ne ${SendSeq} ]; then
			case ${FEPID} in
				#"SHC02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (19°Ç:¾÷Ã¼ÂÊ Àü¹®ÀÌ»ó)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*°Ç:³»¿ëÀÛ¼º)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))

	done


	#############################################################
	printf "[Á¡¿ÜCD]------------------------------------------\n"

	INFOS="GBK01BGFÄ³½¬³Ý_Á¡¿ÜCD.\
				 HNT01ÇÑ³×Æ®_Á¡¿ÜCD....\
				 HYS01È¿¼º_Á¡¿ÜCD......\
				 KBK01KISBANK_Á¡¿ÜCD...\
				 CHO01Ã»È£_Á¡¿ÜCD......\
				 LOT01·Ôµ¥_Á¡¿ÜCD......"

	NUM=1

	for INFO in ${INFOS}
	do

		FEPID=`/usr/bin/expr substr ${INFO} 1 5`
		FEPNAME=`/usr/bin/expr substr ${INFO} 6 19`


		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01

		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

		printf "   %2d" ${NUM};

		if [ ${RecvSeq} -ne ${SendSeq} ]; then
			case ${FEPID} in
				#"KBK01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1°Ç:¾÷Ã¼ÂÊ ¹Ì°³¹ßÁ¶È¸Àü¹®)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*°Ç:³»¿ëÀÛ¼º)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ ±âÅ¸ ]------------------------------------------\n"

	INFOS="DHI01ÇÑÈ­»ý¸í_½Å¿ë´ëÃâ\
				 DHI02ÇÑÈ­»ý¸í_ATM.....\
				 DHI03ÇÑÈ­»ý¸í_ÀÚµ¿³³ÀÔ\
				 TAT01ThinkAT_2Ã¤³ÎARS.\
				 KIS01³ªÀÌ½º...........\
				 KIS02³ªÀÌ½º_ÈÞ´ëÆùÀÎÁõ\
				 KSN01KS-NETÆß¹ðÅ·.....\
				 KSN02KS-NETÆß¹ðÅ·-ÀÚ±Ý\
				 SSN01»ï¼º³×Æ®¿÷_Æß¹ðÅ·\
				 LGU01LGÀ¯ÇÃ·¯½º_PG...."

	NUM=1

	for INFO in ${INFOS}
	do

		FEPID=`/usr/bin/expr substr ${INFO} 1 5`
		FEPNAME=`/usr/bin/expr substr ${INFO} 6 19`


		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01

		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

		printf "   %2d" ${NUM};

		if [ ${RecvSeq} -ne ${SendSeq} ]; then
			case ${FEPID} in
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*°Ç:³»¿ëÀÛ¼º)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*°Ç:³»¿ëÀÛ¼º)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done
	printf "       ===========================================\n"

}

# ----------------------------------------------------------------------
# ÇÔ¼ö : »ç¿ë¹æ¹ý
func_use()
{
	echo "USE : ${0} [ SEQ | ALL ]"
	echo "    ±âº»Ãâ·Â : SEQ"
	echo "    »ó¼¼Ãâ·Â : ALL"
	echo ""
}

########################################################################

# ----------------------------------------------------------------------
# Main Precess

if [ $# = 0 ];
then
	func_seqall

else
	func_use
fi
