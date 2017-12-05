#!/bin/bash
export SERVER_ROOT=`pwd`
cd ..
export BASEROOT=`pwd`
cd ..
BASEBASEROOT=`pwd`
cd ..
BASEBASEBASEROOT=`pwd`
cd ${SERVER_ROOT}
export PUBLIC_DIR=${BASEBASEBASEROOT}/public
export FRAMECOMMON=${BASEBASEROOT}/framecommon
export UTILS=${BASEBASEROOT}/utils
export SRVFRAME=${BASEBASEROOT}/srvframe
