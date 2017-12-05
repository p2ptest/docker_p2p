/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: mastersrvdef.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2011-11-24
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-04-01			1.0			
* =======================================================================
*/
#ifndef __MASTERSRVDEF_H__
#define __MASTERSRVDEF_H__

#include "framecommon/framecommon.h"
using namespace std;
using namespace MYFRAMECOMMON;

//thread type
const int ONTIMEENTITY = 111;
const int DBBACKUPENTITY = 112;
const int ENTITYTYPE_CONNECT_LISTEN = LISTENENTITY; //100
const int ENTITYTYPE_RECV_AND_SEND = RECVANDSENDMSGENTITY;	//103

//msg type
const int MSGTYPE_RECV_CONNECT = 2001; //RECVMSGTYPE; //2001
const int MSGTYPE_RECV_HTTP_CONNECT = 1023;

//cmd type
const int PUBLISHCMD = 40;
const int PUBLISHRESCMD = 41;
const int SOURCESYNCMD = 42;
const int SOURCESYNRESCMD = 43;
const int SOURCEINCSYNCMD = 44;
const int SOURCEINCSYNRESCMD = 45;
const int SOURCEREPORTCMD = 46;
const int SOURCEREPORTRESCMD = 47;
const int GETTASKCMD = 48;
const int GETTASKRESCMD = 49;
const int TASKREPORTOLDCMD = 50;
const int TASKREPORTOLDRESCMD = 51;
const int DELETECMD = 52;
const int DELETERESCMD = 53;
const int STATSRV_ENTIRESOURCE_SYNC_CMD = 54;
const int STATSRV_ENTIRESOURCE_SYNC_RESCMD = 55;
const int PERSISTSOURCESYNCMD = 56;
const int PERSISTSOURCESYNRESCMD = 57;
const int TASKREPORTCMD = 60;
const int TASKREPORTRESCMD = 61;
const int SOURCESYN4CACHECMD = 66;
const int SOURCESYN4CACHERESCMD = 67;
const int SOURCEINCSYN4CACHECMD = 68;
const int SOURCEINCSYN4CACHERESCMD = 69;

//task type
const int MSGPROCESSTASK = 1101;
const int TASKTYPE_CONNECT_LISTEN = 0; //LISTENTASK; //0
const int TASKTYPE_HTTP_RECV_AND_SEND = 33;

//log type
const int MASTERSRV_DEBUG_TYPE = 80;
const int MASTERSRV_TASKDEBUG_TYPE = 81;
const int MASTERSRV_REGULAR_TYPE = 82;
const int MASTERSRV_DB_TYPE = 83;

//other
const int MAX_RECV_BUFF_LEN = 16 * 1024 * 1024;
const int UPDATE_INTERVAL = 10;
const unsigned int CID20_LEN = 20;
const unsigned int CID40_LEN = 40;

//error
const int PUBLISH_EXIST = 1;
const int INVALID_SOURCETYPE_ERROR = 2;
const int INVALID_TASKTYPE_ERROR = 3;
const int DB_ERROR = 4;
const int INVALID_USER = 5;
const int GCID_ALREADY_FIND = 6;
const int TASK_NOT_FIND = 7;
const int TASK_SUC = 8;
const int NOT_DOWNLOAD_TASK = 9;

//dbtable
const string TASK_INFO_TABLE = "task_info";
const string CLUSTER_CONF = "t_cluster_conf";
const string CLUSTER_INFO = "t_cluster_info";

//for HttpRecvAndSendTask
const unsigned int MIN_HTTP_HEAD_LEN = 512;
const unsigned int MIN_HTTP_BODY_LEN = 512;
const unsigned int MAX_RECV_LEN = 10240;
const unsigned int MAX_RECV_BODY_LEN = 1048576;

//用于构造常驻内存缓存
const int SOURCETYPENUM = 7;
enum ESourceType
{
	EST_ColdSource = 0, //只放冷资源
	EST_HotSource = 1, //只放热资源
	EST_AllSource =2, //不用,没有对应的HashTable;
	EST_DelSource = 3, //不用,没有对应的HashTable;
	EST_PaidSource = 4, //只放VIP资源
	EST_ShortSource = 5, //只放额外数据库
	EST_FileSource = 6, //只放文件列表资源
};

//task:cdnmaster会接受的其它服务器的指令
enum EOpsType
{
	EOT_Pub = 0,
	EOT_Del = 1,
	EOT_H2C = 2,
	EOT_C2H = 3,
	EOT_ReOnline = 4,
};

