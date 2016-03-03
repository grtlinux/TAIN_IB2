#!/bin/ksh

Today=`/bin/date +%Y%m%d`
Time=`/bin/date`
SName=`hostname`

#telnet 172.30.222.3 41506 > fep03Seq

if [ $# = 0 ]; then

    printf "${SName}       ${Time}\n"
    printf "=========================================\n"
    printf "FEPID  Batch����          RECV     SEND\n"
    printf "=========================================\n"

	#############################################################

   cd /hw03/ibridge/SEQ2/${Today}

	INFOS="SHC51����ī��_�ۼ���..  \
		   HDC52����ī��_�ۼ���..  \
		   SSN51�Ｚ��Ʈ��_�ϰ�..  \
		   KSN51KS-NET_�ϰ���ġ..  \
		   CTC51��Ƽī��_�ϰ�����  \
		   LTC51�Ե�ī��_��ġ....  \
		   "
	STOTAL=0
	RTOTAL=0

	for INFO in ${INFOS}
	do
		FEPID=`expr substr ${INFO} 1 5`
		FEPNAME=`expr substr ${INFO} 6 17`

#       /bin/echo ${FEPID} ${FEPNAME}

		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01


		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

        RTOTAL=$(( ${RTOTAL} + ${RecvSeq} ))

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi
		
		STOTAL=$(( ${STOTAL} + ${SendSeq} ))

			printf "%5s %15s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};

	done

	printf "\n"

	INFOS="KFT51�ݰ��_�ϰ�����..  \
		   KFT52�ݰ��_�ϰ�����..  \
		   KFT53�ݰ��_CMS_��ȭ..  \
		   KFT54�ݰ��_CMS_����..  \
		   KFT55�ݰ��_����......  \
		   "

	for INFO in ${INFOS}
	do
		FEPID=`expr substr ${INFO} 1 5`
		FEPNAME=`expr substr ${INFO} 6 17`

#       /bin/echo ${FEPID} ${FEPNAME}

		RecvFQname=OAR${FEPID}01
		SendFQname=OAS${FEPID}01


		if [ -f ${RecvFQname} ]; then
			RecvSeq=`/bin/cat ${RecvFQname} | /bin/awk '{print substr($0,1,8)}'`
			RecvSeq=$(( 1${RecvSeq} - 100000000 ))
		else
			RecvSeq=0;
		fi

        RTOTAL=$(( ${RTOTAL} + ${RecvSeq} ))

		if [ -f ${SendFQname} ]; then
			SendSeq=`/bin/cat ${SendFQname} |/bin/awk '{print substr($0,1,8)}'`;
			SendSeq=$(( 1${SendSeq} - 100000000 ))
		else
			SendSeq=0;
		fi

        STOTAL=$(( ${STOTAL} + ${SendSeq} ))

			printf "%5s %15s %8d %8d\n" ${FEPID} ${FEPNAME} ${RecvSeq} ${SendSeq};

	done


	#############################################################
    printf "-----------------------------------------\n"
	printf "                  TOTAL %8d %8d" ${RTOTAL} ${STOTAL}
    printf "\n=========================================\n"

else
	printf "==========================================================\n\n"
	printf "[check data count]\n"
	printf "Usage: ibcnt.sh <no option> by ������ at 2013.10.23 \n\n"
	printf "==========================================================\n"
fi
