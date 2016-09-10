#ifndef __ErMahjong_CARD_OUT_LIST_H__
#define __ErMahjong_CARD_OUT_LIST_H__

#include "YZNetExport.h"
#include "ErMahjongMessageHead.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ErMahjongCard.h"
namespace ErMahjong
{
	class GameTableUI;

	//////////////////////////////////////////////////////////////////////////
	class CardOutList: public YZ::YZLayer
	{
	public:
		static CardOutList* create(BYTE seatNo);
		virtual bool init(BYTE seatNo);

        void initBoards();
        
        void addAllCard2OutListOneTimes(std::vector<BYTE> cards);
        
        void addCard2OutList(MJOutCard card);
        
        void setCallFunction(std::function<void(cocos2d::Vec2)> _outCardFinish);
        
        //void addCard2OutListEnd(bool isRemove);
        void clear();
        
        void removeLastCard();
    
        void removeLastCardZhiShiSp();
    protected:
        
        void setMJBoardDirection(MJPostion direction);
		CardOutList();
		virtual ~CardOutList();
        MJPostion _direction;  //方位
        Vec2 _cardPoint[MAX_COUNT * 2];
        Sprite * cardSprite[MAX_COUNT];
        std::vector<BYTE> _cardsPtr;
        
        std::function<void (Vec2)> _outCardEndFunc;
        
        
        Vector<Sprite*> cardSps;
        
        string getCardTextureFileName(BYTE cardValue,MJPostion pos);
        
        Sprite* _lastOutCard;
    };
}

#endif