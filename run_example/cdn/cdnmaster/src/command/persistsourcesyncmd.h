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
#ifndef __PERSISTSOURCESYNCMD_H__
#define __PERSISTSOURCESYNCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TPersistSourceSyn
{
	char m_cIfNatIp;
	int m_nIp;

	int Decode(CBufMgr &bufMgr)
	{
		if (bufMgr.GetChar(m_cIfNatIp)) {return -1;}
		if (bufMgr.GetInt(m_nIp)) {return -1;}
		return 0;
	}
};

struct TPersistSourceSynRes
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

class CPersistSourceSynCmd: public MasterSrvDecodeCmd<TCmdHead, TPersistSourceSyn>
{
public:
	CPersistSourceSynCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TPersistSourceSyn>(pBuf, nLen){}
	virtual ~CPersistSourceSynCmd(){}
	unsigned int GetIp();
protected:
	int ProcessCmd();
private:
};

class CPersistSourceSynCmdRes: public MasterSrvEncodeCmd<TCmdHead, TPersistSourceSynRes>
{
public:
	CPersistSourceSynCmdRes(CPersistSourceSynCmd &cmdReq, unsigned int nClientIp);
	virtual ~CPersistSourceSynCmdRes(){}
protected:
	int ProcessCmd();
private:
	unsigned int m_nIp;
};

#endif
