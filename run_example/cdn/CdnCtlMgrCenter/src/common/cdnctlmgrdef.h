/*
 * =====================================================================================
 *
 *       Filename:  cdnctlmgrdef.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2010-10-03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  zhangguoqi, zhangguoqi@baofeng.com
 *        Company:  Baofeng, Inc.
 *
 * =====================================================================================
 */

#ifndef _CDNCTLMGRDEF_H_DD614B48_1248_11E0_89F3_0015C55D3D75_
#define _CDNCTLMGRDEF_H_DD614B48_1248_11E0_89F3_0015C55D3D75_

const int GETPARENTCDN_CMD = 100;
const int GETPARENTCDN_CMD_RES = 101;

const int GETPARENTSCDN_CMD = 102;				//add by yinhaichen 2011-03-29
const int GETPARENTSCDN_CMD_RES = 103;

const int GETCDNSRVINFO_CMD = 104;				//add by yinhaichen 2011-03-18
const int GETCDNSRVINFO_CMD_RES = 105;



const int GETNETRELATION_CMD = 106;				//modified by hezhipeng 2012-04-01
const int GETNETRELATION_CMD_RES = 107;

const int NEWGETCDNSRVINFO_CMD = 108;			//added by hezhipeng 2012-04-01
const int NEWGETCDNSRVINFO_CMD_RES = 109;

const int _3RDGETCDNSRVINFO_CMD = 110;			//added by chenhui 2017-05-05
const int _3RDGETCDNSRVINFO_CMD_RES = 111;

const int _2NDGETNETRELATION_CMD = 112;			//added by chenhui 2017-05-05
const int _2NDGETNETRELATION_CMD_RES = 113;

const int RELOADDB_MAINCMD	= 10;

const int CDNCTLMGRSRV_LOG = 10;				//cdn控制管理服务器的特殊日志类型
const int CMDPROCESS_LOG = 11;					//命令响应及处理日志

#endif

