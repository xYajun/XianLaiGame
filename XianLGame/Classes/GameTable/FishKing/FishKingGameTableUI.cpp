#include "FishKingGameTableUI.h"
#include "FishKingGameLoading.h"
#include "FishKingGameVortexLayer.h"
#include "FishKingGameSceneFishTrace.h"
#include "FishKingGameFishManager.h"
#include "FishKingGameToolbar.h"
#include "FishKingGameAudio.h"
#include "FishKingGameAdmin.h"
#include <time.h>
#include "YZUIExport.h"
#include "GamePrompt.h"
#include "HallLayer.h"
#include "GameDesk.h"
#include "FontConfig.h"

namespace FishKing
{
	static const int	outTime_tag				=   999;
	static const char*	BG_PATH					=	"fishKing/Scene/bg/";
	static const char*	TABLEUI_PATH			=	"fishKing/GameTableUi/Node_Table.json";
	static const char*  Notice_Bg				=	"fishKing/notic/control.png";
	static const char*  Notice_Bg_Stencil		=	"fishKing/notic/Notice_Bg.png";
	static const char*  Score_number_path		=	"fishKing/number/score_num1.png";
	static const char*  Settlemen_path			=	"fishKing/GameTableUi/SettlementUi.json";

	static const FLOAT	SWITCH_SCENE_TIME		=	5.5f;
	#define Offset_Width      10

	GameTableUI::GameTableUI()
		: _isAllowFire (true)
		, _isAutoFire (false)
		, _isReadyLock (false)
		, _iBullet_id (0)
		, _lockFishId (0)
		, _touchPoint (Size::ZERO)
		, _currentBg (nullptr)
		, _tableUi (nullptr)
	{
		memset(_players, 0x0, sizeof(_players));
		_resultInfo = new catchResultInfo();

		//捕鱼游戏为了防止手机切后台后回来时出现一大堆鱼，在这里加个控制字段
		//YZSocketThread里面读取为true时跳过MDM_GM_GAME_NOTIFY消息
		auto userDefault = UserDefault::getInstance();
		userDefault->setBoolForKey("bControl", true);
		userDefault->flush();
	}

	GameTableUI::~GameTableUI()
	{
		YZ_SAFE_DELETE(_tableLogic);
		YZ_SAFE_DELETE(_fishManager);
		YZ_SAFE_DELETE(_resultInfo);
		YZ_SAFE_DELETE(_fishTrace);

		//捕鱼游戏为了防止手机切后台后回来时出现一大堆鱼，在这里加个控制字段
		//YZSocketThread里面读取为true时跳过MDM_GM_GAME_NOTIFY消息
		//退出捕鱼游戏时设置回来，防止其他游戏也被跳过MDM_GM_GAME_NOTIFY消息
		auto userDefault = UserDefault::getInstance();
		userDefault->setBoolForKey("bControl", false);
		userDefault->flush();

		//AnimationCache::getInstance()->destroyInstance();
		//ArmatureDataManager::getInstance()->removeArmatureFileInfo("fishKing/fishAnimation/bigFishAnimation.ExportJson");		
		
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("fishKing/bullet/netImg.plist");
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("fishKing/fishAnimation/fishImg.plist");
		//SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("fishKing/Scene/water.plist");

		//SpriteFrameCache::getInstance()->removeSpriteFrames();
		//Director::getInstance()->getTextureCache()->removeAllTextures();
		//Director::getInstance()->getTextureCache()->removeUnusedTextures();
		
	}

	void GameTableUI::onEnter()
	{
		YZGameUIBase::onEnter();
	}

	void GameTableUI::onExit()
	{
		unscheduleAllCallbacks();
		YZAudioEngine::getInstance()->stopBackgroundMusic();
		stopAllActions();
		YZGameUIBase::onExit();
	}
	void GameTableUI::onEnterTransitionDidFinish()
	{
		YZGameUIBase::onEnterTransitionDidFinish();

		//releaseUnusedRes();
	}

	GameTableUI* GameTableUI::create(BYTE bDeskIndex, bool autoCreate)
	{
		GameTableUI *tableUI = new GameTableUI();
		if (tableUI && tableUI->init(bDeskIndex, autoCreate))
		{ 
			tableUI->autorelease();
			return tableUI;
		} 
		CC_SAFE_DELETE(tableUI);
		return nullptr;
	}

	bool GameTableUI::init(BYTE bDeskIndex, bool autoCreate)
	{
		if (!YZLayer::init()) return false;

		_deskIndex		= bDeskIndex;
		_bAutoCreate	= autoCreate;

		//异步加载资源
		auto loadingLayer = FishKingGameLoading::create(true);
		addChild(loadingLayer);
		loadingLayer->onCloseCallBack = [&](){
			/*auto vortexLayer = VortexLayer::create();
			vortexLayer->setTargetLayer(FishKingGameLoading::create(false));
			addChild(vortexLayer);
			vortexLayer->onCloseCallBack = [this](){
			initGameUI();
			};*/
			initGameUI();
		};
		loadingLayer->loadRes();
		return true;
	}

