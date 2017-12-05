/*
 * =====================================================================================
 *
 *       Filename:  newgetcdnsrvinfocmd.h
 *
 *    Description:  
 *
 *        Version:  2.0
 *        Created:  2012-04-01
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  hezhipeng, hezhipeng@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef __3RDGETCDNSRVINFOCMD_H__
#define __3RDGETCDNSRVINFOCMD_H__

#include "cdnctlmgrbasecmd.h"
#include "getcdnsrvinfocmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct T3rdGetCdnSrvInfoCmd
{
	char m_nIsNatIP;
	int m_nIp;

	T3rdGetCdnSrvInfoCmd()
	{
		m_nIsNatIP = 0;
		m_nIp = 0;
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


struct T3rdGetCdnSrvInfoCmdRes
{
	char m_nResult;
	unsigned int m_nClusterId;
	char m_nIsHot;
	char m_nHash;
	char m_LimitMode;
	unsigned int m_LimitSpeed;
	unsigned int m_MaxSpeed;
	unsigned int m_MultipleSpeed;
	vector<TCdnStatus> m_vecCdnSrvInfo;

	int ClearSelf()
	{
		m_nClusterId = -1;
		m_nIsHot = -1;
		m_nHash = -1;
		m_LimitMode = -1;
		m_LimitSpeed = -1;
		m_MaxSpeed = -1;
		m_MultipleSpeed = -1;
		m_vecCdnSrvInfo.clear();
		return 0;
	}

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
		nRet += bufMgr.PutInt(m_nClusterId);
		nRet += bufMgr.PutChar(m_nIsHot);
		nRet += bufMgr.PutChar(m_nHash);
		nRet += bufMgr.PutChar(m_LimitMode);
		nRet += bufMgr.PutInt((int)m_LimitSpeed);
		nRet += bufMgr.PutInt((int)m_MaxSpeed);
		nRet += bufMgr.PutInt((int)m_MultipleSpeed);
		int nCdnSrvNum = (int)m_vecCdnSrvInfo.size();
		nRet += bufMgr.PutInt(nCdnSrvNum);
		for(int nA = 0; nA < nCdnSrvNum; ++ nA)
		{
			nRet += bufMgr.PutInt(m_vecCdnSrvInfo[nA].m_nHostId);
			nRet += bufMgr.PutInt(m_vecCdnSrvInfo[nA].m_nState);
			nRet += bufMgr.PutInt(m_vecCdnSrvInfo[nA].m_nPublicIp);
			nRet += bufMgr.PutInt(m_vecCdnSrvInfo[nA].m_nPrivateIp);
		}
		return nRet;
	}

	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_nResult);
		nLen += sizeof(m_nClusterId);
		nLen += sizeof(m_nIsHot);
		nLen += sizeof(m_nHash);
		nLen += sizeof(m_LimitMode);
		nLen += sizeof(m_LimitSpeed);
		nLen += sizeof(m_MaxSpeed);
		nLen += sizeof(m_MultipleSpeed);
		int nCdnSrvNum = (int)m_vecCdnSrvInfo.size();
		nLen += sizeof(nCdnSrvNum);
		for (int nA = 0; nA < nCdnSrvNum; ++ nA)
		{
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nHostId);
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nState);
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nPublicIp);
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nPrivateIp);
		}
		return nLen;
	}
};

class C3rdGetCdnSrvInfoCmd
	: public CCdnCtlMgrBaseCmd<T3rdGetCdnSrvInfoCmd, T3rdGetCdnSrvInfoCmdRes>
{
public:
	C3rdGetCdnSrvInfoCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<T3rdGetCdnSrvInfoCmd, T3rdGetCdnSrvInfoCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = _3RDGETCDNSRVINFO_CMD_RES;
	}

	~C3rdGetCdnSrvInfoCmd() {}
protected:
	int ProcessCmd();
};

#endif

