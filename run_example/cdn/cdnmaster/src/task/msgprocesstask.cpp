/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: msgprocesstask.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-04-01			1.0			
* =======================================================================
*/
#include "msgprocesstask.h"
#include "mastersrvdef.h"
#include "mastersrv.h"
#include "securitymgr.h"

#include "mastersrvbasecmd.h"
#include "publishcmd.h"
#include "gettaskcmd.h"
#include "taskreportoldcmd.h"
#include "deletecmd.h"
#include "sourceincsyncmd.h"
#include "sourceincsyn4cachecmd.h"
#include "sourcereportcmd.h"
#include "sourcesyncmd.h"
#include "sourcesyn4cachecmd.h"
#include "taskreportcmd.h"
#include "taskreportoldcmd.h"
#include "statsyncallsrccmd.h"
#include "persistsourcesyncmd.h"


CMsgProcessTask::CMsgProcessTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread)
	: CRecvAndSendTask(pSocket, pWorkThread, 0)
{
	m_state = ReadPacketHead;
	m_nReceiveSize = 0;
	m_nReceiveLeft = TCmdHead::HEAD_LEN;
	m_recvBuff = new char[MAX_RECV_BUFF_LEN];
}

CMsgProcessTask::~CMsgProcessTask()
{
	delete [] m_recvBuff;
}

int CMsgProcessTask::Init()
{
	AddToEpoll();
	return 0;
}

int CMsgProcessTask::DestroySelf()
{
	delete this;
	return 0;
}

int CMsgProcessTask::AddToEpoll()
{
	int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN;                         
	if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
	{
		tmpEvent = tmpEvent|MYPOLLOUT;
	}

	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->AddToEpoll(this, tmpEvent);
	return 0;
}

int CMsgProcessTask::PutMsgToSendList(const char *pBuf, int nLen)
{        
	if (pBuf == NULL || nLen == 0)
	{       
		return -1;
	}               
	char *pTmpBuf = new char[nLen];
	memcpy(pTmpBuf, pBuf, nLen);
	CSendMsg *pSendMsg = new CSendMsg(nLen, pTmpBuf);
    
	m_sendList.push_back(pSendMsg);
	int tmpEvent = MYPOLLERR|MYPOLLHUP|MYPOLLIN|MYPOLLOUT;
	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->ModifyFromEpoll(this, tmpEvent);
	return 0;
}

