#include "HallLayer.h"
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalUnits.h"
#include "PersonalLayer.h"
#include "StoreLayer.h"
#include "SaveBoxLayer.h"
#include "FontConfig.h"
#include "LoginLayer.h"
#include "GamePrompt.h"
#include "GameRoom.h"
#include "ShareLayer.h"

#include "HelpLayer.h"
#include "CommonLayer.h"
#include "ExitLayer.h"
#include "SetLayer.h"
#include "SocketErrorLayer.h"

#include "GameTable/ErMahjong/ErMahjongGameTableUI.h"
#include "GameTable/ErMahjong/ErMahjongMessageHead.h"

using namespace cocostudio;
using namespace cocos2d::ui;
USING_NS_CC;



static const int MENU_CANCEL_TAG = 100;

//////////////////////////////////////////////////////////////////////////
static const int ROOM_ZORDER						= 0;		// 房间层级
static const int CHILD_ZORDER						= 400;		// 子节点弹出框层级

//////////////////////////////////////////////////////////////////////////
static const int CHILD_ROOM_LAYER_TAG				= 500;		// 房间层标签
static const int CHILD_EXIT_LAYER_TAG				= 501;		// 退出层标签
static const int CHILD_BANK_LAYER_TAG				= 502;
static const int CHILD_STORE_LAYER_TAG				= 503;
static const int CHILD_SERVICE_LAYER_TAG			= 504;
static const int CHILD_USERHEAD_LAYER_TAG			= 505;
static const int CHILD_EXCHANGE_LAYER_TAG			= 506;
static const int CHILD_EXCHANGE_RECORD_LAYER_TAG	= 506;
static const int LIGHTTAG	                        = 10001;

static const Size pageViewGamesSize(1000, 400);
static const int maxGameNum = 1;


////////////////////////////////////////////////////////
static const char* IMG_GAME_DDZ				= "HallLayer/game_ddz.png";
#define Name_Local_Head_Pic			"PersonalHead/local_head_"
#define Name_Defaul_Pic				"HallLayer/res/head_default.png"


void HallLayer::createScene()
{
    auto scene = Scene::create();
    auto mainlayer = HallLayer::create();
    scene->addChild(mainlayer);
    mainlayer->setTag(110011);
    Director::getInstance()->replaceScene(scene);
}

bool HallLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    _nRoomType = 0;
    
    waitTime = 15.0f;
    
    _isOpen=false;
    
    auto winsize=Director::getInstance()->getWinSize();
    
    roomLabel = Label::createWithTTF("正在加载玩家信息...", "fonts/DroidSansFallback.ttf", 25);
    
    Sprite* sp = Sprite::create("erMaJiang/loading/MjLoading1.png");
    sp->setPosition(Vec2(winsize.width - sp->getContentSize().width - roomLabel->getContentSize().width , sp->getContentSize().height));
    this->addChild(sp , 1);
    
    roomLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    roomLabel->setPosition(sp->getPosition() + Vec2(sp->getContentSize().width/1.5f, -sp->getContentSize().height/4));
    this->addChild(roomLabel);
    
    Animation* pAnimation = Animation::create();
    for (int i = 1; i <= 8; i++)
    {
        char fileName[60];
        sprintf(fileName, "erMaJiang/loading/MjLoading%d.png", i);
        pAnimation->addSpriteFrameWithFile(fileName);
    }
    pAnimation->setDelayPerUnit(1.5f / 8);
    pAnimation->setRestoreOriginalFrame(true);
    Animate* pAnimate = Animate::create(pAnimation);
    sp->runAction(RepeatForever::create(pAnimate));
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(HallLayer::requestGameTimerCallBack), this, 0.5, 0, 0.0f, false);
    
    return true;
}


void HallLayer::returnHallLayer()
{
    auto scene = Scene::create();
    auto mainlayer = HallLayer::create();
    scene->addChild(mainlayer);
    Director::getInstance()->replaceScene(TransitionSlideInL::create(0.3f, scene));
}


