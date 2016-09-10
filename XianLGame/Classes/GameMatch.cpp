#include "GameMatch.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "StoreLayer.h"
#include "PlatformDefine.h"
#include "GamePrompt.h"
#include "PlatformConfig.h"
#include "LoginLayer.h"
#include "GameQueueWait.h"


void GameMatch::createMatch(ComRoomInfo* roomInfo)
{
    auto matchScene = Scene::create();
    auto match = GameMatch::create(roomInfo);
    matchScene->addChild(match);
    
    Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, matchScene));
}

GameMatch::GameMatch(ComRoomInfo* roomInfo)
{
    _roomInfo = roomInfo;
}

GameMatch* GameMatch::create(ComRoomInfo* roomInfo)
{
    GameMatch *pRet = new GameMatch(roomInfo);
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

GameMatch::~GameMatch()
{
    
}

bool GameMatch::init()
{
    if (!YZRoomEventLayer::init()) return false;
    
    _isApply = false;
    
    auto winSize = Director::getInstance()->getWinSize();
    
    enableKeyboard();
    
    MSG_GR_R_LogonResult& LogonResult = RoomLogic()->loginResult;
    
    _tableWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("platform/match/matchLayer/matchLayer_1.json");
    addChild(_tableWidget);
    
    _topWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("platform/match/matchLayer/matchLayer_2.json");
    _topWidget->setPosition(Vec2(0, winSize.height - 90));
    addChild(_topWidget);
    _button_bm = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_tableWidget, "Button_bm"));
    _button_bm->addTouchEventListener(CC_CALLBACK_2(GameMatch::menuClickedCallback, this));
    _button_bm->setName("apply");
    
    auto button_exit = dynamic_cast<ui::Button*>(ui::Helper::seekWidgetByName(_topWidget, "Button_1"));
    button_exit->addTouchEventListener(CC_CALLBACK_2(GameMatch::returnEventCallBack, this));
    
    int nHour_start=0;
    int nMinute_start=0;
    int nHour_end=0;
    int nMinute_end=0;
    
    struct tm *tm;
    time_t timep = _roomInfo->i64TimeStart;
    tm = localtime(&timep);
    nHour_start = tm->tm_hour;
    nMinute_start = tm->tm_min;
    
    struct tm *tm_end;
    time_t timep_end = _roomInfo->i64TimeEnd;
    tm_end = localtime(&timep_end);
    nHour_end = tm_end->tm_hour;
    nMinute_end = tm_end->tm_min;
    
    char buffer[128];
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%02d:%02d-%d:%02d",nHour_start,nMinute_start, nHour_end, nMinute_end);
    //时间1
    Text* lbl_time = static_cast<Text*>(Helper::seekWidgetByName(_tableWidget, "Label_bmsj"));
    lbl_time->setString(buffer);
    
    schedule(schedule_selector(GameMatch::callEverySecond),1.0f);
    
    return true;
}

void GameMatch::callEverySecond(float delta)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    struct timeval now;
    gettimeofday(&now, NULL);
    localtime(&now.tv_sec);
    if (now.tv_sec > _roomInfo->i64TimeStart)
    {
        // 比赛开始,进入匹配
        //发送获取当前所有玩家当前积分信息
        USERCURSCOREREQ tUserScoreReq;
        tUserScoreReq.nRoomID = _roomInfo->iContestID;
        sendData(MDM_GR_MATCH_INFO,ASS_GR_CUR_SCORE,&tUserScoreReq,sizeof(USERCURSCOREREQ));
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, GameQueueWait::createScene()));
        unschedule(schedule_selector(GameMatch::callEverySecond));
    }
#endif
#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
    struct tm *tm;
    time_t timep;
    time(&timep);
    localtime(&timep);
    if (timep > _roomInfo->i64TimeStart)
    {
        // 比赛开始,进入匹配
        //发送获取当前所有玩家当前积分信息
        USERCURSCOREREQ tUserScoreReq;
        tUserScoreReq.nRoomID = _roomInfo->iContestID;
        sendData(MDM_GR_MATCH_INFO,ASS_GR_CUR_SCORE,&tUserScoreReq,sizeof(USERCURSCOREREQ));
        Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, GameQueueWait::createScene()));
        unschedule(schedule_selector(GameMatch::callEverySecond));
    }
#endif
}


