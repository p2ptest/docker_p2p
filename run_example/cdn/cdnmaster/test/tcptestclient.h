/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com 
 */

#ifndef _TCP_TEST_CLIENT_H_
#define _TCP_TEST_CLIENT_H_

#include "framecommon/framecommon.h"
#include <iostream>
#include <vector>
using namespace std;
using namespace MYFRAMECOMMON;

const int CMDID_JUMP = 116;
const int MAX_HEAD_LEN = 8 * 1024 * 1024;

const unsigned int g_nMaxUserNameLen = 500;
const unsigned int g_nMaxFileNameLen = 500;
const unsigned int g_nMaxFileKeyLen = 500;
const unsigned int g_nMaxUrlLen = 500;
const unsigned int g_nMaxStreamNum = 500;

//for RecvAndSendTask
const unsigned int RECV_BUF_LEN = 40960; //40KB

const unsigned int FILEID_BIN_LEN = 16;
const unsigned int FILE_ID_HEX_LEN = 32;

const unsigned int MD5_BIN_LEN = 16;
const unsigned int MD5_HEX_LEN = 32;

const unsigned int GCID_BIN_LEN = 20;
const unsigned int GCID_HEX_LEN = 40;

const unsigned char FT_PUBLIC = 0;
const unsigned char FT_PRIVATE = 1;

const unsigned char TT_TRANS = 0;
const unsigned char TT_NOT_TRANS = 1;

const unsigned char AT_AUDIT = 0;
const unsigned char AT_NOT_AUDIT = 1;
const unsigned char AT_DELAY_AUDIT = 2;

const unsigned char ST_PLATFORM = 0;
const unsigned char ST_SOFTWARE = 1;

const unsigned char VT_SHORT = 0;
const unsigned char VT_LONG = 1;


//命令字
const int CMDID_WEB_PUB = 127;
const int CMDID_WEB_PUB_RES = 128;
const int CMDID_WEB_DEL = 129;
const int CMDID_WEB_DEL_RES = 130;
const int CMDID_WEB_AUDIT = 131;
const int CMDID_WEB_AUDIT_RES = 132;
const int CMDID_WEB_EDIT = 133;
const int CMDID_WEB_EDIT_RES = 134;

const int CMDID_ACCESS_PUB = 101;
const int CMDID_ACCESS_PUB_RES = 102;
const int CMDID_ACCESS_DEL = 103;
const int CMDID_ACCESS_DEL_RES = 104;
const int CMDID_ACCESS_EDIT = 135;
const int CMDID_ACCESS_EDIT_RES = 136;

const int CMDID_VOD_FIN = 105;
const int CMDID_VOD_FIN_RES = 106;
const int CMDID_VOD_DEL = 107;
const int CMDID_VOD_DEL_RES = 108;

const int CMDID_TRANS_GO = 109;
const int CMDID_TRANS_GO_RES = 110;
const int CMDID_TRANS_FIN = 111;
const int CMDID_TRANS_FIN_RES = 112;
const int CMDID_TRANS_DEL = 113;
const int CMDID_TRANS_DEL_RES = 114;

const int CMDID_AUX_GO = 115;
const int CMDID_AUX_GO_RES = 116;
const int CMDID_AUX_FIN = 117;
const int CMDID_AUX_FIN_RES = 118;

const int CMDID_CDN_PUB = 121;
const int CMDID_CDN_PUB_RES = 122;
const int CMDID_CDN_FIN = 123;
const int CMDID_CDN_FIN_RES = 124;
const int CMDID_CDN_DEL = 125;
const int CMDID_CDN_DEL_RES = 126;
const int CMDID_CDN_EDIT = 127;
const int CMDID_CDN_EDIT_RES = 128;
const int INVALID_CMD = -1;
/**
 * cast string to type T.
 * @param[in] str cast from string
 * @return cast to
 */
template <typename T>
T FromString(const string &str)
{
	istringstream is(str);
	T t;
	is >> t;
	return t;
}

/**
 * cast type T to string.
 * @param[in] t cast from type T
 * @return cast to
 */
template <typename T>
string ToString(const T &t)
{
	ostringstream os;
	os << t;
	return os.str();
}

#pragma pack(push, 1)
struct TLongConnMsgHead
{
	short m_usMsgId;
	int m_uSrcClientIp;
	short m_usSrcClientPort;
	long long m_ullSrcTaskId;
	int m_iSrcProcessId;
	long long m_ullDestTaskId;
	int m_iDestProcessId;
	int m_iMsgLen;
	char m_serverCommandPacketContent[0];

	enum EMsgHead
	{
		HEAD_LEN = 36,
		BODYLEN_OFFSET = 32,
		//CMDID_OFFSET = 12,
		//EXTRA_BODYLEN = 1		//sizeof(m_nCmdId)
	};

