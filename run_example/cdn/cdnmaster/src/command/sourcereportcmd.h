/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcereportcmd.h
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
#ifndef __SOURCEREPORTCMD_H__
#define __SOURCEREPORTCMD_H__

#include "framecommon/framecommon.h"
#include "mastersrvbasecmd.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

struct TSourceReport
{
	vector<TStateInfo> m_vecSourceState;

	int Decode(CBufMgr &bufMgr)
	{
		int nVecNum = (int)m_vecSourceState.size();
		if (bufMgr.GetInt(nVecNum)) {return -1;}
		TStateInfo tTmpSource;
		for (int i = 0; i < nVecNum; i ++)
		{
			if (bufMgr.GetString(tTmpSource.m_strGcid)) {return -1;}
			if (tTmpSource.m_strGcid.size() != CID20_LEN) {return -1;}

			if (bufMgr.GetChar(tTmpSource.m_cState)) {return -1;}

			m_vecSourceState.push_back(tTmpSource);
		}
		return 0;
	}
};

struct TSourceReportRes
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

class CSourceReportCmd: public MasterSrvDecodeCmd<TCmdHead, TSourceReport>
{
public:
	CSourceReportCmd(char* pBuf, int nLen)
		: MasterSrvDecodeCmd<TCmdHead, TSourceReport>(pBuf, nLen){}
	virtual ~CSourceReportCmd(){}
protected:
	int ProcessCmd();
private:
};

class CSourceReportCmdRes: public MasterSrvEncodeCmd<TCmdHead, TSourceReportRes>
{
public:
	CSourceReportCmdRes(CSourceReportCmd &cmdReq, int nResult);
	virtual ~CSourceReportCmdRes(){}
protected:
	int ProcessCmd();
private:
};

#endif
