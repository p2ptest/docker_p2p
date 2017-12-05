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
#ifndef __TASKMANAGER_H__
#define __TASKMANAGER_H__

#include "framecommon/framecommon.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

class CTaskManager
{
public:
	static int Create();
	static int Destroy();
	static CTaskManager *GetInstance()
	{
		return m_pInstance;
	}

private:
	CTaskManager();
	~CTaskManager();

public:
	int Init();
	int InsertTask(const TTaskInfo &tInfo);
	int DeleteTask(const std::string &strGcid);
	int GetCacheTask(unsigned int nIp, vector<TTaskInfo> &vecTasks);
	int DelCacheTask(unsigned int nIp, const std::string &strGcid);
	int LoadClusterInfo();
	int GetOverTimeTask(vector<TTaskInfo> &vecTask);
	int AddDownloadTaskSuc(const std::string &strGcid);
	int FindTask(const std::string &strGcid, TTaskInfo &tInfo);
	int IfExist(const std::string &strGcid);

	unsigned int GetClusterNum();
	unsigned int GetHotClusterNum();
	unsigned int GetColdClusterNum();
	unsigned int GetFullClusterNum();
	int GetHostIndex(unsigned int nIp, unsigned int &nClusterId, unsigned int &nIndex, unsigned int &nNum);
	//GCID应该被分被哪台NODE上
	int GetHostIndexByGCid(unsigned int nClusterId, string strGCid, unsigned int &nIndex);

protected:
	int InitDataBase();
	int LoadTaskInfoFromDb();
	int ReplaceTaskToDb(const TTaskInfo &tInfo);
	int DeleteTaskFromDb(const std::string &strGcid);

protected:
	int InsertTaskToCluster(const TTaskInfo &tInfo, unsigned int nClusterId = 0);
	int InsertTaskToCluster(const TTaskInfo &tInfo, map<unsigned int, TClusterTask>::iterator &itrClusterTask);
	void PrintClusterInfo();

private:
    //原始任务
	CThreadRwLock m_mtxBigTaskInfo;
	map<string, TTaskInfo> m_mapTaskInfo;

	//集群任务（由原始任务通过InsertTaskToCluster函数打散生成）
	CThreadRwLock m_mtxClusterTaskInfo;
	map<unsigned int, TClusterTask> m_mapClusterTaskInfo;//first=clusterid
	map<unsigned int, CConsistentHash> m_mapConsistentHash;//first=clusterid
	map<unsigned int, unsigned int> m_mapIptoClusterId;
	map<unsigned int, unsigned int> m_mapIptoHostId;
	unsigned int m_nClusterNum;
	unsigned int m_nHotClusterNum;
	unsigned int m_nColdClusterNum;
	unsigned int m_nFullClusterNum;

private:
	static CTaskManager *m_pInstance;
};

#endif
