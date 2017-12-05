#!/bin/bash
chmod 644 crontab_cdnmaster_file
chown root:root crontab_cdnmaster_file 
cp crontab_cdnmaster_file /etc/cron.d
touch /etc/cron.d/* 


