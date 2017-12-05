#!/bin/bash
if [ $# -ne 2 ]
then
        echo "usage:$0 rundir prgname"
        exit
fi
rundir=$1
prgname=$2
num=`ps -ef c | grep  -o '[^ ]*$'|grep "^$prgname$"|wc -l`
if [ $num -lt 1 ]
then
        echo "`date` restart $1 " >> $rundir/restart.log
        eval "cd $rundir;./start.sh"
fi

