/*
* Copyright (c) 2008 暴风网际
* All rights reserved.
* 
* 文件名称: protocol.h
* 文件标识:
* 摘 要: 定义p2p下载的协议
* 当前版本: 1.0
* 作 者: chen.qian.jiang
* 开始日期: 2008-03-16
* 完成日期: 
* 其它说明： 
* 修改日期 版本号 修改人 修改内容
* -----------------------------------------------
* 2008/03/16 V1.0 陈黔江 创建
* 2008/03/18 V1.1 张周 增加peer类型的宏定义; 修改"打孔"部分的协议
*/

#ifndef _PROTOCOL__H
#define _PROTOCOL__H
#include <string>

#ifdef _WIN32
	#pragma warning(disable:4200)
#endif

#pragma pack(push, 1)

#define TCP_HTTP_ASCII_HEAD		 "GET /abcd HTTP/1.1\r\n"
#define TCP_HTTP_ASCII_HEAD_SIZE strlen(TCP_HTTP_ASCII_HEAD)

#define VOD_PROTOCOL_VER      0x0103
#define VOD_PRE_PROTOCOL_VER  0x0102
#define VOD_MINI_PROTOCOL_VER 0x0101

#define CRCCHECK_OFFSET       59
#define CRCCHECK_SIZE         20//(8+4+4+4)
#define NEW_CLIENT_VOD_PROTOCOL_VER		0x65

struct base_head
{
	int protocol_ver;		//协议版本
	int seq;				//包序号
	int command_len;		//命令长度
};

typedef struct request_head_t:public base_head
{
	short command_id;		//命令id
}common_head_t;

struct response_head_t
{
	int protocol_ver;	   //协议版本
	int seq;			       //包序号
	int command_len;	   //命令长度
	short command_id;	   //命令id
	char succ;			     //是否成功
};

enum
{
	PEER_TYPE_NP,
	PEER_TYPE_HVP = 2,
	PEER_TYPE_CVP,
	PEER_TYPE_MIF
};

#define 	CONNECT_DIRECT		    1	  //直接连接
#define 	CONNECT_HOLE_REQUEST    2	  //打洞连接
#define 	CONNECT_INITINATIVE     3     //主动连接
#define 	CONNECT_PASSIVITY	    4	  //被动连接

struct peer_info
{
	std::string peer_id;
	unsigned int internal_ip;
	unsigned int protocal_ver;
	short tcp_port;
	short udp_port;
	int p2p_attribute;
	int peerpriority;
	int downscale;
	char peertype;
	char connecttype; 
	char connectpassivity;

	peer_info()
	{
		internal_ip = 0;
		tcp_port = 0;
		udp_port = 0;
		p2p_attribute = 0;
		peerpriority = 0;
		downscale = 0;
		peertype = 0;
		connecttype = 0;
		connectpassivity = 0;
	}

	void operator =(const peer_info &peerinfo) 
	{
		peer_id = peerinfo.peer_id;
		internal_ip = peerinfo.internal_ip;
		tcp_port = peerinfo.tcp_port;
		udp_port = peerinfo.udp_port;
		p2p_attribute = peerinfo.p2p_attribute;
		peerpriority = peerinfo.peerpriority;
		downscale = peerinfo.downscale;
		peertype = peerinfo.peertype;
		connectpassivity = peerinfo.connectpassivity;
		connecttype = peerinfo.connecttype;
	}

	bool operator==(const peer_info &item)
	{
		if (peer_id != item.peer_id)
		{
			return false;
		}

		if (tcp_port != item.tcp_port)
		{
			return false;
		}

		if (udp_port != item.udp_port)
		{
			return false;
		}
		
		return true;
	}

};

struct play_item 
{
	play_item(std::string cid, std::string name)
	{
		s_cid = cid;
		s_name = name;
	}
	std::string s_cid;
	std::string s_name;
};
// 信息码 add：
#define ON_RESULT_OK 0
#define ON_RESULT_FAILE 1


