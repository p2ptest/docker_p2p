/*
 * =====================================================================================
 *
 *       Filename:  getparentscdncmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-10-17
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  wangfangzhu, wangfangzhu@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef __GETNETRELATIONCMD_H__
#define __GETNETRELATIONCMD_H__
#include <vector>
#include "cdnctlmgrbasecmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct TGetNetReLationCmd
{
	int m_nIp;
	char m_nIsNatIP;

	TGetNetReLationCmd():m_nIp(0), m_nIsNatIP(0)
	{
	}

	int Decode(CBufMgr& bufMgr)
	{
		if (bufMgr.GetChar(m_nIsNatIP) != 0) {return -1;}
		
		if (bufMgr.GetInt(m_nIp) != 0) {return -1;}
		if(m_nIsNatIP)
		{
			in_addr iaAddr;
			iaAddr.s_addr = htonl(m_nIp);
		}
		return 0;
	}
};

struct TGetNetReLationCmdRes
{
	char m_nResult;
	unsigned int m_nClusterId;
	unsigned int m_nHostId;
	TRelationCluster m_tSelfCluster;
	vector<TRelationCluster> m_vecParentCluster;
	vector<TRelationCluster> m_vecSonCluster;
	unsigned int m_nLay;

	int ClearSelf()
	{
		m_nClusterId = 0xffffffff;
		m_nHostId = 0xffffffff;
		m_nLay = 0xffff;
		m_tSelfCluster.ClearSelf();
		m_vecParentCluster.clear();
		m_vecSonCluster.clear();
		return 0;
	}
	
	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
		nRet += bufMgr.PutInt(m_nClusterId);
		nRet += bufMgr.PutInt(m_nHostId);
		nRet += m_tSelfCluster.Encode(bufMgr);
		int nCdnParentClusterNum = (int)m_vecParentCluster.size();
		nRet += bufMgr.PutInt(nCdnParentClusterNum);
		for(int nA = 0; nA < nCdnParentClusterNum; ++ nA)
		{
			nRet += m_vecParentCluster[nA].Encode(bufMgr);
		}
		int nCdnSonClusterNum = (int)m_vecSonCluster.size();
		nRet += bufMgr.PutInt(nCdnSonClusterNum);
		for(int nA = 0; nA < nCdnSonClusterNum; ++ nA)
		{
			nRet += m_vecSonCluster[nA].Encode(bufMgr);
		}
		nRet += bufMgr.PutInt(m_nLay);
		return nRet;
	}

	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_nResult);
		nLen += sizeof(m_nClusterId);
		nLen += sizeof(m_nHostId);
		nLen += m_tSelfCluster.GetSize();
		int nCdnParentClusterNum = (int)m_vecParentCluster.size();
		nLen += sizeof(nCdnParentClusterNum);
		for (int nA = 0; nA < nCdnParentClusterNum; ++ nA)
		{
			nLen += m_vecParentCluster[nA].GetSize();
		}
		int nCdnSonClusterNum = (int)m_vecSonCluster.size();
		nLen += sizeof(nCdnSonClusterNum);
		for (int nA = 0; nA < nCdnSonClusterNum; ++ nA)
		{
			nLen += m_vecSonCluster[nA].GetSize();
		}
		nLen += sizeof(m_nLay);
		return nLen;
	}
};

class CGetNetReLationCmd
	: public CCdnCtlMgrBaseCmd<TGetNetReLationCmd, TGetNetReLationCmdRes>
{
public:
	CGetNetReLationCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<TGetNetReLationCmd, TGetNetReLationCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = GETNETRELATION_CMD_RES;
	}

	~CGetNetReLationCmd() {}
protected:
	int ProcessCmd();
};

#endif
