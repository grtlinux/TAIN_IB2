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
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
	# WEBĳ��

	FEPID=$1
	DESC=$2
	WARN=$3

	if [ "AP" = "${SVRNM}" ]
	then
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
		/bin/echo "[44m--- �� DAT (${FEPID} : ${DESC}) -----------------------------[0m"
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
# 1. �¶��� : ����

fileps2 KBB01 "����:�������� ����"
fileps2 SHB01 "����:�������� �߹�ŷ"
fileps2 WRB01 "����:�츮���� ��ü"
fileps2 WRB02 "����:�츮���� ����"
fileps2 WRB03 "����:�츮���� �ڵ���ü"
fileps2 WRB04 "����:�츮���� ���뿹��"
fileps2 WRB05 "����:�츮���� �ڱ�����"
fileps2 WRB06 "����:�츮���� �ǽð�"
fileps2 WRB07 "����:�츮���� ��������"
fileps2 WRB08 "����:�츮���� �������"
fileps2 TKB01 "����:�뱸���� ��ü"
fileps2 EXB01 "����:��ȯ���� ����"
fileps2 EXB02 "����:��ȯ���� ȯ����������"
fileps2 HNB01 "����:�ϳ����� ����"
fileps2 HNB02 "����:�ϳ����� �ֹ����ű�"
#fileps2 HNB03 "����:�ϳ����� ��ü"
fileps2 PSB01 "����:�λ����� ��ü"
fileps2 POS01 "����:�� ü �� ����"
fileps2 NHB01 "����:��    �� ����"
fileps2 NHB02 "����:��    �� ����(��ü)"
fileps2 NHB03 "����:��    �� ������"
fileps2 GUB01 "����:������� ����"
fileps2 SCB01 "����:SC�������� ��ü"                       "��ȣȭ ����"

# 2. �¶��� : ī��

fileps2 SHC01 "ī��:����ī�� �¶���TR"
fileps2 SHC02 "ī��:����ī�� �¶���BL"
fileps2 SHC03 "ī��:����ī�� ��û"
fileps2 SHC04 "ī��:����ī�� �߹�ŷ"
fileps2 HDC01 "ī��:����ī�� ���������"
fileps2 HDC02 "ī��:����ī�� �߱�/������"
fileps2 HDC03 "ī��:����ī�� BL"
fileps2 HDC04 "ī��:����ī�� CMA�ſ�ī��"
fileps2 LTC01 "ī��:�Ե�ī�� ��û/������û"
fileps2 LTC02 "ī��:�Ե�ī�� BL"
fileps2 SSC01 "ī��:�Ｚī�� CMA�ſ�ī��"
fileps2 CTC01 "ī��:��Ƽī�� ����"

# 3. �¶��� : ��Ÿ

fileps2 DHI01 "��Ÿ:����  ���� �ſ����"
fileps2 DHI02 "��Ÿ:����  ���� ATM��Ǯ��"
fileps2 DHI03 "��Ÿ:����  ���� �ڵ�����"
fileps2 KIS01 "��Ÿ:��  ��  �� ����"
fileps2 KFT01 "��Ÿ:���������� Ÿ��ȯ"                     "��ð�(09:00~19:00)"
fileps2 KFT02 "��Ÿ:���������� CD/ATM"
fileps2 KFT03 "��Ÿ:���������� ���ڱ���"
fileps2 KFT04 "��Ÿ:���������� �������(ARS)"
fileps2 KFT05 "��Ÿ:���������� ���ͳ�����"
fileps2 KFT06 "��Ÿ:���������� �ǽð�����"                 "��ð�(09:00~19:00)"
fileps2 KFT07 "��Ÿ:���������� CMS�ΰ�����"
fileps2 KFT08 "��Ÿ:���������� PG"
fileps2 KFT09 "��Ÿ:���������� CMS�ΰ�����(��ȭ����)"
fileps2 KFT10 "��Ÿ:���������� CMS�ΰ�����(���絧��)"
fileps2 KFT11 "��Ÿ:���������� ���ͳ�����(Ÿ��)"
fileps2 KFT12 "��Ÿ:���������� �ǽð�����(Ÿ��)"           "��ð�(09:00~19:00)"
fileps2 HNT01 "��Ÿ:��  ��  Ʈ ����CD"
fileps2 GBK01 "��Ÿ:����ũ��ũ ����CD"
fileps2 HYS01 "��Ÿ:ȿ      �� ����CD"
fileps2 CHO01 "��Ÿ:û      ȣ ����CD"
fileps2 KBK01 "��Ÿ:KIS   BANK ����CD"
fileps2 LOT01 "��Ÿ:��      �� ����CD"
fileps2 SSN01 "��Ÿ:�Ｚ��Ʈ�� �߹�ŷ"
fileps2 KSN01 "��Ÿ:KS  -  NET �߹�ŷ"
fileps2 LGU01 "��Ÿ:LG���÷��� PG"                         "�ʿ�ø� ����"
fileps2 TAT01 "��Ÿ:��ũ����Ƽ 2ä�� ����"

# 4. ��ġ

#fileps1 HDC51 "ī��:����ī�� �ϰ���ġ     (WEBĳ��)"       "WEBĳ��"
#fileps2 SSC51 "ī��:�Ｚī�� �ϰ���ġ���� (WEBĳ��)"       "WEBĳ��"
#fileps1 NHC51 "ī��:����ī�� �ϰ���ġ���� (WEBĳ��)"       "WEBĳ��"
#fileps1 BCC51 "ī��:B C ī�� ���μ���     (WEBĳ��)"       "WEBĳ��"
#fileps1 KBC51 "ī��:����ī�� ���μ���     (WEBĳ��)"       "WEBĳ��"
#fileps1 SSC52 "ī��:�Ｚī�� ������������ (WEBĳ��)"       "WEBĳ��"

fileps2 HDC52 "ī��:����ī�� �ϰ���ġ"    
fileps2 CTC51 "ī��:��Ƽī�� �ϰ���ġ����"
fileps2 SHC51 "ī��:����ī�� �ϰ���ġ"
fileps2 LTC51 "ī��:�Ե�ī�� �ϰ���ġ����"
fileps2 SSN51 "��Ÿ:�Ｚ��Ʈ�� �ϰ���ġ"
fileps2 KSN51 "��Ÿ:KS  -  NET �ϰ���ġ"

fileps2 KFT51 "��Ÿ:���������� �ϰ�����"
fileps2 KFT52 "��Ÿ:���������� CMS"
fileps2 KFT53 "��Ÿ:���������� CMS(��ȭ����)"
fileps2 KFT54 "��Ÿ:���������� CMD(���絧��)"
fileps2 KFT55 "��Ÿ:���������� �ϰ�����(����)"