// P2pattribute属性:
#define PEER_IN_NAT 1  // 内网
#define PEER_NAT    2  // 支持穿透
#define PEER_IN_SAME_NAT 4// 同一个内网

///////////////////////////////////
//seed服务器协议
#define SEED_LOGIN			1000
#define SEED_LOGIN_RES		1001
#define SEED_REPORT_RC		1002
#define SEED_REPORT_RC_RES	1003
#define SEED_INSERT_RC		1004
#define SEED_INSERT_RC_RES	1005
#define SEED_DELETE_RC		1006
#define SEED_DELETE_RC_RES	1007
#define SEED_GETPEER		1008
#define SEED_GETPEER_RES	1009
#define	SEED_INVALID_PEER	1010

//online服务器协议
#define ONLINE_PING			2000
#define ONLINE_PING_RES		2001
#define ONLINE_LOGOUT		2002
#define ONLINE_MONITOR		2100
#define ONLINE_MONITORRES	2101

//online与seed服务器交互协议
#define SYN_ONLINE			3000
#define SYN_LOGOUT			3002
#define SYN_ONTIME			3004
#define SYN_AFFIRM			3006
#define SYN_AFFIRM_RES		3007
#define SYN_HANDSHAKE		3008

//pcserver服务器协议
#define PC_REQUEST_NEIGHBOR		5000
#define PC_REQUEST_NEIGHBOR_RES	5001
#define PC_REPORT_NEIGHBOR		5002
#define PC_REPORT_NEIGHBOR_RES	5003
#define PC_INQUIRE_NEIGHBOR		5004
#define PC_INQUIRE_NEIGHBOR_RES 5005
#define PC_BROADCAST_MSG		5006
#define PC_REQUEST_PUBLICKEY		5008
#define PC_REQUEST_PUBLICKEY_RES	5009
#define PC_BROADCAST_CTL_MSG		5010
#define PC_BROADCAST_CTL_MSG_RES	5011

//NetBroadcast p2p
#define NB_P2P_PING				5100				
#define NB_P2P_PING_RES			5101
#define NB_P2P_BROADCAST_MSG    5102

//Message Id
#define CONNECT_SERVER_COMMON 2000
#define ONLINE_SYN	2001		//wangtiefeng : 2009-01-10
#define CACHE_GET_DATA 12000
#define CACHE_GET_DATA_RES 12001
#define CACHE_INTEREST 12002
#define CACHE_INTEREST_RES 12003

//连上后
#define PEER_PRE_HANDSHAKE	3000
#define PEER_PRE_HANDSHAKE_RES 3001


/***************************************/
//错误码
#define E_OK 		    100
#define E_CLIENTID      101
#define E_CLIENTID_LEN  102
#define E_UDPPORT       103
#define E_UDPPORT_LEN   104
#define E_DNSLIST_LEN   105
#define E_ROUTERLIST_LEN 106
#define E_REPORTPOS      107
#define E_REPORTPOS_LEN  108
#define E_GETREPORTPARENT 109
#define E_BITMAP_NUM     110
#define E_BITMAP_LEN     111
#define E_CHID 		112
#define E_CHID_LEN      113
#define E_BLKSZIE 	114
#define E_PEER 		115 
#define E_NOCIS 	116
#define E_PWD 		117
#define E_CISID 	118
#define E_VERSION 	119
#define E_VPEERID 	120
#define E_CHN 		121
#define E_NOPEER 	122
#define E_SEEK_HEARTBEAT_PARENT 123
#define E_UNDEFINE      124
#define E_INCOMPLETE_INFO 125
#define E_FATAL         126
#define E_FEC_MISSED     127
#define E_DNSINDEX_LEN   128
#define E_DNSNUM_LEN     129
#define E_STATE_LEN      130
#define E_PEERLIST_LEN   131
#define E_ROUTERINDEX_LEN 132
#define E_SYSTEM_BUSY	  133
#define E_FECPACKETHEAD   134
#define E_FECPACKET_LEN   135
#define E_CHANNELNUM      136
#define E_NOCHNFILE 		121


