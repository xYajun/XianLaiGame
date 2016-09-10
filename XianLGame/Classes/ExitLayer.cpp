#include "ExitLayer.h"
#include "LoginLayer.h"

static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// 子节点弹出框层级

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// 子节点弹出框层级

ExitLayer::ExitLayer()
{
    
}

ExitLayer::~ExitLayer()
{
    
}

bool ExitLayer::init()
{
    if ( !YZLayer::init() )
    {
        return false;
    }
    Size winSize = Director::getInstance()->getWinSize();
    auto layout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("exitLayer/PromptLayer.json"));
    layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    layout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    addChild(layout);
    
    
    auto Button_OK = (Button*)Helper::seekWidgetByName(layout, "btn_ok");
    Button_OK->setTag(10001);
    Button_OK->addTouchEventListener(CC_CALLBACK_2(ExitLayer::menuEventCallBack, this));
    
    auto Button_Cancel = (Button*)Helper::seekWidgetByName(layout, "btn_cancle");
    Button_Cancel->setTag(10002);
    Button_Cancel->addTouchEventListener(CC_CALLBACK_2(ExitLayer::menuEventCallBack, this));
    
    return true;
}

void ExitLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    auto btn=(Button*)pSender;
    if (Widget::TouchEventType::ENDED != type) return;
    switch (btn->getTag())
    {
        case 10001:
        {
            RoomLogic()->closeRoom();
            PlatformLogic()->closePlatform();
            auto scene =LoginLayer::createScene();
            Director::getInstance()->replaceScene(scene);
        }
            break;
        case 10002:
            removeFromParentAndCleanup(true);
            break;
        default:
            break;
    }
    
}

void ExitLayer::showPrompt()
{
    Node* root = Director::getInstance()->getRunningScene();
    CCAssert(nullptr != root, "root is null");
    
    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
}