#ifndef _YZ_RoomMessage_H__
#define _YZ_RoomMessage_H__

#include "YZComStruct.h"

/*********************************************************************************/
#define MAX_TALK_LEN			500								///最大聊天数据长度

//数组长度
#define  MIN_LEN   50
#define  MAX_LEN   200

//最大一桌人数
#define  MAX_DESK_COUNT 16




///	通信标识定义 

/*********************************************************************************/
//连接消息
#define MDM_GR_CONNECT					1						//连接消息类型
#define	ASS_GR_CONNECT_SUCCESS			3						


//登录游戏房间的相关定义
/*--------------------------------------------------------------------------------*/
#define	MDM_GR_LOGON					100						//登陆游戏房间

#define MDM_GR_QUEUE_MSG				114						///排队消息

//登录返回结果
#define ASS_GR_LOGON_SUCCESS			2						//登陆成功
#define ASS_GR_LOGON_ERROR				3						//登陆失败
#define ASS_GR_SEND_FINISH				4						//登陆完成
#define ASS_GR_LOGON_BY_ID				5						//通过用户ID登陆
#define ASS_GR_IS_VIPROOM               16                       //是否VIP房间
#define ASS_GR_VIP_PW                   17                       //VIP房间需要密码
#define ASS_GR_VIP_NO_PW                18                       //VIP房间不需要密码(第一个进入不需要密码)
#define ASS_GR_NO_VIP                   19                       //不是VIP房间
#define ASS_GR_VIPROOM_PW               20                       //VIP房间密码
#define ASS_GR_VIPROOM_PW_RIGHT         21                       //VIP房间密码正确

/*----------------------------------------------------------------------------------*/
//服务端主动推送
//用户列表主ID
#define MDM_GR_USER_LIST				101						//登录成功后返回个人信息以及该房间玩家信息
//用户列表辅助ID
#define ASS_GR_ONLINE_USER				1						//在线用户
#define ASS_GR_NETCUT_USER				2						//断线用户
#define ASS_GR_DESK_STATION				3						//桌子状态
/*-----------------------------------------------------------------------------------*/

//客户端发送

//玩家动作
#define MDM_GR_USER_ACTION				102						//用户动作，玩家坐桌，起身，断线等都是以该值为主标志
//玩家动作辅助ID
#define ASS_GR_USER_UP					1						//用户起来
#define ASS_GR_USER_SIT					2						//用户坐下
#define ASS_GR_WATCH_UP					3						//旁观起来
#define ASS_GR_WATCH_SIT				4						//旁观坐下

#define ASS_GR_USER_COME				5						//用户进入
#define ASS_GR_USER_LEFT				6						//用户离开

#define ASS_GR_USER_CUT					7						//用户断线
#define ASS_GR_SIT_ERROR				8						//坐下错误
#define ASS_GR_SET_TABLE_BASEPOINT		9						//改变桌子倍数

#define ASS_GR_USER_HIT_BEGIN			10						//用户同意开始

#define ASS_GR_JOIN_QUEUE				11						//加入排队机
#define ASS_GR_QUIT_QUEUE				12						//退出排队机
#define ASS_GR_QUEUE_USER_SIT			13						//排队用户坐下
#define ASS_GR_LEASE_TIMEOVER			14						//排队用户坐下
#define ASS_GR_CONTEST_APPLY			17						//比赛场报名
#define ASS_GR_CONTEST_CANCEL			18						//比赛场退赛

//玩家动作部分操作结果：（专用于比赛房间）
#define ERR_GR_APPLY_SUCCEEDED			1						//参赛或退赛成功
#define ERR_GR_APPLY_ALREADYSIGN		2						//已经报名
#define ERR_GR_APPLY_BEGIN				3						//比赛已经开始
#define ERR_GR_APPLY_NOTENOUPH_MONEY	4						//由于钱包金币不够，导致报名失败


