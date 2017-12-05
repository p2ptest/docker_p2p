/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: taskreportcmd.h
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
#ifndef __TASKREPORTOLDCMD_H__
#define __TASKREPORTOLDCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TTaskReportOld
{
	vector<TStateInfo> m_vecTaskState;

	int Decode(CBufMgr &bufMgr)
	{
		int nTaskNum;
		if (bufMgr.GetInt(nTaskNum) != 0) {return -1;}
		TStateInfo tTmpState;
		for (int i = 0; i < nTaskNum; i ++)
		{
			if (bufMgr.GetString(tTmpState.m_strGcid) != 0) {return -1;}
			if (tTmpState.m_strGcid.size() != CID20_LEN)
			{
				return -1;
			}
			if (bufMgr.GetChar(tTmpState.m_cState) != 0) {return -1;}
			m_vecTaskState.push_back(tTmpState);
		}
		return 0;
	}
};

struct TTaskReportOldRes
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

class CTaskReportOldCmd : public MasterSrvDecodeCmd<TCmdHead, TTaskReportOld>
{
public:
	CTaskReportOldCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TTaskReportOld>(pBuf, nLen){m_nClientIp = 0;}
	virtual ~CTaskReportOldCmd(){}
	void SetIp(int nClientIp);
protected:
	int ProcessCmd();
private:
	int m_nClientIp;
};

class CTaskReportOldCmdRes: public MasterSrvEncodeCmd<TCmdHead, TTaskReportOldRes>
{
public:
	CTaskReportOldCmdRes(CTaskReportOldCmd &cmdReq, int nResult);
	virtual ~CTaskReportOldCmdRes(){}
protected:
	int ProcessCmd();
private:
};

#endif
