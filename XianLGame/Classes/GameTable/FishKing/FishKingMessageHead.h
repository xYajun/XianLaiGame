#ifndef __FishKing_MessageHead_h__
#define __FishKing_MessageHead_h__

#include "YZNetExport.h"

namespace FishKing
{
//////////////////////////////////////////////////////////////////////////
//#define GAME_NAME             TEXT("渔王争霸")
//////////////////////////////////////////////////////////////////////////

enum FishKing_CONST
{
	GAME_NAME_ID			=			33003206,					//游戏 ID
	PLAY_COUNT			    =			6,							//游戏人数
	//GAME_PLAYER				=			8,

	//游戏状态
	GAME_STATUS_FREE		=			0,							//空闲状态
	GAME_STATUS_PLAY		=			100,						//游戏状态
	GAME_STATUS_WAIT		=			200,						//等待状态

	INVALID_FISHID			=			0,							//错误鱼id
};

const FLOAT kResolutionWidth = 1440;
const FLOAT kResolutionHeight = 900;

/*------------------------------------------------------------------------------*/
#define DEF_SCREEN_WIDTH	  1440			// 默认屏宽
#define DEF_SCREEN_HEIGHT	  900			// 默认屏高

/*------------------------------------------------------------------------*/
//游戏窗口大小
static float screen_width_	= DEF_SCREEN_WIDTH;
static float screen_height_ = DEF_SCREEN_HEIGHT;
//电脑显示屏大小
static int screen_width_PC	= DEF_SCREEN_WIDTH;
static int screen_height_PC = DEF_SCREEN_HEIGHT;

static	float g_hscale = 1.0f;
static	float g_vscale = 1.0f;
/*------------------------------------------------------------------------------*/
	
/*
#ifndef SCORE
#define SCORE __int64
#endif
*/

//#ifndef M_PI
//#define M_PI    3.14159265358979323846f
//#define M_PI_2  1.57079632679489661923f
//#define M_PI_4  0.785398163397448309616f
//#define M_1_PI  0.318309886183790671538f
//#define M_2_PI  0.636619772367581343076f
//#endif
//const float kChairDefaultAngle[GAME_PLAYER] = { M_PI, M_PI, M_PI, 0, 0, 0, -M_PI_2, M_PI_2 };

struct Position {
	float x;
	float y;
};

enum TraceType {
  TRACE_LINEAR = 0,
  TRACE_BEZIER
};

struct FPoint {
  float x;
  float y;
};

struct FPointAngle {
  float x;
  float y;
  float angle;
};

enum SceneStyle {
	SCENE_STYLE_1 = 0,
	SCENE_STYLE_2,
	SCENE_STYLE_3,

	SCENE_STYLE_COUNT
};

//////////////////////////////////////////////////////////////////////////
// 游戏定义
//////////////////////////////////////////////////////////////////////////

/*
// 座位号
-------------
  0   1   2 
               
  5   4   3
-------------
*/

enum SceneKind {
  SCENE_KIND_1 = 0,
  SCENE_KIND_2,
  SCENE_KIND_3,
  SCENE_KIND_4,
  SCENE_KIND_5,

  SCENE_KIND_COUNT
};


enum FishKind 
{
	FISH_KIND_1 = 0 ,		//蜗牛鱼
	FISH_KIND_2,			//绿草鱼
	FISH_KIND_3,			//黄草鱼
	FISH_KIND_4,			//大眼鱼
	FISH_KIND_5,			//黄扁鱼
	FISH_KIND_6,			//小丑鱼
	FISH_KIND_7,			//小刺鱼
	FISH_KIND_8,			//蓝鱼
	FISH_KIND_9,			//红灯笼鱼
	FISH_KIND_10,			//绿灯笼鱼
	FISH_KIND_11,			//海龟
	FISH_KIND_12,			//花瓣鱼
	FISH_KIND_13,			//蝴蝶鱼
	FISH_KIND_14,			//孔雀鱼
	FISH_KIND_15,			//剑鱼
	FISH_KIND_16,			//蝙蝠鱼
	FISH_KIND_17,			//银鲨鱼
	FISH_KIND_18,			//金鲨鱼
	FISH_KIND_19,			//金箍棒
	FISH_KIND_20,			//大鲨鱼
	FISH_KIND_21,			//银龙
	FISH_KIND_22,			//金龙
	FISH_KIND_23,			//海藻
	FISH_KIND_24,			//全屏炸弹
	FISH_KIND_25,			//金色金蟾
	FISH_KIND_26,			//双头鲨

