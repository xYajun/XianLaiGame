#include "GameMatchRoom.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "PlatformDefine.h"
#include "YZUIExport.h"
#include <string>
#include "GameDesk.h"
#include "GamePrompt.h"
#include "PlatformConfig.h"
#include "GameQueueWait.h"

void GameMatchRoom::createMatchRoom()
{
    auto matchScene = Scene::create();
    auto match = GameMatchRoom::create();
    matchScene->addChild(match);
    
    Director::getInstance()->replaceScene(matchScene);
}


GameMatchRoom::GameMatchRoom()
: _currentSelectedRoom(nullptr)
, _pageViewRooms(nullptr)
, _leftArrow(nullptr)
, _rightArrow(nullptr)
{
    /*auto listener = EventListenerTouchOneByOne::create();
     listener->onTouchBegan = CC_CALLBACK_2(GameRoom::onTouchBegan, this);
     listener->onTouchEnded = CC_CALLBACK_2(GameRoom::onTouchEnded, this);
     listener->setSwallowTouches(true);
     _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);*/
}

GameMatchRoom::~GameMatchRoom()
{
    
}

bool GameMatchRoom::init()
{
    if (!YZRoomEventLayer::init())
    {
        return false;
    }
    
    //	Size winSize = Director::getInstance()->getWinSize();
    
    // 	auto bg = LayerColor::create( Color4B(0,0,0,180));
    // 	bg->setPosition(Vec2(0,0));
    // 	addChild(bg);
    //
    // 	auto waiting = Sprite::create("landlord/game/studioUI/uiRes/waiting.png");
    // 	waiting->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
    // 	addChild(waiting);
    setBackGroundImage("platform/common_bg.png");
    
    Size winSize = Director::getInstance()->getWinSize();
    
    auto animation = Animation::create();
    for( int i = 1;i<10;i++)
    {
        char szName[100] = {0};
        sprintf(szName, "platform/waitaction/%d.png", i);
        animation->addSpriteFrameWithFile(szName);
    }
    
    animation->setDelayPerUnit(1.0f / 9.0f);
    animation->setRestoreOriginalFrame(false);
    
    auto bgSp = Sprite::create("platform/kauang.png");
    bgSp->setPosition(winSize.width/2, winSize.height/2);
    addChild(bgSp);
    
    
    auto cache = AnimationCache::getInstance();
    cache->addAnimation(animation, "waitingAction");
    
    _loadNode = Sprite::create("alpha_0.png");
    _loadNode->setPosition(winSize.width/2, winSize.height/2 + 30);
    this->addChild(_loadNode, 1);
    
    _hintSp = Sprite::create("waiting.png");
    _hintSp->setPosition(winSize.width/2, winSize.height/2);;
    this->addChild(_hintSp, 1);
    
    Size size(Size::ZERO);
    _menuItem = MenuItemImage::create("tuichu.png",
                                      "tuichu.png",
                                      "tuichu.png",
                                      CC_CALLBACK_1(GameMatchRoom::menuClickedCallback, this));
    size = _menuItem->getContentSize();
    _menuItem->setPosition(winSize.width - size.width / 2 - 10, 10 + size.height / 2);
    _menuItem->setVisible(false);
    
    Menu* menu = Menu::create(_menuItem, nullptr);
    this->addChild(menu);
    menu->setPosition(Vec2::ZERO);
    
    CallFunc* callfunc = CallFunc::create(CC_CALLBACK_0(GameMatchRoom::updateWord, this));
    Sequence* sequence = Sequence::create(callfunc, DelayTime::create(0.2f), nullptr);
    auto action_play = Animate::create(AnimationCache::getInstance()->getAnimation("waitingAction"));
    _loadNode->runAction(RepeatForever::create(action_play));
    _hintSp->runAction(RepeatForever::create(sequence));
    _hintSp->setVisible(false);
    
    bgSp->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=]()
                                                                              {
                                                                                  RoomLogic()->connectRoom(RoomLogic()->getSelectedRoom()->szServiceIP, RoomLogic()->getSelectedRoom()->uServicePort);
                                                                              }),NULL));
    
    log("444");
    return true;
}

void GameMatchRoom::onExit()
{
    YZRoomEventLayer::onExit();
}

void GameMatchRoom::onDisConnect()
{
    LoadingLayer::removeLoading(this);
    
    YZLOG_ERROR("onDisConnect");
}

void GameMatchRoom::onConnectMessage(bool success)
{
    if (success)
    {
        YZLOG_DEBUG("the room connected to success!");
        ComRoomInfo* room = RoomLogic()->getSelectedRoom();
        RoomLogic()->roomLogin(room->uNameID);
    }
    else
    {
        LoadingLayer::removeLoading(this);
    }
}


