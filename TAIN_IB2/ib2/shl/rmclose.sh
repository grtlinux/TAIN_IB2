#!/bin/ksh

TODAY=`date "+%Y%m%d"`

###################################################################
# DATE 2014.03.05

#------------------------------------------------------------------
func_ibend()
{
	echo "ibend..."
}

#------------------------------------------------------------------
func_check_test()
{
	for WORD in `fepstat.sh | grep CLOSE_WAIT`
	do
		case ${WORD} in
			OISTAT0101 )
				echo "OIS> ibend2 ${WORD}"
				ibend2 ${WORD}
				;;
			* )
				# echo "> ${WORD}"
				;;
		esac
	done
}

#------------------------------------------------------------------
func_check()
{
	for WORD in `fepstat.sh | grep CLOSE_WAIT`
	do
		case ${WORD} in
			OIS??????? )
				# echo "OIS> ibend2 ${WORD}"
				ibend2 ${WORD}
				;;
			OIR??????? )
				# echo "OIR> ibend2 ${WORD}"
				ibend2 ${WORD}
				;;
			* )
				# echo "> ${WORD}"
				;;
		esac
	done
}

#------------------------------------------------------------------
func_list()
{
	while ( true )
	do
		echo "---------------------------------------"
		date
		#func_check_test
		func_check
		sleep 60
	done
}

###################################################################
# DATE 2014.03.05

case ${1} in
	"list" ) func_list;;
	* )
		echo "USAGE : rmclose.sh list"
		;;
esac;


