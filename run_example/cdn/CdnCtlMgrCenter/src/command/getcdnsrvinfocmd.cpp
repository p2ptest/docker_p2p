/*
 * =====================================================================================
 *
 *       Filename:  getcdnsrvinfocmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-03-18
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  yinhaichen, yinhaichen@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "getcdnsrvinfocmd.h"

int CGetCdnSrvInfoCmd::ProcessCmd()
{
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	int nRet = 0;
    char nLimitMode = -1;
    unsigned int nMaxSpeed = -1;
    unsigned int nMultipleSpeed = -1;
	 vector<TCdnSrvInfo> vecCdnInfo;
	nRet = pMgr->GetCdnSrvInfo(m_nIp, vecCdnInfo);

	m_outBody.m_vecCdnSrvInfo.clear();
	int nSrvNum = (int)vecCdnInfo.size();
	for (int nA = 0; nA < nSrvNum; ++ nA)
	{
		TCdnStatus CdnStatus;
		CdnStatus.m_nState = vecCdnInfo[nA].m_nState;
		CdnStatus.m_nPublicIp = vecCdnInfo[nA].m_nPublicIp;
		CdnStatus.m_nPrivateIp = vecCdnInfo[nA].m_nPrivateIp;
		m_outBody.m_vecCdnSrvInfo.push_back(CdnStatus);
	}
	
	if (nRet != 0)
	{
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		WriteRunInfo::WriteError("GetCdnSrvInfo::ProcessCmd : Public(%s) is UnKnow", inet_ntoa(iaAddr));
		m_outBody.m_nResult = -1;
		m_outBody.m_vecCdnSrvInfo.clear();
		return 0;
	}

	nRet = pMgr->GetLimitSpeed((unsigned)m_nIp, nLimitMode, m_outBody.m_limitSpeed, nMaxSpeed, nMultipleSpeed);

	if (nRet != 0)
	{
		m_outBody.m_nResult = -1;
		m_outBody.m_vecCdnSrvInfo.clear();
		return 0;
	}
	else
	{
		m_outBody.m_nResult = 0; 
	}
	return 0;
}


