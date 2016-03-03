#!/bin/ksh

# ----------------------------------------------------------------------
# �⺻���� ����

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
# �⺻���� ���

echo "--------- �⺻���� ��� ------------------------------------------"
echo "HOSTNAME=[${HOSTNAME}]"
echo "PROCESS =[${PROCESS}]"
echo "SVRNM   =[${SVRNM}]"
echo "DATE    =[${DATE}]"
echo "TIME    =[${TIME}]"
date
echo "------------------------------------------------------------"
echo ""

#########################################################################
# �Լ� : ��� ���μ��� ����
func_end()
{
#ps_man ����
/hw01/ibridge/ib2/shl/ps_man.sh stop

#Ÿ��ȯ
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0101
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0101
/hw01/ibridge/ib2/shl/ibend2 OISKFT0101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0101

#CD/ATM
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0201
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0201
/hw01/ibridge/ib2/shl/ibend2 OISKFT0201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0201

#���ڱ���
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0301
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0301
/hw01/ibridge/ib2/shl/ibend2 OISKFT0301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0301

#�������
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0401
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0401
/hw01/ibridge/ib2/shl/ibend2 OISKFT0401
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0401

#���ͳ�����
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0501
/hw01/ibridge/ib2/shl/ibend2 OISKFT0501
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0501

#�ǽð�����
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0601
/hw01/ibridge/ib2/shl/ibend2 OISKFT0601
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0601

#CMS�ΰ�
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0701
/hw01/ibridge/ib2/shl/ibend2 OISKFT0701
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0701

#PG
/hw01/ibridge/ib2/shl/ibend2 OXSKFT0801
/hw01/ibridge/ib2/shl/ibend2 OXRKFT0801
/hw01/ibridge/ib2/shl/ibend2 OISKFT0801
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0801

#CMS�ΰ�_��ȭ
/hw01/ibridge/ib2/shl/ibend2 OXAKFT0901
/hw01/ibridge/ib2/shl/ibend2 OISKFT0901
/hw01/ibridge/ib2/shl/ibend2 OIRKFT0901

#CMS�ΰ�_����
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1001
/hw01/ibridge/ib2/shl/ibend2 OISKFT1001
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1001

#���ͳ����� Ÿ��
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1101
/hw01/ibridge/ib2/shl/ibend2 OISKFT1101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1101

#�ǽð����� Ÿ��
/hw01/ibridge/ib2/shl/ibend2 OXAKFT1201
/hw01/ibridge/ib2/shl/ibend2 OISKFT1201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1201

#�߹�ŷ(BC)
/hw01/ibridge/ib2/shl/ibend2 OXSKFT1301
/hw01/ibridge/ib2/shl/ibend2 OXRKFT1301
/hw01/ibridge/ib2/shl/ibend2 OISKFT1301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT1301

#�츮���� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0101
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0101
/hw01/ibridge/ib2/shl/ibend2 OISWRB0101
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0101

#�츮���� ����
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0201
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0201
/hw01/ibridge/ib2/shl/ibend2 OISWRB0201
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0201

#�츮���� �ڵ���ü
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0301
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0301
/hw01/ibridge/ib2/shl/ibend2 OISWRB0301
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0301

#�츮���� ���뿹��
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0401
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0401
/hw01/ibridge/ib2/shl/ibend2 OISWRB0401
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0401

#�츮���� �ڱ�����
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0501
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0501
/hw01/ibridge/ib2/shl/ibend2 OISWRB0501
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0501

#�츮���� RealTime
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0601
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0601
/hw01/ibridge/ib2/shl/ibend2 OISWRB0601
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0601

#�츮���� ��������
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0701
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0701
/hw01/ibridge/ib2/shl/ibend2 OISWRB0701
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0701

#�츮���� �������
/hw01/ibridge/ib2/shl/ibend2 OXSWRB0801
/hw01/ibridge/ib2/shl/ibend2 OXRWRB0801
/hw01/ibridge/ib2/shl/ibend2 OISWRB0801
/hw01/ibridge/ib2/shl/ibend2 OIRWRB0801

#�������� �߹�ŷ
/hw01/ibridge/ib2/shl/ibend2 OXSSHB0101
/hw01/ibridge/ib2/shl/ibend2 OXRSHB0101
/hw01/ibridge/ib2/shl/ibend2 OISSHB0101
/hw01/ibridge/ib2/shl/ibend2 OIRSHB0101

#SC���� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXASCB0101
/hw01/ibridge/ib2/shl/ibend2 OISSCB0101
/hw01/ibridge/ib2/shl/ibend2 OIRSCB0101

#�뱸���� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXSTKB0101
/hw01/ibridge/ib2/shl/ibend2 OXRTKB0101
/hw01/ibridge/ib2/shl/ibend2 OISTKB0101
/hw01/ibridge/ib2/shl/ibend2 OIRTKB0101

#�λ����� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXSPSB0101
/hw01/ibridge/ib2/shl/ibend2 OXRPSB0101
/hw01/ibridge/ib2/shl/ibend2 OISPSB0101
/hw01/ibridge/ib2/shl/ibend2 OIRPSB0101

#�������� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXSKBB0101
/hw01/ibridge/ib2/shl/ibend2 OXRKBB0101
/hw01/ibridge/ib2/shl/ibend2 OISKBB0101
/hw01/ibridge/ib2/shl/ibend2 OIRKBB0101

#���� ����
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0101
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0101
/hw01/ibridge/ib2/shl/ibend2 OISNHB0101
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0101

#���� ����
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0201
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0201
/hw01/ibridge/ib2/shl/ibend2 OISNHB0201
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0201

#���� ������
/hw01/ibridge/ib2/shl/ibend2 OXSNHB0301
/hw01/ibridge/ib2/shl/ibend2 OXRNHB0301
/hw01/ibridge/ib2/shl/ibend2 OISNHB0301
/hw01/ibridge/ib2/shl/ibend2 OIRNHB0301

#��ȯ���� ����
/hw01/ibridge/ib2/shl/ibend2 OXSEXB0101
/hw01/ibridge/ib2/shl/ibend2 OXREXB0101
/hw01/ibridge/ib2/shl/ibend2 OISEXB0101
/hw01/ibridge/ib2/shl/ibend2 OIREXB0101

#��ȯ���� ȯ��
/hw01/ibridge/ib2/shl/ibend2 OXSEXB0201
/hw01/ibridge/ib2/shl/ibend2 OXREXB0201
/hw01/ibridge/ib2/shl/ibend2 OISEXB0201
/hw01/ibridge/ib2/shl/ibend2 OIREXB0201

#�ϳ����� ��ü
/hw01/ibridge/ib2/shl/ibend2 OXSHNB0101
/hw01/ibridge/ib2/shl/ibend2 OXRHNB0101
/hw01/ibridge/ib2/shl/ibend2 OISHNB0101
/hw01/ibridge/ib2/shl/ibend2 OIRHNB0101

#�ϳ����� ����
/hw01/ibridge/ib2/shl/ibend2 OXSHNB0201
/hw01/ibridge/ib2/shl/ibend2 OXRHNB0201
/hw01/ibridge/ib2/shl/ibend2 OISHNB0201
/hw01/ibridge/ib2/shl/ibend2 OIRHNB0201

#��ü�� ����
/hw01/ibridge/ib2/shl/ibend2 OXAPOS0101
/hw01/ibridge/ib2/shl/ibend2 OISPOS0101
/hw01/ibridge/ib2/shl/ibend2 OIRPOS0101

#������� ����
/hw01/ibridge/ib2/shl/ibend2 OXSGUB0101
/hw01/ibridge/ib2/shl/ibend2 OXRGUB0101
/hw01/ibridge/ib2/shl/ibend2 OISGUB0101
/hw01/ibridge/ib2/shl/ibend2 OIRGUB0101

#����ī�� �¶���TR
/hw01/ibridge/ib2/shl/ibend2 OXASHC0101
/hw01/ibridge/ib2/shl/ibend2 OISSHC0101
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0101

#����ī�� BL
/hw01/ibridge/ib2/shl/ibend2 OXASHC0201
/hw01/ibridge/ib2/shl/ibend2 OISSHC0201
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0201

#����ī�� ��û
/hw01/ibridge/ib2/shl/ibend2 OXASHC0301
/hw01/ibridge/ib2/shl/ibend2 OISSHC0301
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0301

#����ī�� �߹�ŷ
/hw01/ibridge/ib2/shl/ibend2 OXASHC0401
/hw01/ibridge/ib2/shl/ibend2 OISSHC0401
/hw01/ibridge/ib2/shl/ibend2 OIRSHC0401

#����ī�� ����
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0101
/hw01/ibridge/ib2/shl/ibend2 OISHDC0101
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0101

#����ī�� �߱�
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0201
/hw01/ibridge/ib2/shl/ibend2 OISHDC0201
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0201

#����ī�� BL
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0301
/hw01/ibridge/ib2/shl/ibend2 OISHDC0301
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0301

#����ī�� CMA�ſ�
/hw01/ibridge/ib2/shl/ibend2 OXAHDC0401
/hw01/ibridge/ib2/shl/ibend2 OISHDC0401
/hw01/ibridge/ib2/shl/ibend2 OIRHDC0401

#�Ｚī�� CMA
/hw01/ibridge/ib2/shl/ibend2 OXASSC0101
/hw01/ibridge/ib2/shl/ibend2 OISSSC0101
/hw01/ibridge/ib2/shl/ibend2 OIRSSC0101

#�Ե�ī�� ��û����
/hw01/ibridge/ib2/shl/ibend2 OXALTC0101
/hw01/ibridge/ib2/shl/ibend2 OISLTC0101
/hw01/ibridge/ib2/shl/ibend2 OIRLTC0101

#�Ե�ī�� BL
/hw01/ibridge/ib2/shl/ibend2 OXALTC0201
/hw01/ibridge/ib2/shl/ibend2 OISLTC0201
/hw01/ibridge/ib2/shl/ibend2 OIRLTC0201

#��Ƽī��
/hw01/ibridge/ib2/shl/ibend2 OXACTC0101
/hw01/ibridge/ib2/shl/ibend2 OISCTC0101
/hw01/ibridge/ib2/shl/ibend2 OIRCTC0101

#BFGĳ����
/hw01/ibridge/ib2/shl/ibend2 OXSGBK0101
/hw01/ibridge/ib2/shl/ibend2 OXRGBK0101
/hw01/ibridge/ib2/shl/ibend2 OISGBK0101
/hw01/ibridge/ib2/shl/ibend2 OIRGBK0101

#�ѳ�Ʈ
/hw01/ibridge/ib2/shl/ibend2 OXSHNT0101
/hw01/ibridge/ib2/shl/ibend2 OXRHNT0101
/hw01/ibridge/ib2/shl/ibend2 OISHNT0101
/hw01/ibridge/ib2/shl/ibend2 OIRHNT0101

#ȿ��
/hw01/ibridge/ib2/shl/ibend2 OXSHYS0101
/hw01/ibridge/ib2/shl/ibend2 OXRHYS0101
/hw01/ibridge/ib2/shl/ibend2 OISHYS0101
/hw01/ibridge/ib2/shl/ibend2 OIRHYS0101

#KISBANK
/hw01/ibridge/ib2/shl/ibend2 OXSKBK0101
/hw01/ibridge/ib2/shl/ibend2 OXRKBK0101
/hw01/ibridge/ib2/shl/ibend2 OISKBK0101
/hw01/ibridge/ib2/shl/ibend2 OIRKBK0101

#ûȣ ����CD
/hw01/ibridge/ib2/shl/ibend2 OXSCHO0101
/hw01/ibridge/ib2/shl/ibend2 OXRCHO0101
/hw01/ibridge/ib2/shl/ibend2 OISCHO0101
/hw01/ibridge/ib2/shl/ibend2 OIRCHO0101

#�Ե� ����CD
/hw01/ibridge/ib2/shl/ibend2 OXSLOT0101
/hw01/ibridge/ib2/shl/ibend2 OXRLOT0101
/hw01/ibridge/ib2/shl/ibend2 OISLOT0101
/hw01/ibridge/ib2/shl/ibend2 OIRLOT0101

#��ȭ���� �ſ����
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0101
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0101
/hw01/ibridge/ib2/shl/ibend2 OISDHI0101
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0101

#��ȭ���� ATM
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0201
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0201
/hw01/ibridge/ib2/shl/ibend2 OISDHI0201
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0201

#��ȭ���� �ڵ�����
/hw01/ibridge/ib2/shl/ibend2 OXSDHI0301
/hw01/ibridge/ib2/shl/ibend2 OXRDHI0301
/hw01/ibridge/ib2/shl/ibend2 OISDHI0301
/hw01/ibridge/ib2/shl/ibend2 OIRDHI0301

#ThinkAT_2ä��
/hw01/ibridge/ib2/shl/ibend2 OXATAT0101
/hw01/ibridge/ib2/shl/ibend2 OISTAT0101
/hw01/ibridge/ib2/shl/ibend2 OIRTAT0101

# ���̽�
/hw01/ibridge/ib2/shl/ibend2 OXAKIS0101
/hw01/ibridge/ib2/shl/ibend2 OISKIS0101
/hw01/ibridge/ib2/shl/ibend2 OIRKIS0101

# ���̽�_�޴�������
/hw01/ibridge/ib2/shl/ibend2 OXAKIS0201
/hw01/ibridge/ib2/shl/ibend2 OISKIS0201
/hw01/ibridge/ib2/shl/ibend2 OIRKIS0201

# KS-NET �߹�ŷ
/hw01/ibridge/ib2/shl/ibend2 OXSKSN0101
/hw01/ibridge/ib2/shl/ibend2 OXRKSN0101
/hw01/ibridge/ib2/shl/ibend2 OISKSN0101
/hw01/ibridge/ib2/shl/ibend2 OIRKSN0101

# KS-NET �߹�ŷ-�ڱ�
/hw01/ibridge/ib2/shl/ibend2 OXSKSN0201
/hw01/ibridge/ib2/shl/ibend2 OXRKSN0201
/hw01/ibridge/ib2/shl/ibend2 OISKSN0201
/hw01/ibridge/ib2/shl/ibend2 OIRKSN0201

# �Ｚ��Ʈ�p �߹�ŷ
/hw01/ibridge/ib2/shl/ibend2 OXSSSN0101
/hw01/ibridge/ib2/shl/ibend2 OXRSSN0101
/hw01/ibridge/ib2/shl/ibend2 OISSSN0101
/hw01/ibridge/ib2/shl/ibend2 OIRSSN0101

# LG���÷��� PG
/hw01/ibridge/ib2/shl/ibend2 OXALGU0101
/hw01/ibridge/ib2/shl/ibend2 OISLGU0101
/hw01/ibridge/ib2/shl/ibend2 OIRLGU0101

# ����ī�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISSHC5101
/hw01/ibridge/ib2/shl/ibend2 OIRSHC5101

# ����ī�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISHDC5201
/hw01/ibridge/ib2/shl/ibend2 OIRHDC5201

# �Ｚ��Ʈ�p �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISSSN5101
/hw01/ibridge/ib2/shl/ibend2 OIRSSN5101

# KS-NET �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISKSN5101
/hw01/ibridge/ib2/shl/ibend2 OIRKSN5101

# ��Ƽī�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISCTC5101

# �Ե�ī�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISLTC5101

# �ݰ�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISKFT5101
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5101

# �ݰ�� �ϰ�����
/hw01/ibridge/ib2/shl/ibend2 OISKFT5201
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5201

# �ݰ�� CMS_��ȭ
/hw01/ibridge/ib2/shl/ibend2 OISKFT5301
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5301

# �ݰ�� CMS_����
/hw01/ibridge/ib2/shl/ibend2 OISKFT5401
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5401

# �ݰ�� ����
/hw01/ibridge/ib2/shl/ibend2 OISKFT5501
/hw01/ibridge/ib2/shl/ibend2 OIRKFT5501

# ��ġ
/hw01/ibridge/ib2/shl/ibend2 BXRLTC5101
/hw01/ibridge/ib2/shl/ibend2 BXRCTC5101
/hw01/ibridge/ib2/shl/ibend2 BXRHDC5201

# ��ġ

pid=`ps -ef | grep bxacom2 | grep -v vi | grep -v tail | grep -v grep | awk '{ print $2 }'`

if [ "$pid" = "" ];
then
            echo "bxacom2.sh is not running..."
else
    kill -9 $pid
    echo "���μ��� ���� [$pid]"
fi

#��ī �¶���
/hw01/ibridge/vela/banca/shl/bancaOnline.sh stop
#��ī ��ġ
/hw01/ibridge/vela/banca/shl/bancaBatch.sh stop
#����WEB
/hw01/ibridge/webserver/xend
# ����͸�
/hw01/ibridge/ib2/shl/ibend2 IBNETD0000
#����WEB - KANG
/hw01/ibridge/ib2/web/tomcat6/bin/shutdown.sh

}

#########################################################################
# �Լ� : ��� ���μ��� ���� - �Ŵ����� ����
func_start()
{
#ps_man
/hw01/ibridge/ib2/shl/ps_man.sh start
#��ī �¶���
/hw01/ibridge/vela/banca/shl/bancaOnline.sh start
#��ī ��ġ
/hw01/ibridge/vela/banca/shl/bancaBatch.sh start
#����WEB
/hw01/ibridge/webserver/xrun
#����WEB - KANG
/hw01/ibridge/ib2/web/tomcat6/bin/startup.sh
}


#########################################################################
# �Լ� : �����
func_usage()
{
	echo "USAGE : ${0} [ START | STOP ]"
	echo "    ��ü���� : START"
	echo "    ��ü���� : STOP"
	echo ""
}

#########################################################################

# ----------------------------------------------------------------------
# Main Process

if [ $# -gt 0 ];
then
	# --- �빮�� ��ȯ
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
