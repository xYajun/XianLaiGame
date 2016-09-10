#ifndef SaveBoxLayer_h__
#define SaveBoxLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "PlatformDefine.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;


// btn tag
#define SAVEBOX_BTN_BANK		    1001
#define SAVEBOX_BTN_GIVE		    1002
#define SAVEBOX_BTN_PWD		        1003

#define SAVEBOX_BTN_OK        2001
#define SAVEBOX_BTN_CANCLE	  2002
#define SAVEBOX_BTN_EXIT      2003

// panel tag 
#define SAVEBOX_PANEL_BANK		3001
#define SAVEBOX_PANEL_GIVE		3002
#define SAVEBOX_PANEL_PWD	    3003

#define SAVEBOX_BTN_CQTYPE_1	    4001
#define SAVEBOX_BTN_CQTYPE_2	    4002
#define SAVEBOX_BTN_CQTYPE_3	    4003
#define SAVEBOX_BTN_CQTYPE_4	    4004
#define SAVEBOX_BTN_GTYPE_1	        4005
#define SAVEBOX_BTN_GTYPE_2	        4006
#define SAVEBOX_BTN_GTYPE_3	        4007
#define SAVEBOX_BTN_GTYPE_4	        4008
#define SAVEBOX_BTN_ALLTAKE	        4009
#define SAVEBOX_BTN_ALLSTORE	    4010
#define SAVEBOX_BTN_STORE           4011
#define SAVEBOX_BTN_TAKE	        4012
#define SAVEBOX_BTN_GETNICK          4013
#define SAVEBOX_BTN_GIVEMONEY	    4014
#define SAVEBOX_BTN_MODIFY          4015


class SaveBoxLayer : public Layer,public IPlatformNotifyDelegate
{
private:
	Layout* Panel_bank;
	Layout* Panel_give;
	Layout* Panel_modifyPwd;

	Button* btn_bank;
	Button* btn_give;
	Button* btn_modifyPwd;

	Text*  label_bankMoney;
	Text*  label_walltMoney;
	Text*  label_nick;
	TextField* tf_pwd;
	TextField* tf_inputMoney;
	TextField* tf_id;
	TextField* tf_giveMoney;
	TextField* tf_oldPwd;
	TextField* tf_newPwd;
	TextField* tf_confirmPwd;

	LLONG _bankMoney;			// Ç®¹ñ;
	LLONG _walletMoney;		// Ç®°üµÄÇ®;

	std::string _userMD5Pwd;
	bool _isGetNick;

public:
	static SaveBoxLayer* createLayer(MoneyChangeNotify* delegate,bool isOpen);
public:
	//static cocos2d::Layer* createLayer();
	virtual bool init(bool isOpen);
	void refreshGold(int change);
	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void onFcBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void openSaveBox(Node* parent, Vec2 vec, int zorder, int tag);
	void setChangeDelegate(MoneyChangeNotify* delegate);
private:
    void openSaveBoxLayer();
	void initPanelBank();
	void initPanelGive();
	void initPanelPwd();

	bool isDigitStr(const char* cstr);//ÅÐ¶ÏÊÇ·ñÊÇ´¿Êý×Ö

	bool openSaveBoxEventBack( YZSocketMessage* socketMessage );
	bool bankerEventSelector( YZSocketMessage* socketMessage );
	bool modifyPasswordSelector(YZSocketMessage* socketMessage);
	bool showUserNickEventSelector(YZSocketMessage* socketMessage);
	bool bankerForwardSelector( YZSocketMessage* socketMessage);
private:
	MoneyChangeNotify*				_delegate;
};




#endif // SaveBoxLayer_h__