//房间信息
#define MDM_GR_ROOM							103		
//房间信息辅助ID
#define		ASS_GR_NORMAL_TALK				1					//普通聊天
#define		ASS_GR_HIGH_TALK				2					//高级聊天
#define		ASS_GR_USER_AGREE				3					//用户同意
#define		ASS_GR_GAME_BEGIN				4					//游戏开始
#define		ASS_GR_GAME_END					5					//游戏结束
#define		ASS_GR_USER_POINT				6					//用户经验值
#define		ASS_GR_SHORT_MSG				7					//聊短信息
#define		ASS_GR_ROOM_SET					8					//房间设置
#define		ASS_GR_INVITEUSER				9					//邀请用户
#define		ASS_GR_INSTANT_UPDATE			10					//即时更新分数金币
#define		ASS_GR_UPDATE_CHARM				11					//即时更新魅力
#define		ASS_GR_ROOM_PASSWORD_SET		12					//房间设置
#define		ASS_GR_ROOM_QUEUE_READY			13					//排队机准备
#define		ASS_GR_GET_NICKNAME_ONID        14					//根据ID获取昵称
#define		ASS_GR_OWNER_T_ONE_LEFT_ROOM    15					//房主踢玩家离开房间
#define		ASS_GR_GET_NICKNAME_ONID_INGAME 16					//根据ID获取昵称 游戏中
#define		ASS_GR_USER_CONTEST				17					//用户比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，排名有更改
#define		ASS_GR_AVATAR_LOGO_CHANGE		18					//用户形象更改信息
#define		ASS_GR_CAYZGE_ROOM				19					//比赛开始，未报名用户切换房间
#define		ASS_GR_CONTEST_GAMEOVER			20					//比赛结束
#define		ASS_GR_CONTEST_KICK				21					//用户被淘汰
#define		ASS_GR_CONTEST_WAIT_GAMEOVER	22					//比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
#define		ASS_GR_INIT_CONTEST				23					//比赛开始，初始化比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，初始化排名信息
#define		ASS_GR_CONTEST_PEOPLE			24					//获取报名数量，登陆完成后服务端主动发送NET_ROOM_CONTEST_PEOPLE_RESULT
#define		ASS_GR_CONTEST_RECORD			25					//个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
#define		ASS_GR_CONTEST_AWARDS			26					//比赛奖励，一连串的NET_ROOM_CONTEST_AWARD_RESULT
#define		ASS_GR_CONTEST_GETINFO			27					//获取比赛信息（服务端返回24,25,26三个消息包）

#define     ASS_GR_CREATE_BACKROOM          30			//创建密室
#define     ASS_GR_OPERAT_STATUS			34			//可操作状态
#define     ASS_GR_QUERY_BACKROOM			35			//获取入座信息
#define     ASS_GR_DISMISS_BACKROOM_REQ		32			//请求解散
#define     ASS_GR_DISMISS_BACKROOM_AGREE   33          //同意解散
#define     ASS_PROP_ROOMCARD_USE			0x35		//使用或赠送房卡
#define     ASS_PROP_ROOMCARD_BUY			0x34		//购买密室房卡
#define     ASS_PROP_ROOMCARD_TYPEINFO		0x37        //查询房卡类型信息       C -> S
#define     ASS_PROP_ROOMCARD_GETINFO		0x36		//查询玩家房卡信息    C -> S
#define     ASS_GR_DISMISS_BACKROOM         31          //解散密室


//比赛消息
#define MDM_GR_MATCH_INFO				104		
#define ASS_GR_MATCH_USER				1									///比赛用户
#define ASS_GR_MATCH_DELETE				2									///取消比赛
#define ASS_GR_UPDATE_SCORE             3                                   //更新比赛玩家积分
#define ASS_GR_CUR_SCORE                4                                   //获取所有玩家当前积分 20140711 wzd
#define ASS_GR_MATCH_RULE               5                                   //获取比赛奖励方案请求 20140720 wzd
/*------------------------------------------------------------------------------------------------------------------------*/

//框架消息
#define MDM_GM_GAME_FRAME				150						//框架消息
//框架消息辅助ID
#define ASS_GM_GAME_INFO				1						//游戏信息
#define ASS_GM_GAME_STATION				2						//游戏状态
#define ASS_GM_FORCE_QUIT				3						//强行退出
#define ASS_GM_NORMAL_TALK				4						//普通聊天
#define ASS_GM_HIGH_TALK				5						//高级聊天
#define ASS_GM_WATCH_SET				6						//旁观设置	
#define ASS_GM_CLEAN_USER               9                        //比赛场清理用户信息
#define ASS_GM_USE_KICK_PROP            7						//使用踢人卡
#define ASS_GM_USE_ANTI_KICK_PROP       8						//使用防踢卡
#define ASS_GM_SET_VIDEOADDR			10						//设置视频服务器地址


