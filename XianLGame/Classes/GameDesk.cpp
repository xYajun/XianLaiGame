#include "GameDesk.h"
#include "HallLayer.h"
#include "FontConfig.h"
#include "StoreLayer.h"
#include "PlatformDefine.h"
#include "GamePrompt.h"
#include "PlatformConfig.h"
#include "LoginLayer.h"
#include "GlobalUnits.h"
#include "ExitLayer.h"
#include "SetLayer.h"
#include "RecordLayer.h"
#include "Util/JzLog.h"
#include "UI/HttpSprite.h"
#include "HelpLayer.h"
#include "LoadingScene.h"
#include "PlayerInfoLayer.h"
#include "SocketErrorLayer.h"
//////////////////////////////////////////////////////////////////////////
static const int CHILD_ZORDER			= 4000;			// ◊”Ω⁄µ„µØ≥ˆøÚ≤„º∂
static const int ROOM_PEOPLE_TAG		= 1000;          //µ±«∞∑øº‰»À ˝
static const int CHILD_STORE_LAYER_TAG	= 1001;			// …ÃµÍ≤„±Í«©

static const int GAME_PDK_TAG	= 2001;
static const int GAME_RULEEXIT_TAG	= 2005;
static const int GAME_SETEXIT_TAG	= 2006;
static const int GAME_SETOK_TAG	    = 2007;
static const int GAME_INPUTOK_TAG	    = 2008;

static const char* enter_desk_text			= "正在请求加入房间...";


//////////////////////////////////////////////////////////////////////////
static const char* NUMBER			= "桌号";
static const char* BLIND_NOTE		= "倍数";
static const char* SUM_OF_MONEY		= "金额";
static const char* PEOPLE_NUM		= "人数";
//////////////////////////////////////////////////////////////////////////
static const float game_network_check_timer_time = 0.5f;
//////////////////////////////////////////////////////////////////////////
static const char* ROOMLIST_UP		= "platform/room/desklist_up.png";                         //backGround up
static const char* LIST_BG			= "platform/room/destList_bg.jpg";         //listBG
static const char* RULEBTN		    = "platform/btn_rule.png";
static const char* SETBTN		    = "platform/btn_set.png";
static const char* RETURNBTN		="platform/btn_return.png";                     //returnBtn
static const char* QUICK_START	    = "platform/btn_quick.png";               //quick_start_btn


static const char* DESK_ITEM_PATH	= "platform/DestItemUi.json";
static const char* FIVE_DESK_ITEM_PATH	= "platform/DestItemUi2.json";

static const char * OPEN_ROOM_HALL_BG = "HallLayer/HallLayer_1.json";
static const char * OPEN_ROOM_HALL_TOP = "HallLayer/Hall_Top.json";

static const char * CREATE_ROOM_LAYER = "platform/openRoom/RoomLayer/RoomLayer_1.json";     //鍒涘缓鎴块棿
static const char * ENTER_IVCODE_LAYER = "platform/openRoom/RoomLayer/RoomLayer_3.json";     //閭�鐮佺晫闈�
static const char * GET_ROOM_CARD_LAYER = "platform/openRoom/RoomLayer/RoomLayer_4.json";     //鍏戞崲鎴垮崱
static const char * ENTER_GAME_LAYER = "platform/openRoom/RoomLayer/RoomLayer_5.json";       //鍔犲叆鎴块棿

static const char * OPEN_CREATE_ROOM_IAMGE = "HallLayer/res/btn_craeteRoom.png";
static const char * RECOVERY_CREATE_ROOM_IMAGE = "HallLayer/res/btn_recoveryRoom.png";


#define TAG_BTN_ADD_MONEY      200
#define TAG_BTN_EXIT_CREATE_ROOM       201
#define TAG_BTN_CREATE_ROOM    202
#define TAG_BTN_ENTER_ROOM     203
#define TAG_BTN_OPEN_CREATEROOM 204
#define TAG_BTN_CLOSE_ENTER_ROOM 205
#define TAG_BTN_RECOVERY_ROOM 206
#define TAG_BTN_KEFU 207        //客服按钮
#define TAG_BTN_WANFA 208       //玩法
#define TAG_BTN_RECORD     209
#define TAG_BTN_SHOP 263       //商店

#define TAG_BTN_ADD_BUY_CARD 210        //添加购买房卡数量
#define TAG_BTN_SUB_BUY_CARD 211        //减少购买房卡数量
#define TAG_BTN_OK_BUY_CARD  212        //确认购买房卡
#define TAG_BTN_CANCEL_BUY_CARD  213    //取消购买房卡

#define TAG_CR_CB_1             250        //创建房间选择框8局
#define TAG_CR_CB_2             251        //创建房间选择框16局


#define TAG_ZM_CB_1             260        //创建房间扎码选择框
#define TAG_ZM_CB_2             261
#define TAG_ZM_CB_3             262


#define TAG_BTN_INPUT_NUM_0     300
#define TAG_BTN_INPUT_NUM_1     301
#define TAG_BTN_INPUT_NUM_2     302
#define TAG_BTN_INPUT_NUM_3     303
#define TAG_BTN_INPUT_NUM_4     304
#define TAG_BTN_INPUT_NUM_5     305
#define TAG_BTN_INPUT_NUM_6     306
#define TAG_BTN_INPUT_NUM_7     307
#define TAG_BTN_INPUT_NUM_8     308
#define TAG_BTN_INPUT_NUM_9     309
#define TAG_BTN_INPUT_NUM_10    310        //清除
#define TAG_BTN_INPUT_NUM_11    311        //删除


//////////////////////////////////////////////////////////////////////////

void GameDesk::createDesk(ComRoomInfo* roomInfo, bool isHaveDesk)
{
    
    auto deskScene = Scene::create();
    auto desk = GameDesk::create(isHaveDesk);
    desk->_roomInfo = roomInfo;
    //desk->addDesk(roomInfo);
    deskScene->addChild(desk);
    
    JzLog::shareLog()->bindViewToScene(deskScene);
    
    //Director::getInstance()->replaceScene(TransitionSlideInR::create(0.3f, deskScene));
    Director::getInstance()->replaceScene(deskScene);
}

GameDesk::GameDesk(bool isHaveDesk)
: _currentSelectedDesk(nullptr)
, _roomPeopleCount(0)
, _quicklyGame(true)
, _isSelect(false)
, _selectIndex(-1)
, _roomInfo(nullptr)
,_seatNo(255)
,_iMySet(false)
,_iCanEnter(false)
,_myDeskNo(0)
,_currentSelectDeskNo(-1)
,_selectRoomCount(10)
{
    _isHaveDesk = isHaveDesk;
    
}

