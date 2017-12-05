/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: taskmanager.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-04-01			1.0			
* =======================================================================
*/
#include <algorithm>
#include "taskmanager.h"
#include "mastersrv.h"
#include "dbbackmgr.h"

class CompareServerTask
{
public:
	bool operator()(const TServerTask &tLeftTask, const TServerTask &tRightTask) const
	{
		return tLeftTask.m_nHostId <= tRightTask.m_nHostId;
	}
};

//static member
CTaskManager *CTaskManager::m_pInstance = NULL;

int CTaskManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CTaskManager;
	}
	return 0;
}

int CTaskManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	return 0;
}

CTaskManager::CTaskManager()
{
	m_nClusterNum = 0;
	m_nHotClusterNum = 0;
	m_nColdClusterNum = 0;
	m_nFullClusterNum = 0;
}

CTaskManager::~CTaskManager()
{

}

int CTaskManager::Init()
{
	int nRet = InitDataBase();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("CTaskManager InitDataBase fail");
		return nRet;
	}
	
	nRet = LoadClusterInfo();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("CTaskManager LoadClusterInfo fail");
		return nRet;
	}

	nRet = LoadTaskInfoFromDb();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("CTaskManager LoadTaskInfoFromDb fail");
		return nRet;
	}
	
	return 0;
}

int CTaskManager::InsertTask(const TTaskInfo &tInfo)
{
	std::string strTaskId = CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size());

	m_mtxBigTaskInfo.GetWriteLock();
	int nRet = ReplaceTaskToDb(tInfo);
	if (nRet == 0)
	{
		std::map<std::string, TTaskInfo>::iterator itrTask = m_mapTaskInfo.find(tInfo.m_strGcid);
		if (itrTask != m_mapTaskInfo.end())
		{
			itrTask->second = tInfo;
		}
		else
		{
			m_mapTaskInfo.insert(make_pair(tInfo.m_strGcid, tInfo));
		}
	}
	m_mtxBigTaskInfo.ReleaseLock();

	if (nRet == 0)
	{
		InsertTaskToCluster(tInfo); //InsertTask
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::InsertTask : insert task %s to db", strTaskId.c_str());
	}
	return nRet;
}

int CTaskManager::DeleteTask(const std::string &strGcid)
{
	TTaskInfo tTaskInfo;
	tTaskInfo.m_strGcid = strGcid;

	m_mtxBigTaskInfo.GetWriteLock();
	std::map<std::string, TTaskInfo>::iterator itrTask = m_mapTaskInfo.find(strGcid);
	if (itrTask != m_mapTaskInfo.end())
	{
		if (DeleteTaskFromDb(strGcid) != 0)
		{
			m_mtxBigTaskInfo.ReleaseLock();
			return -1;
		}
		m_mapTaskInfo.erase(itrTask);
	}
	m_mtxBigTaskInfo.ReleaseLock();

	WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::DeleteTask : erase task %s from local task list and DB",
		   	CToolKit::BiToHex(strGcid.c_str(), strGcid.size()).c_str());
	return 0;
}

int CTaskManager::GetCacheTask(unsigned int nIp, vector<TTaskInfo> &vecTasks)
{
	struct in_addr tAddr;
	tAddr.s_addr = htonl(nIp);

	m_mtxClusterTaskInfo.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIpToClusterId = m_mapIptoClusterId.find(nIp);
	if (itrIpToClusterId == m_mapIptoClusterId.end())
	{
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::GetCacheTask: ip: %s to clusterid fail", inet_ntoa(tAddr));
		m_mtxClusterTaskInfo.ReleaseLock();
		return INVALID_USER;
	}

	map<unsigned int, unsigned int>::iterator itrIpToHostId = m_mapIptoHostId.find(nIp);
	if (itrIpToHostId == m_mapIptoHostId.end())
	{
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::GetCacheTask : ip: %s to hostid fail", inet_ntoa(tAddr));
		m_mtxClusterTaskInfo.ReleaseLock();
		return INVALID_USER;
	}
	
	map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.find(itrIpToClusterId->second);
	if (itrClusterTask == m_mapClusterTaskInfo.end())
	{
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::GetCacheTask : find cluster: %u fail", itrIpToClusterId->second);
		m_mtxClusterTaskInfo.ReleaseLock();
		return INVALID_USER;
	}
	
	TServerTask tServerTask;
	tServerTask.m_nHostId = itrIpToHostId->second;
	
	vector<TServerTask>::iterator itrServerTask = find(itrClusterTask->second.vecServers.begin(), itrClusterTask->second.vecServers.end(), tServerTask);
	if (itrServerTask == itrClusterTask->second.vecServers.end())
	{
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::GetCacheTask : ip: %s has no task", inet_ntoa(tAddr));
		m_mtxClusterTaskInfo.ReleaseLock();
		return INVALID_USER;
	}
	itrServerTask->m_mtxTaskInfo.GetReadLock();
	vecTasks = itrServerTask->vecTasks;
	itrServerTask->m_mtxTaskInfo.ReleaseLock();

	m_mtxClusterTaskInfo.ReleaseLock();

	WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::GetCacheTask : ip: %s get %d task", inet_ntoa(tAddr), vecTasks.size());
	return 0;
}

