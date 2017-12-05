/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcemanager.cpp
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
#include <time.h>
#include "sourcemanager.h"
#include "mastersrv.h"
#include "dbbackmgr.h"
#include "taskmanager.h"

const string SOURCE_INFO_TABLE = "source_info";
const string PAID_MOVIE_INFO_TABLE = "p2pvod_videos";
const string SHORT_MOVIE_INFO_TABLE = "p2pvod_s_videos";

#define CREATE_RESOURCE_TABLE			                                \
	"create table if not exists %s.%s                                   \
	(																	\
      `gcid` varchar(40) NOT NULL,										\
	  `sourcetype` int(10)  default 0,									\
	  `publishtime` datetime NOT NULL default '0000-00-00 00:00:00',	\
	  `lastreporttime` datetime NOT NULL default '0000-00-00 00:00:00',	\
	  `copynum`int(10) default 0,										\
	  primary key(gcid)													\
	);"                                                                 \

//static member
CSourceManager *CSourceManager::m_pInstance = NULL;

int CSourceManager::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSourceManager;
	}
	return 0;
}

int CSourceManager::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	return 0;
}

//member
CSourceManager::CSourceManager()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	for (int nA = 0; nA < SOURCETYPENUM; nA ++)
	{
		if (nA != EST_AllSource && nA != EST_DelSource)
		{
			m_sourceTable[nA] = new CSourceHashTable(conf.nSourceHashBucketLen);
		}
	}
}

CSourceManager::~CSourceManager()
{
	for (int nA = 0; nA < SOURCETYPENUM; nA ++)
	{
		if (nA != EST_AllSource && nA != EST_DelSource)
		{
			delete m_sourceTable[nA];
			m_sourceTable[nA] = NULL;
		}
	}
}

int CSourceManager::Init()
{
	int nRet = InitDataBase();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("CSourceManager::Init : InitDataBase fail");
		return nRet;
	}

	nRet = InitDataBaseBak();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("CSourceManager::Init : InitDataBaseBak fail");
		return nRet;
	}

	unsigned long timeBegin = clock();
	nRet = LoadFromDb();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("CSourceManager::Init : LoadFromDb fail");
		return nRet;
	}

	WriteRunInfo::WriteLog("CSourceManager::Init : load gcid from db const %d s.", (clock() - timeBegin) / CLOCKS_PER_SEC);
	return 0;
}

int CSourceManager::ReOnlineSource(const TSourceInfo &tInfo)
{
	if (tInfo.m_nSourceType != EST_HotSource && tInfo.m_nSourceType != EST_ColdSource)
	{
		WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::ReOnlineSource : invalid source type %d", tInfo.m_nSourceType);
		return INVALID_SOURCETYPE_ERROR;
	}
	m_mtxSource.GetWriteLock();
	TSourceInfo tTmpInfo;
	if (m_sourceTable[EST_HotSource]->FindSource(tInfo.m_strGcid, tTmpInfo) == 0)
	{
		if (m_sourceTable[EST_ColdSource]->FindSource(tInfo.m_strGcid, tTmpInfo) == 0)
		{
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::ReOnlineSource : not find gcid %s",
					CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.length()).c_str());
			return 0;
		}
	}

	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::ReOnlineSource : insert src: %s",
				CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str());

	TTaskInfo TaskInfo(tInfo);
	TaskInfo.m_nTaskType = EOT_ReOnline;
	TaskInfo.m_nCopyNum = 0;
	int nRet = CTaskManager::GetInstance()->InsertTask(TaskInfo); //ReOnline
	m_mtxSource.ReleaseLock();
	return nRet;
}

int CSourceManager::PublishSource(const TSourceInfo &tInfo)
{
	if (tInfo.m_nSourceType != EST_HotSource && tInfo.m_nSourceType != EST_ColdSource)
	{
		WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::PublishSource : invalid source type %d", tInfo.m_nSourceType);
		return INVALID_SOURCETYPE_ERROR;
	}
	m_mtxSource.GetWriteLock();
	TSourceInfo tTmpInfo;
	if (m_sourceTable[EST_HotSource]->FindSource(tInfo.m_strGcid, tTmpInfo) == 1)
	{
		m_mtxSource.ReleaseLock();
		WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::PublishSource : source %s exist (hot source)", 
			CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str());
		return PUBLISH_EXIST;
	}
	if (m_sourceTable[EST_ColdSource]->FindSource(tInfo.m_strGcid, tTmpInfo) == 1)
	{
		m_mtxSource.ReleaseLock();
		WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::PublishSource : source %s exist (cold source)",
			CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str());
		return PUBLISH_EXIST;
	}

	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::PublishSource : insert src: %s",
				CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str());

	TTaskInfo TaskInfo(tInfo);
	TaskInfo.m_nTaskType = EOT_Pub;
	TaskInfo.m_nCopyNum = 0;
	int nRet = CTaskManager::GetInstance()->InsertTask(TaskInfo); //Publish
	m_mtxSource.ReleaseLock();
	return nRet;
}

