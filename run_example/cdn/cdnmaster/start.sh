PRONAME=cdnmaster
CHECK=`ps -ef  c | grep  -o '[^ ]*$' | grep -o "^${PRONAME}$"`
if [ "${CHECK}" = "${PRONAME}" ]
then
    echo ==============================================================================================
    echo "Warning : Program [${PRONAME}] is running ,please kill the running program first before start!" 
    echo ==============================================================================================
    exit 0
fi
ulimit -c unlimited
ulimit -n 100000
./${PRONAME} &

cd install
chmod +x *.sh 
dos2unix *.sh
./crontab_install.sh 

(usleep 1000000
echo
echo "[Program stat:========]"
ps -ef | grep ${PRONAME}| grep -v grep
echo "[Listen port:=========]"
netstat -nap | grep ${PRONAME})&