	int Decode(CBufMgr& BufMgr)
	{
		if (BufMgr.GetShort(m_usMsgId) != 0) {return -1;}

		if (BufMgr.GetInt(m_uSrcClientIp) != 0) {return -1;}
		if (BufMgr.GetShort(m_usSrcClientPort) != 0) {return -1;}

		if (BufMgr.GetLong64(m_ullSrcTaskId) != 0) {return -1;}
		if (BufMgr.GetInt(m_iSrcProcessId) != 0) {return -1;}
		if (BufMgr.GetLong64(m_ullDestTaskId) != 0) {return -1;}
		if (BufMgr.GetInt(m_iDestProcessId) != 0) {return -1;}

		if (BufMgr.GetInt(m_iMsgLen) != 0) {return -1;}
		return 0;
	}

	int Encode(CBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.PutShort(m_usMsgId);

		nRet += BufMgr.PutInt(m_uSrcClientIp);
		nRet += BufMgr.PutShort(m_usSrcClientPort);

		nRet += BufMgr.PutLong64(m_ullSrcTaskId);
		nRet += BufMgr.PutInt(m_iSrcProcessId);
		nRet += BufMgr.PutLong64(m_ullDestTaskId);
		nRet += BufMgr.PutInt(m_iDestProcessId);

		nRet += BufMgr.PutInt(m_iMsgLen);

		return nRet;
	}

	int Print(void)
	{
		cout << "m_usMsgId = " << m_usMsgId << endl;;
		cout << "m_uSrcClientIp = " << m_uSrcClientIp << endl;
		cout << "m_usSrcClientPort = " << m_usSrcClientPort << endl;
		cout << "m_ullSrcTaskId = " << m_ullSrcTaskId << endl;
		cout << "m_iSrcProcessId = " << m_iSrcProcessId << endl;
		cout << "m_ullDestTaskId = " << m_ullDestTaskId << endl;
		cout << "m_iDestProcessId = " << m_iDestProcessId << endl;
		cout << "m_iMsgLen = " << m_iMsgLen << endl;
		return 0;
	}

	static int GetBodyLen(char* pBuf, int nBufLen)
	{
		if (nBufLen < HEAD_LEN)
		{
			WriteRunInfo::WriteError("GetBodyLen : nBufLen error");
			return -1;
		}
		int nBodyLen;
		memcpy(&nBodyLen, pBuf + BODYLEN_OFFSET, sizeof(nBodyLen));
		return ntohl(nBodyLen);
	}
};
#pragma pack(pop)

struct TCmdHead
{
	int m_nProtocolVer;
	int m_nSequenceNo;
	int m_nBodyLen;
	char m_nCmdId;

	enum ECmdHead
	{
		HEAD_LEN = 13,
		BODYLEN_OFFSET = 8,
		CMDID_OFFSET = 12,
		EXTRA_BODYLEN = 1		//sizeof(m_nCmdId)
	};

	int Decode(CBufMgr& BufMgr)
	{
		if (BufMgr.GetInt(m_nProtocolVer) != 0) {return -1;}
		if (BufMgr.GetInt(m_nSequenceNo) != 0) {return -1;}
		if (BufMgr.GetInt(m_nBodyLen) != 0) {return -1;}
		if (BufMgr.GetChar(m_nCmdId) != 0) {return -1;}
		return 0;
	}

	int Encode(CBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.PutInt(m_nProtocolVer);
		nRet += BufMgr.PutInt(m_nSequenceNo);
		nRet += BufMgr.PutInt(m_nBodyLen);
		nRet += BufMgr.PutChar(m_nCmdId);
		return nRet;
	}

	int GetSize()
	{
		return HEAD_LEN;
	}

	int VerifyBodyLen(int nBufLen)
	{
		if ((m_nBodyLen + HEAD_LEN - EXTRA_BODYLEN) != nBufLen)
		{
			return -1;
		}
		return 0;
	}

	int SetBodyLen(int nBodyLen)
	{
		m_nBodyLen = nBodyLen + EXTRA_BODYLEN;
		return 0;
	}

	int GetBodyLen()
	{
		return m_nBodyLen;
	}

	static int GetCmdId(char* pBuf, int nBufLen)
	{
		if (nBufLen < HEAD_LEN)
		{
			WriteRunInfo::WriteError("GetCmdId : nBufLen error");
			return -1;
		}
		unsigned char nCmdId;
		memcpy(&nCmdId, pBuf + CMDID_OFFSET, sizeof(nCmdId));
		return (int)nCmdId;
	}

