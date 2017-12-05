/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: deletecmd.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-06-21
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-06-21			1.0			
* =======================================================================
*/
#include "deletecmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

int CDeleteCmd::ProcessCmd()
{
	if (m_CmdBody.m_cDeleteType != ETT_DeleteForeverTask)
	{
		WriteRunInfo::WriteError("CDeleteCmd::ProcessCmd : invalid delete type");
		return -1;
	}
	int nRet = CSourceManager::GetInstance()->DeleteSource(m_CmdBody.m_strGcid);
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("CDeleteCmd::ProcessCmd : delete source fail");
		return nRet;
	}

	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "delete source %s", CToolKit::BiToHex(m_CmdBody.m_strGcid.c_str(), m_CmdBody.m_strGcid.length()).c_str());
	return 0;
}

CDeleteCmdRes::CDeleteCmdRes(CDeleteCmd &cmdReq, int nResult)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = DELETERESCMD;
	if (nResult != 0)
	{
		m_CmdBody.m_cResult = -1;
	}
	else
	{
		m_CmdBody.m_cResult = 0;
	}
}

int CDeleteCmdRes::ProcessCmd()
{
	return 0;
}
