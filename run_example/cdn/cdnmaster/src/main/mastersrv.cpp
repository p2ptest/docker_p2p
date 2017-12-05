/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: livemediasrv.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2011-10-18			1.0			
* =======================================================================
*/
#include "myclassfactory.h"
#include "mastersrv.h"
#include "myshortconnectprocessthread.h"
#include "ontimethread.h"
#include "mastersrvdef.h"
#include "securitymgr.h"
#include "dbbackmgr.h"
#include "sourcemanager.h"
#include "taskmanager.h"
#include "mymultilistenthread.h"

CMasterSrv::CMasterSrv()
{
	m_pShortListenThreadGrp = NULL;
	m_pShortConnectedProcessThreadGrp = NULL;
	m_pOnTimeThreadGrp = NULL;
	m_pTimerThreadGrp = NULL;
	m_pDbBackupThreadGrp = NULL;
	m_pMaintainUdpThread = NULL;
}

int CMasterSrv::InitServer()
{
	int nRet = CMyserver::InitServer();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError ("Init server fail");
		return -1;
	}

	nRet = m_conf.Init();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError ("Load conf fail");
		return -1;
	}

	m_pClassFactory = new CMyClassFactory();
	if (NULL == m_pClassFactory)
	{
		WriteRunInfo::WriteError ("create classfactory fail");
		return -1;
	}

	CDbBackMgr::Create(m_conf.GetConfItem().tMasterDb, m_conf.GetConfItem().tMasterDbBak);
	nRet = CDbBackMgr::GetInstance()->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("init dbbackmgr fail");
		return -1;
	}

	CSecurityMgr::Create();
	nRet = CSecurityMgr::GetInstance()->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("init securitymgr fail");
		return -1;
	}
	
	CSourceManager::Create();
	nRet = CSourceManager::GetInstance()->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("init source manager fail");
		return -1;
	}

	CTaskManager::Create();
	nRet = CTaskManager::GetInstance()->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("init task manager fail");
		return -1;
	}

	return 0;
}

int CMasterSrv::StartServer()
{
	int nRet = 0;
	WriteRunInfo::WriteLog("==========================Begin start server====================================");

	m_pTimerThreadGrp = new CThreadGroup(1, TIMERENTITY);
	nRet = m_pTimerThreadGrp->Init();
	if (0 != nRet)
	{
		WriteRunInfo::WriteError ("Init timer thread fail");
		return -1;
	}
	WriteRunInfo::WriteLog ("Init timer thread succeed");

	int nShortConnectProcessThreadNum = m_conf.GetConfItem().nShortConnectProcessThreadNum;
	m_pShortConnectedProcessThreadGrp = new CThreadGroup(nShortConnectProcessThreadNum, RECVANDSENDMSGENTITY);
	nRet = m_pShortConnectedProcessThreadGrp->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("Init short connect process threads fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init short connect process threads succeed");

	m_pOnTimeThreadGrp = new CThreadGroup(1, ONTIMEENTITY);
	nRet = m_pOnTimeThreadGrp->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("Init ontime threads fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init ontime threads succeed");

	m_pDbBackupThreadGrp = new CThreadGroup(1, DBBACKUPENTITY);
	nRet = m_pDbBackupThreadGrp->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("Init dbbackup threads fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init dbbackup threads succeed");

	m_pMaintainUdpThread = new CMaintainUdpThread(m_baseConf.GetConfigItem().sMaintainIp, m_baseConf.GetConfigItem().nMaintainPort);
	nRet = m_pMaintainUdpThread->Start();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("Init maintain udp thread fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init maintain udp thread succeed");

	string strHost = m_baseConf.GetConfigItem().sServerAddr;
	unsigned short nTcpPort = m_baseConf.GetConfigItem().nTcpPort;
	unsigned short nHttpPort = m_conf.GetConfItem().nHttpPort;

	CMyMultiListenThread::AddListen(strHost, nTcpPort, LT_TCP);
	CMyMultiListenThread::AddListen(strHost, nHttpPort, LT_HTTP);

	m_pShortListenThreadGrp = new CThreadGroup(1, ENTITYTYPE_CONNECT_LISTEN);
	nRet = m_pShortListenThreadGrp->Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteLog("Init listen thread fail");
		return -1;
	}
	WriteRunInfo::WriteLog("Init listen thread succeed");

	WriteRunInfo::WriteLog("MasterServer start succeed ...");
	return 0;
}
