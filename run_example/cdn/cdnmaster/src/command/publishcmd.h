/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: publishcmd.h
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
#ifndef __PUBLISHCMD_H__
#define __PUBLISHCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TPublish
{
	string m_strGcid;
	char m_bIfHot;

	int Decode(CBufMgr &bufMgr)
	{
		if (bufMgr.GetString(m_strGcid)) {return -1;}
		if (m_strGcid.size() != CID20_LEN)
		{
			return -1;
		}
		if (bufMgr.GetChar(m_bIfHot)) {return -1;}
		return 0;
	}
};

struct TPublishRes
{
	char m_cResult;

	int Encode(CBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_cResult);
		return nRet;
	}
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_cResult);
		return nLen;
	}
};

class CPublishCmd: public MasterSrvDecodeCmd<TCmdHead, TPublish>
{
public:
	CPublishCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TPublish>(pBuf, nLen){}
	virtual ~CPublishCmd(){}
protected:
	int ProcessCmd();
private:
};

class CPublishCmdRes: public MasterSrvEncodeCmd<TCmdHead, TPublishRes>
{
public:
	CPublishCmdRes(CPublishCmd &cmdReq, int nResult);
	virtual ~CPublishCmdRes(){}
protected:
	int ProcessCmd();
private:
};

#endif