GameDesk* GameDesk::create(bool isHaveDesk)
{
    GameDesk *pRet = new GameDesk(isHaveDesk);
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

GameDesk::~GameDesk()
{
    clearDesksData();
}

bool GameDesk::init()
{
    if (!YZRoomEventLayer::init()) return false;
    
    YZAudioEngine::getInstance()->stopBackgroundMusic();
    YZAudioEngine::getInstance()->playBackgroundMusic(GAME_BACKGROUND_MUSICYL, true);
    
    if (g_GlobalUnits.isOpenRoom) {
        
        JzLog::shareLog()->log(kLogLevelDebug, "开房模式....");
        
        initOpenRoomDesk();
        
    }else{
        
    }
    
    return true;
}

//void GameDesk::onDeskInfoMessage(){
//	MSG_GR_DeskStation curDeskStation = RoomLogic()->deskStation;
//	return;
//}

#pragma mark -------------- 开房模式 --------------

void GameDesk::initOpenRoomDesk(){
    
    Size size = Director::getInstance()->getVisibleSize();
    waitLayer = LayerColor::create(Color4B::BLACK, size.width, size.height);
    this->addChild(waitLayer , 999);

    auto roomLabel = Label::createWithTTF("正在加载房间信息...", "fonts/DroidSansFallback.ttf", 25);
    
    Sprite* sp = Sprite::create("erMaJiang/loading/MjLoading1.png");
    sp->setPosition(Vec2(size.width - sp->getContentSize().width - roomLabel->getContentSize().width , sp->getContentSize().height));
    waitLayer->addChild(sp , 1);
    
    roomLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    roomLabel->setPosition(sp->getPosition() + Vec2(sp->getContentSize().width/1.5f, -sp->getContentSize().height/4));
    waitLayer->addChild(roomLabel);
    
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
    waitLayer->runAction(Sequence::createWithTwoActions(DelayTime::create(10.0f), CallFunc::create([=]{
        waitLayer->removeFromParentAndCleanup(true);
        waitLayer = NULL;
    })));
    
    auto _uiRoomLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(OPEN_ROOM_HALL_BG);
    _uiRoomLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiRoomLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    addChild(_uiRoomLayer);
    
    auto hallBG =(ImageView*)(Helper::seekWidgetByName(_uiRoomLayer,"hall_bg"));
    hallBG->setScale(Win_w/hallBG->getContentSize().width,Win_h/hallBG->getContentSize().height);
    
    auto _uiRoomLayerTop = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(OPEN_ROOM_HALL_TOP);
    _uiRoomLayerTop->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiRoomLayerTop->setPosition(Vec2(Win_w / 2, Win_h - _uiRoomLayerTop->getContentSize().height/2));
    addChild(_uiRoomLayerTop, 2);
    
    auto btnRecord = (Button*)(Helper::seekWidgetByName(_uiRoomLayerTop,"btn_record"));
    btnRecord->setTag(TAG_BTN_RECORD);
    btnRecord->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    // 创建房间按钮
    createRoomBtn = (Button*)(Helper::seekWidgetByName(_uiRoomLayer,"btn_createRoom"));
    createRoomBtn->setTag(TAG_BTN_OPEN_CREATEROOM);
    createRoomBtn->setVisible(false);
    createRoomBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    auto enterGameBtn = (Button*)(Helper::seekWidgetByName(_uiRoomLayer,"btn_enterRoom"));
    enterGameBtn->setTag(TAG_BTN_ENTER_ROOM);
    enterGameBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    std::string headUrl = RoomLogic()->loginResult.pUserInfoStruct.szHeadUrl;
    
    JzDebug("加入房间 自己的头像链接为 : %s " , headUrl.c_str());
    
    _headImg = (ImageView*)(Helper::seekWidgetByName(_uiRoomLayerTop,"head_img"));
    _headImg->setVisible(false);
    //auto httpSp = HttpSprite::create(headUrl, "pdk/game/head_defult.png");
    auto httpSp = HttpSprite::create(headUrl, RoomLogic()->loginResult.pUserInfoStruct.bBoy ? "erMaJiang/DisplayPicture/man.png" : "erMaJiang/DisplayPicture/woman.png");
    httpSp->setImgSize(_headImg->getContentSize()/1.5f);
    httpSp->setPosition(_headImg->getPosition());
    httpSp->onClickSpriteCallBack(CC_CALLBACK_0(GameDesk::clickHeadImage, this));
    _uiRoomLayerTop->addChild(httpSp,10);
  
    
    char str[60]={0};
    auto label_nick = (Text*)(Helper::seekWidgetByName(_uiRoomLayerTop,"label_nick"));
    label_nick->setString(GBKToUtf8(PlatformLogic()->loginResult.nickName));
    
    JzDebug("nickname : %s " ,GBKToUtf8(PlatformLogic()->loginResult.nickName));
    JzDebug("dwUserID : %d " ,PlatformLogic()->loginResult.dwUserID);

    SaveIntegerToXML(SAVE_USERID, PlatformLogic()->loginResult.dwUserID);

    label_roomCount = (Text*)(Helper::seekWidgetByName(_uiRoomLayerTop,"label_gold"));
    
    //创建房间
    _uiCreateRoomLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(CREATE_ROOM_LAYER);
    _uiCreateRoomLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiCreateRoomLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    _uiCreateRoomLayer->setVisible(false);
    addChild(_uiCreateRoomLayer , 4);
    
    auto doCreateRoomBtn = (Button*)(Helper::seekWidgetByName(_uiCreateRoomLayer,"btn_create"));
    doCreateRoomBtn->setTag(TAG_BTN_CREATE_ROOM);
    doCreateRoomBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    auto exitCreateRoomBtn = (Button*)(Helper::seekWidgetByName(_uiCreateRoomLayer,"btn_exit"));
    exitCreateRoomBtn->setTag(TAG_BTN_EXIT_CREATE_ROOM);
    exitCreateRoomBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    
    char strRoomCountName[64] = {0};
    for (int i = 0; i < 2; i ++) {
        sprintf(strRoomCountName,"inning_cb%d",i + 1);
        auto checkBox = (ui::CheckBox*)(Helper::seekWidgetByName(_uiCreateRoomLayer,strRoomCountName));
        checkBox->setTag(TAG_CR_CB_1 + i);
        if (i == 0) {
            checkBox->setSelected(true);
        }else{
            checkBox->setSelected(false);
        }
        checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(GameDesk::selectedRoomCountStateEvent));
        roomCountCBoxVec.push_back(checkBox);
    }
    
    
    char strRoomZMName[64] = {0};
    for (int i = 0; i < 3; i ++) {
        sprintf(strRoomZMName,"zm_cb%d",i + 1);
        auto checkBox = (ui::CheckBox*)(Helper::seekWidgetByName(_uiCreateRoomLayer,strRoomZMName));
        checkBox->setTag(TAG_ZM_CB_1 + i);
        if (i == 0) {
            checkBox->setSelected(true);
        }else{
            checkBox->setSelected(false);
        }
        checkBox->addEventListenerCheckBox(this, checkboxselectedeventselector(GameDesk::selectedRoomZMStateEvent));
        
        roomZMCBoxVec.push_back(checkBox);
    }
    
    
    
    _uiGetRoomCardLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(GET_ROOM_CARD_LAYER);
    _uiGetRoomCardLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiGetRoomCardLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    _uiGetRoomCardLayer->setVisible(false);
    addChild(_uiGetRoomCardLayer , 5);
    
    //添加购买房卡数量按钮
    auto btnAddBuyCard=(ui::Button*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"btn_add"));
    btnAddBuyCard->setTag(TAG_BTN_ADD_BUY_CARD);
    btnAddBuyCard->addTouchEventListener(CC_CALLBACK_2(GameDesk::btnBuyRoomCardCallBack, this));
    
    //减少购买房卡数量按钮
    auto btnSubBuyCard=(ui::Button*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"btn_sub"));
    btnSubBuyCard->setTag(TAG_BTN_SUB_BUY_CARD);
    btnSubBuyCard->addTouchEventListener(CC_CALLBACK_2(GameDesk::btnBuyRoomCardCallBack, this));
    
    //确认购买房卡按钮
    auto btnOkBuyCard=(ui::Button*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"btn_recharge"));
    btnOkBuyCard->setTag(TAG_BTN_OK_BUY_CARD);
    btnOkBuyCard->addTouchEventListener(CC_CALLBACK_2(GameDesk::btnBuyRoomCardCallBack, this));
    
    //放弃购买房卡按钮
    auto btnCancelCard=(ui::Button*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"btn_giveUp"));
    btnCancelCard->setTag(TAG_BTN_CANCEL_BUY_CARD);
    btnCancelCard->addTouchEventListener(CC_CALLBACK_2(GameDesk::btnBuyRoomCardCallBack, this));
    
    _buyRoomCardCountLabel = (ui::Text*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"Label_number"));
    _buyRoomCardCountLabel->setString("1");
    
    _buyRoomCardMoneyCount = (ui::Text*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"Label_cardNum"));
    
    _afterBuyRoomCardMoneyCount = (ui::Text*)(Helper::seekWidgetByName(_uiGetRoomCardLayer,"Label_mNum"));
    
    _uiEnterGameLayer = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(ENTER_GAME_LAYER);
    _uiEnterGameLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiEnterGameLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    _uiEnterGameLayer->setVisible(false);
    addChild(_uiEnterGameLayer , 3);
    
    auto closeEnterRoomLayer = (Button*)(Helper::seekWidgetByName(_uiEnterGameLayer,"btn_close"));
    closeEnterRoomLayer->setTag(TAG_BTN_CLOSE_ENTER_ROOM);
    closeEnterRoomLayer->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    auto btnExit=(ui::Button*)(Helper::seekWidgetByName(_uiRoomLayerTop,"btn_kefu"));
    btnExit->setTag(TAG_BTN_KEFU);
    btnExit->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    auto btnSet=(ui::Button*)(Helper::seekWidgetByName(_uiRoomLayerTop,"btn_wanFa"));
    btnSet->setTag(TAG_BTN_WANFA);
    btnSet->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    auto btnShop=(ui::Button*)(Helper::seekWidgetByName(_uiRoomLayerTop,"btn_addGold"));
    btnShop->setTag(TAG_BTN_SHOP);
    btnShop->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClickCallBack, this));
    
    char inputName[64] = {0};
    for(int i = 0; i <= 11; i++)
    {
        sprintf(inputName, "btn_num%d", i);
        auto numInput = (Button*)(Helper::seekWidgetByName(_uiEnterGameLayer,inputName));
        numInput->setTag(TAG_BTN_INPUT_NUM_0 + i);
        numInput->addTouchEventListener(CC_CALLBACK_2(GameDesk::inputRoomIdCallBack, this));
    }
    
    /*
     //清除按钮
     auto clearInputRoomId = (Button*)(Helper::seekWidgetByName(_uiEnterGameLayer,"btn_num10"));
     clearInputRoomId->setTag(TAG_BTN_INPUT_NUM_10);
     clearInputRoomId->addTouchEventListener(CC_CALLBACK_2(GameDesk::inputRoomIdCallBack, this));
     
     //后退按钮
     auto deleteLastInputRoomId = (Button*)(Helper::seekWidgetByName(_uiEnterGameLayer,"btn_num11"));
     deleteLastInputRoomId->setTag(TAG_BTN_INPUT_NUM_11);
     deleteLastInputRoomId->addTouchEventListener(CC_CALLBACK_2(GameDesk::inputRoomIdCallBack, this));
     */
    
    char inputLabel[64] = {0};
    for(int i = 1; i <= 6; i++)
    {
        sprintf(inputLabel, "Label_num%d", i);
        _roomNumTxt[i - 1]= (ui::Text*)(Helper::seekWidgetByName(_uiEnterGameLayer,inputLabel));
    }
    
    _selectRoomCount = 10;
    _selectRoomZMCount = 2;
    nowRoomNumIndex = 0;
    _selectBuyRoomCardCount = 1;
}

