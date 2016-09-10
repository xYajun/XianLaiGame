#ifndef _YZ_NetMessageHall_H_
#define _YZ_NetMessageHall_H_

#include "YZBaseType.h"
#include "YZComStruct.h"

//////////////////////////////////////////////////////////////////////////
///常量定义
#define MAX_PEOPLE				180				///最大游戏人数 百家乐 modify 8 -> 180 by wlr 20090714
#define MAX_SEND_SIZE			2044			///最大消息包
#define MAX_TALK_LEN			500				///最大聊天数据长度
#define NORMAL_TALK_LEN			200				///普通聊天数据长度

//////////////////////////////////////////////////////////////////////////
//游戏通讯指令宏定义

//////////////////////////////////////////////////////////////////////////
// 客户端到服务端心跳指令
#define	MDM_CONNECT				1
///辅助处理消息标志
#define ASS_NET_TEST					1			///网络测试
#define ASS_CONNECT_SUCCESS 			3			///连接成功

// 请求中心服务器
#define	MDM_GP_REQURE_GAME_PARA	102
//////////////////////////////////////////////////////////////////////////

/***********************注册通信主标识*********************************************************************/
#define	MDM_GP_REGISTER										99
//*********************************************************************************************************/
#define ASS_GP_VISITOR_REGISTER								1		 //游客请求注册
#define ASS_GP_VISITOR_BIND									2		 //游客请求转正
#define ASS_GP_USER_REGISTER								3		 //用户请求注册
#define ASS_GP_VISITOR_REGISTER_RESULT						4		 //游客注册结果(0为失败)
#define ASS_GP_VISITOR_BIND_RESULT							5		 //游客转正结果
#define ASS_GP_USER_REGISTER_RESULT							6		 //用户请求结果(0为失败)
#define ASS_GP_USER_NAME_VERIFICTAION						7		 //用户请求验证账号
#define ASS_GP_USER_NAME_VERIFICTAION_RESULT				8		 //用户验证账号(0为成功 -1失败)


#define			ERR_REGISTER_ERROR				0			// 注册失败
#define			ERR_REGISTER_SUCCESS			1			// 注册成功
#define			ERR_REGISTER_NAME_EXIST			2			// 用户名存在


// 大厅主标识
#define MDM_GP_CONNECT					1			// 连接消息类型
#define MDM_GP_LOGONUSERS	 			119			// 统计登录人数


//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

// 支付通知主ID和，辅助ID标志
#define MDM_GP_NOTIFY_PAY				88			//
#define		ASS_GP_NOTIFY_PAY				1			//

//////////////////////////////////////////////////////////////////////////

//游戏主标识
#define MDM_GM_GAME_FRAME				150			//框架消息
#define MDM_GM_MESSAGE					151			//信息消息


//大厅辅助处理消息标志
#define		ASS_GP_NET_TEST					1			//网络测试
#define		ASS_GP_CONNECT_SUCCESS 			3			//连接成功

//////////////////////////////////////////////////////////////////////////
//大厅登陆
#define	MDM_GP_LOGON					100			
#define		ASS_GP_LOGON_BY_NAME			1			//通过用户名字登陆
#define		ASS_GP_LOGON_BY_ACC				2			//通过用户ACC 登陆
#define		ASS_GP_LOGON_BY_MOBILE			3			//通过用户手机登陆
#define		ASS_GP_LOGON_REG				4			//用户注册
#define		ASS_GP_LOGON_SUCCESS			5			//登陆成功
#define		ASS_GP_LOGON_ERROR				6			//登陆失败
#define		ASS_GP_LOGON_ALLO_PART			7			//异地登陆
#define		ASS_GP_LOGON_LOCK_VALID			8			//锁机验证
#define		ASS_GP_LOGON_BY_SOFTWARE        10
#define		ASS_GP_LOGON_MOBILE_VALID		11			//手机验证

