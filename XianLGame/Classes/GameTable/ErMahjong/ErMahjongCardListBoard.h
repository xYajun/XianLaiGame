#ifndef __ErMahjong_Action_Board_H__
#define __ErMahjong_Action_Board_H__

#include "ErMahjongCard.h"

#include "YZNetExport.h"
#include "ErMahjongDefine.h"
#include "ErMahjongMessageHead.h"

namespace ErMahjong
{
	class CardListBoard: public YZ::YZLayer
	{
	public:
		static CardListBoard* create(bool isMySelf, BYTE seatNo);
		virtual bool init(bool isMySelf, BYTE seatNo);

        void initBoards();
        void setMJBoardDirection(MJPostion direction);
	
        void addOneCard(BYTE value);
        void addCards(const std::vector<BYTE>& values);
        void checkAllCardPoints();
        bool getCanOutCard();
        void setCanOutCard(bool iCan);
        MJOutCard getCanOutCardValue();
        void outCard(BYTE cardValue);
        void playerOutCard(BYTE cardValue);
        void setCallFunction(std::function<void(cocos2d::Node*)> callfunc,std::function<void(MJPostion _dir)> finishfunc ,std::function<void (BYTE cardValue)> _outCardFunc);
        void setGangActionEndCallFunc(std::function<void(BYTE seatNo , BYTE cardValue)> callfunc);
        void setCaiShenCard(BYTE card);
        
        void showDarkGuangCard(BYTE card);
        
        void clear();
        
        MahjongCard * createMahjongCard(BYTE value);
        
        int getCardIndex(BYTE card);
        int getPengCardIndex(BYTE card);
        
        Vec2 getTwoCardLength();
        void doUserOperateCard(bool bDarkGuang ,  ActionType actionType , BYTE card);
        void resetAllCardPoint();
        
        void addGuangCard(MahjongCard * card);
        
        void addAnGuangCard(MahjongCard * card);
        
        void showAllWinCard(bool isWin , BYTE winCardValue ,const std::vector<BYTE> values);
        
        bool checkIsPengCard(BYTE card);
        
        void createUserActionCard(bool isDarkGuang , ActionType actionType , BYTE card);
        
        void addAllCardOneTimes(std::vector<BYTE> cards);
        void addAllActionCards(std::vector<CMD_WeaveItem> weaveItems);
        
        BYTE getGuangCardValue();
    protected:
		CardListBoard();
		virtual ~CardListBoard();

		bool onTouchBegan(Touch* touch, Event *event);
		void onTouchMoved(Touch* touch, Event *event);
		void onTouchEnded(Touch* touch, Event *event);

        void addCard(BYTE value);
        void scheduleRun(float delta);
        
        void upCard(MahjongCard* card);
        void downCard(MahjongCard* card);
        
        int touchCheck(const Vec2& pos);
        
        void cardMove2Index(MahjongCard* card ,  int index);
        
        std::vector<int>  getCardValueAllIndex(BYTE cardValue);  //获取牌值的所有索引
        
    private:
        int getCardCount(MahjongCard* card);
        
        
		std::vector<BYTE> _undoList;
		std::vector<MahjongCard*> _cardsPtr;
		bool _isMySelf;
		BYTE _seatNo;
        BYTE _caiShenCard;      //财神牌
        BYTE _restrictCard;     //限制牌
        std::function<void (BYTE cardValue)> _outCardFunc;
        std::function<void (cocos2d::Node*)> _callfunc;
        std::function<void (MJPostion _dir)> _initCardFinishFunc;
        std::function<void (BYTE seatNo , BYTE cardValue)> _gangActionEndFunc;
        
        Vec2 _cardPoint[MAX_COUNT];
        Vec2 _actionPoint[4];
        MahjongCard* _guangActionCard;
        
        
        MJOutCard _outCardValue;
		bool _canOut;
		MJPostion _direction;  //方位
        int _upCardIndex;
        int _outCardIndex;
        int _cardStartIndex;  //第一张牌的起始
        int _needMoveCount;
        bool _isOuting;
        
        Sprite * outCardLine;
	};
}
#endif