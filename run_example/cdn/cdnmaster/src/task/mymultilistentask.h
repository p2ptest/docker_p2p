
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#ifndef _MY_MULTI_LISTEN_TASK_H_
#define _MY_MULTI_LISTEN_TASK_H_

#include "sockettask.h"
#include "framecommon/framecommon.h"
using namespace std;

enum EListenType
{
	LT_TCP = 0,
	LT_HTTP = 1
};

struct TListenBlock
{
	int nSock;
	CThreadMutex mutex;
	TListenBlock()
	{
		nSock = -1;
	}
};

class CMyMultiListenTask : public CSocketTask
{
public:
	CMyMultiListenTask(CTcpSocket *pSocket, CNetProcessThread* pWorkThread);
	~CMyMultiListenTask();
	void SetListType(EListenType nListenType)
	{
		m_nListenType = nListenType;
	}

	virtual int Init();
	int Process(CMsg *pMsg);
	int HandleProcess(int event);

protected:
	int DestroySelf();
	int PutMsgToSendList(const char *pBuf, int nLen);
	int AddToEpoll();

private:
	EListenType m_nListenType;
	//static CThreadMutex m_listenMutex;
};

#endif