/// 错误代码
#define			ERR_GP_ERROR_UNKNOW				0		///未知错误
#define			ERR_GP_LOGON_SUCCESS			1		///登陆成功
#define			ERR_GP_USER_NO_FIND				2		///登陆名字错误
#define			ERR_GP_USER_PASS_ERROR			3		///用户密码错误
#define			ERR_GP_USER_VALIDATA			4		///用户帐号禁用
#define			ERR_GP_USER_IP_LIMITED			5		///登陆 IP 禁止
#define			ERR_GP_USER_EXIST				6		///用户已经存在
#define			ERR_GP_PASS_LIMITED				7		///密码禁止效验
#define			ERR_GP_IP_NO_ORDER				8		///不是指定地址 
#define			ERR_GP_LIST_PART				9		///部分游戏列表
#define			ERR_GP_LIST_FINISH				10		///全部游戏列表
#define			ERR_GP_USER_LOGON				11		///此帐号已经登录
#define			ERR_GP_USERNICK_EXIST			12		///此昵称已经存在
#define			ERR_GP_USER_BAD					13		///未法字符
#define			ERR_GP_IP_FULL					14		///IP已满
#define			ERR_GP_LOCK_SUCCESS				15		///锁定机器成功	2009-11-12
#define			ERR_GP_ACCOUNT_HAS_LOCK			16		///机器已经处于锁定状态	2009-11-12
#define			ERR_GP_UNLOCK_SUCCESS			17		///解除锁定成功 2009-11-12
#define			ERR_GP_NO_LOCK					18		///机器根本就没有锁定，所以解锁失败 2009-11-12
#define			ERR_GP_CODE_DISMATCH			19		///机器码不匹配，解锁失败。	2009-11-12
#define			ERR_GP_ACCOUNT_LOCKED			20		///本账号锁定了某台机器，登录失败 2009-11-12 zxj
#define			ERR_GP_MATHINE_LOCKED			21

// PengJiLin, 2010-6-7, 第三方认证返回的错误码。鸥朴
#define			ERR_GP_USER_NOT_EXIST           30      // 用户不存在
#define			ERR_GP_USER_OVERDATE            31      // 用户已过期

//JianGuankun 20111110
#define			EXP_GP_ALLO_PARTY				50		///本账号异地登陆

#define DTR_GP_PLACE_VER_LOW_LIMIT				81									//低于最小版本 必须升级 add by wy 20160527
#define DTR_GP_PLACE_VER_MAX_LIMIT				82									//高于最大版本 不支持盖版本 

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#define	MDM_GP_LIST						101			// 游戏列表

///通信辅助标识										
#define		ASS_GP_LIST_KIND				1			//获取游戏类型列表
#define		ASS_GP_LIST_NAME				2			//获取游戏名字列表
#define		ASS_GP_LIST_ROOM				3			//获取游戏房间列表
#define		ASS_GP_LIST_COUNT				4			//获取游戏人数列表
#define		ASS_GP_ROOM_LIST_PEOCOUNT		5			//获取游戏人数列表
#define		ASS_GP_ROOM_PASSWORD			6			//发送房间密码 Add by lxl 2010-10-08，试图进入密码房间时发送此消息
#define		ASS_GP_GET_SELLGAMELIST			7			//获取游戏销售列表
#define		ASS_GP_ROOM_CHECK_MONEY			8		    //获取房间金币下限请求
#define	    ASS_GP_LIST_CHECK_MONEY			9		    //获取房间金币下限应答

//获取房间最低金币和用户钱包信息请求
struct MSG_GP_RoomCheckMoney
{
	UINT					uRoomID;							//房间 ID
	long int				dwUserID;							//用户 ID
};

//////////////////////////////////////////////////////////////////////////
//添加用户资料管理通讯协议
//{{ Added by JianGuankun 20111108 添加用户资料管理通讯协议
#define MDM_GP_USERINFO	 				115
#define		ASS_GP_USERINFO_UPDATE_BASE		1		//用户更新基本信息
#define		ASS_GP_USERINFO_UPDATE_DETAIL	2		//用户更新详细信息
#define		ASS_GP_USERINFO_UPDATE_PWD		3		//用户修改密码
#define		ASS_GP_USERINFO_NICKNAMEID		10		//根据玩家昵称找ID或ID找昵称

#define		HC_USERINFO_NOTACCEPT							0		//个人资料修改失败
#define		HC_USERINFO_ACCEPT								1		//个人资料修改成功
#define     HC_USERINFO_REPEAT                              2       //昵称已重复
#define		HC_USERINFO_BADNICK								3		//不允许的昵称


//*********************W与B互连时，W与B的通讯协议*******************************************************/
#define MDM_WD_PACKAGE                                      124
#define	ASS_WD_RANK										    2

