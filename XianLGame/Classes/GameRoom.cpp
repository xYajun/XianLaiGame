#include "GameRoom.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "PlatformDefine.h"
#include "YZUIExport.h"
#include <string>
#include "GameDesk.h"
#include "GamePrompt.h"
#include "PlatformConfig.h"
#include "GameQueueWait.h"
#include "LoginLayer.h"

//////////////////////////////////////////////////////////////////////////
static const float enter_game_desk_outtime_timer = 30.0f;
static const float update_game_room_people_timer = 2.0f;
//////////////////////////////////////////////////////////////////////////	//
static const int	MENU_LEFT_ARROW_TAG		= 101;			//左箭头
static const int	MENU_RIGHT_ARROW_TAG	= 102;			//右箭头

static const int	ROOMLAYER_TAG	= 201;			//右箭头
//////////////////////////////////////////////////////////////////////////
static const char* connect_room_text		= "连接房间......";
static const char* login_room_text			= "登陆房间......";
static const char* request_room_info_text	= "获取房间数据......";
//////////////////////////////////////////////////////////////////////////
static const char* ROOM_ITEM_UI = "platform/room/RoomItemUi_1.json";
//////////////////////////////////////////////////////////////////////////
static const Size pageViewRoomsSize(700, 350);


//////////////////////////////////////////////////////////////////////////


GameRoom::GameRoom()
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

GameRoom::~GameRoom()
{
    
}

void GameRoom::setGameInfo(const long& lGameTag)
{
    if (g_GlobalUnits.isOpenRoom) {
        CCLOG("设置Room GameInfo tag：%ld" , lGameTag);
    }else{
        auto tittleBg = (ImageView*)(Helper::seekWidgetByName(_uiRoomLayer,"tittle_word"));
        switch (lGameTag)
        {
            case GAME_PDK:
                tittleBg->loadTexture("platform/roomInfo_res/game_title_01.png");
                break;
            default:
                break;
        }
    }
}

bool GameRoom::init()
{
    if (!YZRoomEventLayer::init())
    {
        return false;
    }
    
    CCLOG("开房模式");
    YZAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSICYL, true);

    int roomAllCount = RoomInfoModule()->getRoomCount();
   
    if (roomAllCount <= 0) {
        CHAR buf[512];
        sprintf(buf, "没有找到游戏房间, 请稍后再试...");
        YZLOG_ERROR("%s", GBKToUtf8(buf));
        GamePromptLayer::create()->showPrompt(GBKToUtf8(buf));
        
        this->runAction(Sequence::create(DelayTime::create(10.0f),CallFunc::create([=]{
            RoomLogic()->closeRoom();
            PlatformLogic()->closePlatform();
            auto scene =LoginLayer::createScene();
            Director::getInstance()->replaceScene(scene);
        }), NULL));
    }
    
    CCLOG("roomAllCount : %d" , roomAllCount);
    
    roomInfo = RoomInfoModule()->getByRoomID(3);
    
    if (roomInfo != NULL) {
        
        RoomLogic()->setSelectedRoom(roomInfo);
        RoomLogic()->setRoomRule(roomInfo->dwRoomRule);
        
        if (RoomLogic()->getRoomRule() & GRR_CONTEST)	// 定时淘汰比赛场
        {
            CCLOG("手机比赛游戏正在研发中。");
        }
        else												// 金币场不扣积分
        {
            // 连接房间服务器
            UserInfoModule()->clear();
            RoomLogic()->connectRoom(roomInfo->szServiceIP, roomInfo->uServicePort);
        }
    }
    
    return true;
}

void GameRoom::onExit()
{
    YZRoomEventLayer::onExit();
}


void GameRoom::createRoomList()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    _pageViewRooms = PageView::create();
    _pageViewRooms->setContentSize(pageViewRoomsSize);
    float xScale = winSize.width / PlatformConfig::getInstance()->getPlatformDesignSize().width;
    float yScale = winSize.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;
    //_pageViewRooms->setScale(xScale, yScale);
    _pageViewRooms->setAnchorPoint(Vec2(0.5f, 0.5f));
    _pageViewRooms->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f ));
    _pageViewRooms->setTouchEnabled(true);
    _pageViewRooms->setCustomScrollThreshold(10);
    addChild(_pageViewRooms, 1);
    
    
    int roomAllCount = RoomInfoModule()->getRoomCount();
    int currentIndex = 0;
    
    int pageCount = (roomAllCount / 3);
    if (pageCount > 0)
    {
        for (int currentPage  = 0; currentPage < pageCount; currentPage++)
        {
            ComRoomInfo* roomInfo0 = RoomInfoModule()->getRoom(currentIndex);
            ComRoomInfo* roomInfo1 = RoomInfoModule()->getRoom(currentIndex + 1);
            ComRoomInfo* roomInfo2 = RoomInfoModule()->getRoom(currentIndex + 2);
            createRoom(roomInfo0, roomInfo1, roomInfo2);
            currentIndex = currentIndex + 3;
        }
    }
    
    int remainder = (roomAllCount % 3);
    if (remainder > 0)
    {
        ComRoomInfo* roomInfo[4] = {0};
        int i = 0;
        while (i < remainder)
        {
            roomInfo[i] = RoomInfoModule()->getRoom(currentIndex + i);
            i++;
        }
        createRoom(roomInfo[0], roomInfo[1], roomInfo[2]);
    }
    //updateArrow(_pageViewRooms);
}

