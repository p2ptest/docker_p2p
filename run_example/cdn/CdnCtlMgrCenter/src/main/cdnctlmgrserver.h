/*
 * =====================================================================================
 *
 *       Filename:  infoserver.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-2
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef __CDNCTLMGRSERVER_H_777E5CD0_1266_11E0_917A_0015C55D3D75_
#define __CDNCTLMGRSERVER_H_777E5CD0_1266_11E0_917A_0015C55D3D75_

#include "myserver.h"
#include "cdnctlmgrconf.h"
#include "maintainudpthread.h"

class CCdnCtlMgrServer
	: public CMyserver
{
public:
	CCdnCtlMgrServer();
	~CCdnCtlMgrServer();
	
	int InitServer();
	int StartServer();

	CCdnCtlMgrConf& GetConf()
	{
		return m_conf;
	}

private:
	CCdnCtlMgrConf m_conf;

	CThreadGroup* m_pTimerThreadGrp;
	CThreadGroup* m_pShortConnectListenThreadGrp;
	CThreadGroup* m_pShortConnectProcThreadGrp;

	CMaintainUdpThread* m_pMaintainUdpThread;
};

#endif