int CTaskManager::DelCacheTask(unsigned int nIp, const std::string &strGcid)
{
	struct in_addr tAddr;
	tAddr.s_addr = htonl(nIp);
	m_mtxClusterTaskInfo.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIpToClusterId = m_mapIptoClusterId.find(nIp);
	if (itrIpToClusterId == m_mapIptoClusterId.end())
	{
		m_mtxClusterTaskInfo.ReleaseLock();
		WriteRunInfo::WriteError("CTaskManager::DelCacheTask : cache(ip: %s) not in any cluster", inet_ntoa(tAddr));
		return INVALID_USER;
	}

	map<unsigned int, unsigned int>::iterator itrIpToHostId = m_mapIptoHostId.find(nIp);
	if (itrIpToClusterId == m_mapIptoHostId.end())
	{
		m_mtxClusterTaskInfo.ReleaseLock();
		WriteRunInfo::WriteError("CTaskManager::DelCacheTask : cache(ip: %s) has no hostid", inet_ntoa(tAddr));
		return INVALID_USER;
	}
	
	map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.find(itrIpToClusterId->second);
	if (itrClusterTask == m_mapClusterTaskInfo.end())
	{
		m_mtxClusterTaskInfo.ReleaseLock();
		WriteRunInfo::WriteError("CTaskManager::DelCacheTask : find cluster_task of cluster: %u fail", itrIpToClusterId->second);
		return INVALID_USER;
	}
	
	TServerTask tServerTask;
	tServerTask.m_nHostId = itrIpToHostId->second;
	vector<TServerTask>::iterator itrServerTask = std::find(itrClusterTask->second.vecServers.begin(), itrClusterTask->second.vecServers.end(),	tServerTask);
	if (itrServerTask == itrClusterTask->second.vecServers.end())
	{
		m_mtxClusterTaskInfo.ReleaseLock();
		WriteRunInfo::WriteError("CTaskManager::DelCacheTask : vp(ip: %s) in cluster %d has no task", inet_ntoa(tAddr), itrIpToClusterId->second);
		return INVALID_USER;
	}
	itrServerTask->m_mtxTaskInfo.GetWriteLock();
	std::vector<TTaskInfo>::iterator itrSrvTask = itrServerTask->vecTasks.begin();
	for (; itrSrvTask != itrServerTask->vecTasks.end(); ++itrSrvTask)
	{
		if (itrSrvTask->m_strGcid == strGcid)
		{
			if (WriteRunInfo::LOGTYPEARRAY[MASTERSRV_TASKDEBUG_TYPE].nLogOff)
			{
				struct in_addr tAddr;
				tAddr.s_addr = htonl(nIp);
				WriteRunInfo::WriteAllLog(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::DelCacheTask: delete task %s of vp(ip: %s) in cluster %d, type = %d",
					   	CToolKit::BiToHex(itrSrvTask->m_strGcid.c_str(), itrSrvTask->m_strGcid.size()).c_str(),
						inet_ntoa(tAddr), itrIpToClusterId->second, itrSrvTask->m_nTaskType);
			}
			if (itrSrvTask->m_nTaskType == ETT_DownLoadTask)
			{
				itrServerTask->vecTasks.erase(itrSrvTask);
			}
			else
			{
				itrServerTask->m_mtxTaskInfo.ReleaseLock();
				m_mtxClusterTaskInfo.ReleaseLock();
				WriteRunInfo::WriteError("CTaskManager::DelCacheTask: not download task %s of vp(ip: %s) in cluster %d, type = %d",
					   	CToolKit::BiToHex(itrSrvTask->m_strGcid.c_str(), itrSrvTask->m_strGcid.size()).c_str(),
						inet_ntoa(tAddr), itrIpToClusterId->second, itrSrvTask->m_nTaskType);
				return NOT_DOWNLOAD_TASK;
			}
			break;
		}
	}
	itrServerTask->m_mtxTaskInfo.ReleaseLock();
	m_mtxClusterTaskInfo.ReleaseLock();
	return 0;
}

