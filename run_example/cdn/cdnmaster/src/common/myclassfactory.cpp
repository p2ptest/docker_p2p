/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: myclassfactory.cpp
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
#include "myclassfactory.h"
#include "myshortconnectprocessthread.h"
#include "ontimethread.h"
#include "mymultilistenthread.h"
#include "mymultilistentask.h"
#include "myhttprecvandsendtask.h"
#include "msgprocesstask.h"
#include "mastersrvdef.h"
#include "dbbackthread.h"

CWorkThread * CMyClassFactory::GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void *arg)
{
	CWorkThread* pWorkThread = NULL;

	switch(nEntityType)
	{
	case ENTITYTYPE_CONNECT_LISTEN:
	{
		pWorkThread = new CMyMultiListenThread(nEntityType, nEntityId);
		break;
	}
	case ENTITYTYPE_RECV_AND_SEND:
	{
		pWorkThread = new CMyShortConnectProcessThread(nEntityType, nEntityId);
		break;
	}
	case ONTIMEENTITY:
	{
		pWorkThread = new COnTimeThread(nEntityType, nEntityId);
		break;
	}
	case DBBACKUPENTITY:
	{
		pWorkThread = new CDbBackThread(nEntityType, nEntityId);
		break;
	}
	default:
	{
		pWorkThread = CClassFactory::GenWorkThread(nEntityType, nEntityId, sAnnexData, arg);
		break;
	}
	}

	if (NULL == pWorkThread)
	{
		WriteRunInfo::WriteError ("Invalid thread entity %d", nEntityType);
	}

	return pWorkThread;
}

CSocketTask *CMyClassFactory::GenSocketTask(CNetProcessThread *pWorkThread, CTcpSocket *pTcpSocket, int iTaskType, CTaskArg * pTaskArg /*= NULL*/)
{
    CSocketTask *pSocketTask = NULL;

	switch(iTaskType)
	{
		case MSGPROCESSTASK:
		{
			pSocketTask = new CMsgProcessTask(pTcpSocket, pWorkThread);
			break;
		}
		case TASKTYPE_HTTP_RECV_AND_SEND:
		{
			pSocketTask = new CMyHttpRecvAndSendTask(pTcpSocket, pWorkThread);
			break;
		}
		case TASKTYPE_CONNECT_LISTEN:
		{
			pSocketTask = new CMyMultiListenTask(pTcpSocket, pWorkThread);
			break;
		}
		default:
	{
		pSocketTask = CClassFactory::GenSocketTask(pWorkThread, pTcpSocket, iTaskType);
		break;
	}
	
	}
	return pSocketTask;
}

