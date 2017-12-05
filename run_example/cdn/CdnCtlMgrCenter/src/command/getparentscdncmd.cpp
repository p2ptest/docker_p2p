/*
 * =====================================================================================
 *
 *       Filename:  getparentscdncmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-03-29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  yinhaichen, yinhaichen@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "getparentscdncmd.h"

int CGetParentsCdnCmd::ProcessCmd()
{
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	int nRet = 0;
	nRet = pMgr->GetParentsCdn(m_nIp, m_outBody.m_vecParentCluster);
	if (nRet == 0)
	{
		m_outBody.m_nResult = 0; 
	}
	else
	{
		m_outBody.m_nResult = -1;
		m_outBody.m_vecParentCluster.clear();

		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		WriteRunInfo::WriteError("CGetParentsCdnCmd::ProcessCmd : Public(%s) is UnKnow", inet_ntoa(iaAddr));
	}
	return 0;
}


