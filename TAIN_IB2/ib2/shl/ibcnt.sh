#!/bin/ksh

# ----------------------------------------------------------------------
# �⺻���� ����
TODAY=`/bin/date +%Y%m%d`
TIME=`/bin/date`
# ----------------------------------------------------------------------


cd /hw03/ibridge/SEQ2/${TODAY}


########################################################################
# ----------------------------------------------------------------------
# �Լ� : ��/���ŰǼ� ��� (�Ǽ� ���̳��� ���� �Բ� ���)
func_seqall()
{
	printf "\t\t${TIME}\n"
	printf "       ===========================================\n"
	printf "        FEPID ���񽺸�              RECV     SEND"
	#############################################################

	printf "\n[ KFT ]-------------------------------------------\n"

	INFOS="KFT01KFTC_Ÿ��ȯ......\
				 KFT02KFTC_CD/ATM......\
				 KFT03KFTC_���ڱ���....\
				 KFT04KFTC_�������\
				 KFT05KFTC_���ͳ�����..\
				 KFT06KFTC_�ǽð�����..\
				 KFT07KFTC_CMS�ΰ�.....\
				 KFT08KFTC_PG..........\
				 KFT09KFTC_CMS�ΰ�_��ȭ\
				 KFT10KFTC_CMS�ΰ�_����\
				 KFT11KFTC_���ͳ�����Ÿ\
				 KFT12KFTC_�ǽð�����Ÿ\
				 KFT13KFTC_BCī���߹�ŷ"

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
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*��:�����ۼ�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ ���� ]------------------------------------------\n"

	INFOS="WRB01�츮����_��ü....\
				 WRB02�츮����_����..\
				 WRB03�츮����_�ڵ���ü\
				 WRB04�츮����_���뿹��\
				 WRB05�츮����_�ڱ�����\
				 WRB06�츮����_RealTIME\
				 WRB07�츮����_��������\
				 WRB08�츮����_�������\
				 SHB01��������_�߹�ŷ..\
				 SCB01SC����_��ü......\
				 TKB01�뱸����_��ü....\
				 PSB01�λ�����_��ü....\
				 KBB01��������_����....\
				 NHB01����_����........\
				 NHB02����_����........\
				 NHB03����_������......\
				 EXB01��ȯ����_����....\
				 EXB02��ȯ����_ȯ��....\
				 HNB01�ϳ�����_��ü....\
				 HNB02�ϳ�����_����....\
				 POS01��ü��_����......\
				 GUB01�������_����...."

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
				#"KBB01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (12��:�����̻�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"NHB02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1��:�������� ������)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi

		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ ī�� ]------------------------------------------\n"

	INFOS="SHC01����ī��_�¶���TR\
				 SHC02����ī��_BL......\
				 SHC03����ī��_��û....\
				 SHC04����ī��_�߹�ŷ..\
				 HDC01����ī��_����....\
				 HDC02����ī��_�߱�....\
				 HDC03����ī��_BL......\
				 HDC04����ī��_CMA�ſ�.\
				 SSC01�Ｚī��_CMA.....\
				 LTC01�Ե�ī��_��û����\
				 LTC02�Ե�ī��_BL......\
				 CTC01��Ƽī��........."

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
				#"SHC02" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (19��:��ü�� �����̻�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*��:�����ۼ�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))

	done


	#############################################################
	printf "[����CD]------------------------------------------\n"

	INFOS="GBK01BGFĳ����_����CD.\
				 HNT01�ѳ�Ʈ_����CD....\
				 HYS01ȿ��_����CD......\
				 KBK01KISBANK_����CD...\
				 CHO01ûȣ_����CD......\
				 LOT01�Ե�_����CD......"

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
				#"KBK01" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (1��:��ü�� �̰�����ȸ����)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*��:�����ۼ�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

				* ) printf "   [41m%5s %14s %8d %8d   => %4d[0m\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
			esac
		else
			printf "   %5s %14s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};
		fi


		NUM=$(( ${NUM}+1 ))
	done

	#############################################################
	printf "[ ��Ÿ ]------------------------------------------\n"

	INFOS="DHI01��ȭ����_�ſ����\
				 DHI02��ȭ����_ATM.....\
				 DHI03��ȭ����_�ڵ�����\
				 TAT01ThinkAT_2ä��ARS.\
				 KIS01���̽�...........\
				 KIS02���̽�_�޴�������\
				 KSN01KS-NET�߹�ŷ.....\
				 KSN02KS-NET�߹�ŷ-�ڱ�\
				 SSN01�Ｚ��Ʈ��_�߹�ŷ\
				 LGU01LG���÷���_PG...."

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
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*��:�����ۼ�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;
				#"FEPID" ) printf "   [41m%5s %14s %8d %8d   => %4d[0m (*��:�����ۼ�)\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq} $((${RecvSeq}-${SendSeq}));;

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
# �Լ� : �����
func_use()
{
	echo "USE : ${0} [ SEQ | ALL ]"
	echo "    �⺻��� : SEQ"
	echo "    ����� : ALL"
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
