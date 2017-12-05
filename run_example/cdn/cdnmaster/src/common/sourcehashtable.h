/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: sourcehashtable.h
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
#ifndef __SOURCEHASHTABLE_H__
#define __SOURCEHASHTABLE_H__

#include "framecommon/framecommon.h"
#include "mastersrvdef.h"
using namespace std;
using namespace MYFRAMECOMMON;

class CSourceHashTable: public CMyHashTable<TSourceInfo, list<TSourceInfo> >
{
public:
	CSourceHashTable(int nHashBucketLen, int nIfLock = 1);
	~CSourceHashTable();
	int AddSource(TSourceInfo &tInfo);
	int AddCopyNum(const string &strGcid);
	int DeleteSource(const string &strGcid);
	int FindSource(const string &strGcid, TSourceInfo &tInfo);
	bool IfExist(const string &strGcid);
	int UpdateSource(TSourceInfo &tInfo);
	int GetAllSource(vector<TSourceInfo> &vecInfo);
	int GetAllGCid(vector<string> &vecGCidList);
	int GetGCidByIndex(unsigned int nClusterId, unsigned int nIndex, unsigned int nNum, vector<string> &vecGCidList);
};

#endif
