/**
 *    @file       dbbackthread.cpp
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       08/24/2013 02:28:59 AM
 *
 *    @author      wangtiefeng (), 
 */
#include "dbbackthread.h"
#include "dbbackmgr.h"

CDbBackThread::CDbBackThread(int nEntityType, int nEntityId)
	: CWorkThread(nEntityType, nEntityId)
{

}

CDbBackThread::~CDbBackThread()
{

}

int CDbBackThread::Init()
{
	return 0;
}


int CDbBackThread::Run()
{
	while (m_nRunFlag)
	{
		CDbBackMgr::GetInstance()->SynToBackupDb();
		usleep(10000);
	}
	return 0;
}

int CDbBackThread::PutMsgToInputList(CMsg *pMsg)
{
	return 0;
}

int CDbBackThread::Process(CMsg *pMsg)
{
	return 0;
}
