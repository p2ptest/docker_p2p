/**
 *    @file       dbbackthread.h
 *
 *    @brief      
 *    @details
 *
 *    @version    1.0
 *    @date       08/24/2013 02:28:55 AM
 *
 *    @author      wangtiefeng (), 
 */
#ifndef _DBBACKTHREAD_H_
#define _DBBACKTHREAD_H_
#include "framecommon/framecommon.h"
#include "workthread.h"

using namespace std;
using namespace MYFRAMECOMMON;

class CDbBackThread : public CWorkThread
{
public:
	CDbBackThread(int nEntityType, int nEntityId);
	virtual ~CDbBackThread();

	virtual int Init();
	virtual int Run();
	virtual int PutMsgToInputList(CMsg *pMsg);
private:
	virtual int Process(CMsg *pMsg);
};
#endif


