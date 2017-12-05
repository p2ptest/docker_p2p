/*
 * =====================================================================================
 *
 *       Filename:  main.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-09
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#include "cdnctlmgrdef.h"
#include "cdnctlmgrserver.h"
#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

int main(int argc, char* argv[])
{

	TLogType LogType;

	LogType.nLogtype = CDNCTLMGRSRV_LOG;
	LogType.nFileNum = 0;
	LogType.nLogOff = 1;
	strncpy (LogType.sFileNameSuffix, ".ctlmgr", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	LogType.nLogtype = CMDPROCESS_LOG;
	LogType.nFileNum = 0;
	LogType.nLogOff = 0;
	strncpy (LogType.sFileNameSuffix, ".cmdpro", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	WriteRunInfo::Init("cdnctlmgrcenter");

	WriteRunInfo::WriteLog("Begin start cdnctlmgrcenter...");
	CCdnCtlMgrServer* pServer = new CCdnCtlMgrServer();
	CMyserver::SetServer(pServer);
	if (pServer->InitServer() != 0)
	{
		WriteRunInfo::WriteError ("Init server fail");
		fprintf(stderr, "Init server fail\n");
		return -1;
	}
	if (pServer->StartServer() != 0)
	{
		WriteRunInfo::WriteError ("Start server fail");
		fprintf(stderr, "Start server fail\n");
		return -1;
	}

	while (1)
	{
		sleep (100);
	}
	return 0;
}