int CTaskManager::LoadClusterInfo()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	char szSqlBuf[1024] = {0};
	map<unsigned int, TClusterTask> mapClusterId;
	map<unsigned int, TClusterTask> mapHostInfo;
	map<unsigned int, unsigned int> tmpMapIpToClusterId;
	map<unsigned int, unsigned int> tmpMapIpToHostId;
	map<unsigned int, TClusterTask>::iterator itrHostInfo;
	map<unsigned int, TClusterTask>::iterator itrClusterTask;

	map<unsigned int, list<int> > mapCluster2Nodes;
	int nHotClusterNum = 0;
	int nColdClusterNum = 0;
	int nFullClusterNum = 0;
	int nHotHostNum = 0;
	int nColdHostNum = 0;
	int nFullHostNum = 0;
	int nNormalHashNum = 0;
	int nConsistenHashNum = 0;

	try
	{
		CMysql mySql;
		mySql.Init();
		mySql.Connect(conf.tClusterInfoDb.strIp, conf.tClusterInfoDb.strUser, conf.tClusterInfoDb.strPasswd);

		//（A）加载集群信息
		string strQuery = "select * from %s.%s where isp > 0;";
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tClusterInfoDb.strDbName.c_str(), CLUSTER_CONF.c_str());
		mySql.Query(szSqlBuf);
		while (mySql.FetchRow())//load cluster info
		{
			unsigned int nClusterId = strtoul(mySql.GetField("ClusterId"), 0, 10);
			unsigned int nState = strtoul(mySql.GetField("status"), 0, 10);
			unsigned int nClusterType  = (unsigned int)strtoul(mySql.GetField("ishot"), 0, 10);
			unsigned int nHashType  = (unsigned int)strtoul(mySql.GetField("hash"), 0, 10);

			if (nClusterType != ECT_Cold && nClusterType != ECT_Hot && nClusterType != ECT_Full)
			{
				WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::LoadClusterInfo : wrong cluster type %d %d", nClusterId, nClusterType);
				continue;
			}

			if (nState == 1) //m_nState整个集群可用
			{
				if (nClusterType == ECT_Cold)
				{
					nColdClusterNum ++;
				}
				else if (nClusterType == ECT_Hot)
				{
					nHotClusterNum ++;
				}
				else if (nClusterType == ECT_Full)
				{
					nFullClusterNum ++;
				}

				if (nHashType == EHT_Normal)
				{
					nNormalHashNum ++;
				}
				else if (nHashType == EHT_Consistent)
				{
					nConsistenHashNum ++;
				}

				TClusterTask tTmpClusterTask;
				tTmpClusterTask.nClusterType = nClusterType;
				tTmpClusterTask.nHashType = nHashType;
				mapClusterId.insert(make_pair(nClusterId, tTmpClusterTask));
			}
		}
		mySql.FreeResult();

		//（B）加载主机信息
		strQuery = "select * from %s.%s order by hostid;";
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tClusterInfoDb.strDbName.c_str(), CLUSTER_INFO.c_str());
		mySql.Query(szSqlBuf);
		while (mySql.FetchRow())//load host info
		{
			unsigned int nClusterId = strtoul(mySql.GetField("clusterid"), 0, 10);
			unsigned int nHostId = strtoul(mySql.GetField("hostid"), 0, 10);
			string strIp = mySql.GetField("vpexternalip");
			unsigned int nVpExternalIp = ntohl(inet_addr(strIp.c_str()));
			unsigned int nEnableFlag = strtoul(mySql.GetField("enableflag"), 0, 10);

			itrClusterTask = mapClusterId.find(nClusterId);
			if (itrClusterTask != mapClusterId.end())
			{
				if (itrClusterTask->second.nHashType == EHT_Consistent)
				{
					if (nEnableFlag == 0) //对于一致性Hash,忽略被禁用的主机.
					{
						continue;
					}
					mapCluster2Nodes[nClusterId].push_back(nHostId);
				}

				if (itrClusterTask->second.nClusterType == ECT_Hot)
				{
					nHotHostNum ++;
				}
				else if (itrClusterTask->second.nClusterType == ECT_Cold)
				{
					nColdHostNum ++;
				}
				else if (itrClusterTask->second.nClusterType == ECT_Full)
				{
					nFullHostNum ++;
				}
			}

			TServerTask tTmpServerTask;
			tTmpServerTask.m_nHostId = nHostId;
			tTmpServerTask.m_nIp = nVpExternalIp;
			tTmpServerTask.m_nState = nEnableFlag; //单台机器可用

			itrHostInfo = mapHostInfo.find(nClusterId);
			if (itrHostInfo != mapHostInfo.end())
			{
				itrHostInfo->second.vecServers.push_back(tTmpServerTask);
			}
			else
			{
				TClusterTask tTmpClusterTask;
				tTmpClusterTask.vecServers.push_back(tTmpServerTask);
				mapHostInfo.insert(make_pair(nClusterId, tTmpClusterTask));
			}
			tmpMapIpToClusterId[nVpExternalIp] = nClusterId;
			tmpMapIpToHostId[nVpExternalIp] = nHostId;

		}
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::LoadClusterInfo : fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : H:[%d|%d] C:[%d|%d] F:[%d|%d] Hash:[%d|%d]", 
			nHotClusterNum, nHotHostNum, nColdClusterNum, nColdHostNum, 
			nFullClusterNum, nFullHostNum, nNormalHashNum, nConsistenHashNum);

	m_mtxClusterTaskInfo.GetWriteLock();

	//（1）处理m_mapClusterTaskInfo中集群一级的东西（增，删，改）
	map<unsigned int, TClusterTask>::iterator itrClusterId = mapClusterId.begin();
	while (itrClusterId != mapClusterId.end())//change cluster info
	{
		itrClusterTask = m_mapClusterTaskInfo.find(itrClusterId->first);
		if (itrClusterTask != m_mapClusterTaskInfo.end())
		{
			if (itrClusterTask->second.nClusterType != itrClusterId->second.nClusterType)
			{
				WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Cluster Update ClusterType [%d]:[%d|%d]", 
						itrClusterTask->first, itrClusterTask->second.nClusterType, itrClusterId->second.nClusterType); 
				itrClusterTask->second.nClusterType = itrClusterId->second.nClusterType;
			}

			if (itrClusterTask->second.nHashType != itrClusterId->second.nHashType)
			{
				WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Cluster Update HashType [%d]:[%d|%d]", 
						itrClusterTask->first, itrClusterTask->second.nHashType, itrClusterId->second.nHashType); 
				itrClusterTask->second.nHashType = itrClusterId->second.nHashType;
			}
		}
		else
		{
			WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Cluster Add [%d]", itrClusterTask->first);
			m_mapClusterTaskInfo.insert(make_pair(itrClusterId->first, itrClusterId->second));
		}
		itrClusterId ++;
	}

	itrClusterTask = m_mapClusterTaskInfo.begin();
	while (itrClusterTask != m_mapClusterTaskInfo.end())//erase useless cluster
	{
		if (mapClusterId.find(itrClusterTask->first) == mapClusterId.end())
		{
			WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Cluster Del [%d]", itrClusterTask->first);
			m_mapClusterTaskInfo.erase(itrClusterTask ++);
			continue;
		}
		itrClusterTask ++;
	}

	//（2）处理m_mapClusterTaskInfo中主机一级的东西 （增，删，改）
	vector<TServerTask>::iterator itrServerTask;
	vector<TServerTask>::iterator itrServerTask2;
	itrHostInfo = mapHostInfo.begin();
	while (itrHostInfo != mapHostInfo.end())//change host info
	{
		itrClusterTask = m_mapClusterTaskInfo.find(itrHostInfo->first);
		if (itrClusterTask != m_mapClusterTaskInfo.end())
		{
			for (unsigned int i = 0; i < itrHostInfo->second.vecServers.size(); i ++)
			{
				itrServerTask = find(itrClusterTask->second.vecServers.begin(), itrClusterTask->second.vecServers.end(), itrHostInfo->second.vecServers[i]);
				if (itrServerTask != itrClusterTask->second.vecServers.end())
				{
					if (itrServerTask->m_nIp != itrHostInfo->second.vecServers[i].m_nIp)
					{
						char szIp1[16];
						inet_ntop(AF_INET, &itrServerTask->m_nIp, szIp1, sizeof(szIp1));

						char szIp2[16];
						inet_ntop(AF_INET, &(itrHostInfo->second.vecServers[i].m_nIp), szIp2, sizeof(szIp2));

						WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Host Update IP [%d]:[%s|%s]", 
								itrServerTask->m_nHostId, szIp1, szIp2);

						itrServerTask->m_nIp = itrHostInfo->second.vecServers[i].m_nIp;
					}

					if (itrServerTask->m_nState != itrHostInfo->second.vecServers[i].m_nState) //状态变化 
					{
						WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Host Update State [%d]:[%d|%d]", 
								itrServerTask->m_nHostId, itrServerTask->m_nState, itrHostInfo->second.vecServers[i].m_nState);
						itrServerTask->m_nState = itrHostInfo->second.vecServers[i].m_nState;
					}
				}
				else
				{
					WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Host Add [%d]", itrHostInfo->second.vecServers[i].m_nHostId);
					itrClusterTask->second.vecServers.push_back(itrHostInfo->second.vecServers[i]);
					sort(itrClusterTask->second.vecServers.begin(), itrClusterTask->second.vecServers.end(), CompareServerTask());
				}
			}

			itrServerTask2 = itrClusterTask->second.vecServers.begin();
			while (itrServerTask2 != itrClusterTask->second.vecServers.end())
			{
				itrServerTask = find(itrHostInfo->second.vecServers.begin(), itrHostInfo->second.vecServers.end(), *itrServerTask2);
				if (itrServerTask == itrHostInfo->second.vecServers.end())
				{
					WriteRunInfo::WriteLog("CTaskManager::LoadClusterInfo : Host Del [%d]", itrServerTask->m_nHostId);
					itrServerTask2 = itrClusterTask->second.vecServers.erase(itrServerTask2);
					continue;
				}
				itrServerTask2 ++;
			}
		}
		itrHostInfo ++;
	}

	//（3）更新一致性Hash （增，删，改）
	map<unsigned int, list<int> >::iterator itrMap = mapCluster2Nodes.begin();
	for (; itrMap != mapCluster2Nodes.end(); itrMap++)
	{
		m_mapConsistentHash[itrMap->first].UpdateNode(itrMap->second);
	}

	map<unsigned int, CConsistentHash >::iterator itrMap2 = m_mapConsistentHash.begin();
	while (itrMap2 != m_mapConsistentHash.end())
	{
		itrMap = mapCluster2Nodes.find(itrMap2->first);
		if (itrMap == mapCluster2Nodes.end())
		{
			m_mapConsistentHash.erase(itrMap2++);
		}
		else
		{
			itrMap2++;
		}
	}
	
	//（4）统计成员 m_nClusterNum  m_nHotClusterNum;
	m_nClusterNum = m_mapClusterTaskInfo.size();
	m_nHotClusterNum = 0;
	m_nColdClusterNum = 0;
	m_nFullClusterNum = 0;
	itrClusterTask = m_mapClusterTaskInfo.begin();
	while (itrClusterTask != m_mapClusterTaskInfo.end())
	{
		if (itrClusterTask->second.nClusterType == ECT_Hot)
		{
			m_nHotClusterNum ++;
		}
		else if (itrClusterTask->second.nClusterType == ECT_Cold)
		{
			m_nColdClusterNum ++;
		}
		else if (itrClusterTask->second.nClusterType == ECT_Full)
		{
			m_nFullClusterNum ++;
		}
		itrClusterTask ++;
	}
	m_mapIptoClusterId.swap(tmpMapIpToClusterId);
	m_mapIptoHostId.swap(tmpMapIpToHostId);

	m_mtxClusterTaskInfo.ReleaseLock();

	PrintClusterInfo();

	return 0;
}