//Added by JianGuankun 20111108}}

//////////////////////////////////////////////////////////////////////////

#define	ASS_GP_LOGONUSERS_COUNT		1

#define ASS_GP_USERINFO_QUERYINFO_BYUID						11      //根据玩家ID查询资料 add by hou_wy   子命令

//////////////////////////////////////////////////////////////////////////
///	网络通信数据包定义 

// 连接成功消息 
typedef struct tagMSG_S_ConnectSuccess 
{
	BYTE							bMaxVer;							///最新版本号码
	BYTE							bLessVer;							///最低版本号码
	BYTE							bReserve[2];						///保留字段
	LLONG							i64CheckCode;						///加密后的校验码，由客户端解密在包头中返回
} GNUC_PACKED MSG_S_ConnectSuccess;

//用户注册发送信息数据包
struct MSG_GP_S_VistorRegister
{
	char								szMD5Pass[50];						///用户加密密码
    char                                szUID[128];
};

///用户注册信息数据包
struct MSG_GP_R_VisitorRegister
{
	UINT								dwUserID;							///用户ID
//	bool								bClose;  //add by wy 20160526
};

struct MSG_GP_S_VisitorBind
{
	int									dwUserID;							///用户ID
	char								szOldPass[50];						///用户老密码
	char								szAccount[20];						///用户新账号
	char								szNewPass[50];						///用户新密码
	char								szNick[20];							///用户昵称
	int									szSex;								///用户性别
};

struct MSG_GP_R_VisitorBind
{
	int									iResult;							///绑定结果(0为成功, 1账号已存在， 2用户ID不存在)
};

struct MSG_GP_S_UserRegister
{
	char								szAccount[20];						///用户账号
	char								szMD5Pass[50];						///用户密码
	char								szNick[20];							///用户昵称
	char								szPass[12];							///用户明文密码
	int									szSex;								///用户性别
	int									iHeadID;							///用户头像ID
	char                                szUID[128];
};

struct MSG_GP_R_UserRegister
{
	int							dwUserID;							///用户ID(0为账号已存在)
	bool						bClose;                             //add by wy 20160526
};

struct MSG_GP_R_User_Verification
{
	int							dwRes;
};
// 用户登陆（帐号）结构
typedef struct tagMSG_GP_S_LogonByNameStruct 
{
	UINT								uRoomVer;						///大厅版本
	CHAR								szName[64];						///登陆名字
	CHAR								TML_SN[128];						
	CHAR								szMD5Pass[52];					///登陆密码
	CHAR								szMathineCode[64];				///本机机器码 锁定机器
	CHAR                                szCPUID[24];					//CPU的ID
	CHAR                                szHardID[24];					//硬盘的ID
	CHAR								szIDcardNo[64];					//证件号
	CHAR								szMobileVCode[8];				//手机验证码
	INT									gsqPs;	
	INT									iUserID;						//用户ID登录，如果ID>0用ID登录
	char                                szUID[128];
    char								szHeadUrl[256];   //微信头像URL
} GNUC_PACKED MSG_GP_S_LogonByNameStruct;

struct MSG_GP_S_User_Verification
{
	char								szAccount[20];						///用户账号
};

