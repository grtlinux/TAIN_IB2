#!/bin/ksh

# ----------------------------------------------------------------------
# 기본변수 세팅
TODAY=`/bin/date +%Y%m%d`
TIME=`/bin/date`
# ----------------------------------------------------------------------


cd /hw03/ibridge/SEQ2/${TODAY}


########################################################################
# ----------------------------------------------------------------------
# 함수 : 송/수신건수 출력 (건수 차이나는 원인 함께 출력)
func_seqall()
{
	printf "\t\t${TIME}\n"
	printf "       ===========================================\n"
	printf "        FEPID 서비스명              RECV     SEND"
	#############################################################

	printf "\n[ KFT ]-------------------------------------------\n"

	INFOS="KFT01KFTC_타행환......\
				 KFT02KFTC_CD/ATM......\
				 KFT03KFTC_전자금융....\
				 KFT04KFTC_대고객서비스\
				 KFT05KFTC_인터넷지로..\
				 KFT06KFTC_실시간지로..\
				 KFT07KFTC_CMS부가.....\
				 KFT08KFTC_PG..........\
				 KFT09KFTC_CMS부가_한화\
				 KFT10KFTC_CMS부가_프루\
				 KFT11KFTC_인터넷지로타\
				 KFT12KFTC_실시간지로타\
				 KFT13KFTC_BC카드펌뱅킹"

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
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*건:내용작성)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ 은행 ]------------------------------------------\n"

	INFOS="WRB01우리은행_이체....\
				 WRB02우리은행_서비스..\
				 WRB03우리은행_자동이체\
				 WRB04우리은행_보통예금\
				 WRB05우리은행_자금집금\
				 WRB06우리은행_RealTIME\
				 WRB07우리은행_퇴직연금\
				 WRB08우리은행_가상계좌\
				 SHB01신한은행_펌뱅킹..\
				 SCB01SC은행_이체......\
				 TKB01대구은행_이체....\
				 PSB01부산은행_이체....\
				 KBB01국민은행_연계....\
				 NHB01농협_연계........\
				 NHB02농협_가상........\
				 NHB03농협_예수금......\
				 EXB01외환은행_연계....\
				 EXB02외환은행_환율....\
				 HNB01하나은행_이체....\
				 HNB02하나은행_연계....\
				 POS01우체국_제휴......\
				 GUB01기업은행_연계...."

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
				#"KBB01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (12건:전문이상)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"NHB02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1건:개시전문 무응답)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ 카드 ]------------------------------------------\n"

	INFOS="SHC01신한카드_온라인TR\
				 SHC02신한카드_BL......\
				 SHC03신한카드_신청....\
				 SHC04신한카드_펌뱅킹..\
				 HDC01현대카드_승인....\
				 HDC02현대카드_발급....\
				 HDC03현대카드_BL......\
				 HDC04현대카드_CMA신용.\
				 SSC01삼성카드_CMA.....\
				 LTC01롯데카드_신청접수\
				 LTC02롯데카드_BL......\
				 CTC01시티카드........."

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
				#"SHC02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (19건:업체쪽 전문이상)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*건:내용작성)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))

	done


	#############################################################
	printf "[점외CD]------------------------------------------\n"

	INFOS="GBK01BGF캐쉬넷_점외CD.\
				 HNT01한네트_점외CD....\
				 HYS01효성_점외CD......\
				 KBK01KISBANK_점외CD...\
				 CHO01청호_점외CD......\
				 LOT01롯데_점외CD......"

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
				#"KBK01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1건:업체쪽 미개발조회전문)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*건:내용작성)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ 기타 ]------------------------------------------\n"

	INFOS="DHI01한화생명_신용대출\
				 DHI02한화생명_ATM.....\
				 DHI03한화생명_자동납입\
				 TAT01ThinkAT_2채널ARS.\
				 KIS01나이스...........\
				 KIS02나이스_휴대폰인증\
				 KSN01KS-NET펌뱅킹.....\
				 KSN02KS-NET펌뱅킹-자금\
				 SSN01삼성네트웍_펌뱅킹\
				 LGU01LG유플러스_PG...."

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
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*건:내용작성)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*건:내용작성)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

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
# 함수 : 사용방법
func_use()
{
	echo "USE : ${0} [ SEQ | ALL ]"
	echo "    기본출력 : SEQ"
	echo "    상세출력 : ALL"
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
