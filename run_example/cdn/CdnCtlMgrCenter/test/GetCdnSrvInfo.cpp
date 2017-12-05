/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: GetCdnSrvInfo.cpp
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
const int GETCDNSRVINFO_CMD = 104;
const int GETCDNSRVINFO_CMD_RES = 105;

struct TGetCdnSrvInfo
{
	int Encode(CTmpBufMgr& bufMgr)
	{
		return 0;
	}
	
	int GetSize()
	{
		return 0;	
	}
	
	void Print()
	{
		
	}
};

struct TCdnStatus
{
	int m_nState;
	int m_nPublicIp;
	int m_nPrivateIp;
	
	int Decode(CTmpBufMgr& bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.GetInt(m_nState);
		nRet += bufMgr.GetInt(m_nPublicIp);
		nRet += bufMgr.GetInt(m_nPrivateIp);
		return nRet;
	}

	void Print()
	{
		cout << "m_nState = " << m_nState << endl;
		
		struct in_addr addr;
		addr.s_addr = htonl(m_nPublicIp);
		cout << "m_nPublicIp = " << inet_ntoa(addr) << endl;
		
		addr.s_addr = htonl(m_nPrivateIp);
		cout << "m_nPrivateIp = " << inet_ntoa(addr) << endl;
	}
};

struct TGetCdnSrvInfoRes
{
	char m_nResult;
	int m_limitSpeed;
	vector<TCdnStatus> m_vecCdnSrvInfo;
	
	int Decode(CTmpBufMgr& bufMgr)
	{
		if (bufMgr.GetChar(m_nResult) != 0) 
			return -1;
		
		if (bufMgr.GetInt(m_limitSpeed) != 0) 
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
		cout << "m_nResult=" << (int)m_nResult << endl;
		cout << "m_limitSpeed = " << m_limitSpeed << endl;
		for(unsigned int nA = 0; nA < m_vecCdnSrvInfo.size(); ++ nA)
		{
			cout << "nA = " << nA << endl;
			m_vecCdnSrvInfo[nA].Print();
		}	
	}
};



int main(int argc, char *argv[])
{
	
	if (argc != 3)
	{
		printf("Usage:%s host port\n", argv[0]);
		return -1;
	}

	TGetCdnSrvInfo getCdnSrvInfo;
	
	TMyCmdHead BaseCmd;
	BaseCmd.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmd.m_chCmd = GETCDNSRVINFO_CMD;
	BaseCmd.m_nBodylen = getCdnSrvInfo.GetSize() + sizeof(BaseCmd.m_chCmd);
	CTcpCommonTestClient TcpCommonTestClient(argv[1], atoi(argv[2]));
	
	int nRet = TcpCommonTestClient.Send<TMyCmdHead, TGetCdnSrvInfo>(BaseCmd, getCdnSrvInfo);
	if (nRet != 0)
	{
		cout << "Send Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetCdnSrvInfo=========================================" << endl;
		BaseCmd.Print();
		getCdnSrvInfo.Print();
	}
	
	TGetCdnSrvInfoRes getCdnSrvInfoRes;
	TMyCmdHead BaseCmdRes;
	BaseCmdRes.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmdRes.m_chCmd = GETCDNSRVINFO_CMD_RES;
	nRet = TcpCommonTestClient.Recv<TMyCmdHead, TGetCdnSrvInfoRes>(BaseCmdRes, getCdnSrvInfoRes);
	if (nRet != 0)
	{
		cout << "Recv Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetCdnSrvInfoRes=========================================" << endl;
		BaseCmdRes.Print();
		getCdnSrvInfoRes.Print();
	}
	
	sleep(2);
	return 0;
}

