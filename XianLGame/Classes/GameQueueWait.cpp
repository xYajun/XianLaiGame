#include "GameQueueWait.h"
#include "PlatformConfig.h"
#include "GamePrompt.h"
#include "HallLayer.h"

Scene* GameQueueWait::createScene()
{
    Scene* scene = Scene::create();
    GameQueueWait* queueWait = GameQueueWait::create();
    scene->addChild(queueWait);
    return scene;
}

GameQueueWait* GameQueueWait::create()
{
    GameQueueWait* queueWait = new GameQueueWait();
    if(queueWait->init())
    {
        queueWait->autorelease();
        return queueWait;
    }
    CC_SAFE_DELETE(queueWait);
    return nullptr;
}

bool GameQueueWait::init()
{
    if(!YZLayer::init())
    {
        return false;
    }
    
    setBackGroundImage("platform/common_bg.png");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto animation = Animation::create();
    for( int i = 1;i<10;i++)
    {
        char szName[100] = {0};
        sprintf(szName, "platform/waitaction/%d.png", i);
        animation->addSpriteFrameWithFile(szName);
        
    }
    auto bgSp = Sprite::create("platform/kauang.png");
    bgSp->setPosition(winSize.width/2, winSize.height/2);
    addChild(bgSp);
    animation->setDelayPerUnit(1.0f / 9.0f);
    animation->setRestoreOriginalFrame(false);
    
    auto cache = AnimationCache::getInstance();
    cache->addAnimation(animation, "waitingAction");
    
    _loadNode = Sprite::create("alpha_0.png");
    _loadNode->setPosition(winSize.width/2, winSize.height/2 + 30);
    this->addChild(_loadNode, 1);
    
    _hintSp = Sprite::create("waiting.png");
    _hintSp->setPosition(winSize.width/2, winSize.height/2);
    this->addChild(_hintSp, 1);
    
    Size size(Size::ZERO);
    _menuItem = MenuItemImage::create("tuichu.png",
                                      "tuichu.png",
                                      "tuichu.png",
                                      CC_CALLBACK_1(GameQueueWait::menuClickedCallback, this));
    size = _menuItem->getContentSize();
    _menuItem->setPosition(winSize.width - size.width / 2 - 10, 10 + size.height / 2);
    
    Menu* menu = Menu::create(_menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(Vec2::ZERO);
    _menuItem->setVisible(false);
    
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(GameQueueWait::updateWord, this));
    Sequence* sequence = Sequence::create(callfunc, DelayTime::create(0.2f), nullptr);
    auto action_play = Animate::create(AnimationCache::getInstance()->getAnimation("waitingAction"));
    _loadNode->runAction(RepeatForever::create(action_play));
    _hintSp->runAction(RepeatForever::create(sequence));
    _hintSp->setVisible(false);
    
    sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
    
    return true;
}

void GameQueueWait::updateWord()
{
    _hintSp->removeAllChildren();
    for(int i = 0; i < _flag; i++)
    {
        Sprite* sp = Sprite::create("dot.png");
        sp->setPosition(180 + i * sp->getContentSize().width, 50);
        _hintSp->addChild(sp);
    }
    _flag = (_flag + 1) % 4;
}

void GameQueueWait::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
{
    CCAssert(INVALID_DESKNO != deskNo, "invalid deskno.");
    if (INVALID_DESKNO == deskNo) return;
    bool find = false;
    for (auto iter = users.begin(); iter != users.end(); ++iter)
    {
        if((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID)
        {
            find = true;
            break;
        }
    }
    if (find)
    {
        stop();
        // 启动游戏
        bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
        if (!ret)
        {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
        }
    }
}

void GameQueueWait::stop()
{	
    _hintSp->setVisible(false);
    _loadNode->stopAllActions();
    _hintSp->stopAllActions();
    _loadNode->setVisible(false);
    
}

GameQueueWait::GameQueueWait()
: _gameIcon(nullptr)
, _hintSp(nullptr)
, _loadNode(nullptr)
, _menuItem(nullptr)
, _joined(false)
, _flag(0)
{
    
}

GameQueueWait::~GameQueueWait()
{
    
}

void GameQueueWait::menuClickedCallback(Ref* pRef)
{
    RoomLogic()->closeRoom();
    stop();
    HallLayer::createScene();
    
}