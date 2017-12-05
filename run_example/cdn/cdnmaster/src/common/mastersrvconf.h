/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: mastersrvconf.h
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
#ifndef __MASTERSRVCONF_H__
#define __MASTERSRVCONF_H__

#include "framecommon/framecommon.h"
#include "mastersrvdef.h"
using namespace std;
using namespace MYFRAMECOMMON;

struct TMasterSrvConfItem
{
	unsigned short nHttpPort;
	string strRestrictIpList;
	int nLoadPaidSourceInterval; ///< a interval to load paid source from db.
	int nLoadShortSourceInterval;
	int nLoadFileSourceInterval;
	int nHotMinKeepTime;

	int nShortConnectProcessThreadNum;
	int nSourceHashBucketLen;
	int nSourceTableNum;
	int nReloadInterval;
	int nIncSourceOverTime;
	int nTaskOverTime;
	int nLoadValidIpInterval; ///< a interval to load valid ip from db.
	float fSourcePrecent;

	int nPaidEnable;
	int nShortEnable;
	int nFileEnable;
	string strFileDir;

	TDbInfo tMasterDb;
	TDbInfo tMasterDbBak; ///< a backup database's info.
	TDbInfo tClusterInfoDb;
	TDbInfo tPaidInfoDb;
	TDbInfo tShortInfoDb;
};

class CMasterSrvConf
{
public:
	CMasterSrvConf(){}
	~CMasterSrvConf(){}
	int Init();
	TMasterSrvConfItem& GetConfItem()
	{
		return m_confItem;
	}
	void Print();
protected:
private:
	TMasterSrvConfItem m_confItem;
};

#endif
