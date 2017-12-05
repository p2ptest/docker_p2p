/*
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
const int GETPARENTCDN_CMD = 100;
const int GETPARENTCDN_CMD_RES = 101;

struct TGetParentCdnCmd
{
	int m_nIp;

	int Encode(CTmpBufMgr& bufMgr)
	{
		if (bufMgr.PutInt(m_nIp))
		{
			return -1;
		}		
		return 0;
	}
	
	int GetSize()
	{
		return sizeof(m_nIp);
	}
	
	void Print()
	{
		cout << "m_nIp = " << m_nIp << endl;
	}
};

struct TCdnParent
{
	char m_nState;
	int m_nIp;
	
	int Decode(CTmpBufMgr& bufMgr)
	{
			int nRet = 0;
			nRet += bufMgr.GetChar(m_nState);
			nRet += bufMgr.GetInt(m_nIp);
			return nRet;
	}	
	
	void Print()
	{
		cout << "m_nState = " << (int)m_nState << endl;
		in_addr iaAddr;
		iaAddr.s_addr = htonl(m_nIp);
		cout << "m_nIp = " << inet_ntoa(iaAddr) << endl;
	}
};

struct TGetParentCdnCmdRes
{
	char m_nResult;
	vector<TCdnParent> m_vecCdnParent;

    int Decode(CTmpBufMgr& bufMgr)
    {
			int nRet = 0;
			nRet += bufMgr.GetChar(m_nResult);
			int nCdnParentNum = 0;
			nRet += bufMgr.GetInt(nCdnParentNum);
			for(int nA = 0; nA < nCdnParentNum; ++ nA)
			{
				TCdnParent tmpCdnParent;
				nRet += tmpCdnParent.Decode(bufMgr);
				m_vecCdnParent.push_back(tmpCdnParent);
			}
			return nRet;
    }

    void Print()
    {
			cout << "m_nResult = " << (int)m_nResult << endl;
			cout << "Parent ClusterInfo:" << endl;
			cout << "Host num = " << m_vecCdnParent.size() << endl;
			for (unsigned int nA = 0; nA < m_vecCdnParent.size(); ++ nA)
			{
				m_vecCdnParent[nA].Print();
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

	TGetParentCdnCmd getParentCdnCmd;
	
	if( argv[3] != NULL )
	{
		struct in_addr tmpIn_addr;
		inet_pton(AF_INET, argv[3], &tmpIn_addr);
		getParentCdnCmd.m_nIp  = ntohl(tmpIn_addr.s_addr);
		
	}
	else
	{
		getParentCdnCmd.m_nIp = 0;
	}

	TMyCmdHead BaseCmd;
	BaseCmd.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmd.m_chCmd = GETPARENTCDN_CMD;
	BaseCmd.m_nBodylen = getParentCdnCmd.GetSize() + sizeof(BaseCmd.m_chCmd);
	CTcpCommonTestClient TcpCommonTestClient(argv[1], atoi(argv[2]));
	
	int nRet = TcpCommonTestClient.Send<TMyCmdHead, TGetParentCdnCmd>(BaseCmd, getParentCdnCmd);
	if (nRet != 0)
	{
		cout << "Send Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetParentCdnCmd=========================================" << endl;
		BaseCmd.Print();
		getParentCdnCmd.Print();
	}
	
	TGetParentCdnCmdRes getParentCdnCmdRes;
	TMyCmdHead BaseCmdRes;
	BaseCmdRes.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmdRes.m_chCmd = GETPARENTCDN_CMD_RES;
	nRet = TcpCommonTestClient.Recv<TMyCmdHead, TGetParentCdnCmdRes>(BaseCmdRes, getParentCdnCmdRes);
	if (nRet != 0)
	{
		cout << "Recv Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetParentCdnCmdRes=========================================" << endl;
		BaseCmdRes.Print();
		getParentCdnCmdRes.Print();
	}
	
	sleep(2);
	return 0;
}
