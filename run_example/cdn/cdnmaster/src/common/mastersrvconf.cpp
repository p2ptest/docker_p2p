/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: mastersrvconf.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2011-11-24			1.0			
* =======================================================================
*/
#include "mastersrv.h"
#include "mastersrvdef.h"

#define MASTERSRVCONFFILE "conf/mastersrv.conf"

int CMasterSrvConf::Init()
{
	Magic_Config conf(string(MASTERSRVCONFFILE));
	int nRet = conf.Init();
	if (nRet != 0)
	{
		WriteRunInfo::WriteError ("read mastersrv config file error.");
		return -1;
	}
	
	string strTmp;
	conf.ReadItem("SERVERINFO", "HttpPort", "80", strTmp);
	m_confItem.nHttpPort = (unsigned short) atoi(strTmp.c_str());

	conf.ReadItem("SERVERINFO","RestrictIpList", "", strTmp);
	m_confItem.strRestrictIpList = strTmp;

	conf.ReadItem("SERVERINFO", "LoadPaidSourceInterval", "3600", strTmp);
	if (0 == strTmp.size())
	{
		WriteRunInfo::WriteError("conf file error: read LoadPaidSourceInterval failed");
		return -1;
	}
	if ((m_confItem.nLoadPaidSourceInterval = atoi(strTmp.c_str())) <= 0)
	{
		WriteRunInfo::WriteLog("illegal nLoadPaidSourceInterval: %d",
			m_confItem.nLoadPaidSourceInterval);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "LoadShortSourceInterval", "3600", strTmp);
	if (0 == strTmp.size())
	{
		WriteRunInfo::WriteError("conf file error: read LoadShortSourceInterval failed");
		return -1;
	}
	if ((m_confItem.nLoadShortSourceInterval = atoi(strTmp.c_str())) <= 0)
	{
		WriteRunInfo::WriteLog("illegal nLoadShortSourceInterval: %d",
			m_confItem.nLoadShortSourceInterval);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "LoadFileSourceInterval", "3600", strTmp);
	if (0 == strTmp.size())
	{
		WriteRunInfo::WriteError("conf file error: read LoadFileSourceInterval failed");
		return -1;
	}
	if ((m_confItem.nLoadFileSourceInterval = atoi(strTmp.c_str())) <= 0)
	{
		WriteRunInfo::WriteLog("illegal nLoadFileSourceInterval: %d",
			m_confItem.nLoadFileSourceInterval);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "HotMinKeepTime", "3600", strTmp);
	if (0 == strTmp.size())
	{
		WriteRunInfo::WriteError("conf file error: read HotMinKeepTime failed");
		return -1;
	}
	if ((m_confItem.nHotMinKeepTime = atoi(strTmp.c_str())) <= 0)
	{
		WriteRunInfo::WriteLog("illegal nHotMinKeepTime: %d",
			m_confItem.nHotMinKeepTime);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "SHORTCONNECTPROCESSTHREADNUM", "4", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read SHORTCONNECTTHREADNUM error");
		return -1;
	}
	m_confItem.nShortConnectProcessThreadNum = atoi(strTmp.c_str());
	if (m_confItem.nShortConnectProcessThreadNum <= 0 || m_confItem.nShortConnectProcessThreadNum > 100)
	{
		WriteRunInfo::WriteLog("illegal nShortConnectProcessThreadNum: %d", m_confItem.nShortConnectProcessThreadNum);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "SOURCEHASHBUCKETLEN", "5000001", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read SOURCEHASHBUCKETLEN error");
		return -1;
	}
	m_confItem.nSourceHashBucketLen = atoi(strTmp.c_str());
	if (m_confItem.nSourceHashBucketLen <= 0)
	{
		WriteRunInfo::WriteLog("illegal nSourceHashBucketLen: %d", m_confItem.nSourceHashBucketLen);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "SOURCETABLENUM", "11", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read SOURCETABLENUM error");
		return -1;
	}
	m_confItem.nSourceTableNum = atoi(strTmp.c_str());
	if (m_confItem.nSourceTableNum < 1)
	{
		WriteRunInfo::WriteLog("illegal nSourceTableNum: %d", m_confItem.nSourceTableNum);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "RELOADINTERVAL", "60", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read RELOADINTERVAL error");
		return -1;
	}
	m_confItem.nReloadInterval = atoi(strTmp.c_str());
	if (m_confItem.nReloadInterval <= 0)
	{
		WriteRunInfo::WriteLog("illegal nReloadInterval: %d", m_confItem.nReloadInterval);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "INCSOURCEOVERTIME", "600", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read INCSOURCEOVERTIME error");
		return -1;
	}
	m_confItem.nIncSourceOverTime = atoi(strTmp.c_str());
	if (m_confItem.nIncSourceOverTime <= 0)
	{
		WriteRunInfo::WriteLog("illegal nIncSourceOverTime: %d", m_confItem.nIncSourceOverTime);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "TASKOVERTIME", "600", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read TASKOVERTIME error");
		return -1;
	}
	m_confItem.nTaskOverTime = atoi(strTmp.c_str());
	if (m_confItem.nTaskOverTime <= 0)
	{
		WriteRunInfo::WriteLog("illegal nTaskOverTime: %d", m_confItem.nTaskOverTime);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "SOURCEPRECENT", "0.5", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read SOURCEPRECENT error");
		return -1;
	}
	m_confItem.fSourcePrecent = atof(strTmp.c_str());
	if (m_confItem.fSourcePrecent <= 0.0)
	{
		WriteRunInfo::WriteLog("illegal fSourcePrecent: %d", m_confItem.fSourcePrecent);
		return -1;
	}

	conf.ReadItem("SERVERINFO", "PAIDENABLE", "0", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read PAIDENABLE error");
		return -1;
	}
	m_confItem.nPaidEnable = atoi(strTmp.c_str());

	conf.ReadItem("SERVERINFO", "SHORTENABLE", "0", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read SHORTENABLE error");
		return -1;
	}
	m_confItem.nShortEnable = atoi(strTmp.c_str());

	conf.ReadItem("SERVERINFO", "FILEENABLE", "0", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read FILEENABLE error");
		return -1;
	}
	m_confItem.nFileEnable = atoi(strTmp.c_str());

	conf.ReadItem("SERVERINFO", "FILEDIR", "", strTmp);
	m_confItem.strFileDir = strTmp;

	conf.ReadItem("SERVERINFO", "LOADVALIDIPINTERVAL", "300", strTmp);
	if (0 == strTmp.size())
	{
		WriteRunInfo::WriteError("conf file error: read LOADVALIDINTERVAL failed");
		return -1;
	}
	if ((m_confItem.nLoadValidIpInterval = atoi(strTmp.c_str())) <= 0)
	{
		WriteRunInfo::WriteLog("illegal nLoadValidIpInterval: %d",
			m_confItem.nLoadValidIpInterval);
		return -1;
	}

    ///////////////////////////////////////////////////////////////////////
	conf.ReadItem("MASTERDB", "IP", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBIP error");
		return -1;
	}
	m_confItem.tMasterDb.strIp = strTmp;
	if (m_confItem.tMasterDb.strIp.empty())
	{
		WriteRunInfo::WriteLog("illegal db ip");
		return -1;
	}

	conf.ReadItem("MASTERDB", "USER", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBUSER error");
		return -1;
	}
	m_confItem.tMasterDb.strUser = strTmp;
	if (m_confItem.tMasterDb.strUser.empty())
	{
		WriteRunInfo::WriteLog("illegal db user");
		return -1;
	}

	conf.ReadItem("MASTERDB", "PASSWD", "", strTmp);
	m_confItem.tMasterDb.strPasswd = strTmp;

	conf.ReadItem("MASTERDB", "DBNAME", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBNAME error");
		return -1;
	}
	m_confItem.tMasterDb.strDbName = strTmp;
	if (m_confItem.tMasterDb.strDbName.empty())
	{
		WriteRunInfo::WriteLog("illegal db name");
		return -1;
	}

    ///////////////////////////////////////////////////////////////////////
	conf.ReadItem("MASTERDBBAK", "IP", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBIP error");
		return -1;
	}
	m_confItem.tMasterDbBak.strIp = strTmp;
	if (m_confItem.tMasterDbBak.strIp.empty())
	{
		WriteRunInfo::WriteLog("illegal db ip");
		return -1;
	}

	conf.ReadItem("MASTERDBBAK", "USER", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBUSER error");
		return -1;
	}
	m_confItem.tMasterDbBak.strUser = strTmp;
	if (m_confItem.tMasterDbBak.strUser.empty())
	{
		WriteRunInfo::WriteLog("illegal db user");
		return -1;
	}

	conf.ReadItem("MASTERDBBAK", "PASSWD", "", strTmp);
	m_confItem.tMasterDbBak.strPasswd = strTmp;

	m_confItem.tMasterDbBak.strDbName = m_confItem.tMasterDb.strDbName;

    ///////////////////////////////////////////////////////////////////////
	conf.ReadItem("CLUSTERDB", "IP", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBIP error");
		return -1;
	}
	m_confItem.tClusterInfoDb.strIp = strTmp;
	if (m_confItem.tClusterInfoDb.strIp.empty())
	{
		WriteRunInfo::WriteLog("illegal db ip");
		return -1;
	}
	conf.ReadItem("CLUSTERDB", "USER", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBUSER error");
		return -1;
	}
	m_confItem.tClusterInfoDb.strUser = strTmp;
	if (m_confItem.tClusterInfoDb.strUser.empty())
	{
		WriteRunInfo::WriteLog("illegal db user");
		return -1;
	}

	conf.ReadItem("CLUSTERDB", "PASSWD", "", strTmp);
	m_confItem.tClusterInfoDb.strPasswd = strTmp;

	conf.ReadItem("CLUSTERDB", "DBNAME", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBNAME error");
		return -1;
	}
	m_confItem.tClusterInfoDb.strDbName = strTmp;
	if (m_confItem.tClusterInfoDb.strDbName.empty())
	{
		WriteRunInfo::WriteLog("illegal db name");
		return -1;
	}

    ///////////////////////////////////////////////////////////////////////
	conf.ReadItem("PAIDDB", "IP", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBIP error");
		return -1;
	}
	m_confItem.tPaidInfoDb.strIp = strTmp;
	if (m_confItem.tPaidInfoDb.strIp.empty())
	{
		WriteRunInfo::WriteLog("illegal db ip");
		return -1;
	}
	conf.ReadItem("PAIDDB", "USER", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBUSER error");
		return -1;
	}
	m_confItem.tPaidInfoDb.strUser = strTmp;
	if (m_confItem.tPaidInfoDb.strUser.empty())
	{
		WriteRunInfo::WriteLog("illegal db user");
		return -1;
	}

	conf.ReadItem("PAIDDB", "PASSWD", "", strTmp);
	m_confItem.tPaidInfoDb.strPasswd = strTmp;

	conf.ReadItem("PAIDDB", "DBNAME", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBNAME error");
		return -1;
	}
	m_confItem.tPaidInfoDb.strDbName = strTmp;
	if (m_confItem.tPaidInfoDb.strDbName.empty())
	{
		WriteRunInfo::WriteLog("illegal db name");
		return -1;
	}
    ///////////////////////////////////////////////////////////////////////
	conf.ReadItem("SHORTDB", "IP", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBIP error");
		return -1;
	}
	m_confItem.tShortInfoDb.strIp = strTmp;
	if (m_confItem.tShortInfoDb.strIp.empty())
	{
		WriteRunInfo::WriteLog("illegal db ip");
		return -1;
	}
	conf.ReadItem("SHORTDB", "USER", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBUSER error");
		return -1;
	}
	m_confItem.tShortInfoDb.strUser = strTmp;
	if (m_confItem.tShortInfoDb.strUser.empty())
	{
		WriteRunInfo::WriteLog("illegal db user");
		return -1;
	}

	conf.ReadItem("SHORTDB", "PASSWD", "", strTmp);
	m_confItem.tShortInfoDb.strPasswd = strTmp;

	conf.ReadItem("SHORTDB", "DBNAME", "", strTmp);
	if (0 == strTmp.length())
	{
		WriteRunInfo::WriteError("config file error : read DBNAME error");
		return -1;
	}
	m_confItem.tShortInfoDb.strDbName = strTmp;
	if (m_confItem.tShortInfoDb.strDbName.empty())
	{
		WriteRunInfo::WriteLog("illegal db name");
		return -1;
	}
	
	Print();
	return 0;
}

