#include "FishKingGamePlayer.h"
#include "FishKingGameBullet.h"
#include "FishKingGameTableUI.h"
#include "FishKingGameUserMessageBox.h"
#include "FishKingGameAudio.h"
#include "FontConfig.h"

namespace FishKing
{
	static const char*	doubleTube_img		= "gun21_01.png";
	static const char*	threeTube_img		= "gun2_31_01.png";

	GamePlayer::GamePlayer()
		: AtlasLabel_Money (nullptr)
		, AtlasLabel_CannonNum (nullptr)
		, Layout_Table (nullptr)
		, Image_Battery (nullptr)
		, Image_MoneyBox (nullptr)
		, Sprite_Cannon (nullptr)
		, Image_Card (nullptr)
		, Image_Lock (nullptr)
		, Sprite_Bingo (nullptr)
		, _cannonArmature (nullptr)
		, _cannonType (DoubleTube)
		, _bEnergyCannon (false)
		, _currentBulletKind (BULLET_KIND_1_NORMAL)
		, _isMe (false)
		, _isRobot (false)
		, _seatNo (-1)
		, _bLock (false)
		, _tableUI (nullptr)
		, _bulletNum (0)
		, _lockFishId (INVALID_FISHID)
		,_currentFireCell(1)
	{
		memset(_bulletSpeed, 0x0, sizeof(_bulletSpeed));
		memset(_netRadius, 0x0, sizeof(_netRadius));
		char str[24];
		for (int i = 0; i<BULLET_KIND_COUNT; i++)
		{
			sprintf(str, "bullet%d.png", i);
			bulletName[i] = str;
		}
	}
	GamePlayer::~GamePlayer()
	{

	}

	GamePlayer* GamePlayer::createGamePlayer(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager)
	{
		GamePlayer *player = new GamePlayer();
		if (player && player->init(seatNo, Panel_Table, fishManager))
		{ 
			player->autorelease();
			return player;
		} 
		CC_SAFE_DELETE(player);
		return nullptr;
	}

