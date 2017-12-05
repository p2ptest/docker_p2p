/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcesyncmd.cpp
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
#include "sourcesyn4cachecmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"
#include "taskmanager.h"

int CSourceSyn4CacheCmd::ProcessCmd()
{
	return 0;
}

unsigned int CSourceSyn4CacheCmd::GetIp()
{
	if (m_CmdBody.m_cIfNatIp == 1)
	{
		return m_CmdBody.m_nIp;
	}
	return 0;
}

CSourceSyn4CacheCmdRes::CSourceSyn4CacheCmdRes(CSourceSyn4CacheCmd &cmdReq, int nType, unsigned int nIp)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = SOURCESYN4CACHERESCMD;
	m_CmdBody.m_cResult = 0;
	m_nSourceType = nType;
	m_nIp = nIp;
}

int CSourceSyn4CacheCmdRes::ProcessCmd()
{
	m_CmdBody.vecGcid.clear();

	unsigned int nClusterId = 0;
	unsigned int nIndex = 0;
	unsigned int nNum = 0;
	struct in_addr tAddr;
	tAddr.s_addr = htonl(m_nIp);

	int nRet = CTaskManager::GetInstance()->GetHostIndex(m_nIp, nClusterId, nIndex, nNum);
	if (nRet != 0)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "%s persist source syn fail1", inet_ntoa(tAddr));
		if (nRet == -1)
		{
			m_CmdBody.m_cResult = -1;
		}
		else
		{
			//冷集群，不用返回热资源列表
			m_CmdBody.m_cResult = 0;
		}
		return 0;
	}
	//////////////////////////////////////////////////////////////////
	nRet = CSourceManager::GetInstance()->GetGCidByTypeAndIndex(EST_HotSource, nClusterId, nIndex, nNum, m_CmdBody.vecGcid);
	if (nRet != 0)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "%s source cache syn fail2, type = %d", inet_ntoa(tAddr), EST_HotSource);
		m_CmdBody.m_cResult = -1;
		return 0;
	}
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "%s syn %u source cache, type = %d", inet_ntoa(tAddr), m_CmdBody.vecGcid.size(), EST_PaidSource);

	return 0;
}