/*********************************************************************************/
//通知消息
#define MDM_GM_GAME_NOTIFY				180						//游戏消息
#define ASS_GM_AGREE_GAME				1						//同意游戏
/*********************************************************************************/
//道具消息
#define MDM_GR_PROP						160						//道具



/*********************************************************************************/
///错误代码
#define ERR_GR_ERROR_UNKNOW				0						//未知错误
#define ERR_GR_LOGON_SUCCESS			1						//登陆成功
#define ERR_GR_USER_NO_FIND				2						//用户不存在
#define ERR_GR_USER_PASS_ERROR			3						//用户密码错误
#define ERR_GR_USER_VALIDATA			4						//用户帐号禁用
#define ERR_GR_USER_IP_LIMITED			5						//登陆 IP 禁止
#define ERR_GR_IP_NO_ORDER				6						//不是指定地址
#define ERR_GR_ONLY_MEMBER				7						//会员游戏房间
#define ERR_GR_IN_OTHER_ROOM			8						//正在其他房间
#define ERR_GR_ACCOUNTS_IN_USE			9						//帐号正在使用
#define ERR_GR_PEOPLE_FULL				10						//人数已经满
#define ERR_GR_LIST_PART				11						//部分用户列表
#define ERR_GR_LIST_FINISH				12						//全部用户列表
#define ERR_GR_STOP_LOGON				13						//暂停登陆服务

#define ERR_GR_CONTEST_NOSIGNUP			14
#define ERR_GR_CONTEST_TIMEOUT			15						
#define ERR_GR_CONTEST_KICK				16						//比赛被淘汰
#define ERR_GR_CONTEST_NOTSTRAT			17						//比赛未开始
#define ERR_GR_CONTEST_OVER				18						//比赛已经结束
#define ERR_GR_CONTEST_BEGUN			19						//比赛已经开始

#define ERR_GR_MATCH_LOGON				160						//游戏房间
#define ERR_GR_TIME_OVER				161						//时间到期

///wushuqun 2009.6.5
///不在混战场活动时间内
#define ERR_GR_BATTLEROOM_OUTTIME       162

///用户坐下错误码
#define ERR_GR_SIT_SUCCESS				50						//成功坐下
#define ERR_GR_BEGIN_GAME				51						//游戏已经开始
#define ERR_GR_ALREAD_USER				52						//已经有人存在
#define ERR_GR_PASS_ERROR				53						//密码错误
#define ERR_GR_IP_SAME					54						//IP 相同
#define ERR_GR_CUT_HIGH					55						//断线率太高
#define ERR_GR_POINT_LOW				56						//经验值太低
#define ERR_GR_POINT_HIGH				57						//经验值太高
#define ERR_GR_NO_FRIEND				58						//不受欢迎
#define ERR_GR_POINT_LIMIT				59						//经验值不够
#define ERR_GR_CAN_NOT_LEFT				60						//不能离开
#define ERR_GR_NOT_FIX_STATION			61						//不是这位置
#define ERR_GR_MATCH_FINISH				62						//比赛结束
#define ERR_GR_MONEY_LIMIT				63						//金币太低
#define ERR_GR_MATCH_WAIT				64						//比赛场排队提示
#define ERR_GR_IP_SAME_3				65						//IP前3 相同
#define ERR_GR_IP_SAME_4				66						//IP前4 相同
#define ERR_GR_UNENABLE_WATCH			67						//不允许旁观
#define ERR_GR_DESK_FULL				68						// 百家乐 桌子座位满了，无法分配座位给玩家 add by wlr 09.07.14

///游戏房间登陆
typedef struct tagMSG_GR_S_RoomLogon {
	UINT								uNameID;				//游戏 ID
	INT									dwUserID;				//用户 ID
	UINT								uRoomVer;				//大厅版本
	UINT								uGameVer;				//游戏版本
	CHAR								szMD5Pass[50];			//加密密码
} GNUC_PACKED MSG_GR_S_RoomLogon;

//房间管理窗口数据更新
struct MSG_GR_GRM_UpData
{
	//奖池
	bool		bAIWinAndLostAutoCtrl;	//机器人控制输赢开关
	LLONG		iAIWantWinMoney[3];		/**<机器人输赢控制：机器人赢钱区域1,2,3 4所用断点 */
	int			iAIWinLuckyAt[4];			/**<机器人输赢控制：机器人在区域1,2,3,4赢钱的概率 */
	LLONG		iReSetAIHaveWinMoney;		//机器人输赢控制：重置机器人已经赢钱的数目

