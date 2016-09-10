#pragma once

#ifndef AFC_IM_MESSAGE_HEAD_FILE
#define AFC_IM_MESSAGE_HEAD_FILE


/*
 文件说明：本文件定义了与IM相关的消息和结构体
 整合平台时，需要在GameRoomLogonDT中包含本文件
 文件创建：Fred Huang 2008-04-2
 消息格式：MDM_		表示主消息
 ASS_IM	表示与IM相关子消息
 ASS_IMC	表示由客户端向服务器端发送的消息
 ASS_IMS	表示由服务器端向客户端发送的消息
 结构格式：MSG_		表示是消息结构体
 MSG_IM	与头像相关的消息结构
 MSG_IM_C_ 由客户端发给服务器端的消息结构体
 MSG_IM_S_ 由服务器端发给客户端的消息结构体
 */

//主消息
#define MDM_GP_IM						199					//好友相关


//子消息
#define ASS_IMC_GETCOUNT				0x01					//取得服务器上好友数，陌生人中的不取，需要将本地的数量发给服务器
#define ASS_IMC_GETUSERINFO				0x02					//取得某个好友的详细资料
#define ASS_IMC_ADDREQUEST				0x03					//请求加为好友，含有是否是第一次发出，这在对方需要确认时有用
#define ASS_IMC_ADDREQUESTRESPONSE		0x04					//有人请求加为好友，返回是否同意
#define ASS_IMC_SEARCH					0x05					//查询好友，分为在线与不在线（通过ID号或姓名），保留
#define ASS_IMC_MSG						0x0F					//聊天消息
#define ASS_IMC_SETGROUP				0x06					//设置好友组

#define ASS_IMS_GETCOUNT				0x11					//返回好友数，不取黑名单
#define ASS_IMS_GETUSERINFO				0x12					//返回好友的详细资料,最少要以下内容
//ID号，姓名，性别，是否在线
#define ASS_IMS_GETLIST					0x13					//返回好友列表，包括：ID号，姓名，性别，是否在线
#define ASS_IMS_USERONLINE				0x14					//上下线后，通知好友
#define ASS_IMS_GETONLINELIST			0x15					//上线后，返回在线的好友，如果已取得完整列表，则不再返回此消息
#define ASS_IMS_ADDREQUEST				0x16					//有人请求加为好友，需要请确认
#define ASS_IMS_ADDREQUESTRESPONSE		0x17					//转发是否同意加为好友，如果是同意的，则服务器还要操作记录
#define ASS_IMS_USERNOTONLINE			0x1E					//转发聊天消息失败，用户不在线
#define ASS_IMS_MSG						0x1F					//转发聊天消息
#define ASS_IMS_SEARCH					0x24					//查找好友
#define ASS_IMS_DELETE					0x25					//删除好友


#define ASS_IMS_NOAC					0x23					//转发聊天消息


#define ASS_SET_FRIENDTYPE				0x20					///设置个人好友和聊天的接受类型


#define MAX_BLOCK_MSG_SIZE				512						//每个消息的最大数量

#define IM_ADDRESPONSE_AGREE			0x0						//同意加为好友
#define IM_ADDRESPONSE_REFUSE			0x1						//拒绝加为好友
#define IM_ADDRESPONSE_IDENT			0x2						//需要验证

#define IM_SEARCH_BY_ONLINE				0x0						//查询在线用户
#define IM_SEARCH_BY_NAME				0x1						//通过用户姓名查询
#define IM_SEARCH_BY_SEX				0x2						//通过性别查询

//=======================数据结构定义=====================
/*
 结构：取得服务器上好友数/返回服务器上的好友数
 内容：用户的ID（防止错误，可以不要的），本地好友数
	
 */

typedef struct
{
    int			dwUserID;						//用户ID
    int			dwRemoteUserID;					//好友的ID
    int			groupID;						//组ID
}MSG_IM_C_SETGROUP;

typedef struct
{
    int			dwUserID;						//用户ID
    int			dwUserCount;					//用户的好友数
    int			dwUserCountReturn;				//返回的用户好友数
}MSG_IM_C_COUNT,MSG_IM_S_COUNT;

/*
 结构：请求取某个用户的资料
 内容：请求者的ID和被请求者的ID，以及用户名
 */
typedef struct
{
    int			dwUserID;						//请求者ID
    int			dwRequestedUserID;				//被请求者ID
}MSG_IM_C_GETUSERINFO;

/*
 结构：用户的资料信息
 内容：用户的资料信息
 使用在以下消息中：
 */
typedef struct
{
    int			dwUserID;						//用户ID
    int			dwRemoteID;						//发送接收者的ID
    char		sUserName[30];					//姓名
    bool		nSex;							//性别
    int			nOnline;						//是否在线
    int			GroupID;						//分组ID，暂不使用
}MSG_IM_S_GETUSERINFO;

