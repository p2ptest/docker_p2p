

#include "mastersrvdef.h"


int GetRemoteIp(int nSocket)
{
	sockaddr_in remoteAddr;
	socklen_t remoteLen = sizeof(remoteAddr);

	getpeername(nSocket, (struct sockaddr *)&remoteAddr, &remoteLen);
	remoteAddr.sin_family = AF_INET;
	return remoteAddr.sin_addr.s_addr;
}
