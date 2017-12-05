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
#ifndef __NEWGETCDNSRVINFOCMD_H__
#define __NEWGETCDNSRVINFOCMD_H__

#include "cdnctlmgrbasecmd.h"
#include "getcdnsrvinfocmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct TNewGetCdnSrvInfoCmd
{
	char m_nIsNatIP;
	int m_nIp;

	TNewGetCdnSrvInfoCmd()
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


struct TNewGetCdnSrvInfoCmdRes
{
	char m_nResult;
	unsigned int m_nClusterId;
	char m_nIsHot;
	unsigned int m_limitSpeed;
	vector<TCdnStatus> m_vecCdnSrvInfo;

	int ClearSelf()
	{
		m_nClusterId = -1;
		m_nIsHot = -1;
		m_limitSpeed = -1;
		m_vecCdnSrvInfo.clear();
		return 0;
	}

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
		nRet += bufMgr.PutInt(m_nClusterId);
		nRet += bufMgr.PutChar(m_nIsHot);
		nRet += bufMgr.PutInt((int)m_limitSpeed);
		int nCdnSrvNum = (int)m_vecCdnSrvInfo.size();
		nRet += bufMgr.PutInt(nCdnSrvNum);
		for(int nA = 0; nA < nCdnSrvNum; ++ nA)
		{
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
		nLen += sizeof(m_limitSpeed);
		int nCdnSrvNum = (int)m_vecCdnSrvInfo.size();
		nLen += sizeof(nCdnSrvNum);
		for (int nA = 0; nA < nCdnSrvNum; ++ nA)
		{
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nState);
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nPublicIp);
			nLen += sizeof(m_vecCdnSrvInfo[nA].m_nPrivateIp);
		}
		return nLen;
	}
};

class CNewGetCdnSrvInfoCmd
	: public CCdnCtlMgrBaseCmd<TNewGetCdnSrvInfoCmd, TNewGetCdnSrvInfoCmdRes>
{
public:
	CNewGetCdnSrvInfoCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<TNewGetCdnSrvInfoCmd, TNewGetCdnSrvInfoCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = NEWGETCDNSRVINFO_CMD_RES;
	}

	~CNewGetCdnSrvInfoCmd() {}
protected:
	int ProcessCmd();
};

#endif

