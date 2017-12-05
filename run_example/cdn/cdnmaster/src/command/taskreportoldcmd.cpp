/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: taskreportcmd.cpp
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
#include "taskreportoldcmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

void CTaskReportOldCmd::SetIp(int nClientIp)
{
	m_nClientIp = nClientIp;
}

int CTaskReportOldCmd::ProcessCmd()
{
	int nTaskNum = m_CmdBody.m_vecTaskState.size();
	string strGcid;
	struct in_addr tAddr;
	tAddr.s_addr = htonl(m_nClientIp);
	
	for (int i = 0; i < nTaskNum; i ++)
	{
		if (m_CmdBody.m_vecTaskState[i].m_cState == 0)
		{
			CSourceManager::GetInstance()->ReportCacheTask(m_nClientIp, m_CmdBody.m_vecTaskState[i]);
		}
	}
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CTaskReportOldCmd::ProcessCmd : cache %s report %d tasks", inet_ntoa(tAddr), nTaskNum);
	return 0;
}

CTaskReportOldCmdRes::CTaskReportOldCmdRes(CTaskReportOldCmd &cmdReq, int nResult)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = TASKREPORTRESCMD;
	if (nResult != 0)
	{
		m_CmdBody.m_cResult = -1;
	}
	else
	{
		m_CmdBody.m_cResult = 0;
	}
}

int CTaskReportOldCmdRes::ProcessCmd()
{
	return 0;
}
