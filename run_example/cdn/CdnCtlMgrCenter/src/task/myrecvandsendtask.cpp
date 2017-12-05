/*
 * =====================================================================================
 *
 *       Filename:  myrecvandsendtask.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-02
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "myrecvandsendtask.h"
#include "cdnctlmgrserver.h"
#include "cdnctlmgrdef.h"
#include "getparentcdncmd.h"
#include "myclassfactory.h"

CMyRecvAndSendTask::CMyRecvAndSendTask(CTcpSocket* pSocket, CNetProcessThread* pWorkThread, int nListType/*= 1*/ )
	: CRecvAndSendTask(pSocket, pWorkThread, nListType)
	, m_uReceiveSize(0)
	, m_uReceiveLeft(TCdnCtlMgrCmdHead::HEAD_LEN)	
	, m_state(ReadPacketHead)
{
}

CMyRecvAndSendTask::~CMyRecvAndSendTask()
{
}

int CMyRecvAndSendTask::DestroySelf()
{
	delete this;
	return 0;
}

int CMyRecvAndSendTask::Init()
{
	AddToEpoll();
	return 0;
}

int CMyRecvAndSendTask::RecvProcess()
{
	int iReced;

	iReced = recv(GetSocket(), m_szReceiveBuffer + m_uReceiveSize, m_uReceiveLeft, MSG_DONTWAIT);

	if (0 == iReced)
	{
		WriteRunInfo::WriteTrace("CMyRecvAndSendTask::RecvProcess : recv client close , close the socket");
		return -1;
	}
	
	if (iReced < 0)
	{
		if (errno != EINTR && errno != EAGAIN)
		{
			WriteRunInfo::WriteLog("CMyRecvAndSendTask::RecvProcess : recv client error(%d) , close the socket", errno);
			return -1;
		}
		return 0;
	}
	
	m_uReceiveSize += iReced;
	m_uReceiveLeft -= iReced;

	switch(m_state) 
	{
		case ReadPacketHead:
			{
				if(m_uReceiveSize >= TCdnCtlMgrCmdHead::HEAD_LEN)
				{
					m_state = ReadPacket;
					int nBodyLen;
					memcpy(&nBodyLen, m_szReceiveBuffer + TCdnCtlMgrCmdHead::BODYLEN_OFFSET, sizeof(nBodyLen));
					nBodyLen = ntohl(nBodyLen);
					if (nBodyLen + TCdnCtlMgrCmdHead::CMDID_OFFSET > BUFFLEN)
					{
						WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : command body is too long");
						return -1;
					}
					m_uReceiveLeft = nBodyLen + TCdnCtlMgrCmdHead::CMDID_OFFSET - m_uReceiveSize;
					if (m_uReceiveLeft <= 0)				//add by yinhaichen 2011-04-13
					{
						int nRet = ProcessPacket();
						if (-1 == nRet)
						{
							WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : recv invalid packet from client!!");
							return -1;
						}
						m_state = ReadPacketHead;
						m_uReceiveSize = 0;
						m_uReceiveLeft = TCdnCtlMgrCmdHead::HEAD_LEN;
					}
					
				}
				break;
			}
		case ReadPacket:
			{
				if( m_uReceiveLeft <= 0)
				{
					int nRet = ProcessPacket();
					if (-1 == nRet)
					{
						WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : recv invalid packet from client!!");
						return -1;
					}
					m_state = ReadPacketHead;
					m_uReceiveSize = 0;
					m_uReceiveLeft = TCdnCtlMgrCmdHead::HEAD_LEN;
				}
				break;
			}		
		default:
			{
				WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : wrong state!!");
				return -1;
				break;
			}
	}

	return 0;
}

int CMyRecvAndSendTask::SendProcess()
{
	int retValue = 0;
	if (m_pCurSendMsg != NULL)
	{
		retValue = SendData();
	}
	else
	{
		m_pCurSendMsg = GetMsgFromSendList();
		if (m_pCurSendMsg != NULL)
		{
			retValue = SendData(); 
		} 
		else
		{
			int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN;
			CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
			CEpoll *pEoll = pNetThread->GetEpoll();
			pEoll->ModifyFromEpoll(this, tmpEvent);
			retValue = 1;
		}
	}
	if(1 == retValue)
		DestroySelf();
	return retValue;
}

int CMyRecvAndSendTask::ProcessPacket()
{
	if (!m_pThread)
	{
		return -1;
	}

	CCdnCtlMgrServer* pServer = (CCdnCtlMgrServer*)CMyserver::GetInstance();
	CMyClassFactory* pClassFactory = (CMyClassFactory*)(pServer->GetClassFactory());
	int nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
	CCommand* pCmd = pClassFactory->GenBaseCommand((char*)m_szReceiveBuffer, (int)m_uReceiveSize, nClientIp);
	if (NULL == pCmd)	
	{
		return -1;
	}

	int nResult = 0;
	int nRespPackSize = 0;
	int nRet = 0;
	nRet = pCmd->Process(nResult);
	if (nRet == 0)
	{
		char* pRespPackBuf = pCmd->GetResBuf(nRespPackSize);
		pCmd->DisAttachBuf();
		if (NULL != pRespPackBuf && 0 != nRespPackSize)
		{
			PutMsgToSendList(pRespPackBuf, nRespPackSize);
		}
	}
	delete pCmd;
	return nRet;
}

int CMyRecvAndSendTask::AddToEpoll()
{
	int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN;
	if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
	{
		tmpEvent = tmpEvent|MYPOLLOUT;
	}

	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll* pEoll = pNetThread->GetEpoll();
	pEoll->AddToEpoll(this, tmpEvent);
	return 0;
}

int CMyRecvAndSendTask::PutMsgToSendList(const char* pBuf, int iLen)
{
	return SendToList((char *)pBuf, iLen);
}

int CMyRecvAndSendTask::SendToList(char* pBuf, int nLen)
{
	if (NULL == pBuf || 0 == nLen)
	{
		return -1;
	}
	CSendMsg *pSendMsg = new CSendMsg(nLen, pBuf);

	m_sendList.push_back(pSendMsg);
	int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN| MYPOLLOUT;
	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->ModifyFromEpoll(this, tmpEvent);
	return 0;
}