//添加房间
void GameRoom::createRoom(ComRoomInfo* roomInfo1, ComRoomInfo* roomInfo2, ComRoomInfo* roomInfo3)
{
    CCAssert(nullptr != roomInfo1, "room info is nullptr");
    if (nullptr == roomInfo1) return;
    //创建房间列表子页面
    auto roomItemLayout = Layout::create();
    roomItemLayout->setName("page");
    roomItemLayout->setContentSize(pageViewRoomsSize);
    
    if (nullptr != roomInfo1)
    {
        auto roomItem = createRoomItem(roomInfo1);
        if (nullptr != roomItem)
        {
            //roomItem->setScale(0.8f);
            roomItem->setPosition(Vec2(roomItemLayout->getContentSize().width * 0.5, roomItemLayout->getContentSize().height * 0.8f));
            roomItemLayout->addChild(roomItem, 3);
        }
    }
    
    if (nullptr != roomInfo2)
    {
        auto roomItem = createRoomItem(roomInfo2);
        if (nullptr != roomItem)
        {
            //roomItem->setScale(0.8f);
            roomItem->setPosition(Vec2(roomItemLayout->getContentSize().width * 0.5, roomItemLayout->getContentSize().height * 0.48f));
            roomItemLayout->addChild(roomItem, 3);
        }
    }
    
    if (nullptr != roomInfo3)
    {
        auto roomItem = createRoomItem(roomInfo3);
        if (nullptr != roomItem)
        {
            //roomItem->setScale(0.8f);
            roomItem->setPosition(Vec2(roomItemLayout->getContentSize().width * 0.5, roomItemLayout->getContentSize().height * 0.16));
            roomItemLayout->addChild(roomItem, 3);
        }
    }
    
    
    // 添加子页面进入列表中
    _pageViewRooms->addPage(roomItemLayout);
    
    // 拖动监听
    _pageViewRooms->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(GameRoom::pageViewMoveCallBack, this)));
}

Layout* GameRoom::createRoomItem(ComRoomInfo* roomInfo)
{
    auto roomItemLayout = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile(ROOM_ITEM_UI));
    if (nullptr == roomItemLayout) return nullptr;
    
    roomItemLayout->setAnchorPoint(Vec2(0.5f, 0.5f));
    
    auto roomItem = dynamic_cast<Button*>(Helper::seekWidgetByName(roomItemLayout, "Button_RoomItem"));
    if (nullptr == roomItem) return nullptr;
    
    roomItem->setUserData(roomInfo);
    roomItem->addTouchEventListener(CC_CALLBACK_2(GameRoom::enterRoomEventCallBack, this));
    
    char str[128];
    
    auto Label_TittleLimit = dynamic_cast<Text*>(Helper::seekWidgetByName(roomItem, "label_TittleLimit"));
    Label_TittleLimit->setString(GBKToUtf8(roomInfo->szGameRoomName));
    // 金币限制
    auto Label_MoneyLimit = dynamic_cast<Text*>(Helper::seekWidgetByName(roomItem, "Label_MoneyLimit"));
    if (nullptr != Label_MoneyLimit)
    {
        sprintf(str, "%d", roomInfo->iLessPoint);
        Label_MoneyLimit->setString(GBKToUtf8(str));
    }
    
    
    return roomItemLayout;
}

void GameRoom::pageViewMoveCallBack(Ref* pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            //	updateArrow(pageView);
        }
            break;
        default:
            break;
    }
}

