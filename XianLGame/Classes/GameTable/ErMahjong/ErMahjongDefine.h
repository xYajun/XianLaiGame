#ifndef __ErMahjong_Define_h__
#define __ErMahjong_Define_h__
#include "YZNetExport.h"

#define TAG_INIT(tagName) \
	tagName(){Clear();} \
	void Clear()
//”Œœ∑ ˝æ›≥§∂»œ‡πÿ

#define MJ_MAX_CARDS				136		//¬ÈΩ´’≈ ˝
#define MJ_MAX_WALLCARD				34		//≈∆«Ωƒ‹»›ƒ…µƒ¬ÈΩ´øÿº˛ ˝
#define MJ_MAX_WALLDUN				17		//≈∆«Ωƒ‹»›ƒ…µƒ¬ÈΩ´∂’ ˝( ˝÷µ «MJ_MAX_WALLCARDµƒ“ª∞Î)
#define MJ_MAX_HANDCARD				17		// ÷≈∆ƒ‹»›ƒ…µƒ¬ÈΩ´øÿº˛ ˝
#define MJ_MAX_HUA					24		//ª®≈∆µƒ ˝¡ø
#define MJ_MAX_OUTED				68		//◊Ó¥Ûµƒ≥ˆ≈∆ ˝¡ø
#define MJ_MAX_CGP					7		//◊Ó¥Ûµƒ¿∏≈∆ ˝
#define MJ_MAX_HUTYPE				8		//≈∆–Õ÷÷ ˝

#define	MASK_COLOR					0xF0								//ª®…´—⁄¬Î
#define	MASK_VALUE					0x0F								// ˝÷µ—⁄¬Î


//牌权掩码
#define CHR_MASK_SMALL				0x000000FF							//小胡掩码
#define CHR_MASK_GREAT				0xFFFFFF00							//大胡掩码


//小胡牌型
#define CHK_NULL					0x00000000							//非胡类型
#define CHK_JI_HU					0x00000001							//鸡胡类型
#define CHK_PING_HU					0x00000002							//平胡类型
#define CHK_SAN_CAI					0x00000004							//财神会
#define CHK_QI_XIAO_DUI				0x00000008							//小7对
#define CHK_ALL_FENG				0x00000010							//字清 字一色 全风向
#define CHK_SHI_SAN_YAO				0x00000020							//十三幺 十三不搭
#define	CHK_PENG_PENG				0x00000040
#define CHK_SERVER_SHI_SAN_YAO		0x00000080							//七字十三幺 七风齐（十三不搭）
//////////////////////////////////////////////////////////////////////////
//胡牌权位

//大胡权位
#define CHR_DI						0x00000100							//地胡权位
#define CHR_TIAN					0x00000200							//天胡权位
#define CHR_QING_YI_SE				0x00000400							//清一色牌
#define CHR_QIANG_GANG				0x00000800							//抢杠权位
#define CHR_QING_QI_DUI				0x00001000							//清七对
#define CHR_GANG_KAI				0x00002000							//杠开权位
#define CHR_DU_DIAO					0x00004000							//独吊  抛麻花（单吊牌为财神）
#define CHR_HUN_YI_SE				0x00040000							//混一色
#define CHR_ZI_MO					0x00080000							//自摸
#define CHR_PAO_MA_HUA              0x00008000                          //抛花
#define CHR_NONE_DRAGON				0x00010000							//无龙
//////////////////////////////////////////////////////////////////////////

#define GAME_PLAYER						4								//”Œœ∑»À ˝


namespace ErMahjong
{
	namespace MjDirect
	{
		const BYTE East	 = 0;
		const BYTE South = 1;
		const BYTE West	 = 2;
		const BYTE North = 3;
		const BYTE Unknown = 255;
	}
	
	namespace MJColor{
		const BYTE Wan = 0;
		const BYTE Tiao = 1;
		const BYTE Bing = 2;
		const BYTE Feng = 3;
	}
    
    struct MJOutCard{
        BYTE seatNo;
        BYTE cardValue;
        Vec2 cardPoint;
    };
    
    struct MJActionData{
        BYTE acSeatNo;
        BYTE outSeatNo;
        BYTE card ;
        bool bDarkGuang;
    };
    
    struct MJGameEndData{
        bool bLiuJu;
        bool bZiMo;
        int iLongCount;
        BYTE  cbContinueNT;                       //连庄
        BYTE  cbProvideCard;
        int   dwChiHuKind[GAME_PLAYER];
        int   dwChiHuRight[GAME_PLAYER];
        int   iChangeMoney[GAME_PLAYER];
        BYTE  winUser;
        std::vector<BYTE> lastCardVec;      //最后剩下的牌
        std::vector<BYTE> cbZaCardData;     //扎码牌数据
        std::vector<int> recvGangSeats;     //接杠消息
        std::vector<int> giveGangSeats;     //放杠消息
        std::vector<int> lightGangSeats;     //明杠消息
        std::vector<int> darkGangSeats;     //明杠消息
    };
  
    
    enum MJPostion{
		Bottom = 0,
		Right,
		Top,
		Left,
	};
    
    
    //游戏中动作的标识，一般来说动作优先级别越高，开关位越高
    struct MjActFlag
    {
        const BYTE    WIK_NULL        =     0x00;       //没有类型
        const BYTE    WIK_LEFT        =     0x01;       //左吃类型
        const BYTE    WIK_CENTER      =		0x02;		//中吃类型
        const BYTE    WIK_RIGHT       =		0x04;		//右吃类型
        const BYTE    WIK_PENG        =		0x08;		//碰牌类型
        const BYTE    WIK_GANG        =		0x10;		//杠牌类型
        const BYTE    WIK_LISTEN      =		0x20;		//听牌类型
        const BYTE    WIK_CHI_HU      =		0x40;		//吃胡类型
    };
    
    enum PlayerState{
        
        
    };
    
    
}
#endif