	LLONG		iAIHaveWinMoney;		//机器人赢的钱
	//输赢控制
	bool		bWinProbCtrl;			//开关
	INT dwUserID_win[20];		//可读取20组，多了影响性能,玩家ID
	int			iProb_win[20];			//输赢概率[0,100]
	INT	dwUserID_los[20];		//可读取20组，多了影响性能,玩家ID
	int			iProb_los[20];			//输赢概率[0,100]

	MSG_GR_GRM_UpData()
	{
		memset(this,0,sizeof(MSG_GR_GRM_UpData));
	}
}GNUC_PACKED;


//比赛基本信息
typedef struct tag_MATCHBASEINFO
{
	char szMatchDesp[MAX_LEN];      //赛制详情	
	int        nReportCounts;       //报名人数
	int        nMatchActors;        //参赛人数	
	int        nScoreDiff;          //距离上几名的积分差,默认上一名
	int        nPeopleLimit;        //开赛人数限制	
	bool       ISBegin;             //人未满是否仍开赛 
	bool       ISMatchEnd;          //比赛是否已经结束

	tag_MATCHBASEINFO()
	{
		memset(szMatchDesp,0,MAX_LEN);		
		nReportCounts = 0;
		nMatchActors  = 0;		
		nScoreDiff    = 1;
		nPeopleLimit  = 0;		
		ISBegin       = false;
		ISMatchEnd    = false;
	}

	tag_MATCHBASEINFO &operator =(const tag_MATCHBASEINFO &tMatchBInfo)
	{
		strcpy(szMatchDesp,tMatchBInfo.szMatchDesp);			
		nMatchActors  = tMatchBInfo.nMatchActors; 		
		nScoreDiff    = tMatchBInfo.nScoreDiff;   
		nReportCounts = tMatchBInfo.nReportCounts;
		nPeopleLimit  = tMatchBInfo.nPeopleLimit; 		
		ISBegin       = tMatchBInfo.ISBegin;   

		return *this;
	}


}MATCHBASEINFO;


///游戏房间登陆
typedef struct tagMSG_GR_R_LogonResult
{
	INT                                 dwGamePower;                        //用户权限
	INT                                 dwMasterPower;                      //管理权限
	INT                                 dwRoomRule;                         //设置规则
	UINT								uLessPoint;							///最少经验值
	UINT								uMaxPoint;							///最多经验值
	UserInfoStruct						pUserInfoStruct;						///用户信息
	RECEIVEMONEY                        strRecMoney;                        //金币场金币不足自动赠送 20130829 WZD
	MATCHBASEINFO                       tMatchBaseInfo;                     //新比赛系统基本信息 20140709 wzd	
	///wushuqun 2009.6.6
	///登录房间时即时获取虚拟玩家人数
	int									nVirtualUser;
	int									nPresentCoinNum;  ///< 赠送金币数量

	//比赛专用
	int									iContestID;
	int									iLowCount;
	LLONG								i64Chip;
	LLONG								i64TimeStart;
	LLONG								i64TimeEnd;
	LLONG								i64LowChip;
	int									iTimeout;
	int									iBasePoint;
} GNUC_PACKED MSG_GR_R_LogonResult;

///游戏房间登陆
typedef struct tagMSG_GR_R_OtherRoom {
	INT									iRoomID;
	CHAR								szGameRoomName[61];					///房间名字
} GNUC_PACKED MSG_GR_R_OtherRoom;

///游戏桌子状态
typedef struct tagMSG_GR_DeskStation {
	BYTE								bDeskStation[100];					///桌子状态
	BYTE								bDeskLock[100];						///锁定状态
	INT									iBasePoint[100];					///桌子倍数
	BYTE                                bVirtualDesk[100];                  ///虚拟状态  yjj 090325
} GNUC_PACKED MSG_GR_DeskStation;

///用户进入房间
typedef struct tagMSG_GR_R_UserCome {
	UserInfoStruct						pUserInfoStruct;					///用户信息
} GNUC_PACKED MSG_GR_R_UserCome;

///用户离开房间
typedef struct tagMSG_GR_R_UserLeft {
	INT								dwUserID;							///用户 ID
} GNUC_PACKED MSG_GR_R_UserLeft;