	bool GameTableUI::initGameUI()
	{
		//监听物理按键
		//enableKeyboard();
		auto winSize = Director::getInstance()->getWinSize();

		Texture2D::setDefaultAlphaPixelFormat(Texture2D::PixelFormat::RGBA8888);

		FishKingGameAudio::playBackgroundMusic();

		
		//背景
		std::string bgName(BG_PATH);
		bgName.append("bg1.png");
		_currentBg = ImageView::create(bgName);
		float scaleX = winSize.width / 1136;
		float scaleY = winSize.height / 640;
		_currentBg->setPosition(winSize / 2);
		_currentBg->setScale(scaleX, scaleY);
		addChild(_currentBg);

		_tableUi = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/GameTableUi/table/Note_Table.json"));
	    addChild(_tableUi,PLAYER_ZORDER);
		//_tableUi->setScale(scaleX, scaleY);
	    _tableUi->setAnchorPoint(Vec2(0.5f, 0.5f));
	    _tableUi->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

		auto touchImage=ImageView::create("fishKing/GameTableUi/alpha.png");
		touchImage->setPosition(winSize / 2);
		_tableUi->addChild(touchImage,4);
		//打开点击
		auto disptch = Director::getInstance()->getEventDispatcher();
		auto myListener = EventListenerTouchOneByOne::create();
		myListener->onTouchBegan = CC_CALLBACK_2(GameTableUI::onTouchBegan,this);
		myListener->onTouchMoved = CC_CALLBACK_2(GameTableUI::onTouchMoved,this);
		myListener->onTouchEnded = CC_CALLBACK_2(GameTableUI::onTouchEnded,this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(myListener, touchImage);
	 
		//广播通知
		/*_NoticeBG = Sprite::create(Notice_Bg);
		_NoticeBG->setPosition(Vec2(winSize.width / 2, winSize.height * 0.80f));
		_NoticeBG->setCascadeOpacityEnabled(true);
		this->addChild(_NoticeBG, NOTICE_ZORDER);
		_NoticeBG->setVisible(false);*/

		/*_notic = GameNotice::createGameNotice(Notice_Bg_Stencil);
		_notic->setPosition(Vec2(_NoticeBG->getContentSize() / 2)); 
		_notic->setCallBack([this](){
			_NoticeBG->setVisible(false);
		});
		_NoticeBG->addChild(_notic);*/
	/*	_text = Label::createWithTTF(gFontConfig_22, "");
		_text->setAnchorPoint(Vec2(0.5f, 0.5f));
		_text->setPositionY(_NoticeBG->getContentSize().height / 2);

		if(_text->getContentSize().width > _NoticeBG->getContentSize().width)
		{
			_text->setPositionX(_text->getContentSize().width / 2 + Offset_Width);
		}
		else
		{
			_text->setPositionX(_NoticeBG->getContentSize().width / 2);
		}

		auto stencil = YZSprite::create(Notice_Bg_Stencil);
		stencil->setAnchorPoint(Vec2(0.5f, 0.5f));
		stencil->setPosition(_NoticeBG->getContentSize().width / 2, _NoticeBG->getContentSize().height / 2);
		ClippingNode* clipNode = ClippingNode::create();
		clipNode->setStencil(stencil);
		clipNode->setAnchorPoint(Vec2::ZERO);
		clipNode->setPosition(Vec2::ZERO);
		this->addChild(clipNode);

		clipNode->addChild(_text);
		showNoticeMessage("欢迎畅游梦幻海洋世界！！");*/

		//游戏帮助
		auto btnHelp = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableUi,"Button_Help"));
		btnHelp->addClickEventListener([=](Ref*){
			//停止超时退出倒计时
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);

			auto helpLayer = dynamic_cast<Layout*>(ui::Helper::seekWidgetByName(_tableUi,"Panel_Help"));
			helpLayer->setLocalZOrder(1);
			helpLayer->setOpacity(255);
			helpLayer->setVisible(true);
			auto btnClose = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableUi,"Button_Close"));
			btnClose->addClickEventListener([=](Ref*){
				helpLayer->runAction(Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr));
				//开始下次倒计时
				auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
					gameOutTime(true);
				}), nullptr);

				seq->setTag(outTime_tag);
				this->runAction(seq);
			});
		});

		//超端控制
		auto btnAdmin = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableUi,"Button_Admin"));
		btnAdmin->setVisible(false);
		btnAdmin->addClickEventListener([=](Ref*){
			//停止超时退出倒计时
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);

			GameAdmin* adminLayer = GameAdmin::create(_tableLogic);
			adminLayer->setPosition(winSize / 2);
			adminLayer->setName("adminLayer");
			addChild(adminLayer, MAX_ZORDER);

			adminLayer->onCloseCallBack = [this](){
				//开始下次倒计时
				auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
					gameOutTime(true);
				}), nullptr);

				seq->setTag(outTime_tag);
				this->runAction(seq);
			};
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_UPDATE_SUPER_INFO_SIG);
		});

		//适配
		Vector<Node*> children = _tableUi->getChildren();
		float scX = winSize.width/1136;
		float scY = winSize.height/640;

		for (auto child : children)
		{
			//if (winSize.width / winSize.height <= 1.7f)
			//{
				child->setScaleY(scY);
				child->setScaleX(scX);
			//}
		}

		//初始化鱼管理类
		_fishManager = new GameFishManager(this);

		//锁定成功回调
		_fishManager->onLockFishCallBack = [this](INT fishId){
			_isReadyLock = false;
			_lockFishId = fishId;
			//设置炮台锁定鱼
			_players[_tableLogic->_MYSEATNO]->setIsLockFish(true, _lockFishId);

			//去除锁定提示
			auto lockTip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi,"Image_LockTip"));
			lockTip->setVisible(false);
		};

		//解锁成功回调
		_fishManager->unLockFishCallBack = [this](int fishId){
			//如果解锁的鱼是自己锁定的鱼，则自己开炮不再锁定
			if (_lockFishId == fishId)
			{
				_lockFishId = INVALID_FISHID;				
			}

			for (int i = 0; i < PLAY_COUNT; i++)
			{
				_players[i]->setIsLockFish(false, fishId);
			}
		};

		_fishTrace	= new SceneFishTrace();

		//初始化玩家炮台
		for (int i = 0; i < PLAY_COUNT; i++)
		{
			auto player = GamePlayer::createGamePlayer(i, _tableUi, _fishManager);
			player->setBatteryVisible(false);
			_players[i] = player;
			addChild(player);
		}

		auto newThread = new std::thread(&GameTableUI::asyncBuildData, this);
		newThread->detach();

		scheduleOnce(schedule_selector(GameTableUI::delayedLoadAction), 1.0f);
		//初始化工具栏
		initToolBarData();

		_tableLogic = new GameTableLogic(this, _deskIndex, _bAutoCreate);
		_tableLogic->enterGame();

		return true;
	}

	void GameTableUI::asyncBuildData()
	{
		auto winSize = Director::getInstance()->getWinSize();

		//生成鱼潮路径坐标
		_fishTrace->BuildSceneKind1Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind2Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind3Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind4Trace(winSize.width, winSize.height);
		_fishTrace->BuildSceneKind5Trace(winSize.width, winSize.height);
	}

	void GameTableUI::delayedLoadAction(float dt)
	{
		auto winSize = Director::getInstance()->getWinSize();

		//水光动画
		char name[24];
		auto waterAnimation = Animation::create();
		waterAnimation->setDelayPerUnit(0.1f);
		waterAnimation->setLoops(-1);
		for(int i = 1; i <= 16; i++)
		{
			sprintf(name, "water%d.png", i);
			waterAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(name));
		}
		auto  animate = Animate::create(waterAnimation);
		auto water = Sprite::createWithSpriteFrameName(name);
		float X = winSize.width / water->getContentSize().width;
		float Y = winSize.height / water->getContentSize().height;
		water->setScale(X, Y);
		water->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
		water->setOpacity(0);
		water->runAction(Sequence::create(FadeIn::create(0.5f), animate, nullptr));
		addChild(water, WATER_ZORDER);

		//气泡粒子效果
		ParticleSystem *meteor = ParticleSystemQuad::create("fishKing/particle/qipao.plist");
		meteor->setPosition(winSize / 2);
		meteor->setAutoRemoveOnFinish(true);
		addChild(meteor, 1);
	}

	void GameTableUI::initToolBarData()
	{
		//auto toolBarBG = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi, "Image_ToolBarBG"));
		auto toolBar = FishKingToolbar::create(_tableUi);
		addChild(toolBar);

		toolBar->_onExit = [this](){
			gameSettlementAndExit();
		};

		toolBar->_onAutoFire = [this](){
			if (!_isAutoFire)
			{
				_isAutoFire = true;
				schedule(schedule_selector(GameTableUI::setAtuoFire), 0.2f);
			}
		};

		/*toolBar->_onSettingCannon = [this](const int cannonNum){
			auto me = (GamePlayer*)_players[_tableLogic->_MYSEATNO];

			float multiple = (me->getMaxMultiple() - me->getMinMultiple()) / 100.0f;
			int firePower = cannonNum * multiple + me->getMinMultiple();

			if (cannonNum == 100)
			{
				firePower = me->getMaxMultiple();
			}

			me->setCannonNum(firePower);
		};*/

		toolBar->_onLockFish = [this](){
			_isReadyLock = true;
			auto lockTip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi,"Image_LockTip"));
			lockTip->setVisible(true);
		};

		toolBar->_unLockFish = [this](){
			_isReadyLock = false;
			_players[_tableLogic->_MYSEATNO]->setIsLockFish(false, _lockFishId);
			_lockFishId = INVALID_FISHID;
			auto lockTip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi,"Image_LockTip"));
			lockTip->setVisible(false);
			
		};

		toolBar->_onLockRedFish = [this](){
			_fishManager->setLockFish(Vec2::ZERO, REDFISH);
		};

		toolBar->_onLockBigFish = [this](){
			_fishManager->setLockFish(Vec2::ZERO, BIGFISH);
		};
	}

	void GameTableUI::addUser(BYTE seatNo, bool bMe)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setBatteryVisible(true);
		_players[seatNo]->setIsMe(bMe);
		_players[seatNo]->setUserID(_tableLogic->getUserId(seatNo));
        _players[seatNo]->setCannonAddOrSubVisible(_tableLogic->getUserId(seatNo),true);
		_players[seatNo]->setCannonType(GamePlayer::DoubleTube);
		_players[seatNo]->setCannonNum(10);
		_players[seatNo]->setGameTable(this);
	}

	void GameTableUI::removeUser(BYTE seatNo, bool bMe)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setBatteryVisible(false);
		_players[seatNo]->setIsEnergyCannon(false);
		_players[seatNo]->setIsLockFish(false, INVALID_FISHID);
		_players[seatNo]->setCannonNum(0);
		_players[seatNo]->setUserMoney(0);
        _players[seatNo]->setCannonAddOrSubVisible(_tableLogic->getUserId(seatNo),false);
	}

	void GameTableUI::dealSetUserMoney(BYTE seatNo, LLONG value)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setUserMoney(value);
	}

	void GameTableUI::dealSetUserCannonNum(BYTE seatNo, INT value,INT valueCell)
	{
		if (!isValidSeat(seatNo))	return;
		_players[seatNo]->setCannonNum(value);
		_players[seatNo]->setCannonNumCell(valueCell);
	}

	void GameTableUI::dealSetGameConfig(CMD_S_GameConfig* gConfig)
	{
		_fishManager->setConfig(gConfig);
		
		for (BYTE i = 0; i < PLAY_COUNT; i++)
		{
			_players[i]->setConfig(gConfig);
		}
	}

	//进游戏时游戏的背景图，为了跟pc同步
	void GameTableUI::dealSetSceneStyle(SceneStyle Scene)
	{
		char name[12];
		std::string bgName(BG_PATH);
		sprintf(name, "bg%d.png", Scene + 1);
		bgName.append(name);
		_currentBg->loadTexture(bgName);
		
		if (!_isAutoFire && _isAllowFire)
		{
			//开始超时退出倒计时
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		}
	}

	void GameTableUI::dealLockFishTimeOut()
	{
		_fishManager->setAllFishIsPause(false);
	}

	void GameTableUI::dealBulletTimeOut(BYTE seatNo)
	{
		_players[seatNo]->setIsEnergyCannon(false);
	}

	//开炮
	void GameTableUI::dealGameFire(CMD_S_UserFire* pFire, bool bMeFire)
	{
		BYTE seatNo = pFire->chair_id;
		if (seatNo >= PLAY_COUNT || seatNo < 0) return;
		if (!_players[seatNo]->batteryIsVisible()) return;

		//转换玩家开炮角度
		float degree = 0.0f;
		auto LockFish = dynamic_cast<Fish*>(_fishManager->getFishById(pFire->lock_fishid));

		//如果有锁定，则开炮角度为炮口和目标鱼的角度，否则为服务器发来的玩家开炮角度
		if (pFire->lock_fishid != INVALID_FISHID && nullptr != LockFish)
		{
			//此次开炮有锁定，则需要设置锁定信息
			_players[seatNo]->setIsLockFish(true, pFire->lock_fishid);
			//转换炮口到目标鱼之间的开炮角度
			Vec2 cononpoint = _players[seatNo]->getCannonPoint();
			float angle = MathAide::CalcAngle(LockFish->getPositionX(), LockFish->getPositionY(), cononpoint.x, cononpoint.y);
			degree = CC_RADIANS_TO_DEGREES(angle);
		}
		else
		{
			_players[seatNo]->setIsLockFish(false, INVALID_FISHID);
			degree = CC_RADIANS_TO_DEGREES(pFire->angle);
		}
		
		//这个bMeFire是指本地开炮，防止网络延迟，自己开炮不等服务器消息返回直接显示
		if (!bMeFire)
		{
			//更新玩家金币
			_players[seatNo]->setUserMoney(pFire->fish_score);
		}

		if (!_players[seatNo]->getIsMe())
		{
			//显示玩家开炮
			_players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid, pFire->bIsRobot);
		}
		else
		{
			//如果开炮的是自己，则只显示本地开炮消息
			if (bMeFire)
			{
				_players[seatNo]->gameFire(degree, seatNo, pFire->bullet_id, pFire->bullet_kind, pFire->bullet_mulriple, pFire->lock_fishid);
			}
		}
	}

	//添加普通鱼信息（此信息包含的鱼的数目不定）
	void GameTableUI::dealAddFishInfo(void* object, WORD objectSize)
	{
		CCASSERT(objectSize % sizeof(CMD_S_FishTrace) == 0, "CMD_S_FishTrace size is error");
		if (objectSize % sizeof(CMD_S_FishTrace) != 0) return;
		
		BYTE count = objectSize / sizeof(CMD_S_FishTrace);
		CMD_S_FishTrace* pTrace = (CMD_S_FishTrace*)(object);

		auto winSize = Director::getInstance()->getWinSize();

		for (BYTE i = 0; i < count; i++)
		{
			std::vector<Vec2> initPos;
			for (BYTE j = 0;  j < pTrace->init_count; j++)
			{
				//因为pc端y轴坐标为上面是0，跟手游y轴反向，所以y轴取反
				auto pos = Vec2(pTrace->init_pos[j].x / kResolutionWidth * winSize.width,
					(1.0f - pTrace->init_pos[j].y / kResolutionHeight) * winSize.height);
				initPos.push_back(pos);
			}
			std::vector<FPointAngle> fishTraceList;
			_fishManager->addFish(pTrace->fish_kind, pTrace->fish_id, &initPos, fishTraceList, pTrace->fish_kind_ex, pTrace->trace_type);

			++pTrace;
		}
	}

	//打死带盘鱼王会在此位置出来三圈鱼群
	void GameTableUI::dealAddGroupFishInfo(CMD_S_GroupFishTrace* pFish)
	{
		if (0 == pFish->byIndex)
		{
			if (_fpCircleFishCenter[0].x == 0 && _fpCircleFishCenter[0].y == 0)
			{
				return;
			}
		}
		else
		{
			if (_fpCircleFishCenter[1].x == 0 && _fpCircleFishCenter[1].y == 0)
			{
				return;
			}
		}

		//根据保存的带盘鱼王死亡位置设置出来2圈鱼的行动路径
		for (int i = 0; i < pFish->fish_count; ++i)
		{
			Vec2 init_pos[5];
			int center_x ;
			int center_y ;
			if (0 == pFish->byIndex)
			{
				center_x = _fpCircleFishCenter[0].x;
				center_y = _fpCircleFishCenter[0].y;
			}
			else
			{
				center_x = _fpCircleFishCenter[1].x;
				center_y = _fpCircleFishCenter[1].y;
			}

			auto winSize = Director::getInstance()->getWinSize();
			float fAngle = (float)((M_PI*2) / (pFish->fish_count));

			init_pos[0].x = static_cast<float>(center_x);
			init_pos[0].y = static_cast<float>(center_y);
			init_pos[1].x = static_cast<float>(center_x + sin((float)(i*fAngle))*500);
			init_pos[1].y = static_cast<float>(center_y + cos((float)(i*fAngle))*500);
			init_pos[2].x = static_cast<float>(center_x + sin((float)(i*fAngle))*1440);
			init_pos[2].y = static_cast<float>(center_y + cos((float)(i*fAngle))*900);

			if (true) 
			{
				init_pos[1].x = init_pos[2].x;
				init_pos[1].y = init_pos[2].y;
			}

			std::vector<Vec2> initPos;
			for (BYTE j = 0;  j < 3; j++)
			{
				auto pos = Vec2(init_pos[j].x, init_pos[j].y);
				initPos.push_back(pos);
			}
			std::vector<FPointAngle> fishTraceList;
			_fishManager->addFish(pFish->fish_kind[i], pFish->fish_id[i], &initPos, fishTraceList, pFish->fish_red_kind, TRACE_LINEAR, true);
		}
	}

	void GameTableUI::dealCatchFish(CMD_S_CatchFish* pFish)
	{
		Fish* fish = _fishManager->getFishById(pFish->fish_id);
		if (nullptr == fish) return;

		if (pFish->bullet_ion)
		{
			_players[pFish->chair_id]->setIsEnergyCannon(true);
		}

		if (_tableLogic->_MYSEATNO == pFish->chair_id)
		{
			//自己捕到的这种鱼的类型加1（为了结算展示）
			CCASSERT( pFish->fish_kind < FISH_KIND_COUNT , "访问数组越界");
			if (pFish->fish_kind < FISH_KIND_COUNT)
			{
				_resultInfo->numberOfKindFIsh[pFish->fish_kind]++;
				_resultInfo->userGoldMoney += pFish->fish_score;
			}
		}

		//抓到定海神针让所有鱼停止游动
		if (FISH_KIND_19 == pFish->fish_kind)
		{
			_fishManager->setAllFishIsPause(true);
		}

		if (fish != nullptr)
		{
			FishKingGameAudio::playCatchEffect();
			//播放抓到鱼金币翻滚和显示奖金动画
			playAwardFishAnimation(pFish->chair_id, pFish->fish_score, pFish->i64UserFishScore_, fish);
			//显示鱼死亡动画并移除
			_fishManager->removeOrSetFishDieById(pFish->fish_id, true);
		}
	}

	void GameTableUI::playAwardFishAnimation(BYTE bSeatNo, LLONG money, LLONG userMoney, Fish* fish)
	{
		//抓到的鱼类型大于16则要做震屏特效
		if (fish->getFishKind() >= FISH_KIND_17)
		{
			FishKingGameAudio::playGoldEffect();

			this->stopActionByTag(100);
			this->setPosition(Size::ZERO);
			auto shake = Shake::create(1.0f, 30, 30);
			shake->setTag(100);
			this->runAction(shake);
		}

		//以下种类的鱼要显示bingo转盘
		if ((fish->getFishKind() >= FISH_KIND_18 && fish->getFishKind() <= FISH_KIND_21) ||
			(fish->getFishKind() >= FISH_KIND_25 && fish->getFishKind() <= FISH_KIND_30)) 
		{
			if (money > 0)
			{
				_players[bSeatNo]->setBingo(money);
			}
		}

		ParticleSystem *meteor = nullptr;
		switch (fish->getFishKind())
		{
		case FISH_KIND_17:
		case FISH_KIND_18:
		case FISH_KIND_20:
			{
				meteor = ParticleSystemQuad::create("fishKing/particle/shayu.plist");
			}
			break;
		case FISH_KIND_19:
			{
				meteor = ParticleSystemQuad::create("fishKing/particle/ding.plist");

				auto ding  = Sprite::create("fishKing/particle/Ding_Magic.png");
				ding->setScale(0.5f);
				ding->setPosition(fish->getPosition());
				addChild(ding, FISH_ZORDER + 1);
				ding->runAction(Sequence::create(ScaleTo::create(1.0f, 1.0f), DelayTime::create(1.5f), RemoveSelf::create(true), nullptr));
			}
			break;
		case FISH_KIND_21:
		case FISH_KIND_22:
		case FISH_KIND_23:
			{
				meteor = ParticleSystemQuad::create("fishKing/particle/jinlong.plist");
			}
			break;
		case FISH_KIND_25:
		case FISH_KIND_26:
			{
				meteor = ParticleSystemQuad::create("fishKing/particle/bomb.plist");
			}
			break;
		case FISH_KIND_27:
		case FISH_KIND_28:
		case FISH_KIND_29:
		case FISH_KIND_30:
			{
				meteor = ParticleSystemQuad::create("fishKing/particle/dasanyuan.plist");
			}
			break;
		default:
			break;
		}

		if (nullptr != meteor)
		{
			meteor->setPosition(fish->getPosition());
			meteor->setAutoRemoveOnFinish(true);
			addChild(meteor, FISH_ZORDER + 1);
		}
		
		char str[24];
		if (money > 0)
		{
			//单条鱼获得金币数量		
			sprintf(str, "%lld", money);
			auto moneyText = TextAtlas::create(str, Score_number_path, 54, 62, ".");
			if (fish->getFishKind() <= FISH_KIND_5)
			{
				moneyText->setScale(0.6f);
			}
			moneyText->setPosition(fish->getPosition());
			this->addChild(moneyText, FISH_ZORDER + 2);

			moneyText->runAction(Sequence::create(
				JumpBy::create(0.3f, Vec2(this->getPosition()), 15, 1), 
				DelayTime::create(0.5f),
				Spawn::create(MoveBy::create(1.0f, Vec2(0, 50)), 
				FadeOut::create(1.0f), nullptr), 
				CallFunc::create([=](){
					//FishKingGameAudio::playCatchEffect();
					moneyText->removeFromParent();
			}), nullptr));
		}

		Vec2 pos = _players[bSeatNo]->getMoneyPoint();
		auto eFKind = fish->getFishKind();
		Vec2 fish_pos = fish->getPosition();

		//需要根据鱼类型不同产生不同个数金币
		int coin_count = G_Fish_Coin_Count[eFKind];
		auto cache = SpriteFrameCache::getInstance();
		
		//根据获得的个数和位置制作翻滚金币动画
		for (int i = 0; i < coin_count; ++i) 
		{
			Animation *animation = Animation::create();
			animation->setDelayPerUnit(0.05f);
			animation->setLoops(-1);
			for (BYTE i = 1; i <= 18; i++)
			{
				std::string name("coin21_");
				if (fish->getFishKind() <= FISH_KIND_10)
				{
					name = "coin11_";
				}
				sprintf(str,"%02d.png", i);
				name.append(str);
				animation->addSpriteFrame(cache->getSpriteFrameByName(name));
			}
			//设置随机位置
			int k = (rand() % 200) - 100;
			int s = (rand() % 200) - 100;
			
			auto animate = Animate::create(animation);
			auto coin = Sprite::create();
			coin->setPosition(Vec2(fish_pos.x + k, fish_pos.y + s));
			this->addChild(coin, FISH_ZORDER + 1);
			coin->runAction(Spawn::create(animate, Sequence::create(DelayTime::create(1.5f),
				MoveTo::create(0.5f, pos), CallFunc::create([=](){
					_players[bSeatNo]->setUserMoney(userMoney);
			}), RemoveSelf::create(), nullptr), nullptr));
		}
	}

	void GameTableUI::dealCatchSweepFish(CMD_S_CatchSweepFish* pSweep)
	{
		Fish* fish = _fishManager->getFishById(pSweep->fish_id);
		if (nullptr == fish) return;

		//抓到大鱼要把当前屏幕中鱼的信息反馈服务器
		CMD_C_CatchSweepFish pRet = _fishManager->catchSweepFish(pSweep->chair_id , pSweep->fish_id);
		_tableLogic->sendSweepFish(&pRet, sizeof(CMD_C_CatchSweepFish));

		//带盘子鱼王，会出现鱼阵，需要记录下坐标
		if (fish->getFishKind() == FISH_KIND_32)
		{
			_fishManager->getFishPositionByFishId(fish->getFishID());
			if (0 == pSweep->byIndex)
			{
				_fpCircleFishCenter[0].x = fish->getPositionX();
				_fpCircleFishCenter[0].y = fish->getPositionY();
			}
			else
			{
				_fpCircleFishCenter[1].x = fish->getPositionX();
				_fpCircleFishCenter[1].y = fish->getPositionY();
			}
		}
	}

	void GameTableUI::dealCatchSweepFishResult(CMD_S_CatchSweepFishResult* pResult)
	{
		auto winSize = Director::getInstance()->getWinSize();
		auto fish = _fishManager->getFishById(pResult->fish_id);

		if (fish != nullptr)
		{
			FishKingGameAudio::playGoldEffect();

			if (_tableLogic->_MYSEATNO == pResult->chair_id)
			{
				//自己捕到的这种鱼的类型加1（为了结算展示）
				if (fish->getFishKind() < FISH_KIND_COUNT)
				{
					_resultInfo->numberOfKindFIsh[fish->getFishKind()]++;
					_resultInfo->userGoldMoney += pResult->fish_score_Sum;
				}
			}
			//显示大鱼死亡动画并移除（炸弹，鱼王）
			_fishManager->removeOrSetFishDieById(pResult->fish_id, true);
			//播放抓到大鱼金币翻滚和显示奖金动画
			playAwardFishAnimation(pResult->chair_id, pResult->fish_score_Sum, pResult->i64UserFishScore_, fish);

			if (pResult->fish_score_Sum > 0)
			{
				_players[pResult->chair_id]->setBingo(pResult->fish_score_Sum);
			}

			FishKingGameAudio::playBombEffect();

			ParticleSystem *meteor = nullptr;
			switch (fish->getFishKind())
			{
			case FISH_KIND_24:
				{
					meteor = ParticleSystemQuad::create("fishKing/particle/bomb.plist");
				}
				break;
			case FISH_KIND_31:
				{
					meteor = ParticleSystemQuad::create("fishKing/particle/xuehua.plist");
				}
				break;
			case FISH_KIND_32:
				{
					meteor = ParticleSystemQuad::create("fishKing/particle/yanhua.plist");
				}
				break;
			default:
				break;
			}

			if (nullptr != meteor)
			{
				meteor->setPosition(fish->getPosition());
				meteor->setAutoRemoveOnFinish(true);
				addChild(meteor, FISH_ZORDER);
			}

			//抓到全屏炸弹，所有鱼死掉
			for (int i = 0; i < pResult->catch_fish_count; i++)
			{
				int smallFishId = pResult->catch_fish_id[i];
				auto smallFish = _fishManager->getFishById(smallFishId);
				if (nullptr != smallFish)
				{
					//抓到红鱼鱼王和带盘子鱼王，辅助类型相同的鱼会死掉，播放链接闪电特效
					if (fish->getFishKind() == FISH_KIND_31 || fish->getFishKind() == FISH_KIND_32)
					{
						Vec2 fishPos = fish->getPosition();
						Vec2 sFishPos = _fishManager->getFishPositionByFishId(smallFishId);

						std::string name = (fish->getFishKind() == FISH_KIND_31) ? "fishKing/particle/Light_Blue.png" : "fishKing/particle/Light_Red.png";
						auto light = Sprite::create(name);
						auto range = sqrt(pow(abs(sFishPos.x - fishPos.x), 2) + pow(abs(sFishPos.y - fishPos.y), 2));
						float degree = (atan2(sFishPos.x - fishPos.x, sFishPos.y - fishPos.y) * 180 / 3.14f) - 90;
						float scale = range / light->getContentSize().width;
						light->setScaleX(0.0f);
						light->setRotation(degree);
						light->setAnchorPoint(Vec2(0, 0.5f));
						light->setPosition(fishPos);

						addChild(light, FISH_ZORDER);
						light->runAction(Sequence::create(ScaleTo::create(1.0f, scale), DelayTime::create(0.5f),
							RemoveSelf::create(true), nullptr));
					}

					//显示鱼死亡动画并移除
					_fishManager->removeOrSetFishDieById(smallFishId, true);
					//播放抓到鱼金币翻滚和显示奖金动画
					playAwardFishAnimation(pResult->chair_id, pResult->fish_score[i], pResult->i64UserFishScore_, smallFish);
				}				
			}	
		}
	}

	void GameTableUI::dealCatchSweepFishResultEx(CMD_S_CatchSweepFishResultEx* pResult)
	{
		char str[128];
		sprintf(str, "%s%d号桌的%s被玩家%s捕到，获得%lld金币奖励！！", pResult->cRoomName, pResult->iDeskID + 1, pResult->cFishName, 
			pResult->szNickName, pResult->fish_score);
		showNoticeMessage(GBKToUtf8(str));
	}

	void GameTableUI::dealSwitchScene(CMD_S_SwitchScene* pScene)
	{
		_switchScene = *pScene;

		auto winSize = Director::getInstance()->getWinSize();
		//播放波浪音效
		FishKingGameAudio::pauseBackgroundMusic();
		FishKingGameAudio::playWaveEffect();

		//波浪动画
		char name[64];
		auto waterAnimation = Animation::create();
		waterAnimation->setDelayPerUnit(0.5f);
		waterAnimation->setLoops(-1);
		for(int i = 1; i <= 2; i++)
		{
			sprintf(name, "fishKing/Scene/wave/wave_%02d.png", i);
			waterAnimation->addSpriteFrameWithFile(name);
		}
		auto  animate = Animate::create(waterAnimation);
		auto water = Sprite::create(name);
		float ScaleY = winSize.height/ water->getContentSize().height;
		water->setScaleY(ScaleY);
		water->setAnchorPoint(Vec2(0, 0.5f));
		water->setPosition(Vec2(winSize.width - 200, winSize.height / 2));
		water->runAction(animate);
		addChild(water, 200);

		//切换背景图
		std::string bgName(BG_PATH);
		sprintf(name, "bg%d.png", pScene->Scene_Style + 1);
		bgName.append(name);

		auto backGround = ImageView::create(bgName);
		backGround->setAnchorPoint(Vec2(0 , 0.5f));
		float _xScale = winSize.width / backGround->getContentSize().width;
		float _yScale = winSize.height/ backGround->getContentSize().height;
		backGround->setScaleX(_xScale);
		backGround->setScaleY(_yScale);
		backGround->setPosition(Vec2(winSize.width, winSize.height / 2));
		addChild(backGround);

		auto oldBG = _currentBg;
		_currentBg = backGround;

		_isAllowFire = false;
		//移除跟背景图碰撞的鱼
		_fishManager->setSwitchScene(backGround, true);

		//背景图从右往左运动
		auto water_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - winSize.width, 0));
		auto bg_move = MoveBy::create(SWITCH_SCENE_TIME, Vec2( - winSize.width, 0));
		water->runAction(Sequence::create(water_move, FadeOut::create(0.2f), RemoveSelf::create(true), nullptr));

		backGround->runAction(Sequence::create(bg_move, CallFunc::create([=](){
			oldBG->removeFromParent();
			_fishManager->setSwitchScene(nullptr, false);
			_isAllowFire = true;
			switchScene();
		}), nullptr));		
	}


	void GameTableUI::switchScene()
	{
		FishKingGameAudio::resumeBackgroundMusic();
		FishKingGameAudio::playBackgroundMusic();

		switch (_switchScene.scene_kind)
		{
		case SCENE_KIND_1:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_1_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_1_trace_))
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];
						}
						index++;
					}

					//拷贝生成的鱼潮路径数据给当前鱼
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_1_trace_[i].begin(), _fishTrace->scene_kind_1_trace_[i].end(), std::back_inserter(fishTraceList));
					//创建此条鱼
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_2:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_2_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_2_trace_)) 
					return;

				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					//拷贝生成的鱼潮路径数据给当前鱼
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_2_trace_[i].begin(), _fishTrace->scene_kind_2_trace_[i].end(), std::back_inserter(fishTraceList));
					//创建此条鱼
					auto fish = _fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
					//鱼潮2要求上下两排小鱼游到屏幕中等待大鱼经过，所以设置停止时机
					if (nullptr != fish)
					{
						if (i < 200) 
						{
							fish->setFishStop(_fishTrace->scene_kind_2_small_fish_stop_index_[i], _fishTrace->scene_kind_2_small_fish_stop_count_);
						} 
						else 
						{
							fish->setFishStop(_fishTrace->scene_kind_2_big_fish_stop_index_, _fishTrace->scene_kind_2_big_fish_stop_count_);
						}
					}
				}
			}
			break;
		case SCENE_KIND_3:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_3_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_3_trace_)) 
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];

						}
						index++;
					}

					//拷贝生成的鱼潮路径数据给当前鱼
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_3_trace_[i].begin(), _fishTrace->scene_kind_3_trace_[i].end(), std::back_inserter(fishTraceList));
					//创建此条鱼
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_4:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_4_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_4_trace_)) 
					return;

				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					//拷贝生成的鱼潮路径数据给当前鱼
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_4_trace_[i].begin(), _fishTrace->scene_kind_4_trace_[i].end(), std::back_inserter(fishTraceList));
					//创建此条鱼
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		case SCENE_KIND_5:
			{
				assert(_switchScene.fish_count == arraysize(_fishTrace->scene_kind_5_trace_));
				if (_switchScene.fish_count != arraysize(_fishTrace->scene_kind_5_trace_)) 
					return;

				int index = 0;
				for (int i = 0; i < _switchScene.fish_count; ++i) 
				{
					FishKind fish_kind_ex[5] = {FISH_KIND_1};
					if (_switchScene.fish_kind[i] == FISH_KIND_31)
					{
						for(int k=0; k<5; k++)
						{
							fish_kind_ex[k] = _switchScene.fish_kindEx[index];
						}
						index++;
					}
					//拷贝生成的鱼潮路径数据给当前鱼
					std::vector<FPointAngle> fishTraceList;
					std::copy(_fishTrace->scene_kind_5_trace_[i].begin(), _fishTrace->scene_kind_5_trace_[i].end(), std::back_inserter(fishTraceList));
					//创建此条鱼
					_fishManager->addFish(_switchScene.fish_kind[i], _switchScene.fish_id[i], nullptr, fishTraceList, fish_kind_ex);
				}
			}
			break;
		default:
			break;
		}
	}

	//结算退出
	void GameTableUI::gameSettlementAndExit()
	{
		//停止超时退出倒计时
		gameOutTime(false);
		this->stopActionByTag(outTime_tag);
		auto winSize = Director::getInstance()->getWinSize();
		auto panel = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(Settlemen_path);
		panel->setAnchorPoint(Vec2(0.5,0.5));
		panel->setPosition(winSize/2);
		addChild(panel, PLAYER_ZORDER);
		panel->setName("SettlementNode");
	

		auto settleBG	= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(panel,"Image_SettlementBG"));
		//倒计时
		
		auto time = dynamic_cast<TextAtlas*>(ui::Helper::seekWidgetByName(settleBG,"AtlasLabel_Time"));
		timing(time, 20, [=](){
			panel->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
		});
		//关闭按钮
		auto btnClose	= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(settleBG,"Button_Close"));
		btnClose->addClickEventListener([=](Ref* ref){
			time->setVisible(false);
			panel->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			//开始下次倒计时
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		});
		//取消按钮
		auto btnCancle	= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(settleBG,"Button_Cancle"));
		btnCancle->addClickEventListener([=](Ref* ref){
			time->setVisible(false);
			panel->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			//开始下次倒计时
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		});
		//退出按钮
		auto btnTrue	= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(settleBG,"Button_True"));
		btnTrue->addClickEventListener([this](Ref* ref){
			returnPlatform();
		});

		char str[32];

		//玩家抓鱼收获金币
		sprintf(str, "%lld", _resultInfo->userGoldMoney);
		auto fishMoney	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(settleBG,"AtlasLabel_FishMoney"));
		fishMoney->setString(str);

		//玩家总金币
		sprintf(str, "%lld", _players[_tableLogic->_MYSEATNO]->getCurrentMoney());
		auto userMoney	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(settleBG,"AtlasLabel_UserMoney"));
		userMoney->setString(str);


		//每条鱼抓到的数量
		for (int i = 0; i < 31; i++)
		{
			sprintf(str, "AtlasLabel_Fish%d", i);
			auto fishNum = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(settleBG,str));
			sprintf(str, "%d", _resultInfo->numberOfKindFIsh[i]);
			fishNum->setString(str);
		}
	}

	bool GameTableUI::isValidSeat(BYTE seatNo)
	{
		return (seatNo < PLAY_COUNT && seatNo >= 0);
	}

	
	/*------------------------------------------------------------------------------*/

	//手机返回键回调
	void GameTableUI::onKeyReleased(EventKeyboard::KeyCode keyCode,Event * pEvent)
	{
/*#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		if (EventKeyboard::KeyCode::KEY_BACK != keyCode) return;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		if (EventKeyboard::KeyCode::KEY_F1 != keyCode) return;
#endif

		auto node = dynamic_cast<Node*>(_tableUi->getChildByName("SettlementNode"));
		auto helpLayer = dynamic_cast<Layout*>(_tableUi->getChildByName("Panel_Help"));

		if (helpLayer->isVisible())
		{
			helpLayer->runAction(Sequence::create(FadeOut::create(0.1f), Hide::create(), nullptr));
			return;
		}
		if (nullptr != node)
		{
			node->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(true), nullptr));
			return;
		}
		else
		{
			gameSettlementAndExit();
		}	*/	
	}

	void GameTableUI::onDisconnect()
	{
		//_tableLogic->stop();
		stopAllActions();
		unscheduleAllCallbacks();
		auto prompt = GamePromptLayer::create();
		prompt->showPrompt(GBKToUtf8("网络连接已经断开，请重新登陆！"));
		prompt->setCallBack([this](){			
			RoomLogic()->closeRoom();
			PlatformLogic()->closePlatform();
		//	GameMenu::createMenu();
		});
	}

	void GameTableUI::returnPlatform()
	{
		PlatformLogic()->loginResult.i64Money =_players[_tableLogic->_MYSEATNO]->getCurrentMoney();
		_tableLogic->sendUserUp();
		_tableLogic->sendForceQuit();
	}

	void GameTableUI::dealLeaveDesk()
	{
		RoomLogic()->closeRoom();
	    HallLayer::createScene();
		/*if (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME)
		{
			RoomLogic()->closeRoom();
			HallLayer::createScene();
		}
		else
		{
			GameDesk::createDesk(RoomLogic()->getSelectedRoom(), true);
		}*/
	}

	//游戏开始通知
	void GameTableUI::dealGameStart()
	{
		
	}

	void GameTableUI::gameOutTime(bool bShow)
	{
		auto tip = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(_tableUi, "Image_ExitTip"));
		auto time = (TextAtlas*)(ui::Helper::seekWidgetByName(tip,"AtlasLabel_Time"));

		tip->setVisible(bShow);
		time->setVisible(bShow);

		if (bShow)
		{
			//超时倒计时结束，直接发送站起离开游戏
			timing(time, 10, [this](){
				returnPlatform();
			});
		}
	}

	//倒计时处理,function回调为传进来的倒计时结束要做的处理
	void GameTableUI::timing(Node* node, int dt, std::function<void()> callFunc)
	{
		if (nullptr == node || !node->isVisible()) return;

		char str[12];
		sprintf(str, "%d", dt);
		dt--;
		if (dt < 0) 
		{
			callFunc();
			return;
		}

		TextAtlas* text = (TextAtlas*)node;
		text->setString(str);
		node->runAction(Sequence::create(DelayTime::create(1.0f),
			CallFuncN::create(CC_CALLBACK_1(GameTableUI::timing, this, dt, callFunc)), nullptr));
	}

	bool GameTableUI::onTouchBegan(Touch *touch, Event *unused_event)
	{
		//停止上次倒计时
		gameOutTime(false);
		this->stopActionByTag(outTime_tag);

		//如果处于自动射击状态则停止自动射击
		if (_isAutoFire)
		{
			_isAutoFire = false;
			unschedule(schedule_selector(GameTableUI::setAtuoFire));
		}

		//如果处于准备锁定鱼的状态则在未锁定成功之前不能射击
		if (_isReadyLock)
		{
			//查找锁定
			_fishManager->setLockFish(touch->getLocation(), NORMAL);
			return false;
		}

		//长按连续射击定时器
		_touchPoint = touch->getLocation();
		schedule(schedule_selector(GameTableUI::continuousFire), 0.2f);
		return true;
	}

	void GameTableUI::onTouchMoved(Touch *touch, Event *unused_event)
	{
		_touchPoint = touch->getLocation();
	}

	void GameTableUI::onTouchEnded(Touch *touch, Event *unused_event)
	{
		//松开手指卸载长按连续射击定时器
		unschedule(schedule_selector(GameTableUI::continuousFire));
		continuousFire(0.0f);		

		if (!_isAutoFire && _isAllowFire)
		{
			//开始下次倒计时
			auto seq = Sequence::create(DelayTime::create(60.f), CallFunc::create([this](){
				gameOutTime(true);
			}), nullptr);

			seq->setTag(outTime_tag);
			this->runAction(seq);
		}
	}

	void GameTableUI::setAtuoFire(float dt)
	{
		if (!_isAllowFire) return;
		auto winSize = Director::getInstance()->getWinSize();
		int width	= winSize.width * 0.7f;
		int height	= winSize.height * 0.7f;
		int posX	= rand() % width + 100;
		int posY	= rand() % height + 80;

		openFireByTouch(Vec2(posX, posY));
	}

	void GameTableUI::continuousFire(float dt)
	{
		if (!_isAllowFire) return;
		openFireByTouch(_touchPoint);
	}

	// 触摸屏幕开火
	void GameTableUI::openFireByTouch(const Vec2& touchPoint)
	{
		//已经发射子弹大于20颗，停止射击
		if (_players[_tableLogic->_MYSEATNO]->getBulletNum() > 20)
		{
			return;
		}

		if(!_players[_tableLogic->_MYSEATNO]->isFireEnable())
		{
			showNoticeMessage(GBKToUtf8("玩家金币不够，无法开火！"));
			return ;
		}

		if (!PlatformLogic()->connectPlatform())
		{
			onDisconnect();
			return;
		}

		if (_isAutoFire)
		{
			//停止超时退出倒计时
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);
		}

		Vec2 firePos = Vec2::ZERO;

		//如果没有锁定的鱼，则开炮角度为手指触摸的地方
		if (_lockFishId == INVALID_FISHID)
		{
			firePos = touchPoint;
		}
		else
		{
			auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
			if (nullptr != fish)
			{
				firePos = fish->getPosition();
			}
		}

		Vec2 cononpoint = _players[_tableLogic->_MYSEATNO]->getCannonPoint();
		CMD_C_UserFire userFire;
		userFire.bullet_kind = _players[_tableLogic->_MYSEATNO]->getBullet_Kind();
		userFire.angle = MathAide::CalcAngle(firePos.x , firePos.y , cononpoint.x , cononpoint.y);
		userFire.bullet_mulriple = _players[_tableLogic->_MYSEATNO]->getFirepower();
		userFire.lock_fishid = _lockFishId;
		userFire.bullet_id = ++_iBullet_id;

		if (_players[_tableLogic->_MYSEATNO]->getCurrentMoney() >= userFire.bullet_mulriple)
		{
			_players[_tableLogic->_MYSEATNO]->setUserMoney(_players[_tableLogic->_MYSEATNO]->getCurrentMoney() - userFire.bullet_mulriple);
		}
		else
		{
			Node* root = Director::getInstance()->getRunningScene();
			auto Prompt = dynamic_cast<GamePromptLayer*>(root->getChildByName("Prompt"));
			if (!Prompt)
			{
				Prompt = GamePromptLayer::create();
				Prompt->setName("Prompt");
				Prompt->showPrompt(GBKToUtf8("玩家金币不够，无法开火！"));
			}

			unscheduleAllCallbacks();
			if (_isAutoFire) _isAutoFire = false;
			return;
		}

		if (_isAutoFire)
		{
			//停止超时退出倒计时
			gameOutTime(false);
			this->stopActionByTag(outTime_tag);
		}

		_tableLogic->sendOpenFire(&userFire, sizeof(userFire));

		//为了防止网络延迟造成用户体验问题，自己开炮直接显示
		CMD_S_UserFire meFire;
		meFire.bullet_kind = userFire.bullet_kind;
		meFire.chair_id = _tableLogic->_MYSEATNO;
		meFire.angle = userFire.angle;
		meFire.bullet_mulriple = userFire.bullet_mulriple;
		meFire.lock_fishid = _lockFishId;
		meFire.bullet_id = userFire.bullet_id;
		dealGameFire(&meFire, true);
	}

	void GameTableUI::sendHitFish(BYTE bSeatNo, INT fishId, INT bulletId, BulletKind bulletKind, INT bulletMuriple)
	{
		_tableLogic->sendHitFish(bSeatNo, fishId, bulletId, bulletKind, bulletMuriple);
	}

	void GameTableUI::showNoticeMessage(const std::string& message)
	{
		//_NoticeBG->setVisible(true);
		//_notic->postMessage(message);
		//_NoticeBG->setVisible(true);
		//_text->setString(GBKToUtf8(message.c_str()));
	}

	////////////////////////////////////////////////////////////////////
	//超端
	////////////////////////////////////////////////////////////////////
	void GameTableUI::dealSetAdminConfig(bool bAdmin)
	{
		auto btnAdmin = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_tableUi, "Button_Admin"));
		btnAdmin->setVisible(bAdmin);
	}

	void GameTableUI::dealStockOperateResult(CMD_S_StockOperateResult* pStock)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			char str[32];
			if (3 == pStock->operate_code)
			{
				sprintf(str, "当前抽水：%lld", pStock->stock_score);
			}
			else
			{
				sprintf(str, "当前奖池：%lld", pStock->stock_score);
			}

			admin->showStockOperateResult(str);
		}
	}

	void GameTableUI::dealUpdateOnlinePlayerList(std::vector<OnLineUserInfo> users)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			admin->createOnlineList(users);
		}
	}

	void GameTableUI::dealUpdateContorlList(S_C_UPDATE_CONTROL_INFO* info)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			admin->createControlList(info);
		}
	}

	void GameTableUI::dealUpdateSpecialList(S_C_UPDATE_SPECIAL_INFO* pSpecial)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			admin->createSprcialList(pSpecial);
		}
	}

	void GameTableUI::dealSetSwitchInfo(S_C_SWITCH_INFO* pSwitch)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			admin->setCheckBoxState(pSwitch);
		}
	}
		
	void GameTableUI::dealUpdateFishName(S_C_FISH_NAME* pName)
	{
		GameAdmin* admin = dynamic_cast<GameAdmin*>(this->getChildByName("adminLayer"));
		if (nullptr != admin)
		{
			admin->setSpecialFishName(pName);
		}
	}
}

