/*
 * =====================================================================================
 *
 *       Filename:  myclassfactory.cpp
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

#include "myclassfactory.h"
#include "myrecvandsendtask.h"
#include "ShortConnectedProcessThread.h"
#include "cdnctlmgrdef.h"
#include "getparentcdncmd.h"
#include "getparentscdncmd.h"
#include "getnetrelationcmd.h"
#include "2ndgetnetrelationcmd.h"
#include "getcdnsrvinfocmd.h"
#include "newgetcdnsrvinfocmd.h"
#include "3rdgetcdnsrvinfocmd.h"
#include "reloadmaintaincmd.h"

CMyClassFactory::CMyClassFactory()
{
}

CMyClassFactory::~CMyClassFactory()
{
}

CWorkThread* CMyClassFactory::GenWorkThread(int nEntityType, int nEntityId, string sAnnexData, void *arg)
{
	CWorkThread *pWorkThread = NULL;

	switch (nEntityType)
	{
		case RECVANDSENDMSGENTITY:
			{
				pWorkThread = new CShortConnectedProcessThread (nEntityType, nEntityId);
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
		WriteRunInfo::WriteLog ("CMyClassFactory::GenWorkThread : Invalid thread entity %d", nEntityType);
	}

	return pWorkThread;
}

CSocketTask* CMyClassFactory::GenSocketTask(CNetProcessThread *pWorkThread, CTcpSocket *pTcpSocket, int iTaskType, CTaskArg *pTaskArg /*= NULL*/)
{
	CSocketTask *pSocketTask = NULL;

	switch (iTaskType)
	{
	case RECVANDSENDTASK:
		{
			pSocketTask = new CMyRecvAndSendTask(pTcpSocket, pWorkThread);
			break;
		}
	default:
		{
			pSocketTask = CClassFactory::GenSocketTask (pWorkThread, pTcpSocket, iTaskType);
			break;
		}
	}
	return pSocketTask;
}

CCommand* CMyClassFactory::GenBaseCommand(char* pBuf, int nLen, int nIp /* = 0 */)
{
	CCommand* pCmd = NULL;
	int nCmdId = CCdnCtlMgrBaseCmd<int>::GetCommandId((char*)pBuf, (int)nLen);
	if (nCmdId == INVALID_CMD_ID)
	{
		WRITE_TRACE("CMyClassFactory::GenBaseCommand : GetCommandId Fail");
		return pCmd;
	}
	
	in_addr iaAddr;
	iaAddr.s_addr = ntohl(nIp);
	WriteRunInfo::WriteLog("recv cmd %d ip %s", nCmdId, inet_ntoa(iaAddr));

	switch (nCmdId)
	{
		case GETPARENTCDN_CMD:
		{
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new CGetParentCdnCmd(tmpBuf, nLen);
			break;
		}
		case GETCDNSRVINFO_CMD://add by yinhaichen 2011-03-18
		{
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new CGetCdnSrvInfoCmd(tmpBuf, nLen);
			break;
		}
		case GETPARENTSCDN_CMD://add by yinhaichen 2011-03-29
		{	
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new CGetParentsCdnCmd(tmpBuf, nLen);
			break;
		}
		case NEWGETCDNSRVINFO_CMD: //added by hezhipeng
		{
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new CNewGetCdnSrvInfoCmd(tmpBuf, nLen);
			break;
		}
		case GETNETRELATION_CMD://added by wangfangzhu 2011-10-17 ,modified by hezhipeng
			{
				char *tmpBuf = new char[nLen];
				memcpy (tmpBuf, pBuf, nLen);
				pCmd = new CGetNetReLationCmd(tmpBuf, nLen);
				break;
			}
		case _3RDGETCDNSRVINFO_CMD: //added by chenhui
		{
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new C3rdGetCdnSrvInfoCmd(tmpBuf, nLen);
			break;
		}
		case _2NDGETNETRELATION_CMD://added by chenhui
		{
			char *tmpBuf = new char[nLen];
			memcpy (tmpBuf, pBuf, nLen);
			pCmd = new C2ndGetNetReLationCmd(tmpBuf, nLen);
			break;
		}
		default:
		{
			WriteRunInfo::WriteError ("CMyClassFactory::GenBaseCommand : invalid cmd type %d", nCmdId);
			break;
		}
	}

	if (pCmd)
	{
		pCmd->SetIp(nIp);
		WRITE_TRACE ("CMyClassFactory::GenBaseCommand : Recv from client a cmd(%d)", (int)nCmdId);
	}
	return pCmd;
}

CBaseMaintainCmd* CMyClassFactory::GenMaintainCmd(int nMsgType, int nBufLen, char* pBuf)
{
	CBaseMaintainCmd *pCmd = NULL;
	switch( nMsgType )
	{
		case RELOADDB_MAINCMD:
			{
				pCmd = new CReloadMainTainCmd(nMsgType, nBufLen, pBuf);
				break;
			}
		default:
			{
				pCmd = CClassFactory::GenMaintainCmd(nMsgType, nBufLen, pBuf);
				break;
			}
	}
	return pCmd;
}


