/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: ontimethread.h
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
#ifndef __ONTIMETHREAD_H__
#define __ONTIMETHREAD_H__

#include "framecommon/framecommon.h"
#include "workthread.h"
#include "asynqueue.h"

using namespace std;
using namespace MYFRAMECOMMON;

class COnTimeThread : public CWorkThread
{
public:
	COnTimeThread(int nEntityType, int nEntityId);
	virtual ~COnTimeThread();

	virtual int Init();
	virtual int Run();
	virtual int PutMsgToInputList(CMsg *pMsg);
protected:
	CAsynQueue  m_inputList;
private:
	virtual int Process(CMsg *pMsg);
};

#endif
