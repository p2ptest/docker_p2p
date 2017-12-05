
/*
 * Copyright (C) Chen Hui (Buck), chenhui101@gmail.com
 */

#include "myhttprecvandsendtask.h"
#include "json/phpjson.h"
#include "json/phpjsonparser.h"
#include "sourcemanager.h"

CMyHttpRecvAndSendTask::CMyHttpRecvAndSendTask(CTcpSocket* pSocket, CNetProcessThread* pWorkThread,
	int nListType/* = 0 */,  unsigned int nHttpHeadBufLen/* = MAX_RECV_LEN */, unsigned int nHttpBodyLen/* = MAX_RECV_BODY_LEN */)
	: CRecvAndSendTask(pSocket, pWorkThread, nListType)
{
	m_bIfClose = false;
	m_nHttpHeadLen = nHttpHeadBufLen >= MIN_HTTP_HEAD_LEN ? nHttpHeadBufLen : MIN_HTTP_HEAD_LEN;
	m_nHttpBodyLen = nHttpBodyLen >= MIN_HTTP_BODY_LEN ? nHttpBodyLen : MIN_HTTP_BODY_LEN;
	m_nRecvBufLen = m_nHttpBodyLen + m_nHttpHeadLen;
	m_strResponseCode = "200";
	m_strResponsePhrase = "OK";

	m_newRecvBuf = NULL;
	m_bEncodeHttpHead = true;
	ReSetRecvParameter();
}

CMyHttpRecvAndSendTask::~CMyHttpRecvAndSendTask()
{
	if (m_newRecvBuf)
	{
		delete [] m_newRecvBuf;
		m_newRecvBuf = NULL;
	}
}

int CMyHttpRecvAndSendTask::DestroySelf()
{
	delete this;
	return 0;
}

int CMyHttpRecvAndSendTask::Init()
{
	if (!m_newRecvBuf)
	{
		if (NULL == (m_newRecvBuf = new char[m_nRecvBufLen]))
		{
			return -1;
		}
		memset(m_newRecvBuf, 0, m_nRecvBufLen);
	}
	AddToEpoll();
	return 0;
}

int CMyHttpRecvAndSendTask::AddToEpoll(void)
{
	int nEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN;
	if (m_sendList.GetNum() != 0 || m_pCurSendMsg != NULL)
	{
		nEvent = nEvent | MYPOLLOUT;
	}

	CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll *pEoll = pNetThread->GetEpoll();
	pEoll->AddToEpoll(this, nEvent);
	return 0;
}

int CMyHttpRecvAndSendTask::PutMsgToSendList(const char* pBuf, int nLen)
{
	return SendToList((char *)pBuf, nLen);
}

//先发送报头和报体长度，报体等会再发
int CMyHttpRecvAndSendTask::SendHeadResToList(long long nContenLen)
{
	string strHttpHead;
	char szBodyLen[12];
	memset(szBodyLen, 0, sizeof(szBodyLen));
	snprintf(szBodyLen, sizeof(szBodyLen), "%lld", nContenLen);
	strHttpHead = GEN_HTTP_RET_HEAD(strHttpHead, szBodyLen, m_strResponseCode, m_strResponsePhrase);

	char *pHttpReHeadBuf = new char[strHttpHead.length()];
	memcpy(pHttpReHeadBuf, strHttpHead.c_str(), strHttpHead.length());
	CSendMsg *pSendMsg = new CSendMsg(strHttpHead.length(), pHttpReHeadBuf);
	m_sendList.push_back(pSendMsg);

	int nEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN | MYPOLLOUT;
	CNetProcessThread* pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll* pEoll = pNetThread->GetEpoll();
	pEoll->ModifyFromEpoll(this, nEvent);
	return 0;
}

