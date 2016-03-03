#!/bin/ksh

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/hw01/hwsec/lib

TODAY=`/bin/date "+%Y%m%d"`
LOG_PATH=/hw02/ibridge/LOG2
LOG_FILE=${LOG_PATH}/${TODAY}/log_man.log

/bin/touch ${LOG_FILE}

/hw01/ibridge/ib2/bin/log_zip >> ${LOG_FILE} 2>&1

#/hw01/ibridge/ib2/bin/log_del >> ${LOG_FILE} 2>&1

/hw01/ibridge/ib2/bin/log_sec ENC ALL>> ${LOG_FILE} 2>&1


