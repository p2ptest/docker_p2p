#!/bin/bash
if [ $# -lt 1 ]
then
	echo "Usage:${0} clusterid"
	exit 1
fi

CLUSTERID=${1}

CDNCC_DB=cdnctlmgrcenter
CDNCC_TB=CdnClusterRelation_tbl
echo "delete clusterid = ${CLUSTERID} from ${CDNCC_DB}.${CDNCC_TB}"
mysql -uroot -e "use ${CDNCC_DB}; delete from ${CDNCC_TB} where ClusterId = ${CLUSTERID};"
mysql -uroot -e "use ${CDNCC_DB}; delete from ${CDNCC_TB} where ParentClusterId = ${CLUSTERID};"

mysql -uroot -e "use ${CDNCC_DB}; select * from ${CDNCC_TB} where ClusterId = ${CLUSTERID};"
mysql -uroot -e "use ${CDNCC_DB}; select * from ${CDNCC_TB} where ParentClusterId = ${CLUSTERID};"

MONITOR_DB=monitorcenter
CLUSTER_INFO_TB=t_cluster_info
CLUSTER_CONF_TB=t_cluster_conf
echo "delete clusterid = ${CLUSTERID} from ${MONITOR_DB}.${CLUSTER_INFO_TB}"
mysql -uroot -e "use ${MONITOR_DB}; delete from ${CLUSTER_INFO_TB} where clusterid = ${CLUSTERID};"
mysql -uroot -e "use ${MONITOR_DB}; select * from ${CLUSTER_INFO_TB} where clusterid = ${CLUSTERID}"

echo "delete clusterid = ${CLUSTERID} from ${MONITOR_DB}.${CLUSTER_CONF_TB}"
mysql -uroot -e "use ${MONITOR_DB}; delete from ${CLUSTER_CONF_TB} where ClusterId = ${CLUSTERID};"
mysql -uroot -e "use ${MONITOR_DB}; select * from ${CLUSTER_CONF_TB} where ClusterId = ${CLUSTERID}"

QUERYVP_DB=vp_info_db
QUERYVP_TB=t_vp_info
echo "delete clusterid = ${CLUSTERID} from ${QUERYVP_DB}.${QUERYVP_TB}"
mysql -uroot -e "use ${QUERYVP_DB}; delete from ${QUERYVP_TB} where clusterid = ${CLUSTERID};"
mysql -uroot -e "use ${QUERYVP_DB}; select * from ${QUERYVP_TB} where clusterid = ${CLUSTERID};     "