enum ETaskType
{
	ETT_DownLoadTask = 0,
	ETT_DeleteTask = 1,         //假删除：由Cdn内部决策出来的删除
	ETT_DeleteForeverTask = 2	//真删除：用于上层用户触发的删除
};

//t_cluster_conf中ishot的值
enum EClusterType
{
	ECT_Full = 0, //冷集群
	ECT_Hot = 1,  //热集群
	ECT_Cold = 2, //冷片集群:废弃
};

//已经不用了
//enum ETaskTargetType
//{
//	E3T_ToAllCluster = 7, //add: publish hot;	delete: delete source
//
//	E3T_ToHotCluster = 1, //add: stat cold to hot;	delete: stat hot to cold
//	E3T_ToFullCluster = 2, //add: publish cold;
//	E3T_ToColdCluster = 4, //add: publish cold; delete: delete source
//};

struct TDbInfo
{
	string strIp;
	string strUser;
	string strPasswd;
	string strDbName;
};

struct TSourceInfo 
{
	string m_strGcid;
	int m_nSourceType;
	unsigned int m_nCopyNum; //db中存放的CopyNum没有什么参考意义
	unsigned int m_nPublishTime;
	unsigned int m_nStartTime;
	unsigned int m_nLastReportTime;

	TSourceInfo()
	{
		m_nSourceType = 0;
		m_nCopyNum = 0;
		m_nPublishTime = 0;
		m_nStartTime = 0;
		m_nLastReportTime = 0;
	}
	
	unsigned int GetIndex() const
	{
		return CToolKit::shortELFhash(m_strGcid.c_str(), (unsigned)m_strGcid.length());
	}

	bool operator==(const TSourceInfo& tInfo) const
	{
		return (this->m_strGcid == tInfo.m_strGcid);
	}
};

struct TTaskInfo : public TSourceInfo
{
	unsigned int m_nTaskType;
	unsigned int m_nTarget; //在老版用到，在新版恒为0

	TTaskInfo()
	{
		m_nTaskType = 0;
		m_nTarget = 0;
	}

	TTaskInfo(const TSourceInfo &SourceInfo)
	{
		m_strGcid = SourceInfo.m_strGcid;
		m_nSourceType = SourceInfo.m_nSourceType;
		m_nCopyNum = SourceInfo.m_nCopyNum;
		m_nPublishTime = SourceInfo.m_nPublishTime;
		m_nStartTime = SourceInfo.m_nStartTime;
		m_nLastReportTime = SourceInfo.m_nLastReportTime;

		m_nTaskType = 0;
		m_nTarget = 0;
	}
};

struct TStateInfo
{
	TStateInfo()
	{
		m_cState = 0;
	}
	
	string m_strGcid;
	char m_cState;
};

struct TServerTask
{
	TServerTask()
	{
		m_nHostId = 0;
		m_nIp = 0;
		m_nState = 0;
	}

	bool operator==(const TServerTask& tTask) const
	{
		return (this->m_nHostId == tTask.m_nHostId);
	}
	
	unsigned int m_nHostId;
	unsigned int m_nIp;
	unsigned int m_nState;
	vector<TTaskInfo> vecTasks;
	CThreadRwLock m_mtxTaskInfo;
};

struct TClusterTask
{
	TClusterTask()
	{
		nClusterType = ECT_Full;
	}
	unsigned int nClusterType;
	unsigned int nHashType;
	vector<TServerTask> vecServers;
};

struct TIpInfo
{
	int nBegIp;
	int nEndIp;
};

enum EHashType
{
	EHT_Normal = 0,
	EHT_Consistent = 1,
};

//temp code
//const int VNODE_REPLICA_NUM = 128;
//class CConsistentHash
//{
//public:
//	CConsistentHash() {};
//	~CConsistentHash() {};
//	int UpdateNode(const list<int>& lstNode) { return 0; }
//	int GetNode(const string& strGcid) { return 0; }
//
//private:
//	int AddNode(int nHostId, int nVirtualNodeNum = VNODE_REPLICA_NUM) { return 0; }
//	int DelNode(int nHostId) { return 0; }
//
//private:
//	list<int> m_lstNode;
//	map<unsigned int, int> m_mapVirtualNode;
//	CThreadRwLock m_rwLockNode;
//};

extern int GetRemoteIp(int nSocket);
#endif