///用户坐下
typedef struct tagMSG_GR_S_UserSit {
	BYTE								bDeskIndex;							///桌子索引
	BYTE								bDeskStation;						///桌子位置
	CHAR								szPassword[61];						///桌子密码
} GNUC_PACKED MSG_GR_S_UserSit;

///用户坐下
typedef struct tagMSG_GR_R_UserSit {
	INT									dwUserID;							///用户 ID
	BYTE								bLock;								///是否密码
	BYTE								bDeskIndex;							///桌子索引
	BYTE								bDeskStation;						///桌子位置
	BYTE								bUserState;							///用户状态
	BYTE								bIsDeskOwner;						///台主离开
} GNUC_PACKED MSG_GR_R_UserSit;

///用户坐起来
typedef MSG_GR_R_UserSit MSG_GR_R_UserUp;

///用户断线
typedef struct tagMSG_GR_R_UserCut {
	INT									dwUserID;							///用户 ID
	BYTE								bDeskNO;							///游戏桌号
	BYTE								bDeskStation;						///位置号码
} GNUC_PACKED MSG_GR_R_UserCut;

///用户同意结构
typedef struct tagMSG_GR_R_UserAgree {
	BYTE								bDeskNO;							///游戏桌号
	BYTE								bDeskStation;						///位置号码
	BYTE								bAgreeGame;							///同意标志
} GNUC_PACKED MSG_GR_R_UserAgree;

///用户分数
typedef struct tagMSG_GR_R_InstantUpdate {
	INT									dwUserID;							///用户 ID
	INT									dwPoint;							///用户分数
	INT									dwMoney;							///用户金币
} GNUC_PACKED MSG_GR_R_InstantUpdate;

///用户经验值
typedef struct tagMSG_GR_R_UserPoint {	//广播消息
	INT									dwUserID;							///用户 ID
	LLONG								dwPoint;							///用户经验值
	LLONG								dwMoney;							///用户金币
	LLONG								dwSend;								///赠送
	BYTE								bWinCount;							///胜局
	BYTE								bLostCount;							///输局
	BYTE								bMidCount;							///平局
	BYTE								bCutCount;							///逃局
	RECEIVEMONEY                        strAutoSendMoney;                   //添加自动赠送 2013-01-29 wangzhitu
} GNUC_PACKED MSG_GR_R_UserPoint;

/********************************************************************************************/

///游戏信息
typedef struct tagMSG_GM_S_GameInfo {
	BYTE								bGameStation;						///游戏状态
	BYTE								bWatchOther;						///允许旁观
	BYTE								bWaitTime;							///等待时间
	BYTE								bReserve;							///保留字段
	CHAR								szMessage[1000];					///系统消息
} GNUC_PACKED MSG_GM_S_GameInfo;


///修改用户经验值
typedef struct tagMSG_GR_S_RefalshMoney {
	INT									dwUserID;							///用户 ID
	LLONG								i64Money;							///用户金币
} GNUC_PACKED MSG_GR_S_RefalshMoney;

///旁观设置
typedef struct tagMSG_GM_WatcYZet {
	INT									dwUserID;							///设置对象
} GNUC_PACKED MSG_GM_WatcYZet;

///消息数据包
typedef struct tagMSG_GA_S_Message {
	BYTE								bFontSize;							///字体大小
	BYTE								bCloseFace;							///关闭界面
	BYTE								bShowStation;						///显示位置
	CHAR								szMessage[1000];					///消息内容
} GNUC_PACKED MSG_GA_S_Message;

//游戏公告消息ATT
typedef struct tagGameNoticeMessage {
	BYTE			bDeskIndex;			//桌子号
	BYTE			bAwardCardShape;	//牌型奖励
	CHAR			szMessage[255];		//未进入游戏 在房间 右边框显示公告内容
} GNUC_PACKED GameNoticeMessage;

//初始化获取所有参赛玩家当前积分请求
typedef struct tag_USERCURSCOREREQ
{
	int nRoomID;                     //比赛房间ID

}USERCURSCOREREQ;


//奖励方案
typedef struct tag_RANKPRIZE
{
	int  nFirstRankNO;                 //初始奖励名次
	int  nNextRankNO;                  //下一奖励名次
	int  i64LotteryPrize;              //奖励奖券数

	tag_RANKPRIZE()
	{
		nFirstRankNO  = 0;
		nNextRankNO   = 0;
		i64LotteryPrize = 0; 
	}

}RANKPRIZE;

