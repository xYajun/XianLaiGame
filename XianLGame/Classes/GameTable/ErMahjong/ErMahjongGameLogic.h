#ifndef __ErMahjong_GameLogic_h__
#define __ErMahjong_GameLogic_h__

#include "YZBaseType.h"
#include "ErMahjongDefine.h"
#include <vector>

namespace ErMahjong
{
	class GameLogic
	{
	public:
		//构造函数		
		GameLogic(void);
		//析构函数
		virtual ~GameLogic(void);
	public:
		//通过牌值返回牌的类型
		BYTE GetKind(BYTE byPs);
		//判断一张牌是否有效
		bool Verify(BYTE byPs);
		//通过牌值返回牌的点数
		BYTE GetDian(BYTE byPs);
		//获取两个随机的数值的色子
	    BYTE CastDice(BYTE& byDice0,BYTE& byDice1);
		//洗牌算法
	    void RandCards(BYTE* pCards,int iNums);
		//删除一张牌
	    bool RemoveOneCard(BYTE byCard,BYTE bySrc[],int iSrcNums);

		//排序
	    void sort(BYTE byCards[], int iNums,bool bFirstBig);

		bool WzFengZiLocked(int nDeskStation,BYTE byHandCard[MJ_MAX_HANDCARD], BYTE iCardCount,int iCaiShen);
	};
}

#endif // GameLogic_h__
