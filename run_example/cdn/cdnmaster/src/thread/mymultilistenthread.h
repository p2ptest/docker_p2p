
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#ifndef _MY_MULTI_LISTEN_THREAD_H_
#define _MY_MULTI_LISTEN_THREAD_H_

#include "netprocessthread.h"
#include "mymultilistentask.h"

struct TListenInfo
{
	string m_strHost;
	unsigned short m_nPort;
	EListenType m_nListenType;

	TListenBlock ListenBlock;

	TListenInfo(const string& strHost, unsigned short nPort, EListenType nListenType) 
		: m_strHost(strHost), m_nPort(nPort), m_nListenType(nListenType)
	{
	}
};

class CMyMultiListenThread : public CNetProcessThread
{
public:
	CMyMultiListenThread(int iEntityType, int iEntityId)
		: CNetProcessThread(iEntityType, iEntityId)
	{
	}

	~CMyMultiListenThread()
	{
	}
	int Init();
	TListenBlock *GetListenBlock(int nIndex)
	{
		if ((unsigned int) nIndex >= m_vecListenInfo.size())
		{
			return NULL;
		}
		return &(m_vecListenInfo[nIndex].ListenBlock);
	}

public:
	static int AddListen(const string& strHost, unsigned short nPort, EListenType nListenType);

private:
	static vector<TListenInfo> m_vecListenInfo;
};

#endif