void CMasterSrvConf::Print()
{
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[configuration]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[SERVERINFO]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nHttpPort:%d", m_confItem.nHttpPort);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strRestrictIpList:%s", m_confItem.strRestrictIpList.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nLoadPaidSourceInterval:%d", m_confItem.nLoadPaidSourceInterval);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nLoadShortSourceInterval:%d", m_confItem.nLoadShortSourceInterval);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nLoadFileSourceInterval:%d", m_confItem.nLoadFileSourceInterval);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nHotMinKeepTime:%d", m_confItem.nHotMinKeepTime);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nShortConnectProcessThreadNum:%d", m_confItem.nShortConnectProcessThreadNum);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nSourceHashBucketLen:%d", m_confItem.nSourceHashBucketLen);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nSourceTableNum:%d", m_confItem.nSourceTableNum);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nReloadInterval:%d", m_confItem.nReloadInterval);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nIncSourceOverTime:%d", m_confItem.nIncSourceOverTime);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nTaskOverTime:%d", m_confItem.nTaskOverTime);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "fSourcePrecent:%f", m_confItem.fSourcePrecent);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nLoadValidIpInterval:%d", m_confItem.nLoadValidIpInterval);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nPaidEnable:%d", m_confItem.nPaidEnable);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nFileEnable:%d", m_confItem.nFileEnable);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "nShortEnable:%d", m_confItem.nShortEnable);
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strFileDir:%s", m_confItem.strFileDir.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[MASTERDB]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strIp:%s", m_confItem.tMasterDb.strIp.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strUser:%s", m_confItem.tMasterDb.strUser.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strPasswd:%s", m_confItem.tMasterDb.strPasswd.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strDbName:%s", m_confItem.tMasterDb.strDbName.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[MASTERDB1]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strIp:%s", m_confItem.tMasterDbBak.strIp.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strUser:%s", m_confItem.tMasterDbBak.strUser.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strPasswd:%s", m_confItem.tMasterDbBak.strPasswd.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strDbName:%s", m_confItem.tMasterDbBak.strDbName.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[CLUSTERDB]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strIp:%s", m_confItem.tClusterInfoDb.strIp.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strUser:%s", m_confItem.tClusterInfoDb.strUser.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strPasswd:%s", m_confItem.tClusterInfoDb.strPasswd.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strDbName:%s", m_confItem.tClusterInfoDb.strDbName.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[PAIDDB]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strIp:%s", m_confItem.tPaidInfoDb.strIp.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strUser:%s", m_confItem.tPaidInfoDb.strUser.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strPasswd:%s", m_confItem.tPaidInfoDb.strPasswd.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strDbName:%s", m_confItem.tPaidInfoDb.strDbName.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "[SHORTDB]");
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strIp:%s", m_confItem.tShortInfoDb.strIp.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strUser:%s", m_confItem.tShortInfoDb.strUser.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strPasswd:%s", m_confItem.tShortInfoDb.strPasswd.c_str());
	WRITE_ALL_LOG(MASTERSRV_DEBUG_TYPE, "strDbName:%s", m_confItem.tShortInfoDb.strDbName.c_str());
}
