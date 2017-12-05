/*
 * =====================================================================================
 *
 *       Filename:  reloadmaintaincmd.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "reloadmaintaincmd.h"
#include "cdnclustermgr.h"

int CReloadMainTainCmd::Process()
{
	int nRet = CCdnClusterMgr::GetInstance()->LoadFromDb();
	if(nRet == 0)
	{
		 WriteRunInfo::WriteLog("CReloadMainTainCmd::Process : LoadFromDb Suc");
	}
	else
	{
		WriteRunInfo::WriteError("CReloadMainTainCmd::Process : LoadFromDb Fail");
	}
	return nRet;
}
