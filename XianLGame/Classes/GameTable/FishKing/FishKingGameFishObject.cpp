#include "FishKingGameFishObject.h"
#include "FishKingGameMathAide.h"
#include "FontConfig.h" 

namespace FishKing
{
	static const char*	FISH_ANIMI			=	"fishAnimation";
	static const char*	DASANYUAN			=	"fishKing/fishAnimation/dasanyuan.csb";
	static const char*	DASIXI				=	"fishKing/fishAnimation/dasixi.csb";
	static const char*	ZXDASANYUAN			=	"fishKing/fishAnimation/ZXdasanyuan.csb";
	static const char*	ZXDASIXI			=	"fishKing/fishAnimation/ZXdasixi.csb";
	static const char*	PLATEKING			=	"fishKing/fishAnimation/KingTray.png";

	Fish::Fish(int fishId, FishKind fishKind, FishKind fishKind_ex[5])
		: _fishID (fishId), _fishKind (fishKind), onMoveOutCallBack (nullptr)
		, _isTouch(true), _isPause(false), _fishSize (Size::ZERO), _isLock (false)
		, _fishMoveAction (nullptr), _fishSpeed (0), _fishRect(0, 0, 0, 0)
		, _oldPosition (Vec2(0, 0)), _desPosition (Vec2(0, 0)), _isSwitch (false)
		, _gameBG (nullptr), _isUseFishTrace (true),_stop_index(0), _stop_count(0), _current_stop_count(0)
		, onUnLockCallBack (nullptr)
	{	
		_winSize = Director::getInstance()->getWinSize();
		memcpy(_fishKind_ex, fishKind_ex, sizeof(fishKind_ex));
	}

	Fish::~Fish()
	{
		//release();
	}

	Fish* Fish::create(int fishId, FishKind fishKind, FishKind fishKind_ex[5])
	{
		Fish* pRet = new Fish(fishId, fishKind, fishKind_ex);
		if (pRet && pRet->init())
		{ 
			pRet->autorelease();
			//此处需要retain一次，因为重复利用的时候有把鱼移除父节点，会导致鱼被释放，析构函数release
			pRet->retain();
			return pRet;
		} 
		CC_SAFE_DELETE(pRet);
		return nullptr;
	}

	bool Fish::init()
	{
		if (!YZSprite::init()) return false;
		//设置子节点跟着父节点的透明度变化，如果不设置会导致鱼不能隐藏
		this->setCascadeOpacityEnabled(true);
		//设置帧动画图片信息
		onRender();

		switch (_fishKind)
		{
		case  FISH_KIND_27: //三角大三元
			createDaSanYuanFish(_fishKind_ex[0]);
			break;
		case  FISH_KIND_28: //直线大三元
			createZXDaSanYuanFish(_fishKind_ex[0]);
			break;
		case  FISH_KIND_29: //四角大四喜
			createDaSixiFish(_fishKind_ex[0]);
			break;
		case  FISH_KIND_30: //直线大四喜
			createZXDaSiXiFish(_fishKind_ex[0]);
			break;
		case  FISH_KIND_31: //红色鱼王
			createRedKingFish(_fishKind_ex[0]);
			break;
		case  FISH_KIND_32: //带底盘鱼王
			createPlatekingFish(_fishKind_ex[0]);
			break;
		default:			//普通鱼
			createNormalFish(_fishKind);
			break;
		}
		return true;
	}

	Animation* Fish::addFishData(FishSpirteInfo fish, BYTE kind)
	{
		auto spCache = SpriteFrameCache::getInstance();
		auto aniCache = AnimationCache::getInstance();
		char str[32];

		//创建鱼游动动画
		Animation *bMove = Animation::create();
		bMove->setDelayPerUnit(fish.time);
		bMove->setLoops(-1);
		for (int i=1; i <= fish.count; i++)
		{
			std::string name(fish.name);
			sprintf(str, "walking1_%02d.png",i);
			name.append(str);
			bMove->addSpriteFrame(spCache->getSpriteFrameByName(name));
		}
		sprintf(str, "fish_%d", kind);
		aniCache->addAnimation(bMove, str);
		return bMove;
	}

