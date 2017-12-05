/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcereportcmd.cpp
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
#include "sourcereportcmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CSourceReportCmd::ProcessCmd()
{
	int nSourceNum = m_CmdBody.m_vecSourceState.size();
	string strGcid;
	int nType = 0;
	int nC2H = 0;
	int nH2C = 0;
	for (int i = 0; i < nSourceNum; i ++)
	{
		strGcid = m_CmdBody.m_vecSourceState[i].m_strGcid;
		nType = (int)m_CmdBody.m_vecSourceState[i].m_cState;
	
		int nRet = 0;
		if (nType == EST_HotSource)
		{
			nRet = CSourceManager::GetInstance()->SourceColdToHot(strGcid);
			nC2H ++;
		}
		else if (nType == EST_ColdSource)
		{
			nRet = CSourceManager::GetInstance()->SourceHotToCold(strGcid);
			nH2C ++;
		}
		else
		{
			nRet = -1;
		}
		if (nRet != 0)
		{
			WriteRunInfo::WriteError("CSourceReportCmd::UpdateSourceState : gcid=%s,type=%d",
					CToolKit::BiToHex(strGcid.c_str(), strGcid.length()).c_str(), nType);
			return nRet;
		}
	}
	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "statsrv report [Total:%d]-[C2H:%d]-[H2C:%d]", nSourceNum, nC2H, nH2C);
	return 0;
}

CSourceReportCmdRes::CSourceReportCmdRes(CSourceReportCmd &cmdReq, int nResult)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = SOURCEREPORTRESCMD;
	if (nResult != 0)
	{
		m_CmdBody.m_cResult = -1;
	}
	else
	{
		m_CmdBody.m_cResult = 0;
	}
}

int CSourceReportCmdRes::ProcessCmd()
{
	return 0;
}