//子页面按钮回调
void GameRoom::enterRoomEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    switch (type)
    {
        case Widget::TouchEventType::ENDED:
        {
            Size winSize = Director::getInstance()->getWinSize();
            YZAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
            
            _currentSelectedRoom = dynamic_cast<Button*>(pSender);
            
            ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(_currentSelectedRoom->getUserData());
            MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
            if (LogonResult.i64Money<roomInfo->iLessPoint)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("金币不足，无法进入房间！"));
                return;
            }
            
            // 参数校验
            CCAssert(nullptr != roomInfo, "room is nullptr!");
            if (nullptr == roomInfo) return;
            
            do
            {
                // 进入房间最低限额
                // 				if (PlatformLogic()->loginResult.i64Money < roomInfo->iLessPoint)
                // 				{
                // 					char buffer[128];
                // 					sprintf(buffer, "您的金币小于%u, 不能进入房间, 请到商店充值。", roomInfo->iLessPoint);
                // 					GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                // 					break;
                // 				}
                //
                // 				// 进入房间最大限额
                // 				if (roomInfo->iMaxPoint != 0 && (PlatformLogic()->loginResult.i64Money > roomInfo->iMaxPoint))
                // 				{
                // 					char buffer[128];
                // 					sprintf(buffer, "您的金币大于%u, 不能进入房间。", roomInfo->iMaxPoint);
                // 					GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                // 					break;
                // 				}
                
                RoomLogic()->setSelectedRoom(roomInfo);
                RoomLogic()->setRoomRule(roomInfo->dwRoomRule);
                
                if (RoomLogic()->getRoomRule() & GRR_CONTEST)	// 定时淘汰比赛场
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("手机比赛游戏正在研发中。"));
                }
                else												// 金币场不扣积分
                {
                    // 连接房间服务器
                    UserInfoModule()->clear();
                    RoomLogic()->connectRoom(roomInfo->szServiceIP, roomInfo->uServicePort);
                    
                    Director::getInstance()->getScheduler()->schedule(schedule_selector(GameRoom::enterRoomOutTimerTimerCallBack), this, enter_game_desk_outtime_timer, 0, 0.0f, false);
                    
                    // 转圈动画
                    //LoadingLayer::createLoading(this, gFontConfig_22, GBKToUtf8(connect_room_text), LOADING);
                }
                
            } while (0);
        }
            break;
        default:
            break;
    }
}

void GameRoom::arrowCallBack(Ref* pSender, Widget::TouchEventType type, INT buttonTag)
{
    if (Widget::TouchEventType::ENDED != type) return;
    
    //CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    
    INT page = _pageViewRooms->getCurPageIndex();
    
    if (MENU_LEFT_ARROW_TAG == buttonTag)
    {
        _pageViewRooms->scrollToPage(--page);
    }
    
    if (MENU_RIGHT_ARROW_TAG == buttonTag)
    {
        _pageViewRooms->scrollToPage(++page);
    }
}

void GameRoom::updateArrow(PageView* pageView)
{
    INT pageIndex = pageView->getCurPageIndex();
    
    ssize_t pageCount =  pageView->getPages().size();
    // 更新左右指示箭头
    if (pageIndex == 0 && pageCount == 1)
    {
        _leftArrow->setVisible(false);
        _rightArrow->setVisible(false);
        return;
    }
    else if (pageIndex == 0 && pageCount > 1)
    {
        _leftArrow->setVisible(false);
        _rightArrow->setVisible(true);
        return;
    }
    else if (pageIndex != 0 && pageIndex == pageCount - 1)
    {
        _leftArrow->setVisible(true);
        _rightArrow->setVisible(false);
        return;
    }
    else if (pageIndex == pageCount - 1)
    {
        _leftArrow->setVisible(true);
        _rightArrow->setVisible(false);
        return;
    }
    else
    {
        _leftArrow->setVisible(true);
        _rightArrow->setVisible(true);
    }
}

//////////////////////////////////////////////////////////////////////////
void GameRoom::onDisConnect()
{
    LoadingLayer::removeLoading(this);
    
    YZLOG_ERROR("onDisConnect");
}

void GameRoom::onConnectMessage(bool success)
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

void GameRoom::onLoginMessage(bool success, UINT dwErrorCode)
{
    if (success)
    {
        YZLOG_DEBUG("the room login is success");
        //LoadingLayer::createLoading(this, gFontConfig_22, GBKToUtf8(request_room_info_text), LOADING);
    }
    else
    {
        LoadingLayer::removeLoading(this);
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
            RoomLogic()->closeRoom();
        } while (0);
        
        GamePromptLayer::create()->showPrompt(GBKToUtf8(errorText.c_str()));
    }
}

