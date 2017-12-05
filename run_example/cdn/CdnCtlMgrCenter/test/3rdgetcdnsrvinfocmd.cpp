/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: NewGetCdnSrvInfo.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : hezhipeng(hzp), hezhipeng@baofeng.com
* Company		: 
* Date			: 2012-04-06
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		hezhipeng		2012-04-06			1.0			
* =======================================================================
*/
#include <sys/time.h>
#include <fcntl.h>

#include "CdnCltMgrCenterTestClient.h"


const int PROTOCAL_VER = 100;
const int NEWGETCDNSRVINFO_CMD = 110;
const int NEWGETCDNSRVINFO_CMD_RES = 111;

struct TNewGetCdnSrvInfo
{
	char m_nIsNat;
	int m_nRealIp;
	
	int Encode(CTmpBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutChar(m_nIsNat);
		nRet += bufMgr.PutInt(m_nRealIp);
		
		return nRet;
	}
	
	int GetSize()
	{
		int nLen = 0; 
		nLen += sizeof(m_nIsNat);
		nLen += sizeof(m_nRealIp);
		
		return nLen;	
	}
	
	void Print()
	{
		cout << "m_nIsNat=" << (int)m_nIsNat << endl;
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nRealIp);
		cout << "m_nRealIp = " << inet_ntoa(iaAddr) << endl ;
	}
};

struct TCdnStatus
{
	int m_nHostId;
	int m_nState;
	int m_nPublicIp;
	int m_nPrivateIp;
	
	int Decode(CTmpBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.GetInt(m_nHostId);
		nRet += bufMgr.GetInt(m_nState);
		nRet += bufMgr.GetInt(m_nPublicIp);
		nRet += bufMgr.GetInt(m_nPrivateIp);
		return nRet;
	}

	void Print()
	{
		cout << "m_nHostId = " << m_nHostId << endl;
		cout << "m_nState = " << m_nState << endl;
		
		struct in_addr addr;
		addr.s_addr = htonl(m_nPublicIp);
		cout << "m_nPublicIp = " << inet_ntoa(addr) << endl;
		
		addr.s_addr = htonl(m_nPrivateIp);
		cout << "m_nPrivateIp = " << inet_ntoa(addr) << endl;
	}
};

struct TNewGetCdnSrvInfoRes
{
	char m_nResult;
	int m_nClusterId;
	char m_nIsHot;
	char m_nHash;
	char m_nLimitMode;
	int m_nLimitSpeed;
	int m_nMaxSpeed;
	int m_nMultipleSpeed;
	vector<TCdnStatus> m_vecCdnSrvInfo;
	
	int Decode(CTmpBufMgr& bufMgr)
	{
		if (bufMgr.GetChar(m_nResult) != 0) 
			return -1;
		
		if(bufMgr.GetInt(m_nClusterId) != 0)
		{
			return -1;
		}
		
		if (bufMgr.GetChar(m_nIsHot) != 0) 
			return -1;

		if (bufMgr.GetChar(m_nHash) != 0) 
			return -1;
		
		if (bufMgr.GetChar(m_nLimitMode) != 0) 
			return -1;

		if (bufMgr.GetInt(m_nLimitSpeed) != 0) 
			return -1;

		if (bufMgr.GetInt(m_nMaxSpeed) != 0) 
			return -1;

		if (bufMgr.GetInt(m_nMultipleSpeed) != 0) 
			return -1;
		
		int nHostNum = 0;
		if(bufMgr.GetInt(nHostNum) != 0)
			return -1;
		for(int nA = 0; nA < nHostNum; ++ nA)
		{
			TCdnStatus cdnStatusTmp;
			if(cdnStatusTmp.Decode(bufMgr) != 0)
				return -1;
			m_vecCdnSrvInfo.push_back(cdnStatusTmp);
		}	
		return 0;
	}

	void Print()
	{
		cout << "m_nResult = " << (int)m_nResult << endl;
		cout << "m_nClusterId = " << m_nClusterId << endl;
		cout << "m_nIsHot = " << (int)m_nIsHot << endl;
		cout << "m_nHash = " << (int)m_nHash << endl;
		cout << "m_nLimitMode = " << (int)m_nLimitMode << endl;
		cout << "m_nLimitSpeed = " << m_nLimitSpeed << endl;
		cout << "m_nMaxSpeed = " << m_nMaxSpeed << endl;
		cout << "m_nMultipleSpeed = " << m_nMultipleSpeed << endl;
		int nHostNum =  m_vecCdnSrvInfo.size();
		for(int nA = 0; nA < nHostNum; ++ nA)
		{
			cout << "nA = " << nA << endl;
			m_vecCdnSrvInfo[nA].Print();
		}	
	}
};



int main(int argc, char *argv[])
{
	
	if (argc != 4 && argc != 3)
	{
		printf("Usage:%s host port [realip]\n", argv[0]);
		return -1;
	}

	TNewGetCdnSrvInfo newGetCdnSrvInfo;
	
	if( argv[3] != NULL )
	{
		newGetCdnSrvInfo.m_nIsNat = 1;
		struct in_addr tmpIn_addr;
		inet_pton(AF_INET, argv[3], &tmpIn_addr);
		newGetCdnSrvInfo.m_nRealIp  = ntohl(tmpIn_addr.s_addr);
		
	}
	else
	{
		newGetCdnSrvInfo.m_nIsNat = 0;
		newGetCdnSrvInfo.m_nRealIp = 0;
	}

	TMyCmdHead BaseCmd;
	BaseCmd.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmd.m_chCmd = NEWGETCDNSRVINFO_CMD;
	BaseCmd.m_nBodylen = newGetCdnSrvInfo.GetSize() + sizeof(BaseCmd.m_chCmd);
	CTcpCommonTestClient TcpCommonTestClient(argv[1], atoi(argv[2]));
	
	int nRet = TcpCommonTestClient.Send<TMyCmdHead, TNewGetCdnSrvInfo>(BaseCmd, newGetCdnSrvInfo);
	if (nRet != 0)
	{
		cout << "Send Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TNewGetCdnSrvInfo=========================================" << endl;
		BaseCmd.Print();
		newGetCdnSrvInfo.Print();
	}
	
	TNewGetCdnSrvInfoRes newGetCdnSrvInfoRes;
	TMyCmdHead BaseCmdRes;
	BaseCmdRes.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmdRes.m_chCmd = NEWGETCDNSRVINFO_CMD_RES;
	nRet = TcpCommonTestClient.Recv<TMyCmdHead, TNewGetCdnSrvInfoRes>(BaseCmdRes, newGetCdnSrvInfoRes);
	if (nRet != 0)
	{
		cout << "Recv Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TNewGetCdnSrvInfoRes=========================================" << endl;
		BaseCmdRes.Print();
		newGetCdnSrvInfoRes.Print();
	}
	
	sleep(2);
	return 0;
}

