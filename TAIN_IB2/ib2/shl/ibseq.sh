#!/bin/ksh

Today=`/bin/date +%Y%m%d`
Time=`/bin/date`
SName=`hostname`

#telnet 172.30.222.3 41506 > fep03Seq

if [ $# = 0 ]; then

    printf "${SName}       ${Time}\n"
    printf "=========================================\n"
    printf "FEPID  Batch서비스          RECV     SEND\n"
    printf "=========================================\n"

	#############################################################

   cd /hw03/ibridge/SEQ2/${Today}

	INFOS="SHC51신한카드_송수신..  \
		   HDC52현대카드_송수신..  \
		   SSN51삼성네트웍_일괄..  \
		   KSN51KS-NET_일괄배치..  \
		   CTC51시티카드_일괄수신  \
		   LTC51롯데카드_배치....  \
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

	INFOS="KFT51금결원_일괄전송..  \
		   KFT52금결원_일괄전송..  \
		   KFT53금결원_CMS_한화..  \
		   KFT54금결원_CMS_프루..  \
		   KFT55금결원_지로......  \
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
	printf "Usage: ibcnt.sh <no option> by 서지숙 at 2013.10.23 \n\n"
	printf "==========================================================\n"
fi
