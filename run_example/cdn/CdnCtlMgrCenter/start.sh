CHECK=`ps -ef | grep -o ./CdnCtlMgrCenter$`
if [ "${CHECK}" = "./storageinfo" ]
then
    echo ===================================================================================
    echo "Warning : Program CdnCtlMgrCenter is running ,please kill the running program first before start!"
    echo ===================================================================================
    exit 0
fi 

ulimit -c unlimited
ulimit -n 102400
./CdnCtlMgrCenter &
cd install 
chmod +x *.sh 
dos2unix *
./crontab_install.sh 
echo check install storageinfo 
echo `ps -ef | grep storageinfo | grep -v grep`
