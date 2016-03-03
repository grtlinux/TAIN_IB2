#!/bin/ksh

# ----------------------------------------------------------------------
# 기본변수 세팅

HOSTNAME=`hostname`
DATE=`date "+%Y%m%d"`
TIME=`date "+%H%M%S"`
PROCESS=ps_man

case ${HOSTNAME} in
	"dbizap01"   ) SVRNM=AP;;
	"pbizap01"   ) SVRNM=AP;;
	"pbizap02"   ) SVRNM=AP;;
	"dfep03"     ) SVRNM=FEP;;
	"pfep03"     ) SVRNM=FEP;;
	*            ) SVRNM=NOTHING;;
esac

DAT1_PATH=/hw03/ibridge/DAT
SEQ1_PATH=/hw03/ibridge/SEQ
LOG1_PATH=/hw02/ibridge/

DAT2_PATH=/hw03/ibridge/DAT2
SEQ2_PATH=/hw03/ibridge/SEQ2
LOG2_PATH=/hw02/ibridge/LOG2

# ----------------------------------------------------------------------
# 기본변수 출력

echo "--------- 기본변수 출력 ------------------------------------------"
echo "HOSTNAME=[${HOSTNAME}]"
echo "PROCESS =[${PROCESS}]"
echo "SVRNM   =[${SVRNM}]"
echo "DATE    =[${DATE}]"
echo "TIME    =[${TIME}]"
date
echo "------------------------------------------------------------"
echo ""

