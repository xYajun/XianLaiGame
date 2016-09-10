#ifndef __ErMahjong_MessageHead_H__
#define __ErMahjong_MessageHead_H__

#include "YZNetExport.h"
#include "ErMahjongDefine.h"

//”Œœ∑–≈œ¢
#define PLAYER_COUNT					4								//”Œœ∑»À ˝
#define GAME_PLAYER						4								//”Œœ∑»À ˝

//≥£¡ø∂®“Â
#define MAX_WEAVE					4									//◊Ó¥Û◊È∫œ
#define MAX_INDEX					42									//◊Ó¥ÛÀ˜“˝
#define MAX_COUNT					14									//◊Ó¥Û ˝ƒø
#define MAX_REPERTORY				112									//◊Ó¥Ûø‚¥Ê
#define MAX_ZMCOUNT                 8								//”Œœ∑»À ˝

#pragma pack(1)

namespace ErMahjong
{
//”Œœ∑ID
	enum ErMahjong_Game_ID
	{
		GAME_NAME_ID = 20600400
	};

	//”Œœ∑÷∏¡ÓID
	enum ErMahjong_CMD_ID
	{		
		SUB_S_GAME_START			=	100,									//”Œœ∑ø™ º
		SUB_S_OUT_CARD				=	101,								//≥ˆ≈∆√¸¡Ó
		SUB_S_SEND_CARD				=	102,									//∑¢ÀÕ∆ÀøÀ
		SUB_S_LISTEN_CARD			=	103,									//Ã˝≈∆√¸¡Ó
		SUB_S_OPERATE_NOTIFY		=	104,									//通知玩家操作
		SUB_S_OPERATE_RESULT		=	105,									//通知玩家操作结果
		SUB_S_GAME_END				=	106,									//”Œœ∑Ω· ¯
		SUB_S_TRUSTEE				=	107,									//”√ªßÕ–π‹
		SUB_S_GAME_YAZI				=	108,

		SUB_S_SEZI					=	109,
		SUB_S_GAME_MESSAGE			=	110,
		SUB_S_FEE					=	111,
        SUB_S_LISTEN_CARD_NOTICE    =   112,
		SUB_C_OUT_CARD				=	151,									//≥ˆ≈∆√¸¡Ó
		SUB_C_LISTEN_CARD			=	152,									//Ã˝≈∆√¸¡Ó
		SUB_C_OPERATE_CARD			=	153,									//≤Ÿ◊˜∆ÀøÀ
		SUB_C_TRUSTEE				=	154,									//”√ªßÕ–π‹
		SUB_C_YAZI					=	155,
	};

	//”Œœ∑◊¥Ã¨
	enum ErMahjong_Game_State
	{	
		GS_WAIT_SETGAME				=	0,				//µ»¥˝∂´º“…Ë÷√◊¥Ã¨
		GS_WAIT_ARGEE				=	1,				//µ»¥˝Õ¨“‚…Ë÷√
		GS_MJ_YAZI					=	20,				//—∫◊”Ω◊∂Œ
		GS_MJ_PLAY					=   21,				//”Œœ∑◊¥Ã¨
		GS_WAIT_NEXT				=	23,				//µ»¥˝œ¬“ª≈Ãø™ º
		GS_PLAYING					=	29,	//∏˜ÕÊº“Ω¯––¥Ú≈∆◊¥Ã¨
        GS_GAMEEND                  =   30,
	};

    
    struct CMD_WeaveItem
    {
        BYTE							cbWeaveKind;            //类型
        BYTE							cbCenterCard;           //中心牌
        BYTE							cbPublicCard;           //名牌
        WORD							wProvideUser;           //提供者
    };
    
	struct GameStationBase
	{
		BYTE bGameStation;
	};

