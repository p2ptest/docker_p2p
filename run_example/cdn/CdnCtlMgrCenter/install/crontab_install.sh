#!/bin/bash
chmod 644 crontab_cdnctlmgr_file
chown root crontab_cdnctlmgr_file
chgrp root crontab_cdnctlmgr_file
cp crontab_cdnctlmgr_file /etc/cron.d
touch /etc/cron.d/* 


