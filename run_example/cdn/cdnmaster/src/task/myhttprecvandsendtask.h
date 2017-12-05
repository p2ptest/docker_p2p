
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#ifndef _MY_HTTP_RECV_AND_SEND_TASK_
#define _MY_HTTP_RECV_AND_SEND_TASK_

#include "framecommon/framecommon.h"
#include "recvandsendtask.h"
#include "netprocessthread.h"
#include "mastersrv.h"

using namespace MYFRAMECOMMON;

#define HTTP_LIND_END "\r\n"
#define HTTP_HEAD_END "\r\n\r\n"
#define HTTP_COMMON_CONTENT_TYPE "application/json"
#define HTTP_COMMON_CONNECT_TYPE "Keep-Alive"
//#define HTTP_COMMON_CONNECT_TYPE "Close"
//#define HTTP_RETURN_VERSION "HTTP/1.1 200 OK"
#define HTTP_RETURN_VERSION "HTTP/1.1"

#define GEN_HTTP_RET_HEAD(sHTTPHEAD, sContentLen, sStatusCode, sReasonPhrase) \
	(sHTTPHEAD.clear(), sHTTPHEAD + HTTP_RETURN_VERSION +  " " + sStatusCode + " " + sReasonPhrase + HTTP_LIND_END\
	+ "Content-Length: " + sContentLen + HTTP_LIND_END\
	+ "Content-Type: " + HTTP_COMMON_CONTENT_TYPE + HTTP_LIND_END\
	+ "Connection: " + HTTP_COMMON_CONNECT_TYPE + HTTP_HEAD_END);

class CMyHttpRecvAndSendTask : public CRecvAndSendTask
{
public:
	CMyHttpRecvAndSendTask(CTcpSocket *pSocket, CNetProcessThread *pWorkThread, 
		int nListType = 0, unsigned int nHttpHeadBufLen = MAX_RECV_LEN, unsigned int nHttpBodyLen = MAX_RECV_BODY_LEN);
	virtual ~CMyHttpRecvAndSendTask();

	virtual int DestroySelf();
	virtual int Init();
	virtual int PutMsgToSendList(const char *pBuf, int iLen);
	virtual int SendHeadResToList(long long nContenLen);

	virtual int ProcessGetMethod(const char* pPacket, int nPacketLen);
	virtual int ProcessHeadMethod(const char* pPacket, int nPachetLen);
	virtual int ProcessPostMethod(const char *pPacket, int nPacketLen);

private:
	int ProcReOnline(const char *pPacket, int nPacketLen);

protected:        
	virtual int RecvProcess();
	virtual int SendProcess();

protected:
	void ReSetRecvParameter();
	bool IfFinishRecv();

protected:
	int AddToEpoll(void);
	int SendToList(char* pBuf, int nLen);
	int ParsePackets(char *pPacket, int nPacketLen);
	int GetHttpHead();
	int GetHttpBody();
	int GetChunked(const char* pBuf, int nLen);

protected:
	bool m_bIfClose;

	enum ERecvState
	{
		ReadHttpHead = 0,
		ReadHttpBody,
	};

	enum ESendStat
	{
		SendHttpHead = 0,
		SendHttpBody
	};

	ERecvState m_RecvState;
	ESendStat m_SendState;

	char *m_newRecvBuf;
	unsigned int m_nRecvBufLen;

	unsigned int m_nHttpHeadLen;
	unsigned int m_nHttpBodyLen;

	string m_strHttpHead;
	string m_strCmdUrl;
	unsigned int m_nReqContentLen;
	unsigned int m_nHaveRecvLen;
	unsigned int m_nHaveRecvBodyLen;

	long long m_nResContentLen;
	long long m_nHaveSendLen;

	string m_strResponseCode;
	string m_strResponsePhrase;
	bool m_bEncodeHttpHead;			//是否组装HTTP头
};

#endif