/*
 结构：请求加为好友
 内容：请求者的ID，被请求者的ID，如果对方需要验证，则发第二次请求，复用此结构体，因此，要包含是否是第一次发送，以及请求内容
 注	：现在只支持添加在线的用户
 */
typedef struct
{
    int			dwUserID;						//请求者用户ID
    
    char		sUserName[61];					//请求者姓名  	//change by yjj 090323
    int			dwRequestedUserID;				//被请求用户ID
    char		sRequestedUserName[61];			//请求者姓名  	//change by yjj 090323
    int			nMsgLength;						//请求消息的长度
    int			cbSize;							//整个消息的长度
    char		sRequestNotes[128];				//请求的内容，实际长度与nMsgLength相关
}MSG_IM_C_ADDREQUEST,MSG_IM_S_ADDREQUEST;

/*
 结构：是否同意加为好友
 */
typedef struct
{
    int			dwUserID;						//还是请求者的用户ID
    int			dwRequestedUserID;				//被请求者（即回应用户）的ID
    int			dwRefusedUserID;				//请求后被拒绝的用户id
    char		sRequestedUserName[61];			//被请求者（即回应用户）的姓名//change by yjj 090323
    char        sRequirUsrName[61]  ;           //请求者的姓名  add by yjj 090323
    int			nResponse;						//回应方式：IDYES-同意,IDNO-不同意
    int			nSex;							//回应者的性别
}MSG_IM_C_ADDRESPONSE,MSG_IM_S_ADDRESPONSE;

/*
 结构：查询用户
 内容：可以是查询在线用户，也可以是通过用户姓名查询
 注意：不能通过ID号查询，因为ID号对用户而言是不透明的
 返回结果是MSG_IM_S_GETUSERINFO
 */
typedef struct
{
    int			dwUserID;						//请求查询的用户ID
    int			nSearchType;					//查询的方式，现只支持单项查询，不支持组合查询
    int			nSex;							//只有在nSearchType==IM_SEARCH_BY_SEX时有效
    char		sSearcHSame[30];				//只有在nSearchType==IM_SEZRCH_BY_NAME时有效
}MSG_IM_C_SEARCHUSER;

/*
 结构：返回用户列表，即多个用户信息
 */
typedef struct
{
    int			nListCount;						//此消息体中包含的用户列表数
    int			nBodyLength;					//此消息体中总共的数据长度
    char		sBody[MAX_BLOCK_MSG_SIZE];		//消息体，多个MSG_IM_S_GETUSERINFO
}MSG_IM_S_USERLIST;

/*
 结构：即时消息
 内容：要包括文字信息
 */
typedef struct
{
    int			dwUserID;						//用户的ID号
    char		szUserName[30];					//用户的姓名
    int			dwRemoteUserID;					//对方的ID号
    //CHARFORMAT	cf;								//消息格式
    char		szFontname[50];					//字体名称，CHARFORMAT中可能会不保留这个信息，因为它是一个指针
    int			szMsgLength;					//消息长度
    int			cbSize;							//本条消息总长度
    char		szMsg[MAX_BLOCK_MSG_SIZE];		//消息体
}MSG_IM_C_MSG,MSG_IM_S_MSG;


//查找用户
typedef struct
{
    long dwUserID;
    long dwRemoteID;
}MSG_IM_C_SEARCH,MSG_IM_S_SEARCH;

//删除用户
typedef struct
{
    long dwUserID;
    long dwRemoteID;
}MSG_IM_C_DELETE,MSG_IM_S_DELETE;

//查找结果
typedef struct
{
    long dwUserID;
    long dwRemoteID;
    int	 nHeadType;
    int  nVipLevel;
    int  nDiamond;
    int  nOnLineFlag;
    int	 nSex;
    int  nResult;
    char sNickName[50];
}MSG_IM_C_SEARCH_RESULT;

//删除结果
typedef struct
{
    long dwUserID;
    long dwRemoteID;
    int	 nResult;
}MSG_IM_C_DELETE_RESULT;

//获取玩家信息的应答根据ID   add by hou_wy 2016-4-19
typedef struct
{
    long  dwUserID;
}MSG_GP_QueryUserInfoByUid;
typedef struct
{
    int                                 nRet;                           //返回值， 0表示成功
    long 							dwUserID;							///用户 ID
    LLONG								i64Money;							//用户金币
    int									iDiamond;							//钻石数量
    UINT								bLogoID;							///用户头像
    bool								bBoy;								///性别
    char								szNickName[20];						///昵称
    char								szSignDescr[128];					///个性签名
}MSG_GP_QueryUserInfoByUidResult;
#endif