Layer* HallLayer::craeteHallLayer()
{
    return HallLayer::create();
}

void HallLayer::onHandleRoomListMessage()
{
    this->unschedule(schedule_selector(HallLayer::reRequest));
    
    Size winSize = Director::getInstance()->getWinSize();
    auto gameRoomLayer = GameRoom::create();
    gameRoomLayer->setPosition(Vec2(0, 0));
    gameRoomLayer->setGameInfo(_nRoomType);
    //屏蔽其它层的消息
    gameRoomLayer->setTouchEnabled(true);
    gameRoomLayer->setTouchMode(kCCTouchesOneByOne);
    addChild(gameRoomLayer, ROOM_ZORDER, CHILD_ROOM_LAYER_TAG);
    
    //gameRoomLayer->runAction(EaseSineOut::create(MoveBy::create(0.001f, Vec2(winSize.width, 0))));
}

void HallLayer::onHandleGameListMessage()
{
    this->unschedule(schedule_selector(HallLayer::reRequest));
    roomLabel->setString("正在加载大厅信息...");
    
    ComNameInfo* nameInfo = GamesInfoModule()->findGameName(ErMahjong::GAME_NAME_ID);
    GameCreator()->addGame(ErMahjong::GAME_NAME_ID, nameInfo->uKindID, YZGameCreator::NORMAL, GAME_CREATE_SELECTOR(ErMahjong::GameTableUI::create));
   
    this->scheduleOnce(schedule_selector(HallLayer::reRequest), waitTime);
    
    PlatformLogic()->requestRoomListInfo(nameInfo->uKindID, ErMahjong::GAME_NAME_ID);
}

void HallLayer::reRequest(float dt){
    auto layer = SocketErrorLayer::createLayer();
    addChild(layer, 1000);
}



void HallLayer::requestGameTimerCallBack(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(HallLayer::requestGameTimerCallBack), this);
    
    if (PlatformLogic()->connected())
    {
        this->scheduleOnce(schedule_selector(HallLayer::reRequest), waitTime);
        roomLabel->setString("正在加载游戏信息...");
        // 请求游戏列表信息
        PlatformLogic()->requestGameListInfo();
    }
    else
    {
        LoadingLayer::removeLoading(this);
        auto prompt = GamePromptLayer::create();
        prompt->showPrompt(GBKToUtf8("网络断开，请重新登陆。"));
        prompt->setCallBack([]()
                            {
                                LoginLayer::returnLogin();
                            });
    }
}


void HallLayer::gameSelect(Ref * pSender,Widget::TouchEventType type)
{
    ImageView *image = (ImageView *)pSender;
    
    _nRoomType = image->getTag();
    if(type==Widget::TouchEventType::ENDED && image)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        switch (image->getTag())
        {
            case GAME_PDK:
            {
                //LoadingLayer::createLoading(this, gFontConfig_22, GBKToUtf8("获取房间列表......"), LOADING);
                Director::getInstance()->getScheduler()->schedule(schedule_selector(HallLayer::requestGameTimerCallBack), this, 0.5, 0, 0.0f, false);
                ComNameInfo* nameInfo = GamesInfoModule()->findGameName(ErMahjong::GAME_NAME_ID);
                GameCreator()->addGame(ErMahjong::GAME_NAME_ID, nameInfo->uKindID, YZGameCreator::NORMAL, GAME_CREATE_SELECTOR(ErMahjong::GameTableUI::create));
                PlatformLogic()->requestRoomListInfo(nameInfo->uKindID, ErMahjong::GAME_NAME_ID);
            }
                break;
            default:
                break;
        }
    }
}


bool HallLayer::isDigitStr(const char* cstr)
{
    if (NULL == cstr || cstr[0] == 0)
    {
        return false;
    }
    
    int len = strlen(cstr);
    int pos = 0;
    if (cstr[0] == '-' || cstr[0] == '+')
    {
        if (len <= 1)
        {
            return false;
        }
        
        pos++;
    }
    
    while (pos < len)
    {
        if (cstr[pos] < '0' || cstr[pos] > '9')
        {
            return false;
        }
        pos++;
    }
    
    return true;
    
}