	FISH_KIND_27,			//三角大三元
	FISH_KIND_28,			//直线大三元
	FISH_KIND_29,			//四角大四喜
	FISH_KIND_30,			//直线大四喜

	FISH_KIND_31,			//红鱼--鱼王
	FISH_KIND_32,			//带盘子的鱼-鱼王
	FISH_KIND_COUNT
};

//对应鱼生成的金币数量
enum Fish_Coin_Count
{
	FISH_1_COIN_COUNT = 2,
	FISH_2_COIN_COUNT = 3,
	FISH_3_COIN_COUNT = 4,
	FISH_4_COIN_COUNT = 5,
	FISH_5_COIN_COUNT = 6,
	FISH_6_COIN_COUNT = 7,
	FISH_7_COIN_COUNT = 8,
	FISH_8_COIN_COUNT = 9,
	FISH_9_COIN_COUNT = 10,
	FISH_10_COIN_COUNT = 11,
	FISH_11_COIN_COUNT = 12,
	FISH_12_COIN_COUNT = 13,
	FISH_13_COIN_COUNT = 14,
	FISH_14_COIN_COUNT = 15,
	FISH_15_COIN_COUNT = 16,
	FISH_16_COIN_COUNT = 17,
	FISH_17_COIN_COUNT = 18,
	FISH_18_COIN_COUNT = 19,
	FISH_19_COIN_COUNT = 20,
	FISH_20_COIN_COUNT = 21,
	FISH_21_COIN_COUNT = 22,
	FISH_22_COIN_COUNT = 23,
	FISH_23_COIN_COUNT = 24,
	FISH_24_COIN_COUNT = 25,
	FISH_25_COIN_COUNT = 26,
	FISH_26_COIN_COUNT = 27,
	FISH_27_COIN_COUNT = 28,
	FISH_28_COIN_COUNT = 29,
	FISH_29_COIN_COUNT = 30,
	FISH_30_COIN_COUNT = 31,
	FISH_31_COIN_COUNT = 32,
};

//打死指定鱼 生成多少个鱼币个数
static unsigned int	G_Fish_Coin_Count[FISH_KIND_COUNT] = 
{
	FISH_1_COIN_COUNT,
	FISH_2_COIN_COUNT,
	FISH_3_COIN_COUNT,
	FISH_4_COIN_COUNT,
	FISH_5_COIN_COUNT,
	FISH_6_COIN_COUNT,
	FISH_7_COIN_COUNT,
	FISH_8_COIN_COUNT,
	FISH_9_COIN_COUNT,
	FISH_10_COIN_COUNT,
	FISH_11_COIN_COUNT,
	FISH_12_COIN_COUNT,
	FISH_13_COIN_COUNT,
	FISH_14_COIN_COUNT,
	FISH_15_COIN_COUNT,
	FISH_16_COIN_COUNT,
	FISH_17_COIN_COUNT,
	FISH_18_COIN_COUNT,
	FISH_19_COIN_COUNT,
	FISH_20_COIN_COUNT,
	FISH_21_COIN_COUNT,
	FISH_22_COIN_COUNT,
	FISH_23_COIN_COUNT,
	FISH_24_COIN_COUNT,
	FISH_25_COIN_COUNT,
	FISH_26_COIN_COUNT,
	FISH_27_COIN_COUNT,
	FISH_28_COIN_COUNT,
	FISH_29_COIN_COUNT,
	FISH_30_COIN_COUNT,
	FISH_31_COIN_COUNT,
};

//环形渔政个数
static	int G_Fish_Circle_Count[FISH_KIND_COUNT] = {60,60,50,35,30,35,30,25,20,20,10,8,0,0,0,0,0,0,0,0,0,};

enum BulletKind 
{
  BULLET_KIND_1_NORMAL = 0,
  BULLET_KIND_2_NORMAL,
  BULLET_KIND_3_NORMAL,
  BULLET_KIND_4_NORMAL,
  BULLET_KIND_1_ION,
  BULLET_KIND_2_ION,
  BULLET_KIND_3_ION,
  BULLET_KIND_4_ION,