	bool GamePlayer::init(BYTE seatNo, Layout* Panel_Table, GameFishManager* fishManager)
	{
		if (!YZLayer::init()) return false;

		_seatNo = seatNo;
		_fishManager = fishManager;
		Layout_Table = Panel_Table;
		char str[24];
		sprintf(str, "Image_Battery%d", seatNo);
		Image_Battery			= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Layout_Table,str));

		auto Image_Board		= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Image_Board"));
		Image_Board->setLocalZOrder(5);

		AtlasLabel_Money		= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(Image_Battery,"AtlasLabel_Money"));
		AtlasLabel_CannonNum	= dynamic_cast<Text*>(ui::Helper::seekWidgetByName(Image_Battery,"AtlasLabel_CannonNum"));
		AtlasLabel_CannonNum->setLocalZOrder(6);
	
		Image_MoneyBox          = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Image_MoneyBox"));
		auto Image_Cannon           = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Sprite_Cannon"));
		Image_Cannon->setVisible(false);
		Sprite_Cannon=Sprite::create("fishKing/GameTableUi/table/gun21_01.png");
		Sprite_Cannon->setPosition(Image_Cannon->getPosition());
		Image_Battery->addChild(Sprite_Cannon);
		//Sprite_Cannon = dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Sprite_Cannon"));
		
		Image_Card				= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Image_Card"));
		Image_Card->setCascadeOpacityEnabled(true);
		Image_Lock				= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Image_Lock"));
		Image_Lock->setLocalZOrder(1);
		Image_Lock->setCascadeOpacityEnabled(true);


		btn_add				= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(Image_Battery,"Button_add"));
		btn_add->setTag(31);
		btn_add->addTouchEventListener(CC_CALLBACK_2(GamePlayer::menuEventCallBack,this));
		btn_sub				= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(Image_Battery,"Button_sub"));
		btn_sub->setTag(32);
		btn_sub->addTouchEventListener(CC_CALLBACK_2(GamePlayer::menuEventCallBack,this));
		
        btn_add->setVisible(false);
        btn_sub->setVisible(false);
		

		auto Image_Bingo			= dynamic_cast<ImageView*>(ui::Helper::seekWidgetByName(Image_Battery,"Sprite_Bingo"));
		Image_Bingo->setVisible(false);
		Sprite_Bingo=Sprite::create("fishKing/GameTableUi/Res/bingo1_01.png");	
		Sprite_Bingo->setPosition(Image_Bingo->getPosition());
		Image_Battery->addChild(Sprite_Bingo);
		if (seatNo<3)
		{
			Sprite_Bingo->setRotation(180);
		}

		auto uNum = createLabel(gFontConfig_25, "0", colorOrange);
		uNum->setTag(101);
		uNum->setPosition(Vec2(Sprite_Bingo->getContentSize().width*0.5, Sprite_Bingo->getContentSize().height*0.5));
		Sprite_Bingo->addChild(uNum, 2);
		Sprite_Bingo->setVisible(false);

		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标  
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = Image_MoneyBox->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);      
			if (rect.containsPoint(locationInNode))
			{
				if(Layout_Table == nullptr || !Image_Battery->isVisible())	return true;

				Size winSize = Director::getInstance()->getWinSize();
				/*float scalex = PlatformConfig::getInstance()->getGameDesignSize().width / winSize.width;
				float scaley = PlatformConfig::getInstance()->getGameDesignSize().height / winSize.height;

				Vec2 pos;
				if (PlatformConfig::getInstance()->getGameDesignSize().width < 960)
				{
					pos = target->convertToWorldSpace(touch->getLocation());
				}
				else
				{
					pos = touch->getLocation();
				}*/
				Vec2 pos = touch->getLocation();
				FishKingUserMessageBox* message = FishKingUserMessageBox::createMessageBox(_tableUI, _userId, _currentMoney);
				message->setAnchorPoint(Vec2(1,1));
				message->setPosition(_tableUI->getContentSize().width / 2, _tableUI->getContentSize().height / 2);
				message->setHidePoint(pos);
				message->setTag(111);
				_tableUI->reorderChild(message, MAX_ZORDER);

				/*if (winSize.width / winSize.height <= 1.5f)
				{
					message->setScaleY(scaley);
				}
				else
				{
					message->setScaleX(scalex);
				}*/
				return true;
			}
			else return false;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, Image_MoneyBox);

		return true;
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////
	void GamePlayer::setBatteryVisible(bool bVisible)
	{
		if (nullptr != Image_Battery)
		{
			Image_Battery->setVisible(bVisible);
		}
	}

	void GamePlayer::setUserMoney(const LLONG money)
	{
		_currentMoney = money;
		if (nullptr != AtlasLabel_Money)
		{
			char str[32];
			sprintf(str, "%lld", money);
			AtlasLabel_Money->setString(str);
		}				
	}

	void GamePlayer::setConfig(CMD_S_GameConfig* gConfig)
	{
		_max_bullet_multiple = gConfig->max_bullet_multiple;
		_min_bullet_multiple = gConfig->min_bullet_multiple;

		for (BYTE i = 0; i < BULLET_KIND_COUNT; i++)
		{
			_bulletSpeed[i] = gConfig->bullet_speed[i];
			_netRadius[i] =  gConfig->net_radius[i];
		}
	}

	void GamePlayer::setCannonNum(const INT cannonNum)
	{
		_currentFirepower = cannonNum;

		if (nullptr != AtlasLabel_CannonNum)
		{
			char str[12];
			sprintf(str, "%d", _currentFirepower);
			AtlasLabel_CannonNum->setString(str);
		}

		if (_currentFirepower < 1000)
		{
			setCannonType(CANNON_TYPE::DoubleTube);
			if (_bEnergyCannon)
			{
				_currentBulletKind = BULLET_KIND_1_ION;
			}
			else
			{
				_currentBulletKind = BULLET_KIND_1_NORMAL;
			}
		}
		else
		{
			setCannonType(CANNON_TYPE::ThreeTube);
			if (_bEnergyCannon)
			{
				_currentBulletKind = BULLET_KIND_3_ION;
			}
			else
			{
				_currentBulletKind = BULLET_KIND_3_NORMAL;
			}
		}				
	}

	void GamePlayer::setCannonNumCell(const INT cannonNumCell)
	{
		_currentFireCell=cannonNumCell;
	}

	//设置鱼炮类型
	void GamePlayer::setCannonType(CANNON_TYPE type)
	{
		_cannonType = type;
		switch (type)
		{
		case FishKing::GamePlayer::DoubleTube:
			Sprite_Cannon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(doubleTube_img));
			break;
		case FishKing::GamePlayer::ThreeTube:
			Sprite_Cannon->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(threeTube_img));
			break;
		default:
			break;
		}
	}

	//设置是否能量炮
	void GamePlayer::setIsEnergyCannon(bool isEnergyCannon)
	{
		_bEnergyCannon = isEnergyCannon;

		if (_bEnergyCannon)
		{
			if (!Image_Card->isVisible())
			{
				Image_Card->setVisible(true);
				Image_Card->setOpacity(0);
				auto fadeIn = FadeIn::create(1.5f);
				Image_Card->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
			}
			if (_isMe)
			{
				if (_currentBulletKind == BULLET_KIND_1_NORMAL || _currentBulletKind == BULLET_KIND_2_NORMAL)
				{
					_currentBulletKind = BULLET_KIND_2_ION;
				}

				if (_currentBulletKind == BULLET_KIND_3_NORMAL || _currentBulletKind == BULLET_KIND_4_NORMAL)
				{
					_currentBulletKind = BULLET_KIND_3_ION;
				}
			}
		}
		else
		{
			Image_Card->stopAllActions();
			Image_Card->setVisible(false);

			if (_isMe)
			{
				if (_currentBulletKind == BULLET_KIND_1_ION || _currentBulletKind == BULLET_KIND_2_ION)
				{
					_currentBulletKind = BULLET_KIND_1_NORMAL;
				}

				if (_currentBulletKind == BULLET_KIND_3_ION || _currentBulletKind == BULLET_KIND_4_ION)
				{
					_currentBulletKind = BULLET_KIND_3_NORMAL;
				}
			}
		}			
	}

	void GamePlayer::setIsLockFish(bool bLock, INT fishId)
	{
		if (bLock)
		{
			_bLock = bLock;
			auto lockFish = _fishManager->getFishById(fishId);
			lockFish->setFishIsLock(true);

			schedule(schedule_selector(GamePlayer::showLockIndicatorLine), 0.02f);

			if (!Image_Lock->isVisible())
			{
				Image_Lock->setVisible(true);
				Image_Lock->setOpacity(0);
				auto fadeIn = FadeIn::create(1.5f);
				Image_Lock->runAction(RepeatForever::create(Sequence::create(fadeIn, DelayTime::create(1.0f), fadeIn->reverse(), nullptr)));
			}

			if (_lockFishId != fishId)
			{
				_lockFishId = fishId;
				Image_Lock->removeAllChildren();

				auto lockFish = _fishManager->getFishById(fishId);
				auto fish = Fish::create(0, lockFish->getFishKind(), lockFish->_fishKind_ex);
				fish->setPosition(Vec2(Image_Lock->getContentSize().width / 2, Image_Lock->getContentSize().height * 0.4f));
				if (lockFish->getFishKind() < FISH_KIND_9 || (lockFish->getFishKind() == FISH_KIND_31 && lockFish->getFishKindEx() < FISH_KIND_9))
				{
					fish->setScale(1.0f);
				}
				if (lockFish->getFishKind() >= FISH_KIND_9 &&  lockFish->getFishKind() < FISH_KIND_19 
					|| (lockFish->getFishKind() == FISH_KIND_31 && lockFish->getFishKindEx() >= FISH_KIND_9
					|| lockFish->getFishKind() == FISH_KIND_32))
				{
					fish->setScale(0.6f);
				}
				else if ((lockFish->getFishKind() >= FISH_KIND_19 && lockFish->getFishKind() < FISH_KIND_23)
					|| (lockFish->getFishKind() >= FISH_KIND_27 && lockFish->getFishKind() <= FISH_KIND_30))
				{
					fish->setScale(0.3f);
				}
				else if ((lockFish->getFishKind() >= FISH_KIND_23 && lockFish->getFishKind() <= FISH_KIND_26)
					|| lockFish->getFishKind() == FISH_KIND_32)
				{
					fish->setScale(0.5f);
				}
				else
				{

				}
				
				fish->setRotation(-90);
				Image_Lock->addChild(fish);
			}			
		}
		else
		{
			if (_lockFishId == fishId)
			{
				_bLock = bLock;
				_lockFishId = INVALID_FISHID;
				for (auto bullet : _bulletList)
				{
					bullet->setLockFish_id(INVALID_FISHID);
				}
				_bulletList.clear();
				Image_Lock->setVisible(false);
				Image_Lock->removeAllChildren();
				Image_Lock->stopAllActions();
			}
		}		
	}

	//锁定指示气泡线条
	void GamePlayer::showLockIndicatorLine(float dt)
	{
		auto cache = SpriteFrameCache::getInstance();
		if (_lockFishId != INVALID_FISHID)
		{
			auto fish = dynamic_cast<Fish*>(_fishManager->getFishById(_lockFishId));
			if (nullptr != fish)
			{
				auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
				if (nullptr != oldTip)
				{
					oldTip->removeFromParent();
				}
				for (auto bubble : _bubbleList)
				{
					bubble->removeFromParent();
				}
				_bubbleList.clear();

				auto fishPos = fish->getPosition();
				auto cannonPos = getCannonPoint();

				char str[24];
				sprintf(str, "lock_flag_%d.png", _seatNo + 1);
				auto tip = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName(str));
				tip->setPosition(fishPos);
				tip->setName("bubbleTip");
				_tableUI->addChild(tip, FISH_ZORDER + 2);

				float range = sqrt(pow(abs(fishPos.x - cannonPos.x), 2) + pow(abs(fishPos.y - cannonPos.y), 2));

				float radians = atan2(fishPos.y - cannonPos.y, fishPos.x - cannonPos.x);
				float degree = 270 - CC_RADIANS_TO_DEGREES(radians);
				if (_seatNo >= 3)
				{
					degree -= 180;
				}

				//Sprite_Cannon->setRotation(degree);

				int num = (int)(range / 70);

				for (int i = 1, k = 0; i <= num; i++, k++)
				{
					auto bubble = Sprite::createWithSpriteFrame(cache->getSpriteFrameByName("lock_line.png"));

					float temp = (float)k / (num + 5);
					float scaly = 0.5f + temp;
					bubble->setScale(scaly);

					float len = 70 * i;
					float posx = cannonPos.x + len * cos(radians);
					float posy = cannonPos.y + len * sin(radians);

					bubble->setPosition(Vec2(posx, posy));
					_tableUI->addChild(bubble, FISH_ZORDER + 2);
					_bubbleList.push_back(bubble);

					if (_seatNo == 0)
					{
						bubble->setColor(Color3B(255, 0, 0));
						tip->setColor(Color3B(255, 0, 0));
					}
					else if (_seatNo == 1)
					{
						bubble->setColor(Color3B(255, 255, 0));
						tip->setColor(Color3B(255, 255, 0));
					}
					else if (_seatNo == 2)
					{
						bubble->setColor(Color3B(0, 255, 0));
						tip->setColor(Color3B(0, 255, 0));
					}
					else if (_seatNo == 3)
					{
						bubble->setColor(Color3B(0, 0, 255));
						tip->setColor(Color3B(0, 0, 255));
					}
					else if (_seatNo == 4)
					{
						bubble->setColor(Color3B(255, 0, 255));
						tip->setColor(Color3B(255, 0, 255));
					}
					else
					{
						bubble->setColor(Color3B(255, 165, 0));
						tip->setColor(Color3B(255, 165, 0));
					}
				}
			}
		}
		else
		{
			unschedule(schedule_selector(GamePlayer::showLockIndicatorLine));

			auto oldTip = dynamic_cast<Sprite*>(_tableUI->getChildByName("bubbleTip"));
			if (nullptr != oldTip)
			{
				oldTip->removeFromParent();
			}
			for (auto bubble : _bubbleList)
			{
				bubble->removeFromParent();
			}
			_bubbleList.clear();
		}
	}

	void GamePlayer::setBingo(LLONG money)
	{
		char str[12];
		auto bingoMoney=(Label*)Sprite_Bingo->getChildByTag(101);
		//auto bingoMoney = dynamic_cast<Text*>(ui::Helper::seekWidgetByName(Sprite_Bingo,"AtlasLabel_BingoMoney"));
		Sprite_Bingo->setVisible(true);
		Sprite_Bingo->setOpacity(255);
		sprintf(str, "%lld", money);
		bingoMoney->setString(str);
		bingoMoney->setRotation(-20.0f);
		bingoMoney->stopAllActions();
		auto rotate = RotateBy::create(0.4f, 40.0f);
		bingoMoney->runAction(RepeatForever::create(Sequence::create(rotate, rotate->reverse(), nullptr)));

		Animation *animation = Animation::create();
		animation->setDelayPerUnit(0.2f);
		animation->setLoops(-1);

		FishKingGameAudio::playBingoEffect();

		for (BYTE i = 1; i <= 10; i++)
		{
			std::string name("fishKing/GameTableUi/Res/bingo1_");
			sprintf(str,"%02d.png",i);
			name.append(str);
			animation->addSpriteFrameWithFile(name);
		}
		auto animate = Animate::create(animation);
		Sprite_Bingo->runAction(animate);

		Sprite_Bingo->runAction(Sequence::create(DelayTime::create(5.0f),CallFunc::create([=]{
			Sprite_Bingo->setVisible(false);
		}),  nullptr));
	}
    
    void GamePlayer::setCannonAddOrSubVisible(int userId,bool visible)
    {
        if (visible)
        {
            if (userId==PlatformLogic()->getInstance()->loginResult.dwUserID)
            {
                btn_add->setVisible(true);
                btn_sub->setVisible(true);
            }
        }
        else
        {
            btn_add->setVisible(false);
            btn_sub->setVisible(false);
        }
        
        
    }

