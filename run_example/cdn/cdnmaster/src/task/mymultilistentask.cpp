
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#include "mastersrv.h"
#include "mastersrvconf.h"
#include "mastersrvdef.h"

#include "mymultilistentask.h"
#include "mymultilistenthread.h"
#include "workthread.h"
#include "myepoll.h"
#include "netprocessthread.h"
#include "myserver.h"
#include "recvtask.h"
#include "commmsg.h"
#include "recvandsendtask.h"

CMyMultiListenTask::CMyMultiListenTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread)
	: CSocketTask(pSocket,  pWorkThread)
{
	m_iTaskType = TASKTYPE_CONNECT_LISTEN;
	m_nTaskTimeOut = -1;
}

CMyMultiListenTask::~CMyMultiListenTask()
{
}

int CMyMultiListenTask::Init()
{
	AddToEpoll();
	return 0;
}

int CMyMultiListenTask::Process(CMsg *pMsg)
{
	return 0;
}

int CMyMultiListenTask::HandleProcess(int events)
{
	CMyMultiListenThread *pThread = (CMyMultiListenThread*)m_pThread;
	pThread->GetListenBlock(m_nListenType)->mutex.Lock();
	if (events & (MYPOLLIN | MYPOLLRDNORM))
	{
		struct sockaddr_in tmpAddr;
		memset(&tmpAddr, 0, sizeof(sockaddr_in));
		int iSocketSize = sizeof(sockaddr_in);
		while (1)
		{
			int iSocket = accept(m_pTcpSocket->GetSocket(), (struct sockaddr *)&tmpAddr, (socklen_t*)&iSocketSize);
			if (iSocket > 0)
			{
				WRITE_TRACE("Recv a client connect request");
				CMyserver::GetInstance()->GetCommonStat().AddNormalStat(RECVPKGNUMSTAT);
				if (CTcpSocket::GetNum() > CMyserver::GetInstance()->GetBaseConf().GetConfigItem().nMaxFdSize) //如果太忙了， 不再接入
				{
					WriteRunInfo::WriteInfoRun("The fdsize is %d, greater than %d, close the connect", CTcpSocket::GetNum(),
						CMyserver::GetInstance()->GetBaseConf().GetConfigItem().nMaxFdSize);
					close(iSocket);
					continue;
				}
				CTimeStat timestat;
				timestat.Begin(); //打印开始的时间
				CRecvMsgBody *pBody = new CRecvMsgBody(iSocket, tmpAddr);
				pBody->SetTimeStat(timestat);
				if (m_nListenType == LT_TCP)
				{
					m_pThread->SendMsg(ENTITYTYPE_RECV_AND_SEND, RANDOMENTITYID, 0, MSGTYPE_RECV_CONNECT, pBody, 0);
				}
				else
				{
					m_pThread->SendMsg(ENTITYTYPE_RECV_AND_SEND, RANDOMENTITYID, 0, MSGTYPE_RECV_HTTP_CONNECT, pBody, 0);
				}
			}
			else  //句柄accept完毕
			{
				break;
			}
		}
	}
	else
	{
		WriteRunInfo::WriteRunLog("Receive invalid event %d", events);
	}
	pThread->GetListenBlock(m_nListenType)->mutex.UnLock();
	return 0;
}

int CMyMultiListenTask::PutMsgToSendList(const char *pBuf, int nLen)
{
	return 0;
}

int CMyMultiListenTask::DestroySelf()
{
	delete this;
	return 0;
}

int CMyMultiListenTask::AddToEpoll()
{
	int tmpEvent = MYPOLLIN | MYPOLLRDNORM;
	CNetProcessThread* pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->AddToEpoll(this, tmpEvent);
	return 0;
}

