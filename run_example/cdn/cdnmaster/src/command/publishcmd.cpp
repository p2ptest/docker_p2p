/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: publishcmd.cpp
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
#include "publishcmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CPublishCmd::ProcessCmd()
{
	TSourceInfo tTmpSourceInfo;
	tTmpSourceInfo.m_strGcid = m_CmdBody.m_strGcid;
	tTmpSourceInfo.m_nSourceType = (m_CmdBody.m_bIfHot) ? EST_HotSource:EST_ColdSource;
	tTmpSourceInfo.m_nPublishTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
	tTmpSourceInfo.m_nStartTime = tTmpSourceInfo.m_nPublishTime;
	tTmpSourceInfo.m_nLastReportTime= tTmpSourceInfo.m_nPublishTime;
	int nRet = CSourceManager::GetInstance()->PublishSource(tTmpSourceInfo);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("CPublishCmd::InsertSource fail %d", nRet);
		return nRet;
	}

	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "publish suc, gcid: %s, srctype: %d", CToolKit::BiToHex(tTmpSourceInfo.m_strGcid.c_str(), tTmpSourceInfo.m_strGcid.size()).c_str(), tTmpSourceInfo.m_nSourceType);
	return 0;
}

CPublishCmdRes::CPublishCmdRes(CPublishCmd &cmdReq, int nResult)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = PUBLISHRESCMD;
	if (nResult == PUBLISH_EXIST)
	{
		m_CmdBody.m_cResult = PUBLISH_EXIST;
	}
	else if (nResult == 0)
	{
		m_CmdBody.m_cResult = 0;
	}
	else
	{
		m_CmdBody.m_cResult = -1;
	}
}

int CPublishCmdRes::ProcessCmd()
{
	return 0;
}