// 大厅登录返回结果
typedef struct tagMSG_GP_R_LogonResult 
{
	INT									dwUserID;							//用户 ID 
	INT                                 iIsMaster;                          ///是否管理员
	INT									dwGamePower;						//游戏权限
	INT									dwMasterPower;						//管理权限
	INT									dwMobile;							//手机号码
	INT									dwAccID;							//Acc 号码
	UINT								dwLastLogonIP;						//上次登陆 IP
	UINT								dwNowLogonIP;						//现在登陆 IP
	UINT								bLogoID;							//用户头像
	bool								bBoy;								//性别
	CHAR								szName[61];							//用户登录名
	CHAR								TML_SN[128];						//数字签名
	CHAR								szMD5Pass[50];						//用户密码
	CHAR								nickName[32];						//用户昵称
	LLONG								i64Money;							//用户金币
	LLONG								i64Bank;							//用户财富
	LLONG                             i64Ingot;                           //用户元宝
	LLONG                             i64TotalMoney;                      //总金币
	INT                                 iLotteries;                         //用户奖券
	INT									dwFascination;						//魅力

	//JianGK 20111107新用户资料
	CHAR								szSignDescr[128];					//个性签名
	CHAR								szRealName[20];						//真实姓名
	CHAR								szIDCardNo[36];						//证件号
	CHAR								szMobileNo[50];						//移动电话
	CHAR								szQQNum[20];						//QQ号码
	CHAR								szAdrNation[50];					//玩家的国藉
	CHAR								szAdrProvince[50];					//玩家所在的省份
	CHAR								szAdrCity[50];						//玩家所在的城市
	CHAR								szZipCode[10];						//邮政编码
	CHAR								szEmail[50];						//电子邮件
	CHAR								szAddress[128];						//联系地址
	CHAR								szSchooling[20];					//文化程度
	CHAR								szHomePage[128];					//个人主页
	CHAR								szTelNo[20];						//固定电话
	CHAR								szMSNID[50];						//MSN帐号
	//end JianGK 20111107

	INT									dwTimeIsMoney;						///上次登陆时长所换取的金币
	INT									iVipTime;							///
	INT									iDoublePointTime;					///双倍积分时间
	INT									iProtectTime;						///护身符时间，保留
	bool								bLoginBulletin;						///是否有登录公告，Fred Huang,2008-05-20
	INT									iLockMathine;						///当前帐号是否锁定了某台机器，1为锁定，0为未锁定 zxj 2009-11-13
	INT									iBindMobile;						///当前帐号是否绑定手机号码，1为绑定，0为未绑定 jianguankun 2012.10.10
	INT									iBindIDCardNo;						///当前帐号是否绑定身份证号码，1为绑定，0为未绑定 jianguankun 2012.12.7

	INT									iAddFriendType;						///是否允许任何人加为好友

	INT									iAgencyID;							///代理ID

	//JianGK 20130506新加用户资料
	CHAR								szBirthday[16];						///生日，格式为“yyyy-mm-dd”
	INT									iAge;								///年龄
	bool                                bNickname;                          //昵称是否已修改

	bool								bIsUseIngot;
	bool								bIngotBuyProp;
	bool								bEnableExchange;

	CHAR                                szQuestion1[50];
	CHAR                                szAnswer1[50];
	CHAR                                szQuestion2[50];  
	CHAR                                szAnswer2[50];

	//yule
	INT									iTitles;							//头衔(根据钻石数量评定)
	INT									iTreasureLv;						//宝箱等级(根据宝箱积分评定)
	INT									iDiamond;							//钻石数量
	INT 								iTrumpet;							//喇叭数量
	INT 								iKickCard;							// 踢人卡数量
	INT 								iSendGifts;							//可送礼包数
	INT									iMyGifts;							//我的礼包数
	INT 								iTotalGifts;						//总礼包数
	INT 								iGift[15];							//礼物列表(15个礼物)
	INT									iVipLv;								//VIP等级
	INT									iVipPoint;							//VIP点数
	INT									iTreasurePoint;						//总宝箱积分
	INT									iDayTreasurePoint;					//每日宝箱积分			
	INT									iPhotoState[3];						//相册照片状态
	int									iFirstLogin;						//当日首次登入
	int									iDays;								//连续登入天数
	int									iGiveType;							//登入奖励类型
	int									iGiveCount;							//登入奖励数量
	int									iIsDial;
	bool								bClose;  //add by wy 20160526
    char								szHeadUrl[256];
} GNUC_PACKED MSG_GP_R_LogonResult;

typedef MSG_GP_R_LogonResult MSG_GP_UserInfo;

// 登陆服务器登陆信息
typedef struct tagDL_GP_RoomListPeoCountStruct 
{
	UINT							uID;				///房间ID 
	UINT							uOnLineCount;		///在线人数
	INT								uVirtualUser;		///扩展机器人人数///20081211 , Fred Huang
} GNUC_PACKED DL_GP_RoomListPeoCountStruct;

//////////////////////////////////////////////////////////////////////////
// 统计登录人数
typedef struct tagONLINEUSERSCOUNT 
{
	UINT								uiLogonPeopCount;	///登录人数
} GNUC_PACKED ONLINEUSERSCOUNT;

//////////////////////////////////////////////////////////////////////////
// 获取游戏房间数据包
typedef struct tagMSG_GP_SR_GetRoomStruct 
{
	UINT								uKindID;		///类型 ID
	UINT								uNameID;		///名字 ID
} GNUC_PACKED MSG_GP_SR_GetRoomStruct;

