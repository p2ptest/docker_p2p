/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcehashtable.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-04-01
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-04-01			1.0			
* =======================================================================
*/
#include "sourcehashtable.h"
#include "taskmanager.h"

CSourceHashTable::CSourceHashTable(int nHashBucketLen, int nIfLock)
	: CMyHashTable<TSourceInfo, list<TSourceInfo> >(nHashBucketLen, nIfLock)
{

}

CSourceHashTable::~CSourceHashTable()
{

}

int CSourceHashTable::AddSource(TSourceInfo &tInfo)
{
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			break;
		}
	}
	int nRet = 0;
	if (listItr == m_list[nRealIndex].end())
	{
		m_list[nRealIndex].push_back(tInfo);
		m_nNodeNum ++;
		nRet = 1;
	}
	HASHUNLOCK(nRealIndex)
	return nRet;
}

int CSourceHashTable::AddCopyNum(const string &strGcid)
{
	TSourceInfo tInfo;
	tInfo.m_strGcid = strGcid;
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	int nRet = 0;
	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			listItr->m_nCopyNum ++; // By CSourceHashTable::AddCopyNum
			nRet = 1;
			break;
		}
	}
	HASHUNLOCK(nRealIndex)
	return nRet;
}

int CSourceHashTable::DeleteSource(const string &strGcid)
{
	TSourceInfo tInfo;
	tInfo.m_strGcid = strGcid;
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	int nRet = 0;
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			m_list[nRealIndex].erase(listItr);
			m_nNodeNum --;
			nRet = 1;
			break;
		}
	}
	HASHUNLOCK(nRealIndex)

	return nRet;
}

int CSourceHashTable::FindSource(const string &strGcid, TSourceInfo &tInfo)
{
	tInfo.m_strGcid = strGcid;
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	int nRet = 0;
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			tInfo = *listItr;
			nRet = 1;
			break;
		}
	}
	HASHUNLOCK(nRealIndex)

	return nRet;
}

bool CSourceHashTable::IfExist(const string &strGcid)
{
	TSourceInfo tInfo;
	tInfo.m_strGcid = strGcid;
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	int nRet = 0;
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			nRet = 1;
			break;
		}
	}
	HASHUNLOCK(nRealIndex)

	return nRet;
}

int CSourceHashTable::UpdateSource(TSourceInfo &tInfo)
{
	int nTmpIndex = tInfo.GetIndex(); //hash
	int nRealIndex = nTmpIndex % m_nHashBucketLen;

	HASHLOCK(nRealIndex)
	HashIterator listItr = m_list[nRealIndex].begin();
	int nRet = 0;
	for (; listItr != m_list[nRealIndex].end(); listItr ++)
	{
		if (tInfo == *listItr)
		{
			nRet = 1;
			break;
		}
	}
	if (listItr != m_list[nRealIndex].end())
	{
		*listItr = tInfo;
	}
	HASHUNLOCK(nRealIndex)
	return nRet;
}

int CSourceHashTable::GetAllSource(vector<TSourceInfo> &vecInfo)
{
	for (int i = 0; i < m_nHashBucketLen; i ++)
	{
		HASHLOCK(i);
		HashIterator listItr = m_list[i].begin();
		for (; listItr != m_list[i].end(); listItr ++)
		{
			vecInfo.push_back(*listItr);
		}
		HASHUNLOCK(i);
	}

	return 0;
}

int CSourceHashTable::GetAllGCid(vector<string> &vecGCidList)
{
	vecGCidList.reserve(vecGCidList.size() + m_nNodeNum + 10000);

	for (int i = 0; i < m_nHashBucketLen; i ++)
	{
		HASHLOCK(i);
		HashIterator listItr = m_list[i].begin();
		for (; listItr != m_list[i].end(); listItr ++)
		{
			vecGCidList.push_back(listItr->m_strGcid);
		}
		HASHUNLOCK(i);
	}
	return 0;
}

int CSourceHashTable::GetGCidByIndex(unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<string> &vecGCidList)
{
	if (nNum == 0)
	{
		return 0;
	}
	for (int i = 0; i < m_nHashBucketLen; i ++)
	{
		HASHLOCK(i);
		HashIterator listItr = m_list[i].begin();
		for (; listItr != m_list[i].end(); listItr ++)
		{
			unsigned int nRealIndex = 0;
			if (CTaskManager::GetInstance()->GetHostIndexByGCid(nClusterId, listItr->m_strGcid, nRealIndex) == 0)
			{
				if (nIndex == nRealIndex)
				{
					vecGCidList.push_back(listItr->m_strGcid);
				}
			}
		}
		HASHUNLOCK(i);
	}
	return 0;
}

