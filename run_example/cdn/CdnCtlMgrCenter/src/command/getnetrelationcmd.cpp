/*
 * =====================================================================================
 *
 *       Filename:  getparentscdncmd2.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2011-10-17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wangfangzhu, wangfangzhu@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "getnetrelationcmd.h"

int CGetNetReLationCmd::ProcessCmd()
{
	m_outBody.m_vecParentCluster.clear();
	m_outBody.m_vecSonCluster.clear();
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	int nRet = 0;
	
	if(m_inBody.m_nIsNatIP )
	{
		SetIp(m_inBody.m_nIp);
	}
	in_addr iaAddr;
	iaAddr.s_addr = htonl(m_nIp);
	do
	{
		nRet = pMgr->GetClusterId(m_nIp, m_outBody.m_nClusterId);
		if(nRet != 0)
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) get clusterId fail......", inet_ntoa(iaAddr));
			break;
		}

		nRet = pMgr->GetHostId(m_nIp, m_outBody.m_nHostId);
		if(nRet != 0)
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) get hostId fail......", inet_ntoa(iaAddr));
			break;
		}

		nRet = pMgr->GetIsp(m_outBody.m_nClusterId ,m_outBody.m_tSelfCluster.m_nIsp);
		if(nRet != 0)
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) GetIsp fail", inet_ntoa(iaAddr));
			break;
		}

		nRet = pMgr->CheckIfHotCluster(m_outBody.m_nClusterId, m_outBody.m_tSelfCluster.m_nIsHot, m_outBody.m_tSelfCluster.m_nHash);
		if(nRet != 0)
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) CheckIfHotCluster fail", inet_ntoa(iaAddr));
			break;
		}

		m_outBody.m_tSelfCluster.m_nClusterId = m_outBody.m_nClusterId;
		m_outBody.m_tSelfCluster.m_nParentAttribute = 1;
	
		vector < TCdnSrvInfo > tmpVecCdnInfo;
		nRet = pMgr->GetCdnSrvInfo(m_nIp, tmpVecCdnInfo);
		if(nRet != 0)
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) GetCdnSrvInfo fail", inet_ntoa(iaAddr));
			break;
		}
		for(unsigned int i = 0; i < tmpVecCdnInfo.size(); i++)
		{
			TCdnHostInfo tmpCdnHostInfo;
			tmpCdnHostInfo.m_nHostId = tmpVecCdnInfo[i].m_nHostId;
			tmpCdnHostInfo.m_nState = tmpVecCdnInfo[i].m_nState;
			tmpCdnHostInfo.m_nIp = tmpVecCdnInfo[i].m_nPublicIp;
			m_outBody.m_tSelfCluster.m_vecHostInCLuster.push_back(tmpCdnHostInfo);
		}

		nRet = pMgr->GetNetRelation(m_nIp, m_outBody.m_vecParentCluster, m_outBody.m_vecSonCluster, m_outBody.m_nLay);
		if (nRet == 0)
		{
			m_outBody.m_nResult = 0; 
		}
		else
		{
			m_outBody.m_nResult = -1;
			WriteRunInfo::WriteError("CGetNetReLationCmd::ProcessCmd : Public(%s) GetNetRelation fail", inet_ntoa(iaAddr));
			break;
		}
	}while(0);
	
	if(m_outBody.m_nResult == -1)
	{
		m_outBody.ClearSelf();
	}
	return 0;
}


