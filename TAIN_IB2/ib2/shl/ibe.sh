
# Ignore HUP, INT, QUIT now.
trap "" 1 2 3 15

PROC_IRM=bxacom2

    pid=`ps -ef | grep ${PROC_IRM} | grep -v vi | grep -v tail | grep -v grep | awk '{ print $2 }'`

    if [ "$pid" = "" ];
    then
                echo ""
                echo "┏━━━━━━━━━━━━━━━━━┓"
                echo "┃ ${PROC_IRM} is not running...       ┃"
                echo "┗━━━━━━━━━━━━━━━━━┛"
                echo ""
    else
        kill -9 $pid
        echo ""
        echo "┏━━━━━━━━━━━━━━━━━┓"
        echo "┃ 프로세스 종료 [$pid]    ┃"
        echo "┗━━━━━━━━━━━━━━━━━┛"
        echo ""
    fi
