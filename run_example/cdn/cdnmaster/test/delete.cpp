/*
* =======================================================================
* Copyright (c) 2010-
* All rights reserved 
* -----------------------------------------------------------------------
* Version		: 1.0
* FileName		: delete.cpp
* Description	: 
* -----------------------------------------------------------------------
* Author        : yinhaichen
* Company		: 
* Date			: 2012-06-21
* -----------------------------------------------------------------------
* History		: 
*		<author>		  <time>		<version>		<description>
*      ---------------------------------------------------------------
*		yinhaichen		2012-06-21			1.0			
* =======================================================================
*/
#include <iostream>
#include <string>
#include "framecommon/framecommon.h"

using namespace std;
using namespace MYFRAMECOMMON;

const int PROTOCAL_VER = 0;
const int CMDID = 52;

struct TCmdHead
{
	int m_nProtocolVersion;
	int m_nSeqence;
	int m_nBodylen;
	char m_nCmd;

	enum EHeadPos
	{
		HEAD_LEN = 13,
		BODYLEN_OFFSET = 8,
		CMDID_OFFSET = 12
	};
	int Decode(CBufMgr& BufMgr)
	{
		if (BufMgr.GetInt(m_nProtocolVersion) != 0) {return -1;}
		if (BufMgr.GetInt(m_nSeqence) != 0) {return -1;}
		if (BufMgr.GetInt(m_nBodylen) != 0) {return -1;}
		if (BufMgr.GetChar(m_nCmd) != 0) {return -1;}
		return 0;
	}
	int Encode(CBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.PutInt(m_nProtocolVersion);
		nRet += BufMgr.PutInt(m_nSeqence);
		nRet += BufMgr.PutInt(m_nBodylen);
		nRet += BufMgr.PutChar(m_nCmd);
		return nRet;
	}
	int GetSize()
	{
		return HEAD_LEN;
	}
	static int GetBodyLen(char* pBuf, int nBufLen)
	{
		if (!pBuf || nBufLen < HEAD_LEN)
		{
			return -1;
		}
		return htonl(*((int*)(pBuf + BODYLEN_OFFSET)));
	}
	void Print()
	{
		printf("*******PUBLISH********\n");
		printf("m_nProtocolVersion = %d\n", m_nProtocolVersion);
		printf("m_nSeqence = %d\n", m_nSeqence);
		printf("m_nBodylen = %d\n", m_nBodylen);
		printf("m_nCmd = %d\n", m_nCmd);
	}
};

struct TDelete
{
	string m_strGcid;
	char m_cDeleteType;

	int Encode(CBufMgr &bufMgr)
	{
		int nRet = 0;
		nRet += bufMgr.PutString(m_strGcid);
		nRet += bufMgr.PutChar(m_cDeleteType);
		return nRet;
	}

	int GetSize()
	{
		int nLen = 0;
		nLen += m_strGcid.length() + 4;
		nLen += sizeof(m_cDeleteType);
		return nLen;
	}
	
};

struct TDeleteRes
{
	char m_cResult;

	int Decode(CBufMgr& bufMgr)
	{
		if (bufMgr.GetChar(m_cResult)) {return -1;}
		return 0;
	}

	void Print()
	{
		printf("m_cResult = %d\n", m_cResult);
	}
};

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		printf("Usage:./%s host port Gcid deletetype\n", argv[0]);
		return 0;
	}
	
	TCmdHead sendHead;
	TCmdHead recvHead;
	TDelete sendBody;
	TDeleteRes recvBody;

	if(strlen(argv[3]) != 40)
	{
		printf("invalid gcid len %d\n", (int)strlen(argv[3]));
		return -1;
	}
	
	sendBody.m_strGcid = CToolKit::HexToBi(argv[3], 40);
	sendBody.m_cDeleteType = atoi(argv[4]);

	sendHead.m_nProtocolVersion = 1;
	sendHead.m_nSeqence = 1;
	sendHead.m_nCmd = CMDID;
	sendHead.m_nBodylen = sizeof(sendHead.m_nCmd) + sendBody.GetSize();

	char *buffer1 = new char[1024 * 1024];
	memset(buffer1, 0, 1024 * 1024);
	char *buffer2 = new char[1024 * 1024];
	memset(buffer2, 0, 1024 * 1024);
	CBufMgr sendBuffer(buffer1, 1024 * 1024), recvBuffer(buffer2, 1024 * 1024);
	sendHead.Encode(sendBuffer);
	sendBody.Encode(sendBuffer);

	int nRet = 0;
	CTcpSocket tmpSocket;
	nRet = tmpSocket.Connect(argv[1], atoi(argv[2]));
	if (nRet != 0)
	{
		printf("connect to %s:%d fail, errno = %d\n", argv[1], atoi(argv[2]), errno);
		return -1;
	}

	int nResult = 0;
	nRet = tmpSocket.Send(sendBuffer.GetBuf(), sendBuffer.GetRealBufLen(), 10, nResult);
	if (nRet <= 0)
	{
		printf("send fail, sendlen = %d, errno = %d\n", nRet, errno);
		return -1;
	}

	nRet = tmpSocket.Receive(buffer2, TCmdHead::HEAD_LEN, 10, nResult);
	if (nRet <= 0)
	{
		printf("recv head fail, recvlen = %d, errno = %d\n", nRet, errno);
		return -1;
	}

	int nBodyLen = TCmdHead::GetBodyLen(buffer2, TCmdHead::HEAD_LEN) - 1;
	nRet = tmpSocket.Receive(buffer2 + TCmdHead::HEAD_LEN, nBodyLen, 10, nResult);
	if (nRet <= 0)
	{
		printf("recv body fail, recvlen = %d, errno = %d\n", nRet, errno);
		return -1;
	}

	recvHead.Decode(recvBuffer);
	recvBody.Decode(recvBuffer);
	recvHead.Print();
	recvBody.Print();
	
	return 0;
}

