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
#ifndef __SOURCESYN4CACHECMD_H__
#define __SOURCESYN4CACHECMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TSourceSyn4Cache
{
	char m_cIfNatIp;
	int m_nIp;
	char nType;

	int Decode(CBufMgr &bufMgr)
	{
		if (bufMgr.GetChar(m_cIfNatIp)) {return -1;}
		if (bufMgr.GetInt(m_nIp)) {return -1;}
		if (bufMgr.GetChar(nType)) {return -1;}
		if (nType != EST_HotSource) //Cache只会同步热资源
		{
			WriteRunInfo::WriteError("TSourceSyn4Cache Wrong Type %d", nType);
			return -1;
		}
		return 0;
	}
};

struct TSourceSyn4CacheRes
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

class CSourceSyn4CacheCmd: public MasterSrvDecodeCmd<TCmdHead, TSourceSyn4Cache>
{
public:
	CSourceSyn4CacheCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TSourceSyn4Cache>(pBuf, nLen){}
	virtual ~CSourceSyn4CacheCmd(){}
	unsigned int GetIp();
protected:
	int ProcessCmd();
private:
};

class CSourceSyn4CacheCmdRes: public MasterSrvEncodeCmd<TCmdHead, TSourceSyn4CacheRes>
{
public:
	CSourceSyn4CacheCmdRes(CSourceSyn4CacheCmd &cmdReq, int nType, unsigned int nIp);
	virtual ~CSourceSyn4CacheCmdRes(){}
protected:
	int ProcessCmd();
private:
	int m_nSourceType;
	unsigned int m_nIp;
};

#endif
