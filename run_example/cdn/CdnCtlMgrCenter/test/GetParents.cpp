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
const int GETPARENTSCDN_CMD = 102;
const int GETPARENTSCDN_CMD_RES = 103;

struct TGetParentsCdnCmd
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

struct TGetParentsCdnCmdRes
{
	char m_nResult;
	vector< vector<TCdnParent> >  m_vecParentCluster;

    int Decode(CTmpBufMgr& bufMgr)
    {
			int nRet = 0;
			nRet += bufMgr.GetChar(m_nResult);
			int nParentsNum = 0;
			nRet += bufMgr.GetInt(nParentsNum);
			for(int nA = 0; nA < nParentsNum; ++ nA)
			{
				vector<TCdnParent> tmpVecCdnParent;
				int nHostNum = 0;
				nRet += bufMgr.GetInt(nHostNum);
				for(int nAA = 0; nAA < nHostNum; ++nAA)
				{
						TCdnParent tmpCdnParent;
						nRet += tmpCdnParent.Decode(bufMgr);
						tmpVecCdnParent.push_back(tmpCdnParent);
				}
				m_vecParentCluster.push_back(tmpVecCdnParent);
			}
			return nRet;
    }

    void Print()
    {
			cout << "m_nResult = " << (int)m_nResult << endl;
			cout << "Parent ClusterInfo:" << endl;
			cout << "Parent num = " << m_vecParentCluster.size() << endl;
			for(unsigned int i = 0; i < m_vecParentCluster.size(); i++)
			{
					cout << "Parent[" << i << "] have host number = " << m_vecParentCluster[i].size() << endl;
					for(unsigned int ii = 0; ii < m_vecParentCluster[i].size(); ii++)
					{
						m_vecParentCluster[i][ii].Print();
					}
			}
    }
};

int main(int argc, char *argv[])
{
	
	if (argc != 4 && argc != 3)
	{
		printf("Usage:%s host port\n", argv[0]);
		return -1;
	}

	TGetParentsCdnCmd getParentsCdnCmd;

	TMyCmdHead BaseCmd;
	BaseCmd.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmd.m_chCmd = GETPARENTSCDN_CMD;
	BaseCmd.m_nBodylen = getParentsCdnCmd.GetSize() + sizeof(BaseCmd.m_chCmd);
	CTcpCommonTestClient TcpCommonTestClient(argv[1], atoi(argv[2]));
	
	int nRet = TcpCommonTestClient.Send<TMyCmdHead, TGetParentsCdnCmd>(BaseCmd, getParentsCdnCmd);
	if (nRet != 0)
	{
		cout << "Send Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetParentsCdnCmd=========================================" << endl;
		BaseCmd.Print();
		getParentsCdnCmd.Print();
	}
	
	TGetParentsCdnCmdRes getParentsCdnCmdRes;
	TMyCmdHead BaseCmdRes;
	BaseCmdRes.m_nProtocolVersion = PROTOCAL_VER;
	BaseCmdRes.m_chCmd = GETPARENTSCDN_CMD_RES;
	nRet = TcpCommonTestClient.Recv<TMyCmdHead, TGetParentsCdnCmdRes>(BaseCmdRes, getParentsCdnCmdRes);
	if (nRet != 0)
	{
		cout << "Recv Failed" << endl;
		return -1;
	}
	else
	{
		cout << "TGetParentsCdnCmdRes=========================================" << endl;
		BaseCmdRes.Print();
		getParentsCdnCmdRes.Print();
	}
	
	sleep(2);
	return 0;
}