//奖励方案应答
typedef struct tag_RANKPRIZEREP
{
	int nPrizeNum;                        //配置奖励方案个数
	RANKPRIZE szRankPrize[50];             //奖励方案集

}RANKPRIZEREP;



//参赛玩家当前积分结构体
typedef struct tag_USERCURSCORE
{
	int   dwUserID;               //玩家ID;    
	LLONG    i64CurScore;            //当前积分
	LLONG i64ReportTime;          //报名时间	
	LLONG    i64ChangeScore;         //上局结算积分	
	int        nContestCounts;         //玩的局数

	tag_USERCURSCORE()
	{
		dwUserID       = 0;	
		i64CurScore    = 0;
		i64ReportTime  = 0;	
		i64ChangeScore = 0;
		nContestCounts = 0;
	}

}USERCURSCORE;




//获取所有参赛玩家当前积分应答
typedef struct tag_USERCURSCOREREP
{
	BYTE                       bMatcYZum;                   //报名人数     
	LLONG                     tMatchEndTime;               //距离比赛结束时间
	USERCURSCORE               tMatcYZcore[180];            //应答包体，所有玩家报名信息	

	tag_USERCURSCOREREP()
	{
		tMatchEndTime  = 0;
		bMatcYZum = 0;
	}

}USERCURSCOREREP;



//参赛玩家积分更新结构体
typedef struct tag_UPDATEUSERSCORE
{
	int dwUserID;         //玩家ID		
	LLONG i64ChangePoint;    //玩家结算分数    

	tag_UPDATEUSERSCORE()
	{			
		dwUserID       = 0;
		i64ChangePoint = 0;
	}

	bool operator ==(const tag_UPDATEUSERSCORE &upContest)
	{
		if ((dwUserID == upContest.dwUserID)
			&& (i64ChangePoint == upContest.i64ChangePoint))
		{
			return true;
		}

		return false;
	}

	tag_UPDATEUSERSCORE &operator =(const tag_UPDATEUSERSCORE &upContest)
	{
		if (this == &upContest)
		{
			return *this;
		}

		i64ChangePoint = upContest.i64ChangePoint;
		dwUserID       = upContest.dwUserID;	

		return *this;
	}

}UPDATEUSERSCORE;

//赛玩家积分更新请求 20140710 wzd
typedef struct tag_UPDATEUSERSCOREREQ
{
	BYTE             bDeskCouts;                         //一桌人数	
	UPDATEUSERSCORE  szUpUserScore[MAX_DESK_COUNT];      //该桌积分更新信息

}UPDATEUSERSCOREREQ;


//玩家比赛状态更新结构体
typedef struct tag_UPDATEUSERSTATE
{
	long int   dwUserID;               //玩家ID
	BYTE       bContestState;          //玩家比赛状态 0 报名， 1 参赛， 2 比赛结束

	tag_UPDATEUSERSTATE()
	{
		dwUserID      = 0;
		bContestState = 0;
	}

}UPDATEUSERSTATE;



//玩家状态更新请求结构体
typedef struct tag_UPDATEUSERSTATEREQ
{
	BYTE    bUpdateCounts;                  //人数
	UPDATEUSERSTATE szUpUserState[180];     //玩家状态更新信息

	tag_UPDATEUSERSTATEREQ()
	{
		bUpdateCounts = 0;
	}

}tag_UPDATEUSERSTATEREQ;

//玩家排名界面信息
typedef struct tag_USERRANKSHOW
{
	long int dwUserID;                            //玩家ID
	LLONG i64CurScore;                          //当前积分
	int     nCurRank;                             //当前排名
	int     nTotalRank;                           //总排名
	LLONG i64DiffScore;                         //距离上N名相差的积分
	LLONG  tMatchEndTime;                        //距离比赛结束时间
	LLONG  tRecTime;                             //客户端接收到数据时时间

	tag_USERRANKSHOW()
	{
		dwUserID     = 0;
		i64CurScore  = 0;
		nCurRank     = 0;
		nTotalRank   = 0;
		i64DiffScore = 0;
		tMatchEndTime = 0;
		tRecTime      = 0;
	}

}USERRANKSHOW;

///用户比赛信息
struct MSG_GR_ContestChange
{
	long int					dwUserID;							///用户 ID	
	int							iContestCount;						///比赛局数
	LLONG						i64ContestScore;					///比赛分数
	int							iRankNum;							///比赛排名
};

