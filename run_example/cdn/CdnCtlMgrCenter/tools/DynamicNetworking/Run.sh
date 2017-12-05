#!/bin/bash
if [ $# -ne 2 ]
then
        echo "usage:$0 rundir prgname"
        exit
fi
rundir=$1
prgname=$2
num=`ps -ef c  | awk '{print $9}'| grep  -o "^$prgname$" | wc -l`
if [ $num -lt 1 ]
then
        #echo "`date` restart $1 " >> $rundir/restart.log
		eval "cd $rundir;./$prgname&"
else
	echo exist
fi


