/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: main.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2011-10-18
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2011-10-18			1.0			
* =======================================================================
*/
#include "framecommon/framecommon.h"
#include "mastersrv.h"
#include "mastersrvdef.h"

using namespace std;
using namespace MYFRAMECOMMON;

int main (int argc, char** argv)
{
	int nTraceLogFlab = 0;
	if (argc == 2)
	{
		string strArg = argv[1];
		if (strArg == "-l")
		{
			nTraceLogFlab = 1;
		}
	}
	TLogType LogType;

	LogType.nLogtype = MASTERSRV_DEBUG_TYPE;
	LogType.nFileNum = 0;
	LogType.nLogOff = nTraceLogFlab;
	strncpy (LogType.sFileNameSuffix, ".debug", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	LogType.nLogtype = MASTERSRV_TASKDEBUG_TYPE;
	LogType.nFileNum = 0;
	LogType.nLogOff = nTraceLogFlab;
	strncpy (LogType.sFileNameSuffix, ".taskdebug", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	LogType.nLogtype = MASTERSRV_REGULAR_TYPE;
	LogType.nFileNum = 0;
	LogType.nLogOff = 1;
	strncpy (LogType.sFileNameSuffix, ".reg", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	LogType.nLogtype = MASTERSRV_DB_TYPE;
	LogType.nFileNum = 0;
	LogType.nLogOff = 1;
	strncpy (LogType.sFileNameSuffix, ".db", sizeof (LogType.sFileNameSuffix));
	WriteRunInfo::LOGTYPEARRAY.InsertLogType (LogType);

	WriteRunInfo::Init ("mastersrv");
	WriteRunInfo::WriteLog ("Begin start mastersrv ...");

	CMasterSrv* pServer = new CMasterSrv();
	CMyserver::SetServer (pServer);
	if (pServer->InitServer() != 0)
	{
		WriteRunInfo::WriteError ("Init mastersrv fail");
		return -1;
	}
	if (pServer->StartServer() != 0)
	{
		WriteRunInfo::WriteError ("Start mastersrv fail");
		return -1;
	}

	while (1)
	{
		sleep (100);
	}
	return 0;
}