void GameDesk::hideCreateRoomLayer(){
    _uiCreateRoomLayer->setVisible(false);
}

//显示创建房间局数Layer
#pragma mark  --  确认创建房间
void GameDesk::createRoom(){
    
    //自动购买需要处理
    JzDebug( "使用房卡创建一个局数为 ：%d 的桌子..." , _selectRoomCount);
    
    this->hideCreateRoomLayer();
    
    MSG_PROP_C_OP_ROOMCARD data;
    memset(&data, 0, sizeof(data));
    data.dwUserID = PlatformLogic()->loginResult.dwUserID;
    
    int propId = 0;
    std::map<int , PropInfo>::iterator it;
    for(it=userPorpInfos.begin();it!=userPorpInfos.end();++it)
        propId = it->second.nPropID;
    data.nPropCount = 1;
    data.nPropID = propId;
    data.dwGameRoundNum = _selectRoomCount;
    data.dwZaCodeNum = _selectRoomZMCount;
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_USE, &data, sizeof(data));
}

void GameDesk::clickHeadImage(){
    CCLOG("点击头像， 显示玩家信息界面");
    
    MSG_GP_R_LogonResult res = PlatformLogic()->loginResult;
    
    PlayerInfo pInfo;
    pInfo.bBoy = res.bBoy;
    pInfo.nickName = res.nickName;
    pInfo.dwUserID = res.dwUserID;
    pInfo.dwUserIP = res.dwNowLogonIP;
    pInfo.isHall = true;
    pInfo.headUrl = res.szHeadUrl;
    auto layer = PlayerInfoLayer::createLayer(pInfo);
    addChild(layer, 1000);
}

//显示加入房间Layer
void GameDesk::enterRoomDesk(){
    
    char roomId[6];
    std::string str= "";
    for (int i = 0; i< 6; i++) {
        
        str +=_roomNumTxt[i]->getString().c_str();
        roomId[i] = str[i];
        roomPwd[i] =  str[i];
    }
    
    //memset(&roomPwd, 0, sizeof(roomPwd));
    //strcpy(roomPwd, roomId);
    
    JzDebug("加入房间 房间号为: %s " , str.c_str());
    
    this->queryBackRoom();
}



void GameDesk::onBtnClickCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype){
    if (Widget::TouchEventType::ENDED != touchtype)	return;
    Button* ptr = (Button*)pSender;
    switch (ptr->getTag()){
        case TAG_BTN_ADD_MONEY:
            CCLOG("购买金币...");
            break;
            //case TAG_BTN_ADD_CARD:
            //    CCLOG("购买房卡..");
            //    this->showBuyRoomCardLayer();
            //    break;
        case TAG_BTN_OPEN_CREATEROOM:
        {
            _selectRoomCount = 10;
            _uiCreateRoomLayer ->setVisible(true);
        }
            break;
            
        case TAG_BTN_EXIT_CREATE_ROOM:
            _uiCreateRoomLayer->setVisible(false);
            break;
            
        case TAG_BTN_RECOVERY_ROOM:
            this->queryBackRoom();
            break;
        case TAG_BTN_ENTER_ROOM:
            
            for (int i = 0; i < 6; i++) {
                _roomNumTxt[i]->setString("");
            }
            nowRoomNumIndex = 0;
            _uiEnterGameLayer->setVisible(true);
            break;
        case TAG_BTN_CREATE_ROOM:
            this->createRoom();
            break;
            
        case TAG_BTN_CLOSE_ENTER_ROOM:
            _uiEnterGameLayer->setVisible(false);
            break;
        case TAG_BTN_KEFU:
        {
            
        }
            break;
        case TAG_BTN_WANFA:
        {
            auto layer = HelpLayer::create();
            addChild(layer, 1000);
        }
            break;
        case TAG_BTN_RECORD:
        {
            this->queryGameRecordFromServer();
        }
            break;
        case TAG_BTN_SHOP:
        {
            int propId = 0;
            std::map<int , PropInfo>::iterator it;
            for(it=userPorpInfos.begin();it!=userPorpInfos.end();++it)
                propId = it->second.nPropID;
            auto layer = StoreLayer::createLayer(this , propId);
            addChild(layer, 1000);
        }
            
            
            break;
    }
}


void GameDesk::btnBuyRoomCardCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype){
    if (Widget::TouchEventType::ENDED != touchtype)	return;
    
    Button* ptr = (Button*)pSender;
    switch (ptr->getTag()){
        case TAG_BTN_ADD_BUY_CARD:{
            changeBuyRoomCardCount(1);
        }
            break;
        case TAG_BTN_SUB_BUY_CARD:
        {
            changeBuyRoomCardCount(-1);
        }   break;
        case TAG_BTN_OK_BUY_CARD:
        {
            this->buyRoomCard();
        }
            break;
        case TAG_BTN_CANCEL_BUY_CARD:
        {
            this->showBuyRoomCardLayer(false);
        }   break;
    }
}

void GameDesk::changeBuyRoomCardCount(int count){
    
    if (count < 0  && _selectBuyRoomCardCount <= 1) {
        return;
    }
    _selectBuyRoomCardCount += count;
    char name[60]={0};
    sprintf(name,"%d",_selectBuyRoomCardCount);
    _buyRoomCardCountLabel->setString(name);
    
    sprintf(name,"%lld",RoomLogic()->loginResult.pUserInfoStruct.i64Money);
    _buyRoomCardMoneyCount->setString(name);
    // _buyRoomCardMoneyCount->setString(to_string(RoomLogic()->loginResult.pUserInfoStruct.i64Money));
    LLONG afterBuyMoneyCount = RoomLogic()->loginResult.pUserInfoStruct.i64Money - userPorpInfos[_selectRoomCount].iPrice * _selectBuyRoomCardCount;
    sprintf(name,"%lld",afterBuyMoneyCount);
    _afterBuyRoomCardMoneyCount->setString(name);
}



void GameDesk::inputRoomIdCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype){
    if (Widget::TouchEventType::ENDED != touchtype)	return;
    
    Button* ptr = (Button*)pSender;
    
    int tag = ptr->getTag();
    
    if (tag >= TAG_BTN_INPUT_NUM_0 && tag <= TAG_BTN_INPUT_NUM_9) {
        
        if (nowRoomNumIndex >= 6) {
            return;
        }
        
        char name[60]={0};
        sprintf(name,"%d",tag - TAG_BTN_INPUT_NUM_0);
        _roomNumTxt[nowRoomNumIndex]->setString(name);
        nowRoomNumIndex ++;
        
        if (nowRoomNumIndex == 6) {
            this->enterRoomDesk();
        }
        
    }else if(tag == TAG_BTN_INPUT_NUM_10){
        //清理
        
        for (int i = 0; i < 6; i++) {
            _roomNumTxt[i]->setString("");
        }
        
        nowRoomNumIndex = 0;
        
    }else if(tag == TAG_BTN_INPUT_NUM_11){
        //删除
        if (nowRoomNumIndex > 0 && nowRoomNumIndex <= 6) {
            _roomNumTxt[nowRoomNumIndex - 1]->setString("");
            
            nowRoomNumIndex--;
        }
    }
}


void GameDesk::btnChooseRoomCountCallBack(cocos2d::Ref* pSender, Widget::TouchEventType touchtype){
    if (Widget::TouchEventType::ENDED != touchtype)	return;
    
    Button* ptr = (Button*)pSender;
    int key = ptr->getTag();
    
    if (key == _selectRoomCount) {
        return;
    }
    
    map<int,PropInfo>::iterator it = userPorpInfos.find(key);
    if (it != userPorpInfos.end()) {
        PropInfo propInfo = userPorpInfos.at(key);
        
        int oldSelectRoom = _selectRoomCount;
        
        _selectRoomCount  = propInfo.attribValue;
    }
}

void GameDesk::queryBackRoom(){
    
    MSG_QueryBACKROOM_INFO qData;
    memset(&qData, 0, sizeof(qData));
    qData.dwUserID = PlatformLogic()->loginResult.dwUserID;
    
    std::string str= "";
    
    for(int i = 0 ; i < 6 ; i ++){
        qData.szPwd[i] = roomPwd[i];
        str += roomPwd[i];
    }
    
    JzDebug("设置房间密码为 : %s" , str.c_str());
    
    RoomLogic()->setInRoomNum(str);
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_QUERY_BACKROOM, &qData, sizeof(qData));
    
    // 转圈动画
    // LoadingLayer::createLoading(this, gFontConfig_22, enter_desk_text , LOADING);
}

//查询游戏记录
void GameDesk::queryGameRecordFromServer(){
    this->onQueryGameRecordMessage();
}

void GameDesk::onQueryGameRecordMessage(){
    auto layer = RecordLayer::create();
    addChild(layer, 1000);
}


