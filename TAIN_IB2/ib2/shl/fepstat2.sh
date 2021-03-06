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
	# ${EXE_FEPSTAT} KFT03 '금융결제원 전자금융           '      ; ${EXE_ECHO}
	# return;

	/bin/echo
	/bin/echo "실행시간 : ${DATETIME}"
	/bin/echo
	/bin/echo "[44m##### 온라인 : 은행 #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} KBB01 '국민은행 연계                 '      ; /bin/echo

	${EXE_FEPSTAT} SHB01 '신한은행 펌뱅킹               '      ; /bin/echo

	${EXE_FEPSTAT} WRB01 '우리은행 이체                 '      ; /bin/echo
	${EXE_FEPSTAT} WRB02 '우리은행 서비스               '      ; /bin/echo
	${EXE_FEPSTAT} WRB03 '우리은행 자동이체             '      ; /bin/echo
	${EXE_FEPSTAT} WRB04 '우리은행 보통예금             '      ; /bin/echo
	${EXE_FEPSTAT} WRB05 '우리은행 자금집금             '      ; /bin/echo
	${EXE_FEPSTAT} WRB06 '우리은행 RealTime             '      ; /bin/echo
	${EXE_FEPSTAT} WRB07 '우리은행 퇴직연금             '      ; /bin/echo
	${EXE_FEPSTAT} WRB08 '우리은행 가상계좌             '      ; /bin/echo
	# ${EXE_FEPSTAT} WRB09 'X)우리은행 결제정보(매매)     '      ; /bin/echo  # 미사용으로 삭제
	# ${EXE_FEPSTAT} WRB10 'X)우리은행 결제정보(수수료)   '      ; /bin/echo  # 미사용으로 삭제

	${EXE_FEPSTAT} TKB01 '대구은행 이체                 '      ; /bin/echo

	${EXE_FEPSTAT} EXB01 '외환은행 연계                 '      ; /bin/echo
	${EXE_FEPSTAT} EXB02 '외환은행 환율정보수신         '      ; /bin/echo

	${EXE_FEPSTAT} HNB01 '하나은행 이체                 '      ; /bin/echo
	${EXE_FEPSTAT} HNB02 '하나은행 연계                 '      ; /bin/echo
	#${EXE_FEPSTAT} HNB03 '하나은행 주문증거금           '      ; /bin/echo  #미사용으로 삭제

	${EXE_FEPSTAT} PSB01 '부산은행 이체(X.25)           '      ; /bin/echo

	${EXE_FEPSTAT} POS01 '우체국   제휴                 '      ; /bin/echo

	${EXE_FEPSTAT} NHB01 '농협     예수금(연계)         '      ; /bin/echo
	${EXE_FEPSTAT} NHB02 '농협     가상(이체)           '      ; /bin/echo
	${EXE_FEPSTAT} NHB03 '농협     연계(예수금)         '      ; /bin/echo

	${EXE_FEPSTAT} GUB01 '기업은행 연계(AP01,AP02)      '      ; /bin/echo

	${EXE_FEPSTAT} SCB01 'SC제일은행 이체(Xecure.)      '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### 온라인 : 카드 #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} SHC01 '신한카드 온라인TR             '      ; /bin/echo
	${EXE_FEPSTAT} SHC02 '신한카드 온라인BL             '      ; /bin/echo
	${EXE_FEPSTAT} SHC03 '신한카드 신청                 '      ; /bin/echo
	${EXE_FEPSTAT} SHC04 '신한카드 펌뱅킹               '      ; /bin/echo

	${EXE_FEPSTAT} HDC01 '현대카드 승인입출금           '      ; /bin/echo
	${EXE_FEPSTAT} HDC02 '현대카드 발급/비번등록        '      ; /bin/echo
	${EXE_FEPSTAT} HDC03 '현대카드 BL                   '      ; /bin/echo
	${EXE_FEPSTAT} HDC04 '현대카드 CMA신용카드          '      ; /bin/echo

	${EXE_FEPSTAT} LTC01 '롯데카드 신청/접수진행        '      ; /bin/echo
	${EXE_FEPSTAT} LTC02 '롯데카드 BL                   '      ; /bin/echo

	${EXE_FEPSTAT} SSC01 '삼성카드 CMA신용카드          '      ; /bin/echo   # MegaBox
	# ${EXE_FEPSTAT} SSC51 '삼성카드 일괄배치수신         '      ; /bin/echo   # MegaBox

	${EXE_FEPSTAT} CTC01 '시티카드 제휴                 '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### 온라인 : 기타 #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} DHI01 '대한생명   신용대출           '      ; /bin/echo
	${EXE_FEPSTAT} DHI02 '대한생명   ATM입출금          '      ; /bin/echo
	${EXE_FEPSTAT} DHI03 '대한생명   자동납입           '      ; /bin/echo

	${EXE_FEPSTAT} KIS01 '나이스     연계               '      ; /bin/echo
	${EXE_FEPSTAT} KIS02 '나이스     휴대폰인증         '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS02 '한신평     조기경보조회       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS03 '한신평     조기경보등록       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS04 '한신평     CB SCORE조회       '      ; /bin/echo

	${EXE_FEPSTAT} KFT01 '금융결제원 타행환 (07~18)     '      ; /bin/echo
	${EXE_FEPSTAT} KFT02 '금융결제원 CD/ATM             '      ; /bin/echo
	${EXE_FEPSTAT} KFT03 '금융결제원 전자금융           '      ; /bin/echo
	${EXE_FEPSTAT} KFT04 '금융결제원 대고객서비스(ARS)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT05 '금융결제원 인터넷지로         '      ; /bin/echo
	${EXE_FEPSTAT} KFT06 '금융결제원 실시간지로 (09~23) '      ; /bin/echo
	${EXE_FEPSTAT} KFT07 '금융결제원 CMS부가서비스      '      ; /bin/echo
	${EXE_FEPSTAT} KFT08 '금융결제원 PG                 '      ; /bin/echo
	${EXE_FEPSTAT} KFT09 '금융결제원 CMS부가(한화증권)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT10 '금융결제원 CMS부가(프루덴셜)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT11 '금융결제원 인터넷지로-타발  '        ; /bin/echo
	${EXE_FEPSTAT} KFT12 '금융결제원 실시간지로-타발(09~23)'   ; /bin/echo
	${EXE_FEPSTAT} KFT13 '금융결제원 펌뱅킹(BC)'               ; /bin/echo

	# ${EXE_FEPSTAT} NCE01 'NICE       점외CD             '      ; /bin/echo   # 금결원 CD/ATM사용
	# ${EXE_FEPSTAT} NCE02 '신)NICE    신용정보조회       '      ; /bin/echo   # 원복
	# ${EXE_FEPSTAT} NCE03 '신)NICE    조기경보조회       '      ; /bin/echo   # 원복
	# ${EXE_FEPSTAT} NCE04 '신)NICE    조기경보등록       '      ; /bin/echo   # 원복
	# ${EXE_FEPSTAT} NCE05 '신)NICE    CB SCORE조회       '      ; /bin/echo   # 원복

	${EXE_FEPSTAT} HNT01 '한네트     점외CD             '      ; /bin/echo

	${EXE_FEPSTAT} GBK01 '훼미리, BGF캐쉬넷 점외CD      '      ; /bin/echo   # 훼미리, BGF캐쉬넷
	#${EXE_FEPSTAT} GBK01 '게이트뱅크 점외CD             '      ; /bin/echo

	${EXE_FEPSTAT} HYS01 '효성       점외CD (080-920-2220) '   ; /bin/echo

	${EXE_FEPSTAT} CHO01 '청호       점외CD             '      ; /bin/echo

	${EXE_FEPSTAT} KBK01 'KISBANK    점외CD             '      ; /bin/echo

	${EXE_FEPSTAT} LOT01 '롯데       점외CD             '      ; /bin/echo

	${EXE_FEPSTAT} SSN01 '삼성네트웍 펌뱅킹             '      ; /bin/echo

	${EXE_FEPSTAT} KSN01 'KS-NET     펌뱅킹             '      ; /bin/echo

	${EXE_FEPSTAT} KSN02 'KS-NET     펌뱅킹-자금팀      '      ; /bin/echo

	${EXE_FEPSTAT} LGU01 'LG유플러스 PG (필요시접속)    '      ; /bin/echo

	${EXE_FEPSTAT} TAT01 '씽크에이티 2채널인증          '      ; /bin/echo

	/bin/echo
	/bin/echo
