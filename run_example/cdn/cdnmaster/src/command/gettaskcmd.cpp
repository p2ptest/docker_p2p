/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: gettaskcmd.cpp
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
#include "gettaskcmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CGetTaskCmd::ProcessCmd()
{
	return 0;
}

unsigned int CGetTaskCmd::GetIp()
{
	if (m_CmdBody.m_cIfNatIp == 1)
	{
		return m_CmdBody.m_nIp;
	}
	return 0;
}

CGetTaskCmdRes::CGetTaskCmdRes(CGetTaskCmd &cmdReq, unsigned int nIp)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = GETTASKRESCMD;
	m_CmdBody.m_cResult = 0;
	m_nIp = nIp;
}

int CGetTaskCmdRes::ProcessCmd()
{
	vector<TTaskInfo> vecTmpTaskInfo;
	CSourceManager::GetInstance()->GetCacheTask(m_nIp, vecTmpTaskInfo);
	int nTaskNum = (int)vecTmpTaskInfo.size();
	TStateInfo tTmpTaskInfo;
	for (int i = 0; i < nTaskNum; i ++)
	{
		tTmpTaskInfo.m_strGcid = vecTmpTaskInfo[i].m_strGcid;
		tTmpTaskInfo.m_cState = (char)vecTmpTaskInfo[i].m_nTaskType;
		m_CmdBody.m_vecTask.push_back(tTmpTaskInfo);
	}

	if ((WriteRunInfo::LOGTYPEARRAY[MASTERSRV_DEBUG_TYPE].nLogOff))
	{
		struct in_addr tAddr;
		tAddr.s_addr = htonl(m_nIp);
		WriteRunInfo::WriteAllLog(MASTERSRV_DEBUG_TYPE, "%s get %d tasks", inet_ntoa(tAddr), nTaskNum);
		for (int i = 0; i < nTaskNum; i ++)
		{
			string strHexGCid = CToolKit::BiToHex(vecTmpTaskInfo[i].m_strGcid.c_str(), vecTmpTaskInfo[i].m_strGcid.length());
			WriteRunInfo::WriteAllLog(MASTERSRV_DEBUG_TYPE, "%s get tasks %s", inet_ntoa(tAddr), strHexGCid.c_str());
		}
	}

	return 0;
}