   	//游戏场景状态
    struct CMD_S_StatusFree : GameStationBase
    {
        //游戏版本
        BYTE							iVersion;							//游戏版本号
        BYTE							iVersion2;						    //游戏版本号
        INT                             lCellScore;							//基础金币
        WORD							wBankerUser;						//庄家用户
        WORD							wServerType;						//房间类型
        bool							bTrustee[GAME_PLAYER];				//是否托管
        BYTE							cbSiceCount[2];						//骰子点数
        int								m_iPlayGameRounds;					//“—¥Úæ÷ ˝
        int								m_iTotalGameRounds;					//◊‹æ÷ ˝
    };
    
    struct CMD_S_StatusYaZi : GameStationBase
    {
        BYTE							iVersion;							//”Œœ∑∞Ê±æ∫≈
        BYTE							iVersion2;						    //”Œœ∑∞Ê±æ∫≈
        bool							bPower;
        bool							bXianBan;
        signed int						lCellScore;
        int								m_iPlayGameRounds;					//“—¥Úæ÷ ˝
        int								m_iTotalGameRounds;					//◊‹æ÷ ˝
    };
    
    struct CMD_S_StatusPlay : GameStationBase
    {
        BYTE							iVersion;                                   //”Œœ∑∞Ê±æ∫≈
        BYTE							iVersion2;                                  //”Œœ∑∞Ê±æ∫≈
        signed int						lCellScore;									//µ•‘™ª˝∑÷
        BYTE							cbSiceCount[2];								//˜ª◊”µ„ ˝
        WORD							wBankerUser;								//庄家
        WORD							wCurrentUser;								//当前出牌用户
        WORD							wServerType;								//∑øº‰¿‡–Õ
        
        BYTE							cbActionCard;								//动作牌
        BYTE							cbActionMask;								//动作掩码
        BYTE							cbHearStatus[GAME_PLAYER];					//状态
        BYTE							cbLeftCardCount;							//剩余牌数量
        BYTE							cbMinusLastCount;							//
        bool							bTrustee[GAME_PLAYER];						//托管状态
        
        WORD							wOutCardUser;								//当前出牌用户
        BYTE							cbOutCardData;								//当前打出的牌
        BYTE							cbDiscardCount[GAME_PLAYER];				//打出的牌的数量
        BYTE							cbDiscardCard[GAME_PLAYER][60];				//打出的牌
        BYTE							cbHuaCardCount[GAME_PLAYER];				//ª® ˝ƒø
        BYTE							cbHuaCardData[GAME_PLAYER][8];				//ª®º«¬º
        WORD							m_CurrentYaZi[GAME_PLAYER];
        bool							m_bXiaBan[GAME_PLAYER];
        
        BYTE							cbCardCount;								//∆ÀøÀ ˝ƒø
        BYTE							cbCardData[MAX_COUNT];						//当前玩家手牌
        BYTE							cbSendCardData;								//∑¢ÀÕ∆ÀøÀ
        BYTE							cbDaCardData;								//¥Ó≈∆∆ÀøÀ
        BYTE							cbFanDaCardData;							//∑≠¥Ó≈∆∆ÀøÀ
        BYTE							cbFanDaCardPoint;							//∑≠¥Ó≈∆Œª÷√
        BYTE							cbFetchCardPoint;						    //◊•≈∆ø™ ºŒª÷√
        WORD							wFanCardChair;						        //ø™≈∆ÕÊº“Œª÷√
        WORD							wDaCardChair;							    //¥Ó≈∆ÕÊº“Œª÷√
        
        
        
        BYTE							cbWeaveCount[GAME_PLAYER];					//◊È∫œ ˝ƒø
        CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//◊È∫œ∆ÀøÀ
        int								m_iPlayGameRounds;					//“—¥Úæ÷ ˝
        int								m_iTotalGameRounds;					//◊‹æ÷ ˝
        int								m_cbZaCodeNum;
    };
    
    
    //游戏开始
    struct CMD_S_GameStart
    {
        BYTE							cbSiceCount[2];								//骰子点数
        WORD							wBankerUser;								//庄家用户
        WORD							wCurrentUser;								//当前用户
        WORD							wFanCardUser;								//开牌玩家
        BYTE							cbUserAction;								//用户动作
        BYTE							cbCardData[MAX_COUNT];						//扑克列表
        BYTE							cbDaCardData;								//搭牌扑克 真正的财神牌
        BYTE							cbFanDaCardData;							//翻搭牌扑克 翻出来的财神牌
        BYTE							cbFetchCardPoint;							//取牌位置
        BYTE							cbFanDaCardPoint;							//翻搭牌位置
        bool							bTrustee[GAME_PLAYER];						//是否托管
        WORD							m_cbLeftCardCount;
        //BYTE							cbYaZi[GAME_PLAYER];
        //bool							bXiaBan[GAME_PLAYER];
        INT                             lCellScore;
        int								m_iPlayGameRounds;			//“—¥Úæ÷ ˝
        int								m_iTotalGameRounds;			//◊‹æ÷
        int								m_cbZaCodeNum;
    };
    
