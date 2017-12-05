/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcemanager.h
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
#ifndef __SOURCEMANAGER_H__
#define __SOURCEMANAGER_H__


#include "framecommon/framecommon.h"
#include "mastersrvdef.h"
#include "sourcehashtable.h"

using namespace std;
using namespace MYFRAMECOMMON;

class CSourceManager
{
public:
	static int Create();
	static int Destroy();
	static CSourceManager *GetInstance()
	{
		return m_pInstance;
	}

private:
	CSourceManager();
	~CSourceManager();

public:
	int Init();
	int ReOnlineSource(const TSourceInfo &tInfo);
	int PublishSource(const TSourceInfo &tInfo);
	int DeleteSource(const std::string &strGcid);
	int SourceHotToCold(const string &strGcid);
	int SourceColdToHot(const string &strGcid);

	int GetSourceByType(int nType, vector<TSourceInfo> &vecInfo);
	int GetGCidByType(int nType, vector<string> &vecGCid);
	int GetIncSource(vector<TSourceInfo> &vecInfo);
	int GetIncSourceByIndex(unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<TSourceInfo> &vecInfo);
	int GetGCidByTypeAndIndex(int nType, unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<string> &vecGCid);

	void ChecktimeOutTaskMgr();
	void ClearTimeoutIncSource();
	int CheckPaidSource();
	int CheckFileSource();
	int CheckShortSource();

	int GetCacheTask(unsigned int nIp, vector<TTaskInfo> &vecTasks);
	int ReportCacheTask(unsigned int nIp, TStateInfo &TaskState);

protected:
	int InitDataBase();
	int InitDataBaseBak();
	int LoadFromDb();
	int LoadPaidFromDb(vector<TSourceInfo>& vec);
	int LoadFileFromDb(vector<TSourceInfo>& vec);
	int LoadShortFromDb(vector<TSourceInfo>& vec);
	int InsertToDb(TSourceInfo &tInfo);
	int UpdateToDb(TSourceInfo &tInfo);
	int DeleteFromDb(const std::string &strGcid);

protected:
	int AddCopyNum(const string &strGcid);

private:
	CSourceHashTable *m_sourceTable[SOURCETYPENUM];
	CThreadRwLock m_mtxSource;
	map<string, TSourceInfo> m_mapIncSource;

private:
	static CSourceManager *m_pInstance;
};

#endif
