#include "FishKingGameFishManager.h"
#include "FishKingGameTableUI.h"
#include "YZUIExport.h"
#include "FishKingGameMathAide.h"
#include "FishKingGameAudio.h"

namespace FishKing
{
	GameFishManager::GameFishManager(GameTableUI* gameTableUI)
		: _gameTableUI(gameTableUI)
		, _isSwitch (false)
		, onLockFishCallBack (nullptr)
		, unLockFishCallBack (nullptr)
	{
		init();
	}

	GameFishManager::~GameFishManager()
	{
		for(auto fish: _fishList)
		{
			fish->release();
		}
	}

	Fish* GameFishManager::addFish(FishKind fishKind, int fishId, std::vector<Vec2>* initPos, std::vector<FPointAngle> fishTraceList, FishKind fishKind_ex[5], TraceType type, bool isGroup)
	{
		if (fishKind >= FISH_KIND_COUNT || fishKind < FISH_KIND_1 || _isSwitch)
		{
			return nullptr;
		}
		if (getFishListByKind(FISH_KIND_29).size()>0&&fishKind==FISH_KIND_27)
		{
			return nullptr;
		}

		Fish* fish = nullptr;
		//小鱼重复利用
		if (fishKind <= FISH_KIND_26)
		{
			for (auto fishTemp : _fishList)
			{
				if (fishTemp->getFishKind() == fishKind && !fishTemp->isVisible())
				{
					fish = fishTemp;
					fish->setFishID(fishId);
					break;
				}
			}
		}

		//没有可重复利用的则新创建一条鱼
		if (nullptr == fish)
		{
			fish = Fish::create(fishId, fishKind, fishKind_ex);
			_fishList.push_back(fish);
		}
		
		//把鱼从父节点上移除，然后重新添加，防止重复利用的鱼渲染层级不对
		if (nullptr != fish->getParent())
		{
			fish->removeFromParentAndCleanup(false);
		}

		_gameTableUI->addChild(fish, FISH_ZORDER);
		fish->setPosition(Vec2(0, -200));

		auto winSize = Director::getInstance()->getWinSize();
		auto scaleX = winSize.width / kResolutionWidth;
		auto scaleY = winSize.height / kResolutionHeight;
		//适配
		if (winSize.width / winSize.height <= 1.5f)
		{
			fish->setScale(scaleX);
		}
		else
		{
			fish->setScale(scaleY);
		}

		if (!fishTraceList.empty())
		{
			//设置鱼是按照鱼阵路径运动
			fish->setFishTraceList(fishTraceList, false);
		}
		
		float fishW = _fishSize[fishKind].width * 0.85f;
		float fishH = _fishSize[fishKind].height * 0.85f;
		//给鱼儿身上绑定一个矩形，用来检测子弹碰撞，大小为服务端发来
		fish->setFishSize(Size(fishW, fishH));
		//如果是鱼阵，则速度固定
		if (isGroup)
		{
			fish->playFishAction(initPos, type, groupFishSpeed);
		}
		else
		{
			fish->playFishAction(initPos, type, _fishSpeed[fishKind]);
		}

		//鱼游动到目的地之后回调
		fish->onMoveOutCallBack = [this](int fishId){
			removeOrSetFishDieById(fishId, false);
		};

		//鱼解锁时回调
		fish->onUnLockCallBack = [this](int fishId){
			if (nullptr != unLockFishCallBack)
			{
				_lockFishIdList.clear();
				unLockFishCallBack(fishId);
			}
		};
		return fish;
	}

	void GameFishManager::setSwitchScene(ImageView* gameBg, bool bSwitch)
	{
		_isSwitch = bSwitch;
		for (auto fish : _fishList)
		{
			if (fish->isVisible())
			{
				fish->setGameBG(gameBg);
				fish->setSwitchScene(bSwitch);
			}
		}
	}

