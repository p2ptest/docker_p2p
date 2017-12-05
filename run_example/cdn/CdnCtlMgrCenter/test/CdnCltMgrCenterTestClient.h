/*
* =======================================================================
* Copyright (c) 2009-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: CdnCtlMgrCenterTestClient.h
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
#ifndef __CDNCTLMGRCENTERTESTCLIENT_H__
#define __CDNCTLMGRCENTERTESTCLIENT_H__

#include "framecommon/framecommon.h"
#include <iostream>
#include <vector>
#include <errno.h>
#include <string.h>
using namespace std;
using namespace MYFRAMECOMMON;

const int INVALID_CMD_ID=-1;

template <class T, class T1> 
class CComEncodeCommand
{
public:
	CComEncodeCommand(char *pBuf, int nBufLen, T &CmdHead, T1 &CmdBody)
		: m_BufMgr(NULL, 0, 0, 0), m_CmdHead(CmdHead), m_CmdBody(CmdBody)
	{
		m_BufMgr.SetIfNetTrans(1);
		m_BufMgr.AttachBuf(pBuf, nBufLen);
		
	}
	virtual ~CComEncodeCommand(){}
	char* GetPacket()
	{
		return m_BufMgr.GetBuf();
	}
	int GetPacketLen()
	{
		return m_BufMgr.GetRealBufLen();
	}	
	int Encode()
	{
		int ret=0;
		ret += m_CmdHead.Encode(m_BufMgr);
		ret += m_CmdBody.Encode(m_BufMgr);
		return ret;
	}
	CTmpBufMgr & GetBufMgr()
	{
		return m_BufMgr;
	}
protected:	
	CTmpBufMgr m_BufMgr;
	T& m_CmdHead;
	T1& m_CmdBody;
};

template <class T, class T1> 
class CComDecodeCommand
{
public:
	template<class TCmdId>
	static int GetCommandId(char *pBuf, int nBufLen, int nHeadSize, int nOffset)
	{
		if(nBufLen < nHeadSize) 
		{
			WriteRunInfo::WriteLog("The cmd buf len is less than %d too short", nHeadSize);
			return INVALID_CMD_ID;
		}
		TCmdId CmdId;
		memcpy(&CmdId, pBuf + nOffset, sizeof(CmdId));
		return CmdId;
	}
	CComDecodeCommand(char *pBuf, int nBufLen, T &CmdHead, T1 &CmdBody)
		: m_BufMgr(NULL, 0, 0, 0), m_CmdHead(CmdHead), m_CmdBody(CmdBody)
	{
		m_BufMgr.SetIfNetTrans(1);
		m_BufMgr.AttachBuf(pBuf, nBufLen);
		m_nIfDecode=0;
	}
	virtual ~CComDecodeCommand(){}
	int Decode()
	{
		int ret=0;
		if (m_nIfDecode == 0)
		{
			ret += m_CmdHead.Decode(m_BufMgr);
			ret += m_CmdBody.Decode(m_BufMgr);
			m_nIfDecode = 1;
		}            
		return ret;
	}
	CBufMgr& GetBufMgr()
	{
		return m_BufMgr;
	}
protected:
	int m_nIfDecode;
	CTmpBufMgr m_BufMgr;
	T &m_CmdHead;
	T1 &m_CmdBody;
};

class CTcpCommonTestClient
{
public:
	CTcpCommonTestClient(const string &sHost, int nPort)
	{
		m_sHost = sHost;
		m_nPort = nPort;
		m_connect = false;
	}

	~CTcpCommonTestClient()
	{
	}
	int Connect()
	{
		int nRet = 0;
		if (!m_connect)
		{
			nRet = m_socket.Connect(m_sHost, m_nPort);
			if (nRet == 0)
				m_connect=true;
		}
		return nRet;
	}

	template<class T, class T1>
	int Send(T &CmdHead, T1 &CmdBody)
	{
		if (Connect() != 0)
		{
			cout << "Connect Failed" << endl;
			return -1;
		}

		int nPacketLen = CmdHead.GetSize() + CmdBody.GetSize();
		char* pBuf = new char[nPacketLen];
		CBufMgr RecvBufMgr(pBuf, nPacketLen);
		CComEncodeCommand<T, T1> EncodeCmd(pBuf, nPacketLen, CmdHead, CmdBody);
		if (EncodeCmd.Encode() != 0)
		{
			cout << "Encode Failed" << endl;
			return -1;
		}
		
		int nResult = 0;
		int nRet = m_socket.Send(EncodeCmd.GetPacket(), EncodeCmd.GetPacketLen(), 10, nResult);
		if (nRet != EncodeCmd.GetPacketLen())
		{ 
			cout << "Send Packet Fail" << endl;
			return -1;
		}
		return 0;
	}
	template<class T, class T1>
	int Recv(T &CmdHead, T1 &CmdBody)
	{
		//接收头部
		int nResult = 0;
		char tmpBuf[1000] = {0};
		int nRet = m_socket.Receive(tmpBuf, CmdHead.GetSize(), 10, nResult);
		if (nRet != CmdHead.GetSize())
		{
			cout << "Recv Head Fail" << endl;
			return -1;
		}
//////////////////////////////////////////////////////////////////////////
		int nBodyLen = ntohl(CmdHead.GetBodyLen(tmpBuf)) - sizeof(CmdHead.m_chCmd);
		if (nBodyLen < 0)
		{
			cout << "GetBodyLen FAILED" << endl;
			return -1;
		}
		int nPacketLen = CmdHead.GetSize() + nBodyLen ;
		char* pBuf = new char[nPacketLen];
		CBufMgr RecvBufMgr(pBuf, nPacketLen);		//退出函数后，pBuf会被自动释放
		memcpy(pBuf, tmpBuf, CmdHead.GetSize());
		
		//如果nBodyLen == 0，则说明无消息体，无序再接收
		if (nBodyLen > 0)
		{
			nRet = m_socket.Receive(pBuf + CmdHead.GetSize(), nBodyLen , 10, nResult);
			if (nRet != nBodyLen)
			{
				cout << "Recv Body Fail, bodylen = " << nBodyLen << "Cur Recv Len = " << nRet << endl;
				cout << "Head = " << CToolKit::BiToHex(pBuf, CmdHead.GetSize()) << endl;
				return -1;
			}
		}		

		CComDecodeCommand<T, T1> DecodeCmd(pBuf, nPacketLen, CmdHead, CmdBody);
		if (DecodeCmd.Decode() != 0)
		{
			cout << "Decode Failed" << endl;
			return -1;
		}
		return 0;
	}
private:
	CTcpSocket m_socket;
	string m_sHost;
	int m_nPort;
	bool m_connect;
};

//////////////////////////////////////////////////////////////////////////

struct TMyCmdHead
{
	int m_nProtocolVersion;		//协议版本号
	int m_nSeqence;				//包序列号	
	int m_nBodylen;				//包体长	
	short m_chCmd;				//命令ID

	TMyCmdHead()
	{
		m_nProtocolVersion = 101;
		m_nSeqence = 0;
		m_nBodylen = 0;
		m_chCmd = 0;
	}


	enum EHeadPos
	{
		HEAD_LEN = 14,
		BODYLEN_OFFSET = 8,
		CMDID_OFFSET = 12
	};

		
	int Encode(CTmpBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.PutInt(m_nProtocolVersion);
		nRet += BufMgr.PutInt(m_nSeqence);
		nRet += BufMgr.PutInt(m_nBodylen);
		nRet += BufMgr.PutShort(m_chCmd);
		return nRet;
	}
	int Decode(CTmpBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.GetInt(m_nProtocolVersion);
		nRet += BufMgr.GetInt(m_nSeqence);
		nRet += BufMgr.GetInt(m_nBodylen);
		nRet += BufMgr.GetShort(m_chCmd);
		return nRet;
	}

	static int GetSize()
    {
        int iLength = 0;
        iLength += sizeof(int);
        iLength += sizeof(int);
        iLength += sizeof(int);
        iLength += sizeof(short);
        return iLength;
    }
			
	void Print()
	{
        
        cout << "m_nProtocolVersion = " << m_nProtocolVersion << endl;
		cout << "m_nSeqence = " << m_nSeqence << endl;
        cout << "m_nBodylen = " << m_nBodylen << endl;
		cout << "m_chCmd = " << m_chCmd << endl;
	}

	int GetBodyLen(char *pHead)
    {
		if (!pHead)
		{
			return -1;
		}
		return *(int *)(pHead + BODYLEN_OFFSET);
    }
			
};

#endif

