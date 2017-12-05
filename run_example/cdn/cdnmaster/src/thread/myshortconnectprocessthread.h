/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: myshortconnectprocessthread.h
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
#ifndef __MYSHORTCONNECTPROCESSTHREAD_H__
#define __MYSHORTCONNECTPROCESSTHREAD_H__

#include "framecommon/framecommon.h"
#include "netprocessthread.h"
using namespace std;
using namespace MYFRAMECOMMON;

class CMyShortConnectProcessThread: public CNetProcessThread
{
public:
	CMyShortConnectProcessThread(int iEntityType, int iEntityId);
	virtual ~CMyShortConnectProcessThread();
protected:
private:
	virtual int Process(CMsg *pMsg);
};

#endif
