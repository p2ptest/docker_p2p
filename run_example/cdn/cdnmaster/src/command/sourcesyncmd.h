/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcesyncmd.h
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
#ifndef __SOURCESYNCMD_H__
#define __SOURCESYNCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TSourceSyn
{
	char nType;

	int Decode(CBufMgr &bufMgr)
	{
		if (bufMgr.GetChar(nType)) {return -1;}
		if (nType != EST_HotSource) //QueryVP只会同步热资源
		{
			WriteRunInfo::WriteError("TSourceSyn Wrong Type %d", nType);
			return -1;
		}
		return 0;
	}
};

struct TSourceSynRes
{
	char m_cResult;
	vector<string> vecGcid;

	int Encode(CBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_cResult);
		int nVecSize = (int)vecGcid.size();
		nRet += bufMgr.PutInt(nVecSize);
		for (int i = 0; i < nVecSize; i ++)
		{
			nRet += bufMgr.PutString(vecGcid[i]);
		}
		return nRet;
	}
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_cResult);

		int nVecSize = (int)vecGcid.size();
		nLen += sizeof(nVecSize);
		nLen += nVecSize * (sizeof(int) + CID20_LEN);

		return nLen;
	}
};

class CSourceSynCmd: public MasterSrvDecodeCmd<TCmdHead, TSourceSyn>
{
public:
	CSourceSynCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TSourceSyn>(pBuf, nLen){}
	virtual ~CSourceSynCmd(){}
protected:
	int ProcessCmd();
private:
};

class CSourceSynCmdRes: public MasterSrvEncodeCmd<TCmdHead, TSourceSynRes>
{
public:
	CSourceSynCmdRes(CSourceSynCmd &cmdReq, int nType);
	virtual ~CSourceSynCmdRes(){}
protected:
	int ProcessCmd();
private:
	int m_nSourceType;
};

#endif
