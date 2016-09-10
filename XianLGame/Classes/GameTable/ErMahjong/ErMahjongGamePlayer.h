#ifndef __ErMahjong_GAME_PLAYER_H__
#define __ErMahjong_GAME_PLAYER_H__

#include "cocos2d.h"
#include "YZNetExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>

using namespace ui;
using namespace std;

namespace ErMahjong
{
	class PokerCard;

	class PlayerUI: public YZLayer
	{
	public:
		static PlayerUI* create(LLONG userId , INT score);
		virtual bool init(LLONG userId , INT score);
	
	public:
		void setUserId(LLONG userId);
		void setUserName(const std::string& name);
		void changeUserScore(INT score);
		void setUserCardCount(int count);
		void setAutoHead(bool isAuto);
		void setHead(UserInfoStruct* userInfo);
		void setBanker(bool ibanker);
		void showMoney(bool visible);
		void showCard(bool visible);
		void showTingCard(bool visible);
		void setTableUI(cocos2d::Node* tableUI);
        bool getIsBoy();
        
        std::string getUserName();
        
	protected:
		PlayerUI();
		virtual ~PlayerUI();
	
	protected:
		bool onTouchBegan(Touch *touch, Event *unused_event);
		void onTouchEnded(Touch *touch, Event *unused_event);	
		void loadComponet(ui::Widget* widget);

	private:
		LLONG		_userId;
		INT		_score;
		std::string _name;

	private:
		ui::ImageView*	_ivHead;
		ui::Text*		_textName;
		ui::Text*		_textMoney;
		ui::ImageView*  _imgBanker;
		ui::ImageView*  _imgTing;
		cocos2d::Node*	_tableUI;
        ui::Layout *    _panelName;
        
        bool _isBoy;
        UserInfoStruct* _userInfo;
	};
}



#endif // !_GAME_PLAYER_