void GameMatchRoom::onLoginMessage(bool success, UINT dwErrorCode)
{
    if (success)
    {
        YZLOG_DEBUG("the room login is success");
    }
    else
    {
        YZLOG_ERROR("room login is error [%d]", dwErrorCode);
        std::string errorText("");
        do
        {
            if (ERR_GR_USER_PASS_ERROR == dwErrorCode)
            {
                errorText = "用户密码错误。";
                break;
            }
            
            if (ERR_GR_CONTEST_TIMEOUT == dwErrorCode)
            {
                errorText = "连接超时。";
                break;
            }
            
            if (ERR_GR_IN_OTHER_ROOM == dwErrorCode)
            {
                errorText = "正在其他房间。";
            }
            else if (ERR_GR_ACCOUNTS_IN_USE == dwErrorCode)
            {
                errorText = "帐号正在使用。";
            }
            else if (ERR_GR_STOP_LOGON == dwErrorCode)
            {
                errorText = "暂停登陆服务。";
            }
            else if (ERR_GR_PEOPLE_FULL == dwErrorCode)
            {
                errorText = "房间人数已经满。";
            }
            else if (ERR_GR_CONTEST_TIMEOUT == dwErrorCode)
            {
                errorText = "比赛房无法进入";
            }
            else if (ERR_GR_CONTEST_NOSIGNUP == dwErrorCode)
            {
                errorText = "比赛房无法进入";
            }
            else if (ERR_GR_CONTEST_KICK == dwErrorCode)
            {
                errorText = "抱歉，您已经被淘汰！";
            }
            else if (ERR_GR_CONTEST_NOTSTRAT == dwErrorCode)
            {
                errorText = "比赛未开始。";
            }
            else if (ERR_GR_CONTEST_OVER == dwErrorCode)
            {
                errorText = "比赛已结束。";
            }
            else if (ERR_GR_CONTEST_BEGUN == dwErrorCode)
            {
                errorText = "比赛已开始，由于您没有报名，不能进入房间。";
            }
            RoomLogic()->closeRoom();
        } while (0);
        
        GamePromptLayer::create()->showPrompt(GBKToUtf8(errorText.c_str()));
    }
}

void GameMatchRoom::onLoginFinishMessage()
{
    YZLOG_WARNING("the user enters a room complete message!");
    
    if (RoomLogic()->getRoomRule() & GRR_CONTEST)		//定时淘汰比赛场
    {
        sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
    }
    else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)		//防作弊排队
    {
        sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
    }
}

void GameMatchRoom::onUserComeMessage(UserInfoStruct* user)
{
    
}

// 用户离开房间
void GameMatchRoom::onUserLeftMessage(UserInfoStruct* user)
{
    
}

// 用户断线
void GameMatchRoom::onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE bDeskStation)
{
    
}

// 用户坐下
void GameMatchRoom::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
{
    if (nullptr == userSit) return;
    if (nullptr == user) return;
    
    // 断线重连进来
    if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
    {
        if (INVALID_DESKNO != userSit->bDeskIndex && INVALID_DESKSTATION != userSit->bDeskStation)
        {
            // 启动游戏
            bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, userSit->bDeskIndex, true);
            if (!ret)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
            }
        }
    }
}

// 用户站起
void GameMatchRoom::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
{
}

// 排队用户坐下
void GameMatchRoom::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
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
        LoadingLayer::removeLoading(this);
        // 启动游戏
        bool ret = GameCreator()->startGameClient(RoomLogic()->getSelectedRoom()->uNameID, deskNo, true);
        if (!ret)
        {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("游戏启动失败。"));
        }
    }
}

bool GameMatchRoom::onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
{
    return false;
}


// 跳转到房间列表
//void GameRoom::returnEventCallBack(Ref* pSender, Widget::TouchEventType type)
void GameMatchRoom::returnEventCallBack()
{
    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    removeFromParentAndCleanup(true);
    //RoomLogic()->closeRoom();
    HallLayer::returnHallLayer();
}

void GameMatchRoom::menuClickedCallback(Ref* pRef)
{
    RoomLogic()->closeRoom();
    HallLayer::createScene();
    
}

void GameMatchRoom::updateWord()
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

void GameMatchRoom::stop()
{	
    _hintSp->setVisible(false);
    _loadNode->stopAllActions();
    _hintSp->stopAllActions();
    _loadNode->setVisible(false);
    
}


