#ifndef ShareLayer_h__
#define ShareLayer_h__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


#include "network/HttpClient.h"
#include "YZNetExport.h"

#define BTN_SHARECLOSE_TAG 1001
USING_NS_CC;

using namespace ui;
using namespace cocostudio;
using namespace network;



class ShareLayer : public Layer
{
    
    
    
public:
    static cocos2d::Layer* createLayer();
    virtual bool init();
    
    void shareShot();
    
    static ShareLayer * _GameShareLayer;
    static ShareLayer* getInstance();
    
    void screenShotCallback(bool b, const std::string& name);
    
    CREATE_FUNC(ShareLayer);
};


#endif // ShareLayer_h__
