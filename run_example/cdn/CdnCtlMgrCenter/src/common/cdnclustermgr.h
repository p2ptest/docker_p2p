/*
 * =====================================================================================
 *
 *       Filename:  cdnclustermgr.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef __CDNCLUSTERMGR_H__
#define __CDNCLUSTERMGR_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <map>
#include <vector>
#include <string>
#include <set>

#include "framecommon/framecommon.h"
#include "cdnctlmgrdef.h"
#include "cdnctlmgrserver.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TCdnSrvInfo
{
	unsigned int m_nClusterId;
	unsigned int m_nHostId;
	int m_nState;
	unsigned int m_nPublicIp;
	unsigned int m_nPrivateIp;

	enum ECdnState
	{
		ECS_AVAILABLE=0,
		ECS_NOTAVAILABLE=1
	};


	TCdnSrvInfo() : m_nClusterId(0), m_nHostId(0), m_nState(0), m_nPublicIp(0), m_nPrivateIp(0)
	{

	}

	TCdnSrvInfo(unsigned nClusterId, unsigned int nHostId, int nState, unsigned int nPublic, unsigned int nPrivate)
		: m_nClusterId(nClusterId), m_nHostId(nHostId), m_nState(nState), m_nPublicIp(nPublic), m_nPrivateIp(nPrivate)
	{

	}

	void WriteLog(const string& strLogHead = "TCdnSrvInfo")
	{
		in_addr iaAddrPub;
		iaAddrPub.s_addr = htonl(m_nPublicIp);
		in_addr iaAddrPri;
		iaAddrPri.s_addr = htonl(m_nPrivateIp);
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: ClusterId=%u,HostId=%u,State=%u,PublicIp=%s,PrivateIp=%s", 
			strLogHead.c_str(), m_nClusterId, m_nHostId, m_nState, inet_ntoa(iaAddrPub), inet_ntoa(iaAddrPri));
	}
};

struct TClusterConf
{
	unsigned int m_nClusterId;
	char m_nState;
	char m_nIsHot;
	char m_nHash;
	char m_nIsp;
	char m_nLimitMode;
	unsigned int m_nSpeedLimit;
	unsigned int m_nMaxLimit;
	unsigned int m_nMultipleLimit;
	string m_sComment;
	void WriteLog(const string& strLogHead = "TClusterConf")
	{
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: ClusterId=%u,State=%d,LimitMode=%d,SpeedLimit=%u,MaxLimit=%u,MultipleLimit=%u,IsHot=%d,Hash=%d", 
			strLogHead.c_str(), m_nClusterId, m_nState, m_nLimitMode, m_nSpeedLimit, m_nMaxLimit, m_nMultipleLimit, m_nIsHot,m_nHash);
	}
};

struct TCdnParent
{
	char m_nState;
	unsigned int m_nIp;

	TCdnParent() : m_nState(0), m_nIp(0)
	{
		
	}

	TCdnParent(char nState, unsigned int nIp)
		: m_nState(nState), m_nIp(nIp)
	{

	}

	void WriteLog(const string& strLogHead = "TCdnParent")
	{
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: Ip=%s,State=%u",strLogHead.c_str(), inet_ntoa(iaAddr), m_nState);
	}
};

struct TCdnHostInfo
{
	int m_nHostId;
	char m_nState;
	unsigned int m_nIp;

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutInt(m_nHostId);
		nRet += bufMgr.PutChar(m_nState);
		nRet += bufMgr.PutInt(m_nIp);
		return nRet;
	}
	
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_nHostId);
		nLen += sizeof(m_nState);
		nLen += sizeof(m_nIp);
		return nLen;
	}
	
	void WriteLog(const string& strLogHead = "TCdnHostInfo")
	{
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: m_nHostId=%d,m_nState=%d,m_nIp=%s",strLogHead.c_str(), m_nHostId, (int)m_nState, inet_ntoa(iaAddr));
	}
};

struct TRelationCluster
{
	int m_nClusterId;
	char m_nIsHot;
	char m_nHash;
	char m_nIsp;
	char m_nParentAttribute;
	vector<TCdnHostInfo> m_vecHostInCLuster;

	int ClearSelf()
	{
		m_nClusterId = 0xffffffff;
		m_nIsHot = -1;
		m_nHash = -1;
		m_nIsp = -1;
		m_nParentAttribute = -1;
		m_vecHostInCLuster.clear();
		return 0;
	}
	
	int Encode(CBufMgr &bufMgr, bool bHash = false)
	{
		int nRet = 0;
		nRet += bufMgr.PutInt(m_nClusterId);
		nRet += bufMgr.PutChar(m_nIsHot);
		if (bHash)
		{
			nRet += bufMgr.PutChar(m_nHash);
		}
		nRet += bufMgr.PutChar(m_nIsp);
		nRet += bufMgr.PutChar(m_nParentAttribute);
		int nHostNum = m_vecHostInCLuster.size();
		nRet += bufMgr.PutInt(nHostNum);
		for(int nA = 0; nA < nHostNum; nA++)
		{
			nRet += m_vecHostInCLuster[nA].Encode(bufMgr);
		}
		return nRet;
	}

	int GetSize(bool bHash = false)
	{
		int nLen = 0;
		nLen += sizeof(m_nClusterId);
		nLen += sizeof(m_nIsHot);
		if (bHash)
		{
			nLen += sizeof(m_nHash);
		}
		nLen += sizeof(m_nIsp);
		nLen += sizeof(m_nParentAttribute);
		nLen += sizeof(int);
		int nHostNum = m_vecHostInCLuster.size();
		for(int nA = 0; nA < nHostNum; nA++)
		{
			nLen += m_vecHostInCLuster[nA].GetSize();
		}
		return nLen;
	}
	
	void WriteLog(const string& strLogHead = "TRelationCluster")
	{
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: m_nClusterId=%d,m_nIsHot=%d,m_nIsp=%d,m_nParentAttribute=%d",strLogHead.c_str(), m_nClusterId, (int)m_nIsHot, (int)m_nIsp, (int)m_nParentAttribute);
		int nHostNum = m_vecHostInCLuster.size();
		for(int nA = 0; nA < nHostNum; nA++)
		{
			m_vecHostInCLuster[nA].WriteLog("\t");
		}
	}
};


struct TCdnClusterInfo
{
	int m_nClusterId;
	vector<TCdnSrvInfo> m_vecCdnSrvInfo;
	void WriteLog(const string& strLogHead = "TCdnClusterInfo")
	{
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: ClusterId=%u", strLogHead.c_str(), m_nClusterId);
		int nCdnSrvNum = m_vecCdnSrvInfo.size();
		for (int nA = 0; nA < nCdnSrvNum; ++ nA)
		{
			m_vecCdnSrvInfo[nA].WriteLog("\t");
		}
	}
};

struct TCdnLevelInfo
{
	unsigned int m_nClusterId;
	unsigned int m_nLevel;
};

struct TCdnRelationInfo
{
	unsigned int m_nClusterId;
	unsigned int m_nParentClusterId;
	bool m_bIfUsePublicIp;
	bool m_bParentAttribute;
	vector<TCdnHostInfo> m_vecParentCdn;
	vector<TCdnHostInfo> m_vecSonCdn;

	TCdnRelationInfo() : m_nClusterId(0), m_nParentClusterId(0), m_bIfUsePublicIp(false)
	{

	}

	TCdnRelationInfo(unsigned int nClusterId, unsigned int nLevel, unsigned int nParentClusterId, bool bIfUsePublicIp)
		: m_nClusterId(nClusterId), m_nParentClusterId(nParentClusterId), m_bIfUsePublicIp(bIfUsePublicIp)
	{

	}

	void WriteLog(const string& strLogHead = "TRelationCluster")
	{
		WRITE_ALL_LOG(CDNCTLMGRSRV_LOG, "\t%s: m_nClusterId=%d,m_nParentClusterId=%d,m_bIfUsePublicIp=%d,m_bParentAttribute=%d",strLogHead.c_str(), m_nClusterId, m_nParentClusterId, (int)m_bIfUsePublicIp, (int)m_bParentAttribute);
		int nParentHostNum = m_vecParentCdn.size();
		for(int nA = 0; nA < nParentHostNum; nA++)
		{
			m_vecParentCdn[nA].WriteLog("\t");
		}
		
		int nSonHostNum = m_vecSonCdn.size();
		for(int nA = 0; nA < nSonHostNum; nA++)
		{
			m_vecSonCdn[nA].WriteLog("\t");
		}
	}
};

class CCdnClusterMgr
{
private:
	CCdnClusterMgr();
	~CCdnClusterMgr();

public:
	static CCdnClusterMgr* GetInstance();
	static void CreateInstance();
	static void DeleteInstance();
public:
	int InitClusterMgr();
	int GetParentCdn(unsigned int nIp, vector<TCdnParent>& vecCdnParent);
	int GetParentsCdn(unsigned int nIp, vector<vector<TCdnParent> >& vecCdnParent);					//add by yinhaichen 2011-03-29
	int GetNetRelation(unsigned int nIp, vector<TRelationCluster> &vecParentCluster, vector<TRelationCluster> &vecSonCluster, unsigned int& nLay);
	int GetCdnSrvInfo(unsigned int nIp, vector<TCdnSrvInfo>& vecCdnInfo);							//add by yinhaichen 2011-03-18
	int GetLimitSpeed(unsigned int nIp, char& nLimitMode, unsigned int& nLimitSpeed, unsigned int& nMaxSpeed, unsigned int& nMultipleSpeed);				//add by yinhaichen 2011-04-12
	int GetClusterId(unsigned int nIp, unsigned int& nClusterId);
	int GetHostId(unsigned int nClusterId, unsigned int& nHostId);
	int GetIsp(unsigned int nClusterId, char& nIsp);
	int CheckIfHotCluster(unsigned int nClusterId, char& nIsHot, char& nHash);
	void WriteLog();
	int LoadFromDb();
private:
	static void* ReloadThread(void* arg);
	int CheckIfHaveCircleRelation(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnRelation);
	bool CheckCircle(map<unsigned int, vector<TCdnRelationInfo> >& inMap, map<unsigned int, vector<TCdnRelationInfo> >::iterator& inIterNode, set<unsigned int>& inSet);
private:
	int LoadClusterConf(map<unsigned int, TClusterConf>& mapClusterConf);
	//mapClusterConf用于过滤
	int LoadClusterInfo(map<unsigned int, TCdnClusterInfo>& mapCdnCluster, map<unsigned int, unsigned int>&	mapIp2ClusterId, 
		map<unsigned int, TClusterConf>& mapClusterConf, map<unsigned int, unsigned int>& mapIp2HostId);
	//mapCdnCluster用于校对
	int LoadClusterRelation(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnParentRelation, 
										map<unsigned int, vector<TCdnRelationInfo> >& mapCdnSonRelation, 
										map<unsigned int, TCdnClusterInfo>& mapCdnCluster, 
										map<unsigned int, TClusterConf>& mapClusterConf);

	//根据集群父子关系，算出每个节点的Lay
	int GenAllClusterLay(map<unsigned int, vector<TCdnRelationInfo> >& mapCdnRelation, map<unsigned int, unsigned int>& mapClusterLay, map<unsigned int, TClusterConf>& mapClusterConf); //added by wangfangzhu 2011-10-17
private:
	static CCdnClusterMgr* m_hInstance;
private:
	CThreadRwLock m_rwlkCdnClusterMgr;
	int m_nReloadInterval;
	map<unsigned int, TCdnClusterInfo> m_mapCdnCluster;
	map<unsigned int, unsigned int> m_mapIp2ClusterId;
	map<unsigned int, unsigned int> m_mapIp2HostId;
	map<unsigned int, vector<TCdnRelationInfo> > m_mapCdnParentRelation;
	map<unsigned int, vector<TCdnRelationInfo> > m_mapCdnSonRelation;
	map<unsigned int, TClusterConf> m_mapClusterConf;				//集群配置
	map<unsigned int, unsigned int> m_mapClusterLay;				//集群Lay， <clusterid, lay>		//added by wangfangzhu 2011-10-17	
};


#endif