int CMyHttpRecvAndSendTask::SendToList(char *pBuf, int nLen)
{
	if (pBuf == NULL || nLen == 0)
	{
		return -1;
	}
	string strHttpHead;
	long long llResContentLen = nLen;
	if (m_bEncodeHttpHead)
	{
		char szBodyLen[12];
		memset(szBodyLen, 0 , sizeof(szBodyLen));
		snprintf(szBodyLen, sizeof(szBodyLen), "%lld", llResContentLen);
		strHttpHead = GEN_HTTP_RET_HEAD(strHttpHead, szBodyLen, m_strResponseCode, m_strResponsePhrase);
	}

	char* pHttpResHeadBuf = new char[strHttpHead.length() + nLen];
	memcpy(pHttpResHeadBuf, strHttpHead.c_str(), strHttpHead.length());
	memcpy(pHttpResHeadBuf + strHttpHead.length(), pBuf, nLen);

	//在将协议封装至独立的command以前，不用显示释放内存
	//delete [] pBuf;

	CSendMsg *pSendMsg = new CSendMsg(strHttpHead.length() + nLen, pHttpResHeadBuf);
	m_sendList.push_back(pSendMsg);

	m_nHaveSendLen += nLen;  //已经送至发送队列的数据长度

	int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN | MYPOLLOUT;
	CNetProcessThread* pNetThread = (CNetProcessThread*)m_pThread;
	CEpoll* pEoll = pNetThread->GetEpoll();
	pEoll->ModifyFromEpoll(this, tmpEvent);
	return 0;
}

//if this function returns -1 ,task will be destroy
int CMyHttpRecvAndSendTask::RecvProcess()
{
	int nRet = 0;
	switch (m_RecvState)
	{
		case ReadHttpHead:
		{
			nRet = GetHttpHead();
			break;
		}

		case ReadHttpBody:
		{
			nRet = GetHttpBody();
			break;
		}
	}
	if (IfFinishRecv())
	{
		nRet = ParsePackets((char *)m_newRecvBuf, m_nHaveRecvLen);
		if (nRet != 0)
		{
			return nRet;
		}
	}
	return nRet;
}


//if this function returns -1 ,task will be destroy
int CMyHttpRecvAndSendTask::SendProcess()
{
	int nRet = 0;
	if (m_pCurSendMsg != NULL)
	{
		nRet = SendData();
	}
	else //NULL
	{
		m_pCurSendMsg = GetMsgFromSendList();
		if (m_pCurSendMsg != NULL)
		{
			nRet = SendData();
		}
		else
		{
			int tmpEvent = MYPOLLERR | MYPOLLHUP | MYPOLLIN;
			CNetProcessThread *pNetThread = (CNetProcessThread*)m_pThread;
			CEpoll *pEoll = pNetThread->GetEpoll();
			pEoll->ModifyFromEpoll(this, tmpEvent);
			//nRet = 1;
		}
	}
	return nRet;
}

void CMyHttpRecvAndSendTask::ReSetRecvParameter()
{

	m_RecvState = ReadHttpHead;
	m_SendState = SendHttpHead;

	m_strResponseCode.assign("200");
	m_strResponsePhrase.assign("OK");
	m_nHaveSendLen = 0;
	m_nResContentLen = 0;

	m_nHaveRecvLen = 0;
	m_nHaveRecvBodyLen = 0;
	m_nReqContentLen = 0;

	if (m_newRecvBuf)
	{
		memset(m_newRecvBuf, 0, m_nRecvBufLen);
	}
	m_strHttpHead.clear();
}