#pragma mark ----- 房间消息处理 ---------------


void GameDesk::updateRoomPeople(INT count)
{
    count = count < 0 ? 0 : count;
    char str[128];
    sprintf(str, GBKToUtf8("»À ˝:%d"), count);
    auto labelPeople = (Label*)this->getChildByTag(ROOM_PEOPLE_TAG);
    labelPeople->setString(str);
}


void GameDesk::onQueryPropRoomCardInfo(MSG_PROP_S_QUERY_ROOMCARD * data , BYTE bHandleCode) {
    if (bHandleCode == 0) {
        userPorpInfos.at(data->attribValue).count = data->nHoldCount;
    }else if (bHandleCode == 1){
        //所有的道具查询完成。向服务器查询房间状态信息
        queryBackRoomInfoFromService();
        
        map<int,PropInfo>::iterator it;
        for(it=userPorpInfos.begin();it!=userPorpInfos.end();++it){
            JzDebug("查询道具信息    道具ID ： %d   道具名 : %s  ,  数量 ：%d " , it->first , it->second.szPropName.c_str() , it->second.count);
            char str[128];
            sprintf(str, GBKToUtf8("房卡 : %d"), it->second.count);
            label_roomCount->setString(str);
        }
    }
}


#pragma mark -- 查询房卡类型
void GameDesk::onQueryPropKindMessage(MSG_PROP_S_KIND_ROOMCARD * data , BYTE bHandleCode ){
    if (bHandleCode == 1) {
        
        INT userID = PlatformLogic()->loginResult.dwUserID;
        
        MSG_PROP_C_QUERY_ROOMCARD propData;
        memset(&propData, 0, sizeof(propData));
        propData.dwUserID = userID;
        propData.nPropID = 0;
        
        YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_GETINFO, &propData, sizeof(propData));
    }else if(bHandleCode == 0){
        PropInfo propInfo;
        propInfo.nPropID = data->nPropID;
        propInfo.szPropName = GBKToUtf8(data -> szPropName);					//道具名称
        propInfo.attribAction = data -> attribAction;				//操作属性
        propInfo.attribValue = data -> attribValue;					//取值属性
        propInfo.iPrice = data -> iPrice;							//道具价格
        propInfo.iVipPrice = data -> iVipPrice;						//VIP道具价格
        propInfo.count = 0;                          //数量
        userPorpInfos.insert(make_pair(propInfo.attribValue, propInfo));
    }
    
}

void GameDesk::showBuyRoomCardLayer(bool isShow){
    
    _uiGetRoomCardLayer->setVisible(isShow);
    
    if (!isShow) {
        _selectBuyRoomCardCount = 1;
        char name[60]={0};
        sprintf(name,"%d",_selectBuyRoomCardCount);
        _buyRoomCardCountLabel->setString(name);
    }else{
        this->changeBuyRoomCardCount(0);
    }
}

void GameDesk::onUserComeMessage(UserInfoStruct* user)
{
    CCLOG("用户加入房间....");
}

void GameDesk::onUserLeftMessage(UserInfoStruct* user)
{
    CCLOG("用户离开房间....");
}

#pragma mark - 用户坐下
void GameDesk::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
{
    do
    {
        if (nullptr == userSit)
        {
            GamePromptLayer::create()->showPrompt(GBKToUtf8("√‹¬Î¥ÌŒÛ°£"));
            break;
        }
        
        
        if (nullptr == user) break;
        
        if (INVALID_DESKSTATION == userSit->bDeskIndex) break;
        
        MSG_GP_R_LogonResult aa = PlatformLogic()->loginResult;
        if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
        {
            _myDeskNo=userSit->bDeskIndex;
        }
        
        if (_quicklyGame&&userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
        {
            Director::getInstance()->replaceScene(LoadingScene::createScene(_roomInfo->uNameID, userSit->bDeskIndex, true));
            
            //加入游戏
            /*
            bool ret = GameCreator()->startGameClient(_roomInfo->uNameID, userSit->bDeskIndex, true);
            if (!ret)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("”Œœ∑∆Ù∂Ø ß∞‹°£"));
            }
            else
            {
                //auto pDesk = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(item, "Image_desk"));
                //pDesk->loadTexture("platform/desk_res/comon_desk2.png");
                
            }
             */
        }
        //}
    } while (0);
    //	LoadingLayer::removeLoading(this);
}

bool GameDesk::onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
{
    MSG_GR_DeskStation * pDeskStation = (MSG_GR_DeskStation *)object;
    _curDeskStation = *pDeskStation;
    //add by LG
    addDesk(_roomInfo);
    
    /*if (messageHead->bMainID==MDM_GR_USER_LIST&&messageHead->bAssistantID==ASS_GR_DESK_STATION)
     {
     log("-----------------------------------");
     }
     else
     {
     return __super::onRoomFrameMessage(messageHead, object,objectSize);
     }*/
    return true;
}

void GameDesk::onBackRoomMessage(MSG_GR_BACKROOM_STATUS *data){
    
    if (data->cbCanBack) {
        //表示房间已经存在 恢复房间
        string pwd = "";
        for(int i = 0; i < 6; i++)
        {
            pwd += data->szPwd[i];
        }
        
        JzDebug("桌位号为 : %d   密码为 : %s " , data->uDeskIndex , pwd.c_str());
        
        strcpy(roomPwd,  data->szPwd);
        
        createRoomBtn->loadTextures(RECOVERY_CREATE_ROOM_IMAGE, RECOVERY_CREATE_ROOM_IMAGE);
        
        createRoomBtn->setTag(TAG_BTN_RECOVERY_ROOM);
        
        //this->queryBackRoom();
    }else{
        //表示用户第一次进入， 创建房间
        createRoomBtn->loadTextures(OPEN_CREATE_ROOM_IAMGE, OPEN_CREATE_ROOM_IAMGE);
        
        createRoomBtn->setTag(TAG_BTN_OPEN_CREATEROOM);
    }
    
    createRoomBtn->setVisible(true);

    if (waitLayer != NULL) {
        waitLayer->stopAllActions();
        waitLayer->removeFromParentAndCleanup(true);
        waitLayer= NULL;
    }
}

//使用房卡消息
void GameDesk::onUseRoomCardMessage(MSG_PROP_S_OP_ROOMCARD * data){
    CCLOG("GameDesk onUseRoomCardMessage..");
    //userPorpInfos[data->attribValue].count = data->nHoldCount;
}

//创建房间消息
void GameDesk::onCreateBackRoomMessage(MSG_BACKROOM_INFO * data){
    CCLOG("GameDesk onCreateBackRoomMessage..");
    
    int deskIndex = data->uDeskIndex;
    
    string pwd = "";
    for(int i = 0; i < 6; i++)
    {
        pwd += data->szPwd[i];
        roomPwd[i] = data->szPwd[i];
    }
    
    JzDebug("创建房间  桌位号为 : %d   密码为 : %s " , deskIndex , pwd.c_str());
    
    //strcpy(roomPwd,  data->szPwd);
    
    this->queryBackRoom();
    
}
#pragma mark - 查询房间信息回调
void GameDesk::onQueryBakcRoomMessage(MSG_QueryBACKROOM_RSP_INFO * data , BYTE code){
    
    //LoadingLayer::removeLoading(this);
    
    for (int i = 0; i < 6; i++) {
        _roomNumTxt[i]->setString("");
    }
    
    nowRoomNumIndex = 0;
    
    if (code == 0) {
        JzDebug("用户坐下  桌位号为 : %d   座位号为 : %d " , data->uDeskIndex , data->cbDeskStation);
        RoomLogic()->addDeskOwnerInfo(data->uDeskIndex, data->dwDeskOwner);
        RoomLogic()->setRoomGameRoundInfo(data->dwGameRounds, data->dwPlayGameRound);
        RoomLogic()->setRoomZMCount(data->dwZaCodeNum);
        this->userSit( data->uDeskIndex, data->cbDeskStation);
    }else{
        
        CHAR buf[512];
        if (code == 1) {
            sprintf(buf, ("加入房间失败,房间不存在,请确认房间号..."));
        }else if (code == 2){
            sprintf(buf, ("加入房间失败,房间已满..."));
        }else{
            sprintf(buf, ("加入房间失败,请稍后再试..."));
        }
      
        YZLOG_ERROR("%s", GBKToUtf8(buf));
        GamePromptLayer::create()->showPrompt(buf);
    }
}


void GameDesk::userSit(INT destNo , BYTE seatNo){
   	MSG_GR_S_UserSit UserSit;
    UserSit.bDeskIndex = destNo;
    UserSit.bDeskStation = seatNo;
    YZRoomLogic::getInstance()->sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &UserSit, sizeof(UserSit));
}

#pragma mark - 购买房卡
//向服务器发送购买方法请求
void GameDesk::buyRoomCard(){
    INT userID = PlatformLogic()->loginResult.dwUserID;
    MSG_PROP_C_BUY_ROOMCARD data;
    memset(&data, 0, sizeof(data));
    data.dwUserID = userID;
    data.nPropID = userPorpInfos.at(_selectRoomCount).nPropID;
    data.nPropCount = _selectBuyRoomCardCount;
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_BUY, &data, sizeof(data));
    
    this->showBuyRoomCardLayer(false);
}