	Vec2 GameFishManager::getFishPositionByFishId(INT fishId)
	{
		auto fish = getFishById(fishId);
		if (nullptr != fish)
		{
			return fish->getPosition();
		}
		return Vec2::ZERO;
	}

	Fish* GameFishManager::getFishById(int id)
	{
		for (auto fish : _fishList)
		{
			if (fish->isVisible() && fish->getFishID() == id)
			{
				return fish;
			}			
		}
		return nullptr;
	}

	Vector<Fish*> GameFishManager::getFishListById(int id)
	{
		Vector<Fish*> fishList;
		for (auto fish : _fishList)
		{
			if (fish->isVisible() && fish->getFishID() == id)
			{
				fishList.pushBack(fish);
			}			
		}
		return fishList;
	}

	Vector<Fish*> GameFishManager::getFishListByKind(int kind)
	{
		Vector<Fish*> fishList;
		for (auto fish : _fishList)
		{
			if (fish->isVisible() && fish->getFishKind() == kind)
			{
				fishList.pushBack(fish);
			}			
		}
		return fishList;
	}

	void GameFishManager::removeOrSetFishDieById(INT fishId, bool bDied)
	{
		bool canErase = false;
		Fish* fish = nullptr;
		auto ite = _fishList.begin();
		for (; ite != _fishList.end(); ite++)
		{
			auto fish_ = (Fish*)*ite;
			if (nullptr == fish_) continue;

			if (fish_->getFishID() == fishId)
			{
				fish = fish_;

				if (fish->getFishKind() > FISH_KIND_26)
				{
					canErase = true;
				}

				if (bDied)
				{
					if (!fish->isVisible()) continue;
					//播放死亡动画特效
					fish->playFishDieAction();
					//播放鱼死亡音效
					if (fish->getFishKind() > FISH_KIND_9 && fish->getFishKind() < FISH_KIND_18)
					{
						FishKingGameAudio::playSmallFishDiead(fish->getFishKind());
					}
				}
				else
				{
					if (fish->getFishKind() > FISH_KIND_26)
					{
						fish->removeFromParent();
					}
				}
				break;
			}
		}
		if (canErase)
		{
			((Fish*)*ite)->release();			
			_fishList.erase(ite);
		}
	}

	void GameFishManager::setAllFishIsPause(bool isPause /* = false */)
	{
		for (auto fish : _fishList)
		{
			if (fish->isVisible())
			{
				fish->setPauseAction(isPause);
			}			
		}
	}

	//设置锁定鱼
	void GameFishManager::setLockFish(Vec2 pos, LockType type)
	{
		auto winSize = Director::getInstance()->getWinSize();
		for (auto fish : _fishList)
		{
			if (!fish->getCanTouch() || !fish->isVisible()) continue;

			switch (type)
			{
			case FishKing::NORMAL:  //普通锁定（触摸鱼锁定）
				{
					Rect rect = fish->getFishRect();
					auto currentSize = Size(rect.size.width * 1.5f , rect.size.height * 1.5f);
					rect.size = currentSize;
					if (rect.containsPoint(pos)) //鱼检测手指
					{
						if (nullptr != onLockFishCallBack)
						{
							fish->setFishIsLock(true);
							//向tableUi报告 锁定的鱼
							onLockFishCallBack(fish->getFishID());
							return;
						}
					}
				}
				break;
			case FishKing::REDFISH:  //锁定红鱼
				{
					if (fish->getFishKind() == FISH_KIND_31)
					{
						bool canLock = true;
						//如果当前鱼已经被锁定过一次，再次点击锁定的时候就要跳过
						for (auto id : _lockFishIdList)
						{
							if (id == fish->getFishID())
							{
								fish->setFishIsLock(false);
								canLock = false;
								break;;
							}
						}

						//如果鱼不在屏幕中，锁定时要跳过
						if (fish->getPositionX() < 0 || fish->getPositionX() > winSize.width 
							|| fish->getPositionY() < 0 || fish->getPositionY() > winSize.height)
						{
							canLock = false;
							break;
						}

						if (canLock && nullptr != onLockFishCallBack)
						{
							fish->setFishIsLock(true);
							_lockFishIdList.push_back(fish->getFishID());
							//向tableUi报告 锁定的鱼
							onLockFishCallBack(fish->getFishID());
							return;
						}
					}
				}
				break;
			case FishKing::BIGFISH:  //锁定大鱼
				{
					if (fish->getFishKind() >= FISH_KIND_17)
					{
						bool canLock = true;
						//如果当前鱼已经被锁定过一次，再次点击锁定的时候就要跳过
						for (auto id : _lockFishIdList)
						{
							if (id == fish->getFishID())
							{
								fish->setFishIsLock(false);
								canLock = false;
								break;;
							}
						}

						//如果鱼不在屏幕中，锁定时要跳过
						if (fish->getPositionX() < 0 || fish->getPositionX() > winSize.width 
							|| fish->getPositionY() < 0 || fish->getPositionY() > winSize.height)
						{
							canLock = false;
							break;
						}

						if (canLock && nullptr != onLockFishCallBack)
						{
							fish->setFishIsLock(true);
							_lockFishIdList.push_back(fish->getFishID());
							//向tableUi报告 锁定的鱼
							onLockFishCallBack(fish->getFishID());
							return;
						}
					}
				}
				break;
			default:
				break;
			}			
		}
	}