//peer类型
#define PEERTYPE_NP 	0 //normal peer (nat peer)
#define PEERTYPE_VP 	2 //virtual peer
#define PEERTYPE_HTTP 	3 //http peer


//********** index 30 - 39************
//use tcp
#define c_s_index_peerlogin 			30 // peer 登录到index 
	#define c_s_index_peerlogin_clientid 	0 //8 byte
	#define c_s_index_peerlogin_dnsip 		1 //变长 
	#define c_s_index_peerlogin_routerip 	2 //变长

#define s_c_index_peerlogin 					31 // peer登录回应 
	#define s_c_index_peerlogin_clientid 		0 //8byte
	#define s_c_index_peerlogin_dns_index 		1 //变长
	#define s_c_index_peerlogin_router_index    2 //变长
	#define s_c_index_peerlogin_publicip 		3 //4 bytes
	#define s_c_index_peerlogin_status 			4 //4 bytes
	#define s_c_index_peerlogin_vpeer_regionno	5 //4 bytes


//************ cis 60 - 79 **************
//use tcp
#define c_s_cis_getclientid 				68 //optional
	#define c_s_cis_getclientid_clientid		0 // 8byte 

#define s_c_cis_getclientid 				61 //optional
	#define s_c_cis_getclientid_clientid 		0 // 8byte
	#define s_c_cis_getclientid_publicip 		1 // 4byte
	#define s_c_cis_getclientid_status          2 // 4byte 

//use tcp
#define c_s_cis_getheartbeatparent 			62   //现在不插入，只new出公共对象
	#define c_s_cis_getheartbeatparent_clientid     0 // 8byte  // 开始new对象 
   	#define c_s_s_cis_getheartbeatparent_port       1 // 2byte 
	#define c_s_cis_getheartbeatparent_dns_index 	2 // 变长 
	#define c_s_cis_getheartbeatparent_router_index	3 // 变长 
	#define c_s_cis_getheartbeatparent_posindex 	4 //4byte，为-1表示第一次取


#define s_c_cis_getheartbeatparent 			63
	#define s_c_cis_getheartbeatparent_peerinfo 	0 //报活父亲的信息
	#define s_c_cis_getheartbeatparent_posindex 	1 //4 bytes，绝对位置值  // 换父亲 
	#define s_c_cis_getheartbeatparent_len 			2 //2 bytes，位图长度
	#define s_c_cis_getheartbeatparent_layer		3 //1 byte，层数
	#define s_c_cis_getheartbeatparent_status 		4 // 4byte

#define c_s_cis_heartbeat 			        64    //插入各个频道下的dns和路由表
	#define c_s_cis_heartbeat_clientid 	        0
	#define c_s_cis_heartbeat_bitmap_num        1 // 2byte
	#define c_s_cis_heartbeat_bitmap 			2 // 变长 
	#define c_s_cis_heartbeat_posindex			3 // 4 bytes，绝对位置值
	#define c_s_cis_heartbeat_channelid         4 // 变长,（sizeof（report_t）*（bitmap中唯一的位的个数）） 
	#define c_s_cis_heartbeat_status_num		5 // 为统计NP 数而报的人数  4 byte chnid + 4 byte Watch_NP_num + 4 bytes Share_NP_num ... 
	#define c_s_cis_heartbeat_np_num			6 // 4byte 


#define s_c_cis_heartbeat				65  
	#define s_c_cis_heartbeat_clientid		0
	#define s_c_cis_heartbeat_status		1   

#define c_s_heartbeat_handshake					66//报活握手
	#define c_s_heartbeat_handshake_clientid	0
	#define c_s_heartbeat_handshake_posindex	1	// 4 bytes，绝对位置值
	#define c_s_heartbeat_handshake_layer		2	//1 byte

#define s_c_heartbeat_handshake					67
	#define s_c_heartbeat_handshake_clientid	0
	#define s_c_heartbeat_handshake_status		1	// 4byte

struct report_t
{
	short m_nChannelNum;
	char * m_pChannel;
};

struct reportnp_t
{
	int m_iChannelId;
	int m_iWatchNpNum;
	int m_iShareNpNum;
};