//购买房卡回调
void GameDesk::onBuyRoomCardMessage(MSG_PROP_S_BUY_ROOMCARD * data){
    
    userPorpInfos.at(data->attribValue).count = data->nHoldCount;
    
    char str[128];
    sprintf(str, GBKToUtf8("房卡 : %d"), data->nHoldCount);
    label_roomCount->setString(str);
    JzDebug("购买后 %d 道具数量为 : %d  花费金钱 : %lld " , data->attribValue ,userPorpInfos.at(data->attribValue).count ,data->dwCost );
}

#pragma mark - 玩家站起
void GameDesk::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
{
    INT userID = PlatformLogic()->loginResult.dwUserID;
    
    MSG_GR_BACKROOM_STATUS data;
    memset(&data, 0, sizeof(data));
    data.iUserID = userID;
    
    YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_OPERAT_STATUS, &data, sizeof(data));
}

// ∑øº‰¡¨Ω”≥…π¶
void GameDesk::onConnectMessage(bool result)
{
    
}

#pragma mark -------------- 标准加入房间模式 --------------

void GameDesk::gameNetworkCheckCallBack(float dt)
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(GameDesk::gameNetworkCheckCallBack), this);
    if (!PlatformLogic()->connected())
    {
        auto prompt = GamePromptLayer::create();
        prompt->showPrompt(GBKToUtf8("Õ¯¬Á∂œø™£¨«Î÷ÿ–¬µ«¬Ω°£"));
        prompt->setCallBack([]()
                            {
                                LoginLayer::returnLogin();
                            });
    }
}

void GameDesk::onEnter()
{
    YZRoomEventLayer::onEnter();
    
    CCLOG("GameDesk::onEnter...o09999999998");
    
    //查询服务器配置的房卡类型
    MSG_PROP_C_KIND_ROOMCARD propData;
    memset(&propData, 0, sizeof(propData));
    propData.nPropID = 0;
    YZRoomLogic::getInstance()->sendData(MDM_GR_PROP, ASS_PROP_ROOMCARD_TYPEINFO, &propData, sizeof(propData));
}


void GameDesk::selectedRoomCountStateEvent(cocos2d::Ref* pSender, CheckBoxEventType type)
{
    CheckBox * box = static_cast<CheckBox *>(pSender);
    
    if (type == CHECKBOX_STATE_EVENT_UNSELECTED) {
        box->setSelected(true);
        return;
    }
    
    bool isSelected = type == CHECKBOX_STATE_EVENT_SELECTED ? true : false;
    
    if (box->getTag() == TAG_CR_CB_1) {
        roomCountCBoxVec.at(0)->setSelected(isSelected);
        roomCountCBoxVec.at(1)->setSelected(!isSelected);
        _selectRoomCount = 10;
    }else{
        roomCountCBoxVec.at(0)->setSelected(!isSelected);
        roomCountCBoxVec.at(1)->setSelected(isSelected);
        _selectRoomCount = 20;
    }
}

void GameDesk::selectedRoomZMStateEvent(cocos2d::Ref* pSender, CheckBoxEventType type){
    CheckBox * box = static_cast<CheckBox *>(pSender);
    
    if (type == CHECKBOX_STATE_EVENT_UNSELECTED) {
        box->setSelected(true);
        return;
    }
    
    int tag = box->getTag();
    
    for (int i = TAG_ZM_CB_1;  i <= TAG_ZM_CB_3; i++) {
        if (tag == i) {
            roomZMCBoxVec.at(i - TAG_ZM_CB_1)->setSelected(true);
        }else{
            roomZMCBoxVec.at(i - TAG_ZM_CB_1)->setSelected(false);
        }
    }
    
    switch (tag) {
        case TAG_ZM_CB_1:
            _selectRoomZMCount = 2;
            break;
        case TAG_ZM_CB_2:
            _selectRoomZMCount = 4;
            break;
        case TAG_ZM_CB_3:
            _selectRoomZMCount = 6;
            break;
    }
    //_selectRoomZMCount = tag - TAG_ZM_CB_1 + 2;
}


void GameDesk::queryBackRoomInfoFromService(){
    INT userID = PlatformLogic()->loginResult.dwUserID;
    
    MSG_GR_BACKROOM_STATUS data;
    memset(&data, 0, sizeof(data));
    data.iUserID = userID;
    YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_OPERAT_STATUS, &data, sizeof(data));
}

void GameDesk::onExit()
{
    YZRoomEventLayer::onExit();
}


void GameDesk::createMenu()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    
    // ∑µªÿ∞¥≈•
    auto returnBtn = Button::create(RETURNBTN);
    returnBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::returnEventCallBack, this));
    returnBtn->setPosition(Vec2(winSize.width*0.05f, winSize.height*0.945f));
    addChild(returnBtn,5);
    
    // πÊ‘Ú∞¥≈•
    _ruleBtn = Button::create(RULEBTN);
    _ruleBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::ruleEventCallBack, this));
    _ruleBtn->setPosition(Vec2(winSize.width*0.88f, winSize.height*0.94f));
    addChild(_ruleBtn,5);
    
    
    // …Ë÷√∞¥≈•
    _setBtn = Button::create(SETBTN);
    _setBtn->addTouchEventListener(CC_CALLBACK_2(GameDesk::setEventCallBack, this));
    _setBtn->setPosition(Vec2(winSize.width*0.95f, winSize.height*0.947f));
    addChild(_setBtn,5);
    //_setBtn->setVisible(false);
    
}

// ¥¥Ω®≈∆◊¿¡–±Ì
void GameDesk::createDeskList()
{
    auto winSize = Director::getInstance()->getWinSize();
    
    _deskListBG = Sprite::create(LIST_BG);
    _deskListBG->setPosition(Vec2(winSize.width * 0.5f, winSize.height * 0.5f));
    float xScale = winSize.width / PlatformConfig::getInstance()->getPlatformDesignSize().width;
    float yScale = winSize.height / PlatformConfig::getInstance()->getPlatformDesignSize().height;
    _deskListBG->setScale(xScale, yScale);
    addChild(_deskListBG, 2);
    
    
    
    // øÏÀŸø™ º
    _quickStart = Button::create(QUICK_START);
    _quickStart->addTouchEventListener(CC_CALLBACK_2(GameDesk::quickStartCallBack, this));
    _quickStart->setPosition(Vec2(winSize.width * 0.75f, winSize.height * 0.95f));
    _quickStart->setScale(0.8);
    addChild(_quickStart,5);
    
    // ◊¿◊”¡–±Ì
    _deskItems = ListView::create();
    _deskItems->setItemsMargin(-1);		//¡–±Ìº‰œ∂
    _deskItems->setBounceEnabled(true);	//ªÿµØ
    _deskItems->setGravity(ListView::Gravity::CENTER_HORIZONTAL);//ª¨∂Ø∑ΩœÚ(…œœ¬)
    _deskItems->setContentSize(Size(1120, 535));//¡–±Ì∑∂Œß
    _deskItems->setAnchorPoint(Vec2(0.5f, 0.5f));
    _deskItems->setPosition(Vec2(winSize.width *0.5, winSize.height*0.5f-20));
    addChild(_deskItems,3);
    
    // ¡–±Ìº‡Ã˝
    _deskItems->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameDesk::selectedItemEventCALLBACK, this)));
}

// ÷ª˙∑µªÿº¸º‡Ã˝ªÿµ˜∫Ø ˝
void GameDesk::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
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




// ÃÌº”◊¿◊”
void GameDesk::addDesk(ComRoomInfo* romInfo)
{
    
}

