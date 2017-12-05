/*
 * =====================================================================================
 *
 *       Filename:  getcdnsrvinfocmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-03-18
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  yinhaichen, yinhaichen@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef __GETCDNSRVINFOCMD_H__
#define __GETCDNSRVINFOCMD_H__

#include "cdnctlmgrbasecmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct TGetCdnSrvInfoCmd
{
	int Decode(CBufMgr& bufMgr)
	{		
		return 0;
	}
};


struct TCdnStatus
{
    unsigned int m_nHostId; //for 3rdgetcdnsrvinfocmd use only
	int m_nState;
	unsigned int m_nPublicIp;
	unsigned int m_nPrivateIp;
};

struct TGetCdnSrvInfoCmdRes
{
	char m_nResult;
	unsigned int m_limitSpeed;
	vector<TCdnStatus> m_vecCdnSrvInfo;

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
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

class CGetCdnSrvInfoCmd
	: public CCdnCtlMgrBaseCmd<TGetCdnSrvInfoCmd, TGetCdnSrvInfoCmdRes>
{
public:
	CGetCdnSrvInfoCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<TGetCdnSrvInfoCmd, TGetCdnSrvInfoCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = GETCDNSRVINFO_CMD_RES;
	}

	~CGetCdnSrvInfoCmd() {}
protected:
	int ProcessCmd();
};

#endif


