/*
 * =====================================================================================
 *
 *       Filename:  infoserver.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-2
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "cdnctlmgrserver.h"
#include "myclassfactory.h"
#include "cdnclustermgr.h"

CCdnCtlMgrServer::CCdnCtlMgrServer()
{
}

CCdnCtlMgrServer::~CCdnCtlMgrServer()
{
}

int CCdnCtlMgrServer::InitServer()
{
	int nRet = CMyserver::InitServer();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError("Init server fail!");
		return -1;
	}

	nRet = m_conf.Init();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError("Load conf fail");
		return -1;
	}

	CCdnClusterMgr::CreateInstance();
	CCdnClusterMgr* pMgr = CCdnClusterMgr::GetInstance();
	nRet = pMgr->InitClusterMgr();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError("Load DB Config Item fail");
		return -1;
	}

	m_pClassFactory = new CMyClassFactory();
	if (NULL == m_pClassFactory)
	{
		return -1;
	}
	return 0;
}


int CCdnCtlMgrServer::StartServer()
{
	int nRet = 0;
	WriteRunInfo::WriteLog("==========================Begin start server====================================");

	{
		m_pTimerThreadGrp = new CThreadGroup(1, TIMERENTITY);
		nRet = m_pTimerThreadGrp->Init();
		if (0 != nRet)
		{
			WriteRunInfo::WriteError ("Init timer thread fail");
			return -1;
		}
		WriteRunInfo::WriteLog ("Init timer thread succeed");
	}

	{
		int nPort = m_baseConf.GetConfigItem().nTcpPort;
		string strHost = m_baseConf.GetConfigItem().sServerAddr;
		string strTmpData = string((char*)(&nPort), 4);
		strTmpData.append(strHost);

		m_pShortConnectListenThreadGrp = new CThreadGroup(1, LISTENENTITY, strTmpData);
		nRet = m_pShortConnectListenThreadGrp->Init();
		if (0 != nRet)
		{
			WriteRunInfo::WriteError ("Init short connect listen thread fail");
			return -1;
		}
		WriteRunInfo::WriteLog("Init short connect listen thread succeed");
	}

	{
		int nTcpThreadNum = m_baseConf.GetConfigItem().nTcpThreadNum;
		m_pShortConnectProcThreadGrp = new CThreadGroup(nTcpThreadNum, RECVANDSENDMSGENTITY);
		nRet = m_pShortConnectProcThreadGrp->Init();
		if (0 != nRet)
		{
			WriteRunInfo::WriteError ("Init short connect porcess thread fail");
			return -1;
		}
		WriteRunInfo::WriteLog ("Init short connect process thread succeed");
	}

	{
		m_pMaintainUdpThread = new CMaintainUdpThread(m_baseConf.GetConfigItem().sMaintainIp, m_baseConf.GetConfigItem().nMaintainPort);
		nRet = m_pMaintainUdpThread->Start();
		if (0 != nRet)
		{
			WriteRunInfo::WriteError("Init maintain udp thread fail");
			return -1;
		}
		WriteRunInfo::WriteLog ("Init maintain udp thread succeed");
	}

	WriteRunInfo::WriteLog("infoserver start succeed ...");
	return nRet;
}