    struct CMD_S_ListenCard1
    {
        WORD                             wListenUser;						//听牌用户
        BYTE							cbListenCardData[28];				//听牌数据
    };
    
    struct CMD_S_GameYaZi
    {
        bool							bCanXiaBan;
        BYTE							cbCurrentBanker;
        signed int						lCellScore;
    };
    struct CMD_S_GameSeZi
    {
        BYTE							cbSiceCount[2];
    };
    
    //出牌命令
    struct CMD_S_OutCard
    {
        WORD							wOutCardUser;						//出牌用户
        BYTE							cbOutCardData;						//出牌扑克
        BYTE							cbRspcode;							//出牌状态  1 成功 2 吃碰限制 3 不能吃碰啥打啥 4 不能打出财神
    };
    
    //∑¢ÀÕ∆ÀøÀ
    struct CMD_S_SendCard
    {
        bool							bGangStatus;						//∏‹≈∆◊¥Ã¨
        BYTE							cbCardData;							//∆ÀøÀ ˝æ›
        BYTE							cbActionMask;						//∂Ø◊˜—⁄¬Î
        BYTE							cbMinusLastCount;					//Œ≤≤øø’»±
        WORD							wCurrentUser;						//µ±«∞”√ªß
        WORD							wLeftCardCount;
    };
    
    //Ã˝≈∆√¸¡Ó
    struct CMD_S_ListenCard
    {
        WORD							wListenUser;						//Ã˝≈∆”√ªß
    };
    
    //动作通知
    struct CMD_S_OperateNotify
    {
        WORD							wResumeUser;						//ªπ‘≠”√ªß
        BYTE							cbActionMask;						//∂Ø◊˜—⁄¬Î
        BYTE							cbActionCard;						//∂Ø◊˜∆ÀøÀ
    };
    
    //操作命令
    struct CMD_S_OperateResult
    {
        WORD							wOperateUser;						//操作用户
        WORD							wProvideUser;						//供应用户
        BYTE							cbOperateCode;						//操作代码
        BYTE							cbOperateCard;						//操作扑克
        bool							bDarkGuang;							//暗杠标志
    };
    
    //游戏结束
    struct CMD_S_GameEnd
    {
        signed int						lGameTax;							//游戏税收
        //结束信息
        WORD							wBaoCardUser;						//包牌用户
        WORD							wProvideUser;						//供应用户 自摸。 放炮
        BYTE							cbProvideCard;						//供应扑克
        INT							dwChiHuKind[GAME_PLAYER];               //胡牌类型
        INT							dwChiHuRight[GAME_PLAYER];              //胡牌类型  自摸
        //积分信息
        INT								lGameScore[GAME_PLAYER];			//游戏积分
        INT						        iChangeMoney[GAME_PLAYER];			//游戏实际分
        INT								dwWinTimes;							//龙数
        BYTE							cbContinueNT;						//连庄
        UINT						lGameTaiCount;						//游戏台数
        UINT						lPengGangTaiCount;					//台数目
        UINT						lHuaTaiCount;						//台数目
        UINT						lDaTaiCount;						//台数目
        //扑克信息
        BYTE							cbCardCount[GAME_PLAYER];			//扑克数目
        BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//扑克数据
        bool							cbLiuJu;
        BYTE							cbYaZi[GAME_PLAYER];
        bool							bXiaBan[GAME_PLAYER];
        BYTE							cbLeftCardData[MAX_REPERTORY];
        BYTE							cbLeftCardCount;
        BYTE							cbWeaveCount[GAME_PLAYER];					//组合数目
        CMD_WeaveItem					WeaveItemArray[GAME_PLAYER][MAX_WEAVE];		//组合扑克
        BYTE							cbZaCardData[MAX_ZMCOUNT];

    };
    
    
    struct CMD_S_Trustee
    {
        bool							bTrustee;							// «∑ÒÕ–π‹
        WORD							wChairID;							//Õ–π‹”√ªß
    };
    struct CMD_S_GameMessage
    {
        char							szMessage[500];
    };
    struct CMD_S_Fee
    {
        signed int						lGameFee;
    };
    
