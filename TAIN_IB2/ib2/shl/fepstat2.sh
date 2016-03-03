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
	# ${EXE_FEPSTAT} KFT03 '���������� ���ڱ���           '      ; ${EXE_ECHO}
	# return;

	/bin/echo
	/bin/echo "����ð� : ${DATETIME}"
	/bin/echo
	/bin/echo "[44m##### �¶��� : ���� #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} KBB01 '�������� ����                 '      ; /bin/echo

	${EXE_FEPSTAT} SHB01 '�������� �߹�ŷ               '      ; /bin/echo

	${EXE_FEPSTAT} WRB01 '�츮���� ��ü                 '      ; /bin/echo
	${EXE_FEPSTAT} WRB02 '�츮���� ����               '      ; /bin/echo
	${EXE_FEPSTAT} WRB03 '�츮���� �ڵ���ü             '      ; /bin/echo
	${EXE_FEPSTAT} WRB04 '�츮���� ���뿹��             '      ; /bin/echo
	${EXE_FEPSTAT} WRB05 '�츮���� �ڱ�����             '      ; /bin/echo
	${EXE_FEPSTAT} WRB06 '�츮���� RealTime             '      ; /bin/echo
	${EXE_FEPSTAT} WRB07 '�츮���� ��������             '      ; /bin/echo
	${EXE_FEPSTAT} WRB08 '�츮���� �������             '      ; /bin/echo
	# ${EXE_FEPSTAT} WRB09 'X)�츮���� ��������(�Ÿ�)     '      ; /bin/echo  # �̻������ ����
	# ${EXE_FEPSTAT} WRB10 'X)�츮���� ��������(������)   '      ; /bin/echo  # �̻������ ����

	${EXE_FEPSTAT} TKB01 '�뱸���� ��ü                 '      ; /bin/echo

	${EXE_FEPSTAT} EXB01 '��ȯ���� ����                 '      ; /bin/echo
	${EXE_FEPSTAT} EXB02 '��ȯ���� ȯ����������         '      ; /bin/echo

	${EXE_FEPSTAT} HNB01 '�ϳ����� ��ü                 '      ; /bin/echo
	${EXE_FEPSTAT} HNB02 '�ϳ����� ����                 '      ; /bin/echo
	#${EXE_FEPSTAT} HNB03 '�ϳ����� �ֹ����ű�           '      ; /bin/echo  #�̻������ ����

	${EXE_FEPSTAT} PSB01 '�λ����� ��ü(X.25)           '      ; /bin/echo

	${EXE_FEPSTAT} POS01 '��ü��   ����                 '      ; /bin/echo

	${EXE_FEPSTAT} NHB01 '����     ������(����)         '      ; /bin/echo
	${EXE_FEPSTAT} NHB02 '����     ����(��ü)           '      ; /bin/echo
	${EXE_FEPSTAT} NHB03 '����     ����(������)         '      ; /bin/echo

	${EXE_FEPSTAT} GUB01 '������� ����(AP01,AP02)      '      ; /bin/echo

	${EXE_FEPSTAT} SCB01 'SC�������� ��ü(Xecure.)      '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### �¶��� : ī�� #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} SHC01 '����ī�� �¶���TR             '      ; /bin/echo
	${EXE_FEPSTAT} SHC02 '����ī�� �¶���BL             '      ; /bin/echo
	${EXE_FEPSTAT} SHC03 '����ī�� ��û                 '      ; /bin/echo
	${EXE_FEPSTAT} SHC04 '����ī�� �߹�ŷ               '      ; /bin/echo

	${EXE_FEPSTAT} HDC01 '����ī�� ���������           '      ; /bin/echo
	${EXE_FEPSTAT} HDC02 '����ī�� �߱�/������        '      ; /bin/echo
	${EXE_FEPSTAT} HDC03 '����ī�� BL                   '      ; /bin/echo
	${EXE_FEPSTAT} HDC04 '����ī�� CMA�ſ�ī��          '      ; /bin/echo

	${EXE_FEPSTAT} LTC01 '�Ե�ī�� ��û/��������        '      ; /bin/echo
	${EXE_FEPSTAT} LTC02 '�Ե�ī�� BL                   '      ; /bin/echo

	${EXE_FEPSTAT} SSC01 '�Ｚī�� CMA�ſ�ī��          '      ; /bin/echo   # MegaBox
	# ${EXE_FEPSTAT} SSC51 '�Ｚī�� �ϰ���ġ����         '      ; /bin/echo   # MegaBox

	${EXE_FEPSTAT} CTC01 '��Ƽī�� ����                 '      ; /bin/echo

	/bin/echo
	/bin/echo
	/bin/echo "[44m##### �¶��� : ��Ÿ #####[0m"
	/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	/bin/echo

	${EXE_FEPSTAT} DHI01 '���ѻ���   �ſ����           '      ; /bin/echo
	${EXE_FEPSTAT} DHI02 '���ѻ���   ATM�����          '      ; /bin/echo
	${EXE_FEPSTAT} DHI03 '���ѻ���   �ڵ�����           '      ; /bin/echo

	${EXE_FEPSTAT} KIS01 '���̽�     ����               '      ; /bin/echo
	${EXE_FEPSTAT} KIS02 '���̽�     �޴�������         '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS02 '�ѽ���     ����溸��ȸ       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS03 '�ѽ���     ����溸���       '      ; /bin/echo
	# ${EXE_FEPSTAT} KIS04 '�ѽ���     CB SCORE��ȸ       '      ; /bin/echo

	${EXE_FEPSTAT} KFT01 '���������� Ÿ��ȯ (07~18)     '      ; /bin/echo
	${EXE_FEPSTAT} KFT02 '���������� CD/ATM             '      ; /bin/echo
	${EXE_FEPSTAT} KFT03 '���������� ���ڱ���           '      ; /bin/echo
	${EXE_FEPSTAT} KFT04 '���������� �������(ARS)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT05 '���������� ���ͳ�����         '      ; /bin/echo
	${EXE_FEPSTAT} KFT06 '���������� �ǽð����� (09~23) '      ; /bin/echo
	${EXE_FEPSTAT} KFT07 '���������� CMS�ΰ�����      '      ; /bin/echo
	${EXE_FEPSTAT} KFT08 '���������� PG                 '      ; /bin/echo
	${EXE_FEPSTAT} KFT09 '���������� CMS�ΰ�(��ȭ����)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT10 '���������� CMS�ΰ�(���絧��)  '      ; /bin/echo
	${EXE_FEPSTAT} KFT11 '���������� ���ͳ�����-Ÿ��  '        ; /bin/echo
	${EXE_FEPSTAT} KFT12 '���������� �ǽð�����-Ÿ��(09~23)'   ; /bin/echo
	${EXE_FEPSTAT} KFT13 '���������� �߹�ŷ(BC)'               ; /bin/echo

	# ${EXE_FEPSTAT} NCE01 'NICE       ����CD             '      ; /bin/echo   # �ݰ�� CD/ATM���
	# ${EXE_FEPSTAT} NCE02 '��)NICE    �ſ�������ȸ       '      ; /bin/echo   # ����
	# ${EXE_FEPSTAT} NCE03 '��)NICE    ����溸��ȸ       '      ; /bin/echo   # ����
	# ${EXE_FEPSTAT} NCE04 '��)NICE    ����溸���       '      ; /bin/echo   # ����
	# ${EXE_FEPSTAT} NCE05 '��)NICE    CB SCORE��ȸ       '      ; /bin/echo   # ����

	${EXE_FEPSTAT} HNT01 '�ѳ�Ʈ     ����CD             '      ; /bin/echo

	${EXE_FEPSTAT} GBK01 '�ѹ̸�, BGFĳ���� ����CD      '      ; /bin/echo   # �ѹ̸�, BGFĳ����
	#${EXE_FEPSTAT} GBK01 '����Ʈ��ũ ����CD             '      ; /bin/echo

	${EXE_FEPSTAT} HYS01 'ȿ��       ����CD (080-920-2220) '   ; /bin/echo

	${EXE_FEPSTAT} CHO01 'ûȣ       ����CD             '      ; /bin/echo

	${EXE_FEPSTAT} KBK01 'KISBANK    ����CD             '      ; /bin/echo

	${EXE_FEPSTAT} LOT01 '�Ե�       ����CD             '      ; /bin/echo

	${EXE_FEPSTAT} SSN01 '�Ｚ��Ʈ�� �߹�ŷ             '      ; /bin/echo

	${EXE_FEPSTAT} KSN01 'KS-NET     �߹�ŷ             '      ; /bin/echo

	${EXE_FEPSTAT} KSN02 'KS-NET     �߹�ŷ-�ڱ���      '      ; /bin/echo

	${EXE_FEPSTAT} LGU01 'LG���÷��� PG (�ʿ������)    '      ; /bin/echo

	${EXE_FEPSTAT} TAT01 '��ũ����Ƽ 2ä������          '      ; /bin/echo

	/bin/echo
	/bin/echo
