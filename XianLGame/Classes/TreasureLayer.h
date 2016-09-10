#ifndef TreasureLayer_h__
#define TreasureLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "PlatformDefine.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;

// btn tag
#define BTN_TBOX1 1001
#define BTN_TBOX2 1002
#define BTN_TBOX3 1003
#define BTN_TBOX4 1004
#define BTN_EXIT 1005

// image tag

// panel tag


class TreasureLayer : public Layer
{

private:
	Widget* _uiTreasureLayer;
	Text* _score;
	ImageView* _headPic;
	ImageView* _roleStar;
	ProgressTimer* _progress;

public:
	static TreasureLayer* createLayer(MoneyChangeNotify* delegate);

public:
	//static cocos2d::Layer* createLayer();
	virtual bool init();
	void refreshGold(int change);
	void onImageClicked(Ref * pSender,Widget::TouchEventType type);
	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);
	void setChangeDelegate(MoneyChangeNotify* delegate);

private:
	MoneyChangeNotify*				_delegate;
};


#endif // TreasureLayer_h__
