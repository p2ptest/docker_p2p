/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: msgprocesstask.h
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
#ifndef __MSGPROCESSTASK_H__
#define __MSGPROCESSTASK_H__

#include "recvandsendtask.h"
#include "netprocessthread.h"

class CMsgProcessTask: public CRecvAndSendTask
{
public:
	CMsgProcessTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread);
	virtual ~CMsgProcessTask();
	virtual int Init();
	virtual int DestroySelf();
	virtual int PutMsgToSendList(const char *pBuf, int nLen);
protected:
	int  AddToEpoll();
	virtual int RecvProcess();
	virtual int SendProcess();
	int ProcessPacket();
	int CheckValidIp();
private:
	enum State
	{
		ReadPacketHead = 0,
		ReadPacket = 1
	};
	State m_state;
	char *m_recvBuff;
	int m_nReceiveSize;
	int m_nReceiveLeft;
};

#endif