void GameRoom::onLoginFinishMessage()
{
    YZLOG_WARNING("the user enters a room complete message!");
    
    if (GameCreator()->getCurrentGameType() == YZGameCreator::NORMAL)
    {
        GameDesk::createDesk(roomInfo, true);
    } else
    {
        GameDesk::createDesk(roomInfo, false);
    }
    
    /*
    
    ComRoomInfo* roomInfo = static_cast<ComRoomInfo*>(_currentSelectedRoom->getUserData());
    RoomLogic()->setSelectedRoom(roomInfo);
    RoomLogic()->setRoomRule(roomInfo->dwRoomRule);
    
    if (RoomLogic()->getRoomRule() & GRR_CONTEST)		//定时淘汰比赛场
    {
        //GameMatch::createMatch(roomInfo);
    }
    else if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)	// 排队机
    {
        //LoadingLayer::createLoading(this, gFontConfig_22, GBKToUtf8(allocation_table_please_wait_text), LOADING);
        // 进入排队游戏
        //sendData(MDM_GR_USER_ACTION, ASS_GR_JOIN_QUEUE);
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, GameQueueWait::createScene()));
        //LoadingLayer::removeLoading(this);
        //GameQueueWait* queueWait = GameQueueWait::create();
        //queueWait->setPosition(0, 0);
        //this->addChild(queueWait, 1000);
    }
    else												// 金币场不扣积分
    {
        LoadingLayer::removeLoading(this);
        if (GameCreator()->getCurrentGameType() == YZGameCreator::NORMAL)
        {
            GameDesk::createDesk(roomInfo, true);
        }
        else
        {
            GameDesk::createDesk(roomInfo, false);
        }
    }*/
}

void GameRoom::onUserComeMessage(UserInfoStruct* user)
{
    updateRoomPeopleTimerCallBack(0.0f);
}

// 用户离开房间
void GameRoom::onUserLeftMessage(UserInfoStruct* user)
{
    updateRoomPeopleTimerCallBack(0.0f);
}

// 用户断线
void GameRoom::onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE bDeskStation)
{
    
}

// 用户坐下
void GameRoom::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
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
void GameRoom::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
{
    updateRoomPeopleTimerCallBack(0.0f);
}

// 排队用户坐下
void GameRoom::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
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

bool GameRoom::onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
{
    return false;
}

void GameRoom::enterRoomOutTimerTimerCallBack(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameRoom::enterRoomOutTimerTimerCallBack), this);
    if (!RoomLogic()->connected() || !RoomLogic()->login())
    {
        RoomLogic()->closeRoom();
        LoadingLayer::removeLoading(this);
    }
}

void GameRoom::updateRoomPeopleTimerCallBack(float dt)
{
    /*
    Vector<Layout*> pages = _pageViewRooms->getPages();
    char buffer[64];
    bool _exit = false;
    RoomInfoModule()->transform([&](ComRoomInfo* room, INT index)
                                {
                                    _exit = false;
                                    for (auto iter = pages.begin(); iter != pages.end(); ++iter)
                                    {
                                        Layout* romItemLayout = *iter;
                                        
                                        Vector<Node*> childs = romItemLayout->getChildren();
                                        for (auto iter1 = childs.begin(); iter1 != childs.end(); ++iter1)
                                        {
                                            auto layout = dynamic_cast<Layout*>(*iter1);
                                            auto roomButton = dynamic_cast<Button*>(Helper::seekWidgetByName(layout, "Button_RoomItem"));
                                            auto Label_PeopleCount = dynamic_cast<Text*>(Helper::seekWidgetByName(layout, "Label_PeopleCount"));
                                            
                                            if (nullptr != roomButton)
                                            {
                                                ComRoomInfo* pRoom = static_cast<ComRoomInfo*>(roomButton->getUserData());
                                                if (pRoom->uRoomID == room->uRoomID)
                                                {
                                                    pRoom->uPeopleCount = room->uPeopleCount;
                                                    pRoom->uVirtualUser = room->uVirtualUser;
                                                    if (nullptr != Label_PeopleCount)
                                                    {
                                                        sprintf(buffer, "%d人", pRoom->uPeopleCount);
                                                        Label_PeopleCount->setString(GBKToUtf8(buffer));
                                                    }
                                                    _exit = true;
                                                    break;
                                                }
                                            }
                                        }
                                        if (_exit) break;
                                    }
                                });
     */
}

bool GameRoom::onTouchBegan(Touch *touch, Event *unused_event)
{
    return	true;
}

void GameRoom::onTouchEnded(Touch *touch, Event *unused_event)
{
}

// 跳转到房间列表
//void GameRoom::returnEventCallBack(Ref* pSender, Widget::TouchEventType type)
void GameRoom::returnEventCallBack()
{
    //YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    removeFromParentAndCleanup(true);
    //RoomLogic()->closeRoom();
    HallLayer::returnHallLayer();
}