int CSourceManager::DeleteSource(const std::string &strGcid)
{
	m_mtxSource.GetWriteLock();
	int nSourceType = -1;
	if (m_sourceTable[EST_HotSource]->IfExist(strGcid))
	{
		nSourceType = EST_HotSource;
	}
	else
	{
		if (m_sourceTable[EST_ColdSource]->IfExist(strGcid))
		{
			nSourceType = EST_ColdSource;
		}
	}

	if (nSourceType < 0)
	{
		if (!CTaskManager::GetInstance()->IfExist(strGcid))
		{
			WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::DeleteSource : source %s is not exist ",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			m_mtxSource.ReleaseLock();
			return 0;
		}
	}
	else
	{
		int nRet = DeleteFromDb(strGcid);
		if (nRet != 0)
		{
			m_mtxSource.ReleaseLock();
			return nRet;
		}
		m_sourceTable[nSourceType]->DeleteSource(strGcid);  //如果完全是删除操作，就立即在内存与DB中处理，不用等待上报
		m_sourceTable[EST_PaidSource]->DeleteSource(strGcid);
	}
	
	TTaskInfo tTaskInfo;
	tTaskInfo.m_strGcid = strGcid;
	tTaskInfo.m_nSourceType = EST_DelSource;
	tTaskInfo.m_nTaskType = EOT_Del;
	tTaskInfo.m_nStartTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
	CTaskManager::GetInstance()->InsertTask(tTaskInfo);

	m_mapIncSource[strGcid] = tTaskInfo;
	m_mtxSource.ReleaseLock();
	return 0;
}

int CSourceManager::SourceHotToCold(const string &strGcid)
{
	m_mtxSource.GetWriteLock();
	TSourceInfo tSourceInfo;
	tSourceInfo.m_strGcid = strGcid;

	if (m_sourceTable[EST_PaidSource]->FindSource(strGcid, tSourceInfo) == 1)
	{
		m_mtxSource.ReleaseLock();
		WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceHotToCold (FAIL): vip gcid %s",
				CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
		return 0;
	}

	if (m_sourceTable[EST_HotSource]->FindSource(strGcid, tSourceInfo) == 0)
	{
		if (m_sourceTable[EST_ColdSource]->FindSource(strGcid, tSourceInfo) == 0)
		{
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceHotToCold (FAIL): not find gcid %s",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			return -1;
		}
		else
		{
			//Already HOT
			m_mapIncSource[strGcid] = tSourceInfo;
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceHotToCold (FAIL): already cold gcid %s",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			return 0;
		}
	}
	else
	{
		CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
		TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
		unsigned int nCurTime = pServer->GetTimeVal().tv_sec;
		if (nCurTime - tSourceInfo.m_nLastReportTime <= (unsigned int)conf.nHotMinKeepTime)
		{
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceHotToCold (FAIL): keep hot gcid %s",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			return 0;
		}
	}

	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceHotToCold : src: %s",
				CToolKit::BiToHex(strGcid.c_str(), strGcid.size()).c_str());

	tSourceInfo.m_nSourceType = EST_ColdSource; 	
	TTaskInfo tTaskInfo(tSourceInfo);
	tTaskInfo.m_nTaskType = EOT_H2C;
	tTaskInfo.m_nStartTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
	tTaskInfo.m_nCopyNum = 0;
	int nRet = CTaskManager::GetInstance()->InsertTask(tTaskInfo); //HotToCold

	m_mtxSource.ReleaseLock();

	return nRet;
}