//////////////////////////////////////////////////////////////////////////
// 玩家列表操作
typedef struct tagMSG_GP_User_Opt_Struct 
{
	INT									dwTargetID;		///操作对像
	INT									dwUserID;		///操作id
	UINT								uType;			///操作类型
} GNUC_PACKED MSG_GP_User_Opt_Struct;

//////////////////////////////////////////////////////////////////////////
// 获取游戏房间数据包
typedef struct tagMSG_GP_SR_OnLineStruct 
{
	UINT								uKindID;		///类型 ID
	UINT								uNameID;		///名字 ID
	UINT								uOnLineCount;	///在线人数
} GNUC_PACKED MSG_GP_SR_OnLineStruct;

//////////////////////////////////////////////////////////////////////////
// 游戏信息
typedef struct tagMSG_GM_S_ClientInfo 
{
	BYTE								bEnableWatch;						///允许旁观
} GNUC_PACKED MSG_GM_S_ClientInfo;

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 购买商品信息
typedef struct taMSG_GP_CS_NOTIFY_IOSPayResult 
{
	UINT								userID;								// 用户ID
	UINT								productPrice;						// 商品价格
	LLONG								llMoney;							// 购买的金币
	CHAR								productSerialNumber[128];			// 商品ID
} GNUC_PACKED MSG_GP_CS_NOTIFY_IOSPayResult;

//////////////////////////////////////////////////////////////////////////
// 用户修改密码
typedef struct MSG_GP_S_ChPassword
{
	UINT dwUserID;		//用户ID 
	CHAR szHardID[24];		//硬盘ID
	CHAR szMD5OldPass[80];  //用户老密码
	CHAR szMD5NewPass[80];  //用户新密码
	MSG_GP_S_ChPassword()
	{
		::memset(this, 0x0, sizeof(MSG_GP_S_ChPassword));
	}
} GNUC_PACKED MSG_GP_S_ChPassword;

//////////////////////////////////////////////////////////////////////////
//手机短信验证码获取
#define	MDM_GP_SMS				117
#define		ASS_GP_SMS_VCODE			1
typedef struct MSG_GP_SmsVCode
{
	char szName[61];		//用户名
	char szMobileNo[50];	//手机号码
	char szVCode[36];		//产生的验证码，使用MD5加密
	UINT nType;				//短信验证码类型
								//0为登陆验证
								//1为锁机验证
								//2为解锁机验证
								//3为修改手机验证
								//4为绑定手机验证
								//5为解除绑定手机验证
} GNUC_PACKED MSG_GP_SmsVCode;;

//////////////////////////////////////////////////////////////////////////
///绑定手机号码

#define	MDM_GP_BIND_MOBILE		112
#define		ASS_GP_BIND_MOBILE	1

typedef struct MSG_GP_S_BindMobile
{
	UINT								dwUserID;				///用户ID
	UINT								dwCommanType;			///命令请求类型,1表示要求绑定，0表示要求解除绑定
	char								szMobileNo[50];			///手机号码  Added by Jianguankun 2012.5.16
} GNUC_PACKED MSG_GP_S_BindMobile;

///绑定/解除绑定手机返回数据包
typedef struct MSG_GP_R_BindMobile
{
	UINT	dwUserID;
	UINT	dwCommanType;			///请求命令类型（0请求解除绑定手机结果，1请求绑定手机结果）
	UINT	dwCommanResult;			///请求的结果
} GNUC_PACKED MSG_GP_R_BindMobile;

//////////////////////////////////////////////////////////////////////////
#define	MDM_GP_MESSAGE		103
#define	MDM_GP_SYSMSG		104





//*********************签到通讯主标识***************************************************************/
#define MDM_GP_LOGIN_SIGN                                   130         //登陆签到消息
//*********************签到通讯辅标识***************************************************************/
#define ASS_GP_S_LOGIN_REWARD_CFG                           1			//获取登陆奖励配置
#define	ASS_GP_S_LOGIN_SIGN_IN		          				2			//签到
#define	ASS_GP_R_LOGIN_REWARD_CFG						    3			
#define	ASS_GP_R_LOGIN_SIGN_IN								4			
//*********************通信错误标识*****************************************************************/

struct MSG_GP_S_LOGIN_SIGN_IN
{
	long int dwUserID;		//用户ID
};

