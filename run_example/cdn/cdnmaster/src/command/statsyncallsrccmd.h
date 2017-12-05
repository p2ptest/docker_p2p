/**
 * ============================================================================
 * @file    statsyncallsrccmd.h
 *
 * @brief   a command definition between statsrv and cdnmaster.
 * @details 
 *
 * @version 1.0  
 * @date    2013-03-23 05:51:32
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#ifndef __STATSYNCALLSRCCMD_H__
#define __STATSYNCALLSRCCMD_H__ 
#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace MYFRAMECOMMON;

struct TStatSourceEntireSync
{
	int Decode(CBufMgr &bufMgr)
	{
		return 0;
	}
};

struct TStatSourceEntireSyncRes
{
	char m_chResult;
	vector<string> m_vecHotSource;
	vector<string> m_vecColdSource;

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_chResult);
		int nHotSrcNum = (int)m_vecHotSource.size();
		nRet += bufMgr.PutInt(nHotSrcNum);
		for (int nA = 0; nA < nHotSrcNum; nA ++)
		{
			nRet += bufMgr.PutString(m_vecHotSource[nA]);
		}

		int nColdSrcNum = (int)m_vecColdSource.size();
		nRet += bufMgr.PutInt(nColdSrcNum);
		for (int nA = 0; nA < nColdSrcNum; nA ++)
		{
			nRet += bufMgr.PutString(m_vecColdSource[nA]);
		}

		return nRet;
	}

	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_chResult);

		int nHotSrcNum = (int)m_vecHotSource.size();
		nLen += sizeof(nHotSrcNum);
		nLen += nHotSrcNum * (sizeof(int) + CID20_LEN);

		int nColdSrcNum = (int)m_vecColdSource.size();
		nLen += sizeof(nColdSrcNum);
		nLen += nColdSrcNum * (sizeof(int) + CID20_LEN);

		return nLen;
	}
};

class CStatSourceEntireSyncCmd : public MasterSrvDecodeCmd<TCmdHead, TStatSourceEntireSync>
{
public:
	CStatSourceEntireSyncCmd(char *pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TStatSourceEntireSync>(pBuf, nLen) {}
	virtual ~CStatSourceEntireSyncCmd() {}

protected:
	int ProcessCmd() {return 0;}
};

class CStatSourceEntireSyncResCmd : public MasterSrvEncodeCmd<TCmdHead,	TStatSourceEntireSyncRes>
{

public:
	CStatSourceEntireSyncResCmd(CStatSourceEntireSyncCmd &cmdReq);
	virtual ~CStatSourceEntireSyncResCmd() {}

protected:
	int ProcessCmd();
};

#endif 