int CTaskManager::AddDownloadTaskSuc(const std::string &strGcid)
{
	m_mtxBigTaskInfo.GetWriteLock();
	std::map<std::string, TTaskInfo>::iterator itrTask = m_mapTaskInfo.find(strGcid);
	if (itrTask == m_mapTaskInfo.end() || itrTask->second.m_nTaskType == EOT_Del)
	{
		m_mtxBigTaskInfo.ReleaseLock();
		return TASK_NOT_FIND; //7
	}
	itrTask->second.m_nCopyNum ++; //Task DownloadSuc
	itrTask->second.m_nLastReportTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
	ReplaceTaskToDb(itrTask->second);
	
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	m_mtxClusterTaskInfo.GetReadLock();
	unsigned int nHotClusterNum = m_nHotClusterNum;
	unsigned int nFullClusterNum = m_nFullClusterNum;
	unsigned int nColdClusterNum = m_nColdClusterNum;
	m_mtxClusterTaskInfo.ReleaseLock();

	unsigned int nPercentNum = 0;
	if (itrTask->second.m_nTaskType == EOT_Pub)
	{
		if (itrTask->second.m_nSourceType == EST_HotSource)
		{
			nPercentNum = (unsigned int)((nHotClusterNum + nFullClusterNum) * conf.fSourcePrecent);
		}
		else
		{
			nPercentNum = (unsigned int)((nColdClusterNum + nFullClusterNum) * conf.fSourcePrecent);
		}
	}
	else if (itrTask->second.m_nTaskType == EOT_C2H)
	{
		nPercentNum = (unsigned int)((nHotClusterNum + nFullClusterNum) * conf.fSourcePrecent);
	}
	else if (itrTask->second.m_nTaskType == EOT_H2C)
	{
		nPercentNum = (unsigned int)(nColdClusterNum * conf.fSourcePrecent);
	}
	else if (itrTask->second.m_nTaskType == EOT_ReOnline)
	{
		nPercentNum = (unsigned int)((nHotClusterNum + nFullClusterNum) * conf.fSourcePrecent);
	}
	////////////////////////////////////////////////////////////////////

	if (itrTask->second.m_nCopyNum >= nPercentNum)
	{
		m_mtxBigTaskInfo.ReleaseLock();
		return TASK_SUC; //8
	}
	m_mtxBigTaskInfo.ReleaseLock();
	return 0;
}