bool CMyHttpRecvAndSendTask::IfFinishRecv()
{
	if (m_strHttpHead.size() > 0 && m_nHaveRecvBodyLen >= 0 && 
			(m_nHaveRecvLen - (unsigned)m_strHttpHead.size()) >= m_nReqContentLen)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CMyHttpRecvAndSendTask::ProcessPostMethod(const char *pPacket, int nPacketLen)
{
	int nRet = 0;
	int nIp = GetRemoteIp(GetTcpSocket()->GetSocket());
	char szIp[16];
	inet_ntop(AF_INET, &nIp, szIp, sizeof(szIp));
	string strRestrictIpList = ((CMasterSrv*) CMyserver::GetInstance())->GetConf().GetConfItem().strRestrictIpList;
	if (!strRestrictIpList.empty())
	{
		if (strRestrictIpList.find(szIp) == string::npos)
		{
			WriteRunInfo::WriteError("CMyHttpRecvAndSendTask::ProcessPostMethod : [%s] random or attack packet from source:[%s]", 
					m_strCmdUrl.c_str(), szIp);
			return -1;
		}
	}
	//////////////////////////////////////////////
	if (m_strCmdUrl.compare("/online") == 0)
	{
		nRet = ProcReOnline(pPacket, nPacketLen);
	}
	else
	{
		WriteRunInfo::WriteError("ProcessPostMethod - Error Cmd Url : [%s]", m_strCmdUrl.c_str());
		nRet = 3; //url请求地址不合法
	}

	if (nRet != 0)
	{
		WriteRunInfo::WriteError("ProcessPostMethod - Process Cmd Url:[%s], Content:[%s] Error:[%d]", 
				m_strCmdUrl.c_str(), pPacket, nRet);
	}

	//////////////////////回包生成//////////////////////////////////////
	char szContent[1024];
	snprintf(szContent, sizeof(szContent), "{\"status\":%d}", nRet);
	SendToList(szContent, strlen(szContent));
	////////////////////日志统计打印////////////////////////////////////////
	//WRITE_ALL_LOG(LOGTYPE_COMMAND, "****************WebPubCmdRes*******************");
	//WRITE_ALL_LOG(LOGTYPE_COMMAND, "m_cResult : [%d]", nRet);
	//WRITE_ALL_LOG(LOGTYPE_COMMAND, "m_strFileID : [%s]", strHexFileID.c_str());
	//WRITE_ALL_LOG(LOGTYPE_COMMAND, "m_strGCid : [%s]", strHexGCid.c_str());
	return (nRet == -1) ? -1 : 0;
}

int CMyHttpRecvAndSendTask::ProcReOnline(const char *pPacket, int nPacketLen)
{
	int nRet = 0;
	int nNum = 0;
	vector<string> vecGcids;
	phpjsonparser jsonparser;
	phpjson* pjson = jsonparser.decode((unsigned char*)pPacket, nPacketLen);

	if (pjson)
	{
		//////////////////////协议校验////////////////////////////////////
		if ((*pjson)["iid"].get_type() == pjtype_array)
		{
			nNum = (*pjson)["iid"].get_value().m_array->size();
			vector<phpjson*> vec = *((*pjson)["iid"].get_value().m_array);
			for (int nA = 0; nA < nNum; nA++)
			{
				string strHexGcid = *vec[nA]->get_value().m_string;
				if (strHexGcid.length() != 40)
				{
					nRet = 2; //json串不合法
					break;
				}
				vecGcids.push_back(CToolKit::HexToBi(strHexGcid.c_str(), strHexGcid.length()));

			}
			////////////////////日志统计打印////////////////////////////////////////
			//WRITE_ALL_LOG(LOGTYPE_COMMAND, "****************WebPubCmd*******************");
			//WRITE_ALL_LOG(LOGTYPE_COMMAND, "m_nUserID : [%llu]", nUserID);

			////////////////////业务处理////////////////////////////////////////
			if (nRet == 0)
			{
				for (int nA = 0; nA < nNum; nA++)
				{
					TSourceInfo tTmpSourceInfo;
					tTmpSourceInfo.m_strGcid = vecGcids[nA];
					tTmpSourceInfo.m_nSourceType = EST_HotSource;
					tTmpSourceInfo.m_nPublishTime = CMyserver::GetInstance()->GetTimeVal().tv_sec;
					tTmpSourceInfo.m_nStartTime = tTmpSourceInfo.m_nPublishTime;
					tTmpSourceInfo.m_nLastReportTime= tTmpSourceInfo.m_nPublishTime;
					int nTmp = CSourceManager::GetInstance()->ReOnlineSource(tTmpSourceInfo);
					if (nTmp == DB_ERROR)
					{
						nRet = 1; //数据库失败
						break;
					}
				}
			}
		}
		else
		{
			nRet = 2; //json串不合法
		}
	}
	else
	{
		nRet = 2; //json串不合法
	}
	WRITE_ALL_LOG(MASTERSRV_REGULAR_TYPE, "http reonline num:%d ret:%d", nNum, nRet);
	return nRet;
}

int CMyHttpRecvAndSendTask::ProcessGetMethod(const char* pPacket, int nPacketLen)
{
	return 0;
}

int CMyHttpRecvAndSendTask::ProcessHeadMethod(const char* pPacket, int nPacketLen)
{
	return 0;
}

int CMyHttpRecvAndSendTask::ParsePackets(char *pPacket, int nPacketLen)
{
	char *pCurPos = pPacket;
	char *pEndPos = pPacket + nPacketLen;
	int nRet = 0;
	unsigned int nRestLen = pEndPos - pCurPos;
	while (1)
	{
		char *pTmp = strstr(pCurPos, "\r\n\r\n");
		if (pTmp == NULL)
		{
			m_RecvState = ReadHttpHead;
			break;
		}
		//Get Http Head
		m_strHttpHead = string(pCurPos, pTmp - pCurPos + 4);

		//Get Http Head Method
		char* pMethodTmp = strstr(pCurPos, " ");
		if (NULL == pMethodTmp)
		{
			WriteRunInfo::WriteLog("ParsePackets - There is not Http Head Method ");
			WriteRunInfo::WriteLog("%s", m_strHttpHead.c_str());
			return -1;
		}
		string strMethod(pCurPos, pMethodTmp - pCurPos);
		if (0 == strMethod.compare("GET"))
		{
			m_nReqContentLen = 0;
			m_nHaveRecvBodyLen = 0;
			m_RecvState = ReadHttpHead;
			nRet = ProcessGetMethod(m_strHttpHead.c_str(), m_strHttpHead.length());
			if (nRet != 0)
			{
				WriteRunInfo::WriteLog("ProcessGetMethod Failed");
				WriteRunInfo::WriteLog("Buf = %s", m_strHttpHead.c_str());
				return -1;
			}

			pCurPos += m_strHttpHead.length();
			nRestLen = pEndPos - pCurPos;
			m_strHttpHead.clear();
		}
		else if (0 == strMethod.compare("HEAD"))
		{
			m_nReqContentLen = 0;
			m_nHaveRecvBodyLen = 0;
			m_RecvState = ReadHttpHead;
			nRet = ProcessHeadMethod(m_strHttpHead.c_str(), (int)m_strHttpHead.length());
			if (nRet != 0)
			{
				WriteRunInfo::WriteLog("ProcessHeadMethod Failed");
				WriteRunInfo::WriteLog("Buf = %s", m_strHttpHead.c_str());
				return -1;
			}

			pCurPos += m_strHttpHead.length();
			nRestLen = pEndPos - pCurPos;
			m_strHttpHead.clear();
		}
		else if (0 == strMethod.compare("POST"))
		{
			string sOut;
			int nTmp;
			//find url 
			nTmp = CToolKit::GetStringByLabel(m_strHttpHead, "POST ", " HTTP", sOut, 0, 0);
			if (nTmp != 0)
			{
				WriteRunInfo::WriteLog("There is no Cmd Url");
				return -1;
			}
			m_strCmdUrl = sOut;
			//find content-len
			nTmp = CToolKit::GetStringByLabel(m_strHttpHead, "Content-Length:", "\r\n", sOut, 0, 0);
			if (nTmp != 0)
			{
				nTmp = CToolKit::GetStringByLabel(m_strHttpHead, "Content-length:", "\r\n", sOut, 0, 0);
				if (nTmp != 0)
				{
					WriteRunInfo::WriteLog("There is no Content-Length");
					return -1;
				}
			}
			m_nReqContentLen = strtoull(sOut.c_str(), NULL, 10);

			if (m_nReqContentLen > (pEndPos - pCurPos) - m_strHttpHead.length())
			{
				m_RecvState = ReadHttpBody;
				m_nHaveRecvBodyLen = (pEndPos - pCurPos) - m_strHttpHead.length();
				break;
			}
			//仅仅传入的是json体
			nRet = ProcessPostMethod(pTmp + 4, m_nReqContentLen);
			if (nRet != 0)
			{
				WRITE_TRACE("ProcessPostMedthod Failed");
				WRITE_TRACE("Buf = %s", pCurPos);
				return -1;
			}
			
			pCurPos += (m_strHttpHead.length() + m_nReqContentLen);
			nRestLen = pEndPos - pCurPos;

			m_strHttpHead.clear();
			m_nHaveRecvBodyLen = 0;
			m_nReqContentLen = 0;
		}
		else
		{
			WRITE_TRACE("Do not support the method %s", strMethod.c_str());
			WRITE_TRACE("%s", m_strHttpHead.c_str());
			return -1;
		}
	}
	
	nRestLen = pEndPos - pCurPos;
	m_nHaveRecvLen = nRestLen;
	if (nRestLen > 0)
	{
		if (nRestLen <= m_nRecvBufLen)
		{
			char TmpBuf[m_nRecvBufLen];
			memcpy(TmpBuf, pCurPos, nRestLen);
			memset(m_newRecvBuf, 0, m_nRecvBufLen);
			memcpy(m_newRecvBuf, TmpBuf, nRestLen);
		}
		else
		{
			nRet = -1;
		}
	}
	else
	{
		ReSetRecvParameter();
	}
	return nRet;
}

int CMyHttpRecvAndSendTask::GetHttpHead()
{
	int nRet=0;
	int needRecv = m_nHttpHeadLen - m_nHaveRecvLen;
	if (needRecv <= 0)
	{   
		WriteRunInfo::WriteLog("The httphead is too large");
		return -1;
	}

	int iReced = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf + m_nHaveRecvLen, needRecv, MSG_DONTWAIT);

	WriteRunInfo::WriteTrace("HEAD : Http Recv m_nHaveRecvLen = %d", m_nHaveRecvLen);
	WriteRunInfo::WriteTrace("HEAD : Http Recv Len = %d", iReced);
	nRet = -1;
	do
	{
		//Recv socket close=======
		if (iReced == 0)
		{
			WriteRunInfo::WriteTrace("the httpclient close the socket %d", m_pTcpSocket->GetSocket());
			break;
		}

		//Recv error==============
		if (iReced < 0)
		{
			if (errno != EINTR && errno != EAGAIN)
			{
				WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
			}
			else
			{
				nRet = 0;
			}
			break;
		}

		//Recv succeed===========
		m_nHaveRecvLen += iReced;

		char *pTmp = strstr(m_newRecvBuf, "\r\n\r\n");
		if (pTmp == NULL)
		{
			nRet = 0;
			break;
		}

		//Get Http Head 
		m_strHttpHead = string(m_newRecvBuf, pTmp - m_newRecvBuf + 4);
		WriteRunInfo::WriteTrace("%s", m_strHttpHead.c_str());

		//Get Http Head Method
		char* pMethodTmp = strstr(m_newRecvBuf, " ");
		if (NULL == pMethodTmp)
		{
			WriteRunInfo::WriteLog("GetHttpHead - There is not Http Head Method ");
			WriteRunInfo::WriteLog("%s", m_strHttpHead.c_str());
			return -1;
		}
		string strMethod(m_newRecvBuf, pMethodTmp - m_newRecvBuf);
		if (0 == strMethod.compare("GET") || 0 == strMethod.compare("HEAD"))
		{
			m_nReqContentLen = 0;
			m_nHaveRecvBodyLen = 0;
		}
		else if (0 == strMethod.compare("POST"))
		{
			string sOut;
			int nTmp;
			//find content-len
			nTmp = CToolKit::GetStringByLabel(m_strHttpHead, "Content-Length:", "\r\n", sOut, 0, 0);
			if (nTmp != 0)
			{
				nTmp = CToolKit::GetStringByLabel(m_strHttpHead, "Content-length:", "\r\n", sOut, 0, 0);
				if (nTmp != 0)
				{
					WriteRunInfo::WriteLog("There is not Content-Length in Http Head ");
					WriteRunInfo::WriteLog("%s", m_strHttpHead.c_str());
					return -1;
				}
			}
			m_nReqContentLen = strtoull(sOut.c_str(), NULL, 10);
			m_nHaveRecvBodyLen = m_nHaveRecvLen - (pTmp - m_newRecvBuf + 4);
		}
		else
		{
			WriteRunInfo::WriteLog("Do not support the method %s", strMethod.c_str());
			WriteRunInfo::WriteLog("%s", m_strHttpHead.c_str());
			return -1;
		}

		if (IfFinishRecv())
		{
			m_RecvState = ReadHttpHead;
		}
		else
		{
			m_RecvState = ReadHttpBody;
		}
		nRet = 0;
	}
	while (0);
	return nRet;
}

