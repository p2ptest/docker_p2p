/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: mastersrv.h
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
#ifndef __MASTERSRV_H__
#define __MASTERSRV_H__

#include "myserver.h"
#include "mastersrvconf.h"
#include "maintainudpthread.h"

class CMasterSrv: public CMyserver
{
public:
	CMasterSrv();
	virtual ~CMasterSrv(){}

	int InitServer();
	int StartServer();
	CMasterSrvConf& GetConf()
	{
		return m_conf;
	}

private:
	CMasterSrvConf m_conf;
	CThreadGroup *m_pShortListenThreadGrp;
	CThreadGroup *m_pShortConnectedProcessThreadGrp;
	CThreadGroup *m_pOnTimeThreadGrp;
	CThreadGroup *m_pTimerThreadGrp;
	CThreadGroup *m_pDbBackupThreadGrp;
	CMaintainUdpThread* m_pMaintainUdpThread;
};

#endif