int CMsgProcessTask::RecvProcess()
{
	int iReced;
	iReced = recv(GetSocket(), m_recvBuff + m_nReceiveSize, m_nReceiveLeft, MSG_DONTWAIT);

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

	m_nReceiveSize += iReced;
	m_nReceiveLeft -= iReced;

	switch(m_state)
	{
	case ReadPacketHead:
		{
			if (m_nReceiveSize >= TCmdHead::HEAD_LEN)
			{
				m_state = ReadPacket;
				int nBodyLen;
				memcpy(&nBodyLen, m_recvBuff + TCmdHead::BODYLEN_OFFSET, sizeof(nBodyLen));
				nBodyLen = ntohl(nBodyLen);
				if (nBodyLen + TCmdHead::CMDID_OFFSET > MAX_RECV_BUFF_LEN)
				{
					WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : command body is too long");
					return -1;
				}
				m_nReceiveLeft = nBodyLen + TCmdHead::CMDID_OFFSET - m_nReceiveSize;
				if (m_nReceiveLeft <= 0)
				{
					int nRet = ProcessPacket();
					if (-1 == nRet)
					{
						WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : recv invalid packet from client!!");
						return -1;
					}
					m_state = ReadPacketHead;
					m_nReceiveSize = 0;
					m_nReceiveLeft = TCmdHead::HEAD_LEN;
				}

			}
			break;
		}
	case ReadPacket:
		{
			if ( m_nReceiveLeft <= 0)
			{
				int nRet = ProcessPacket();
				if (-1 == nRet)
				{
					WriteRunInfo::WriteError("CMyRecvAndSendTask::RecvProcess : recv invalid packet from client!!");
					return -1;
				}
				m_state = ReadPacketHead;
				m_nReceiveSize = 0;
				m_nReceiveLeft = TCmdHead::HEAD_LEN;
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

int CMsgProcessTask::SendProcess()
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
	if (1 == retValue)
		DestroySelf();
	return retValue;
}

int CMsgProcessTask::ProcessPacket()
{
	int nRet = 0;
	int nCmdType = TCmdHead::GetCmdId(m_recvBuff, m_nReceiveSize);
	switch (nCmdType)
	{
	case PUBLISHCMD:
		{
			if (CheckValidIp() != 0)
			{
				return -1;
			}
			CPublishCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CPublishCmdRes resCmd(reqCmd, nRet);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case DELETECMD:
		{
			if (CheckValidIp() != 0)
			{
				return -1;
			}
			CDeleteCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CDeleteCmdRes resCmd(reqCmd, nRet);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case SOURCESYNCMD:
		{
			CSourceSynCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CSourceSynCmdRes resCmd(reqCmd, reqCmd.GetCmdBody().nType);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case SOURCEINCSYNCMD:
		{
			CSourceIncSynCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CSourceIncSynCmdRes resCmd(reqCmd);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case SOURCESYN4CACHECMD:
		{
			CSourceSyn4CacheCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				unsigned int nClientIp = reqCmd.GetIp();
				if (nClientIp == 0)
				{
					nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
				}
				CSourceSyn4CacheCmdRes resCmd(reqCmd, reqCmd.GetCmdBody().nType, nClientIp);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case SOURCEINCSYN4CACHECMD:
		{
			CSourceIncSyn4CacheCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				unsigned int nClientIp = reqCmd.GetIp();
				if (nClientIp == 0)
				{
					nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
				}
				CSourceIncSyn4CacheCmdRes resCmd(reqCmd, nClientIp);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case SOURCEREPORTCMD:
		{
			if (CheckValidIp() != 0)
			{
				return -1;
			}
			CSourceReportCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CSourceReportCmdRes resCmd(reqCmd, nRet);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case GETTASKCMD:
		{
			CGetTaskCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				unsigned int nClientIp = reqCmd.GetIp();
				if (nClientIp == 0)
				{
					nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
				}
				CGetTaskCmdRes resCmd(reqCmd, nClientIp);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case TASKREPORTCMD:
		{
			unsigned int nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
			CTaskReportCmd reqCmd(m_recvBuff, m_nReceiveSize);
			reqCmd.SetIp(nClientIp);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CTaskReportCmdRes resCmd(reqCmd, nRet);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case TASKREPORTOLDCMD:
		{
			unsigned int nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
			CTaskReportOldCmd reqCmd(m_recvBuff, m_nReceiveSize);
			reqCmd.SetIp(nClientIp);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				CTaskReportOldCmdRes resCmd(reqCmd, nRet);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case STATSRV_ENTIRESOURCE_SYNC_CMD:
		{
			CStatSourceEntireSyncCmd reqCmd(m_recvBuff, m_nReceiveSize);
			if ((nRet = reqCmd.Process()) != -1)
			{
				CStatSourceEntireSyncResCmd resCmd(reqCmd);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	case PERSISTSOURCESYNCMD:
		{
			CPersistSourceSynCmd reqCmd(m_recvBuff, m_nReceiveSize);
			nRet = reqCmd.Process();
			if (nRet != -1)
			{
				unsigned int nClientIp = reqCmd.GetIp();
				if (nClientIp == 0)
				{
					nClientIp = ntohl(m_pTcpSocket->GetAddr().sin_addr.s_addr);
				}
				CPersistSourceSynCmdRes resCmd(reqCmd, nClientIp);
				resCmd.Process();
				PutMsgToSendList(resCmd.GetBuf(), resCmd.GetBufLen());
				reqCmd.DetachBuf();
			}
			else
			{
				reqCmd.DetachBuf();
				return -1;
			}
			break;
		}
	default:
		{
			WriteRunInfo::WriteError ("unknown cmd %d", nCmdType);
			break;
		}
	}
	return 0;
}

int CMsgProcessTask::CheckValidIp()
{
	struct in_addr ia;    
	ia.s_addr = m_pTcpSocket->GetAddr().sin_addr.s_addr;
	int nIp = ntohl(ia.s_addr);    
	char szAddr[INET_ADDRSTRLEN] = { 0 }; 
	inet_ntop(AF_INET, &ia, szAddr, INET_ADDRSTRLEN);
	if (CSecurityMgr::GetInstance()->CheckIpValid(nIp) != 0)
	{
		WriteRunInfo::WriteError("CMsgProcessTask::CheckValidIp:  %s", szAddr);
        return -1;        
    }
	return 0;             
}