#	/bin/echo "[44m##### 배치 #####[0m"
#	/bin/echo ${ECHO_TTL}
#	# /bin/echo ${ECHO_BAR}
#	/bin/echo

#	${EXE_FEPSTAT} SHC51 '신한카드 일괄배치             '      ; /bin/echo
#
#	${EXE_FEPSTAT} HDC52 '현대카드 일괄배치             '      ; /bin/echo
#
#	${EXE_FEPSTAT} LTC51 '롯데카드 일괄배치수신         '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSC51 '삼성카드 일괄배치수신         '      ; /bin/echo   # MegaBox

#	${EXE_FEPSTAT} CTC51 '시티카드 일괄배치수신         '      ; /bin/echo
#
#	${EXE_FEPSTAT} KFT51 '금융결제원 일괄전송           '      ; /bin/echo
#	${EXE_FEPSTAT} KFT52 '금융결제원 CMS                '      ; /bin/echo
#	${EXE_FEPSTAT} KFT53 '신)금융결제원 CMS(한화증권)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT54 '신)금융결제원 CMS(프루덴셜)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT55 '신)금융결제원 일괄전송(지로)  '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSN51 '삼성네트웍 일괄배치           '      ; /bin/echo
#
#	${EXE_FEPSTAT} KSN51 'KS-NET     일괄배치           '      ; /bin/echo


	#/bin/echo
	#/bin/echo
	#/bin/echo "[44m##### 방카 #####[0m"
	#/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	#/bin/echo

	# ${EXE_FEPSTAT} HWI01 '신)한화생명 연계              '      ; /bin/echo
	# ${EXE_FEPSTAT} HWI51 '신)한화생명 일괄배치          '      ; /bin/echo

	# ${EXE_FEPSTAT} SHI01 '신)신한생명 연계              '      ; /bin/echo
	# ${EXE_FEPSTAT} SHI51 '신)신한생명 일괄배치          '      ; /bin/echo

	# ${EXE_FEPSTAT} HNI01 '신)하나HSBC생명 연계          '      ; /bin/echo
	# ${EXE_FEPSTAT} HNI51 '신)하나HSBC생명 일괄배치      '      ; /bin/echo

	# ${EXE_FEPSTAT} HKI01 '신)흥국생명 연계              '      ; /bin/echo
	# ${EXE_FEPSTAT} HKI51 '신)흥국생명 일괄배치          '      ; /bin/echo

	# ${EXE_FEPSTAT} KFI01 '신)카디프생명 연계            '      ; /bin/echo
	# ${EXE_FEPSTAT} KFI51 '신)카디프생명 일괄배치        '      ; /bin/echo

	# ${EXE_FEPSTAT} ALI01 '신)알리안츠생명 연계          '      ; /bin/echo
	# ${EXE_FEPSTAT} ALI51 '신)알리안츠생명 일괄배치      '      ; /bin/echo

	# ${EXE_FEPSTAT} SSI01 '신)삼성생명 연계              '      ; /bin/echo
	# ${EXE_FEPSTAT} SSI51 '신)삼성생명 일괄배치          '      ; /bin/echo
	#/bin/echo "[44m##### AP1 : 매퍼 #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA
	#/bin/echo "[44m##### AP2 : 매퍼 #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA


	/bin/echo
	/bin/echo "실행시간 : ${DATETIME}"
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

		# ${EXE_FEPSTAT} ${FEPID} '<<<<<< FEPID=${FEPID} 상태 >>>>>>'      ; /bin/echo
		${EXE_FEPSTAT} ${FEPID} "<<<<<< FEPID=${FEPID} 상태 >>>>>>"      ; /bin/echo

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
