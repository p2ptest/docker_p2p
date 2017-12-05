/**
 *    @file       dbbackmgr.h
 *
 *    @brief      back database
 *    @details
 *
 *    @version    1.0
 *    @date       08/23/2013 07:27:55 PM
 *
 *    @author     wangtiefeng (), 
 */
#ifndef _DBBACKMGR_H_
#define _DBBACKMGR_H_

#include "mastersrvdef.h"

struct TDbBackInfo
{
	long long m_nSequence;
	std::string m_strQuery;
};

class CDbBackMgr
{
public:
	/**
	 * create instance of the singleton
	 * @LocalDbInfom, local db info
	 * @BackDbInfo , backup db info
	 * @return 0, succeed; otherwise，fail
	 */
	static int Create(const TDbInfo &LocalDbInfom, const TDbInfo &BackDbInfo);
	/// destroy the singleton
	static int Destroy();
	/// get instance of the singleton
	static CDbBackMgr* GetInstance()
	{
		return m_pInstance;
	}

private:
	/// constructor
	CDbBackMgr(const TDbInfo &LocalDbInfo, const TDbInfo &BackDbInfo);
	/// destructor
	~CDbBackMgr();

public:
	/// init db back manager
	int Init();
	/// back sql query
	int Query(const string &strQuery);
	/// synchronize sql query to back db
	int SynToBackupDb();
	/// get size of sql query list
	int GetListSize();
	/// load local backup sql statement
	int LoadFromLocalDb();

public:
	/// insert sql statement to local db
	int InsertBackInfoToLocalDb(const TDbBackInfo &DbBackInfo);
	/// delete sql statement from loca db
	int DeleteBackInfoFromLocalDb(long long nSeqNum);
	/// execute sql on backup db
	int InsertBackInfoToBackUpDb(const string &strDbBackInfo);

private:
	TDbInfo m_LocalDbInfo; ///< local db info
	TDbInfo m_BackUpDbInfo; ///< backup db info
	std::list<TDbBackInfo> m_lstDbBackInfo; ///< sql query list
	int m_nListSize; ///< size of sql query list
	long long m_nMaxSeqNum; ///< cur max sequence num
	CThreadRwLock m_rwLock; ///< lock for sql query list
	CMysql m_mySqlLocal;
	CMysql m_mySqlBackUp;

private:
	static CDbBackMgr *m_pInstance;
};

#endif