struct MSG_GP_R_LOGIN_SIGN_IN
{
	int iGold;		//奖励金币(0为失败)
};

struct MSG_GP_S_LOGIN_REWARD_CFG
{
	long int dwUserID;		//用户ID
};

#define MAX_LOGIN_SIGN_DAY	10
struct MSG_GP_R_LOGIN_REWARD_CFG
{
	LLONG i64Time;		// 上一次签到时间(0为从来没签到)
	int iDay;				// 已连续签到的天数
	int arrReward[MAX_LOGIN_SIGN_DAY];
};


//*********************排行榜通讯主标识***************************************************************/
#define MDM_GP_USER_RANK                                    131         //排行榜;
#define MDM_GP_CHARTS_REWARD                                380         //
//*********************排行榜通讯辅标识***************************************************************/
#define ASS_GP_S_USER_RANK_LIST								1			//获取排行列表;
#define	ASS_GP_S_USER_RANK_POS		          				2			//获取排行位置;
#define	ASS_GP_R_USER_RANK_LIST								3			
#define	ASS_GP_R_USER_RANK_POS								4			

#define ASS_GP_S_USER_RANK_POS_TDG							5			//获取排行位置-数组
#define ASS_GP_R_USER_RANK_POS_TDG							6

#define ASS_GP_S_USER_RANK_LIST_TSCORE						7			//获取排行列表-宝箱总积分
#define ASS_GP_R_USER_RANK_LIST_TSCORE						8
#define ASS_GP_S_USER_RANK_LIST_DIAMOND						9			//获取排行列表-头衔（钻石）
#define ASS_GP_R_USER_RANK_LIST_DIAMOND						10
#define ASS_GP_S_USER_RANK_LIST_GIFT						11			//获取排行列表-礼物价值
#define ASS_GP_R_USER_RANK_LIST_GIFT						12
#define ASS_GP_S_USER_RANK_REWARD							13
#define ASS_GP_R_USER_RANK_REWARD							14
#define ASS_GP_S_USER_SHARE_REWARD							15
#define ASS_GP_R_USER_SHARE_REWARD							16

//*********************通信错误标识*****************************************************************/

// 财富排行;
#define MAX_USER_RANK_COUNT 1000
struct MSG_GP_S_USER_RANK_LIST
{
	int dwCount;		//排行的数量;
};

struct MSG_GP_S_USER_RANK_POS
{
	int dwUserID;		//用户ID;
};

struct MSG_GP_R_USER_RANK_LIST
{
	int dwUserID;		//用户ID;
	int dwRankPos;		//排名;
	int dwLogoID;		//头像ID;
	long long i64Money;		//用户总金钱;
	char szNick[20];		//昵称;
};

// ------ Yu Le Ranking -----------
struct MSG_GP_R_USER_RANK_LIST_TSCORE
{
	int				dwUserID;			//用户ID;
	int				dwRankPos;			//排名;
	long long		i64Diamond;			//用户钻石(用于头衔定位);
	long long		dwTreasureScore;	//宝箱总积分
	char 			szNick[20];			//昵称;
};

struct MSG_GP_R_USER_RANK_LIST_DIAMOND
{
	int				dwUserID;			//用户ID;
	int				dwRankPos;			//排名;
	long long		i64Diamond;			//用户钻石(用于头衔定位);
	char 			szNick[20];			//昵称;
};

struct MSG_GP_R_USER_RANK_LIST_GIFT
{
	int				dwUserID;			//用户ID;
	int				dwRankPos;			//排名;
	long long		i64Diamond;			//用户钻石(用于头衔定位);
	long long		i64GiftValue;		//礼物总价值
	char 			szNick[20];			//昵称;
};
// ----------------

struct MSG_GP_R_USER_RANK_POS
{
	int dwRankPos;		//用户的排名;
};

// ------ Yu Le Ranking -----------
struct MSG_GP_R_USER_RANK_POS_TDG
{
	int dwRankPos[3];		//用户的排名 0.积分排名  1.头衔排名  2.礼物价值排名;
	int dwRankGetStatus[3]; //是否已经领奖 0，没有 1，已领
};
// -----------------


