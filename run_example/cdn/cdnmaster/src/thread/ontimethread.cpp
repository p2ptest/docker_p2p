/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: ontimethread.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2011-11-24
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2011-11-24			1.0			
* =======================================================================
*/
#include "ontimethread.h"
#include "mastersrv.h"
#include "sourcemanager.h"
#include "taskmanager.h"
#include "securitymgr.h"

COnTimeThread::COnTimeThread(int nEntityType, int nEntityId)
	: CWorkThread(nEntityType, nEntityId)
{

};

COnTimeThread::~COnTimeThread()
{

}

int COnTimeThread::Init()
{
	return 0;
}

int COnTimeThread::Run()
{
	CMasterSrv* pServer = (CMasterSrv*)CMyserver::GetInstance();
	TMasterSrvConfItem &conf = pServer->GetConf().GetConfItem();
	unsigned int nReloadInterval = (unsigned int )conf.nReloadInterval;
	unsigned int nLoadValidIpInterval = (unsigned int)conf.nLoadValidIpInterval;
	unsigned int nLoadPaidSourceInterval = (unsigned int)conf.nLoadPaidSourceInterval;
	unsigned int nLoadShortSourceInterval = (unsigned int)conf.nLoadShortSourceInterval;
	unsigned int nLoadFileSourceInterval = (unsigned int)conf.nLoadFileSourceInterval;

	int nPaidEnable = conf.nPaidEnable;
	int nFileEnable = conf.nFileEnable;
	int nShortEnable = conf.nShortEnable;

	unsigned int nCurTime = pServer->GetTimeVal().tv_sec;

	unsigned int nLastClearTime = nCurTime;
	unsigned int nLastReloadTime = nCurTime;
	unsigned int nLastLoadValidIpTime = nCurTime;
	unsigned int nLastLoadPaidSourceTime = nCurTime;
	unsigned int nLastLoadShortSourceTime = nCurTime;
	unsigned int nLastLoadFileSourceTime = nCurTime;

	if (nPaidEnable)
	{
		CSourceManager::GetInstance()->CheckPaidSource();
	}

	if (nFileEnable)
	{
		CSourceManager::GetInstance()->CheckFileSource();
	}

	if (nShortEnable)
	{
		CSourceManager::GetInstance()->CheckShortSource();
	}

	while (m_nRunFlag)
	{
		CMsg *pMsg = m_inputList.get();
		if (pMsg != NULL)
		{
			Process(pMsg);
			delete pMsg;
		}
		
		nCurTime = pServer->GetTimeVal().tv_sec;
		if (nCurTime - nLastClearTime >= (unsigned)UPDATE_INTERVAL)
		{
			CSourceManager::GetInstance()->ClearTimeoutIncSource();
			nLastClearTime = nCurTime;
		}

		if (nCurTime - nLastReloadTime >= nReloadInterval)
		{
			CSourceManager::GetInstance()->ChecktimeOutTaskMgr();
			nLastReloadTime = nCurTime;
		}

		if (nCurTime - nLastLoadValidIpTime >= nLoadValidIpInterval)
		{
			CSecurityMgr::GetInstance()->UpdateStrategy();
			nLastLoadValidIpTime = nCurTime;
		}

		if (nPaidEnable && nCurTime - nLastLoadPaidSourceTime >= nLoadPaidSourceInterval)
		{
			CSourceManager::GetInstance()->CheckPaidSource();
			nLastLoadPaidSourceTime = nCurTime;
		}

		if (nShortEnable && nCurTime - nLastLoadShortSourceTime >= nLoadShortSourceInterval)
		{
			CSourceManager::GetInstance()->CheckShortSource();
			nLastLoadShortSourceTime = nCurTime;
		}

		if (nFileEnable && nCurTime - nLastLoadFileSourceTime >= nLoadFileSourceInterval)
		{
			CSourceManager::GetInstance()->CheckFileSource();
			nLastLoadFileSourceTime = nCurTime;
		}
	
		usleep(100000);
	}

	return 0;
}

int COnTimeThread::PutMsgToInputList(CMsg * pMsg)
{
	m_inputList.put(pMsg);
	return 0;
}

int COnTimeThread::Process(CMsg *pMsg)
{
	//TMsg* pTMsg = pMsg->GetTMsg();
	return 0;
}