int CSourceManager::SourceColdToHot(const string &strGcid)
{
	m_mtxSource.GetWriteLock();
	TSourceInfo tSourceInfo;
	tSourceInfo.m_strGcid = strGcid;
	if (m_sourceTable[EST_ColdSource]->FindSource(strGcid, tSourceInfo) == 0)
	{
		if (m_sourceTable[EST_HotSource]->FindSource(strGcid, tSourceInfo) == 0)
		{
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceColdToHot (FAIL): not find gcid %s",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			return -1;
		}
		else
		{
			m_mapIncSource[strGcid] = tSourceInfo;
			m_mtxSource.ReleaseLock();
			WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceColdToHot (FAIL): already hot gcid %s",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str());
			return 0;
		}
	}

	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "CSourceManager::SourceColdToHot : src: %s",
				CToolKit::BiToHex(strGcid.c_str(), strGcid.size()).c_str());
	
	tSourceInfo.m_nSourceType = EST_HotSource; 	
	TTaskInfo tTaskInfo(tSourceInfo);
	tTaskInfo.m_nTaskType = EOT_C2H;
	tTaskInfo.m_nStartTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
	tTaskInfo.m_nCopyNum = 0;
	int nRet = CTaskManager::GetInstance()->InsertTask(tTaskInfo); //ColdToHot
	m_mtxSource.ReleaseLock();

	return nRet;
}