#########################################################################
# 함수 : 모든 프로세스 종료
func_end()
{
#ps_man 종료
/hw01/ibridge/ib2/shl/ps_man.sh stop

#타행환
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0101
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0101
/hw01/ibridge/ib2/shl/ibend2 OISKFT0101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0101

#CD/ATM
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0201
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0201
/hw01/ibridge/ib2/shl/ibend2 OISKFT0201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0201

#전자금융
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0301
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0301
/hw01/ibridge/ib2/shl/ibend2 OISKFT0301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0301

#대고객서비스
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0401
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0401
/hw01/ibridge/ib2/shl/ibend2 OISKFT0401
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0401

#인터넷지로
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0501
/hw01/ibridge/ib2/shl/ibend2 OISKFT0501
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0501

#실시간지로
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0601
/hw01/ibridge/ib2/shl/ibend2 OISKFT0601
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0601

#CMS부가
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0701
/hw01/ibridge/ib2/shl/ibend2 OISKFT0701
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0701

#PG
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0801
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0801
/hw01/ibridge/ib2/shl/ibend2 OISKFT0801
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0801

#CMS부가_한화
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0901
/hw01/ibridge/ib2/shl/ibend2 OISKFT0901
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0901

#CMS부가_프루
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1001
/hw01/ibridge/ib2/shl/ibend2 OISKFT1001
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1001

#인터넷지로 타발
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1101
/hw01/ibridge/ib2/shl/ibend2 OISKFT1101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1101

#실시간지로 타발
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1201
/hw01/ibridge/ib2/shl/ibend2 OISKFT1201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1201

#펌뱅킹(BC)
/hw01/ibridge/ib2/shl/ibend2 OXSKFT1301
/hw01/ibridge/ib2/shl/ibend2 OXRKFT1301
/hw01/ibridge/ib2/shl/ibend2 OISKFT1301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1301

#우리은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0101
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0101
/hw01/ibridge/ib2/shl/ibend2 OISWRB0101
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0101

#우리은행 서비스
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0201
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0201
/hw01/ibridge/ib2/shl/ibend2 OISWRB0201
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0201

#우리은행 자동이체
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0301
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0301
/hw01/ibridge/ib2/shl/ibend2 OISWRB0301
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0301

#우리은행 보통예금
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0401
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0401
/hw01/ibridge/ib2/shl/ibend2 OISWRB0401
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0401

#우리은행 자금집금
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0501
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0501
/hw01/ibridge/ib2/shl/ibend2 OISWRB0501
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0501

#우리은행 RealTime
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0601
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0601
/hw01/ibridge/ib2/shl/ibend2 OISWRB0601
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0601

#우리은행 퇴직연금
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0701
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0701
/hw01/ibridge/ib2/shl/ibend2 OISWRB0701
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0701

#우리은행 가상계좌
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0801
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0801
/hw01/ibridge/ib2/shl/ibend2 OISWRB0801
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0801

#신한은행 펌뱅킹
/hw01/ibridge/ib2/shl/ibend2 OXSSHB0101
/hw01/ibridge/ib2/shl/ibend2 OXRSHB0101
/hw01/ibridge/ib2/shl/ibend2 OISSHB0101
/hw01/ibridge/ib2/shl/ibend2 OIRSHB0101

#SC은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXASCB0101
/hw01/ibridge/ib2/shl/ibend2 OISSCB0101
/hw01/ibridge/ib2/shl/ibend2 OIRSCB0101

#대구은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXSTKB0101
/hw01/ibridge/ib2/shl/ibend2 OXRTKB0101
/hw01/ibridge/ib2/shl/ibend2 OISTKB0101
/hw01/ibridge/ib2/shl/ibend2 OIRTKB0101

#부산은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXSPSB0101
/hw01/ibridge/ib2/shl/ibend2 OXRPSB0101
/hw01/ibridge/ib2/shl/ibend2 OISPSB0101
/hw01/ibridge/ib2/shl/ibend2 OIRPSB0101

#국민은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXSKBB0101
/hw01/ibridge/ib2/shl/ibend2 OXRKBB0101
/hw01/ibridge/ib2/shl/ibend2 OISKBB0101
/hw01/ibridge/ib2/shl/ibend2 OIRKBB0101

#농협 연계
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0101
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0101
/hw01/ibridge/ib2/shl/ibend2 OISNHB0101
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0101

#농협 가상
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0201
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0201
/hw01/ibridge/ib2/shl/ibend2 OISNHB0201
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0201

#농협 예수금
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0301
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0301
/hw01/ibridge/ib2/shl/ibend2 OISNHB0301
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0301

#외환은행 연계
/hw01/ibridge/ib2/shl/ibend2 OXSEXB0101
/hw01/ibridge/ib2/shl/ibend2 OXREXB0101
/hw01/ibridge/ib2/shl/ibend2 OISEXB0101
/hw01/ibridge/ib2/shl/ibend2 OIREXB0101

#외환은행 환율
/hw01/ibridge/ib2/shl/ibend2 OXSEXB0201
/hw01/ibridge/ib2/shl/ibend2 OXREXB0201
/hw01/ibridge/ib2/shl/ibend2 OISEXB0201
/hw01/ibridge/ib2/shl/ibend2 OIREXB0201

#하나은행 이체
/hw01/ibridge/ib2/shl/ibend2 OXSHNB0101
/hw01/ibridge/ib2/shl/ibend2 OXRHNB0101
/hw01/ibridge/ib2/shl/ibend2 OISHNB0101
/hw01/ibridge/ib2/shl/ibend2 OIRHNB0101

#하나은행 연계
/hw01/ibridge/ib2/shl/ibend2 OXSHNB0201
/hw01/ibridge/ib2/shl/ibend2 OXRHNB0201
/hw01/ibridge/ib2/shl/ibend2 OISHNB0201
/hw01/ibridge/ib2/shl/ibend2 OIRHNB0201

#우체국 연계
/hw01/ibridge/ib2/shl/ibend2 OXAPOS0101
/hw01/ibridge/ib2/shl/ibend2 OISPOS0101
/hw01/ibridge/ib2/shl/ibend2 OIRPOS0101

#기업은행 연계
/hw01/ibridge/ib2/shl/ibend2 OXSGUB0101
/hw01/ibridge/ib2/shl/ibend2 OXRGUB0101
/hw01/ibridge/ib2/shl/ibend2 OISGUB0101
/hw01/ibridge/ib2/shl/ibend2 OIRGUB0101

#신한카드 온라인TR
/hw01/ibridge/ib2/shl/ibend2 OXASHC0101
/hw01/ibridge/ib2/shl/ibend2 OISSHC0101
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0101

#신한카드 BL
/hw01/ibridge/ib2/shl/ibend2 OXASHC0201
/hw01/ibridge/ib2/shl/ibend2 OISSHC0201
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0201

#신한카드 신청
/hw01/ibridge/ib2/shl/ibend2 OXASHC0301
/hw01/ibridge/ib2/shl/ibend2 OISSHC0301
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0301

#신한카드 펌뱅킹
/hw01/ibridge/ib2/shl/ibend2 OXASHC0401
/hw01/ibridge/ib2/shl/ibend2 OISSHC0401
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0401

#현대카드 승인
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0101
/hw01/ibridge/ib2/shl/ibend2 OISHDC0101
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0101

#현대카드 발급
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0201
/hw01/ibridge/ib2/shl/ibend2 OISHDC0201
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0201

#현대카드 BL
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0301
/hw01/ibridge/ib2/shl/ibend2 OISHDC0301
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0301

#현대카드 CMA신용
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0401
/hw01/ibridge/ib2/shl/ibend2 OISHDC0401
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0401

#삼성카드 CMA
/hw01/ibridge/ib2/shl/ibend2 OXASSC0101
/hw01/ibridge/ib2/shl/ibend2 OISSSC0101
/hw01/ibridge/ib2/shl/ibend2 OIRSSC0101

#롯데카드 신청접수
/hw01/ibridge/ib2/shl/ibend2 OXALTC0101
/hw01/ibridge/ib2/shl/ibend2 OISLTC0101
/hw01/ibridge/ib2/shl/ibend2 OIRLTC0101

#롯데카드 BL
/hw01/ibridge/ib2/shl/ibend2 OXALTC0201
/hw01/ibridge/ib2/shl/ibend2 OISLTC0201
/hw01/ibridge/ib2/shl/ibend2 OIRLTC0201

#씨티카드
/hw01/ibridge/ib2/shl/ibend2 OXACTC0101
/hw01/ibridge/ib2/shl/ibend2 OISCTC0101
/hw01/ibridge/ib2/shl/ibend2 OIRCTC0101

#BFG캐쉬넷
/hw01/ibridge/ib2/shl/ibend2 OXSGBK0101
/hw01/ibridge/ib2/shl/ibend2 OXRGBK0101
/hw01/ibridge/ib2/shl/ibend2 OISGBK0101
/hw01/ibridge/ib2/shl/ibend2 OIRGBK0101

#한네트
/hw01/ibridge/ib2/shl/ibend2 OXSHNT0101
/hw01/ibridge/ib2/shl/ibend2 OXRHNT0101
/hw01/ibridge/ib2/shl/ibend2 OISHNT0101
/hw01/ibridge/ib2/shl/ibend2 OIRHNT0101

#효성
/hw01/ibridge/ib2/shl/ibend2 OXSHYS0101
/hw01/ibridge/ib2/shl/ibend2 OXRHYS0101
/hw01/ibridge/ib2/shl/ibend2 OISHYS0101
/hw01/ibridge/ib2/shl/ibend2 OIRHYS0101

#KISBANK
/hw01/ibridge/ib2/shl/ibend2 OXSKBK0101
/hw01/ibridge/ib2/shl/ibend2 OXRKBK0101
/hw01/ibridge/ib2/shl/ibend2 OISKBK0101
/hw01/ibridge/ib2/shl/ibend2 OIRKBK0101

#청호 점외CD
/hw01/ibridge/ib2/shl/ibend2 OXSCHO0101
/hw01/ibridge/ib2/shl/ibend2 OXRCHO0101
/hw01/ibridge/ib2/shl/ibend2 OISCHO0101
/hw01/ibridge/ib2/shl/ibend2 OIRCHO0101

#롯데 점외CD
/hw01/ibridge/ib2/shl/ibend2 OXSLOT0101
/hw01/ibridge/ib2/shl/ibend2 OXRLOT0101
/hw01/ibridge/ib2/shl/ibend2 OISLOT0101
/hw01/ibridge/ib2/shl/ibend2 OIRLOT0101

#한화생명 신용대출
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0101
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0101
/hw01/ibridge/ib2/shl/ibend2 OISDHI0101
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0101

#한화생명 ATM
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0201
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0201
/hw01/ibridge/ib2/shl/ibend2 OISDHI0201
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0201

#한화생명 자동납입
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0301
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0301
/hw01/ibridge/ib2/shl/ibend2 OISDHI0301
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0301

#ThinkAT_2채널
/hw01/ibridge/ib2/shl/ibend2 OXATAT0101
/hw01/ibridge/ib2/shl/ibend2 OISTAT0101
/hw01/ibridge/ib2/shl/ibend2 OIRTAT0101

# 나이스
/hw01/ibridge/ib2/shl/ibend2 OXAKIS0101
/hw01/ibridge/ib2/shl/ibend2 OISKIS0101
/hw01/ibridge/ib2/shl/ibend2 OIRKIS0101

# 나이스_휴대폰인증
/hw01/ibridge/ib2/shl/ibend2 OXAKIS0201
/hw01/ibridge/ib2/shl/ibend2 OISKIS0201
/hw01/ibridge/ib2/shl/ibend2 OIRKIS0201

# KS-NET 펌뱅킹
/hw01/ibridge/ib2/shl/ibend2 OXSKSN0101
/hw01/ibridge/ib2/shl/ibend2 OXRKSN0101
/hw01/ibridge/ib2/shl/ibend2 OISKSN0101
/hw01/ibridge/ib2/shl/ibend2 OIRKSN0101

# KS-NET 펌뱅킹-자금
/hw01/ibridge/ib2/shl/ibend2 OXSKSN0201
/hw01/ibridge/ib2/shl/ibend2 OXRKSN0201
/hw01/ibridge/ib2/shl/ibend2 OISKSN0201
/hw01/ibridge/ib2/shl/ibend2 OIRKSN0201

# 삼성네트웤 펌뱅킹
/hw01/ibridge/ib2/shl/ibend2 OXSSSN0101
/hw01/ibridge/ib2/shl/ibend2 OXRSSN0101
/hw01/ibridge/ib2/shl/ibend2 OISSSN0101
/hw01/ibridge/ib2/shl/ibend2 OIRSSN0101

# LG유플러스 PG
/hw01/ibridge/ib2/shl/ibend2 OXALGU0101
/hw01/ibridge/ib2/shl/ibend2 OISLGU0101
/hw01/ibridge/ib2/shl/ibend2 OIRLGU0101

# 신한카드 일괄정송
/hw01/ibridge/ib2/shl/ibend2 OISSHC5101
/hw01/ibridge/ib2/shl/ibend2 OIRSHC5101

# 현대카드 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISHDC5201
/hw01/ibridge/ib2/shl/ibend2 OIRHDC5201

# 삼성네트웤 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISSSN5101
/hw01/ibridge/ib2/shl/ibend2 OIRSSN5101

# KS-NET 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISKSN5101
/hw01/ibridge/ib2/shl/ibend2 OIRKSN5101

# 시티카드 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISCTC5101

# 롯데카드 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISLTC5101

# 금결원 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISKFT5101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5101

# 금결원 일괄전송
/hw01/ibridge/ib2/shl/ibend2 OISKFT5201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5201

# 금결원 CMS_한화
/hw01/ibridge/ib2/shl/ibend2 OISKFT5301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5301

# 금결원 CMS_프루
/hw01/ibridge/ib2/shl/ibend2 OISKFT5401
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5401

# 금결원 지로
/hw01/ibridge/ib2/shl/ibend2 OISKFT5501
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5501

# 배치
/hw01/ibridge/ib2/shl/ibend2 BXRLTC5101
/hw01/ibridge/ib2/shl/ibend2 BXRCTC5101
/hw01/ibridge/ib2/shl/ibend2 BXRHDC5201

# 배치

pid=`ps -ef | grep bxacom2 | grep -v vi | grep -v tail | grep -v grep | awk '{ print $2 }'`

if [ "$pid" = "" ];
then
            echo "bxacom2.sh is not running..."
else
    kill -9 $pid
    echo "프로세스 종료 [$pid]"
fi

#방카 온라인
/hw01/ibridge/vela/banca/shl/bancaOnline.sh stop
#방카 배치
/hw01/ibridge/vela/banca/shl/bancaBatch.sh stop
#관리WEB
/hw01/ibridge/webserver/xend
# 모니터링
/hw01/ibridge/ib2/shl/ibend2 IBNETD0000
#관리WEB - KANG
/hw01/ibridge/ib2/web/tomcat6/bin/shutdown.sh

}