  BULLET_KIND_COUNT
};

const INT kMaxCatchFishCount = 2;

#pragma pack(1)

struct CMD_S_GameStatus 
{
  UINT 						game_version;
  SceneStyle				Scene_Style_Kind;					//场景背景ID
  INT						iUserFireBase[8];			//玩家炮火倍数
  INT	                    iUserFireCell;
  LLONG 					fish_score[8];			//玩家身上金币数
  LLONG 					exchange_fish_score[8];	//玩家身上兑换鱼币数，此游戏没什么用
};

struct CMD_S_GameConfig 
{
	INT 						exchange_ratio_userscore;
	INT 						exchange_ratio_fishscore;
	INT 						exchange_count;

	INT 						min_bullet_multiple;
	INT 						max_bullet_multiple;

	INT 						bomb_range_width;
	INT 						bomb_range_height;

	INT 						fish_multiple[FISH_KIND_COUNT];
	INT 						fish_speed[FISH_KIND_COUNT];
	INT 						fish_bounding_box_width[FISH_KIND_COUNT];
	INT 						fish_bounding_box_height[FISH_KIND_COUNT];
	INT 						fish_hit_radius[FISH_KIND_COUNT];

	INT 						bullet_speed[BULLET_KIND_COUNT];
	INT 						net_radius[BULLET_KIND_COUNT];
};

struct GameStation//断线重回用的数据包sdp
{
	//游戏版本
	BYTE					iVersion;						//游戏版本号
	BYTE					iVersion2;						//游戏版本号
	BYTE					game_status;					//状态标识

	CMD_S_GameStatus		GameStatus;						//状态数据
	CMD_S_GameConfig		GameConfig;						//配置数据

	BYTE GameMessage[260];//游戏消息
};

//玩家进入  主要是把玩家的金币数发送过来
struct CMD_S_GameUserCome
{
	UINT					dwDeskStation;
	LLONG 					i64fish_score;

	CMD_S_GameUserCome()
	{
		memset(this, 0, sizeof(CMD_S_GameUserCome));
		dwDeskStation = 255;
	}
};

struct CMD_S_FishTrace 
{
	FPoint 					init_pos[5];
	INT 					init_count;
	FishKind 				fish_kind;			//鱼的主类型
	FishKind 				fish_kind_ex[5];	//鱼的辅助类型 
	INT 					fish_id;
	TraceType 				trace_type;

	void init()
	{
		memset(this, 0, sizeof(CMD_S_FishTrace));
	}
};

struct	CMD_S_CirleFishGroup
{
	bool					bHave;
	FishKind				iFishKind;
	INT						iIndex;
	INT						iCount;
	INT						iRedIndex;
	INT						iRedIndexIndex;

	CMD_S_CirleFishGroup()
	{
		bHave = false;
		iCount = 0;
		iFishKind = FISH_KIND_COUNT;
		iIndex = -1;
		iRedIndex = 0;
		iRedIndexIndex = 0;
	}
};

struct CMD_S_ExchangeFishScore 
{
	WORD 					chair_id;
	LLONG 					swap_fish_score;//当前玩家实际鱼币数 而不是增量
	LLONG 					exchange_fish_score;
};

struct CMD_S_UserFire 
{
	BulletKind 				bullet_kind;			//子弹类型
	INT 					bullet_id;				//子弹id
	WORD 					chair_id;				//座位号
	WORD 					android_chairid;		//机器人座位号
	FLOAT 					angle;					//开炮角度
	INT 					bullet_mulriple;		//炮火倍数
	INT 					lock_fishid;			//锁定鱼的id
	LLONG 					fish_score;				//鱼的奖励金币
	bool 					bIsRobot;				//是否机器人