#define c_s_cis_getpeer 			66 ///peer登录cis 
    #define c_s_cis_getpeer_chnid 			0 // 4byte
	#define c_s_cis_getpeer_clientid 		1 // 8byte
	#define c_s_cis_getpeer_num 			2 // 2bytes,要求返回的peer的最大个数 
	#define c_s_cis_getpeer_dns_index 		3 // 变长 
	#define c_s_cis_getpeer_router_index 	4 // 变长 
	#define c_s_cis_getpeer_posindex        5
	#define c_s_cis_getpeer_vpeer_regionno	6 // 4bytes 

#define s_c_cis_getpeer 			67 //peer登录回应
    #define s_c_cis_getpeer_chnid			0 // 4byte
	#define s_c_cis_getpeer_clientid 		1 // 8byte
	#define s_c_cis_getpeer_num 			2 // 2bytes,要求返回的peer的最大个数 
	#define s_c_cis_getpeer_peerlist 	    3 // 非定长，见下面struct peerlist_t  
	#define s_c_cis_getpeer_clientlimen     4 //2byte
	#define s_c_cis_getpeer_status 			5 // 4byte

struct peerlist_t
{
	unsigned long long clientid;
	unsigned int publicip;
	short port;
	unsigned char type; // PEERTYPE_NP,PEERTYPE_VP  
};

//////////////////////////////////////////////////////////////////////////

//常连接
#define c_s_cis_vpeerlogin 			70	//vpeer登录到cis
	#define c_s_cis_vpeerlogin_id 			0 // 8字节字符串 
	#define c_s_cis_vpeerlogin_pwd 			1 // 8字节字符串  
	#define c_s_cis_vpeerlogin_chnid 		2 // 4byte chnid + 4byte chnid... 
	#define c_s_cis_vpeerlogin_ifhotvpeer	3 //1 byte 标示vpeer服务器是否为冷vp或热vp,1:热0:冷


#define s_c_cis_vpeerlogin 			71 //vpeer登录回应
	#define s_c_cis_vpeerlogin_id 		0 // 非定长 
	#define s_c_cis_vpeerlogin_status 	1 // 4byte 

#define c_s_cis_vpeerping 			72 //vpeer到cis的ping
	#define c_s_cis_vpeerping_id 		0 // 非定长 
	#define c_s_cis_vpeerping_add_chnid 	1 // 4byte chnid + 4byte chnid... 
	#define c_s_cis_vpeerping_del_chnid     2 // 4byte chnid + 4byte chnid... 
	#define c_s_cis_vpeerping_status_num	3 // 为统计NP 数而报的人数  4 byte chnid + 4 byte Watch_NP_num + 4 bytes Share_NP_num ... 
	#define c_s_cis_vpeerping_getdata_num	4
	#define c_s_cis_vpeerping_num 			5 // 为负载均衡而报的在线人数 4byte 
	#define c_s_cis_vpeerping_ifhotvpeer    6
	#define c_s_cis_vpeerping_numofchn		7 //4byte频道号+4byte人数...变长

#define s_c_cis_vpeerping 			73  //vpeer的ping回应
	#define s_c_cis_vpeerping_id 		0 // 非定长
	#define s_c_cis_vpeerping_status 	1 // 4byte  


//use tcp
#define c_s_cis_getmorepeer_by_dns                 74
	#define c_s_cis_getmorepeer_by_dns_id				0
	#define c_s_cis_getmorepeer_by_dns_chnid			1
	#define c_s_cis_getmorepeer_by_dns_index			2
	#define c_s_cis_getmorepeer_by_dns_num				3

#define s_c_cis_getmorepeer_by_dns                 75
  #define s_c_cis_getmorepeer_by_dns_id					0
  #define s_c_cis_getmorepeer_by_dns_chnid				1
  #define s_c_cis_getmorepeer_by_dns_dns_index          2
  #define s_c_cis_getmorepeer_by_dns_peerlist           3  
  #define s_c_cis_getmorepeer_by_dns_state              4