///////////////////////////////////////////////////////////////////////////////////////////////////

	bool GamePlayer::batteryIsVisible()
	{
		return Image_Battery->isVisible();
	}

	bool GamePlayer::isFireEnable()
	{
		if (0 == _currentMoney)
		{
			return false;
		}
		return _currentMoney >= _currentFirepower ? true : false;
	}

	Vec2 GamePlayer::getCannonPoint()
	{
		return Image_Battery->convertToWorldSpace(Sprite_Cannon->getPosition());
	}

	Vec2 GamePlayer::getMoneyPoint()
	{
		return Image_Battery->convertToWorldSpace(AtlasLabel_Money->getPosition());
	}

	FLOAT GamePlayer::getBulletNetRadiusById(BYTE id)
	{
		auto winSize = Director::getInstance()->getWinSize();
		return _netRadius[id] * winSize.width / kResolutionWidth;
	}

////////////////////////////////////////////////////////////////////////////////////////////////////

	void GamePlayer::gameFire(FLOAT degree, BYTE seatNo, INT bulletId, BulletKind bulletKind, INT bulletMulriple, INT lockFishId, bool isRobot)
	{
		GameBullet* bullet = nullptr;
		FLOAT speed = _bulletSpeed[bulletKind];
		//在服务器没有发来配置文件信息时， 所有玩家开枪的子弹都不显示
		//在屏幕子弹数目大于20颗时，不再显示开火
		if (speed < 0.000001f || _bulletNum > 20) return ;

		if (!_isMe)
		{
			//更新玩家的炮火威力
			setCannonNum(bulletMulriple);
			//更新玩家子弹类型
			setBullet_Kind(bulletKind);
		}

		if (_bEnergyCannon) 
			FishKingGameAudio::playIonFireEffect();
		else
			FishKingGameAudio::playFireEffect();

		bullet = GameBullet::create(bulletName[bulletKind], this, _fishManager, bulletKind , bulletId , 
			_currentFirepower, getBulletNetRadiusById(bulletKind), speed);

		auto cannon_size = Sprite_Cannon->getContentSize();
		Sprite_Cannon->setRotation(degree);
		if (seatNo>=3)
		{
			Sprite_Cannon->setRotation(degree + 180);
		}	

		playCannonFireAction();
		_tableUI->addChild(bullet , BULLET_ZORDER);

		//如果有锁定鱼，则把之后发射的子弹都加进列表中管理
		if (_bLock) 
		{
			bullet->setLockFish_id(lockFishId);
			_bulletList.push_back(bullet);
		}

		bullet->setAnchorPoint(Vec2(0.5f , 0.5f));
		bullet->setScale(0.8f);
		bullet->setRotation(degree);
		bullet->setPosition(Sprite_Cannon->convertToWorldSpace(Vec2(cannon_size.width * 0.55f , cannon_size.height * 0.9f)));
		bullet->runStartAction(CC_DEGREES_TO_RADIANS(degree - 90));

		//每开炮一次子弹计数加1
		_bulletNum++;
		//子弹碰撞到鱼计数减1
		bullet->onHitFishCallBack = [=](INT fishId , INT bulletId , BulletKind bulletKind , INT  bulletMuriple){
			_bulletNum--;
			//如果开炮的是机器人，则也需要做碰撞检测，通知服务端
			if (_isMe || isRobot) _tableUI->sendHitFish(seatNo, fishId, bulletId, bulletKind, bulletMuriple);
		};
	}

	//播放鱼炮开火动画
	void GamePlayer::playCannonFireAction()
	{
		Sprite_Cannon->stopAllActions();

		char str[24];
		auto spCache = SpriteFrameCache::getInstance();
		auto aniCache = AnimationCache::getInstance();
		sprintf(str, "cannon_%d", _cannonType);
		auto animation = dynamic_cast<Animation*>(aniCache->getAnimation(str));

		if (nullptr == animation)
		{
			animation = Animation::create();
			animation->setDelayPerUnit(0.02f);
			animation->setLoops(1);

			for (int i = 1; i <= 6; i++)
			{
				std::string name;
				if (DoubleTube == _cannonType)
				{
					name = "gun21_";
				}
				else if (ThreeTube == _cannonType)
				{
					name = "gun2_31_";
				}

				sprintf(str, "%02d.png", i);
				name.append(str);
				animation->addSpriteFrame(spCache->getSpriteFrameByName(name));

				sprintf(str, "cannon_%d", _cannonType);
				aniCache->addAnimation(animation, str);
			}
		}		
		
		auto animate = Animate::create(animation);
		Sprite_Cannon->runAction(animate);
	}

	void GamePlayer::menuEventCallBack(Ref* pSender, Widget::TouchEventType type)
	{
		if (Widget::TouchEventType::ENDED != type) return;
		auto btn=(Button*)pSender;
		int num=atoi(AtlasLabel_CannonNum->getString().c_str());
		switch (btn->getTag())
		{
		case 31:
			num+=_currentFireCell;
			if(num>_max_bullet_multiple)
			{
				num=_max_bullet_multiple;
			}
			setCannonNum(num);
			break;
		case 32:
			num-=_currentFireCell;
			if (num<_min_bullet_multiple)
			{
				num=_min_bullet_multiple;
			}
			setCannonNum(num);
			break;
		default:
			break;
		}
	}
}