///聊天结构 
typedef struct MSG_GR_RS_NormalTalk
{
	UINT								crColor;							///字体颜色
	short								iLength;							///信息长度
	INT									dwSendID;							///用户 ID
	INT									dwTargetID;							///目标 ID
	INT									nDefaultIndex;						///=0，输入的内容，>0，选择的内容
	CHAR								szMessage[MAX_TALK_LEN+1];			///聊天内容
} GNUC_PACKED MSG_GR_RS_NormalTalk;

//////////////////////////////////////////////////////////////////////////
// 道具获取以及兑换消息。


// 道具ID定义
#define Item_Trumpet_ID			5

////////////////////////////////////

#define			MDM_GP_PROP							140

#define			ASS_PROP_GETUSERPROP				0x01
#define			ASS_PROP_BIG_BOARDCASE				0x07				//大喇叭消息
#define			ASS_PROP_NEW_KICKUSER				0x09				//踢人卡
#define			ASS_PROP_NEW_KICKISER_INFO          0x0a				//踢人卡反馈
#define			ASS_PROP_BUY_NEW					0x0b

#define			DTK_GP_PROP_BUY_ERROR				10
#define			DTK_GP_PROP_BUY_NOMONEY				20
#define			DTK_GP_PROP_BUY_SUCCEED				80//购买成功
#define			DTK_GP_PROP_BUYANDUSE_SUCCEED		81//即买即用成功9

typedef struct _TAG_PROP_BUY
{
	INT		dwUserID;					//购买者ID
	INT		nPropID;					//道具ID
	INT		iPropPayMoney;				//总共的金币
	INT		nPropBuyCount;				//道具数量
	bool	bUse;//手游特别注意，这个字段意思是即买即用（1）还是买了等待被用（0），如果像等下管理员换成话费的应该填0，否则不会存在用户道具表里面

	_TAG_PROP_BUY()
	{
		memset(this, 0x0, sizeof(_TAG_PROP_BUY));
		bUse = false;
	}
} GNUC_PACKED _TAG_PROP_BUY;

typedef struct MSG_PROP_C_GETSAVED
{
	INT		dwUserID;						//用户ID
} GNUC_PACKED MSG_PROP_C_GETSAVED;

#define BROADCAST_MESSAGE_MAX			255
//----------- 使用喇叭广播
typedef struct  
{
	int									nPropID;							//道具ID号
	int									HoldCount;                          //道具数量
	int									dwUserID;							//用户ID
	int									crColor;							//字体颜色
	short								iLength;							//信息长度
	CHAR								szUserName[32];						//用户名字;		
	CHAR								szMessage[BROADCAST_MESSAGE_MAX+1];//聊天内容
} _TAG_BOARDCAST;

// 服务端发送的数据。
// 从服务器上取得的个人道具，每一个道具一条消息。
typedef struct MSG_PROP_S_GETPROP
{
	INT			dwUserID;						//用户ID号
	INT			nPropID;						//道具ID号
	INT			nHoldCount;						//拥有道具的数量
	CHAR		szPropName[50];					//道具名称
	INT			attribAction;					//操作属性
	INT			attribValue;					//取值属性
	INT			dwCost;							//当前该笔交易花了多少钱，由ZXD添加，服务器须提供此数据
	INT			iPrice;							//道具价格
	INT			iVipPrice;						//VIP道具价格
} GNUC_PACKED MSG_PROP_S_GETPROP, _TAG_USERPROP;


//==================游戏记录模块======================//
#pragma  pack(1)

#define ASS_GM_QUERY_GAMERECORD_PDK    11       //查询游戏记录密室版
#define ASS_GR_QUERY_GAMERECORD_PDK 112 //查询游戏记录房间里所有记录

#define ERR_GM_QUERY_NO_RECORD		   1		//没有记录
#define ERR_GM_QUERY_UNKNOWN		   2		//未知
#define ERR_GM_QUERY_SUC			   0		//查询成功

typedef struct MSG_GM_C_QUERY_GAMERECORD_PDK
{
    int dwRoomID;
    int dwDeskIndex;
    int dwUserID;
}GNUC_PACKED MSG_GM_C_QUERY_GAMERECORD_PDK;

typedef struct TAG_GAMERECORD_ITEM_PDK
{
    int dwUserID;
    long long  i64SrcPoint;
    long long  i64ChangePoint;
    long long  i64SrcMoney;
    long long  i64ChangeMoney;
    long long  i64ChangeTax;
    char	 sNickName[50];
}GNUC_PACKED TAG_GAMERECORD_ITEM_PDK;

