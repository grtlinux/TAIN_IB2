#!/bin/ksh

TODAY=`/bin/date +%Y%m%d`
#HOUR=`/bin/date +%Y%m%d%H%M`
HOUR=`/bin/date +%H%M`
PATH=/hw02/ibridge/LOG2/${TODAY}

#/bin/echo "[${TODAY}]"
#/bin/echo "[${HOUR}]"


#/hw01/ibridge/ib2/shl/fileps.sh     >> ${PATH}/ibmon_${HOUR}.log 2>&1

/hw01/ibridge/ib2/shl/fepstat2.sh   >> ${PATH}/ibmon_${HOUR}.log 2>&1

/hw01/ibridge/ib2/shl/ibcnt.sh      >> ${PATH}/ibmon_${HOUR}.log 2>&1

#/bin/df -m                          >> ${PATH}/ibmon_${HOUR}.log 2>&1