//手机返回键监听回调函数
void HallLayer::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if (EventKeyboard::KeyCode::KEY_BACK == keyCode)
    {
        auto exitGame = ExitLayer::create();
        exitGame->showPrompt();
    }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode)
    {
        auto exitGame = ExitLayer::create();
        exitGame->showPrompt();
    }
#endif
    
}

void HallLayer::setOpenSave(bool isOpen)
{
    _isOpen=isOpen;
}


void HallLayer::onBtnClicked(Ref * pSender,Widget::TouchEventType type)
{
    Button *btn = (Button*)pSender;
    
    if(type==Widget::TouchEventType::BEGAN && btn)
    {
        ParticleSystemQuad *mParticle =  ParticleSystemQuad::create("p_click.plist");
        mParticle->setPosition(convertToWorldSpace(btn->getPosition()));
        btn->getParent()->addChild(mParticle,100);
    }
    
    if(type==Widget::TouchEventType::ENDED && btn)
    {
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLICK);
        
        log("tag = %d", btn->getTag());
        switch (btn->getTag())
        {
            case BTN_STORE:
            {
               
            }
                break;
            case BTN_BANK:
            {
                log("saveBox layer opened !!!");
                auto layer = SaveBoxLayer::createLayer(this,_isOpen);
                addChild(layer,1000);
                
            }
                break;
            case BTN_HELP:
            {
                auto layer=HelpLayer::create();
                addChild(layer,10000);
            }
                break;
            case BTN_SHARE:
            {
                log("IMAGE_SHARE");
                auto layer=ShareLayer::create();
                addChild(layer,10000);
                
            }
                break;
            case BTN_SHARE_EXIT:
            {
                removeChildByTag(LAYER_SHARE);
            }
                break;
            case BTN_EXIT:
            {
                auto exitGame = ExitLayer::create();
                exitGame->showPrompt();
            }
                break;
            case BTN_SET:
            {
                auto layer = SetLayer::create();
                addChild(layer, 1000);
            }
                break;
            case BTN_HEAD:
            {
                auto layer=PersonalLayer::create();
                addChild(layer,1000);
            }
                break;
            default:
                break;
        }
    }
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void shareCallback(int platform, int stCode, string& errorMsg)
{
    if ( stCode == 100 )
    {
        log("#### Hello start 10000");
    }
    else if ( stCode == 200 )
    {
        auto scene = Director::getInstance()->getRunningScene();
        if (scene->getChildByTag(110011))
        {
            auto layer = (HallLayer*)scene->getChildByTag(110011);
            layer->sendShareSuc();
        }
    }
    else
    {
        log("#### Hello fail 30000");
    }
    
    log("platform num is : %d.", platform);
}
#endif

void HallLayer::setHallHeadPic()
{
    MSG_GP_R_LogonResult& LogonResult = PlatformLogic()->loginResult;
    auto picId =  LogonResult.bLogoID;
    char picName[60];
    memset(picName, 0, sizeof(picName));
    sprintf(picName, "%s%02d%s", Name_Local_Head_Pic,picId,".png");
    personal->loadTextureNormal(Name_Defaul_Pic);
    personal->loadTexturePressed(Name_Defaul_Pic);
    personal->loadTextureNormal(picName);
    personal->loadTexturePressed(picName);
    
}

void HallLayer::modifyName(std::string name)
{
    label_userName->setString(GBKToUtf8(name.c_str()));
}

void HallLayer::setMsgRollAction()
{
    label_up_msg->stopAllActions();
    Size ppos = label_up_msg->getParent()->getContentSize();
    label_up_msg->setPosition(Vec2(ppos.width*1.1f,ppos.height/2));
    
    auto seq = Sequence::create(MoveTo::create(10.0f,Vec2(-ppos.width*0.1,ppos.height/2)),
                                DelayTime::create(10.0f),
                                MoveTo::create(10.0f,Vec2(-ppos.width*0.1,ppos.height/2)),
                                CallFunc::create([=]()
                                                 {
                                                     label_up_msg->setPosition(Vec2(ppos.width*1.1f,ppos.height/2));
                                                 }),NULL);
    
    label_up_msg->runAction(RepeatForever::create(seq));
}

