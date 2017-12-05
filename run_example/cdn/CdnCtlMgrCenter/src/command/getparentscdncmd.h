/*
 * =====================================================================================
 *
 *       Filename:  getparentscdncmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-03-29
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  yinhaichen, yinhaichen@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef __GETPARENTSCDNCMD_H__
#define __GETPARENTSCDNCMD_H__

#include "cdnctlmgrbasecmd.h"
#include "cdnclustermgr.h"
using namespace std;

struct TGetParentsCdnCmd
{
	int Decode(CBufMgr& bufMgr)
	{	
		return 0;
	}
};

struct TGetParentsCdnCmdRes
{
	char m_nResult;
	typedef vector<TCdnParent> TParents;
	vector<TParents> m_vecParentCluster;

    int Encode(CBufMgr &bufMgr)
    {
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nResult);
		int nCdnParentClusterNum = (int)m_vecParentCluster.size();
		nRet += bufMgr.PutInt(nCdnParentClusterNum);
		for(int nA = 0; nA < nCdnParentClusterNum; ++ nA)
		{
			int nCdnParentNum = m_vecParentCluster[nA].size();
			nRet += bufMgr.PutInt(nCdnParentNum);
			for (int nB = 0; nB < nCdnParentNum; nB ++)
			{
				nRet += bufMgr.PutChar(m_vecParentCluster[nA][nB].m_nState);
				nRet += bufMgr.PutInt(m_vecParentCluster[nA][nB].m_nIp);
			}
		}
		return nRet;
    }

    int GetSize()
    {
		int nLen = 0;
		nLen += sizeof(m_nResult);
		int nCdnParentClusterNum = (int)m_vecParentCluster.size();
		nLen += sizeof(nCdnParentClusterNum);
		for (int nA = 0; nA < nCdnParentClusterNum; ++ nA)
		{
			int nCdnParentNum = m_vecParentCluster[nA].size();
			nLen += sizeof(nCdnParentNum);
			for (int nB = 0; nB < nCdnParentNum; nB ++)
			{
				nLen += sizeof(m_vecParentCluster[nA][nB].m_nState);
				nLen += sizeof(m_vecParentCluster[nA][nB].m_nIp);
			}
		}
        return nLen;
    }
};

class CGetParentsCdnCmd
	: public CCdnCtlMgrBaseCmd<TGetParentsCdnCmd, TGetParentsCdnCmdRes>
{
public:
	CGetParentsCdnCmd(char* pBuf, int nLen)
		: CCdnCtlMgrBaseCmd<TGetParentsCdnCmd, TGetParentsCdnCmdRes>(pBuf, nLen) 
	{
		m_outHead.m_nCmd = GETPARENTSCDN_CMD_RES;
	}

	~CGetParentsCdnCmd() {}
protected:
	int ProcessCmd();
};

#endif


