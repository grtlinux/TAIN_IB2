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
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
	# WEB캐시

	FEPID=$1
	DESC=$2
	WARN=$3

	if [ "AP" = "${SVRNM}" ]
	then
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- ★ DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
# 1. 온라인 : 은행

fileps2 KBB01 "은행:국민은행 연계"
fileps2 SHB01 "은행:신한은행 펌뱅킹"
fileps2 WRB01 "은행:우리은행 이체"
fileps2 WRB02 "은행:우리은행 서비스"
fileps2 WRB03 "은행:우리은행 자동이체"
fileps2 WRB04 "은행:우리은행 보통예금"
fileps2 WRB05 "은행:우리은행 자금집금"
fileps2 WRB06 "은행:우리은행 실시간"
fileps2 WRB07 "은행:우리은행 퇴직연금"
fileps2 WRB08 "은행:우리은행 가상계좌"
fileps2 TKB01 "은행:대구은행 이체"
fileps2 EXB01 "은행:외환은행 연계"
fileps2 EXB02 "은행:외환은행 환율정보수신"
fileps2 HNB01 "은행:하나은행 연계"
fileps2 HNB02 "은행:하나은행 주문증거금"
#fileps2 HNB03 "은행:하나은행 이체"
fileps2 PSB01 "은행:부산은행 이체"
fileps2 POS01 "은행:우 체 국 제휴"
fileps2 NHB01 "은행:농    협 연계"
fileps2 NHB02 "은행:농    협 가상(이체)"
fileps2 NHB03 "은행:농    협 예수금"
fileps2 GUB01 "은행:기업은행 연계"
fileps2 SCB01 "은행:SC제일은행 이체"                       "암호화 적용"

# 2. 온라인 : 카드

fileps2 SHC01 "카드:신한카드 온라인TR"
fileps2 SHC02 "카드:신한카드 온라인BL"
fileps2 SHC03 "카드:신한카드 신청"
fileps2 SHC04 "카드:신한카드 펌뱅킹"
fileps2 HDC01 "카드:현대카드 승인입출금"
fileps2 HDC02 "카드:현대카드 발급/비번등록"
fileps2 HDC03 "카드:현대카드 BL"
fileps2 HDC04 "카드:현대카드 CMA신용카드"
fileps2 LTC01 "카드:롯데카드 신청/접수신청"
fileps2 LTC02 "카드:롯데카드 BL"
fileps2 SSC01 "카드:삼성카드 CMA신용카드"
fileps2 CTC01 "카드:시티카드 제휴"

# 3. 온라인 : 기타

fileps2 DHI01 "기타:대한  생명 신용대출"
fileps2 DHI02 "기타:대한  생명 ATM입풀금"
fileps2 DHI03 "기타:대한  생명 자동납입"
fileps2 KIS01 "기타:나  이  스 연계"
fileps2 KFT01 "기타:금융결제원 타행환"                     "운영시간(09:00~19:00)"
fileps2 KFT02 "기타:금융결제원 CD/ATM"
fileps2 KFT03 "기타:금융결제원 전자금융"
fileps2 KFT04 "기타:금융결제원 대고객서비스(ARS)"
fileps2 KFT05 "기타:금융결제원 인터넷지로"
fileps2 KFT06 "기타:금융결제원 실시간지로"                 "운영시간(09:00~19:00)"
fileps2 KFT07 "기타:금융결제원 CMS부가서비스"
fileps2 KFT08 "기타:금융결제원 PG"
fileps2 KFT09 "기타:금융결제원 CMS부가서비스(한화증권)"
fileps2 KFT10 "기타:금융결제원 CMS부가서비스(프루덴셜)"
fileps2 KFT11 "기타:금융결제원 인터넷지로(타발)"
fileps2 KFT12 "기타:금융결제원 실시간지로(타발)"           "운영시간(09:00~19:00)"
fileps2 HNT01 "기타:한  네  트 점외CD"
fileps2 GBK01 "기타:게이크뱅크 점외CD"
fileps2 HYS01 "기타:효      성 점외CD"
fileps2 CHO01 "기타:청      호 점외CD"
fileps2 KBK01 "기타:KIS   BANK 점외CD"
fileps2 LOT01 "기타:롯      데 점외CD"
fileps2 SSN01 "기타:삼성네트웍 펌뱅킹"
fileps2 KSN01 "기타:KS  -  NET 펌뱅킹"
fileps2 LGU01 "기타:LG유플러스 PG"                         "필요시만 접속"
fileps2 TAT01 "기타:싱크에이티 2채널 인증"

# 4. 배치

#fileps1 HDC51 "카드:현대카드 일괄배치     (WEB캐시)"       "WEB캐시"
#fileps2 SSC51 "카드:삼성카드 일괄배치수신 (WEB캐시)"       "WEB캐시"
#fileps1 NHC51 "카드:농협카드 일괄배치수신 (WEB캐시)"       "WEB캐시"
#fileps1 BCC51 "카드:B C 카드 승인수신     (WEB캐시)"       "WEB캐시"
#fileps1 KBC51 "카드:국민카드 승인수신     (WEB캐시)"       "WEB캐시"
#fileps1 SSC52 "카드:삼성카드 매입정보수신 (WEB캐시)"       "WEB캐시"

fileps2 HDC52 "카드:현대카드 일괄배치"    
fileps2 CTC51 "카드:시티카드 일괄배치수신"
fileps2 SHC51 "카드:신한카드 일괄배치"
fileps2 LTC51 "카드:롯데카드 일괄배치수신"
fileps2 SSN51 "기타:삼성네트웍 일괄배치"
fileps2 KSN51 "기타:KS  -  NET 일괄배치"

fileps2 KFT51 "기타:금융결제원 일괄전송"
fileps2 KFT52 "기타:금융결제원 CMS"
fileps2 KFT53 "기타:금융결제원 CMS(한화증권)"
fileps2 KFT54 "기타:금융결제원 CMD(프루덴셜)"
fileps2 KFT55 "기타:금융결제원 일괄전송(지로)"


