/*
 * =====================================================================================
 *
 *       Filename:  cdnclustermgr.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "cdnclustermgr.h"
#include "cdnctlmgrdef.h"
#include "cdnctlmgrserver.h"
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace MYFRAMECOMMON;

const int MAX_PARENT_LAYER_NUM = 512;

const string g_strCdnClusterRelationTbName = "CdnClusterRelation_tbl";

const string g_strCdnInfoTbName = "t_cluster_info";
const string g_strClusterConfTbName = "t_cluster_conf";

CCdnClusterMgr* CCdnClusterMgr::m_hInstance = NULL;

CCdnClusterMgr::CCdnClusterMgr()
{
	m_nReloadInterval = 0;
}

CCdnClusterMgr::~CCdnClusterMgr()
{

}

CCdnClusterMgr* CCdnClusterMgr::GetInstance()
{
	return m_hInstance;
}

void CCdnClusterMgr::CreateInstance()
{
	if (NULL == m_hInstance)
	{
		m_hInstance = new CCdnClusterMgr();
	}
}

void CCdnClusterMgr::DeleteInstance()
{
	if (m_hInstance)
	{
		delete m_hInstance;
		m_hInstance = NULL;
	}	
}

int CCdnClusterMgr::LoadClusterConf(map<unsigned int, TClusterConf>& mapClusterConf)
{
	int nRet = 0;
	CCdnCtlMgrServer* pServer = (CCdnCtlMgrServer*)CMyserver::GetInstance();
	TInfoConfItem& confItem = pServer->GetConf().GetConfItem();
	CMysql mySql;
	char szSql[1024] = {0};

	mapClusterConf.clear();
	try
	{
		mySql.Init();
		mySql.Connect(confItem.monitorDbItem.strSqlIp.c_str(), confItem.monitorDbItem.strSqlUser.c_str(), confItem.monitorDbItem.strPasswd.c_str());

		//载入cdn集群信息
		TClusterConf ClusterConf;
		snprintf(szSql, sizeof(szSql), "select ClusterId,bandwidth,limitmode,limitspeed,maxspeed,multiplespeed,status,ishot,hash,isp,Comment from %s.%s order by clusterid;",
			confItem.monitorDbItem.strDbName.c_str(), g_strClusterConfTbName.c_str());

		mySql.Query(szSql);
		mySql.StoreResult();
		while(mySql.FetchRow())
		{
			ClusterConf.m_nState = (char)strtoul(mySql.GetField("status"), NULL, 10); //cluster status
			ClusterConf.m_nClusterId = strtoul(mySql.GetField("ClusterId"), NULL, 10);
			ClusterConf.m_nLimitMode =  (char)strtoul(mySql.GetField("limitmode"), NULL, 10);
			ClusterConf.m_nSpeedLimit = strtoul(mySql.GetField("limitspeed"), NULL, 10) * 1024;
			ClusterConf.m_nMaxLimit = strtoul(mySql.GetField("maxspeed"), NULL, 10) * 1024;
			ClusterConf.m_nMultipleLimit = strtoul(mySql.GetField("multiplespeed"), NULL, 10);
			ClusterConf.m_nIsHot =  (char)strtoul(mySql.GetField("ishot"), NULL, 10);
			ClusterConf.m_nHash =  (char)strtoul(mySql.GetField("hash"), NULL, 10);
			ClusterConf.m_nIsp=  (char)strtoul(mySql.GetField("isp"), NULL, 10);
			ClusterConf.m_sComment = mySql.GetField("Comment");
			mapClusterConf.insert(make_pair(ClusterConf.m_nClusterId, ClusterConf));
		}
		mySql.FreeResult();
	}
	catch(CMysqlException& e)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterConf] DB error, sql %s, error %s", szSql, e.GetErrMsg());
		return -1;
	}
	return nRet;
}

int CCdnClusterMgr::LoadClusterInfo(map<unsigned int, TCdnClusterInfo>& mapCdnCluster, map<unsigned int, unsigned int>& mapIp2ClusterId,
									map<unsigned int, TClusterConf>& mapClusterConf, map<unsigned int, unsigned int>& mapIp2HostId)
{
	int nRet = 0;
	CCdnCtlMgrServer* pServer = (CCdnCtlMgrServer*)CMyserver::GetInstance();
	TInfoConfItem& confItem = pServer->GetConf().GetConfItem();
	CMysql mySql;
	char szSql[1024] = {0};

	mapCdnCluster.clear();
	mapIp2ClusterId.clear();
	try
	{
		mySql.Init();
		mySql.Connect(confItem.monitorDbItem.strSqlIp.c_str(), confItem.monitorDbItem.strSqlUser.c_str(), confItem.monitorDbItem.strPasswd.c_str());

		//载入cdn集群信息
		TCdnSrvInfo CdnSrvInfoTmp;
		snprintf(szSql, sizeof(szSql), "select clusterid,hostid,enableflag,vpexternalip,vpinternalip from %s.%s order by clusterid,hostid;",
			confItem.monitorDbItem.strDbName.c_str(), g_strCdnInfoTbName.c_str());

		mySql.Query(szSql);
		mySql.StoreResult();
		while(mySql.FetchRow())
		{
			CdnSrvInfoTmp.m_nClusterId = strtoul(mySql.GetField("clusterid"), NULL, 10);
			if (mapClusterConf.find(CdnSrvInfoTmp.m_nClusterId) == mapClusterConf.end())
			{
				continue;
			}

			CdnSrvInfoTmp.m_nHostId = strtoul(mySql.GetField("hostid"), NULL, 10);
			CdnSrvInfoTmp.m_nState = strtoul(mySql.GetField("enableflag"), NULL, 10); //server status

			string strPublicIp = mySql.GetField("vpexternalip");
			if( inet_pton(AF_INET, strPublicIp.c_str(), &CdnSrvInfoTmp.m_nPublicIp) <= 0)
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterInfo] PublicIp: %s", strPublicIp.c_str());
				nRet = -1;
				break;
			}
			CdnSrvInfoTmp.m_nPublicIp = ntohl(CdnSrvInfoTmp.m_nPublicIp);

			string strPrivateIp = mySql.GetField("vpinternalip");
			if( inet_pton(AF_INET, strPrivateIp.c_str(), &CdnSrvInfoTmp.m_nPrivateIp) <= 0)
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterInfo] PrivateIp: %s", strPrivateIp.c_str());
				nRet = -1;
				break;
			}
			CdnSrvInfoTmp.m_nPrivateIp = ntohl(CdnSrvInfoTmp.m_nPrivateIp);

			map<unsigned int, TCdnClusterInfo>::iterator itrCdnCluster = mapCdnCluster.find(CdnSrvInfoTmp.m_nClusterId);
			if(itrCdnCluster != mapCdnCluster.end())
			{
				itrCdnCluster->second.m_vecCdnSrvInfo.push_back(CdnSrvInfoTmp);
			}
			else
			{
				TCdnClusterInfo CdnClusterInfoTmp;
				CdnClusterInfoTmp.m_nClusterId = CdnSrvInfoTmp.m_nClusterId;
				CdnClusterInfoTmp.m_vecCdnSrvInfo.push_back(CdnSrvInfoTmp);
				mapCdnCluster.insert(make_pair(CdnClusterInfoTmp.m_nClusterId, CdnClusterInfoTmp));
			}
			map<unsigned int, unsigned int>::iterator itrMpaIp2ClusterId = mapIp2ClusterId.find(CdnSrvInfoTmp.m_nPublicIp);
			if (itrMpaIp2ClusterId != mapIp2ClusterId.end())
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterInfo] Different Cluster have Same Ip: %s", strPublicIp.c_str());
				nRet = -1;
				break;
			}
			mapIp2ClusterId.insert(make_pair(CdnSrvInfoTmp.m_nPublicIp, CdnSrvInfoTmp.m_nClusterId));
			map<unsigned int, unsigned int>::iterator itrMpaIp2HostId = mapIp2HostId.find(CdnSrvInfoTmp.m_nPublicIp);
			if (itrMpaIp2HostId != mapIp2HostId.end())
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterInfo] Different Cluster have Same Ip: %s", strPublicIp.c_str());
				nRet = -1;
				break;
			}
			mapIp2HostId.insert(make_pair(CdnSrvInfoTmp.m_nPublicIp, CdnSrvInfoTmp.m_nHostId));
		}
		mySql.FreeResult();
	}
	catch(CMysqlException& e)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterInfo] DB error, sql %s, error %s", szSql, e.GetErrMsg());
		return -1;
	}
	return nRet;
}

int CCdnClusterMgr::LoadClusterRelation(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnParentRelation, 
										map<unsigned int, vector<TCdnRelationInfo> >& mapCdnSonRelation, 
										map<unsigned int, TCdnClusterInfo>& mapCdnCluster, 
										map<unsigned int, TClusterConf>& mapClusterConf)
{
	int nRet = 0;
	CCdnCtlMgrServer* pServer = (CCdnCtlMgrServer*)CMyserver::GetInstance();
	TInfoConfItem& confItem = pServer->GetConf().GetConfItem();
	CMysql mySql;
	char szSql[1024] = {0};

	try
	{
		mySql.Init();
		mySql.Connect(confItem.localDbItem.strSqlIp.c_str(), confItem.localDbItem.strSqlUser.c_str(), confItem.localDbItem.strPasswd.c_str());

		//载入cdn父子关系
		snprintf(szSql, sizeof(szSql), "select ClusterId,ParentClusterId,IfUserPublicIp,ParentAttribute from %s.%s order by ClusterId;",
			confItem.localDbItem.strDbName.c_str(), g_strCdnClusterRelationTbName.c_str());
		mySql.Query(szSql);
		mySql.StoreResult();
		while(mySql.FetchRow())
		{
			TCdnRelationInfo CdnRelationInfoTmp;
			CdnRelationInfoTmp.m_nClusterId = strtoul(mySql.GetField("ClusterId"), NULL, 10);
			CdnRelationInfoTmp.m_nParentClusterId = strtoul(mySql.GetField("ParentClusterId"), NULL, 10);
			CdnRelationInfoTmp.m_bIfUsePublicIp = strtoul(mySql.GetField("IfUserPublicIp"), NULL, 10);
			CdnRelationInfoTmp.m_bParentAttribute= strtoul(mySql.GetField("ParentAttribute"), NULL, 10);
			map<unsigned int, TClusterConf>::iterator itrSonConf = mapClusterConf.find(CdnRelationInfoTmp.m_nClusterId);
			map<unsigned int, TCdnClusterInfo>::iterator itrSonInfo = mapCdnCluster.find(CdnRelationInfoTmp.m_nClusterId);
			if( (itrSonConf == mapClusterConf.end()) ||( itrSonInfo == mapCdnCluster.end()))
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterRelation] Cluster(%u) is not exist", CdnRelationInfoTmp.m_nClusterId);
				nRet = -1;
				break;
			}
			else if( 0 == itrSonConf->second.m_nState) //cluster status
			{
				continue;
			}

			map<unsigned int, TCdnClusterInfo>::iterator itrParentInfo = mapCdnCluster.find(CdnRelationInfoTmp.m_nParentClusterId);
			map<unsigned int, TClusterConf>::iterator itrParentConf = mapClusterConf.find(CdnRelationInfoTmp.m_nParentClusterId);
			if ((itrParentInfo == mapCdnCluster.end()) || (itrParentConf == mapClusterConf.end()))
			{
				WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterRelation] Parent Cluster(%u) is not exist", CdnRelationInfoTmp.m_nParentClusterId);
				nRet = -1;
				break;
			}
			else
			{				
				if (itrParentConf->second.m_nState == 0) //cluster status
				{
					continue;
				}
				int nCdnNum = (int)itrParentInfo->second.m_vecCdnSrvInfo.size();
				TCdnHostInfo CdnParentTmp;
				for (int nA = 0; nA < nCdnNum; ++ nA)
				{
					CdnParentTmp.m_nHostId = itrParentInfo->second.m_vecCdnSrvInfo[nA].m_nHostId;
					CdnParentTmp.m_nState = itrParentInfo->second.m_vecCdnSrvInfo[nA].m_nState;
					if (CdnRelationInfoTmp.m_bIfUsePublicIp)
					{
						CdnParentTmp.m_nIp = itrParentInfo->second.m_vecCdnSrvInfo[nA].m_nPublicIp;
					}
					else
					{
						CdnParentTmp.m_nIp = itrParentInfo->second.m_vecCdnSrvInfo[nA].m_nPrivateIp;
					}
					CdnRelationInfoTmp.m_vecParentCdn.push_back(CdnParentTmp);
				}
				nCdnNum = (int)itrSonInfo->second.m_vecCdnSrvInfo.size();
				TCdnHostInfo CdnSonTmp;
				for (int nA = 0; nA < nCdnNum; ++ nA)
				{
					CdnSonTmp.m_nHostId = itrSonInfo->second.m_vecCdnSrvInfo[nA].m_nHostId;
					CdnSonTmp.m_nState = itrSonInfo->second.m_vecCdnSrvInfo[nA].m_nState;
					if (CdnRelationInfoTmp.m_bIfUsePublicIp)
					{
						CdnSonTmp.m_nIp = itrSonInfo->second.m_vecCdnSrvInfo[nA].m_nPublicIp;
					}
					else
					{
						CdnSonTmp.m_nIp = itrSonInfo->second.m_vecCdnSrvInfo[nA].m_nPrivateIp;
					}
					CdnRelationInfoTmp.m_vecSonCdn.push_back(CdnSonTmp);
				}
			}

			map<unsigned int, vector<TCdnRelationInfo> >::iterator itrCdnParent = mapCdnParentRelation.find(CdnRelationInfoTmp.m_nClusterId);
			if(itrCdnParent != mapCdnParentRelation.end())
			{
				itrCdnParent->second.push_back(CdnRelationInfoTmp);
			}
			else
			{
				vector<TCdnRelationInfo> vecCdnRelationTmp;
				vecCdnRelationTmp.push_back(CdnRelationInfoTmp);
				mapCdnParentRelation.insert(make_pair(CdnRelationInfoTmp.m_nClusterId, vecCdnRelationTmp));
			}
			map<unsigned int, vector<TCdnRelationInfo> >::iterator itrCdnSon = mapCdnSonRelation.find(CdnRelationInfoTmp.m_nParentClusterId);
			if(itrCdnSon != mapCdnSonRelation.end())
			{
				itrCdnSon->second.push_back(CdnRelationInfoTmp);
			}
			else
			{
				vector<TCdnRelationInfo> vecCdnRelationTmp;
				vecCdnRelationTmp.push_back(CdnRelationInfoTmp);
				mapCdnSonRelation.insert(make_pair(CdnRelationInfoTmp.m_nParentClusterId, vecCdnRelationTmp));
			}
		}
		mySql.FreeResult();
	}
	catch(CMysqlException& e)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadClusterRelation] DB error, sql %s, error %s", szSql, e.GetErrMsg());
		return -1;
	}
	if (nRet == 0)
	{
		nRet = CheckIfHaveCircleRelation(mapCdnParentRelation);
	}
	return nRet;
}

int CCdnClusterMgr::GenAllClusterLay(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnRelation, map<unsigned int, unsigned int>& mapClusterLay, map<unsigned int, TClusterConf>& mapClusterConf)		//added by wangfangzhu 2011-10-17
{
	WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "----------------------GenAllClusterLay------------------------");

	map<unsigned int, set<unsigned int> > mapLayer;

	set<unsigned int> clusterLaySet;
	set<unsigned int> clusterUnLaySet;

	//init clusterUnLaySet and init for find Lay0
	set<unsigned int> clusterSet;
	set<unsigned int> parentSet;

	{
		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrClusterId = mapCdnRelation.begin();	
		for(; itrClusterId != mapCdnRelation.end(); itrClusterId++)
		{
			clusterUnLaySet.insert(itrClusterId->first);
			clusterSet.insert(itrClusterId->first);
			vector<TCdnRelationInfo>& vectorParentId = itrClusterId->second;
			int nSize = vectorParentId.size();
			for(int i = 0; i < nSize; i++)
			{
				unsigned int nParentId = vectorParentId[i].m_nParentClusterId;
				clusterUnLaySet.insert(nParentId);
				parentSet.insert(nParentId);
			}
		}
	}

	//find Lay0
	{
		set<unsigned int> setClusterId;
		set<unsigned int>::iterator itrParentId = parentSet.begin();
		for(; itrParentId != parentSet.end(); itrParentId++)
		{
			unsigned int nParentId = *itrParentId;
			if( clusterSet.find(nParentId) == clusterSet.end() )
			{
				clusterLaySet.insert(nParentId);
				clusterUnLaySet.erase(nParentId);
				mapClusterLay[nParentId] = 0;
				WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "<clusterid=%u, layer=0>", nParentId);

				setClusterId.insert(nParentId);
			}
		}

		mapLayer[0] = setClusterId;
	}

	//find each Lay
	{
		while( !clusterUnLaySet.empty() )
		{
			set<unsigned int>::iterator iend = clusterUnLaySet.end();
			set<unsigned int>::iterator itrUnLay = clusterUnLaySet.begin();
			for(; itrUnLay != clusterUnLaySet.end(); itrUnLay++)
			{
				unsigned int nId = *itrUnLay;				
				//is all parents in clusterLaySet?
				map<unsigned int, vector<TCdnRelationInfo> >::iterator iFind =  mapCdnRelation.find(nId);
				if(iFind == mapCdnRelation.end() )
				{
					WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "GenAllClusterLay error, %d can't find relation.\n", nId);
					return -1;
				}

				map<unsigned int, unsigned int> mapParentLay;
				bool bFind = true;
				vector<TCdnRelationInfo>& vectorParentId = iFind->second;
				int nSize = vectorParentId.size();
				if(0 == nSize)
				{
					WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "GenAllClusterLay error, %d no parent.\n", nId);
					return -1;
				}

				unsigned int nMaxLay = 0;
				unsigned int nPtmp;
				for(int i = 0; i < nSize; i++)
				{
					unsigned int nParentId = vectorParentId[i].m_nParentClusterId;
					if(mapClusterLay.find(nParentId) == mapClusterLay.end() )
					{						
						bFind = false;						
						break;
					}

					if(mapClusterLay[nParentId] > nMaxLay)
					{
						nMaxLay = mapClusterLay[nParentId];
						nPtmp = nParentId;
					}	

					if(0 == nMaxLay)
						nPtmp = nParentId;
				}

				if(!bFind)
				{
					continue;
				}

				mapClusterLay[nId] = nMaxLay + 1;
				clusterUnLaySet.erase(itrUnLay);		
				WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "<clusterid=%u, layer=%u, parentid=%u>", nId, mapClusterLay[nId], nPtmp);

				map<unsigned int, set<unsigned int> >::iterator ifind = mapLayer.find(nMaxLay + 1);
				if( ifind != mapLayer.end())
				{
					ifind->second.insert(nId);
				}
				else
				{
					set<unsigned int> setLayer;
					setLayer.insert(nId);
					mapLayer[nMaxLay+1] = setLayer;
				}
				break;
			}
		}		
	}

	{
		map<unsigned int, set<unsigned int> >::iterator itrLay = mapLayer.begin();
		for(; itrLay != mapLayer.end(); itrLay++)
		{
			unsigned int nLayer = itrLay->first;
			set<unsigned int>& setLayer = itrLay->second;

			set<unsigned int>::iterator itrLayer = setLayer.begin();
			for(; itrLayer != setLayer.end(); itrLayer++)
			{
				unsigned int nId = *itrLayer;
				map<unsigned int, TClusterConf>::iterator itrFind =  mapClusterConf.find(nId);
				if(itrFind == mapClusterConf.end())
				{
					WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "GenAllClusterLay error, exception.\n");
					WriteRunInfo::WriteError("GenAllClusterLay error!");
					return -1;
				}
				WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "<layer=%d, desc=%s>", nLayer, itrFind->second.m_sComment.c_str());
			}
		}
	}
	WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "------------GenAllClusterLay succeed----------------");

	return 0;
}


int CCdnClusterMgr::LoadFromDb()
{
	int nRet = 0;

	map<unsigned int, TCdnClusterInfo> mapCdnCluster;				//进群信息
	map<unsigned int, unsigned int> mapIp2ClusterId;				//Ip到集群的映射
	map<unsigned int, unsigned int> mapIp2HostId;
	map<unsigned int, TClusterConf> mapClusterConf;					//进群配置
	map<unsigned int, vector<TCdnRelationInfo> > mapCdnParentRelation;	//cdn组网关系	//modify by yinhaichen 2010-03-30
	map<unsigned int, vector<TCdnRelationInfo> > mapCdnSonRelation;
	map<unsigned int, unsigned int> mapClusterLay;					//集群Layer		//added by wangfangzhu 2011-10-17

	//载入集群配置
	nRet = LoadClusterConf(mapClusterConf);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadFromDb] : LoadClusterConf fail");
		return -1;
	}
	//载入集群信息
	nRet = LoadClusterInfo(mapCdnCluster, mapIp2ClusterId, mapClusterConf, mapIp2HostId);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadFromDb] : LoadClusterInfo fail");
		return -1;
	}
	
	//载入父子关系
	nRet = LoadClusterRelation(mapCdnParentRelation, mapCdnSonRelation, mapCdnCluster, mapClusterConf);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadFromDb] : LoadClusterRelation fail");
		return -1;
	}
	//
	nRet = GenAllClusterLay(mapCdnParentRelation, mapClusterLay, mapClusterConf);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::LoadFromDb] : GenAllClusterLay fail");
		return -1;
	}

	m_rwlkCdnClusterMgr.GetWriteLock();
	m_mapClusterConf.swap(mapClusterConf);
	m_mapCdnCluster.swap(mapCdnCluster);
	m_mapIp2ClusterId.swap(mapIp2ClusterId);
	m_mapIp2HostId.swap(mapIp2HostId);
	m_mapCdnParentRelation.swap(mapCdnParentRelation);
	m_mapCdnSonRelation.swap(mapCdnSonRelation);
	m_mapClusterLay.swap(mapClusterLay);
	m_rwlkCdnClusterMgr.ReleaseLock();

	return nRet;
}

void CCdnClusterMgr::WriteLog()
{
	if (WriteRunInfo::LOGTYPEARRAY[CDNCTLMGRSRV_LOG].nLogOff)
	{
		//net relation print
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "RelationStart==============================");			
		m_rwlkCdnClusterMgr.GetReadLock();
		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrCdnRelation = m_mapCdnParentRelation.begin();
		while (itrCdnRelation != m_mapCdnParentRelation.end())			
		{				
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tStart(%d)==============================", itrCdnRelation->first);
			int nParentNum = itrCdnRelation->second.size();
			for (int nA = 0; nA < nParentNum; ++ nA)				
			{					
				itrCdnRelation->second[nA].WriteLog();
			}
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tEnd(%d)==============================", itrCdnRelation->first);
			itrCdnRelation++;
		}
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "RelationEnd==============================");
		
		//clusterinfo print
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "ClusterInfoStart==============================");
		map<unsigned int, TCdnClusterInfo>::iterator itrCdnCluster = m_mapCdnCluster.begin();
		while(itrCdnCluster != m_mapCdnCluster.end())
		{
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tStart(%d)==============================", itrCdnCluster->first);
			itrCdnCluster->second.WriteLog();
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tEnd(%d)==============================", itrCdnCluster->first);
			itrCdnCluster ++;
		}
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "ClusterInfoEnd==============================");
		
		//cluster conf print
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "ClusterConfStart==============================");
		map<unsigned int, TClusterConf>::iterator itrCdnClusterConf = m_mapClusterConf.begin();
		while (itrCdnClusterConf != m_mapClusterConf.end())
		{
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tStart(%d)==============================", itrCdnClusterConf->first);
			itrCdnClusterConf->second.WriteLog();
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\tEnd(%d)==============================", itrCdnClusterConf->first);
			itrCdnClusterConf ++;
		}
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "ClusterConfEnd==============================");
		
		m_rwlkCdnClusterMgr.ReleaseLock();
	}
}

int CCdnClusterMgr::InitClusterMgr()
{
	int nRet = 0;
	CCdnCtlMgrServer* pServer = (CCdnCtlMgrServer*)CMyserver::GetInstance();
	TInfoConfItem& confItem = pServer->GetConf().GetConfItem();
	m_nReloadInterval = confItem.nReloadInterval;

	nRet = LoadFromDb();
	if(nRet != 0)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::InitClusterMgr] load db failed");
		return -1;
	}
	WriteLog();
	pthread_t tid;	
	nRet = pthread_create(&tid, NULL, ReloadThread, (void*)this);
	if (0 != nRet)
	{
		WriteRunInfo::WriteError("[CCdnClusterMgr::InitClusterMgr] create ReloadThread thread failed, error is %s", strerror(errno));
	}
	else
	{
		WriteRunInfo::WriteLog("[CCdnClusterMgr::InitClusterMgr] create ReloadThread thread succeed");
	}
	
	return nRet;	
}

int CCdnClusterMgr::CheckIfHotCluster(unsigned int nClusterId, char& nIsHot, char& nHash)
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, TClusterConf>::iterator itrClusterConf =  m_mapClusterConf.find(nClusterId);
	if (itrClusterConf != m_mapClusterConf.end())
	{
		nIsHot = itrClusterConf->second.m_nIsHot;
		nHash = itrClusterConf->second.m_nHash;
	}
	else
	{
		nRet = -1;
		nIsHot = -1;
		nHash = -1;
	}	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;
}


int CCdnClusterMgr::GetParentCdn(unsigned int nIp, vector<TCdnParent>& vecCdnParent)
{
	int nRet = 0;
	vecCdnParent.clear();

	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrMapIp2ClusterId = m_mapIp2ClusterId.find(nIp);
	if (itrMapIp2ClusterId != m_mapIp2ClusterId.end())
	{
		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrMapCdnRelation = m_mapCdnParentRelation.find(itrMapIp2ClusterId->second);//modify by yinhaichen 2010-03-30
		if(itrMapCdnRelation != m_mapCdnParentRelation.end())
		{
			vector<TCdnHostInfo> tmpVecHostInfo;
			tmpVecHostInfo = itrMapCdnRelation->second[0].m_vecParentCdn;//get first one.
			for(unsigned int i = 0; i < tmpVecHostInfo.size(); i++)
			{
				TCdnParent tmpCdnParent;
				tmpCdnParent.m_nIp = tmpVecHostInfo[i].m_nIp;
				tmpCdnParent.m_nState = tmpVecHostInfo[i].m_nState;
				vecCdnParent.push_back(tmpCdnParent);
			}
		}
		else
		{
			nRet = -1;
			WRITE_TRACE("CCdnClusterMgr::GetParentCdn : Ip(%u) not find Parent", nIp);
		}
	}
	else
	{
		nRet = -1;
		WriteRunInfo::WriteError("CCdnClusterMgr::GetParentCdn : Ip(%u) not find Parent", nIp);
	}	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;	
}

int CCdnClusterMgr::GetParentsCdn(unsigned int nIp, vector<vector<TCdnParent> >& vecCdnParent)//add by yinhaichen 2011-03-29
{
	int nRet = 0;
	vecCdnParent.clear();

	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrMapIp2ClusterId = m_mapIp2ClusterId.find(nIp);
	if (itrMapIp2ClusterId != m_mapIp2ClusterId.end())
	{
		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrMapCdnRelation = m_mapCdnParentRelation.find(itrMapIp2ClusterId->second);
		if (itrMapCdnRelation != m_mapCdnParentRelation.end())
		{
			for (unsigned int i = 0; i < itrMapCdnRelation->second.size(); i ++)
			{
				vector<TCdnParent> tmpVecCdnParent;
				vector<TCdnHostInfo> tmpVecHostInfo =  itrMapCdnRelation->second[i].m_vecParentCdn;
				for(unsigned int ii = 0; ii < tmpVecHostInfo.size(); ii++)
				{
					TCdnParent tmpCdnParent;
					tmpCdnParent.m_nIp = tmpVecHostInfo[ii].m_nIp;
					tmpCdnParent.m_nState = tmpVecHostInfo[ii].m_nState;
					tmpVecCdnParent.push_back(tmpCdnParent);
				}
				vecCdnParent.push_back(tmpVecCdnParent);
			}
		}

		if (vecCdnParent.empty())
		{
			nRet = -1;
			WRITE_TRACE("CCdnClusterMgr::GetParentsCdn : Ip(%u) not find Parent", nIp);
		}
	}
	else
	{
		nRet = -1;
		WriteRunInfo::WriteError("CCdnClusterMgr::GetParentsCdn : Ip(%u) not find Parent", nIp);
	}	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;	
}

int CCdnClusterMgr::GetNetRelation(unsigned int nIp, vector<TRelationCluster> &vecParentCluster, vector<TRelationCluster> &vecSonCluster, unsigned int& nLay)
{
	int nRet = 0;
	vecParentCluster.clear();
	vecSonCluster.clear();
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrMapIp2ClusterId = m_mapIp2ClusterId.find(nIp);
	if (itrMapIp2ClusterId != m_mapIp2ClusterId.end())
	{
		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrMapCdnRelation = m_mapCdnParentRelation.find(itrMapIp2ClusterId->second);
		if (itrMapCdnRelation != m_mapCdnParentRelation.end())
		{
			for (unsigned int i = 0; i < itrMapCdnRelation->second.size(); i ++)
			{
				TRelationCluster relationClusterTmp;
				relationClusterTmp.m_nClusterId = itrMapCdnRelation->second[i].m_nParentClusterId ;
				for(unsigned int nA = 0; nA < itrMapCdnRelation->second[i].m_vecParentCdn.size(); nA++)
				{
					relationClusterTmp.m_vecHostInCLuster.push_back( itrMapCdnRelation->second[i].m_vecParentCdn[nA]);
				}
				relationClusterTmp.m_nParentAttribute= itrMapCdnRelation->second[i].m_bParentAttribute;
				int nRet = GetIsp(relationClusterTmp.m_nClusterId, relationClusterTmp.m_nIsp);
				if(nRet != 0)
				{
					nRet = -1;
					WriteRunInfo::WriteError("CCdnClusterMgr::GetNetRelation : ClusterId(%u) GetIsp fail", relationClusterTmp.m_nClusterId);
					break;
				}
				nRet = CheckIfHotCluster(relationClusterTmp.m_nClusterId, relationClusterTmp.m_nIsHot, relationClusterTmp.m_nHash);
				if(nRet != 0)
				{
					nRet = -1;
					WriteRunInfo::WriteError("CCdnClusterMgr::GetNetRelation : ClusterId(%u) CheckIfHotCluster fail", relationClusterTmp.m_nClusterId);
					break;
				}
				vecParentCluster.push_back(relationClusterTmp);
			}
		}
		if(nRet)
		{
			vecSonCluster.clear();
			m_rwlkCdnClusterMgr.ReleaseLock();
			return -1;
		}
		if (vecParentCluster.empty())
		{
			m_rwlkCdnClusterMgr.ReleaseLock();
			WRITE_TRACE("CCdnClusterMgr::GetNetRelation : Ip(%u) not find Parent", nIp);
			return -1;
		}

		map<unsigned int, vector<TCdnRelationInfo> >::iterator itrMapSonCdnRelation = m_mapCdnSonRelation.find(itrMapIp2ClusterId->second);
		if(itrMapSonCdnRelation != m_mapCdnSonRelation.end())
		{
			for (unsigned int i = 0; i < itrMapSonCdnRelation->second.size(); i ++)
			{
				TRelationCluster relationClusterTmp;
				relationClusterTmp.m_nClusterId =  itrMapSonCdnRelation->second[i].m_nClusterId;
				for(unsigned int nA = 0; nA < itrMapSonCdnRelation->second[i].m_vecSonCdn.size(); nA++)
				{
					relationClusterTmp.m_vecHostInCLuster.push_back( itrMapSonCdnRelation->second[i].m_vecSonCdn[nA]);
				}
				relationClusterTmp.m_nParentAttribute = itrMapSonCdnRelation->second[i].m_bParentAttribute;
				int nRet = GetIsp(relationClusterTmp.m_nClusterId, relationClusterTmp.m_nIsp);
				if(nRet != 0)
				{
					nRet = -1;
					WriteRunInfo::WriteError("CCdnClusterMgr::GetNetRelation : ClusterId(%u) GetIsp fail", relationClusterTmp.m_nClusterId);
					break;
				}
				nRet = CheckIfHotCluster(relationClusterTmp.m_nClusterId, relationClusterTmp.m_nIsHot, relationClusterTmp.m_nHash);
				if(nRet != 0)
				{
					nRet = -1;
					WriteRunInfo::WriteError("CCdnClusterMgr::GetNetRelation : ClusterId(%u) CheckIfHotCluster fail", relationClusterTmp.m_nClusterId);
					break;
				}
				vecSonCluster.push_back(relationClusterTmp);
			}
		}
		if(nRet)
		{
			vecSonCluster.clear();
			m_rwlkCdnClusterMgr.ReleaseLock();
			return -1;
		}
		
	}
	else
	{
		m_rwlkCdnClusterMgr.ReleaseLock();
		WriteRunInfo::WriteError("CCdnClusterMgr::GetNetRelation : Ip(%u) not find ClusterId", nIp);
		return -1;
	}

	map<unsigned int, unsigned int>::iterator itrClusterLay = m_mapClusterLay.find(itrMapIp2ClusterId->second);
	if( itrClusterLay != m_mapClusterLay.end() )
	{
		nLay = itrClusterLay->second;
	}
	else
	{
		WriteRunInfo::WriteError("CCdnClusterMgr::GetParentsCdn2 : Ip(%u) not find Lay", nIp);
		nLay = -1;
		m_rwlkCdnClusterMgr.ReleaseLock();
		return -1;
	}
	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return 0;	
}

void* CCdnClusterMgr::ReloadThread(void* arg)
{
	CCdnClusterMgr* pMgr = (CCdnClusterMgr*)arg;
	while(true)
	{
		sleep(pMgr->m_nReloadInterval);
		int nRet = pMgr->LoadFromDb();
		if (nRet == 0)
		{
			WRITE_ALL_LOG(CDNCTLMGRSRV_LOG,"CCdnClusterMgr::ReloadThread : LoadFromDb success");
			pMgr->WriteLog();
		}
		else
		{
			WriteRunInfo::WriteError("CCdnClusterMgr::ReloadThread : LoadFromDb fail, nRet=%d", nRet);
		}
	}
	return NULL;
}

int CCdnClusterMgr::GetCdnSrvInfo(unsigned int nIp, vector<TCdnSrvInfo>& vecCdnInfo)				//add by yinhaichen 2011-03-17
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrMpaIp2ClusterId = m_mapIp2ClusterId.find(nIp);									//find cluster id.
	if (itrMpaIp2ClusterId != m_mapIp2ClusterId.end())
	{
		map<unsigned int, TCdnClusterInfo>::iterator itrCdnClusterInfo = m_mapCdnCluster.find(itrMpaIp2ClusterId->second);		//find cluster info.
		if (itrCdnClusterInfo != m_mapCdnCluster.end())
		{
			vecCdnInfo = itrCdnClusterInfo->second.m_vecCdnSrvInfo;
			WRITE_TRACE("CCdnClusterMgr::GetCdnSrvInfo : Ip(%u) find cdnserver info", nIp);
		}
		else
		{
			nRet = -1;
			WRITE_TRACE("CCdnClusterMgr::GetCdnSrvInfo : Ip(%u) not find cdnserver info", nIp);
		}
	}
	else
	{
		nRet = -1;
	}
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;
}

int CCdnClusterMgr::GetClusterId(unsigned int nIp, unsigned int& nClusterId)
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIp2Cluster =  m_mapIp2ClusterId.find(nIp);
	if (itrIp2Cluster != m_mapIp2ClusterId.end())
	{
		nClusterId = itrIp2Cluster->second;
	}
	else
	{
		nClusterId = 0xffff;
		nRet = -1;
		WRITE_TRACE("CCdnClusterMgr::GetLimitSpeed : Ip(%u) not find clusterId", nIp);
	}
	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;	
}

int CCdnClusterMgr::GetHostId(unsigned int nIp, unsigned int& nHostId)
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIp2HostId =  m_mapIp2HostId.find(nIp);
	if (itrIp2HostId != m_mapIp2HostId.end())
	{
		nHostId = itrIp2HostId->second;
	}
	else
	{
		nHostId = 0xffff;
		nRet = -1;
		WRITE_TRACE("CCdnClusterMgr::GetLimitSpeed : Ip(%u) not find hostId", nIp);
	}
	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;	
}

int CCdnClusterMgr::GetIsp(unsigned int nClusterId, char& nIsp)
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	
	map<unsigned int, TClusterConf>::iterator itrClusterConf =  m_mapClusterConf.find(nClusterId);
	if (itrClusterConf != m_mapClusterConf.end())
	{
		nIsp = itrClusterConf->second.m_nIsp;
	}
	else
	{
		nRet = -1;
		nIsp = -1;
		WRITE_TRACE("CCdnClusterMgr::GetIsp : ClusterId(%u) not find cdnserver info", nClusterId);
	}
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;
}

int CCdnClusterMgr::GetLimitSpeed(unsigned int nIp, char& nLimitMode, unsigned int& nLimitSpeed, unsigned int& nMaxSpeed, unsigned int& nMultipleSpeed)
{
	int nRet = 0;
	m_rwlkCdnClusterMgr.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIp2Cluster =  m_mapIp2ClusterId.find(nIp);
	if (itrIp2Cluster != m_mapIp2ClusterId.end())
	{
		map<unsigned int, TClusterConf>::iterator itrClusterConf =  m_mapClusterConf.find(itrIp2Cluster->second);
		if (itrClusterConf != m_mapClusterConf.end())
		{
			nLimitMode = (int)itrClusterConf->second.m_nLimitMode;
			nLimitSpeed = (int)itrClusterConf->second.m_nSpeedLimit;
			nMaxSpeed = (int)itrClusterConf->second.m_nMaxLimit;
			nMultipleSpeed = (int)itrClusterConf->second.m_nMultipleLimit;
		}
		else
		{
			nLimitMode = 0;
			nLimitSpeed = 0;
			nMaxSpeed = 0;
			nMultipleSpeed = 0;
		}
	}
	else
	{
		nLimitSpeed = 0;
		nRet = -1;
		WRITE_TRACE("CCdnClusterMgr::GetLimitSpeed : Ip(%u) not find cdnserver info", nIp);
	}
	
	m_rwlkCdnClusterMgr.ReleaseLock();
	return nRet;
}

int CCdnClusterMgr::CheckIfHaveCircleRelation(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnRelation)//add by yinhaichen 2011-03-17
{
	set<unsigned int> initSet;
	map<unsigned int, vector<TCdnRelationInfo> >::iterator itrCheckCluster =  mapCdnRelation.begin();
	while(itrCheckCluster != mapCdnRelation.end())
	{
		if (CheckCircle(mapCdnRelation, itrCheckCluster, initSet))
		{
			return 1;
		}

		itrCheckCluster ++;
	}
	return 0;
}

bool CCdnClusterMgr::CheckCircle(map<unsigned int, vector<TCdnRelationInfo> >& inMap, map<unsigned int, vector<TCdnRelationInfo> >::iterator& inIterNode, set<unsigned int>& inSet)//add by yinhaichen 2011-03-30
{
	for (unsigned int i = 0; i < inIterNode->second.size(); i ++)
	{
		set<unsigned int>::iterator itrSetTmp = inSet.find(inIterNode->second[i].m_nParentClusterId);
		if (itrSetTmp != inSet.end())
		{
            //////////////////////////////////////////////////////////////
		    WriteRunInfo::WriteError("GOT CIRCLE [%d,%d]", itrSetTmp, inIterNode->second[i].m_nClusterId);
            //////////////////////////////////////////////////////////////
			return true;
		}
		else
		{
			set<unsigned int> tmpSet = inSet;
			tmpSet.insert(inIterNode->second[i].m_nClusterId);
			map<unsigned int, vector<TCdnRelationInfo> >::iterator itrCheckCluster = inMap.find(inIterNode->second[i].m_nParentClusterId);
			if (itrCheckCluster != inMap.end())
			{
				if (CheckCircle(inMap, itrCheckCluster, tmpSet))
				{
					return true;
				}
			}
		}
	}
	
	return false;
}