	CMD_C_CatchSweepFish GameFishManager::catchSweepFish(WORD chairId , int fishId)
	{
		CMD_C_CatchSweepFish catch_sweep_fish;

		Fish* fish = getFishById(fishId);

		if (chairId >= PLAY_COUNT)
		{
			return catch_sweep_fish;
		}

		catch_sweep_fish.chair_id = chairId;
		catch_sweep_fish.fish_id = fishId;

		if (fish->getFishKind() == FISH_KIND_24)
		{
			for (auto smallFish : _fishList)
			{
				if (smallFish->isVisible())
				{
					catch_sweep_fish.catch_fish_id[catch_sweep_fish.catch_fish_count++] = smallFish->getFishID();
					if (catch_sweep_fish.catch_fish_count >= arraysize(catch_sweep_fish.catch_fish_id))
					{
						break;
					}
				}
			}
		}
		else if (fish->getFishKind() == FISH_KIND_31 || fish->getFishKind() == FISH_KIND_32)
		{
			FishKind kind = fish->getFishKindEx();
			Vector<Fish*> pFish = getFishListByKind(kind);
			for (auto smallFish : pFish)
			{
				catch_sweep_fish.catch_fish_id[catch_sweep_fish.catch_fish_count++] = smallFish->getFishID();
				if (catch_sweep_fish.catch_fish_count >= arraysize(catch_sweep_fish.catch_fish_id))
				{
					break;
				}
			}
		}
		
		return catch_sweep_fish;
	}

	void GameFishManager::setConfig(CMD_S_GameConfig* pcene)
	{
		auto winSize = Director::getInstance()->getWinSize();
		BYTE i = 0;
		for ( ; i < FISH_KIND_COUNT; i++)
		{
			_fishMultiple[i]	= pcene->fish_multiple[i];
			_fishSpeed[i]		= pcene->fish_speed[i];
			_fishSize[i].width	= pcene->fish_bounding_box_width[i] * winSize.width / kResolutionWidth;
			_fishSize[i].height = pcene->fish_bounding_box_height[i] * winSize.height / kResolutionHeight;
			_fishHitRadius[i]	= pcene->fish_hit_radius[i];
		}
		_fishSpeed[i] = 1.0f;
	}

	void GameFishManager::init()
	{
		memset(_fishMultiple, 0x0, sizeof(_fishMultiple));
		memset(_fishSpeed, 0x0, sizeof(_fishSpeed));
		memset(_fishSize, 0x0, sizeof(_fishSize));
		memset(_fishHitRadius, 0x0, sizeof(_fishHitRadius));
	}
}