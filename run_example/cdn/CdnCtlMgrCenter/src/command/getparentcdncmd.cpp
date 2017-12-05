/*
 * =====================================================================================
 *
 *       Filename:  getparentcdncmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-2
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "getparentcdncmd.h"

int CGetParentCdnCmd::ProcessCmd()
{
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	int nRet = 0;
	nRet = pMgr->GetParentCdn(m_nIp, m_outBody.m_vecCdnParent);
	
	if (nRet == 0)
	{
		m_outBody.m_nResult = 0; 
	}
	else
	{
		m_outBody.m_nResult = -1;
		m_outBody.m_vecCdnParent.clear();
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		WriteRunInfo::WriteError("CGetParentCdnCmd::ProcessCmd : Public(%s) is UnKnow", inet_ntoa(iaAddr));
	}
	return 0;
}