#define c_s_cis_getmorepeer_by_router              76
  #define c_s_cis_getmorepeer_by_router_id				0
  #define c_s_cis_getmorepeer_by_router_chnid           1
  #define c_s_cis_getmorepeer_by_router_index           2
  #define c_s_cis_getmorepeer_by_router_num             3
  
#define s_c_cis_getmorepeer_by_router              77
  #define s_c_cis_getmorepeer_by_route_id				0
  #define s_c_cis_getmorepeer_by_route_chnid            1
  #define s_c_cis_getmorepeer_by_route_dns_index        2
  #define s_c_cis_getmorepeer_by_route_peerlist         3 
  #define s_c_cis_getmorepeer_by_router_state           4
  
#define c_s_cis_cisping					78
  #define c_s_cis_cisping_id                  0

#define s_c_cis_cisping					79
  #define s_c_cis_cisping_id			      0
  #define s_c_cis_cisping_state               1



// peerinfo 
struct peer_t						// peer_info 
{
	unsigned long long	clientid;		// 客户端ID	 8byte 
	unsigned int 		natip;			// 内网ip 
	unsigned short 		natport;		// 内网port 
	unsigned char		peertype;		// peer类型: PEERTYPE_NP,PEERTYPE_VP 或PEERTYPE_SP  
};

struct seekable_info_table_t {
	unsigned int len;
	unsigned int * table;
};

struct general_info_t
{
	char nType; //0-wmv; 1-rmvb,...
	unsigned int nPlayTime; //in seconds;
	unsigned char nVNameLen;
	unsigned int nFileLen;
	char szVName[0];
};

//peerinfo 
struct peer_info_t					// peer_info 
{
	std::string         clientid;		// 客户端ID	 8byte  
	unsigned int		publicip;		// 公网IP	 4byte  
	unsigned short		publicport;		// 侦听端口	 2byte 
	unsigned int 		natip;
	unsigned short 		natport;
	unsigned char		peertype;		// peer类型	 1byte   
};

// peer 与Peer 之间的通迅协议消息号：40 - 46
#define	c_s_get_media_info			    40 // 2byte请求媒体信息消息
#define	c_s_get_media_info_channelid	    // string
#define	c_s_get_media_info_clientid		    // string 
         
#define	s_c_get_media_info			    41// 2byte媒体信息返回消息
#define	s_c_get_media_info_channelid	// string
#define	s_c_get_media_info_clientid	    // string
#define s_c_get_media_info_filesize	    // 4byte

#define c_c_peer_peerfilemap 		    42// 2byte请求/返回位图消息 
#define c_c_peer_peerfilemap_channelid	// 4byte原样返回
#define c_c_peer_peerfilemap_clientid	// string 
#define c_c_peer_peerfilemap_mapsize    // 4byte
#define c_c_peer_peerfilemap_filemap 	// 不定长 map

#define c_c_peer_peerfilemap_ack 	   43// 2byte请求/返回位图消息 
#define c_c_peer_peerfilemap_channelid	// 4byte原样返回
#define c_c_peer_peerfilemap_clientid	// string
#define c_c_peer_peerfilemap_mapsize    // 4byte
#define c_c_peer_peerfilemap_filemap 	// 不定长 map

#define c_s_peer_peerdata 			    44// 2byte请求数据block消息
#define c_s_peer_peerdata_channelid	    // string
#define c_s_peer_peerdata_clientid	    // string 
#define c_s_peer_peerdata_needmap 	    // 定长 4byte(offset)+4byte(len)... 

#define s_c_peer_peerdata 			    45// 2byte数据block返回消息 
#define s_c_peer_peerdata_channelid	    // string
#define s_c_peer_peerdata_clientid	    // string 
#define s_c_peer_peerdata_datasize      // 4bytes
#define s_c_peer_peerdata_data 		    // 4byte(offset)+(4byte)len+data...
#define s_c_peer_peerdata_status 	    // 4byte 

