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
#include "sourceincsyncmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CSourceIncSynCmd::ProcessCmd()
{
	return 0;
}

CSourceIncSynCmdRes::CSourceIncSynCmdRes(CSourceIncSynCmd &cmdReq)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = SOURCEINCSYNRESCMD;
	m_CmdBody.m_cResult = 0;
}

int CSourceIncSynCmdRes::ProcessCmd()
{
	vector<TSourceInfo> vecSourceInfo;
	CSourceManager::GetInstance()->GetIncSource(vecSourceInfo);
	TStateInfo tTmpState;
	int nSourceNum = (int)vecSourceInfo.size();
	for (int i = 0; i < nSourceNum; i ++)
	{
		tTmpState.m_strGcid = vecSourceInfo[i].m_strGcid;
		tTmpState.m_cState = (char)vecSourceInfo[i].m_nSourceType;
		m_CmdBody.m_vecSourceState.push_back(tTmpState);
	}
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "incsyn %d source", nSourceNum);
	return 0;
}