//单个比赛玩家排名信息
typedef struct tag_MATCHRANK
{	

	USERCURSCORE  tMatchBase;              //基本信息	
	BYTE       bDeskNO;                    //该玩家所在桌子号

	tag_MATCHRANK()
	{			
		bDeskNO         = 255;
	}

}MATCHRANK;

//单桌积分更新包
typedef struct tag_UPUSERSCORE
{
	BYTE    bUpdateCounts;                  //人数
	MATCHRANK tMatchRank[18];               //各个玩家更新

}UPUSERSCORE;


//比赛结束玩家排名信息
typedef struct tag_MATCHENDRANK
{	
	int      nRankNum;               //玩家排名
	long int dwUserID;               //玩家ID
	tag_MATCHENDRANK()
	{
		nRankNum = 0;
		dwUserID = 0;
	}

}MATCHENDRANK;


//比赛房比赛结束玩家排名
typedef struct tag_RMATCHENDRANK
{
	int      nCounts;                 //排名人数
	MATCHENDRANK tMRank[180];         //排名集

	tag_RMATCHENDRANK()
	{
		nCounts = 0;
	}

}RMATCHENDRANK;





//比赛房比赛结束玩家排名
typedef struct tag_RMATCHENDRANKREQ
{
	int      nRoomID;                 //房间ID
	LLONG  i64StartTime;            //比赛开赛时间
	RMATCHENDRANK tRMatchRank;

	tag_RMATCHENDRANKREQ()
	{
		nRoomID       = 0;
		i64StartTime  = 0;
	}

}RMATCHENDRANKREQ;


//单个玩家比赛结束排名信息
typedef struct tag_MATCHENDRANKTOC
{
	int      nRoomID;                 //房间ID
	LLONG  i64StartTime;            //比赛开赛时间
	int      nRankNum;                //玩家排名
	int dwUserID;                //玩家ID
	LLONG i64Money;				 // 玩家金币

	tag_MATCHENDRANKTOC()
	{
		nRoomID  = 0;
		nRankNum = 0;
		dwUserID = 0;
		i64StartTime = 0;
		i64Money = 0;
	}


}MATCHENDRANKTOC;

//比赛结束客户端显示玩家排名信息
typedef struct tag_MATCHENDRANKSHOW
{
	int nRoomID;                      //房间ID
	char nickName[32];                //玩家昵称
	long int dwUserID;                //玩家ID
	char szGameName[61];             //游戏名称
	LLONG  i64StartTime;            //比赛开赛时间
	int      nRankNum;                //玩家排名


	tag_MATCHENDRANKSHOW()
	{		
		nRoomID  = 0;
		nRankNum = 0;
		dwUserID = 0;
		i64StartTime = 0;
		memset(nickName,0,32);
		memset(szGameName,0,sizeof(szGameName));
	}


}MATCHENDRANKSHOW;

//用户规则结构
typedef struct tag_UserRuleStruct
{
	///变量定义
	BYTE						bSameIP;							//相同 IP
	BYTE						bIPLimite;						//不于IP前几位的玩家游戏
	BYTE						bPass;								//设置密码
	BYTE						bLimitCut;							//限制断线
	BYTE						bLimitPoint;						//限制分数
	BYTE						bCutPercent;						//逃跑率
	char						szPass[61];						//桌子密码
    int					dwLowPoint;							//最低分数
    int					dwHighPoint;						//最高分数 
} UserRuleStruct;

//设置数据包
typedef struct tag_MSG_GR_S_UserSet
{
	UserRuleStruct						m_Rule;							//用户规则
}MSG_GR_S_UserSet;



//=========================== 开房 ====================

struct MSG_GR_BACKROOM_STATUS
{
    int    iUserID;        // 用户ID
    UINT		uRoomID;		//房间号
    UINT		uDeskIndex;		//座位号
    BYTE		cbDeskStation;	//桌号
    CHAR		szPwd[50];      //密码
    int		cbCanBack;		//是桌主桌子未解散，桌主可返回
};

typedef struct
{
    int	    dwUserID;						//用户ID
    int			nPropID;						//道具ID号
    int			nPropCount;						//使用的数量
    CHAR        szTargetName[30];				//赠送玩家 IsGive=1 有效 其它清零
    bool		IsGive;
    int         dwGameRoundNum;					//游戏局数
    int			dwZaCodeNum;					//扎码数
} MSG_PROP_C_OP_ROOMCARD;