    //====================客户端发送给服务器的指令==============-//
    
    struct CMD_C_OutCard
    {
        BYTE							cbCardData;							//∆ÀøÀ ˝æ›
    };
    

    struct CMD_C_OperateCard
    {
        BYTE							cbOperateCode;						//≤Ÿ◊˜¥˙¬Î
        BYTE							cbOperateCard;						//≤Ÿ◊˜∆ÀøÀ
    };

    struct CMD_C_Trustee
    {
        bool							bTrustee;							// «∑ÒÕ–π‹
    };
    
    struct CMD_C_YaZi
    {
        BYTE							cbYaZi;
        bool							bXiaBan;
    };
    
    //=========================================================//

	enum ErMahjong_Game_Time
	{
		WAITAGREE_TIME   = 30,
		MAKEDI_TIME      = 25,
		OUTCARD_TIME     = 20,
		ACTION_TIME      = 10,
		THINK_TIME      =  15,
	};

    
    enum ActionType{
        WIK_NULL			=		0x00,								//没有类型
        WIK_LEFT			=		0x01,								//左吃类型
        WIK_CENTER			=		0x02,								//中吃类型
        WIK_RIGHT			=		0x04,								//右吃类型
        WIK_PENG			=		0x08,								//碰牌类型
        WIK_GANG			=		0x10,								//杠牌类型
        WIK_LISTEN			=		0x20,								//听牌类型
        WIK_CHI_HU			=		0x40,								//吃胡类型
    };
    
    
    //返回游戏
    struct BackGameData{
        BYTE							cbActionCard;								//动作牌
        BYTE							cbActionMask;								//动作掩码
        WORD							wCurrentUser;								//当前出牌用户
        WORD							wBankerUser;								//庄家
        bool							bTrustee[GAME_PLAYER];						//托管状态
        WORD							wOutCardUser;								//当前出牌用户
        BYTE							cbOutCardData;								//当前打出的牌
        std::map<BYTE , std::vector<BYTE>> cbDusCardMaps;                           //所有玩家打出的牌   Key 座位号
        std::map<BYTE , std::vector<BYTE>> cbHandCardsMaps;                          //所有玩家手牌列表
        BYTE							cbSendCardData;								//
        std::map<BYTE , std::vector<CMD_WeaveItem>> cbWeaveItemMaps;                //所有玩家动作牌数据  Key 座位号
    };
    

	//ƒ≥ÕÊº“ «∑Ò“‚µ◊◊¢µƒœ˚œ¢∞¸
	struct TAgree 
	{
		BYTE byUser; //œ˚œ¢À˘ ÙµƒÕÊº“
		bool bAgree; // «∑ÒÕ¨“‚
		bool bAllAgree; //µ±«∞ «∑ÒÀ˘”–ÕÊº“∂ºÕ¨“‚”Œœ∑

		TAG_INIT(TAgree)
		{
			byUser = 255; //œ˚œ¢À˘ ÙµƒÕÊº“
			bAgree = true; // «∑ÒÕ¨“‚
			bAllAgree = false; //µ±«∞ «∑ÒÀ˘”–ÕÊº“∂ºÕ¨“‚”Œœ∑
		}
	};
}

#pragma pack(0)

#endif