	CMD_S_UserFire()
	{
		memset(this, 0, sizeof(CMD_S_UserFire));
	}
};

struct CMD_S_CatchFish 
{
	WORD 					chair_id;			//座位号
	INT 					fish_id;			//鱼id
	FishKind 				fish_kind;			//鱼类型
	bool 					bullet_ion;			//是否能量炮
	LLONG 					fish_score;			//打中鱼奖励金币
	LLONG					i64UserFishScore_;	//玩家身上总金币
};

struct CMD_S_BulletIonTimeout 
{
	WORD 					chair_id;
};

struct CMD_S_CatchSweepFish 
{
	BYTE					byIndex;
	WORD 					chair_id;
	INT 					fish_id;
	LLONG 					i64FishSocre;
	CMD_S_CatchSweepFish()
	{
		memset(this,0,sizeof(CMD_S_CatchSweepFish));
	}	
};

struct CMD_S_CatchSweepFishResult 
{
	WORD					chair_id;
	INT						fish_id;
	INT						catch_fish_count;
	INT						catch_fish_id[200];
	INT						fish_score[200];
	LLONG					fish_score_Sum;
	LLONG					i64UserFishScore_;
};

struct CMD_S_CatchSweepFishResultEx 
{
	CHAR    				cRoomName[61];				//房间名字
	INT						exchange_ratio_userscore_;	//换算比例中用户金币
	INT						exchange_ratio_fishscore_;	//换算比例中鱼币
	INT						iDeskID;					//桌子ID
	CHAR					szNickName[61]; 			//昵称
	INT						fish_kind;					//鱼类型ID
	LLONG					fish_score;					//打中鱼赚多少钱
	CHAR    				cFishName[260];				//鱼的名字
};

struct CMD_S_HitFishLK 
{
	WORD 					chair_id;
	INT 					fish_id;
	INT 					fish_mulriple;
};

struct CMD_S_SwitchScene 
{
	SceneKind 				scene_kind;
	SceneStyle				Scene_Style;	//场景ID
	INT 					fish_count;
	INT 					fish_id[245];
	FishKind 				fish_kindEx[10];
	FishKind 				fish_kind[245];

	CMD_S_SwitchScene()
	{
		memset(this, 0x0, sizeof(CMD_S_SwitchScene));
	}
};

struct CMD_S_GroupFishTrace
{
	BYTE 					byIndex;
	INT 					fish_count;
	FishKind 				fish_kind[250];
	FishKind 				fish_red_kind[5];		//单独标记红色鱼王
	INT 					fish_id[250];
};

struct CMD_C_ExchangeFishScore 
{
	bool increase;
};

struct CMD_C_UserFire 
{
	BulletKind 				bullet_kind;
	FLOAT 					angle;
	INT 					bullet_mulriple;
	INT 					lock_fishid;
	INT 					bullet_id;
	bool 					bIsRobot;

	CMD_C_UserFire()
	{
		memset(this, 0, sizeof(CMD_C_UserFire));
	}
};

struct CMD_C_CatchFish 
{
	WORD 					chair_id;
	INT 					fish_id;
	BulletKind 				bullet_kind;
	INT 					bullet_id;
	INT 					bullet_mulriple;
};

struct CMD_C_CatchSweepFish 
{
	WORD 					chair_id;
	INT 					fish_id;
	INT 					catch_fish_count;
	INT 					catch_fish_id[300];

	CMD_C_CatchSweepFish()
	{
		memset(this, 0, sizeof(CMD_C_CatchSweepFish));
	}
};

struct CMD_C_HitFishLK
{
	INT 					fish_id;
};



struct CMD_C_Fish20Config 
{
	UINT 					game_id;
	INT 					catch_count;
	double 					catch_probability;
};

struct CMD_C_AndroidBulletMul 
{
	WORD					chair_id;
	INT						bullet_id;
	INT						bullet_mulriple;
	BulletKind				bullet_kind;
};

//连锁闪电
struct CMD_C_LineInfo
{
	FPoint	fpStart;
	FPoint	fpEnd;
	FLOAT	fChange;
	UINT	dwTime;
	
