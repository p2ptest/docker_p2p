
#include "localdomainparser.h"

CLocalDomainParser* CLocalDomainParser::m_pInst = NULL;

CLocalDomainParser* CLocalDomainParser::GetInstance()
{
	if(NULL == m_pInst)
	{
		m_pInst = new CLocalDomainParser;
	}

	return m_pInst;
}

void CLocalDomainParser::DestroyInstance()
{
	if(NULL != m_pInst)
		delete m_pInst;

	m_pInst = NULL;
}

int CLocalDomainParser::ParseDomain(const string &sDomain, string &sIpAddr)
{
	map<string, IPINFO>::const_iterator it;

	time_t now = time(NULL);
	m_lock.GetReadLock();
	it = m_domainList.find(sDomain);
	if (it == m_domainList.end() || now > it->second.iUpdateTime + FIVEMINUTE)
	{
		m_lock.ReleaseLock();
		if (CToolKit::ParseDomain(sDomain, sIpAddr) == 0)
		{
			IPINFO item;
			item.iUpdateTime = now;
			item.sIp = sIpAddr;
			m_lock.GetWriteLock();
			m_domainList[sDomain] = item;
			m_lock.ReleaseLock();
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		sIpAddr = it->second.sIp;
	}
	m_lock.ReleaseLock();	
	
	return 0;
}