#	/bin/echo "[44m##### ��ġ #####[0m"
#	/bin/echo ${ECHO_TTL}
#	# /bin/echo ${ECHO_BAR}
#	/bin/echo

#	${EXE_FEPSTAT} SHC51 '����ī�� �ϰ���ġ             '      ; /bin/echo
#
#	${EXE_FEPSTAT} HDC52 '����ī�� �ϰ���ġ             '      ; /bin/echo
#
#	${EXE_FEPSTAT} LTC51 '�Ե�ī�� �ϰ���ġ����         '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSC51 '�Ｚī�� �ϰ���ġ����         '      ; /bin/echo   # MegaBox

#	${EXE_FEPSTAT} CTC51 '��Ƽī�� �ϰ���ġ����         '      ; /bin/echo
#
#	${EXE_FEPSTAT} KFT51 '���������� �ϰ�����           '      ; /bin/echo
#	${EXE_FEPSTAT} KFT52 '���������� CMS                '      ; /bin/echo
#	${EXE_FEPSTAT} KFT53 '��)���������� CMS(��ȭ����)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT54 '��)���������� CMS(���絧��)   '      ; /bin/echo
#	${EXE_FEPSTAT} KFT55 '��)���������� �ϰ�����(����)  '      ; /bin/echo
#
#	${EXE_FEPSTAT} SSN51 '�Ｚ��Ʈ�� �ϰ���ġ           '      ; /bin/echo
#
#	${EXE_FEPSTAT} KSN51 'KS-NET     �ϰ���ġ           '      ; /bin/echo


	#/bin/echo
	#/bin/echo
	#/bin/echo "[44m##### ��ī #####[0m"
	#/bin/echo ${ECHO_TTL}
	# /bin/echo ${ECHO_BAR}
	#/bin/echo

	# ${EXE_FEPSTAT} HWI01 '��)��ȭ���� ����              '      ; /bin/echo
	# ${EXE_FEPSTAT} HWI51 '��)��ȭ���� �ϰ���ġ          '      ; /bin/echo

	# ${EXE_FEPSTAT} SHI01 '��)���ѻ��� ����              '      ; /bin/echo
	# ${EXE_FEPSTAT} SHI51 '��)���ѻ��� �ϰ���ġ          '      ; /bin/echo

	# ${EXE_FEPSTAT} HNI01 '��)�ϳ�HSBC���� ����          '      ; /bin/echo
	# ${EXE_FEPSTAT} HNI51 '��)�ϳ�HSBC���� �ϰ���ġ      '      ; /bin/echo

	# ${EXE_FEPSTAT} HKI01 '��)�ﱹ���� ����              '      ; /bin/echo
	# ${EXE_FEPSTAT} HKI51 '��)�ﱹ���� �ϰ���ġ          '      ; /bin/echo

	# ${EXE_FEPSTAT} KFI01 '��)ī�������� ����            '      ; /bin/echo
	# ${EXE_FEPSTAT} KFI51 '��)ī�������� �ϰ���ġ        '      ; /bin/echo

	# ${EXE_FEPSTAT} ALI01 '��)�˸��������� ����          '      ; /bin/echo
	# ${EXE_FEPSTAT} ALI51 '��)�˸��������� �ϰ���ġ      '      ; /bin/echo

	# ${EXE_FEPSTAT} SSI01 '��)�Ｚ���� ����              '      ; /bin/echo
	# ${EXE_FEPSTAT} SSI51 '��)�Ｚ���� �ϰ���ġ          '      ; /bin/echo
	#/bin/echo "[44m##### AP1 : ���� #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA
	#/bin/echo "[44m##### AP2 : ���� #####[0m"
	#telnet 172.30.222.163 41500 | /bin/grep MAA


	/bin/echo
	/bin/echo "����ð� : ${DATETIME}"
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

		# ${EXE_FEPSTAT} ${FEPID} '<<<<<< FEPID=${FEPID} ���� >>>>>>'      ; /bin/echo
		${EXE_FEPSTAT} ${FEPID} "<<<<<< FEPID=${FEPID} ���� >>>>>>"      ; /bin/echo

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