int CMyHttpRecvAndSendTask::GetHttpBody()
{
	int nRet = 0;
	unsigned int uiNeedRecvLen = (unsigned int)m_strHttpHead.size() + m_nReqContentLen - m_nHaveRecvLen;
	if (uiNeedRecvLen > m_nRecvBufLen - (unsigned int)m_nHaveRecvLen)
	{
		WriteRunInfo::WriteLog("The httpbody is too large");
		return -1;
	}
	int ret = recv(m_pTcpSocket->GetSocket(), m_newRecvBuf + m_nHaveRecvLen, uiNeedRecvLen, MSG_DONTWAIT);

	nRet = -1;
	do
	{
		if (ret == 0)
		{
			WriteRunInfo::WriteTrace("the httpclient close the socket %d", m_pTcpSocket->GetSocket());
			break;
		}

		if (ret < 0)
		{
			if ( errno != EINTR && errno != EAGAIN )
			{
				WriteRunInfo::WriteLog("socket error , errnor is %d", errno);
			}
			else nRet = 0;
			break;
		}

		m_nHaveRecvLen += ret;
		m_nHaveRecvBodyLen += ret;
		if (IfFinishRecv())
		{
			m_RecvState = ReadHttpHead;
		}
		else
		{
			m_RecvState = ReadHttpBody;
		}
		nRet = 0;
	} while (0);
	return nRet;
}

