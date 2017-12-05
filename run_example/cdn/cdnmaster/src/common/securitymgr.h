/**
 *    @file       securitymgr.h
 *
 *    @brief      back database
 *    @details
 *
 *    @version    1.0
 *    @date       08/23/2013 07:27:55 PM
 *
 *    @author     wangtiefeng (), 
 */
#ifndef _SECURITYMGR_H_
#define _SECURITYMGR_H_
#include "mastersrvdef.h"


class CSecurityMgr
{
private:
	/// constructor
	CSecurityMgr() {}
	/// destructor
	~CSecurityMgr() {}
public:
	/**
	 * get instance of the singleton
	 * @return instance of the singleton
	 */
	static CSecurityMgr* GetInstance();
	/// create the singleton
	static int Create();
	/// destroy the singleton
	static int Destroy();
public:
	/// init security mgr
	int Init();
	/// load strategy from db and update strategy
	int UpdateStrategy();
	/// check ip valid with strategy
	int CheckIpValid(int nIp);
private:
	static CSecurityMgr* m_pInstance; ///< instance of the singleton
	std::map<int, TIpInfo> m_mapValidIp; ///< valid ip cluster
	CThreadRwLock m_rwValidIpLock; ///< strategy lock
};

#endif


