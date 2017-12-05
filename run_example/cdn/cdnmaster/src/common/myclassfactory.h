/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: myclassfactory.h
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2011-11-24
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2011-11-24			1.0			
* =======================================================================
*/
#ifndef __MYCLASSFACTORY_H__
#define __MYCLASSFACTORY_H__

#include "classfactory.h"

class CMyClassFactory
	:public CClassFactory
{
public:
	CMyClassFactory(){}
	virtual ~CMyClassFactory(){}

	virtual CWorkThread* GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void* arg = NULL);

	virtual CSocketTask* GenSocketTask(CNetProcessThread* pWorkThread, CTcpSocket* pTcpSocket, int nTaskType, CTaskArg* pTaskArg = NULL);
};

#endif
