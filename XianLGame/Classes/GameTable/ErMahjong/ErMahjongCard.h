#ifndef __ErMahjong_Mahjong_CARD_H__
#define __ErMahjong_Mahjong_CARD_H__

#include "cocos2d.h"
#include "YZNetExport.h"
#include "ErMahjongGameLogic.h"
#include <string>
using namespace std;
USING_NS_CC;


namespace ErMahjong
{
	class MahjongCard : public Sprite
	{
	public:
		static MahjongCard* create(BYTE cardValue, MJPostion pos);
		void setCardValue(BYTE cardValue);
        void setCardScaleValue(float value);
        void setCardAnGuangTexture();
		BYTE getCardValue();

        BYTE getMJCardType();
        BYTE getMJCardValue();
        
        void setCaiShenCardValue(BYTE cardValue);
        void showCaiShenSprite(bool isVisable);
		void addTouchedLayer();
		void removeTouchedLayer();
		virtual bool initWithCardValue(BYTE cardValue,MJPostion pos );

        //检查一张牌是否在另一张牌的后面
        bool afterMJCard(MahjongCard * card);
        
        void showCardWinTexture();
        
	protected:
		MahjongCard();
		~MahjongCard();
 
        
        bool operator () (MahjongCard * card)
        {
            return this->getCardValue() == card->getCardValue();
        }

	private:
        Sprite * caiShen;
        float _ScaleValue;
		BYTE _Value;
        BYTE _caiShenCardValue;
        MJPostion _dir;
		GameLogic _gameLogic;
		string getCardTextureFileName(BYTE cardValue,MJPostion pos ,bool iWin);
    };
}
#endif // !_Mahjong_CARD_