// ∏¸–¬◊¿◊”¡–±Ì–≈œ¢
void GameDesk::updateDeskInfo(Widget* deskItem, int isLeft, bool isOwner, int ownerId, bool lockTag)
{
    //Layout* layout;
    Button* button;
    if (isLeft)
    {
        Layout*layout_left = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(deskItem, "Panel_left"));
        auto button1 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, "btn_deskNo1"));
        button1->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button1->setTag(0);
        auto button2 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, "btn_deskNo2"));
        button2->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button2->setTag(1);
        auto button3 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, "btn_deskNo3"));
        button3->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button3->setTag(2);
        auto button4 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, "btn_deskNo4"));
        button4->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button4->setTag(3);
        auto button5 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, "btn_deskNo5"));
        if (button5)
        {
            button5->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
            button5->setTag(4);
        }
        
        DeskInfo* desk = static_cast<DeskInfo*>(deskItem->getUserData());
        
        if (nullptr == desk) return;
        {
            /*if (!desk->isOwner[0]){
             desk->isOwner[0] = isOwner;
             desk->ownerID[0] = ownerId;
             }*/
            
            desk->isOwner[0] = isOwner;
            desk->ownerID[0] = ownerId;
            desk->lockTag[0] = lockTag;
            std::vector<UserInfoStruct*> deskUsers;
            UserInfoModule()->findDeskUsers(desk->deskID[0], deskUsers);
            desk->peopleValue[0] = deskUsers.size();
            if (desk->peopleValue[0]>0)
            {
                for (int i = 0; i < deskUsers.size(); i++)
                {
                    
                    UserInfoStruct* userInfor=deskUsers[i];
                    char str[60];
                    sprintf(str,"Panel_player%d",userInfor->bDeskStation+1);
                    auto layout = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(layout_left, str));
                    layout->setVisible(true);
                    sprintf(str,"btn_deskNo%d",userInfor->bDeskStation+1);
                    auto pBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_left, str));
                    pBtn->setVisible(false);
                    //ÕÊº“–≈œ¢Ω·ππÃÂ
                    UserInfoStruct* LogonResult = UserInfoModule()->findUser(userInfor->dwUserID);
                    auto pHead = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layout, "Image_player"));
                    sprintf(str, "%s%02d%s", "PersonalHead/local_head_",LogonResult->bLogoID,".png");
                    pHead->loadTexture(str);
                    auto pNick= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(layout, "Label_nick"));
                    pNick->setString(GBKToUtf8(LogonResult->nickName));
                    if (userInfor->dwUserID == PlatformLogic()->loginResult.dwUserID)
                    {
                        if ( desk->peopleMax[0]==deskUsers.size()||_iBR)
                        {
                            _iCanEnter=true;
                        }
                        else
                        {
                            _iCanEnter=false;
                        }
                    }
                }
            }
            
        }
    }
    else
    {
        Layout*layout_right = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(deskItem, "Panel_right"));
        auto button1 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, "btn_deskNo1"));
        button1->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button1->setTag(5);
        auto button2 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, "btn_deskNo2"));
        button2->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button2->setTag(6);
        auto button3 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, "btn_deskNo3"));
        button3->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button3->setTag(7);
        auto button4 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, "btn_deskNo4"));
        button4->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
        button4->setTag(8);
        auto button5 = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, "btn_deskNo5"));
        if (button5)
        {
            button5->addTouchEventListener(CC_CALLBACK_2(GameDesk::onBtnClicked, this));
            button5->setTag(9);
        }
        
        
        DeskInfo* desk = static_cast<DeskInfo*>(deskItem->getUserData());
        
        if (nullptr == desk) return;
        {
            /*if (!desk->isOwner[1]){
             desk->isOwner[1] = isOwner;
             desk->ownerID[1] = ownerId;
             }*/
            
            desk->isOwner[1] = isOwner;
            desk->ownerID[1] = ownerId;
            desk->lockTag[1] = lockTag;
            
            std::vector<UserInfoStruct*> deskUsers;
            UserInfoModule()->findDeskUsers(desk->deskID[1], deskUsers);
            desk->peopleValue[1] = deskUsers.size();
            if (desk->peopleValue[1]>0)
            {
                for (int i = 0; i < deskUsers.size(); i++)
                {
                    
                    UserInfoStruct* userInfor=deskUsers[i];
                    char str[60];
                    sprintf(str,"Panel_player%d",userInfor->bDeskStation+1);
                    auto layout = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(layout_right, str));
                    layout->setVisible(true);
                    sprintf(str,"btn_deskNo%d",userInfor->bDeskStation+1);
                    auto pBtn = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(layout_right, str));
                    pBtn->setVisible(false);
                    //ÕÊº“–≈œ¢Ω·ππÃÂ
                    UserInfoStruct* LogonResult = UserInfoModule()->findUser(userInfor->dwUserID);
                    auto pHead = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(layout, "Image_player"));
                    sprintf(str, "%s%02d%s", "PersonalHead/local_head_",LogonResult->bLogoID,".png");
                    pHead->loadTexture(str);
                    auto pNick= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(layout, "Label_nick"));
                    pNick->setString(GBKToUtf8(LogonResult->nickName));
                    if (userInfor->dwUserID == PlatformLogic()->loginResult.dwUserID)
                    {
                        if ( desk->peopleMax[0]==deskUsers.size()||_iBR)
                        {
                            _iCanEnter=true;
                        }
                        else
                        {
                            _iCanEnter=false;
                        }
                    }
                }
            }
            
        }
        
    }
    
}

//¡–±Ìµ„ª˜ªÿµ˜
void GameDesk::selectedItemEventCALLBACK(cocos2d::Ref *pSender, ListView::EventType type)
{
    switch (type)
    {
        case ListView::EventType::ON_SELECTED_ITEM_END:
        {
            /*YZAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
             ListView* deskItems = dynamic_cast<ListView*>(pSender);
             
             UINT selected = deskItems->getCurSelectedIndex();
             
             Widget* deskItem = deskItems->getItem(selected);
             Button* button = dynamic_cast<Button*>(deskItem->getChildByName("Button_9"));
             if (!_isSelect)
             {
             _selectIndex = selected;
             _saveColor = button->getColor();
             button->setColor(Color3B(180, 180, 180));
             _isSelect = true;
             return;
             }
             else if (_isSelect && _selectIndex != selected)
             {
             Widget* deskItem_ = deskItems->getItem(_selectIndex);
             Button* button_ = dynamic_cast<Button*>(deskItem_->getChildByName("Button_9"));
             button_->setColor(_saveColor);
             _selectIndex = selected;
             _saveColor = button->getColor();
             button->setColor(Color3B(180, 180, 180));
             return;
             }*/
            
            //do
            //{
            //	_currentSelectedDesk = static_cast<DeskInfo*>(deskItem->getUserData());
            
            //	// Ω¯»Î”Œœ∑◊ÓµÕœﬁ∂Ó
            //	if (PlatformLogic()->loginResult.i64Money < _currentSelectedDesk->goldMin)
            //	{
            //		char buffer[128];
            //		sprintf(buffer, "ƒ˙µƒΩ±“–°”⁄%u, ≤ªƒ‹Ω¯»Î”Œœ∑, «ÎµΩ…ÃµÍ≥‰÷µ°£", _currentSelectedDesk->goldMin);
            //		GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
            //		break;
            //	}
            
            //	// Ω¯»Î”Œœ∑◊Ó¥Ûœﬁ∂Ó
            //	if (0 != _currentSelectedDesk->goldMax)
            //	{
            //		if (PlatformLogic()->loginResult.i64Money > _currentSelectedDesk->goldMax)
            //		{
            //			char buffer[128];
            //			sprintf(buffer, "ƒ˙µƒΩ±“¥Û”⁄%u, ≤ªƒ‹Ω¯»Î”Œœ∑°£", _currentSelectedDesk->goldMax);
            //			GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
            //			break;
            //		}
            //	}
            
            //	// ≈∆◊¿¬˙»À
            //	if (_currentSelectedDesk->peopleValue == _currentSelectedDesk->peopleMax)
            //	{
            //		GamePromptLayer::create()->showPrompt(GBKToUtf8("◊¿◊”¬˙»À£¨«Î—°‘Ò∆‰À¸◊¿◊”°£"));
            //		break;
            //	}
            
            //	if (!_quicklyGame)
            //	{
            //		INT userID = PlatformLogic()->loginResult.dwUserID;
            //		UserInfoStruct* self = UserInfoModule()->findUser(userID);
            //		if(nullptr != self)
            //		{
            //			if(self->bDeskStation != INVALID_DESKSTATION)
            //			{
            //				sendUserUp();
            //			}
            //		}
            //		bool ret = GameCreator()->startGameClient(_roomInfo->uNameID, _currentSelectedDesk->deskID, false);
            //		if (!ret)
            //		{
            //			GamePromptLayer::create()->showPrompt(GBKToUtf8("”Œœ∑∆Ù∂Ø ß∞‹°£"));
            //		}
            //	}
            //	else
            //	{
            //		quicklyJoinGame(_currentSelectedDesk);
            //	}
            //} while (0);
        }
            break;
        default:
            break;
    }
}

//////////////////////////////////////////////////////////////////////////

void GameDesk::quickStartCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    
    YZAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    _quicklyGame = true;
    _iMySet=false;
    if (!quicklyJoinGame())
    {
        _quicklyGame = false;
    }
}

// Ã¯◊™µΩ∑øº‰¡–±Ì
void GameDesk::returnEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    leaveDesk();
}

//”Œœ∑πÊ‘Ú
void GameDesk::ruleEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    auto btn=(Button*)pSender;
    char str[60];
    sprintf(str,"platform/GameRule%d.json",btn->getTag()-2000);
    _uiRoomRule = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(str);
    _uiRoomRule->setTag(100001);
    _uiRoomRule->setAnchorPoint(Vec2(0.5f, 0.5f));
    _uiRoomRule->setPosition(Vec2(Win_w / 2, Win_h / 2));
    addChild(_uiRoomRule,1000);
    
    auto btn_close=(Button*)(Helper::seekWidgetByName(_uiRoomRule,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
    btn_close->setTag(GAME_RULEEXIT_TAG);
}

//”Œœ∑πÊ‘Ú
void GameDesk::setEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    
    
    uiSetRoomPwdLayer = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("platform/SetRoomPwd.json"));
    uiSetRoomPwdLayer->setTag(100002);
    uiSetRoomPwdLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
    uiSetRoomPwdLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
    addChild(uiSetRoomPwdLayer,1000);
    auto btn_close=(Button*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"btn_close"));
    btn_close->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
    btn_close->setTag(GAME_SETEXIT_TAG);
    _tfSetPwd=(TextField*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"tf_pwd"));
    auto btn_ok=(Button*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"btn_ok"));
    btn_ok->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
    btn_ok->setTag(GAME_SETOK_TAG);
    
    ImageView* checkRemeberPasdBg = (ImageView*)(Helper::seekWidgetByName(uiSetRoomPwdLayer, "Image_rem_bg"));
    checkRemeberPasdBg->setTouchEnabled(true);
    checkRemeberPasdBg->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
    checkRemeberPasdBg->setTag(IS_RM_DESK_PWD_TAG);
    
    ImageView* checkRemeberPasd = (ImageView*)(Helper::seekWidgetByName(uiSetRoomPwdLayer, "Image_rem_select"));
    
    bool rem = LoadBooleanFromXML(IS_RM_DESK_PWD);
    if (rem)
    {
        checkRemeberPasd->setVisible(true);
    }
    
    std::string pwd = LoadStringFromXML(DESK_PASSWD);
    if (pwd.length() > 0)
    {
        _tfSetPwd->setText(pwd.c_str());
    }
    
}

