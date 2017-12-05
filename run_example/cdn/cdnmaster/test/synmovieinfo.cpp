/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: synmovieinfo.cpp
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
#include <iostream>
#include <string>
#include "framecommon/framecommon.h"

using namespace std;
using namespace MYFRAMECOMMON;

int main(int argc, char *argv[])
{
	if (argc != 4)
	{
		printf("Usage:./%s tablenum maxsourcenum hotsourcenum\n", argv[0]);
		return 0;
	}

	unsigned int nTableNum = atoi(argv[1]);
	if(nTableNum == 0)
	{
		printf("invalid table num %d\n", nTableNum);
		return -1;
	}
	unsigned int nMaxSourceNum = atoi(argv[2]);
	unsigned int nHotSourceNum = atoi(argv[3]);
	
	CMysql mySql;
	mySql.Init();
	mySql.Connect("127.0.0.1", "root", "");

	CMysql mySql2;
	mySql2.Init();
	mySql2.Connect("127.0.0.1", "root", "");
	
	string strQuery = "select iid from movieinfo.p2pvod_videos;";
	mySql.Query(strQuery.c_str());
	string strGcid;
	string strTable = "source_info_";
	char strSqlBuf[1024] = {0};
	char strIndex[16] = {0};
	unsigned int nCurSourceNum;
	unsigned int nCurHotNum = 0;
	int nHotFlag = 1;
	while (mySql.FetchRow())
	{
		strGcid = mySql.GetField("iid");
		if(strGcid.length() != 40)
		{
			printf("jump invalid gcid %s\n", strGcid.c_str());
			continue;
		}
		
		nCurHotNum ++;
		if(nCurHotNum > nHotSourceNum)
		{
			nHotFlag = 0;
		}

		string strGcid2 = CToolKit::HexToBi(strGcid.c_str(), 40);
		unsigned int nIndex = CToolKit::shortELFhash(strGcid2.c_str(), (unsigned int)strGcid2.length());
		unsigned int nRealIndex = nIndex % nTableNum;
		sprintf(strIndex, "%d", nRealIndex);
		string strTableName = strTable + strIndex;
		sprintf(strSqlBuf, "replace into %s.%s (gcid,sourcetype,copynum) values('%s', %d, %d);", "dbmastersrv", strTableName.c_str(), strGcid.c_str(), nHotFlag, 0);
		mySql2.Query(strSqlBuf);

		nCurSourceNum ++;
		if(nCurSourceNum >= nMaxSourceNum)
		{
			break;
		}
	}
	return 0;
}
