/**
 *    @file       dbbackmgr.cpp
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       08/23/2013 07:30:55 PM
 *
 *    @author     wangtiefeng (), 
 */
#include "dbbackmgr.h"
#include "mysqld_error.h"

const string LOCAL_BACK_DB = "local_back_db";
//static member
CDbBackMgr *CDbBackMgr::m_pInstance = NULL;
int CDbBackMgr::Create(const TDbInfo &LocalDbInfo, const TDbInfo &BackDbInfo)
{
	if (!m_pInstance)
	{
		m_pInstance = new CDbBackMgr(LocalDbInfo, BackDbInfo);
	}
	return 0;
}

int CDbBackMgr::Destroy()
{
	if (m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
	return 0;
}

//member
CDbBackMgr::CDbBackMgr(const TDbInfo &LocalDbInfo, const TDbInfo &BackDbInfo)
{
	m_LocalDbInfo = LocalDbInfo;
	m_BackUpDbInfo = BackDbInfo;
}

CDbBackMgr::~CDbBackMgr()
{

}

int CDbBackMgr::Init()
{
	try
	{
		m_mySqlLocal.Init();
		my_bool bReconnectLocal = 1;
		m_mySqlLocal.SetOptions(MYSQL_OPT_RECONNECT, &bReconnectLocal);
		m_mySqlLocal.Connect(m_LocalDbInfo.strIp, m_LocalDbInfo.strUser, m_LocalDbInfo.strPasswd, 5);

		m_mySqlBackUp.Init();
		my_bool bReconnectBackUp = 1;
		m_mySqlBackUp.SetOptions(MYSQL_OPT_RECONNECT, &bReconnectBackUp);
		m_mySqlBackUp.Connect(m_BackUpDbInfo.strIp, m_BackUpDbInfo.strUser, m_BackUpDbInfo.strPasswd, 5);
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError( "CDbBackMgr::Init : fail, error %s", e.GetErrMsg());
		return -1;
	}
	int nRet = LoadFromLocalDb();
	return nRet;
}

int CDbBackMgr::Query(const string &strQuery)
{
	m_rwLock.GetWriteLock();
	if (m_nListSize == 0)
	{
		if (InsertBackInfoToBackUpDb(strQuery) == 0)
		{
			m_rwLock.ReleaseLock();
			return 1;
		}
	}

	TDbBackInfo TmpBackInfo;
	TmpBackInfo.m_nSequence = (++ m_nMaxSeqNum);
	TmpBackInfo.m_strQuery = strQuery;
	if (InsertBackInfoToLocalDb(TmpBackInfo) != 0)
	{
		m_rwLock.ReleaseLock();
		WriteRunInfo::WriteError("CDbBackMgr::Query : %s", strQuery.c_str());
		return -1;
	}

	m_nListSize ++;
	m_lstDbBackInfo.push_back(TmpBackInfo);
	m_rwLock.ReleaseLock();

	return 0;
}

int CDbBackMgr::SynToBackupDb()
{
	m_rwLock.GetWriteLock();
	if (m_nListSize == 0)
	{
		m_rwLock.ReleaseLock();
		return 0;
	}
	TDbBackInfo DbBackInfo;
	while (m_nListSize > 0)
	{
		DbBackInfo = m_lstDbBackInfo.front();
		if (InsertBackInfoToBackUpDb(DbBackInfo.m_strQuery) != 0)
		{
			break;
		}
		if (DeleteBackInfoFromLocalDb(DbBackInfo.m_nSequence) != 0)
		{
			break;
		}
		m_lstDbBackInfo.pop_front();
		m_nListSize --;
	}
	m_rwLock.ReleaseLock();
	return 0;
}

int CDbBackMgr::GetListSize()
{
	int nListSize = 0;
	m_rwLock.GetReadLock();
	nListSize = m_nListSize;
	m_rwLock.ReleaseLock();
	return nListSize;
}

int CDbBackMgr::LoadFromLocalDb()
{
	std::list<TDbBackInfo> lstDbBackInfo;
	long long nMaxSeqNum = 0;
	int nListSize = 0;
	try
	{
		char szQuery[1024];	
		snprintf(szQuery, sizeof(szQuery), "select seqnum,sqlquery from %s.%s order by seqnum;",
				m_LocalDbInfo.strDbName.c_str(), LOCAL_BACK_DB.c_str());
		m_mySqlLocal.Query(szQuery);
		while (m_mySqlLocal.FetchRow())
		{
			TDbBackInfo DbBackInfo;
			DbBackInfo.m_nSequence = strtoll(m_mySqlLocal.GetField("seqnum"), 0, 10);
			DbBackInfo.m_strQuery = m_mySqlLocal.GetField("sqlquery");
			nMaxSeqNum = DbBackInfo.m_nSequence;
			nListSize ++;
			lstDbBackInfo.push_back(DbBackInfo);
		}
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("CDbBackMgr::LoadFromLocalDb : %s", e.GetErrMsg());
		return -1;
	}
	m_rwLock.GetReadLock();
	m_lstDbBackInfo.swap(lstDbBackInfo);
	m_nListSize = nListSize;
	m_nMaxSeqNum = nMaxSeqNum;
	m_rwLock.ReleaseLock();

	SynToBackupDb();

	return 0;
}

int CDbBackMgr::InsertBackInfoToLocalDb(const TDbBackInfo &DbBackInfo)
{
	try
	{
		string strBakInfo = DbBackInfo.m_strQuery;
		m_mySqlLocal.EscapeString(strBakInfo);
		string strQuery = "insert into %s.%s(seqnum, sqlquery) values(%lld,'%s');";
		char szQuery[1024];
		snprintf(szQuery, sizeof(szQuery), strQuery.c_str(),
				m_LocalDbInfo.strDbName.c_str(), LOCAL_BACK_DB.c_str(),
				DbBackInfo.m_nSequence, strBakInfo.c_str());
		m_mySqlLocal.Query(szQuery);
	}
	catch (CMysqlException &e)
	{
		WriteRunInfo::WriteError("CDbBackMgr::InsertBackInfoToLocalDb : fail %s:%s",
				e.GetErrMsg(), DbBackInfo.m_strQuery.c_str()) ;
		return -1;
	}
	return 0;
}

int CDbBackMgr::DeleteBackInfoFromLocalDb(long long nSeqNum)
{
	try
	{
		string strQuery = "delete from %s.%s where seqnum = %lld;";
		char szQuery[1024];
		snprintf(szQuery, sizeof(szQuery), strQuery.c_str(),
				m_LocalDbInfo.strDbName.c_str(), LOCAL_BACK_DB.c_str(), nSeqNum);
		m_mySqlLocal.Query(szQuery);
	}
	catch (CMysqlException &e)
	{
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CDbBackMgr::InsertBackInfoToLocalDb : fail %s", e.GetErrMsg());
		return -1;
	}
	return 0;
}


int CDbBackMgr::InsertBackInfoToBackUpDb(const string &strDbBackInfo)
{
	try
	{
		m_mySqlBackUp.Query(strDbBackInfo);
	}
	catch (CMysqlException &e)
	{
		if (e.GetErrno() == ER_DUP_ENTRY)
		{
			return 0;
		}
		WRITE_ALL_LOG(MASTERSRV_DB_TYPE, "CDbBackMgr::InsertBackInfoToBackUpDb : fail %s", e.GetErrMsg());
		return -1;
	}
	return 0;
};

