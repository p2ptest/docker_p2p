/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: GetNetRelation.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : hezhipeng(hzp), hezhipeng@baofeng.com
* Company		: 
* Date			: 2012-04-06
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		hezhipeng		2012-04-06			1.0			
* =======================================================================
*/
#include <sys/time.h>
#include <fcntl.h>

#include "CdnCltMgrCenterTestClient.h"


const int PROTOCAL_VER = 100;
const int GETNETRELATION_CMD = 112;
const int GETNETRELATION_CMD_RES = 113;

struct TGetNetReLationCmd
{
	int m_nRealIp;
	char m_nIsNat;

	TGetNetReLationCmd()
	{
		m_nRealIp = 0;
		m_nIsNat = 0;
	}

	int Encode(CTmpBufMgr& bufMgr)
	{		
		if (bufMgr.PutChar(m_nIsNat) != 0) {return -1;}
		
		if (bufMgr.PutInt(m_nRealIp) != 0) {return -1;}
		return 0;
	}
	
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_nIsNat);
		nLen += sizeof(m_nRealIp);
		return nLen;
	}
	
	void Print()
	{
		cout << "m_nIsNat = " << (int)m_nIsNat << endl ;
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nRealIp);
		cout << "m_nRealIp = " << inet_ntoa(iaAddr) << endl ;
	}
};

struct TCdnHostInfo
{
	int m_nHostId;
	char m_nState;
	int m_nIp;

	int Decode(CTmpBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.GetInt(m_nHostId);
		nRet += bufMgr.GetChar(m_nState);
		nRet += bufMgr.GetInt(m_nIp);
		return nRet;
	}
	
	void Print()
	{
		cout << "m_nHostId = " << m_nHostId << endl ;
		cout << "m_nState = " << (int)m_nState << endl ;
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		cout << "m_nIp = " << inet_ntoa(iaAddr) << endl ;
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

	int Decode(CTmpBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.GetInt(m_nClusterId);
		nRet += bufMgr.GetChar(m_nIsHot);
		nRet += bufMgr.GetChar(m_nHash);
		nRet += bufMgr.GetChar(m_nIsp);
		nRet += bufMgr.GetChar(m_nParentAttribute);
		int nHostNum = 0;
		nRet += bufMgr.GetInt(nHostNum);
		for(int nA = 0; nA < nHostNum; nA++)
		{
			TCdnHostInfo tmpHostInfo;
			nRet += tmpHostInfo.Decode(bufMgr);
			m_vecHostInCLuster.push_back(tmpHostInfo);
		}
		return nRet;
	}
	
	void Print()
	{
		cout << "m_nClusterId = " << m_nClusterId << endl;
		cout << "m_nIsHot = " << (int)m_nIsHot << endl;
		cout << "m_nHash = " << (int)m_nHash << endl;
		cout << "m_nIsp = " << (int)m_nIsp << endl;
		cout << "m_nParentAttribute = " << (int)m_nParentAttribute << endl;
		cout << "cluster host num = " << m_vecHostInCLuster.size() << endl;
		for(unsigned int nA = 0; nA < m_vecHostInCLuster.size(); nA++)
		{
			m_vecHostInCLuster[nA].Print();
		}
	}
};

struct TGetNetReLationCmdRes
{
	char m_nResult;
	int m_nClusterId;
	int m_nHostId;
	TRelationCluster m_tSelfCluster;
	vector<TRelationCluster> m_vecParentCluster;
	vector<TRelationCluster> m_vecSonCluster;
	int m_nLay;

    int Decode(CTmpBufMgr& bufMgr)
    {
			int nRet = 0;
			nRet += bufMgr.GetChar(m_nResult);
			nRet += bufMgr.GetInt(m_nClusterId);
			nRet += bufMgr.GetInt(m_nHostId);
			nRet += m_tSelfCluster.Decode(bufMgr);
			int nCdnParentClusterNum = 0;
			nRet += bufMgr.GetInt(nCdnParentClusterNum);
			for(int nA = 0; nA < nCdnParentClusterNum; ++ nA)
			{
				TRelationCluster tmpRelationCluster;
				nRet += tmpRelationCluster.Decode(bufMgr);
				m_vecParentCluster.push_back(tmpRelationCluster);
			}
			int nCdnSonClusterNum = 0;
			nRet += bufMgr.GetInt(nCdnSonClusterNum);
			for(int nA = 0; nA < nCdnSonClusterNum; ++ nA)
			{
				TRelationCluster tmpRelationCluster;
				nRet += tmpRelationCluster.Decode(bufMgr);
				m_vecSonCluster.push_back(tmpRelationCluster);
			}
			nRet += bufMgr.GetInt(m_nLay);
			return nRet;
    }
	
	void Print()
	{
		cout << "m_nResult = " << (int)m_nResult << endl;
		cout << "m_nClusterId = " << m_nClusterId << endl;
		cout << "m_nHostId = " << m_nHostId << endl;
		cout << endl;
		cout << "Self ClusterInfo:" <<endl;
		m_tSelfCluster.Print();
		cout << endl;
		cout << " Parent Cluster Num = " << m_vecParentCluster.size() << endl;
		for(unsigned int nA = 0; nA < m_vecParentCluster.size(); ++ nA)
		{
			m_vecParentCluster[nA].Print();
		}
		cout << endl;
		cout << " Son Cluster Num = " << m_vecSonCluster.size() << endl;
		for(unsigned int nA = 0; nA < m_vecSonCluster.size(); ++ nA)
		{
			m_vecSonCluster[nA].Print();
		}
	}
	
};


int main(int argc, char *argv[])
{
	
	if (argc != 4 && argc != 3)
	{
		printf("Usage:%s host port [realip]\n", argv[0]);
		return -1;
	}

	TGetNetReLationCmd getNetReLationCmd;
	
	if( argv[3] != NULL )
	{
		getNetReLationCmd.m_nIsNat = 1;
		struct in_addr tmpIn_addr;
		inet_aton(argv[3], &tmpIn_addr);
		getNetReLationCmd.m_nRealIp  = ntohl(tmpIn_addr.s_addr);
		
	}
	else
	{
		getNetReLationCmd.m_nIsNat = 0;
		getNetReLationCmd.m_nRealIp = 0;
	}

	TMyCmdHead BaseCmd;
	BaseCmd.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmd.m_chCmd = GETNETRELATION_CMD;
	BaseCmd.m_nBodylen = getNetReLationCmd.GetSize() + sizeof(BaseCmd.m_chCmd);
	CTcpCommonTestClient TcpCommonTestClient(argv[1], atoi(argv[2]));
	
	int nRet = TcpCommonTestClient.Send<TMyCmdHead, TGetNetReLationCmd>(BaseCmd, getNetReLationCmd);
	if (nRet != 0)
	{
		cout << "Send Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetNetReLationCmd=========================================" << endl;
		BaseCmd.Print();
		getNetReLationCmd.Print();
	}
	
	TGetNetReLationCmdRes getNetReLationCmdRes;
	TMyCmdHead BaseCmdRes;
	BaseCmdRes.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmdRes.m_chCmd = GETNETRELATION_CMD_RES;
	nRet = TcpCommonTestClient.Recv<TMyCmdHead, TGetNetReLationCmdRes>(BaseCmdRes, getNetReLationCmdRes);
	if (nRet != 0)
	{
		cout << "Recv Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetNetReLationCmdRes=========================================" << endl;
		BaseCmdRes.Print();
		getNetReLationCmdRes.Print();
	}
	
	sleep(2);
	return 0;
}

