/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: deletecmd.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-06-21
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-06-21			1.0			
* =======================================================================
*/
#ifndef __DELETECMD_H__
#define __DELETECMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TDelete
{
	string m_strGcid;
	char m_cDeleteType;

	int Decode(CBufMgr &bufMgr)
	{
		if (bufMgr.GetString(m_strGcid)) {return -1;}
		if (m_strGcid.size() != CID20_LEN) {return -1;}

		if (bufMgr.GetChar(m_cDeleteType)) {return -1;}
		return 0;
	}
};

struct TDeleteRes
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

class CDeleteCmd: public MasterSrvDecodeCmd<TCmdHead, TDelete>
{
public:
	CDeleteCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TDelete>(pBuf, nLen){}
	virtual ~CDeleteCmd(){}
protected:
	int ProcessCmd();
private:
};

class CDeleteCmdRes: public MasterSrvEncodeCmd<TCmdHead, TDeleteRes>
{
public:
	CDeleteCmdRes(CDeleteCmd &cmdReq, int nResult);
	virtual ~CDeleteCmdRes(){}
protected:
	int ProcessCmd();
private:
};

#endif