typedef struct TAG_GAMERECORD_PDK
{
    short  wUserCount;
    TAG_GAMERECORD_ITEM_PDK  item[18];
}GNUC_PACKED TAG_GAMERECORD_PDK;


typedef struct MSG_GM_S_QUERY_GAMERECORD_PDK
{
    int dwRoomID;
    int dwDeskIndex;
    int dwRoundNo;
    int dwGameRounds;
    char szEndTime[50];
    char  szVirRoomID[50];
    TAG_GAMERECORD_PDK  rec;
}GNUC_PACKED MSG_GM_S_QUERY_GAMERECORD_PDK;

#pragma  pack(0)

//主ID=140，辅助ID=21
#define MDM_GP_NET_PROP_BUY_LOG		140
#define		ASS_GP_NET_PROP_BUY_LOG		21

typedef struct NET_PROP_BUY_LOG_RESULT
{
	CHAR	szPropName[50];						//道具名称 
	BYTE	bPropID;							//道具ID
	UINT	iPorpCount;							//购买数量
	BYTE	bCostType;							//花费的类型（1-金币，2-奖券）
	UINT	iCost;								//花费的数量
	LLONG	llTime;								//购买的时间
} GNUC_PACKED NET_PROP_BUY_LOG_RESULT;

class CCenterHead
{
public:
	enum enumCenterType
	{
		CenterServerPort=65320//6688
	};
	class Message
	{
	public:
		int m_is_haveZhuanZhang;
		char m_strGameSerialNO[20];////客户端当前版本系列号
		char m_strMainserverIPAddr[20];			////主服务器IP地址
		int m_iMainserverPort;					////主服务器端口号
		char m_strWebRootADDR[128],				////网站根路径，在程序中涉及的文件子目录根据这个地址来扩展
			m_strHomeADDR[128],					////大厅登录后第一次显示的页面
			m_strHelpADDR[128],					////帮助页WEB地址
			m_strDownLoadSetupADDR[128],		////客户端安装程序下载页WEB地址
			m_strDownLoadUpdatepADDR[128],		////客户端更新程序下载页WEB地址
			m_strRallAddvtisFlashADDR[128],		//客户端大厅FLASH广告下载页WEB地址
			//wushuqun 2009.6.5
			m_strRoomRollADDR[200];			//客户端大厅滚动广告

		int	 m_nHallInfoShowClass;
		int	 m_nEncryptType;					//平台所采用的加密方式，1位MD5，2位SHA1，默认为2; 2009-5-30 zxj
		unsigned int m_nFunction;               ///< 功能控制 Y.C.YAN
		int m_lNomalIDFrom;                    ///< 金葫芦的散户ID开始
		int m_lNomalIDEnd;						///< 金葫芦的散户ID结束
		int  m_nIsUsingIMList;					///< 是否使用好友列表

		Message( )
		{
			m_iMainserverPort    = 0;
			m_nHallInfoShowClass = 0;
			m_nEncryptType       = 2;					//平台所采用的加密方式，1位MD5，2位SHA1，默认为2; 2009-5-30 zxj
			m_nFunction          = 0;
			m_nIsUsingIMList     = 1;					//默认为显示好友列表
			memset(m_strGameSerialNO,0,sizeof(m_strGameSerialNO));
			memset(m_strMainserverIPAddr,0,sizeof(m_strMainserverIPAddr));
			memset(m_strHomeADDR,0,sizeof(m_strHomeADDR));
			memset(m_strWebRootADDR,0,sizeof(m_strWebRootADDR));
			memset(m_strHelpADDR,0,sizeof(m_strHelpADDR));
			memset(m_strDownLoadSetupADDR,0,sizeof(m_strDownLoadSetupADDR));
			memset(m_strDownLoadUpdatepADDR,0,sizeof(m_strDownLoadUpdatepADDR));
			memset(m_strRallAddvtisFlashADDR,0,sizeof(m_strRallAddvtisFlashADDR));
			//wushuqun 2009.6.5
			memset(m_strRoomRollADDR,0,sizeof(m_strRoomRollADDR));

		}
		~Message(){}
	};
};

typedef   CCenterHead::Message     CenterServerMsg;

#endif // !_YZ_YZNetMessageHall_H_
