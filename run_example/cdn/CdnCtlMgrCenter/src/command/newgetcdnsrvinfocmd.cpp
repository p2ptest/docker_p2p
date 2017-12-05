/*
 * =====================================================================================
 *
 *       Filename:  newgetcdnsrvinfocmd.cpp
 *
 *    Description:  
 *
 *        Version:  2.0
 *        Created:  2012-04-01
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  hezhipeng, hezhipeng@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "newgetcdnsrvinfocmd.h"

int CNewGetCdnSrvInfoCmd::ProcessCmd()
{
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	int nRet = 0;
	char nHash = -1;
	char nLimitMode = -1;
    unsigned int nMaxSpeed = -1;
    unsigned int nMultipleSpeed = -1;

	if(m_inBody.m_nIsNatIP )
	{
		SetIp(m_inBody.m_nIp);
	}

	in_addr iaAddr;
	iaAddr.s_addr = htonl(m_nIp);
	
	m_outBody.m_nResult = 0;
	
	do
	{
		vector<TCdnSrvInfo> vecCdnInfo;
		nRet = pMgr->GetCdnSrvInfo(m_nIp, vecCdnInfo);
		if (nRet != 0)
		{
			WriteRunInfo::WriteError("GetCdnSrvInfo::ProcessCmd : Public(%s) getCdnSrvInfo fail", inet_ntoa(iaAddr));
			m_outBody.m_nResult = -1;
			break;
		}
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
		
		nRet = pMgr->GetClusterId((unsigned)m_nIp, m_outBody.m_nClusterId);
		if(nRet)
		{
			WriteRunInfo::WriteError("GetClusterId::ProcessCmd : Public(%s) getClusterId fail", inet_ntoa(iaAddr));
			m_outBody.m_nResult = -1;
			break;
		}
	
		nRet = pMgr->CheckIfHotCluster(m_outBody.m_nClusterId, m_outBody.m_nIsHot, nHash);
		if(nRet)
		{
			WriteRunInfo::WriteError("CheckIfHotCluster::ProcessCmd : Public(%s) CheckIfHotCluster fail", inet_ntoa(iaAddr));
			m_outBody.m_nResult = -1;
			break;
		}
	
		nRet = pMgr->GetLimitSpeed((unsigned)m_nIp, nLimitMode, m_outBody.m_limitSpeed, nMaxSpeed, nMultipleSpeed);
		if (nRet != 0)
		{
			WriteRunInfo::WriteError("GetLimitSpeed::ProcessCmd : Public(%s) GetLimitSpeed fail", inet_ntoa(iaAddr));
			m_outBody.m_nResult = -1;
			break;
		}
	}while(0);
	if(m_outBody.m_nResult == -1)
	{
		m_outBody.ClearSelf();
	}
	return 0;
}