int CTaskManager::FindTask(const std::string &strGcid, TTaskInfo &tInfo)
{
	TTaskInfo tTaskInfo;
	tTaskInfo.m_strGcid = strGcid;

	int nRet = 0;
	m_mtxBigTaskInfo.GetReadLock();
	std::map<std::string, TTaskInfo>::iterator itrTask = m_mapTaskInfo.find(strGcid);
	if (itrTask != m_mapTaskInfo.end())
	{
		nRet = 1;
		tInfo = itrTask->second;
	}
	m_mtxBigTaskInfo.ReleaseLock();
	return nRet;
}

int CTaskManager::IfExist(const std::string &strGcid)
{
	TTaskInfo tTaskInfo;
	tTaskInfo.m_strGcid = strGcid;

	int nRet = 0;
	m_mtxBigTaskInfo.GetReadLock();
	std::map<std::string, TTaskInfo>::iterator itrTask = m_mapTaskInfo.find(strGcid);
	if (itrTask != m_mapTaskInfo.end())
	{
		nRet = 1;
	}
	m_mtxBigTaskInfo.ReleaseLock();
	return nRet;
}

unsigned int CTaskManager::GetClusterNum()
{
	unsigned int nClusterNum = 0;
	m_mtxClusterTaskInfo.GetReadLock();
	nClusterNum = m_nClusterNum;
	m_mtxClusterTaskInfo.ReleaseLock();
	return nClusterNum;
}

unsigned int CTaskManager::GetHotClusterNum()
{
	unsigned int nHotClusterNum = 0;
	m_mtxClusterTaskInfo.GetReadLock();
	nHotClusterNum = m_nHotClusterNum;
	m_mtxClusterTaskInfo.ReleaseLock();
	return nHotClusterNum;
}

unsigned int CTaskManager::GetColdClusterNum()
{
	unsigned int nColdClusterNum = 0;
	m_mtxClusterTaskInfo.GetReadLock();
	nColdClusterNum = m_nColdClusterNum;
	m_mtxClusterTaskInfo.ReleaseLock();
	return nColdClusterNum;
}

unsigned int CTaskManager::GetFullClusterNum()
{
	unsigned int nFullClusterNum = 0;
	m_mtxClusterTaskInfo.GetReadLock();
	nFullClusterNum = m_nFullClusterNum;
	m_mtxClusterTaskInfo.ReleaseLock();
	return nFullClusterNum;
}

