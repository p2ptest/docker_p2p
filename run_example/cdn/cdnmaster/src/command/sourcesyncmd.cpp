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
#include "sourcesyncmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CSourceSynCmd::ProcessCmd()
{
	return 0;
}

CSourceSynCmdRes::CSourceSynCmdRes(CSourceSynCmd &cmdReq, int nType)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = SOURCESYNRESCMD;
	m_CmdBody.m_cResult = 0;
	m_nSourceType = nType;
}

int CSourceSynCmdRes::ProcessCmd()
{
	m_CmdBody.vecGcid.clear();
	int nRet = CSourceManager::GetInstance()->GetGCidByType(m_nSourceType, m_CmdBody.vecGcid);
	if (nRet != 0)
	{
		WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "source syn fail, type = %d", m_nSourceType);
		m_CmdBody.m_cResult = -1;
		return 0;
	}
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "syn %u source, type = %d", m_CmdBody.vecGcid.size(), m_nSourceType);
	return 0;
}

