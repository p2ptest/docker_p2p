/**
 *    @file       securitymgr.cpp
 *
 *    @brief      back database
 *    @details
 *
 *    @version    1.0
 *    @date       08/23/2013 07:27:55 PM
 *
 *    @author     wangtiefeng (), 
 */

#include "securitymgr.h"
#include "mastersrv.h"

const std::string VALID_IP_TABLE = "t_valid_ip";
//static member////////////////////////////
CSecurityMgr* CSecurityMgr::m_pInstance = NULL;

CSecurityMgr* CSecurityMgr::GetInstance()
{
	return m_pInstance;
}

int CSecurityMgr::Create()
{
	if (!m_pInstance)
	{
		m_pInstance = new CSecurityMgr;
	}
	return 0;
}

int CSecurityMgr::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	return 0;
}


//member//////////////////////////////////
int CSecurityMgr::Init()
{
	return UpdateStrategy();
}

int CSecurityMgr::UpdateStrategy()
{
	CMasterSrv *pSrv = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pSrv->GetConf().GetConfItem();
	CMysql mySql;
	char szSql[1024] = {0};
	std::map<int, TIpInfo> tValidIpMap;
	try
	{
		mySql.Init();
		mySql.Connect(conf.tMasterDb.strIp, conf.tMasterDb.strUser,  conf.tMasterDb.strPasswd);
		snprintf(szSql, sizeof(szSql), "select * from %s.%s;", conf.tMasterDb.strDbName.c_str(), VALID_IP_TABLE.c_str());
		mySql.Query(szSql);
		while (mySql.FetchRow())
		{
			const char *strBegIp = mySql.GetField("ipbeg");
			const char *strEndIp = mySql.GetField("ipend");
			TIpInfo tIpInfo;
			tIpInfo.nBegIp = atoi(strBegIp);
			tIpInfo.nEndIp = atoi(strEndIp);
			if (tValidIpMap.find(tIpInfo.nEndIp) != tValidIpMap.end())
			{
				WriteRunInfo::WriteError("load valid strategy fail: duplicate ip = %d", tIpInfo.nEndIp);
				return DB_ERROR;
			}
			tValidIpMap[tIpInfo.nEndIp] = tIpInfo;
		}
		m_rwValidIpLock.GetWriteLock();
		m_mapValidIp.swap(tValidIpMap);
		m_rwValidIpLock.ReleaseLock();
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("load valid strategy fail: db error : %s", e.GetErrMsg());
		return DB_ERROR;
	}
	return 0;
}

int CSecurityMgr::CheckIpValid(int nIp)
{
	int nRet = -1;
	m_rwValidIpLock.GetReadLock();
	std::map<int, TIpInfo>::iterator itrMap = m_mapValidIp.lower_bound(nIp);
	if (itrMap != m_mapValidIp.end())
	{
		if (nIp >= itrMap->second.nBegIp && nIp <= itrMap->second.nEndIp)
		{
			nRet = 0;
		}
	}
	m_rwValidIpLock.ReleaseLock();
	return nRet;
}






