/*
 * =====================================================================================
 *
 *       Filename: storageinfoclassfactory.h
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

#ifndef __MYCLASSFACTORY_H_97F2A350_1265_11E0_9929_0015C55D3D75_
#define __MYCLASSFACTORY_H_97F2A350_1265_11E0_9929_0015C55D3D75_

#include "classfactory.h"
#include "cdnctlmgrbasecmd.h"

class CMyClassFactory
	: public CClassFactory
{
public:
	CMyClassFactory();
	~CMyClassFactory();
public:
	virtual CWorkThread* GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void* arg = NULL);
	virtual CSocketTask* GenSocketTask(CNetProcessThread* pWorkThread, CTcpSocket* pTcpSocket, int nTaskType, CTaskArg* pTaskArg = NULL);
	virtual CBaseMaintainCmd* GenMaintainCmd(int nMsgType, int nBufLen, char *pBuf);
	CCommand* GenBaseCommand(char* pBuf, int nLen, int nIp = 0);
};

#endif