typedef struct
{
    int         dwUserID;						//用户ID
    int		    dwRoomOwner;					//房主ID
    int		    dwGameRoundNum;					//游戏总局数
    int			nPropID;						//道具ID号
    int			nHoldCount;						//拥有道具的数量
    CHAR		szPropName[50];					//道具名称
    int			attribAction;					//操作属性
    int			attribValue;					//取值属性
    int			iPrice;							//道具价格
    int			iVipPrice;						//VIP道具价格
    int			dwZaCodeNum;					//扎码数
} MSG_PROP_S_OP_ROOMCARD;

typedef struct
{
    UINT                          uRoomID;
    UINT				          uDeskIndex;
    int						      dwDeskOwner;
    int							  dwGameRounds;
    int							  dwPlayGameRound;
    CHAR						  szPwd[50];
}MSG_BACKROOM_INFO;

//查询房间信息
typedef struct
{
    int						dwUserID;
    CHAR						  szPwd[50];
} MSG_QueryBACKROOM_INFO;


//房间信息
typedef struct
{
    int		                      dwUserID;						//用户ID
    UINT                          uRoomID;
    UINT				          uDeskIndex;
    int                           dwDeskOwner;
    BYTE						  cbDeskStation;
    int							  dwGameRounds;
    int							  dwPlayGameRound;
    int                           dwZaCodeNum;					//扎码数
} MSG_QueryBACKROOM_RSP_INFO;


//查询道具信息  C --> S
typedef struct
{
    int		  nPropID;							//道具ID号 0表示查询所有房卡类型
} MSG_PROP_C_KIND_ROOMCARD;

//查询房卡信息  S -> C
typedef struct
{
    int			nPropID;						//道具ID号
    CHAR		szPropName[50];					//道具名称
    int			attribAction;					//操作属性
    int			attribValue;					//取值属性
    int			iPrice;							//道具价格
    int			iVipPrice;						//VIP道具价格
} MSG_PROP_S_KIND_ROOMCARD;


//  C --> S
typedef struct
{
    int       dwUserID;						//用户ID号
    int		   nPropID;							// 0 为所有
} MSG_PROP_C_QUERY_ROOMCARD;

//查询房卡信息  S -> C
typedef struct
{
    int         dwUserID;						//用户ID号
    int			nPropID;						//道具ID号
    int			nHoldCount;						//拥有道具的数量
    CHAR		szPropName[50];					//道具名称
    int			attribAction;					//操作属性
    int			attribValue;					//取值属性
    int			iPrice;							//道具价格
    int			iVipPrice;						//VIP道具价格
    
} MSG_PROP_S_QUERY_ROOMCARD;


//购买房卡 C -> S
typedef struct
{
    int         dwUserID;						//用户ID号
    int			nPropID;						//道具ID号
    int			nPropCount;						//购买的数量
} MSG_PROP_C_BUY_ROOMCARD;

//购买房卡 S -> C
typedef struct
{
    int         dwUserID;						//用户ID号
    int			nPropID;						//道具ID号
    int			nHoldCount;						//拥有道具的数量
    CHAR		szPropName[50];					//道具名称
    int			attribAction;					//操作属性
    int			attribValue;					//取值属性
    LLONG		dwCost;							//当前该笔交易花了多少钱，由ZXD添加，服务器须提供此数据
    LLONG       dwBuyPropGiveCoins;             //购买道具返还的金币数额
    int			iPrice;							//道具价格
    int			iVipPrice;						//VIP道具价格
} MSG_PROP_S_BUY_ROOMCARD;


#pragma  pack(1)

typedef struct
{
    UINT                 uRoomID;
    UINT				  uDeskIndex;
    INT						  dwInitiator; //发起者
    INT						  dwRespondent;//响应者
    BYTE						  cbAgree;
} MSG_GR_C_BACKROOM_RSP_DISMISS, MSG_GR_S_BACKROOM_RSP_DISMISS;

typedef struct
{
    UINT                  uRoomID;
    UINT				  uDeskIndex;
    INT                   dwInitiator; //发起者
} MSG_GR_C_BACKROOM_REQ_DISMISS, MSG_GR_S_BACKROOM_REQ_DISMISS;


typedef struct
{
    UINT                      uRoomID;
    UINT                      uDeskIndex;
    int						  dwInitiator; //发起者
} MSG_GR_S_BACKROOM_DISMISS_NOTICE;

#pragma  pack(0)

#endif	//_YZ_RoomMessage_H__
