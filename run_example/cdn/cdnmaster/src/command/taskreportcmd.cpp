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
#include "taskreportcmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

void CTaskReportCmd::SetIp(int nClientIp)
{
	m_nClientIp = nClientIp;
}

int CTaskReportCmd::ProcessCmd()
{
	int nTaskNum = m_CmdBody.m_vecTaskState.size();

	int nIp = (1 == m_CmdBody.m_nIfNat)? m_CmdBody.m_nNatIp:m_nClientIp;
	struct in_addr tAddr;
	tAddr.s_addr = htonl(nIp);
	
	string strGcid;
	for (int i = 0; i < nTaskNum; i ++)
	{
		if (m_CmdBody.m_vecTaskState[i].m_cState == 0)
		{
			CSourceManager::GetInstance()->ReportCacheTask(nIp, m_CmdBody.m_vecTaskState[i]);
		}
	    WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "CTaskReportCmd::ProcessCmd : cache %s report tasks %s state %d", 
                inet_ntoa(tAddr), 
                CToolKit::BiToHex(m_CmdBody.m_vecTaskState[i].m_strGcid.c_str(), m_CmdBody.m_vecTaskState[i].m_strGcid.length()).c_str(), 
                m_CmdBody.m_vecTaskState[i].m_cState);
	}

	return 0;
}

CTaskReportCmdRes::CTaskReportCmdRes(CTaskReportCmd &cmdReq, int nResult)
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

int CTaskReportCmdRes::ProcessCmd()
{
	return 0;
}
