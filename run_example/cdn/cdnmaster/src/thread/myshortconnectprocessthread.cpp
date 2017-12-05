/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: myshortconnectprocessthread.cpp
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
#include "myshortconnectprocessthread.h"
#include "mastersrvdef.h"

CMyShortConnectProcessThread::CMyShortConnectProcessThread(int iEntityType, int iEntityId)
	: CNetProcessThread(iEntityType, iEntityId)
{

}

CMyShortConnectProcessThread::~CMyShortConnectProcessThread()
{

}

int CMyShortConnectProcessThread::Process(CMsg *pMsg)
{
	TMsg* pTMsg = pMsg->GetTMsg();
	if (!pTMsg)
	{
		return 0;
	}

	if (pTMsg->srcEntityType == ENTITYTYPE_CONNECT_LISTEN)
	{
		if (pTMsg->msgType == MSGTYPE_RECV_CONNECT)
		{
			CRecvMsgBody* pBody = (CRecvMsgBody*)pMsg->GetMsgBody();
			CTcpSocket* pSocket = new CTcpSocket(pBody->GetSockAddr(), sizeof(sockaddr_in), pBody->GetSocket());
			CSocketTask* pTmpSocketTask = NULL;
			pTmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this, pSocket, MSGPROCESSTASK);
			if (pTmpSocketTask != NULL)
			{
				pTmpSocketTask->Init();
				pTmpSocketTask->SetTimeStat(pBody->GetTimeStat());
			}
			else
			{
				WriteRunInfo::WriteError ("CMyShortConnectProcessThread thread generate MSGPROCESSTASK fail!!");
			}
		}
		else if (pTMsg->msgType == MSGTYPE_RECV_HTTP_CONNECT)
		{
			CRecvMsgBody* pBody = (CRecvMsgBody*)pMsg->GetMsgBody();
			CTcpSocket* pSocket = new CTcpSocket(pBody->GetSockAddr(), sizeof(sockaddr_in), pBody->GetSocket());
			CSocketTask* pTmpSocketTask = NULL;
			pTmpSocketTask = CMyserver::GetInstance()->GetClassFactory()->GenSocketTask(this, pSocket, TASKTYPE_HTTP_RECV_AND_SEND);
			if (pTmpSocketTask != NULL)
			{
				pTmpSocketTask->Init();
				pTmpSocketTask->SetTimeStat(pBody->GetTimeStat());
			}
			else
			{
				WriteRunInfo::WriteError ("CMyShortConnectProcessThread thread generate TASKTYPE_HTTP_RECV_AND_SEND fail!!");
			}
		}
	}
	else
	{
		CNetProcessThread::Process(pMsg);
	}
	
	return 0;
}
