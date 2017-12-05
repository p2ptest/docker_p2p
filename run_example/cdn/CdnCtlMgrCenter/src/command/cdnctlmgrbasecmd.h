/*
 * =====================================================================================
 *
 *       Filename:  cdnctlmgrbasecmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-28
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  wangtiefeng, wangtiefeng@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */
#ifndef _CDNCTLMGRBASECMD_H_B370D57C_122C_11E0_B35F_0015C55D3D75_
#define _CDNCTLMGRBASECMD_H_B370D57C_122C_11E0_B35F_0015C55D3D75_
#include "basecommand.h"
#include "cdnctlmgrdef.h"

struct TCdnCtlMgrCmdHead
{
	int m_nProtocolVersion; //协议版本
	int m_nSeqence;			//消息序列号
	int m_nBodylen;			//消息体长度
	short m_nCmd;			//消息ID
	enum ECdnCtlMgr
	{
		HEAD_LEN = 14,
		BODYLEN_OFFSET = 8,
		CMDID_OFFSET = 12
	};
	int Decode(CBufMgr& BufMgr)
	{
		if (BufMgr.GetInt(m_nProtocolVersion) != 0) {return -1;}
		if (BufMgr.GetInt(m_nSeqence) != 0) {return -1;}
		if (BufMgr.GetInt(m_nBodylen) != 0) {return -1;}
		if (BufMgr.GetShort(m_nCmd) != 0) {return -1;}
		return 0;
	}
	int Encode(CBufMgr& BufMgr)
	{
		int nRet = 0;
		nRet += BufMgr.PutInt(m_nProtocolVersion);
		nRet += BufMgr.PutInt(m_nSeqence);
		nRet += BufMgr.PutInt(m_nBodylen);
		nRet += BufMgr.PutShort(m_nCmd);
		return nRet;
	}
	int GetSize()
	{
		return HEAD_LEN;
	}
};

template <class T1, class T2=CDefaultRes>
class CCdnCtlMgrBaseCmd
	: public CCommand
{
public:
	static int GetCommandId(char* pBuf, int nLen) 
	{
		if(nLen < TCdnCtlMgrCmdHead::HEAD_LEN) 
		{
			WriteRunInfo::WriteLog("CCdnCtlMgrBaseCmd::GetCommandId : cmd buf len is less than %d", TCdnCtlMgrCmdHead::HEAD_LEN);
			return INVALID_CMD_ID;
		}

		unsigned short nCmdId;
		memcpy(&nCmdId, pBuf + TCdnCtlMgrCmdHead::CMDID_OFFSET, sizeof(nCmdId));
		return	ntohs(nCmdId);
	}
public:
	CCdnCtlMgrBaseCmd(char* pBuf, int nLen) : m_outBuf(NULL, 0)
	{
		m_outHead.m_nProtocolVersion = VOD_PROTOCOL_VER;
		m_inBuf.AttachBuf(pBuf, nLen);
		m_bIfDecode = false;
		m_nProcessRet = -1;
	}
	virtual ~CCdnCtlMgrBaseCmd() {}
public:
	TCdnCtlMgrCmdHead& GetInCmdHead() {return m_inHead;} 
	T1& GetInCmdBody() {return m_inBody;}
	virtual void DisAttachBuf() 
	{
		m_outBuf.DistachBuf();
	}
	virtual char* GetResBuf(int& nBufLen)
	{
		nBufLen = m_outBuf.GetRealBufLen();
		return m_outBuf.GetBuf();
	}
	virtual short GetMsgType()
	{
		return m_inHead.m_nCmd;
	}
protected:
	virtual int Process(int& nResult)
	{
		int nRet = Decode();
		if (nRet == 0)
		{
			m_nProcessRet =  ProcessCmd();
			nResult = m_nProcessRet;
			if (m_nProcessRet == 0 && m_nIfRes == 1)
			{   
				GenResHead();
				nRet = Encode();
			}
		}		
		return nRet;
	}
	virtual int Decode()
	{
		int nRet = 0;
		if (!m_bIfDecode)
		{
			m_bIfDecode = true;
			nRet = m_inHead.Decode(m_inBuf);
			if (nRet != 0)
			{
				WriteRunInfo::WriteError("CCdnCtlMgrBaseCmd::Decode : Decode Cmd Head Fail");
				return nRet;
			}
			if ((m_inHead.m_nBodylen + TCdnCtlMgrCmdHead::CMDID_OFFSET) != m_inBuf.GetBufLen())
			{
				WriteRunInfo::WriteError("CCdnCtlMgrBaseCmd::Decode : body len(%d) is invalid", m_inHead.m_nBodylen);
				return -1;
			}
			nRet = m_inBody.Decode(m_inBuf);
		}
		return nRet;
	}
	virtual int GenResHead()
	{
		m_outHead.m_nProtocolVersion = m_inHead.m_nProtocolVersion;
		m_outHead.m_nSeqence = m_inHead.m_nSeqence;
		m_outHead.m_nBodylen = sizeof(m_outHead.m_nCmd) + m_outBody.GetSize();
		return 0;
	}
	virtual int Encode()
	{
		int nPacketLen = m_outHead.GetSize() + m_outHead.m_nBodylen - sizeof(m_outHead.m_nCmd);
		char* pOutBuf = new char[nPacketLen];
		m_outBuf.AttachBuf(pOutBuf, nPacketLen);
		int nRet = m_outHead.Encode(m_outBuf);
		nRet += m_outBody.Encode(m_outBuf);
		return nRet;
	}
protected:
	virtual int ProcessCmd() = 0;
protected:
	bool m_bIfDecode;
	int m_nProcessRet;
	CBufMgr m_inBuf;
	CBufMgr m_outBuf;
	TCdnCtlMgrCmdHead m_inHead;
	TCdnCtlMgrCmdHead m_outHead;
	T1 m_inBody;
	T2 m_outBody;
};

#endif