void GameDesk::exitEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    if (Widget::TouchEventType::ENDED != type) return;
    
    auto btn=(Button*)pSender;
    switch (btn->getTag())
    {
        case GAME_RULEEXIT_TAG:
        {
            removeChildByTag(100001);
        }
            break;
        case GAME_SETEXIT_TAG:
        {
            removeChildByTag(100002);
        }
            break;
        case IS_RM_DESK_PWD_TAG:
        {
            ImageView* checkRemeberPasd = (ImageView*)(Helper::seekWidgetByName(uiSetRoomPwdLayer, "Image_rem_select"));
            if (checkRemeberPasd->isVisible())
            {
                SaveBooleanToXML(IS_RM_DESK_PWD, false);
                checkRemeberPasd->setVisible(false);
                
            }
            else
            {
                SaveBooleanToXML(IS_RM_DESK_PWD, true);
                checkRemeberPasd->setVisible(true);
            }
        }
            break;
        case GAME_SETOK_TAG:
        {
            std::string str_pwd = _tfSetPwd->getStringValue();
            if (str_pwd.length()==0)
            {
                MSG_GR_S_UserSet UserSet;
                UserSet.m_Rule.bCutPercent=0;
                UserSet.m_Rule.bIPLimite=0;
                UserSet.m_Rule.bLimitCut=0;
                UserSet.m_Rule.bLimitPoint=0;
                UserSet.m_Rule.bSameIP=0;
                UserSet.m_Rule.dwHighPoint=0;
                UserSet.m_Rule.dwLowPoint=0;
                UserSet.m_Rule.bPass=0;
                strcpy(UserSet.m_Rule.szPass, "");
                //PlatformLogic()->sendData(MDM_GR_ROOM, ASS_GR_ROOM_SET,&UserSet, sizeof(UserSet));
                YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_ROOM_SET, &UserSet, sizeof(UserSet));
                //removeChildByTag(100002);
                // ∏¸–¬◊¿◊”–≈œ¢
                Widget* item = dynamic_cast<Widget*>(_deskItems->getItem(_myDeskNo));
                if (nullptr != item)
                {
                    auto left = 1;
                    if (_myDeskNo%2 != 0)
                    {
                        left = 0;
                    }
                    
                    updateDeskInfo(item,left,false,-1,false);
                }
                
                //Õ¨ ±…æ≥˝remember
                SaveBooleanToXML(IS_RM_DESK_PWD, false);
                SaveStringToXML(DESK_PASSWD, "");
                
                
                removeChildByTag(100002);
                break;
            }
            if (str_pwd.length()<6)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("√‹¬Î≥§∂»÷¡…ŸŒ™6Œª°£")); break;
            }
            
            auto userMD5Pwd=MD5_CTX::MD5String(str_pwd);
            MSG_GR_S_UserSet UserSet;
            //UserSet.user=0;
            UserSet.m_Rule.bCutPercent=0;
            UserSet.m_Rule.bIPLimite=0;
            UserSet.m_Rule.bLimitCut=0;
            UserSet.m_Rule.bLimitPoint=0;
            UserSet.m_Rule.bSameIP=0;
            UserSet.m_Rule.dwHighPoint=0;
            UserSet.m_Rule.dwLowPoint=0;
            
            
            ImageView* checkRemeberPasd = (ImageView*)(Helper::seekWidgetByName(uiSetRoomPwdLayer, "Image_rem_select"));
            if (checkRemeberPasd->isVisible()){
                UserSet.m_Rule.bPass = 1;
                strcpy(UserSet.m_Rule.szPass, userMD5Pwd.c_str());
                
                SaveBooleanToXML(IS_RM_DESK_PWD, true);
            }
            else{
                UserSet.m_Rule.bPass = 0;
                strcpy(UserSet.m_Rule.szPass, "");
                
                SaveBooleanToXML(IS_RM_DESK_PWD, false);
            }
            SaveStringToXML(DESK_PASSWD, str_pwd);
            
            //PlatformLogic()->sendData(MDM_GR_ROOM, ASS_GR_ROOM_SET,&UserSet, sizeof(UserSet));
            YZRoomLogic::getInstance()->sendData(MDM_GR_ROOM, ASS_GR_ROOM_SET, &UserSet, sizeof(UserSet));
            
            Widget* item = dynamic_cast<Widget*>(_deskItems->getItem(_myDeskNo));
            if (nullptr != item)
            {
                auto left = 1;
                if (_myDeskNo%2 != 0)
                {
                    left = 0;
                }
                
                updateDeskInfo(item,left,true,PlatformLogic()->loginResult.dwUserID,true);
                //updateDeskInfo(item, left, true, -1);
                
            }
            //PlatformLogic()->addEventSelector(MDM_GR_ROOM, ASS_GR_ROOM_SET, YZ_SOCKET_CALLBACK(GameDesk::setRoomPwdEventBack, this));
            
            removeChildByTag(100002);
            
        }
            break;
        case GAME_INPUTOK_TAG:
        {
            std::string str_pwd = _tfInputPwd->getStringValue();
            if (str_pwd.length()==0)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("√‹¬Î≤ªƒ‹Œ™ø’°£")); break;
            }
            if (str_pwd.length()<6)
            {
                GamePromptLayer::create()->showPrompt(GBKToUtf8("√‹¬Î≥§∂»÷¡…ŸŒ™6Œª°£")); break;
            }
            MSG_GR_S_UserSit UserSit;
            UserSit.bDeskIndex = _currentSelectDeskNo;
            UserSit.bDeskStation = _seatNo;
            
            auto userMD5Pwd = MD5_CTX::MD5String(str_pwd);
            strcpy(UserSit.szPassword, userMD5Pwd.c_str());
            
            sendUserSit(&UserSit, sizeof(UserSit));
            removeChildByTag(100002);
        }
            break;
        default:
            break;
    }
}

// π∫¬Ú≥Ô¬Î
void GameDesk::rechargeEventCallBack(Ref* pSender, Widget::TouchEventType type)
{
    
    if (Widget::TouchEventType::ENDED != type) return;
  
}



void GameDesk::leaveDesk()
{
    YZAudioEngine::getInstance()->playEffect(GAME_SOUND_CLOSE);
    RoomLogic()->closeRoom();
    HallLayer::returnHallLayer();
}


//////////////////////////////////////////////////////////////////////////
void GameDesk::clearDesksData()
{
    for (auto iter = _deskinfos.begin(); iter != _deskinfos.end(); ++iter)
    {
        DeskInfo* desk = *iter;
        YZ_SAFE_DELETE(desk);
    }
}

bool GameDesk::quicklyJoinGame()
{
    YZAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
    Vector<Widget*> items = _deskItems->getItems();
    for (auto deskItem = items.begin(); deskItem != items.end(); ++deskItem)
    {
        _currentSelectedDesk = static_cast<DeskInfo*>((*deskItem)->getUserData());
        if (quicklyJoinGame(_currentSelectedDesk,1))
        {
            return true;
        }
        else if (quicklyJoinGame(_currentSelectedDesk,0))
        {
            return true;
        }
    }
    return false;
}

