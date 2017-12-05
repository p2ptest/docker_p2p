/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourceincsyncmd.cpp
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
#include "sourceincsyn4cachecmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"
#include "taskmanager.h"

int CSourceIncSyn4CacheCmd::ProcessCmd()
{
	return 0;
}

unsigned int CSourceIncSyn4CacheCmd::GetIp()
{
	if (m_CmdBody.m_cIfNatIp == 1)
	{
		return m_CmdBody.m_nIp;
	}
	return 0;
}


CSourceIncSyn4CacheCmdRes::CSourceIncSyn4CacheCmdRes(CSourceIncSyn4CacheCmd &cmdReq, unsigned int nIp)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = SOURCEINCSYN4CACHERESCMD;
	m_CmdBody.m_cResult = 0;
	m_nIp = nIp;
}

int CSourceIncSyn4CacheCmdRes::ProcessCmd()
{
	unsigned int nClusterId = 0;
	unsigned int nIndex = 0;
	unsigned int nNum = 0;
	struct in_addr tAddr;
	tAddr.s_addr = htonl(m_nIp);

	int nRet = CTaskManager::GetInstance()->GetHostIndex(m_nIp, nClusterId, nIndex, nNum);
	if (nRet != 0)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "%s inc cache source syn fail1", inet_ntoa(tAddr));
		if (nRet == -1)
		{
			m_CmdBody.m_cResult = -1;
		}
		else
		{
			//冷集群，不用返回增量同步
			m_CmdBody.m_cResult = 0;
		}
		return 0;
	}
	///////////////////////////////////////////////////////////////////////////////////////
	vector<TSourceInfo> vecSourceInfo;
	CSourceManager::GetInstance()->GetIncSourceByIndex(nClusterId, nIndex, nNum, vecSourceInfo);
	TStateInfo tTmpState;
	int nSourceNum = (int)vecSourceInfo.size();
	for (int i = 0; i < nSourceNum; i ++)
	{
		tTmpState.m_strGcid = vecSourceInfo[i].m_strGcid;
		tTmpState.m_cState = (char)vecSourceInfo[i].m_nSourceType;
		m_CmdBody.m_vecSourceState.push_back(tTmpState);
	}
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "%s cache incsyn %d source", inet_ntoa(tAddr), nSourceNum);
	return 0;
}