void HallLayer::walletChanged(LLONG money)
{
    if (user_goldInfo)
    {
        char buffer[32];
        sprintf(buffer, "%lld", money);
        user_goldInfo->setString(buffer);
    }
}

// Add Game PageView
void HallLayer::createGameList()
{
    Size winSize = Director::getInstance()->getWinSize();
    
    _pageViewGames = PageView::create();
    _pageViewGames->setContentSize(pageViewGamesSize);
    _pageViewGames->setAnchorPoint(Vec2(0.5f, 0.5f));
    _pageViewGames->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f - 30));
    _pageViewGames->setTouchEnabled(true);
    _pageViewGames->setCustomScrollThreshold(10);
    addChild(_pageViewGames, 0);
    
    int gameAllCount = maxGameNum;
    int currentIndex = GAME_PDK;
    
    int pageCount = (gameAllCount / 4);
    if (pageCount > 0)
    {
        for (int currentPage  = 0; currentPage < pageCount; currentPage++)
        {
            int game1_tag = currentIndex;
            int game2_tag = currentIndex+1;
            int game3_tag = currentIndex+2;
            int game4_tag = currentIndex+3;
            
            createGame(game1_tag, game2_tag, game3_tag,game4_tag);
            currentIndex = currentIndex + 4;
        }
    }
    
    int remainder = (gameAllCount % 4);
    if (remainder > 0)
    {
        createGame(currentIndex, currentIndex+1, currentIndex+2,currentIndex+3);
    }
    updateArrow(_pageViewGames);
}


//添加房间
void HallLayer::createGame(int _gTag1,int _gTag2,int _gTag3,int _gTag4)
{
    int maxTag = GAME_PDK+maxGameNum;
    if (_gTag1 >= maxTag) return;
    //创建房间列表子页面
    auto gameItemLayout = Layout::create();
    gameItemLayout->setName("page");
    gameItemLayout->setContentSize(pageViewGamesSize);
    
    if (_gTag1 < maxTag)
    {
        auto roomItem = createGameItem(_gTag1);
        if (nullptr != roomItem)
        {
            roomItem->setPosition(Vec2(gameItemLayout->getContentSize().width * 0.12f, gameItemLayout->getContentSize().height * 0.5f));
            gameItemLayout->addChild(roomItem, 3);
        }
    }
    
    if (_gTag2 < maxTag)
    {
        auto roomItem = createGameItem(_gTag2);
        if (nullptr != roomItem)
        {
            roomItem->setPosition(Vec2(gameItemLayout->getContentSize().width * 0.37f, gameItemLayout->getContentSize().height * 0.5f));
            gameItemLayout->addChild(roomItem, 3);
        }
    }
    
    if (_gTag3 < maxTag)
    {
        auto roomItem = createGameItem(_gTag3);
        if (nullptr != roomItem)
        {
            roomItem->setPosition(Vec2(gameItemLayout->getContentSize().width * 0.63f, gameItemLayout->getContentSize().height * 0.5f));
            gameItemLayout->addChild(roomItem, 3);
        }
    }
    if (_gTag4 < maxTag)
    {
        auto roomItem = createGameItem(_gTag4);
        if (nullptr != roomItem)
        {
            roomItem->setPosition(Vec2(gameItemLayout->getContentSize().width * 0.88f, gameItemLayout->getContentSize().height * 0.5f));
            gameItemLayout->addChild(roomItem, 3);
        }
    }
    
    
    
    // 添加子页面进入列表中
    _pageViewGames->addPage(gameItemLayout);
    
    // 拖动监听
    _pageViewGames->addEventListener(PageView::ccPageViewCallback(CC_CALLBACK_2(HallLayer::pageViewMoveCallBack, this)));
}

