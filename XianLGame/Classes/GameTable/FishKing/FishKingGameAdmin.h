#ifndef __FishKing_GameAdmin_H__
#define __FishKing_GameAdmin_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "FishKingMessageHead.h"
#include "FishKingGameTableLogic.h"
#include "ui/CocosGUI.h"
#include "YZNetExport.h"
#include "YZUIExport.h"

USING_NS_CC;

using namespace cocostudio;
using namespace ui;

namespace FishKing
{
	class GameAdmin : public YZLayer
	{
	public:

		typedef std::function<void ()> OnCloseCallBack;
		OnCloseCallBack	onCloseCallBack;


	public:
		GameAdmin(GameTableLogic* tableLogic);
		virtual ~GameAdmin();

	public:
		static GameAdmin* create(GameTableLogic* tableLogic);

	public:
		virtual bool init() override;
		//关闭超端管理页面
		void closeAdmin();

		//创建在线玩家列表
		void createOnlineList(std::vector<OnLineUserInfo> users);
		//创建受控玩家列表
		void createControlList(S_C_UPDATE_CONTROL_INFO* info);
		//创建特殊鱼受控玩家列表
		void createSprcialList(S_C_UPDATE_SPECIAL_INFO* info);

	public:
		//显示奖池操作结果
		void showStockOperateResult(std::string msg);
		//设置控制鱼的名字
		void setSpecialFishName(S_C_FISH_NAME* pName);
		//更新复选框信息
		void setCheckBoxState(S_C_SWITCH_INFO* pSwitch);

	private:
		//在线玩家列表页面
		void getOnlinePageUi();
		//受控玩家列表页面
		void getControlPageUi();
		//特殊鱼受控玩家列表页面
		void getSpecialPageUi();
		//公共按钮
		void getCommonButton();

	private:
		//更新公共按钮状态
		void updateButtonEnabled(BYTE type);
		//更新控制页面按钮和复选框状态
		void updateControlPageButtonEnabled(bool enabled);
		//更新控制页面按钮和复选框状态
		void updateSpecialPageButtonEnabled(bool enabled);
		//添加在线玩家子列
		void addOnlineItem(OnLineUserInfo info);
		//添加受控玩家子列
		void addControlItem(ControlInfo* info);
		//添加特殊鱼受控玩家子列
		void addSpecialItem(SpecialUser user, int idx);
		//添加特殊鱼受控子列
		void addSpecialFishIten(SpecialFish fish);

	private:
		//在线玩家列表点击回调
		void onLineUsersListEventCallBack(Ref* pSender, ui::ListView::EventType type);
		//受控玩家列表点击回调
		void controlUsersListEventCallBack(Ref* pSender, ui::ListView::EventType type);
		//特殊鱼受控玩家列表点击回调
		void specialFishUserListEventCallBack(Ref* pSender, ui::ListView::EventType type);


	private:
		//复选框回调
		void checkBoxCallback(Ref* pSender, CheckBox::EventType type);
		//特殊鱼控制复选框回调
		void specialCheckBoxCallBack(Ref* pSender, CheckBox::EventType type);

	
	protected:
		struct ADMIN_UI
		{
			ImageView*		Image_OnLineBg;		//在线列表背景
			ImageView*		Image_ControlBg;	//玩家控制背景
			ImageView*		Image_SpecialBg;	//特殊鱼控制背景

			Button*			Button_Online;		//在线玩家页面按钮
			Button*			Button_Control;		//受控玩家页面按钮
			Button*			Button_Special;		//特殊鱼受控页面按钮

			Widget*			adminUI;
		}_adminUI;

		ui::ListView*		_ListView_Online;	//在线玩家列表
		ui::ListView*		_ListView_Control;	//受控玩家列表
		ui::ListView*		_ListView_UserID;	//特殊鱼受控玩家列表
		ui::ListView*		_ListView_Fish;		//受控特殊鱼列表

		ui::CheckBox*		_CheckBox_Control;	//玩家是否受控开关复选框
		ui::CheckBox*		_CheckBox_Special;	//特殊鱼是否受控开关复选框

		std::vector<SpecialUser> _userList;		//受控玩家列表数据

		GameTableLogic*		_tableLogic;

	};

}

#endif // __GAMEADMIN_LAYER_H__