	CMD_C_LineInfo()
	{
		memset(this, 0, sizeof(CMD_C_LineInfo));
	}
};


/////////////////////////////////////////////////////////////////////////////
//超端控制
/////////////////////////////////////////////////////////////////////////////

//奖池操作结果
struct CMD_S_StockOperateResult		
{
	BYTE 					operate_code;	//	0查询 1 清除 2 增加 3 查询抽水
	LLONG 					stock_score;	//	操作结果
};

//奖池操作
struct CMD_C_StockOperate 
{
	BYTE 					operate_code;	// 0查询 1 清除 2 增加 3 查询抽水
};

struct CMD_C_UserFilter
{
	UINT 					game_id;
	BYTE 					operate_code;	// 0 黑名单 1 白名单 2 清除
};

struct S_C_SUPER_CONFIG
{
	bool bSuperUser;
};

//在线玩家信息
struct OnLineUserInfo
{
	BYTE	deskNo;
	char	nickName[61];
	int		i64UserMoney;
	UINT	userID;
	OnLineUserInfo()
	{
		memset(this, 0, sizeof(OnLineUserInfo));
	}
};

struct S_C_SWITCH_INFO
{
	bool special_switch_;				//特殊鱼控制开关
	bool control_switch_;				//玩家概率控制开关
	S_C_SWITCH_INFO()
	{
		memset(this, 0, sizeof(S_C_SWITCH_INFO));
	}
};


struct S_C_FISH_NAME
{
	BYTE	byFishKind;
	char	cFishName[4][260];
};

struct S_C_ON_LINE_USER
{	
	bool	bStart;
	OnLineUserInfo user_info_;
	S_C_ON_LINE_USER()
	{
		memset(this, 0, sizeof(S_C_ON_LINE_USER));
	}
};

struct SpecialFish
{
	BYTE fish_kind_;					//鱼类型
	int left_num_;						//剩余数量
	int catch_rate_;					//抓捕概率
	SpecialFish()
	{
		memset(this, 0, sizeof(SpecialFish));
	}
};

struct SpecialUser
{
	SpecialFish special_fish_[4];		//受控的鱼
	int user_id_;						//受控的玩家id
	SpecialUser()
	{
		memset(this, 0, sizeof(SpecialUser));
		user_id_ = -1;
	}
};

struct ControlInfo
{
	bool win_or_lose_;					//控制玩家赢或输
	int user_id_;						//受控玩家id
	int catch_rate_;					//输赢概率
	LLONG limit_score_;					//输赢上限
	ControlInfo()
	{
		memset(this, 0, sizeof(ControlInfo));
		user_id_ = -1;
	}
};

struct C_S_DELETE_CONTROL_INFO 
{
	int user_id_;
};

struct C_S_ADD_SPECIAL
{
	BYTE fish_kind_;					//鱼类型
	int user_id_;						//玩家id
	int left_num_;						//剩余数量
	int catch_rate_;					//抓捕概率

