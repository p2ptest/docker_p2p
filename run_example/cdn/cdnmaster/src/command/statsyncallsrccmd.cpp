/**
 * ============================================================================
 * @file    statsyncallsrccmd.cpp
 *
 * @brief   
 * @details 
 *
 * @version 1.0  
 * @date    2013-03-23 06:07:09
 *
 * @author  shizhijie, shizhijie@baofeng.com
 * ============================================================================
 */

#include <algorithm>
#include "statsyncallsrccmd.h"
#include "mastersrv.h"
#include "sourcemanager.h"

CStatSourceEntireSyncResCmd::CStatSourceEntireSyncResCmd(CStatSourceEntireSyncCmd &cmdReq)
{
	m_CmdHead.m_nProtocolVer = cmdReq.GetCmdHead().m_nProtocolVer;
	m_CmdHead.m_nSequence = cmdReq.GetCmdHead().m_nSequence;
	m_CmdHead.m_nCmdId = STATSRV_ENTIRESOURCE_SYNC_RESCMD;
	m_CmdBody.m_chResult = 0;
}

int CStatSourceEntireSyncResCmd::ProcessCmd()
{
	CMasterSrv *pSrv = (CMasterSrv*)(CMyserver::GetInstance());
	unsigned long timeBegin = pSrv->GetTimeVal().tv_sec;
	CSourceManager::GetInstance()->GetGCidByType(EST_HotSource, m_CmdBody.m_vecHotSource);
	CSourceManager::GetInstance()->GetGCidByType(EST_ColdSource, m_CmdBody.m_vecColdSource);
	WriteRunInfo::WriteLog("GetGCidByType cost %d (s)", pSrv->GetTimeVal().tv_sec - timeBegin);
	return 0;
}
