/*
 * =====================================================================================
 *
 *       Filename:  cdnctlmgrconf.h
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

#ifndef __CDNCTLMGRCONF_H_C2A3252A_1265_11E0_87FB_0015C55D3D75_
#define __CDNCTLMGRCONF_H_C2A3252A_1265_11E0_87FB_0015C55D3D75_

#include "framecommon/framecommon.h"
using namespace MYFRAMECOMMON;

#define CDNCTLMGRCONFFILE "conf/cdnctlmgr.conf"

typedef struct tagDBItem
{	
	string strSqlIp;
	string strSqlUser;
	string strPasswd;
	string strDbName;
}DBITEM, *PDBITEM;

struct TInfoConfItem
{
	int nReloadInterval;
	DBITEM localDbItem;
	DBITEM monitorDbItem;//add by yinhaichen 2011-03-18	
};

class CCdnCtlMgrConf
{
public:
	CCdnCtlMgrConf();
	~CCdnCtlMgrConf();

	int Init();
	TInfoConfItem& GetConfItem()
	{
		return m_confItem;
	}	
private:
	TInfoConfItem m_confItem;
};


#endif