	static int GetBodyLen(char* pBuf, int nBufLen)
	{
		if (nBufLen < HEAD_LEN)
		{
			WriteRunInfo::WriteError("GetBodyLen : nBufLen error");
			return -1;
		}
		int nBodyLen;
		memcpy(&nBodyLen, pBuf + BODYLEN_OFFSET, sizeof(nBodyLen));
		return ntohl(nBodyLen);
	}

	void Print()
	{
		printf("Version=%d\n", m_nProtocolVer);
		printf("Sequence=%d\n", m_nSequenceNo);
		printf("BodyLen=%d\n", m_nBodyLen);
		printf("CmdId=%d\n", m_nCmdId);
	}
};

class CTcpTestClient
{
public:
	CTcpTestClient(const string& strSrvIp, unsigned short nSrvPort)
	{
		m_strSrvIp = strSrvIp;
		m_nSrvPort = nSrvPort;
		m_bIfConnect = false;
	}

	~CTcpTestClient()
	{
	}

public:
	int Connect()
	{
		if (m_bIfConnect)
		{
			return 0;
		}
		int nRet = m_TcpSocket.Connect(m_strSrvIp, m_nSrvPort, 10);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Connect : connect srv fail ip=%s, port=%u", m_strSrvIp.c_str(), m_nSrvPort);
			return nRet;
		}
		m_bIfConnect = true;
		return nRet;
	}

	int Send(char* pBuf, int nBufLen)
	{
		int nRet = Connect();
		if (nRet != 0)
		{
			return -1;
		}
		int nSendResult = 0;
		int nSendLen = m_TcpSocket.Send(pBuf, nBufLen, 10, nSendResult);
		if (nSendLen != nBufLen)
		{
			WriteRunInfo::WriteLog("Send : send fail");
			return -1;
		}
		return 0;
	}

	int Recv(char* pBuf, int& nBufLen)
	{
		int nRecvResult = 0;
		int nRecvLen = m_TcpSocket.Receive(pBuf, nBufLen, 1, nRecvResult);
		if (nRecvLen <= 0)
		{
			return -1;
		}
		nBufLen = nRecvLen;
		return 0;
	}

	template<typename TCmdHead, typename TCmdBody>
	int Send(TCmdHead& CmdHead, TCmdBody& CmdBody)
	{
		int nRet = Connect();
		if (nRet != 0)
		{
			return -1;
		}
		CmdHead.SetBodyLen(CmdBody.GetSize());
		int nPacketLen = TCmdHead::HEAD_LEN + CmdHead.GetBodyLen();
		char* pBuf = new char[nPacketLen];
		CBufMgr BufMgr(pBuf, nPacketLen);
		nRet = CmdHead.Encode(BufMgr);
		nRet += CmdBody.Encode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Send : encode fail");
			return -1;
		}
		int nSendResult = 0;
		int nSendLen = m_TcpSocket.Send(BufMgr.GetBuf(), BufMgr.GetBufLen(), 10, nSendResult);
		if (nSendLen != BufMgr.GetBufLen())
		{
			WriteRunInfo::WriteLog("Send : send fail");
			return -1;
		}
		return 0;
	}
	template<typename TCmdHead, typename TCmdBody>
	int Recv(TCmdHead& CmdHead, TCmdBody& CmdBody)
	{
		char* pHeadBuf = new char[MAX_HEAD_LEN];
		CBufMgr BufMgr(pHeadBuf, MAX_HEAD_LEN);
		int nRecvResult = 0;
		int nRecvLen = m_TcpSocket.Receive(pHeadBuf, TCmdHead::HEAD_LEN, 10, nRecvResult);
		if (nRecvLen != TCmdHead::HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv head fail");
			return -1;
		}
		int nBodyLen = TCmdHead::GetBodyLen(pHeadBuf, TCmdHead::HEAD_LEN);
		int nRestLen = nBodyLen - TCmdHead::EXTRA_BODYLEN;
		if (nRestLen < 0 || nRestLen + TCmdHead::HEAD_LEN > MAX_HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv bodylen(%d) is to big", nBodyLen);
			return -1;
		}
		nRecvLen = m_TcpSocket.Receive(pHeadBuf + TCmdHead::HEAD_LEN, nRestLen, 10, nRecvResult);
		if (nRecvLen != nRestLen)
		{
			WriteRunInfo::WriteLog("Recv : recv body fail");
			return -1;
		}
		
		if (TCmdHead::GetCmdId(pHeadBuf, TCmdHead::HEAD_LEN) == CMDID_JUMP)
		{
			return 1;
		}
		
		int nRet = CmdHead.Decode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Recv : decode head fail");
			return -1;
		}
		nRet = CmdBody.Decode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Recv : decode body fail");
			return -1;
		}
		return 0;
	}

	template<typename TCmdHead, typename TCmdBody>
	int Send(TLongConnMsgHead& LongConnMsgHead, TCmdHead& CmdHead, TCmdBody& CmdBody)
	{
		int nRet = Connect();
		if (nRet != 0)
		{
			return -1;
		}
		CmdHead.SetBodyLen(CmdBody.GetSize());
		int nPacketLen = TLongConnMsgHead::HEAD_LEN + TCmdHead::HEAD_LEN + CmdHead.GetBodyLen();

		LongConnMsgHead.m_usMsgId = ntohl(2000);
		//LongConnMsgHead.m_iMsgLen = ntohl(TCmdHead::HEAD_LEN + CmdHead.GetBodyLen());
		LongConnMsgHead.m_iMsgLen = TCmdHead::HEAD_LEN + CmdHead.GetBodyLen() - TCmdHead::EXTRA_BODYLEN;
		//LongConnMsgHead.Print();
		char* pBuf = new char[nPacketLen];
		CBufMgr BufMgr(pBuf, nPacketLen);
		nRet = LongConnMsgHead.Encode(BufMgr);
		nRet += CmdHead.Encode(BufMgr);
		nRet += CmdBody.Encode(BufMgr);
		//cout << "nPacketLen = " << nPacketLen << endl;
		//cout << "BufLen = " << BufMgr.GetBufLen() << endl;
		if (nRet != 0)
		{
			printf("Send : encode fail\n");
			return -1;
		}
		int nSendResult = 0;
		int nSendLen = m_TcpSocket.Send(BufMgr.GetBuf(), BufMgr.GetBufLen(), 10, nSendResult);
		if (nSendLen != BufMgr.GetBufLen())
		{
			printf("Send : send fail\n");
			return -1;
		}
		return 0;
	}

	template<typename TCmdHead, typename TCmdBody>
	int Recv(TLongConnMsgHead& LongConnMsgHead, TCmdHead& CmdHead, TCmdBody& CmdBody)
	{
		char* pHeadBuf = new char[MAX_HEAD_LEN];
		CBufMgr BufMgr(pHeadBuf, MAX_HEAD_LEN);
		int nRecvResult = 0;

		int nRecvLen = m_TcpSocket.Receive(pHeadBuf, TLongConnMsgHead::HEAD_LEN, 10, nRecvResult);
		if (nRecvLen != TLongConnMsgHead::HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv long msg head fail");
			return -1;
		}

		int nBodyLen = TLongConnMsgHead::GetBodyLen(pHeadBuf, TLongConnMsgHead::HEAD_LEN);
		int nRestLen = nBodyLen;
		if (nRestLen < 0 || nRestLen + TLongConnMsgHead::HEAD_LEN > MAX_HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv bodylen(%d) is to big", nBodyLen);
			return -1;
		}

		nRecvLen = m_TcpSocket.Receive(pHeadBuf + TLongConnMsgHead::HEAD_LEN, TCmdHead::HEAD_LEN, 10, nRecvResult);
		if (nRecvLen != TCmdHead::HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv cmd head fail");
			return -1;
		}
		nBodyLen = TCmdHead::GetBodyLen(pHeadBuf + TLongConnMsgHead::HEAD_LEN, TCmdHead::HEAD_LEN);
		nRestLen = nBodyLen - TCmdHead::EXTRA_BODYLEN;
		if (nRestLen < 0 || nRestLen + TCmdHead::HEAD_LEN > MAX_HEAD_LEN)
		{
			WriteRunInfo::WriteLog("Recv : recv bodylen(%d) is to big", nBodyLen);
			return -1;
		}
		nRecvLen = m_TcpSocket.Receive(pHeadBuf + TLongConnMsgHead::HEAD_LEN + TCmdHead::HEAD_LEN, nRestLen, 10, nRecvResult);
		if (nRecvLen != nRestLen)
		{
			WriteRunInfo::WriteLog("Recv : recv body fail");
			return -1;
		}
		
		if (TCmdHead::GetCmdId(pHeadBuf, TCmdHead::HEAD_LEN) == CMDID_JUMP)
		{
			return 1;
		}
		
		int nRet = LongConnMsgHead.Decode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Recv : decode long head fail");
			return -1;
		}

		nRet = CmdHead.Decode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Recv : decode cmd head fail");
			return -1;
		}
		nRet = CmdBody.Decode(BufMgr);
		if (nRet != 0)
		{
			WriteRunInfo::WriteLog("Recv : decode cmd body fail");
			return -1;
		}
		return 0;
	}
	//template<typename TCmdHead, typename TCmdBody>
	int Close()
	{
		if (m_bIfConnect)
		{
			m_TcpSocket.Close();
		}
		return 0;
	}

private:
	string m_strSrvIp;
	short m_nSrvPort;
	CTcpSocket m_TcpSocket;
	bool m_bIfConnect;
};

#endif