int CTaskManager::GetOverTimeTask(vector<TTaskInfo> &vecTask)
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	if (NULL == pServer)
	{
		return -1;
	}

	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	unsigned int nCurTime = pServer->GetTimeVal().tv_sec;
	
	m_mtxClusterTaskInfo.GetReadLock();

	map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.begin();
	while (itrClusterTask != m_mapClusterTaskInfo.end())
	{
		vector<TServerTask>::iterator itrServerTask = itrClusterTask->second.vecServers.begin();
		while (itrServerTask != itrClusterTask->second.vecServers.end())
		{
			itrServerTask->m_mtxTaskInfo.GetWriteLock();
			vector<TTaskInfo>::iterator itrTask = itrServerTask->vecTasks.begin();
			while (itrTask != itrServerTask->vecTasks.end())
			{
				if (nCurTime - itrTask->m_nStartTime >= (unsigned int)conf.nTaskOverTime)
				{
					if (WriteRunInfo::LOGTYPEARRAY[MASTERSRV_TASKDEBUG_TYPE].nLogOff)
					{
						struct in_addr tAddr;
						tAddr.s_addr = htonl(itrServerTask->m_nIp);
						WriteRunInfo::WriteAllLog(MASTERSRV_TASKDEBUG_TYPE, "erase timeout task %s of vp(ip: %s) in cluster %d",
									  CToolKit::BiToHex(itrTask->m_strGcid.c_str(), itrTask->m_strGcid.size()).c_str(),
									  inet_ntoa(tAddr), itrClusterTask->first);
					}
					itrTask = itrServerTask->vecTasks.erase(itrTask);
					continue;
				}
				itrTask ++;
			}
			itrServerTask->m_mtxTaskInfo.ReleaseLock();
			itrServerTask ++;
		}
		itrClusterTask ++;
	}
	m_mtxClusterTaskInfo.ReleaseLock();

	m_mtxBigTaskInfo.GetWriteLock();
	std::map<std::string, TTaskInfo>::iterator itrTaskInfo = m_mapTaskInfo.begin();
	while (itrTaskInfo != m_mapTaskInfo.end())
	{
		if (pServer->GetTimeVal().tv_sec - itrTaskInfo->second.m_nStartTime >= (unsigned int)conf.nTaskOverTime)
		{
			WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "task %s timeout",
					CToolKit::BiToHex(itrTaskInfo->second.m_strGcid.c_str(), itrTaskInfo->second.m_strGcid.size()).c_str());
			vecTask.push_back(itrTaskInfo->second);
		}
		itrTaskInfo ++;
	}
	m_mtxBigTaskInfo.ReleaseLock();

	return 0;
}

int CTaskManager::InitDataBase()
{
	return 0;
}

int CTaskManager::LoadTaskInfoFromDb()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	unsigned int nCurTime = pServer->GetTimeVal().tv_sec;
	char szSqlBuf[1024] = {0};
	std::map<std::string, TTaskInfo> mapTaskInfo;

	try
	{
		CMysql mySql;
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);

		unsigned int nTaskNum = 0;
		TTaskInfo tTaskInfo;
		string strQuery = "select taskgcid,sourcetype,copynum,";
		strQuery += "unix_timestamp(publishtime) as publishtime,";
		strQuery += "unix_timestamp(starttime) as starttime,";
		strQuery += "unix_timestamp(lastreporttime) as lastreporttime,";
		strQuery += "tasktype,tasktarget ";
	   	strQuery += "from %s.%s;";
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tMasterDb.strDbName.c_str(), TASK_INFO_TABLE.c_str(), nCurTime);
		mySql.Query(szSqlBuf);
		while (mySql.FetchRow())
		{
			const string strGcid = mySql.GetField("taskgcid");
			tTaskInfo.m_strGcid = CToolKit::HexToBi(strGcid.c_str(), strGcid.length());
			tTaskInfo.m_nSourceType = strtol(mySql.GetField("sourcetype"), 0, 10);
			tTaskInfo.m_nCopyNum = strtoul(mySql.GetField("copynum"), 0, 10);
			tTaskInfo.m_nPublishTime = strtoul(mySql.GetField("publishtime"), 0, 10);
			tTaskInfo.m_nStartTime = strtoul(mySql.GetField("starttime"), 0, 10);
			tTaskInfo.m_nLastReportTime = strtoul(mySql.GetField("lastreporttime"), 0, 10);
			tTaskInfo.m_nTaskType = strtoul(mySql.GetField("tasktype"), 0, 10);
			tTaskInfo.m_nTarget = strtoul(mySql.GetField("tasktarget"), 0, 10);
			mapTaskInfo.insert(make_pair(tTaskInfo.m_strGcid, tTaskInfo));
			WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::LoadTaskInfoFromDb : load task: gcid = %s, type = %d, target = %d",
				   	strGcid.c_str(), tTaskInfo.m_nTaskType, tTaskInfo.m_nTarget);
			nTaskNum ++;
		}
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::LoadTaskInfoFromDb : load %u task",  nTaskNum);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::LoadTaskInfoFromDb : fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}
	
	std::map<std::string, TTaskInfo>::iterator itrTaskInfo = mapTaskInfo.begin();
	while (itrTaskInfo != mapTaskInfo.end())
	{
		InsertTaskToCluster(itrTaskInfo->second); //LoadFromDb
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::LoadTaskInfoFromDb : insert task to task list");
		++itrTaskInfo;
	}

	m_mtxBigTaskInfo.GetWriteLock();
	m_mapTaskInfo.swap(mapTaskInfo);
	m_mtxBigTaskInfo.ReleaseLock();

	return 0;
}