	//创建普通鱼（类型0-25）
	void Fish::createNormalFish(BYTE fishKind)
	{
		char name[12];
		sprintf(name, "fish_%d", _fishKind);
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));

		if (nullptr == animation)
		{
			animation = addFishData(_fishName[fishKind], fishKind);
		}
		
		auto animate = Animate::create(animation);
		auto fish = Sprite::create();
		fish->runAction(animate);
		this->addChild(fish, 2);

		//影子
		auto fish_Shadow = Sprite::create();
		fish_Shadow->runAction(animate->clone());
		fish_Shadow->setPosition(Vec2(fish->getPositionX() + 15, fish->getPositionY() + 15));
		fish_Shadow->setColor(Color3B(0, 0, 0));
		fish_Shadow->setOpacity(60);	
		this->addChild(fish_Shadow);
	}

	//创建三角大三元鱼
	void Fish::createDaSanYuanFish(BYTE fishKind)
	{
		auto node=dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/GameTableUi/dasanyuan.json"));
		this->addChild(node);
		char str[12];
		for (int i = 0; i < 4; i++)
		{
			Armature*armature = Armature::create("bigFishAnimation");
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,str));
			fishPos->setVisible(false);
            armature->setPosition(fishPos->getPosition());
			if (i>0)
			{
				armature->getAnimation()->playByIndex(1);
			}
			else
			{
				armature->getAnimation()->playByIndex(0);
			}
            
            node->addChild(armature);
		}
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,str));
			sprintf(str, "fish_%d", fishKind);

			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
			if (nullptr == animation)
			{
				animation = addFishData(_fishName[fishKind], fishKind);
			}

			auto animate = Animate::create(animation);
			auto fish = Sprite::create();
			fish->runAction(animate);
			fish->setPosition(fishPos->getPosition());
			this->addChild(fish, 2);
		}
	}

	//创建直线大三元鱼
	void Fish::createZXDaSanYuanFish(BYTE fishKind)
	{
		auto node=dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/GameTableUi/ZXdasanyuan.json"));
		this->addChild(node);
		char str[12];
		Armature*armature = Armature::create("bigFishAnimation");
		auto fPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,"Armature"));
		armature->setPosition(fPos->getPosition());
		fPos->setVisible(false);
		armature->getAnimation()->playByIndex(3);
		node->addChild(armature);
                   
		for (int i = 0; i < 3; i++)
		{
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,str));
			sprintf(str, "fish_%d", fishKind);
			
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
			if (nullptr == animation)
			{
				animation = addFishData(_fishName[fishKind], fishKind);
			}

			auto animate = Animate::create(animation);
			auto fish = Sprite::create();
			fish->runAction(animate);
			fish->setPosition(fishPos->getPosition());
			this->addChild(fish, 2);
		}
	}

	//创建四角大四喜鱼
	void Fish::createDaSixiFish(BYTE fishKind)
	{
		auto node=dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/GameTableUi/dasixi.json"));
		this->addChild(node); 
		char str[12];
		for (int i = 0; i < 5; i++)
		{
			Armature*armature = Armature::create("bigFishAnimation");
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,str));
			fishPos->setVisible(false);
            armature->setPosition(fishPos->getPosition());
			if (i>0)
			{
				armature->getAnimation()->playByIndex(1);
			}
			else
			{
				armature->getAnimation()->playByIndex(0);
			}
            
            node->addChild(armature);
		}
		for (int i = 0; i < 5; i++)
		{
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,str));
			sprintf(str, "fish_%d", fishKind);
			
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
			if (nullptr == animation)
			{
				animation = addFishData(_fishName[fishKind], fishKind);
			}

			auto animate = Animate::create(animation);
			auto fish = Sprite::create();
			fish->runAction(animate);
			fish->setPosition(fishPos->getPosition());
			this->addChild(fish, 2);
		}
	}

	//创建直线大四喜鱼
	void Fish::createZXDaSiXiFish(BYTE fishKind)
	{
		auto node=dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("fishKing/GameTableUi/ZXdasixi.json"));
		this->addChild(node);
		char str[12];
		Armature*armature = Armature::create("bigFishAnimation");
		auto fPos = static_cast<ImageView*>(ui::Helper::seekWidgetByName(node,"Armature"));
		armature->setPosition(fPos->getPosition());
		fPos->setVisible(false);
		armature->getAnimation()->playByIndex(2);
		node->addChild(armature);
		for (BYTE i = 0; i < 4; i++)
		{
			sprintf(str, "Fish%d", i);
			auto fishPos = static_cast<Node*>(node->getChildByName(str));
			sprintf(str, "fish_%d", fishKind);
			
			auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(str));
			if (nullptr == animation)
			{
				animation = addFishData(_fishName[fishKind], fishKind);
			}

			auto animate = Animate::create(animation);
			auto fish = Sprite::create();
			fish->runAction(animate);
			fish->setPosition(fishPos->getPosition());
			this->addChild(fish, 2);
		}
	}

	//创建红色鱼王
	void Fish::createRedKingFish(BYTE fishKind)
	{
		char name[12];
		sprintf(name, "fish_%d", fishKind);
		
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		if (nullptr == animation)
		{
			animation = addFishData(_fishName[fishKind], fishKind);
		}

		auto animate = Animate::create(animation);

		auto fish = Sprite::create();
		fish->runAction(animate);
		fish->setColor(colorRed);
		this->addChild(fish);

		auto fish_Shadow = Sprite::create();
		fish_Shadow->runAction(animate->clone());
		fish_Shadow->setPosition(Vec2(fish->getPositionX() + 15, fish->getPositionY() + 15));
		fish_Shadow->setColor(Color3B(0, 0, 0));
		fish_Shadow->setOpacity(60);	
		this->addChild(fish_Shadow, 1);
	}

	//创建带底盘的鱼王
	void Fish::createPlatekingFish(BYTE fishKind)
	{
		auto fish = Sprite::create(PLATEKING);
		fish->runAction(RepeatForever::create(RotateBy::create(0.5f, 90.0f)));
		this->addChild(fish, -1);

		char name[12];
		sprintf(name, "fish_%d", fishKind);
		
		auto animation = dynamic_cast<Animation*>(AnimationCache::getInstance()->getAnimation(name));
		if (nullptr == animation)
		{
			animation = addFishData(_fishName[fishKind], fishKind);
		}

		auto animate = Animate::create(animation);
		auto smallFish = Sprite::create();
		smallFish->runAction(animate);
		this->addChild(smallFish);
	}

	/////////////////////////////////////////////////////////
	//播放鱼儿死亡动画
	void Fish::playFishDieAction()
	{
		//设置鱼不能被碰撞，防止鱼死亡动画还在进行然后会阻挡子弹
		this->setCanTouch(false);
		//移除Update让鱼停止前进
		this->unscheduleUpdate();

		//如果此鱼被锁定，当鱼死亡时需要解锁
		if (_isLock)
		{
			if (nullptr != onUnLockCallBack)
			{
				_isLock = false;
				onUnLockCallBack(_fishID);
			}
		}

		switch (_fishKind)
		{
		case  FISH_KIND_27: //三角大三元
		case  FISH_KIND_28: //直线大三元
		case  FISH_KIND_29: //四角大四喜
		case  FISH_KIND_30: //直线大四喜
		case  FISH_KIND_32: //带底盘鱼王
			{
				this->runAction(Sequence::create(DelayTime::create(1.0f), 
					RemoveSelf::create(true), nullptr));
			}
			break;
		case  FISH_KIND_31: //红色鱼王
			{
				this->runAction(Sequence::create(Shake::create(1.0f, 10, 10),
					RemoveSelf::create(true), nullptr));
			}
			break;
		default:			//普通鱼
			{
				this->runAction(Sequence::create(Shake::create(1.0f, 10, 10),
					CallFunc::create([this](){
						setHide();
				}), nullptr));
			}
			break;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////
	void Fish::setPosition(const Vec2& pos)
	{
		Sprite::setPosition(pos);

		float tanX = pos.x - _oldPosition.x;
		float tanY = pos.y - _oldPosition.y;
		float radians = -1 * atan2(tanY, tanX);
		float degree = CC_RADIANS_TO_DEGREES(radians);

		this->setRotation(degree);
		_oldPosition = pos;
	}

	void Fish::setHide()
	{
		if (nullptr != _fishMoveAction)
		{
			_fishMoveAction->release();
			_fishMoveAction = nullptr;
		}
		refreshParams();
		if (nullptr != onMoveOutCallBack)
		{
			onMoveOutCallBack(_fishID);
		}
	}

	void Fish::refreshParams()
	{
		unscheduleUpdate();
		setVisible(false);
		_isTouch = false;
		_isSwitch = false;
		_isLock = false;
		_gameBG = nullptr;
		_FishTraceIndex = 0;
		_isUseFishTrace = true;
		_fishTraceList.clear();
		_stop_count = 0;
		_stop_index = -1;
		_current_stop_count = 0;
		memset(_fishKind_ex, 0, sizeof(_fishKind_ex));
	}

	void Fish::setFishTraceList(std::vector<FPointAngle> fishTraceList, bool isUseFishTtrace)
	{
		_fishTraceList.clear();
		_fishTraceList	= fishTraceList;
		_isUseFishTrace	= isUseFishTtrace;
	}

	void Fish::playFishActionByTraceList()
	{
		if (_fishTraceList.empty()) return;

		_oldPosition = Vec2(_fishTraceList[0].x, _winSize.height - _fishTraceList[0].y);
		this->setPosition(Vec2(_fishTraceList[1].x, _winSize.height - _fishTraceList[1].y));
		_FishTraceIndex = 2;

		scheduleUpdate();
	}

	//鱼按照路线游动，匀速直线类型时只有第一和第二两个点有参数，贝塞尔曲线有三个参数
	FiniteTimeAction* Fish::getActionInThreePoint(Vec2 first, Vec2 second, Vec2 third, TraceType actionType)
	{
		FiniteTimeAction* action = nullptr;
		switch (actionType)
		{
		case TRACE_LINEAR:		//匀速直线
			{
				//求两点之间的直线距离 ： |Xa-Xb|^ + |Ya-Yb|^ = 距离的平方 （abs:绝对值，pow:平方，sqrt:开方）
				auto range = sqrt(pow(abs(first.x - second.x), 2) + pow(abs(first.y - second.y), 2));
				auto time = range / (30 * _fishSpeed);

				action = MoveTo::create(time, second);
			}
			break;
		case TRACE_BEZIER:		//贝塞尔曲线
			{	
				float range1 = sqrt(pow(abs(first.x - second.x), 2) + pow(abs(first.y - second.y), 2));
				float range2 = sqrt(pow(abs(second.x - third.x), 2) + pow(abs(second.y - third.y), 2));
				float range = range1 + range2;
				auto time = range / (30 * _fishSpeed);

				ccBezierConfig bezier;
				bezier.controlPoint_1 = first;
				bezier.controlPoint_2 = second;
				bezier.endPosition	  = third;
				action = BezierTo::create(time, bezier);
			}
			break;
		default:
			break;
		}
		return action;
	}

	void Fish::playFishAction(std::vector<Vec2>* fishTrace, TraceType traceType, FLOAT speed)
	{
		_fishSpeed = speed;
		this->setCanTouch(true);
		this->setVisible(true);
		this->setPauseAction(false);
		this->setSwitchScene(false);

		if (!_isUseFishTrace)
		{
			//鱼潮动画
			playFishActionByTraceList();
			return;
		}

		if (nullptr == fishTrace) return;

		_oldPosition = (*fishTrace)[0];
		if (traceType == TRACE_LINEAR)
		{
			_desPosition = (*fishTrace)[1];
		}
		else
		{
			_desPosition = (*fishTrace)[2];
		}

		float go_degree = MathAide::CalcAngle1(_desPosition.x, _desPosition.y, _oldPosition.x, _oldPosition.y);
		float goX = cos(go_degree);
		float goY = sin(go_degree);
		
		this->setPosition(Vec2((*fishTrace)[0].x + goX, (*fishTrace)[0].y + goY));

		if (nullptr != _fishMoveAction)
		{
			_fishMoveAction->release();
			_fishMoveAction = nullptr;
		}

		if (traceType == TRACE_LINEAR)
		{
			_fishMoveAction = getActionInThreePoint((*fishTrace)[0], (*fishTrace)[1],  Vec2::ZERO, traceType);
		}
		else
		{
			_fishMoveAction = getActionInThreePoint((*fishTrace)[0], (*fishTrace)[1],  (*fishTrace)[2], traceType);
		}
		
		_fishMoveAction->retain();
		_fishMoveAction->startWithTarget(this);

		scheduleUpdate();
	}

	void Fish::setFishStop(std::vector<FPointAngle>::size_type stop_index, std::vector<FPointAngle>::size_type stop_count) 
	{
		_stop_index = stop_index;
		_stop_count = stop_count;
		_current_stop_count = 0;
	}

	void Fish::update(float dt)
	{
		//场景切换的时候鱼碰到新背景图就消除掉
		if (_isSwitch)
		{
			if (this->getPositionX() < 0 || this->getPositionX() > _winSize.width ||
				this->getPositionY() < 0 || this->getPositionY() > _winSize.height)
			{
				if (_isLock && nullptr != onUnLockCallBack)
				{
					_isLock = false;
					onUnLockCallBack(_fishID);
				}
				setHide();
				return;
			}
			auto rect = _gameBG->getBoundingBox();
			if (rect.intersectsRect(this->getFishRect()))
			{
				if (_isLock && nullptr != onUnLockCallBack)
				{
					_isLock = false;
					onUnLockCallBack(_fishID);
				}
				setHide();
				return;
			}
		}

		//如果打中定海神针，鱼停止游动
		if (_isPause)
		{
			return;
		}

		auto winSize = Director::getInstance()->getWinSize();
		//更新鱼身上矩形的位置(用来和子弹进行碰撞检测)
		setFishRect(Rect(this->getPositionX() - _fishSize.width / 2, this->getPositionY() - _fishSize.height / 2,
			_fishSize.width, _fishSize.height));

		//如果此鱼被锁定，当游出屏幕时需要解锁
		if (_isLock)
		{
			if (this->getPositionX() < 0 || this->getPositionX() > winSize.width 
				|| this->getPositionY() < 0 || this->getPositionY() > winSize.height)
			{
				if (nullptr != onUnLockCallBack)
				{
					_isLock = false;
					onUnLockCallBack(_fishID);
				}
			}
		}

		if (nullptr != _fishMoveAction)
		{
			if (_fishMoveAction->isDone())
			{
				setHide();
				return;
			}
			_fishMoveAction->step(dt);
			return;
		}

		if (!_isDoUpdateFUNC)
		{
			_isDoUpdateFUNC = true;
			return;
		}
		_isDoUpdateFUNC = false;

		//鱼潮2上下两排小鱼在一定时间内需要先停止
		if (_stop_count > 0 && _FishTraceIndex == _stop_index && _current_stop_count < _stop_count) 
		{
			++_current_stop_count;
			if (_current_stop_count >= _stop_count) setFishStop(0, 0);
			return;
		}
		else if (_FishTraceIndex >= _fishTraceList.size())
		{
			setHide();
			return;
		}
		this->setPosition(Vec2(_fishTraceList[_FishTraceIndex].x, _winSize.height - _fishTraceList[_FishTraceIndex].y));
		_FishTraceIndex++;
	}

	FishKind Fish::getFishKindEx()
	{
		return _fishKind_ex[0];
	}

	void Fish::onRender()
	{
		_fishName[0].name = "fish_woniuyu_";					//蜗牛鱼
		_fishName[0].count = 16;
		_fishName[0].time = 0.11f;

		_fishName[1].name = "fish_lvcaoyu_";					//绿草鱼
		_fishName[1].count = 16;
		_fishName[1].time = 0.11f;

		_fishName[2].name = "fish_huangcaoyu_";					//黄草鱼
		_fishName[2].count = 23;
		_fishName[2].time = 0.1f;

		_fishName[3].name = "fish_dayanyu_";					//大眼鱼
		_fishName[3].count = 14;
		_fishName[3].time = 0.12f;

		_fishName[4].name = "fish_huangbianyu_";				//黄鳊鱼
		_fishName[4].count = 16;
		_fishName[4].time = 0.12f;

		_fishName[5].name = "fish_xiaochouyu_";					//小丑鱼
		_fishName[5].count = 17;
		_fishName[5].time = 0.12f;

		_fishName[6].name = "fish_xiaociyu_";					//小刺鱼
		_fishName[6].count = 36;
		_fishName[6].time = 0.1f;

		_fishName[7].name = "fish_lanyu_";						//蓝鱼
		_fishName[7].count = 17;
		_fishName[7].time = 0.08f;

		_fishName[8].name = "fish_denglongyu_";					//红灯笼鱼
		_fishName[8].count = 23;
		_fishName[8].time = 0.08f;

		_fishName[9].name = "fish_lvDenglongyu_";				//绿灯笼鱼
		_fishName[9].count = 9;
		_fishName[9].time = 0.12f;

		_fishName[10].name = "fish_haigui_";					//海龟
		_fishName[10].count = 24;
		_fishName[10].time = 0.1f;

		_fishName[11].name = "fish_huabanyu_";					//花瓣鱼
		_fishName[11].count = 24;
		_fishName[11].time = 0.12f;

		_fishName[12].name = "fish_hudieyu_";					//蝴蝶鱼
		_fishName[12].count = 24;
		_fishName[12].time = 0.12f;

		_fishName[13].name = "fish_kongqueyu_";					//孔雀鱼
		_fishName[13].count = 24;
		_fishName[13].time = 0.12f;

		_fishName[14].name = "fish_jianyu_";					//剑鱼
		_fishName[14].count = 23;
		_fishName[14].time = 0.12f;

		_fishName[15].name = "fish_bianfuyu_";					//蝙蝠鱼
		_fishName[15].count = 24;
		_fishName[15].time = 0.1f;

		_fishName[16].name = "fish_yinsha_";					//银鲨鱼
		_fishName[16].count = 24;
		_fishName[16].time = 0.1f;

		_fishName[17].name = "fish_jinsha_";					//金鲨鱼
		_fishName[17].count = 24;
		_fishName[17].time = 0.1f;

		_fishName[18].name = "fish_jingubang_";					//金箍棒
		_fishName[18].count = 1;
		_fishName[18].time = 0.3f;

		_fishName[19].name = "fish_dashayu_";					//大鲨鱼
		_fishName[19].count = 9;
		_fishName[19].time = 0.15f;

		_fishName[20].name = "fish_yinlong_";					//银龙
		_fishName[20].count = 16;
		_fishName[20].time = 0.08f;

		_fishName[21].name = "fish_jinlong_";					//金龙
		_fishName[21].count = 16;
		_fishName[21].time = 0.08f;

		_fishName[22].name = "fish_haizao_";					//海藻
		_fishName[22].count = 4;
		_fishName[22].time = 0.3f;

		_fishName[23].name = "fish_bomb_";						//全屏炸弹
		_fishName[23].count = 3;
		_fishName[23].time = 0.3f;

		_fishName[24].name = "fish_jinjinchan_";				//金色金蝉
		_fishName[24].count = 3;
		_fishName[24].time = 0.3f;

		_fishName[25].name = "fish_shuangshayu_";				//双头鲨
		_fishName[25].count = 9;
		_fishName[25].time = 0.2f;
	}
}