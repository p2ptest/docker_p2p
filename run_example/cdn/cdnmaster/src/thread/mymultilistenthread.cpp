
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#include "mastersrv.h"
#include "mastersrvconf.h"
#include "mastersrvdef.h"
#include "mymultilistenthread.h"
#include "mymultilistentask.h"

//静态变量
vector<TListenInfo> CMyMultiListenThread::m_vecListenInfo;

//静态函数: 初始化时调用，线程不安全
int CMyMultiListenThread::AddListen(const string& strHost, unsigned short nPort, EListenType nListenType)
{
	m_vecListenInfo.push_back(TListenInfo(strHost, nPort, nListenType));
	return 0;
}

int CMyMultiListenThread::Init()
{
	int nRet = CNetProcessThread::Init();
	if (nRet != 0)
	{
		return -1;
	}
	vector<TListenInfo>::iterator itrListen = m_vecListenInfo.begin();
	while (itrListen != m_vecListenInfo.end())
	{
		string strHost = itrListen->m_strHost;
		unsigned short nPort = itrListen->m_nPort;
		itrListen->ListenBlock.mutex.Lock();
		if (itrListen->ListenBlock.nSock == -1)
		{
			//生成一个监听句柄
			int nSocket;
			nRet = CMySocket::CreateSock(SOCK_STREAM, strHost, nPort, nSocket);
			if (nRet < 0)
			{
				WriteRunInfo::WriteLog("Create listen sock fail %d", nRet);
				itrListen->ListenBlock.mutex.UnLock();
				return -1;
			}
			itrListen->ListenBlock.nSock = nSocket;
		}
		itrListen->ListenBlock.mutex.UnLock();
		CTcpSocket *pTcpSocket = new CTcpSocket(strHost, nPort, itrListen->ListenBlock.nSock);
		if (pTcpSocket == NULL)
		{
			WriteRunInfo::WriteLog("New listen socket object fail");
			return -1;
		}
		//起监听端口
		CMyMultiListenTask* pListenTask = (CMyMultiListenTask*)CMyserver::GetInstance()->GetClassFactory()->
			GenSocketTask(this, pTcpSocket, TASKTYPE_CONNECT_LISTEN);
		if (pListenTask == NULL)
		{
			WriteRunInfo::WriteLog("Malloc memory fail or listen fail");
			return -1;
		}
		pListenTask->SetListType(itrListen->m_nListenType);
		nRet = pListenTask->Init();
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Init Listen Task Fail(%s:%d)", strHost.c_str(), nPort);
		}
		itrListen++;
	}
	return 0;
}

