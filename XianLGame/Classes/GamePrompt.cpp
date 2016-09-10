#include "GamePrompt.h"
#include <string>
#include "FontConfig.h"

static const char* PROMPT_JSON		= "prompt/PromptBoxUI_1.json";

static const int GAMEPROMPT_LAYER_ZORDER	= 100000000;		// 子节点弹出框层级

static const int GAMEPROMPT_LAYER_TAG		= 100000000;		// 子节点弹出框层级

static Color3B TEXT_COLOR = Color3B(251, 251, 72);

GamePromptLayer::GamePromptLayer()
: _layout(nullptr)
, _Label_Prompt(nullptr)
, _Button_OK(nullptr)
, _sure(nullptr)
{
    
}

GamePromptLayer::~GamePromptLayer()
{
    
}

void GamePromptLayer::showPrompt(const std::string& prompt)
{
    Node* root = Director::getInstance()->getRunningScene();
    CCAssert(nullptr != root, "root is null");
    
    root->addChild(this, GAMEPROMPT_LAYER_ZORDER, GAMEPROMPT_LAYER_TAG);
    setPrompt(prompt);
}

void GamePromptLayer::closePrompt()
{
    //HSAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    _layout->runAction(Sequence::create(ScaleTo::create(0.1f, 0.3f), CCCallFunc::create([&]()
                                                                                        {
                                                                                            this->removeFromParent();
                                                                                        }), nullptr));
    
    if (nullptr != _sure)
    {
        _sure();
    }
}

void GamePromptLayer::setCallBack(std::function<void ()> sure)
{
    _sure = sure;
}

bool GamePromptLayer::init()
{
    if ( !YZLayer::init()) return false;
    
    auto MyListener = EventListenerTouchOneByOne::create();
    MyListener->setSwallowTouches(true);//阻止触摸向下传递
    MyListener->onTouchBegan = [&](Touch* touch, Event* event)
    {
        auto target = dynamic_cast<GamePromptLayer*>(event->getCurrentTarget());
        Point locationInNode = target->convertToNodeSpace(touch->getLocation());
        Size s = target->getContentSize();
        Rect rect = Rect(0, 0, s.width, s.height);
        if (rect.containsPoint(locationInNode))
        {
            if (_userRect.containsPoint(locationInNode)) return true;
            closePrompt();
            return true;
        }
        else
        {
            return false;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);
    
    Size winSize = Director::getInstance()->getWinSize();
    
    _layout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(PROMPT_JSON));
    _layout->setAnchorPoint(Vec2(0.5f, 0.5f));
    _layout->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    
    addChild(_layout, 2, 3);
    
    _Label_Prompt = (Text*)Helper::seekWidgetByName(_layout, "Label_Prompt");
    _Label_Prompt->setString("");
    
    _Button_OK = (Button*)Helper::seekWidgetByName(_layout, "Button_OK");
    _Button_OK->addTouchEventListener(CC_CALLBACK_2(GamePromptLayer::menuEventCallBack, this));
    
    _layout->setScale(0.3f);
    
    float xScale = winSize.width / Director::getInstance()->getOpenGLView()->getDesignResolutionSize().width;
    float yScale = winSize.height / Director::getInstance()->getOpenGLView()->getDesignResolutionSize().height;
    _layout->runAction(Sequence::create(ScaleTo::create(0.1f, MIN(xScale, yScale)), nullptr));
    
    return true;
}

void GamePromptLayer::setPrompt(const std::string& prompt)
{
    if (nullptr != _Label_Prompt)
    {
        _Label_Prompt->setString(prompt);
    }
}

void GamePromptLayer::setPrompt(const std::string& prompt, const std::string& font, int size)
{
    if (nullptr != _Label_Prompt)
    {
        _Label_Prompt->setFontName(font);
        _Label_Prompt->setFontSize(0);
        _Label_Prompt->setString(prompt);
    }
}

void GamePromptLayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    
    closePrompt();
}
