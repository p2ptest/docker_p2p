
#include "tcptestclient.h"
#include <vector>
#include <set>
#include <fstream>
using namespace std;

#define HTTP_LIND_END "\r\n"
#define HTTP_HEAD_END "\r\n\r\n"
#define HTTP_COMMON_CONTENT_TYPE "application/json"
//#define HTTP_COMMON_CONTENT_TYPE "application/octet-stream"
//#define HTTP_COMMON_CONNECT_TYPE "Keep-Alive"
#define HTTP_COMMON_CONNECT_TYPE "Close"
//#define HTTP_RETURN_VERSION "HTTP/1.1 200 OK"
#define HTTP_VERSION "HTTP/1.1"

#define GEN_HTTP_POST_REQ(sUrl, sHost, sContentLen, sContent) \
	(string("POST") + " " + sUrl + " " + HTTP_VERSION + HTTP_LIND_END\
	+ "Host: " + sHost + HTTP_LIND_END\
	+ "Content-Length: " + sContentLen + HTTP_LIND_END\
	+ "Content-Type: " + HTTP_COMMON_CONTENT_TYPE + HTTP_LIND_END\
	+ "Connection: " + HTTP_COMMON_CONNECT_TYPE + HTTP_HEAD_END\
	+ sContent);

int main(int argc, char* argv[])
{
	if (argc != 4)
	{
		printf("Usage: %s ip port gcid\n", argv[0]);
		printf("./reonlinecmd 127.0.0.1 8898 0123456789012345678901234567890123456789\n");
		return -1;
	}

	string strSrvIp = argv[1];
	unsigned short nSrvPort = (unsigned short)atoi(argv[2]);
	CTcpTestClient TcpTestClient(strSrvIp, nSrvPort);

	char szContent[10240];
	snprintf(szContent, sizeof(szContent), "{\"iid\":[\"%s\"]}", argv[3]);
	string strReq = GEN_HTTP_POST_REQ("/online", argv[1], ToString(strlen(szContent)), szContent);
	int nRet = TcpTestClient.Send((char*) strReq.c_str(), strReq.length());
	if (nRet != 0)
	{
		printf("send request cmd fail\n");
		return -1;
	}
	cout << strReq << endl;

	int nRecvLen = 10240;
	nRet = TcpTestClient.Recv(&szContent[0], nRecvLen);
	if (nRet != 0)
	{
		printf("receive request cmd fail\n");
		return -1;
	}
	cout << szContent << endl;
	return 0;
}