void GameMatch::menuClickedCallback(Ref* pRef, Widget::TouchEventType touchtype)
{
    if (Widget::TouchEventType::ENDED != touchtype)	return;
    
    
    Node* pNode = (Node*)pRef;
    std::string name = pNode->getName();
    if(name.compare("apply") == 0)
    {
        NET_ROOM_CONTEST_APPLY contest_apply;
        contest_apply.dwUserID = PlatformLogic()->loginResult.dwUserID;
        //contest_apply.dwContestID = _roomInfo->uRoomID;
        contest_apply.dwContestID = _roomInfo->iContestID;
        sendData(MDM_GR_USER_ACTION, ASS_GR_CONTEST_APPLY, &contest_apply, sizeof(NET_ROOM_CONTEST_APPLY));
        RoomLogic()->addEventSelector(MDM_GR_USER_ACTION, ASS_GR_CONTEST_APPLY, YZ_SOCKET_CALLBACK(GameMatch::onApplyRet, this));
    }
    else if(name.compare("exit") == 0)
    {
        NET_ROOM_CONTEST_CANCEL contest_exit;
        contest_exit.dwUserID = PlatformLogic()->loginResult.dwUserID;
        contest_exit.dwRoomId = _roomInfo->iContestID;
        contest_exit.nEntryTax = 0;
        sendData(MDM_GR_USER_ACTION, ASS_GR_CONTEST_CANCEL, &contest_exit, sizeof(NET_ROOM_CONTEST_CANCEL));
        RoomLogic()->addEventSelector(MDM_GR_USER_ACTION, ASS_GR_CONTEST_CANCEL, YZ_SOCKET_CALLBACK(GameMatch::onExitRet, this));
    }
}

void GameMatch::returnEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    leaveDesk();
}

bool GameMatch::onApplyRet(YZSocketMessage* socketMessage)
{
    NET_ROOM_CONTEST_APPLY_RESULT* apply_ret = (NET_ROOM_CONTEST_APPLY_RESULT*)socketMessage->object;
    if (apply_ret->nResult == 0)
    {
        _button_bm->setName("exit");
        auto  image_btnbm = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "Image_btnbm"));
        image_btnbm->loadTexture("platform/match/matchLayer/tuichubisai.png");
        // 刷新按钮
        
    }
    else
    {
        log("报名失败");
        
    }
    
    return true;
}

bool GameMatch::onExitRet(YZSocketMessage* socketMessage)
{
    NET_ROOM_CONTEST_CANCEL_RESULT* exit_ret = (NET_ROOM_CONTEST_CANCEL_RESULT*)socketMessage->object;
    if (exit_ret->nResult == 0)
    {
        _button_bm->setName("apply");
        auto  image_btnbm = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableWidget, "Image_btnbm"));
        image_btnbm->loadTexture("platform/match/matchLayer/bs_wybmz.png");
        // 刷新按钮
        
    }
    else
    {
        log("报名失败");
        
    }
    
    return true;
}

// 网络检测回调定时器函数
void GameMatch::gameNetworkCheckCallBack(float dt)
{
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameMatch::gameNetworkCheckCallBack), this);
    if (!PlatformLogic()->connected())
    {
        auto prompt = GamePromptLayer::create();
        prompt->showPrompt(GBKToUtf8("网络断开，请重新登陆。"));
        prompt->setCallBack([]()
                            {
                                LoginLayer::returnLogin();
                            });
    }
}



void GameMatch::onExit()
{
    YZRoomEventLayer::onExit();
}


//手机返回键监听回调函数
void GameMatch::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
#endif
        {
            leaveDesk();
        }
}

void GameMatch::leaveDesk()
{
    YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    RoomLogic()->closeRoom();
    HallLayer::returnHallLayer();
}

// void GameMatch::updateRoomPeople(INT count)
// {
// 	count = count < 0 ? 0 : count;
// 	char str[128];
// 	sprintf(str, GBKToUtf8("人数:%d"), count);
// 	auto labelPeople = (Label*)this->getChildByTag(ROOM_PEOPLE_TAG);
// 	labelPeople->setString(str);
// }



//////////////////////////////////////////////////////////////////////////

void GameMatch::walletChanged(LLONG money)
{
    // 	if (nullptr != _textSpriteUserChips)
    // 	{
    // 		char buffer[32];
    // 		sprintf(buffer, "%lld", money);
    // 		_textSpriteUserChips->setText(gFontConfig_18, buffer, Color4B::YELLOW);
    // 	}
    
}

void GameMatch::onDisConnect()
{
    YZLOG_ERROR("network disconnect!");
}
