/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourceincsyncmd.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-04-01			1.0			
* =======================================================================
*/
#ifndef __SOURCEINCSYNCMD_H__
#define __SOURCEINCSYNCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TSourceIncSyn
{
	int Decode(CBufMgr &bufMgr)
	{
		return 0;
	}
};

struct TSourceIncSynRes
{
	char m_cResult;
	vector<TStateInfo> m_vecSourceState;

	int Encode(CBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_cResult);
		int nVecNum = (int)m_vecSourceState.size();
		if (bufMgr.PutInt(nVecNum)) {return -1;}
		for (int i = 0; i < nVecNum; i ++)
		{
			if (bufMgr.PutString(m_vecSourceState[i].m_strGcid)) {return -1;}
			if (bufMgr.PutChar(m_vecSourceState[i].m_cState)) {return -1;}
		}
		return nRet;
	}
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_cResult);
		int nVecSize = (int)m_vecSourceState.size();
		nLen += sizeof(nVecSize);
		for (int i = 0; i < nVecSize; i ++)
		{
			nLen += sizeof(int);
			nLen += m_vecSourceState[i].m_strGcid.length();
			nLen += sizeof(m_vecSourceState[i].m_cState);
		}
		return nLen;
	}
};

class CSourceIncSynCmd: public MasterSrvDecodeCmd<TCmdHead, TSourceIncSyn>
{
public:
	CSourceIncSynCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TSourceIncSyn>(pBuf, nLen){}
	virtual ~CSourceIncSynCmd(){}
protected:
	int ProcessCmd();
private:
};

class CSourceIncSynCmdRes: public MasterSrvEncodeCmd<TCmdHead, TSourceIncSynRes>
{
public:
	CSourceIncSynCmdRes(CSourceIncSynCmd &cmdReq);
	virtual ~CSourceIncSynCmdRes(){}
protected:
	int ProcessCmd();
private:
};

#endif