	C_S_ADD_SPECIAL()
	{
		memset(this,0,sizeof(C_S_ADD_SPECIAL));
	}
};

struct C_S_DELETE_SPECIAL
{
	int user_id_;
	int fish_kind_;
	C_S_DELETE_SPECIAL()
	{
		memset(this,0,sizeof(C_S_DELETE_SPECIAL));
	}
};

//更新控制玩家信息
struct S_C_UPDATE_CONTROL_INFO
{
	ControlInfo control_infos_[40];
	S_C_UPDATE_CONTROL_INFO()
	{
		memset(this, 0, sizeof(S_C_UPDATE_CONTROL_INFO));
	}
};

//更新特殊鱼控制信息
struct S_C_UPDATE_SPECIAL_INFO
{
	SpecialUser special_infos_[20];
	S_C_UPDATE_SPECIAL_INFO()
	{
		memset(this, 0, sizeof(S_C_UPDATE_SPECIAL_INFO));
	}
};

/////////////////////////////////////////////////////////////////////////

#pragma pack()

enum FishKing_COMMAND
{		
//////////////////////////////////////////////////////////////////////////
// 服务端命令
//////////////////////////////////////////////////////////////////////////
	SUB_S_GAME_CONFIG                  = 100,//设置游戏参数
	SUB_S_FISH_TRACE                   = 101,//生成新鱼的轨迹
	SUB_S_FISH_GROUP_TRACE             = 102,//生成鱼群的轨迹
	SUB_S_EXCHANGE_FISHSCORE           = 103,//兑换鱼币（渔夫码头游戏无效）
	SUB_S_USER_FIRE                    = 104,//玩家射击
	SUB_S_CATCH_FISH                   = 105,//抓到鱼
	SUB_S_BULLET_ION_TIMEOUT           = 106,//能量炮失效
	SUB_S_LOCK_TIMEOUT                 = 107,//定屏炸弹失效时间
	SUB_S_CATCH_SWEEP_FISH             = 108,//抓到特殊鱼（炸弹，鱼王）
	SUB_S_CATCH_SWEEP_FISH_RESULT      = 109,//抓到特殊鱼结果
	SUB_S_HIT_FISH_LK                  = 110,//打中李逵（渔夫码头游戏无效）
	SUB_S_SWITCH_SCENE                 = 111,//切换场景，5种鱼阵
	SUB_S_STOCK_OPERATE_RESULT         = 112,//查询库存，抽水
	SUB_S_SCENE_END                    = 113,//场景切换结束
	SUB_S_GAMEOUT					   = 114,//服务端强行让客户端退出
	SUB_S_CATCH_SWEEP_FISH_RESULT_EX   = 115,//广播通知所有玩家捕到大鱼

	SUB_S_USER_COME	                   = 120,//玩家进入

	S_C_UPDATE_CONTROL_INFO_SIG		   = 121,//更新控制概率信息
	S_C_UPDATE_SPECIAL_INFO_SIG		   = 122,//更新控制特殊鱼信息
	S_C_UPDATE_ONLINEUSER_INFO_SIG	   = 123,//更新在线玩家信息
	S_C_SWITCH_INFO_SIG				   = 124,//控制开关信息
	S_C_UPDATE_FISH_NAME_SIG		   = 125,//更新可控制鱼名称
	S_C_SUPER_CONFIG_SIG			   = 126,//是否超端玩家
	S_C_END_UPDATE_ONLINEUSER_INFO_SIG = 127,//更新在线玩家信息列表
	
//////////////////////////////////////////////////////////////////////////
// 客户端命令
//////////////////////////////////////////////////////////////////////////
	SUB_C_EXCHANGE_FISHSCORE           = 1, //兑换鱼币（渔夫码头游戏无效）
	SUB_C_USER_FIRE                    = 2, //玩家开火
	SUB_C_CATCH_FISH                   = 3, //子弹打中鱼
	SUB_C_CATCH_SWEEP_FISH             = 4, //打中特殊鱼，报告当前屏幕中鱼信息
	SUB_C_HIT_FISH_I                   = 5, //打中李逵（渔夫码头游戏无效）
	SUB_C_STOCK_OPERATE                = 6, //查询库存，抽水
	SUB_C_USER_FILTER                  = 7, //开火
	SUB_C_ANDROID_STAND_UP             = 8, //机器人站起（渔夫码头游戏无效）
	SUB_C_FISH20_CONFIG                = 9, //老版超端控制（渔夫码头游戏无效）
	SUB_C_ANDROID_BULLET_MUL           = 10,//机器人子弹数目（渔夫码头游戏无效）

	C_S_UPDATE_SUPER_INFO_SIG		   = 11,//更新所有信息
	C_S_ADD_CONTROL_INFO_SIG		   = 12,//添加概率控制名单
	C_S_DELETE_CONTROL_INFO_SIG		   = 13,//删除概率控制名单
	C_S_ADD_SPECIAL_SIG				   = 14,//添加特殊鱼控制
	C_S_DELETE_SPECIAL_SIG			   = 15,//删除特殊鱼控制
	C_S_SWITCH_CONTROL_SIG		       = 16,//概率控制名单开关
	C_S_SWITCH_SPECIAL_SIG			   = 17,//特殊鱼控制名单开关
};

};

#endif // CMD_FISH_H_