int CTaskManager::ReplaceTaskToDb(const TTaskInfo &tInfo)
{
	CMysql mySql;
	char szSqlBuf[1024] = {0};
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	const std::string strTaskId = CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size());
	try
	{
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);
		string strQuery = "replace into %s.%s(taskgcid,sourcetype,copynum,publishtime,starttime,lastreporttime,tasktype,tasktarget) ";
		strQuery += "values('%s',%d,%u,from_unixtime(%u),from_unixtime(%u),from_unixtime(%u),%u,%u);";
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(),
				 conf.tMasterDb.strDbName.c_str(), TASK_INFO_TABLE.c_str(),
				 strTaskId.c_str(), tInfo.m_nSourceType, tInfo.m_nCopyNum,
				 tInfo.m_nPublishTime, tInfo.m_nStartTime, tInfo.m_nLastReportTime,
				 tInfo.m_nTaskType, tInfo.m_nTarget);
		mySql.Query(szSqlBuf);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::ReplaceTaskToDb : fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}
	CDbBackMgr::GetInstance()->Query(szSqlBuf);
	return 0;
}

int CTaskManager::DeleteTaskFromDb(const std::string &strTaskId)
{
	CMysql mySql;
	char szSqlBuf[1024] = {0};
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	try
	{
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);
		const std::string strId = CToolKit::BiToHex(strTaskId.c_str(), strTaskId.size());
		snprintf(szSqlBuf, sizeof(szSqlBuf), "delete from %s.%s where taskgcid = '%s';",
				 conf.tMasterDb.strDbName.c_str(), TASK_INFO_TABLE.c_str(), strId.c_str());
		mySql.Query(szSqlBuf);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CTaskManager::DeleteTaskFromDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	CDbBackMgr::GetInstance()->Query(szSqlBuf);
	return 0;
}

int CTaskManager::InsertTaskToCluster(const TTaskInfo &tInfo, unsigned int nClusterId) //nClusterId=0表示对每个集群都尝试加入
{
	m_mtxClusterTaskInfo.GetReadLock();

	map<unsigned int, TClusterTask>::iterator itrClusterTask;
	if (nClusterId == 0)
	{
		itrClusterTask = m_mapClusterTaskInfo.begin();
		while (itrClusterTask != m_mapClusterTaskInfo.end())
		{
			InsertTaskToCluster(tInfo, itrClusterTask);
			itrClusterTask ++;
		}
	}
	else
	{
		itrClusterTask = m_mapClusterTaskInfo.find(nClusterId);
		if (itrClusterTask != m_mapClusterTaskInfo.end())
		{
			InsertTaskToCluster(tInfo, itrClusterTask);
		}
	}

	m_mtxClusterTaskInfo.ReleaseLock();
	return 0;
}

int CTaskManager::InsertTaskToCluster(const TTaskInfo& tInfo, map<unsigned int, TClusterTask>::iterator &itrClusterTask)
{
	std::vector<TServerTask> &srvTaskVec = itrClusterTask->second.vecServers;
	unsigned int nServerNum = srvTaskVec.size();
	if (nServerNum != 0)
	{
		bool bIfFound = false;
		TTaskInfo ServerTask = tInfo;
		if (tInfo.m_nTaskType == EOT_Pub)
		{
			if (tInfo.m_nSourceType == EST_HotSource)
			{
				if (itrClusterTask->second.nClusterType == ECT_Hot || 
						itrClusterTask->second.nClusterType == ECT_Full)
				{
					bIfFound = true;
					ServerTask.m_nTaskType = ETT_DownLoadTask;
				}
			}
			else
			{
				if (itrClusterTask->second.nClusterType == ECT_Cold || 
						itrClusterTask->second.nClusterType == ECT_Full)
				{
					bIfFound = true;
					ServerTask.m_nTaskType = ETT_DownLoadTask;
				}
			}
		}
		else if (tInfo.m_nTaskType == EOT_Del)
		{
			bIfFound = true;
			//ServerTask.m_nTaskType = ETT_DeleteForeverTask;
			ServerTask.m_nTaskType = ETT_DeleteTask; //暂时用假删除
		}
		else if (tInfo.m_nTaskType == EOT_C2H)
		{
			if (itrClusterTask->second.nClusterType == ECT_Hot || 
					itrClusterTask->second.nClusterType == ECT_Full)
			{
				bIfFound = true;
				ServerTask.m_nTaskType = ETT_DownLoadTask;
			}
		}
		else if (tInfo.m_nTaskType == EOT_H2C)
		{
			if (itrClusterTask->second.nClusterType == ECT_Cold)
			{
				bIfFound = true;
				ServerTask.m_nTaskType = ETT_DownLoadTask;
			}
			else if (itrClusterTask->second.nClusterType == ECT_Hot)
			{
				bIfFound = true;
			    //ServerTask.m_nTaskType = ETT_DeleteForeverTask;
			    ServerTask.m_nTaskType = ETT_DeleteTask; //暂时用假删除
			}
		}
		else if (tInfo.m_nTaskType == EOT_ReOnline)
		{
			if (itrClusterTask->second.nClusterType == ECT_Hot || 
					itrClusterTask->second.nClusterType == ECT_Full)
			{
				bIfFound = true;
				ServerTask.m_nTaskType = ETT_DownLoadTask;
			}
		}

		if (bIfFound)
		{
			//int nRealIndex = tInfo.GetIndex() % nServerNum;
			unsigned int nRealIndex = 0;
			if (GetHostIndexByGCid(itrClusterTask->first, tInfo.m_strGcid, nRealIndex) == 0)
			{
				srvTaskVec[nRealIndex].m_mtxTaskInfo.GetWriteLock();
				std::vector<TTaskInfo> &taskInfoVec(srvTaskVec[nRealIndex].vecTasks);
				std::vector<TTaskInfo>::iterator taskIter = std::find(taskInfoVec.begin(), taskInfoVec.end(), tInfo);
				if (taskIter == taskInfoVec.end()) //添加新任务
				{
					taskInfoVec.push_back(ServerTask);
					WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::InsertTaskToCluster : insert new task %s to cluster %d",
								  CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str(), 
								  itrClusterTask->first);
				}
				else //修改已有任务
				{
					*taskIter = ServerTask;
					WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::InsertTaskToCluster : update task %s in cluster %d",
								  CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str(),
								  itrClusterTask->first);
				}
				srvTaskVec[nRealIndex].m_mtxTaskInfo.ReleaseLock();

				struct in_addr tAddr;
				tAddr.s_addr = htonl(srvTaskVec[nRealIndex].m_nIp);
				WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::InsertTaskToCluster : insert task %s to [cluster:%d],[cache:%s:%d],[type:%d]",
							  CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str(),
							  itrClusterTask->first, inet_ntoa(tAddr), nRealIndex, tInfo.m_nTaskType);
			}
		}
	}
	return 0;
}

