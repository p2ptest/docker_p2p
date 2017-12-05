/*
 * =====================================================================================
 *
 *       Filename:  getparentcdncmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-28
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  wangtiefeng, wangtiefeng@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef __GETPARENTCDNCMD_H_5275A63A_1227_11E0_ACB3_0015C55D3D75_
#define __GETPARENTCDNCMD_H_5275A63A_1227_11E0_ACB3_0015C55D3D75_

#include "cdnctlmgrbasecmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct TGetParentCdnCmd
{
	unsigned m_nIp;

	int Decode(CBufMgr& bufMgr)
	{
		if (bufMgr.GetInt((int&)m_nIp))
		{
			return -1;
		}		
		return 0;
	}
};

struct TGetParentCdnCmdRes
{
	char m_nResult;
	vector<TCdnParent> m_vecCdnParent;

    int Encode(CBufMgr &bufMgr)
    {
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
		int nCdnParentNum = (int)m_vecCdnParent.size();
		nRet += bufMgr.PutInt(nCdnParentNum);
		for(int nA = 0; nA < nCdnParentNum; ++ nA)
		{
			nRet += bufMgr.PutChar(m_vecCdnParent[nA].m_nState);
			nRet += bufMgr.PutInt(m_vecCdnParent[nA].m_nIp);
		}
		return nRet;
    }

    int GetSize()
    {
		int nLen = 0;
		nLen += sizeof(m_nResult);
		int nCdnParentNum = (int)m_vecCdnParent.size();
		nLen += sizeof(nCdnParentNum);
		for (int nA = 0; nA < nCdnParentNum; ++ nA)
		{
			nLen += sizeof(m_vecCdnParent[nA].m_nState);
			nLen += sizeof(m_vecCdnParent[nA].m_nIp);
		}
        return nLen;
    }
};

class CGetParentCdnCmd
	: public CCdnCtlMgrBaseCmd<TGetParentCdnCmd, TGetParentCdnCmdRes>
{
public:
	CGetParentCdnCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<TGetParentCdnCmd, TGetParentCdnCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = GETPARENTCDN_CMD_RES;
	}

	~CGetParentCdnCmd() {}
protected:
	int ProcessCmd();
};

#endif


