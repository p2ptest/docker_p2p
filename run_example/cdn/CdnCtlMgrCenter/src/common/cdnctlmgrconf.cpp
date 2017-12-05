/*
 * =====================================================================================
 *
 *       Filename:  cdnctlmgrconf.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#include "cdnctlmgrconf.h"

CCdnCtlMgrConf::CCdnCtlMgrConf()
{
}

CCdnCtlMgrConf::~CCdnCtlMgrConf()
{
}

int CCdnCtlMgrConf::Init()
{
	Magic_Config conf(string(CDNCTLMGRCONFFILE));

	int nRet = conf.Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError("Read config file error.");
		return -1;
	}

	string strTmp;

	conf.ReadItem("SERVERINFO", "RELOADINTERVAL", "", strTmp);
	if (strTmp.empty())
	{
		WriteRunInfo::WriteError("Read config file error : reload interval error!!");
		return -1;
	}
	m_confItem.nReloadInterval = atoi(strTmp.c_str());

	conf.ReadItem("LOCALDBINFO", "DBHOST", "", strTmp);
	if (strTmp.empty())
	{
		WriteRunInfo::WriteError("Read config file error : local db ip of %s", strTmp.c_str());
		return -1;
	}
	m_confItem.localDbItem.strSqlIp = strTmp;

	conf.ReadItem("LOCALDBINFO", "DBUSER", "", strTmp);
	if (strTmp.empty())
	{
		WriteRunInfo::WriteError("Read config file error : local db user of %s", strTmp.c_str());
		return -1;
	}
	m_confItem.localDbItem.strSqlUser = strTmp;

	conf.ReadItem("LOCALDBINFO", "DBPASSWD", "", strTmp);
	m_confItem.localDbItem.strPasswd = strTmp;

	conf.ReadItem("LOCALDBINFO", "DBNAME", "cdnctlmgrcenter", strTmp);
	m_confItem.localDbItem.strDbName = strTmp;

	conf.ReadItem("MONITORDBINFO", "DBHOST", "", strTmp);//add by yinhaichen 2011-03-18
	if (strTmp.empty())
	{
		WriteRunInfo::WriteError("Read config file error : monitor db ip of %s", strTmp.c_str());
		return -1;
	}
	m_confItem.monitorDbItem.strSqlIp = strTmp;

	conf.ReadItem("MONITORDBINFO", "DBUSER", "", strTmp);
	if (strTmp.empty())
	{
		WriteRunInfo::WriteError("Read config file error : monitor db user of %s", strTmp.c_str());
		return -1;
	}
	m_confItem.monitorDbItem.strSqlUser = strTmp;

	conf.ReadItem("MONITORDBINFO", "DBPASSWD", "", strTmp);
	m_confItem.monitorDbItem.strPasswd = strTmp;

	conf.ReadItem("MONITORDBINFO", "DBNAME", "monitorcenter", strTmp);
	m_confItem.monitorDbItem.strDbName = strTmp;

	return 0;
}