#define c_c_peer_peerhave 			    46// 2byte获得新数据block后向其它Peer发送的通知消息 
#define c_c_peer_peerhave_chid 		    // string
#define c_c_peer_peerhave_clientid	    // string 
#define c_c_peer_peerhave_newblkid 	    // blkid(4byte)

#define c_c_peer_datacancel             47// 2byte取消请求数据block消息
#define c_c_peer_datacancel_chid 		// string
#define c_c_peer_datacancel_clientid	// string  
#define c_c_peer_datacancel_cancelblkid // blkid(4byte)

#define c_c_get_media_info	            48//2byte向peer请求请求媒体信息消息

#define c_c_get_media_info_respond      49//2byte Peer向Peer回复媒体信息

//////////////////////////////////////////////////////////////////////////
// 打孔 cmid

// 向natcenter
#define nat_land_request  				4000 // 登陆请求
#define nat_land_confirm				4001 // 登陆回应

// 向supernode（nbs）
#define nat_heartbeat_request			4002 // 向nbs发送心跳包
#define nat_heartbeat_confirm			4003 // 包回应，只在错误时回包

#define nat_direction_request			4004 // 直连请求
#define nat_direction_confirm			4005 // 直连成功回应

// 向natcenter
#define nat_center_request				4006 // 向natcenter查询peer的supernode
#define nat_center_confirm				4007 // 回应

// 向supernode（nbs） 4008 4009不用
#define nat_supernode_request			4008 // 向supernode发送穿透请求
#define nat_supernode_confirm			4009 // 包回应

#define nat_topeer_send					4010 // 向peer发传透信息
#define nat_topeer_confirm				4011 // 信息回应

#define nat_hole_connect_request		4012 // 进行网络穿透
#define nat_hole_connect_confirm		4013 // 信息回应

#define nat_quest_peerinfo_request      4014 // 向supernode请求peer信息
#define nat_quest_peerinfo_confirm      4015 // 回应

#define nat_main_handshke				4016 // 主动方定时发送，不做处理，无意义

#define nat_center_requestlist			4100 // 向natcenter服务器请求supernode列表
#define nat_center_requestlist_res		4101 // natcenter返回supernode列表


//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//用于udp网络层打包、解包的命令
#define udp_nw_layer_cmd 					2000
	#define udp_nw_layer_cmd_main_id 			0	//8 byte
	#define udp_nw_layer_cmd_sub_id 			1	//2 byte
	#define udp_nw_layer_cmd_blk_count 			2	//2 byte
	#define udp_nw_layer_cmd_rddc_count 		3	//2 byte
	#define udp_nw_layer_cmd_reliable 			4	//1 byte 0非可靠 1可靠 
	#define udp_nw_layer_cmd_rddc_rate			5	//1byte
	#define udp_nw_layer_cmd_data				6	// 变长 数据 
	#define udp_nw_layer_cmd_main_all			7
#define udp_nw_layer_ack 						2001
	#define udp_nw_layer_ack_main_id 					0 //8 byte
	#define udp_nw_layer_ack_sub_status 				1 //4 byte
	#define udp_nw_layer_ack_sub_missed_sub_id_list	 	2 //2byte+2byte...sub_id

#define udp_nw_layer_stat_control 						2002
	#define udp_nw_layer_stat_control_status 				0 //4 byte
	#define udp_nw_layer_stat_control_total_blk_count 		1 //2 byte
	#define udp_nw_layer_stat_control_received_blk_count 	2 //2 byte
//////////////////////////////////////////////////////////////////////////

 struct head_t
 {
 	unsigned int length;		//指后面的数据长度
 	unsigned char flag;			//0:打孔 1:p2p 2:报活;3:冗余包;4:cis相关的;5:ins相关
 	unsigned short type;
 	unsigned short version;
     unsigned int cmdno;
 	unsigned char cmdcount;
 };
#define SUB_CMD_HEAD_LEN 5 //by wangtao: sizeof(short type) + sizeof (int length) 
//子命令
struct sub_cmd_t
{
	unsigned char type;
	unsigned int length;
	char cmdbuf[0]; 	
};



#pragma pack(pop)


#endif // !_SERVERPROC__H  