bool GameDesk::quicklyJoinGame(DeskInfo* desk,int isleft)
{
    auto _elementId = (isleft+1)%2;
    // pragma checked
    CCAssert(nullptr != desk, "desk is nullptr");
    if (nullptr == desk) return false;
    
    std::vector<UserInfoStruct*> deskUsers;
    UserInfoModule()->findDeskUsers(desk->deskID[_elementId], deskUsers);
    
    BYTE newDeskStation = INVALID_DESKSTATION;
    BYTE userDeskStation[255];
    memset(userDeskStation, INVALID_DESKSTATION, sizeof(userDeskStation));
    bool ret = false;
    do
    {
        // ≤È’“◊¯œ¬µƒ”√ªß◊˘Œª∫≈
        for (auto user : deskUsers)
        {
            log("user->bUserState = %d", user->bUserState);
            if (USER_SITTING == user->bUserState					// ◊¯œ¬
                || USER_PLAY_GAME == user->bUserState		// ”Œœ∑÷–
                || USER_CUT_GAME == user->bUserState			// ∂œœﬂ
                || USER_ARGEE == user->bUserState)			// ∂œœﬂ
            {
                userDeskStation[user->bDeskStation] = user->bDeskStation;
            }
        }
        
        // ≤È’“ø’◊˘Œª
        if (_iMySet)
        {
            newDeskStation=_seatNo;
        }
        else
        {
            for (INT i = 0; i < YZ_ARRAY_COUNT(userDeskStation); ++i)
            {
                if (INVALID_DESKSTATION == userDeskStation[i])
                {
                    newDeskStation = i;
                    break;
                }
            }
        }
        
        
        // ≤È’“◊¿Œª ß∞‹
        if (INVALID_DESKSTATION == newDeskStation) break;
        
        //if (newDeskStation >= desk->peopleMax[_elementId]) break;
        
        /*MSG_GR_S_UserSit UserSit;
         UserSit.bDeskIndex = desk->deskID[_elementId];
         UserSit.bDeskStation = newDeskStation;
         strcpy(UserSit.szPassword, "");
         sendUserSit( &UserSit, sizeof(UserSit))*/;
        
        _currentSelectDeskNo=desk->deskID[_elementId];
        MSG_GP_R_LogonResult aa = PlatformLogic()->loginResult;
        
        INT leftTag = isleft ? 0 : 1;
        if ((!desk->lockTag[leftTag]) || (desk->ownerID[leftTag] == PlatformLogic()->loginResult.dwUserID) && (desk->isOwner[leftTag]))
            //if ( (!desk->lockTag[leftTag]))
        {
            MSG_GR_S_UserSit UserSit;
            UserSit.bDeskIndex = desk->deskID[_elementId];
            UserSit.bDeskStation = newDeskStation;
            sendUserSit(&UserSit, sizeof(UserSit));
            ret = true;
            
        }
        else
        {
            auto uiSetRoomPwdLayer = dynamic_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("platform/OpenLock.json"));
            uiSetRoomPwdLayer->setTag(100002);
            uiSetRoomPwdLayer->setAnchorPoint(Vec2(0.5f, 0.5f));
            uiSetRoomPwdLayer->setPosition(Vec2(Win_w / 2, Win_h / 2));
            addChild(uiSetRoomPwdLayer,1000);
            auto btn_close=(Button*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"btn_close"));
            btn_close->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
            btn_close->setTag(GAME_SETEXIT_TAG);
            _tfInputPwd=(TextField*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"tf_pwd"));
            auto btn_ok=(Button*)(Helper::seekWidgetByName(uiSetRoomPwdLayer,"btn_ok"));
            btn_ok->addTouchEventListener(CC_CALLBACK_2(GameDesk::exitEventCallBack, this));
            btn_ok->setTag(GAME_INPUTOK_TAG);
            return false;
        }
        
    } while (0);
    
    return ret;
}

//////////////////////////////////////////////////////////////////////////

void GameDesk::walletChanged(LLONG money)
{
    if (nullptr != _textSpriteUserChips)
    {
        char buffer[32];
        sprintf(buffer, "%lld", money);
        _textSpriteUserChips->setText(gFontConfig_18, buffer, Color4B::YELLOW);
    }
}

//////////////////////////////////////////////////////////////////////////



void GameDesk::onDisConnect()
{
    YZLOG_ERROR("network disconnect!");
    auto layer = SocketErrorLayer::createLayer();
    addChild(layer, 1000);
}


void GameDesk::onBtnClicked(Ref* pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    
    if (type == Widget::TouchEventType::ENDED && btn)
    {
        
        
        YZAudioEngine::getInstance()->playEffect(GAME_SOUND_BUTTON);
        ListView* deskItems = dynamic_cast<ListView*>(btn->getParent()->getParent()->getParent()->getParent());
        
        _iMySet=true;
        UINT selected = deskItems->getCurSelectedIndex();
        /*auto selected_save = selected;
         auto btnleft = ( btn->getTag() - 1001 );
         selected = selected * 2 + btnleft;*/
        
        Widget* deskItem = deskItems->getItem(selected);
        
        if (!_isSelect)
        {
            _selectIndex = selected;
            _saveColor = btn->getColor();
            //btn->setColor(Color3B(180, 180, 180));
            _isSelect = true;
            return;
        }
        else if (_isSelect && _selectIndex != selected)
        {
            Widget* deskItem_ = deskItems->getItem(selected);
            btn->setColor(_saveColor);
            _selectIndex = selected;
            _saveColor = btn->getColor();
            //btn->setColor(Color3B(180, 180, 180));
            return;
        }
        
        log("tag = %d", btn->getTag());
        
        
        if (btn->getTag()<5)
        {
            log("left  left ");
            do 
            {
                _currentSelectedDesk = static_cast<DeskInfo*>(deskItem->getUserData());
                _seatNo=btn->getTag();
                
                // Ω¯»Î”Œœ∑◊ÓµÕœﬁ∂Ó
                if (PlatformLogic()->loginResult.i64Money <= _currentSelectedDesk->goldMin[0])
                {
                    char buffer[128];
                    sprintf(buffer, "ƒ˙µƒΩ±“–°”⁄%u, ≤ªƒ‹Ω¯»Î”Œœ∑, «ÎµΩ…ÃµÍ≥‰÷µ°£", _currentSelectedDesk->goldMin[0]);
                    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    break;
                }
                
                // Ω¯»Î”Œœ∑◊Ó¥Ûœﬁ∂Ó
                if (0 != _currentSelectedDesk->goldMax[0])
                {
                    if (PlatformLogic()->loginResult.i64Money > _currentSelectedDesk->goldMax[0])
                    {
                        char buffer[128];
                        sprintf(buffer, "ƒ˙µƒΩ±“¥Û”⁄%u, ≤ªƒ‹Ω¯»Î”Œœ∑°£", _currentSelectedDesk->goldMax[0]);
                        GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                        break;
                    }
                }
                
                // ≈∆◊¿¬˙»À
                if (_currentSelectedDesk->peopleValue[0] == _currentSelectedDesk->peopleMax[0])
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("◊¿◊”¬˙»À£¨«Î—°‘Ò∆‰À¸◊¿◊”°£"));
                    break;
                }
                
                if (!_quicklyGame)
                {
                    INT userID = PlatformLogic()->loginResult.dwUserID;
                    UserInfoStruct* self = UserInfoModule()->findUser(userID);
                    if(nullptr != self)
                    {
                        if(self->bDeskStation != INVALID_DESKSTATION)
                        {
                            sendUserUp();
                        }
                    }
                    bool ret = GameCreator()->startGameClient(_roomInfo->uNameID, _currentSelectedDesk->deskID[0], false);
                    if (!ret)
                    {
                        GamePromptLayer::create()->showPrompt(GBKToUtf8("”Œœ∑∆Ù∂Ø ß∞‹°£"));
                    }
                }
                else
                {
                    quicklyJoinGame(_currentSelectedDesk,1);
                }
            } while (0);
        }
        else
        {
            do 
            {
                _currentSelectedDesk = static_cast<DeskInfo*>(deskItem->getUserData());
                _seatNo=btn->getTag()-5;
                
                // 进入游戏最低限额
                if (PlatformLogic()->loginResult.i64Money <= _currentSelectedDesk->goldMin[1])
                {
                    char buffer[128];
                    sprintf(buffer, "您的金币小于%u, 不能进入游戏, 请到商店充值。", _currentSelectedDesk->goldMin[1]);
                    GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                    break;
                }
                
                // 进入游戏最大限额
                if (0 != _currentSelectedDesk->goldMax[1])
                {
                    if (PlatformLogic()->loginResult.i64Money > _currentSelectedDesk->goldMax[1])
                    {
                        char buffer[128];
                        sprintf(buffer, "您的金币大于%u, 不能进入游戏。", _currentSelectedDesk->goldMax[1]);
                        GamePromptLayer::create()->showPrompt(GBKToUtf8(buffer));
                        break;
                    }
                }
                
                // 牌桌满人
                if (_currentSelectedDesk->peopleValue[1] == _currentSelectedDesk->peopleMax[1])
                {
                    GamePromptLayer::create()->showPrompt(GBKToUtf8("桌子满人，请选择其它桌子。"));
                    break;
                }
                
                
                if (!_quicklyGame)
                {
                    INT userID = PlatformLogic()->loginResult.dwUserID;
                    UserInfoStruct* self = UserInfoModule()->findUser(userID);
                    if(nullptr != self)
                    {
                        if(self->bDeskStation != INVALID_DESKSTATION)
                        {
                            sendUserUp();
                        }
                    }
                    bool ret = GameCreator()->startGameClient(_roomInfo->uNameID, _currentSelectedDesk->deskID[1], false);
                    if (!ret)
                    {
                        GamePromptLayer::create()->showPrompt(GBKToUtf8("”Œœ∑∆Ù∂Ø ß∞‹°£"));
                    }
                }
                else
                {
                    quicklyJoinGame(_currentSelectedDesk,0);
                }
            } while (0);
        }
        
    }
#pragma mark -------------- 标准加入房间模式 --------------
    
    
}
