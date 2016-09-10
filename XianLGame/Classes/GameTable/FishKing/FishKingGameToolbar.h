#ifndef __HN_FishKing_TOOLBAR_H__
#define __HN_FishKing_TOOLBAR_H__

#include "YZNetExport.h"
#include "YZUIExport.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

namespace FishKing
{
	class FishKingToolbar : public YZ::YZLayer
	{
	public:
		static FishKingToolbar* create(Layout* tableUi);
		virtual bool init(Layout* tableUi);


	public:
		bool isOut() const { return _isOut;}
	
	protected:
		void toolBarMove();

	protected:
		FishKingToolbar();
		virtual ~FishKingToolbar();

	public:
		void menuLockFishCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);
		void menuClickCallback(cocos2d::Ref* pSender, ui::Widget::TouchEventType touchtype);

		void setCannonSliderCallback(Ref* pSender, ui::Slider::EventType type);

	private:
		bool			_isOut;
		INT				_cannonNum;
		Layout*		_toolBarBG;

		typedef std::function<void()> ExitCallBack;
		typedef std::function<void()> LockFishCallBack;
		typedef std::function<void()> UnLockFishCallBack;
		typedef std::function<void()> LockRedFishCallBack;
		typedef std::function<void()> LockBigFishCallBack;
		typedef std::function<void()> AutoFireCallBack;
		typedef std::function<void(const int cannonNum)> SettingCannonCallBack;

	public:
		ExitCallBack				_onExit;
		LockFishCallBack			_onLockFish;
		UnLockFishCallBack			_unLockFish;
		LockRedFishCallBack			_onLockRedFish;
		LockBigFishCallBack			_onLockBigFish;
		AutoFireCallBack			_onAutoFire;
		SettingCannonCallBack		_onSettingCannon;

	};
}



#endif