#include "FishKingGameToolbar.h"

using namespace ui;

namespace FishKing
{
	const char* Name_Exit_Menu			=	"Button_exit";
	const char* Name_Auto_Menu			=	"Button_Auto";
	const char* Name_Move_Menu			=	"Button_Move";
	const char* Name_Lock_Menu			=	"Button_Lock";
	const char* Name_UnLock_Menu		=	"Button_UnLock";
	const char* Name_LockRed_Menu		=	"Button_LockRed";
	const char* Name_LockBig_Menu		=	"Button_LockBig";
	const char* Name_SetCannon_Slider	=	"Slider_SetCannon";

	FishKingToolbar::FishKingToolbar()
		: _isOut (false)
		, _cannonNum (0)
		, _onExit (nullptr)
		, _onLockFish (nullptr)
		, _unLockFish (nullptr)
		, _onLockRedFish (nullptr)
		, _onLockBigFish (nullptr)
		, _onAutoFire (nullptr)
		, _onSettingCannon (nullptr)
	{

	}

	FishKingToolbar::~FishKingToolbar()
	{

	}

	FishKingToolbar* FishKingToolbar::create(Layout* tableUi)
	{
		FishKingToolbar* toolbar = new FishKingToolbar();
		if(toolbar->init(tableUi))
		{
			toolbar->ignoreAnchorPointForPosition(false);
			toolbar->autorelease();
			return toolbar;
		}
		CC_SAFE_DELETE(toolbar);
		return nullptr;
	}

	bool FishKingToolbar::init(Layout* tableUi)
	{
		if(!YZ::YZLayer::init())
		{
			return false;
		}		

		_toolBarBG = tableUi;

		auto MyListener = EventListenerTouchOneByOne::create();
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());//获取的当前触摸的目标  
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = _toolBarBG->getContentSize();
			Rect rect = Rect(0, 0, s.width, s.height);      
			if (rect.containsPoint(locationInNode))
			{
				return false;
			}
			else
			{
				if (_isOut)
				{
					//toolBarMove();
				}				
			}
			return false;
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, _toolBarBG);

		auto btnExit			= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Exit_Menu));
		btnExit->setScale(0.7);
		auto btnAutoFire		= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Auto_Menu));
		btnAutoFire->setScale(0.7);
		//auto btnMove			= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Move_Menu));
		auto btnRedFish			= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_LockRed_Menu));
		btnRedFish->setScale(0.7);
		auto btnBigFish			= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_LockBig_Menu));
		btnBigFish->setScale(0.7);
		auto btnLockFish		= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Lock_Menu));
		btnLockFish->setScale(0.7);
		auto btnUnLock			= dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_UnLock_Menu));
		btnUnLock->setScale(0.7);
		//auto setCannonSlider	= dynamic_cast<Slider*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_SetCannon_Slider));

		btnExit->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuClickCallback, this));
		//btnMove->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuClickCallback, this));
		btnAutoFire->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuClickCallback, this));

		btnRedFish->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuLockFishCallback, this));
		btnBigFish->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuLockFishCallback, this));
		btnLockFish->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuLockFishCallback, this));
		btnUnLock->addTouchEventListener(CC_CALLBACK_2(FishKingToolbar::menuLockFishCallback, this));

		//setCannonSlider->addEventListener(Slider::ccSliderCallback(CC_CALLBACK_2(FishKingToolbar::setCannonSliderCallback, this)));

		return true;
	}

	void FishKingToolbar::setCannonSliderCallback(Ref* pSender, ui::Slider::EventType type)
	{
		Slider* slider = dynamic_cast<Slider*>(pSender);
		int num = slider->getPercent();
		
		if (nullptr != _onSettingCannon)
		{
			_onSettingCannon(num);
		}	
	}

	void FishKingToolbar::menuLockFishCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* pMenu = dynamic_cast<Button*>(pSender);

		std::string name = pMenu->getName();

		if (name.compare(Name_LockRed_Menu) == 0)
		{
			if (nullptr != _onLockRedFish)
			{
				_onLockRedFish();
			}			
		}
		if (name.compare(Name_LockBig_Menu) == 0)
		{
			if (nullptr != _onLockBigFish)
			{
				_onLockBigFish();
			}			
		}
		if (name.compare(Name_Lock_Menu) == 0)
		{
			if (nullptr != _onLockFish)
			{
				_onLockFish();
			}			
		}
		if (name.compare(Name_UnLock_Menu) == 0)
		{
			if (nullptr != _unLockFish)
			{
				_unLockFish();
			}			
		}		
	}

	void FishKingToolbar::menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype)
	{
		if (Widget::TouchEventType::ENDED != touchtype)	return;
		Button* pMenu = dynamic_cast<Button*>(pSender);

		std::string name = pMenu->getName();

		if (name.compare(Name_Exit_Menu) == 0)
		{
			//toolBarMove();
			if (nullptr != _onExit)
			{
				_onExit();
			}
		}

		if (name.compare(Name_Auto_Menu) == 0)
		{
			//toolBarMove();
			if (nullptr != _onAutoFire)
			{
				_onAutoFire();
			}			
		}

		if (name.compare(Name_Move_Menu) == 0)
		{
			//toolBarMove();		
		}	
	}

	void FishKingToolbar::toolBarMove()
	{
		auto winSize = Director::getInstance()->getWinSize();
		auto btnMove = dynamic_cast<Button*>(ui::Helper::seekWidgetByName(_toolBarBG, Name_Move_Menu));

		float width = _toolBarBG->getContentSize().width * 0.4f;
		if (!_isOut)
		{
			_isOut = true;
			btnMove->setBright(false);
			_toolBarBG->runAction(MoveBy::create(0.1f, Vec2(-width, 0)));
		}
		else
		{
			_isOut = false;
			btnMove->setBright(true);
			_toolBarBG->runAction(MoveBy::create(0.1f, Vec2(width, 0)));
		}

	}
}

