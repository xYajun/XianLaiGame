#ifndef PlayerInfoLayer_h__
#define PlayerInfoLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "network/HttpClient.h"
#include "YZNetExport.h"

USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;

struct PlayerInfo
{
    int							dwUserID;
    bool						bBoy;
    std::string                 nickName;
    std::string                 headUrl;
    UINT						dwUserIP;
    bool						isHall;
};

class PlayerInfoLayer : public Layer
{
public:
	static cocos2d::Layer* createLayer(PlayerInfo pInfo);
	virtual bool init();

	void onBtnClicked(Ref * pSender,Widget::TouchEventType type);

	CREATE_FUNC(PlayerInfoLayer);

private:
    PlayerInfo uInfo;
};


#endif // StoreLayer_h__