void CTaskManager::PrintClusterInfo()
{
	m_mtxClusterTaskInfo.GetReadLock();
	map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.begin();
	while (itrClusterTask != m_mapClusterTaskInfo.end())
	{
		WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "CTaskManager::PrintClusterInfo : cluster id: %u, ifhot: %u",
				itrClusterTask->first, itrClusterTask->second.nClusterType);
		vector<TServerTask>::iterator itrServerTask = itrClusterTask->second.vecServers.begin();
		while (itrServerTask != itrClusterTask->second.vecServers.end())
		{
			struct in_addr tAddr;
			tAddr.s_addr = htonl(itrServerTask->m_nIp);
			WRITE_ALL_LOG(MASTERSRV_TASKDEBUG_TYPE, "\thost id: %u, ip: %s, nip: %u, useable: %u",
					itrServerTask->m_nHostId, inet_ntoa(tAddr), itrServerTask->m_nIp, itrServerTask->m_nState);
			itrServerTask ++;
		}
		
		itrClusterTask ++;
	}
	m_mtxClusterTaskInfo.ReleaseLock();
}

int CTaskManager::GetHostIndex(unsigned int nIp, unsigned int &nClusterId, unsigned int &nIndex, unsigned int &nNum)
{
	int nRet = 0;
	m_mtxClusterTaskInfo.GetReadLock();
	map<unsigned int, unsigned int>::iterator itrIpToHostId = m_mapIptoHostId.find(nIp);
	if (itrIpToHostId == m_mapIptoHostId.end())
	{
		nRet = -1;
	}
	else
	{
		map<unsigned int, unsigned int>::iterator itrIpToClusterId = m_mapIptoClusterId.find(nIp);
		if (itrIpToClusterId == m_mapIptoClusterId.end())
		{
			nRet = -1;
		}
		else
		{
			nClusterId = itrIpToClusterId->second;
			map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.find(itrIpToClusterId->second);
			if (itrClusterTask == m_mapClusterTaskInfo.end() || itrClusterTask->second.vecServers.empty())
			{
				nRet = -1;
			}
			else
			{
				if (itrClusterTask->second.nClusterType == ECT_Cold)
				{
					//冷集群没有所谓的持久化存储,参考persissourcesyncmd
					nRet = 1;
				}
				else
				{
					nNum = itrClusterTask->second.vecServers.size();
					unsigned int nA = 0;
					for (; nA < nNum; nA ++)
					{
						if (itrClusterTask->second.vecServers[nA].m_nHostId == itrIpToHostId->second)
						{
							break;
						}
					}
					if (nA >= nNum)
					{
						nRet = -1;
					}
					else
					{
						nIndex = nA;
					}
				}
			}
		}
	}
	m_mtxClusterTaskInfo.ReleaseLock();
	return nRet;
}

//GCID应该被分被哪台NODE上，不用加锁
int CTaskManager::GetHostIndexByGCid(unsigned int nClusterId, string strGCid, unsigned int &nIndex)
{
	int nRet = -1;
	map<unsigned int, TClusterTask>::iterator itrClusterTask = m_mapClusterTaskInfo.find(nClusterId);
	if (itrClusterTask != m_mapClusterTaskInfo.end())
	{
		unsigned int nServerNum = itrClusterTask->second.vecServers.size();
		unsigned int nHashType = itrClusterTask->second.nHashType;

		if (nHashType == EHT_Consistent)
		{
			unsigned int nHostId = (unsigned int) m_mapConsistentHash[nClusterId].GetNode(strGCid);
			for (unsigned int nA = 0; nA < nServerNum; nA ++)
			{
				if (itrClusterTask->second.vecServers[nA].m_nHostId == nHostId)
				{
					nIndex = nA;
					nRet = 0;
				}
			}
		}
		else
		{
			TSourceInfo tInfo;
			tInfo.m_strGcid = strGCid;
			nIndex = tInfo.GetIndex() % nServerNum;
			nRet = 0;
		}
	}
	return nRet;
}

