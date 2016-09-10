#ifndef HelpLayer_h__
#define HelpLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"


USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;

#define BTN_HELPCLOSE 10001

class TreatyLayer : public Layer
{

public:
	static cocos2d::Layer* createLayer();
	virtual bool init();

	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);

	CREATE_FUNC(TreatyLayer);

};


#endif // HelpLayer_h__