void CSourceManager::ChecktimeOutTaskMgr()
{
	CTaskManager::GetInstance()->LoadClusterInfo();

	vector<TTaskInfo> vecTask;
	CTaskManager::GetInstance()->GetOverTimeTask(vecTask);
	if (vecTask.empty())
	{
		return;
	}
	int nTaskNum = (int)vecTask.size();
	m_mtxSource.GetWriteLock();
	for (int nA = 0; nA < nTaskNum; nA ++)
	{
		TTaskInfo &TaskInfo = vecTask[nA];

		if (TaskInfo.m_nTaskType == EOT_Del)
		{
			CTaskManager::GetInstance()->DeleteTask(TaskInfo.m_strGcid);
			continue;
		}

		if (TaskInfo.m_nTaskType == EOT_H2C)
		{
			TaskInfo.m_nCopyNum += CTaskManager::GetInstance()->GetFullClusterNum(); //Task ChecktimeOutTaskMgr EOT_H2C
		}
		//////////////////////////////////////////////////////////////
		if (UpdateToDb(TaskInfo) != 0)
		{
			break;
		}
		//////////////////////////////////////////////////////////////
		if (TaskInfo.m_nTaskType == EOT_Pub)
		{
			if (TaskInfo.m_nSourceType == EST_HotSource)
			{
				m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
			}
			else
			{
				m_sourceTable[EST_ColdSource]->AddSource(TaskInfo);
			}
		}
		else if (TaskInfo.m_nTaskType == EOT_C2H)
		{
			m_sourceTable[EST_ColdSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
		}
		else if (TaskInfo.m_nTaskType == EOT_H2C)
		{
			m_sourceTable[EST_HotSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_ColdSource]->AddSource(TaskInfo);
		}
		else if (TaskInfo.m_nTaskType == EOT_ReOnline)
		{
			m_sourceTable[EST_ColdSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
		}
		//////////////////////////////////////////////////////////////
		m_mapIncSource[TaskInfo.m_strGcid] = TaskInfo;
		CTaskManager::GetInstance()->DeleteTask(TaskInfo.m_strGcid);
	}
	m_mtxSource.ReleaseLock();
}

int CSourceManager::GetCacheTask(unsigned int nIp, vector<TTaskInfo> &vecTasks)
{
	return CTaskManager::GetInstance()->GetCacheTask(nIp, vecTasks);
}

//only report download task
int CSourceManager::ReportCacheTask(unsigned int nIp, TStateInfo &TaskState)
{
	int nRet = CTaskManager::GetInstance()->DelCacheTask(nIp, TaskState.m_strGcid);
	if (nRet == INVALID_USER || nRet == NOT_DOWNLOAD_TASK)
	{
		return nRet;
	}
	nRet = CTaskManager::GetInstance()->AddDownloadTaskSuc(TaskState.m_strGcid); //+1计数，只操作m_mapTaskInfo
    //nRet返回值三种情况 0:正常+1计数 7:任务没有找到 8:(TASK_SUC)任务找到达到下载阈值
	if (nRet != TASK_SUC)
	{
		if (nRet == TASK_NOT_FIND) //超过percent后如果还有上报，看情况增加copynum
		{
			AddCopyNum(TaskState.m_strGcid); //+1计数，不是操作m_mapTaskInfo而是操作最终的HashTable的值
		}
		return 0;
	}
	//达到下载阈值了
	TTaskInfo TaskInfo;
	if (CTaskManager::GetInstance()->FindTask(TaskState.m_strGcid, TaskInfo) <= 0)
	{
		return -1;
	}
	m_mtxSource.GetWriteLock();
	nRet = 0;
	do
	{
		if (TaskInfo.m_nTaskType == EOT_Del)
		{
			break;
		}
		//////////////////////////////////////////////////////////////
		if (TaskInfo.m_nTaskType == EOT_H2C)
		{
			TaskInfo.m_nCopyNum += CTaskManager::GetInstance()->GetFullClusterNum(); //Task ReportCacheTask EOT_H2C
		}
		//////////////////////////////////////////////////////////////
		if (UpdateToDb(TaskInfo) != 0)
		{
			nRet = -1;
			break;
		}
		//////////////////////////////////////////////////////////////
		if (TaskInfo.m_nTaskType == EOT_Pub)
		{
			if (TaskInfo.m_nSourceType == EST_HotSource)
			{
				m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
			}
			else
			{
				m_sourceTable[EST_ColdSource]->AddSource(TaskInfo);
			}
		}
		else if (TaskInfo.m_nTaskType == EOT_C2H)
		{
			m_sourceTable[EST_ColdSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
		}
		else if (TaskInfo.m_nTaskType == EOT_H2C)
		{
			m_sourceTable[EST_HotSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_ColdSource]->AddSource(TaskInfo);
		}
		else if (TaskInfo.m_nTaskType == EOT_ReOnline)
		{
			m_sourceTable[EST_ColdSource]->DeleteSource(TaskInfo.m_strGcid);
			m_sourceTable[EST_HotSource]->AddSource(TaskInfo);
		}
		//////////////////////////////////////////////////////////////
		m_mapIncSource[TaskInfo.m_strGcid] = TaskInfo;
	} while(0);
	m_mtxSource.ReleaseLock();
	
	//达到下载阈值了,删除这个下载任务
	if (nRet == 0)
	{
		CTaskManager::GetInstance()->DeleteTask(TaskState.m_strGcid);
	}

	return nRet;
}

int CSourceManager::AddCopyNum(const string &strGcid)
{
	TSourceInfo SourceInfo;
	m_mtxSource.GetWriteLock();
	int nRet = m_sourceTable[EST_HotSource]->FindSource(strGcid, SourceInfo);
	if (nRet <= 0)
	{
		nRet = m_sourceTable[EST_ColdSource]->FindSource(strGcid, SourceInfo);
		if (nRet <= 0)
		{
			m_mtxSource.ReleaseLock();
			return 0;
		}
	}

	unsigned int nCopyNum = 0;
	if (SourceInfo.m_nSourceType == EST_HotSource)
	{
		nCopyNum = CTaskManager::GetInstance()->GetHotClusterNum() + CTaskManager::GetInstance()->GetFullClusterNum();
	}
	else if (SourceInfo.m_nSourceType == EST_ColdSource)
	{
		nCopyNum = CTaskManager::GetInstance()->GetColdClusterNum() + CTaskManager::GetInstance()->GetFullClusterNum();
	}
	else
	{
		WriteRunInfo::WriteError("CSourceManager::AddCopyNum : error m_nSourceType type %d", SourceInfo.m_nSourceType);
		m_mtxSource.ReleaseLock();
		return 0;
	}

	if (SourceInfo.m_nCopyNum >= nCopyNum)
	{
		m_mtxSource.ReleaseLock();
		return 0;
	}
	else
	{
		m_sourceTable[SourceInfo.m_nSourceType]->AddCopyNum(strGcid);
		SourceInfo.m_nCopyNum ++; //By CSourceManager::AddCopyNum
		UpdateToDb(SourceInfo);
		m_mtxSource.ReleaseLock();
		return 0;
	}

	return 0;
}

int CSourceManager::CheckPaidSource()
{
	int nRet = 0;
	vector<TSourceInfo> vec;
	nRet = LoadPaidFromDb(vec);
	if (nRet == 0)
	{
		m_mtxSource.GetWriteLock();
		m_sourceTable[EST_PaidSource]->Clear();
		vector<TSourceInfo>::iterator itrVec = vec.begin();
		while (itrVec != vec.end())
		{
			TSourceInfo tSourceInfo;
			tSourceInfo.m_strGcid = itrVec->m_strGcid;

			bool bIfFound = false;
			if (m_sourceTable[EST_HotSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
			{
				bIfFound = true;
			}
			else
			{
				if (m_sourceTable[EST_ColdSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
				{
					bIfFound = true;
				}
			}
			if (bIfFound)
			{
				m_sourceTable[EST_PaidSource]->AddSource(*itrVec);
			}
			itrVec++;
		}
		m_mtxSource.ReleaseLock();
	}
	return nRet;
}

int CSourceManager::CheckFileSource()
{
	int nRet = 0;
	vector<TSourceInfo> vec;
	nRet = LoadFileFromDb(vec);
	if (nRet == 0)
	{
		m_mtxSource.GetWriteLock();
		m_sourceTable[EST_FileSource]->Clear();
		vector<TSourceInfo>::iterator itrVec = vec.begin();
		while (itrVec != vec.end())
		{
			TSourceInfo tSourceInfo;
			tSourceInfo.m_strGcid = itrVec->m_strGcid;

			bool bIfFound = false;
			if (m_sourceTable[EST_HotSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
			{
				bIfFound = true;
			}
			else
			{
				if (m_sourceTable[EST_ColdSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
				{
					bIfFound = true;
				}
			}
			if (bIfFound)
			{
				m_sourceTable[EST_FileSource]->AddSource(*itrVec);
			}
			itrVec++;
		}
		m_mtxSource.ReleaseLock();
	}
	return nRet;
}

int CSourceManager::CheckShortSource()
{
	int nRet = 0;
	vector<TSourceInfo> vec;
	nRet = LoadShortFromDb(vec);
	if (nRet == 0)
	{
		m_mtxSource.GetWriteLock();
		m_sourceTable[EST_ShortSource]->Clear();
		vector<TSourceInfo>::iterator itrVec = vec.begin();
		while (itrVec != vec.end())
		{
			TSourceInfo tSourceInfo;
			tSourceInfo.m_strGcid = itrVec->m_strGcid;

			bool bIfFound = false;
			if (m_sourceTable[EST_HotSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
			{
				bIfFound = true;
			}
			else
			{
				if (m_sourceTable[EST_ColdSource]->FindSource(itrVec->m_strGcid, tSourceInfo) == 1)
				{
					bIfFound = true;
				}
			}
			if (bIfFound)
			{
				m_sourceTable[EST_ShortSource]->AddSource(*itrVec);
			}
			itrVec++;
		}
		m_mtxSource.ReleaseLock();
	}
	return nRet;
}

//depleted
int CSourceManager::GetSourceByType(int nType, vector<TSourceInfo> &vecInfo)
{
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::GetSourceByType : get source from %d hash table", nType);
	m_mtxSource.GetReadLock();
	if (nType == EST_HotSource)
	{
		m_sourceTable[EST_HotSource]->GetAllSource(vecInfo);
	}
	else if (nType == EST_ColdSource)
	{
		m_sourceTable[EST_ColdSource]->GetAllSource(vecInfo);
	}
	else if (nType == EST_AllSource)
	{
		m_sourceTable[EST_HotSource]->GetAllSource(vecInfo);
		m_sourceTable[EST_ColdSource]->GetAllSource(vecInfo);
	}
	else
	{
		m_mtxSource.ReleaseLock();
		WriteRunInfo::WriteError ("CSourceManager::GetSourceByType : illegal source type %d", nType);
		return INVALID_SOURCETYPE_ERROR;
	}
	m_mtxSource.ReleaseLock();
	return 0;
}

//queryvp only sync hot source
int CSourceManager::GetGCidByType(int nType, vector<string> &vecGCid)
{
	m_mtxSource.GetReadLock();
	if (nType == EST_HotSource)
	{
		m_sourceTable[EST_HotSource]->GetAllGCid(vecGCid);
	}
	else if (nType == EST_ColdSource)
	{
		m_sourceTable[EST_ColdSource]->GetAllGCid(vecGCid);
	}
	else if (nType == EST_AllSource)
	{
		m_sourceTable[EST_HotSource]->GetAllGCid(vecGCid);
		m_sourceTable[EST_ColdSource]->GetAllGCid(vecGCid);
	}
	else if (nType == EST_PaidSource)
    {
		m_sourceTable[EST_PaidSource]->GetAllGCid(vecGCid);
    }
	else
	{
		m_mtxSource.ReleaseLock();
		WriteRunInfo::WriteError ("CSourceManager::GetGCidByType : illegal source type %d", nType);
		return INVALID_SOURCETYPE_ERROR;
	}
	m_mtxSource.ReleaseLock();
	return 0;
}

int CSourceManager::GetGCidByTypeAndIndex(int nType, unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<string> &vecGCid)
{
	m_mtxSource.GetReadLock();
	if (nType == EST_PaidSource || nType == EST_FileSource || nType == EST_ShortSource)
	{
		m_sourceTable[nType]->GetGCidByIndex(nClusterId, nIndex, nNum, vecGCid);
	}
	m_mtxSource.ReleaseLock();
	return 0;
}

int CSourceManager::GetIncSource(vector<TSourceInfo> &vecInfo)
{
	m_mtxSource.GetReadLock();
	map<string, TSourceInfo>::iterator itrIncSource = m_mapIncSource.begin();
	while (itrIncSource != m_mapIncSource.end())
	{
		vecInfo.push_back(itrIncSource->second);
		itrIncSource ++;
	}
	m_mtxSource.ReleaseLock();
	return 0;
}

int CSourceManager::GetIncSourceByIndex(unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<TSourceInfo> &vecInfo)
{
	m_mtxSource.GetReadLock();
	map<string, TSourceInfo>::iterator itrIncSource = m_mapIncSource.begin();
	while (itrIncSource != m_mapIncSource.end())
	{
		unsigned int nRealIndex = 0;
		if (CTaskManager::GetInstance()->GetHostIndexByGCid(nClusterId, itrIncSource->second.m_strGcid, nRealIndex) == 0)
		{
			if (nIndex == nRealIndex)
			{
				vecInfo.push_back(itrIncSource->second);
			}
		}
		itrIncSource ++;
	}
	m_mtxSource.ReleaseLock();
	return 0;
}

void CSourceManager::ClearTimeoutIncSource()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	unsigned int nCurTime = pServer->GetTimeVal().tv_sec;

	m_mtxSource.GetWriteLock();
	map<string, TSourceInfo>::iterator itrIncSource = m_mapIncSource.begin();
	while (itrIncSource != m_mapIncSource.end())
	{
		if (nCurTime - itrIncSource->second.m_nStartTime >= (unsigned int)conf.nIncSourceOverTime)
		{
			WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::ClearTimeoutIncSource : inc source %s is over time",
						  CToolKit::BiToHex(itrIncSource->second.m_strGcid.c_str(),	itrIncSource->second.m_strGcid.size()).c_str());
			m_mapIncSource.erase(itrIncSource ++);
			continue;
		}
		itrIncSource ++;
	}
	m_mtxSource.ReleaseLock();
}

int CSourceManager::InitDataBase()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	int nSourceTableNum = conf.nSourceTableNum;
	CMysql mySql;
	try
	{
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("CSourceManager::InitDataBase: fail, error %s", e.GetErrMsg());
		return DB_ERROR;
	}

	char szTmp[1024];
	char strIndex[16] = {0};
	for (int i = 0; i < nSourceTableNum; i ++)
	{
		memset(szTmp, 0, sizeof(szTmp));
		sprintf(strIndex, "%d", i);
		string strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
		snprintf(szTmp, sizeof(szTmp), CREATE_RESOURCE_TABLE, conf.tMasterDb.strDbName.c_str(), strTableName.c_str());
		try
		{
			mySql.Query(szTmp);
		}
		catch (CMysqlException& e)
		{
			WriteRunInfo::WriteError("CSourceManager::InitDataBase: %s", e.GetErrMsg());
			return DB_ERROR;
		}
	}
	
	return 0;
}

int CSourceManager::InitDataBaseBak()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	int nSourceTableNum = conf.nSourceTableNum;
	CMysql mySql;
	try
	{
		mySql.Init();
		mySql.Connect(conf.tMasterDbBak.strIp, conf.tMasterDbBak.strUser, conf.tMasterDbBak.strPasswd);
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("CSourceManager::InitDataBaseBak: fail, error %s", e.GetErrMsg());
		return DB_ERROR;
	}

	char szTmp[1024];
	char strIndex[16] = {0};
	for (int i = 0; i < nSourceTableNum; i ++)
	{
		memset(szTmp, 0, sizeof(szTmp));
		sprintf(strIndex, "%d", i);
		string strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
		snprintf(szTmp, sizeof(szTmp), CREATE_RESOURCE_TABLE, conf.tMasterDbBak.strDbName.c_str(), strTableName.c_str());
		try
		{
			mySql.Query(szTmp);
		}
		catch (CMysqlException& e)
		{
			WriteRunInfo::WriteError("CSourceManager::InitDataBaseBak: %s", e.GetErrMsg());
			return DB_ERROR;
		}
	}
	
	return 0;
}

int CSourceManager::LoadFromDb()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	char szSqlBuf[1024] = {0};
	unsigned int nSourceNum = 0;

	try
	{
		string strTableName;
		char strIndex[16] = {0};
		TSourceInfo tSourceInfo;
		string strQuery = "select gcid,sourcetype,unix_timestamp(publishtime) as pt,unix_timestamp(lastreporttime) as rt,copynum from %s.%s;";
		int nSourceTableNum = conf.nSourceTableNum;

		CMysql mySql;
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);

		for (int i = 0; i < nSourceTableNum; i ++)
		{
			sprintf(strIndex, "%d", i);
			strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
			snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tMasterDb.strDbName.c_str(), strTableName.c_str());

			mySql.Query(szSqlBuf);
			while (mySql.FetchRow())
			{
				const char *pStrGcid = mySql.GetField("gcid");
				tSourceInfo.m_strGcid = CToolKit::HexToBi(pStrGcid, strlen(pStrGcid));
				tSourceInfo.m_nSourceType= strtoul(mySql.GetField("sourcetype"), 0, 10);
				tSourceInfo.m_nCopyNum = strtoul(mySql.GetField("copynum"), 0, 10);
				tSourceInfo.m_nPublishTime = strtoul(mySql.GetField("pt"), 0, 10);
				tSourceInfo.m_nLastReportTime = strtoul(mySql.GetField("rt"), 0, 10);
				tSourceInfo.m_nStartTime = pServer->GetTimeVal().tv_sec;

				if (tSourceInfo.m_nSourceType == EST_HotSource)
				{
					m_sourceTable[EST_HotSource]->AddSource(tSourceInfo);
				}
				else if (tSourceInfo.m_nSourceType == EST_ColdSource)
				{
					m_sourceTable[EST_ColdSource]->AddSource(tSourceInfo);
				}
				nSourceNum ++;
			}
			mySql.FreeResult();
		}
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("SourceManager::LoadFromDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "SourceManager::LoadFromDb: load %u sources", nSourceNum);
	return 0;
}

int CSourceManager::InsertToDb(TSourceInfo &tInfo)
{
	CMysql mySql;
	char szSqlBuf[1024] = {0};
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	try
	{
		int nTmpIndex = tInfo.GetIndex(); //hash
		int nRealIndex = nTmpIndex % conf.nSourceTableNum;
		string strTableName;
		char strIndex[16] = {0};
		sprintf(strIndex, "%d", nRealIndex);
		strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);
		snprintf(szSqlBuf, sizeof(szSqlBuf), "insert into %s.%s values('%s',%d, from_unixtime(%d), from_unixtime(%d),%u);", 
				 conf.tMasterDb.strDbName.c_str(),
				 strTableName.c_str(),
				 CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str(),
				 tInfo.m_nSourceType, tInfo.m_nPublishTime, tInfo.m_nLastReportTime,
				 tInfo.m_nCopyNum);
		mySql.Query(szSqlBuf);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::InsertToDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	CDbBackMgr::GetInstance()->Query(szSqlBuf);
	return 0;
}

int CSourceManager::UpdateToDb(TSourceInfo &tInfo)
{
	CMysql mySql;
	char szSqlBuf[1024] = {0};
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	try
	{
		int nTmpIndex = tInfo.GetIndex(); //hash
		int nRealIndex = nTmpIndex % conf.nSourceTableNum;
		string strTableName;
		char strIndex[16] = {0};
		sprintf(strIndex, "%d", nRealIndex);
		strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp.c_str(), conf.tMasterDb.strUser.c_str(),
					  conf.tMasterDb.strPasswd.c_str());
		snprintf(szSqlBuf, sizeof(szSqlBuf), 
				"replace into %s.%s(gcid,sourcetype,publishtime, lastreporttime,copynum) values('%s',%d,from_unixtime(%d), from_unixtime(%d),%u);", 
				 conf.tMasterDb.strDbName.c_str(), strTableName.c_str(),
				 CToolKit::BiToHex(tInfo.m_strGcid.c_str(), tInfo.m_strGcid.size()).c_str(),
				 tInfo.m_nSourceType, tInfo.m_nPublishTime, tInfo.m_nLastReportTime,
				 tInfo.m_nCopyNum);
		mySql.Query(szSqlBuf);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::UpdateToDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	CDbBackMgr::GetInstance()->Query(szSqlBuf);
	return 0;
}

int CSourceManager::DeleteFromDb(const std::string &strGcid)
{
	CMysql mySql;
	char szSqlBuf[1024] = {0};
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	try
	{
		TSourceInfo tInfo;
		tInfo.m_strGcid = strGcid;
		int nTmpIndex = tInfo.GetIndex(); //hash
		int nRealIndex = nTmpIndex % conf.nSourceTableNum;
		string strTableName;
		char strIndex[16] = {0};
		sprintf(strIndex, "%d", nRealIndex);
		strTableName = SOURCE_INFO_TABLE + "_" + strIndex;
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser, conf.tMasterDb.strPasswd);
		std::string strTaskId = CToolKit::BiToHex(strGcid.c_str(), strGcid.size());
		snprintf(szSqlBuf, sizeof(szSqlBuf), "delete from %s.%s where gcid = '%s';",
				 conf.tMasterDb.strDbName.c_str(), strTableName.c_str(), strTaskId.c_str());
		mySql.Query(szSqlBuf);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CSourceManager::DeleteFromDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	CDbBackMgr::GetInstance()->Query(szSqlBuf);
	return 0;
}

int CSourceManager::LoadPaidFromDb(vector<TSourceInfo>& vec)
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	char szSqlBuf[1024] = {0};
	unsigned int nSourceNum = 0;

	try
	{
		string strTableName;
		TSourceInfo tSourceInfo;
		string strQuery = "select iid from %s.%s where status<>2 and ispay=1;";

		CMysql mySql;
		mySql.Init();
		mySql.Connect(conf.tPaidInfoDb.strIp, conf.tPaidInfoDb.strUser, conf.tPaidInfoDb.strPasswd);

		strTableName = PAID_MOVIE_INFO_TABLE;
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tPaidInfoDb.strDbName.c_str(), strTableName.c_str());

		mySql.Query(szSqlBuf);
		while (mySql.FetchRow())
		{
			const char *pStrGcid = mySql.GetField("iid");
			tSourceInfo.m_strGcid = CToolKit::HexToBi(pStrGcid, strlen(pStrGcid));
			//tSourceInfo.m_nSourceType= strtoul(mySql.GetField("sourcetype"), 0, 10);
			//tSourceInfo.m_nCopyNum = strtoul(mySql.GetField("copynum"), 0, 10);
			//tSourceInfo.m_nPublishTime = strtoul(mySql.GetField("pt"), 0, 10);
			//tSourceInfo.m_nLastReportTime = strtoul(mySql.GetField("rt"), 0, 10);
			//tSourceInfo.m_nStartTime = pServer->GetTimeVal().tv_sec;

			vec.push_back(tSourceInfo);
			nSourceNum ++;
		}
		mySql.FreeResult();
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("SourceManager::LoadPaidFromDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "SourceManager::LoadPaidFromDb: load %u sources", nSourceNum);
	return 0;
}

int CSourceManager::LoadFileFromDb(vector<TSourceInfo>& vec)
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();

	TSourceInfo tSourceInfo;
	unsigned int nSourceNum = 0;

	FILE *fp = NULL;
	const int MAX_LINE = 1024;
	char strLine[MAX_LINE];
	if ((fp = fopen(conf.strFileDir.c_str(), "r")) != NULL)
	{
		while (!feof(fp))
		{
			if (!fgets(strLine, MAX_LINE, fp))
			{
				break;
			}
			tSourceInfo.m_strGcid = CToolKit::HexToBi(strLine, 40);
			vec.push_back(tSourceInfo);
			nSourceNum ++;
		}
	}
	fclose(fp);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "SourceManager::LoadFileFromDb: load %u sources", nSourceNum);
	return 0;
}

int CSourceManager::LoadShortFromDb(vector<TSourceInfo>& vec)
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	char szSqlBuf[1024] = {0};
	unsigned int nSourceNum = 0;

	try
	{
		string strTableName;
		TSourceInfo tSourceInfo;
		string strQuery = "select iid from %s.%s where movieid > 0;";

		CMysql mySql;
		mySql.Init();
		mySql.Connect(conf.tShortInfoDb.strIp, conf.tShortInfoDb.strUser, conf.tShortInfoDb.strPasswd);

		strTableName = SHORT_MOVIE_INFO_TABLE;
		snprintf(szSqlBuf, sizeof(szSqlBuf), strQuery.c_str(), conf.tShortInfoDb.strDbName.c_str(), strTableName.c_str());

		mySql.Query(szSqlBuf);
		while (mySql.FetchRow())
		{
			const char *pStrGcid = mySql.GetField("iid");
			tSourceInfo.m_strGcid = CToolKit::HexToBi(pStrGcid, strlen(pStrGcid));
			//tSourceInfo.m_nSourceType= strtoul(mySql.GetField("sourcetype"), 0, 10);
			//tSourceInfo.m_nCopyNum = strtoul(mySql.GetField("copynum"), 0, 10);
			//tSourceInfo.m_nPublishTime = strtoul(mySql.GetField("pt"), 0, 10);
			//tSourceInfo.m_nLastReportTime = strtoul(mySql.GetField("rt"), 0, 10);
			//tSourceInfo.m_nStartTime = pServer->GetTimeVal().tv_sec;

			vec.push_back(tSourceInfo);
			nSourceNum ++;
		}
		mySql.FreeResult();
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("SourceManager::LoadShortFromDb: fail, sql %s, error %s", szSqlBuf, e.GetErrMsg());
		return DB_ERROR;
	}

	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "SourceManager::LoadShortFromDb: load %u sources", nSourceNum);
	return 0;
}

