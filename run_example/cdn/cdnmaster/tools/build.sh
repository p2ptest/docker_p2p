#!/bin/sh 
echo "abc"
export SERVER_ROOT=`pwd`
echo "${SERVER_ROOT}"
cd ../../../
BASEROOT=`pwd`
echo ${BASEROOT}
cd ..
BASEBASEROOT=`pwd`
echo ${BASEBASEROOT}
cd ${SERVER_ROOT}
export PUBLIC_DIR=${BASEBASEROOT}/public
echo ${PUBLIC_DIR}
export FRAMECOMMON=${BASEROOT}/framecommon
export SRVFRAME=${BASEROOT}/srvframe
export COMMON=${BASEROOT}/newvod/common

if [ $# == 0 ];then
        make
	

elif [ $# == 1 ];then
        if [ $1 == 'clean' ];then
                make clean
        else
			export FILENAME=$1 
			export BINARY=`echo ${FILENAME} | awk -F'.' '{print $1}'`
			make -f makefile.one
        fi
fi
