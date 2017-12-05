/*
 * =====================================================================================
 *
 *       Filename:  myrecvandsendtask.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef __MYRECVANDSENDTASK_H_B0567B26_1263_11E0_8329_0015C55D3D75_
#define __MYRECVANDSENDTASK_H_B0567B26_1263_11E0_8329_0015C55D3D75_

#include "recvandsendtask.h"
#include "netprocessthread.h"
#include "cdnctlmgrserver.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

#define BUFFLEN 4096

class CMyRecvAndSendTask
	: public CRecvAndSendTask
{
public:
	CMyRecvAndSendTask(CTcpSocket* pSocket, CNetProcessThread * pWorkThread, int nListType = 0);
	virtual ~CMyRecvAndSendTask();
	virtual int DestroySelf();
	virtual int Init();
	virtual int PutMsgToSendList(const char* pBuf, int nLen);

protected:
	virtual int RecvProcess();
	virtual int SendProcess();
	virtual int ProcessPacket();

private:
	int AddToEpoll();
	int SendToList(char *pBuf, int nLen);

private:
	int m_uReceiveSize;
	int m_uReceiveLeft;	

	enum State
	{
		ReadPacketHead = 0,
		ReadPacket = 1
	};
		
	State m_state;
	char  m_szReceiveBuffer[BUFFLEN];

};


#endif