//创建房间的显示图片
ImageView* HallLayer::createGameItem(int _gTag)
{
    ImageView* b_game = NULL;
    
    if (_gTag == GAME_PDK)
    {
        b_game = ImageView::create(IMG_GAME_DDZ);
    }
    
    b_game->addTouchEventListener(CC_CALLBACK_2(HallLayer::gameSelect, this));
    b_game->setTouchEnabled(true);
    b_game->setTag(_gTag);
    
    return b_game;
}

void HallLayer::pageViewMoveCallBack(Ref* pSender, PageView::EventType type)
{
    switch (type)
    {
        case PageView::EventType::TURNING:
        {
            PageView* pageView = dynamic_cast<PageView*>(pSender);
            updateArrow(pageView);
        }
            break;
        default:
            break;
    }
}

//子页面按钮回调
void HallLayer::enterGameEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    
}

void HallLayer::arrowCallBack(Ref* pSender, Widget::TouchEventType type, INT buttonTag)
{
    
}

void HallLayer::updateArrow(PageView* pageView)
{
    
}

void HallLayer::sendShareSuc()
{
    MSG_GP_S_USER_SHARE user_share;
    user_share.dwUserID = PlatformLogic()->loginResult.dwUserID;
    PlatformLogic()->sendData(MDM_GP_USER_RANK, ASS_GP_S_USER_SHARE_REWARD, &user_share, sizeof(user_share));
    PlatformLogic()->addEventSelector(MDM_GP_USER_RANK, ASS_GP_R_USER_SHARE_REWARD, YZ_SOCKET_CALLBACK(HallLayer::shareEventBack, this));
}

bool HallLayer::shareEventBack( YZSocketMessage* socketMessage )
{
    MSG_GP_R_USER_SHARE_REWARD* share_result = (MSG_GP_R_USER_SHARE_REWARD*)socketMessage->object;
    int value = share_result->m_iVuale;
    int day = share_result->m_day;
    PlatformLogic()->loginResult.i64Money += value;
    walletChanged(PlatformLogic()->loginResult.i64Money);
    if (day>0)
    {
        _uiShareLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("AwardLayer/award_2.json");
        _uiShareLayer->setTag(LAYER_SHARE);
        _uiShareLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
        _uiShareLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
        addChild(_uiShareLayer);
        
        auto btn_exit = static_cast<Button*>(Helper::seekWidgetByName(_uiShareLayer, "Button_12"));
        btn_exit->addTouchEventListener(CC_CALLBACK_2(HallLayer::onBtnClicked, this));
        btn_exit->setTag(BTN_SHARE_EXIT);
        
        ImageView* img_guang = (ImageView*)(Helper::seekWidgetByName(_uiShareLayer,"img_light"));
        auto actionTo = RotateBy::create(2,360);
        img_guang->runAction(RepeatForever::create(Sequence::create(actionTo, nullptr)));
        
        auto label = dynamic_cast<ui::TextAtlas*>(ui::Helper::seekWidgetByName(_uiShareLayer, "AtlasLabel_1"));
        char cDay[20] = {0};
        sprintf(cDay, "%d", day);
        label->setString(cDay);
    }
    
    return true;
}

void HallLayer::showLoadAward()
{
    auto _uiAwardLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("AwardLayer/award_1.json");
    _uiAwardLayer->setTag(LAYER_AWARD);
    _uiAwardLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiAwardLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    addChild(_uiAwardLayer);
    
    auto btn_exit = static_cast<Button*>(Helper::seekWidgetByName(_uiAwardLayer, "btn_mask"));
    btn_exit->addTouchEventListener(CC_CALLBACK_2(HallLayer::onBtnClicked, this));
    //btn_exit->setTag(BTN_AWARD_EXIT);
    
    ImageView* img_guang = (ImageView*)(Helper::seekWidgetByName(_uiAwardLayer,"img_light"));
    auto actionTo = RotateBy::create(2,360);
    img_guang->runAction(RepeatForever::create(Sequence::create(actionTo, nullptr)));
    
}



