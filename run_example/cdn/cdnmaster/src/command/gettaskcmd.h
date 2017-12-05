/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: gettaskcmd.h
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
#ifndef __GETTASKCMD_H__
#define __GETTASKCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TGetTask
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

struct TGetTaskRes
{
	char m_cResult;
	vector<TStateInfo> m_vecTask;

	int Encode(CBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_cResult);
		int nVecSize = (int)m_vecTask.size();
		nRet += bufMgr.PutInt(nVecSize);
		for (int i = 0; i < nVecSize; i ++)
		{
			nRet += bufMgr.PutString(m_vecTask[i].m_strGcid);
			nRet += bufMgr.PutChar(m_vecTask[i].m_cState);
		}
		return nRet;
	}
	int GetSize()
	{
		int nLen = 0;
		nLen += sizeof(m_cResult);
		int nVecSize = (int)m_vecTask.size();
		nLen += sizeof(nVecSize);
		for (int i = 0; i < nVecSize; i ++)
		{
			nLen += sizeof(int);
			nLen += m_vecTask[i].m_strGcid.length();
			nLen += sizeof(m_vecTask[i].m_cState);
		}
		return nLen;
	}
};

class CGetTaskCmd: public MasterSrvDecodeCmd<TCmdHead, TGetTask>
{
public:
	CGetTaskCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TGetTask>(pBuf, nLen){}
	virtual ~CGetTaskCmd(){}
	unsigned int GetIp();
protected:
	int ProcessCmd();
private:
};

class CGetTaskCmdRes: public MasterSrvEncodeCmd<TCmdHead, TGetTaskRes>
{
public:
	CGetTaskCmdRes(CGetTaskCmd &cmdReq, unsigned int nIp);
	virtual ~CGetTaskCmdRes(){}
protected:
	int ProcessCmd();
private:
	unsigned int m_nIp;
};

#endif