#########################################################################
# 함수 : 모든 프로세스 시작 - 매니져로 실행
func_start()
{
#ps_man
/hw01/ibridge/ib2/shl/ps_man.sh start
#방카 온라인
/hw01/ibridge/vela/banca/shl/bancaOnline.sh start
#방카 배치
/hw01/ibridge/vela/banca/shl/bancaBatch.sh start
#관리WEB
/hw01/ibridge/webserver/xrun
#관리WEB - KANG
/hw01/ibridge/ib2/web/tomcat6/bin/startup.sh
}


#########################################################################
# 함수 : 사용방법
func_usage()
{
	echo "USAGE : ${0} [ START | STOP ]"
	echo "    전체실행 : START"
	echo "    전체종료 : STOP"
	echo ""
}

#########################################################################

# ----------------------------------------------------------------------
# Main Process

if [ $# -gt 0 ];
then
	# --- 대문자 변환
	CMD=`echo ${1} | tr 'a-z' 'A-Z'`
	
	case ${CMD} in
		"RUN" | "START" | "BEGIN" | "BEG" ) 
			func_start
			;;
		"KILL" | "STOP" | "END" | "FINISH" ) 
			func_end
			;;
		*       )
			func_usage
			;;
	esac
	
	ps -ef | grep ibridge | grep  -Ev "vi|tail|sshd|sleep|ksh|java|grep|ps|sftp"
else
	func_usage
fi
