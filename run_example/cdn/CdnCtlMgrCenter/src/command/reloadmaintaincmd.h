/*
 * =====================================================================================
 *
 *       Filename:  reloadmaintaincmd.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-12-13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef __RELOADMAINTAINCMD_H_52361430_1249_11E0_A98D_0015C55D3D75_
#define __RELOADMAINTAINCMD_H_52361430_1249_11E0_A98D_0015C55D3D75_
#include "basemaintaincmd.h"

using namespace std;

class CReloadMainTainCmd 
	: public CBaseMaintainCmd
{
public:
	CReloadMainTainCmd(int nMsgType, int nBodyLen, char* pBody)
		: CBaseMaintainCmd(nMsgType , nBodyLen , pBody)
	{
	}
	virtual ~CReloadMainTainCmd(){}
	virtual int Process();
};




#endif


