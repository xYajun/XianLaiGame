#include "FishKingGameAdmin.h"
#include "YZUIExport.h"
#include "GamePrompt.h"

namespace FishKing
{
	static const char* ADMINUI_PATH			= "fishKing/AdminUi/AdminNode.csb";
	static const char* LIST_BG1				= "fishKing/AdminUi/Res/listItem_1.png";               //listBG
	static const char* LIST_BG2				= "fishKing/AdminUi/Res/listItem_2.png";               //listBG
	static const char* LIST_BG3				= "fishKing/AdminUi/Res/listItem_3.png";               //listBG

	GameAdmin::GameAdmin(GameTableLogic* tableLogic)
		: _tableLogic (tableLogic)
		, onCloseCallBack (nullptr)
	{
	}

	GameAdmin::~GameAdmin()
	{
	}

	void GameAdmin::closeAdmin()
	{
		_adminUI.adminUI->runAction(Sequence::create(FadeOut::create(0.1f), CCCallFunc::create([&]()
		{
			if (nullptr != onCloseCallBack)
			{
				onCloseCallBack();
			}
			this->removeFromParent();
		}), nullptr));
	}

	GameAdmin* GameAdmin::create(GameTableLogic* tableLogic)
	{
		GameAdmin *admin = new GameAdmin(tableLogic);
		if (admin && admin->init())
		{ 
			admin->autorelease();
			return admin;
		} 
		CC_SAFE_DELETE(admin);
		return nullptr;
	}

	bool GameAdmin::init()
	{
		if ( !YZLayer::init()) return false;

		ignoreAnchorPointForPosition(false);
		setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		Size winSize = Director::getInstance()->getWinSize();

		auto admin_Node = CSLoader::createNode(ADMINUI_PATH);
		this->addChild(admin_Node, 1, 10);

		auto panel_admin = (Layout*)admin_Node->getChildByName("Panel_Admin");
		panel_admin->setAnchorPoint(Vec2(0.5f, 0.5f));
		panel_admin->setPosition(Vec2(winSize.width / 2, winSize.height / 2));

		float scaleX = winSize.width / panel_admin->getContentSize().width;
		float scaleY = winSize.height / panel_admin->getContentSize().height;
		panel_admin->setScale(scaleX, scaleY);

		_adminUI.adminUI = (Layout*)panel_admin->getChildByName("Panel_BG");
		//适配
		float scX = 1280 / winSize.width;
		float scY = 720 / winSize.height;
		_adminUI.adminUI->setScale(scX, scY);

		getOnlinePageUi();
		getControlPageUi();
		getSpecialPageUi();
		getCommonButton();

		return true;
	}

	//在线玩家列表页面
	void GameAdmin::getOnlinePageUi()
	{
		//页面背景
		_adminUI.Image_OnLineBg = dynamic_cast<ImageView*>(_adminUI.adminUI->getChildByName("Image_OnLineBg"));

		if (nullptr!= _adminUI.Image_OnLineBg)
		{
			//在线玩家列表
			_ListView_Online = dynamic_cast<ui::ListView*>(_adminUI.Image_OnLineBg->getChildByName("ListView_Online"));
			_ListView_Online->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameAdmin::onLineUsersListEventCallBack, this)));

			//查询奖池按钮
			auto btnStockQuery = dynamic_cast<Button*>(_adminUI.Image_OnLineBg->getChildByName("Button_StockQuery"));
			btnStockQuery->addClickEventListener([=](Ref*){
				_tableLogic->sendStockOperate(0);
			});

			//增加奖池按钮
			auto btnStockAdd = dynamic_cast<Button*>(_adminUI.Image_OnLineBg->getChildByName("Button_StockAdd"));
			btnStockAdd->addClickEventListener([=](Ref*){
				_tableLogic->sendStockOperate(2);
			});

			//清零奖池按钮
			auto btnStockClear = dynamic_cast<Button*>(_adminUI.Image_OnLineBg->getChildByName("Button_StockClear"));
			btnStockClear->addClickEventListener([=](Ref*){
				_tableLogic->sendStockOperate(1);
			});

			//查询抽水按钮
			auto btnBrokerage = dynamic_cast<Button*>(_adminUI.Image_OnLineBg->getChildByName("Button_Brokerage"));
			btnBrokerage->addClickEventListener([=](Ref*){
				_tableLogic->sendStockOperate(3);
			});
		}
	}

	//受控玩家列表页面
	void GameAdmin::getControlPageUi()
	{
		//页面背景
		_adminUI.Image_ControlBg = dynamic_cast<ImageView*>(_adminUI.adminUI->getChildByName("Image_ControlBg"));
		if (nullptr!= _adminUI.Image_ControlBg)
		{
			//受控玩家列表
			_ListView_Control = dynamic_cast<ui::ListView*>(_adminUI.Image_ControlBg->getChildByName("ListView_Control"));
			_ListView_Control->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameAdmin::controlUsersListEventCallBack, this)));

			//开关复选框
			_CheckBox_Control = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Control"));
			_CheckBox_Control->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameAdmin::checkBoxCallback, this)));

			auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_UserID"));
			auto TextField_Score = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_Score"));
			auto TextField_Chance = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_Chance"));

			//输赢复选框
			auto CheckBox_Win = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Win"));
			auto CheckBox_Lose = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Lose"));
			CheckBox_Win->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameAdmin::checkBoxCallback, this)));
			CheckBox_Lose->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameAdmin::checkBoxCallback, this)));

			auto Button_Add = dynamic_cast<Button*>(_adminUI.Image_ControlBg->getChildByName("Button_Add"));
			Button_Add->addClickEventListener([=](Ref*){
				
				if (!Tools::verifyNumber(TextField_UserID->getString()))
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入正确的玩家ID！"));
					return;
				}

				ControlInfo info;
				info.user_id_ = atoi(TextField_UserID->getString().c_str());
				info.limit_score_ = atoi(TextField_Score->getString().c_str());
				info.catch_rate_ = atoi(TextField_Chance->getString().c_str());
				info.win_or_lose_ = CheckBox_Win->isSelected();

				if (!Tools::verifyNumber(TextField_Score->getString().c_str()) || info.limit_score_ <= 0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入大于0的分数！"));
					return;
				}

				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_ADD_CONTROL_INFO_SIG, &info, sizeof(info));
			});

			auto Button_Cut = dynamic_cast<Button*>(_adminUI.Image_ControlBg->getChildByName("Button_Cut"));
			Button_Cut->addClickEventListener([=](Ref*){

				if (!Tools::verifyNumber(TextField_UserID->getString()))
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入正确的玩家ID！"));
					return;
				}

				C_S_DELETE_CONTROL_INFO control;
				control.user_id_ = atoi(TextField_UserID->getString().c_str());

				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_DELETE_CONTROL_INFO_SIG, &control, sizeof(control));
			});
		}
	}

	//特殊鱼受控玩家列表页面
	void GameAdmin::getSpecialPageUi()
	{
		//页面背景
		_adminUI.Image_SpecialBg = dynamic_cast<ImageView*>(_adminUI.adminUI->getChildByName("Image_SpecialBg"));
		if (nullptr!= _adminUI.Image_SpecialBg)
		{
			//特殊鱼受控玩家列表
			_ListView_UserID = dynamic_cast<ui::ListView*>(_adminUI.Image_SpecialBg->getChildByName("ListView_UserID"));
			_ListView_UserID->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(GameAdmin::specialFishUserListEventCallBack, this)));

			//开关复选框
			_CheckBox_Special = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Special"));
			_CheckBox_Special->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameAdmin::checkBoxCallback, this)));

			//鱼列表
			_ListView_Fish = dynamic_cast<ui::ListView*>(_adminUI.Image_SpecialBg->getChildByName("ListView_Special"));

			char str[24];
			for (int i = 1; i <= 4; i++)
			{
				sprintf(str, "CheckBox_Fish%d", i);
				auto checkBox_Fish = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName(str));
				checkBox_Fish->addEventListener(CheckBox::ccCheckBoxCallback(CC_CALLBACK_2(GameAdmin::specialCheckBoxCallBack, this)));
			}

			auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_UserID"));
			auto TextField_Num = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_Num"));
			auto TextField_Chance = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_Chance"));

			//添加按钮
			auto Button_Append = dynamic_cast<Button*>(_adminUI.Image_SpecialBg->getChildByName("Button_Append"));
			Button_Append->addClickEventListener([=](Ref*){
				if (!Tools::verifyNumber(TextField_UserID->getString()))
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入正确的玩家ID！"));
					return;
				}
				char str[24];
				int kind = 0;
				for (int i = 1; i <= 4; i++)
				{
					sprintf(str, "CheckBox_Fish%d", i);
					auto checkBox_Fish = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName(str));
					if (checkBox_Fish->isSelected())
					{
						kind = checkBox_Fish->getTag();
						break;
					}
				}
				C_S_ADD_SPECIAL add_special;
				add_special.user_id_ = atoi(TextField_UserID->getString().c_str());
				add_special.fish_kind_ = kind;
				add_special.left_num_ = atoi(TextField_Num->getString().c_str());
				add_special.catch_rate_ = atoi(TextField_Chance->getString().c_str());

				if (!Tools::verifyNumber(TextField_Num->getString().c_str()) || add_special.left_num_ <= 0)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入大于0的剩余条数！"));
					return;
				}
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_ADD_SPECIAL_SIG, &add_special, sizeof(add_special));
			});

			//删除按钮
			auto Button_Delete = dynamic_cast<Button*>(_adminUI.Image_SpecialBg->getChildByName("Button_Delete"));
			Button_Delete->addClickEventListener([=](Ref*){
				if (!Tools::verifyNumber(TextField_UserID->getString()))
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8("请输入正确的玩家ID！"));
					return;
				}
				char str[24];
				int kind = 0;
				for (int i = 1; i <= 4; i++)
				{
					sprintf(str, "CheckBox_Fish%d", i);
					auto checkBox_Fish = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName(str));
					if (checkBox_Fish->isSelected())
					{
						kind = checkBox_Fish->getTag();
						break;
					}
				}
				C_S_DELETE_SPECIAL delete_special;
				delete_special.user_id_ = atoi(TextField_UserID->getString().c_str());
				delete_special.fish_kind_ = kind;
				RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_DELETE_SPECIAL_SIG, &delete_special, sizeof(delete_special));
			});
		}
	}

	//公共按钮
	void GameAdmin::getCommonButton()
	{
		//关闭按钮
		Button* btnClose = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Close"));
		btnClose->addClickEventListener([=](Ref*){closeAdmin();});

		//在线玩家页面按钮
		Button* btnOnline = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Online"));
		//受控玩家页面按钮
		Button* btnControl = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Control"));
		//特殊鱼受控玩家页面按钮
		Button* btnSpecial = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Special"));

		btnOnline->addClickEventListener([=](Ref*){
			updateButtonEnabled(1);
		});

		btnControl->addClickEventListener([=](Ref*){
			updateButtonEnabled(2);
		});

		btnSpecial->addClickEventListener([=](Ref*){
			updateButtonEnabled(3);
		});

		//更新数据按钮
		Button* btnUpdate = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Update"));
		btnUpdate->addClickEventListener([=](Ref*){
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_UPDATE_SUPER_INFO_SIG);
		});
	}

	//更新按钮状态
	void GameAdmin::updateButtonEnabled(BYTE type)
	{
		//在线玩家页面按钮
		Button* btnOnline = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Online"));
		//受控玩家页面按钮
		Button* btnControl = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Control"));
		//特殊鱼受控玩家页面按钮
		Button* btnSpecial = dynamic_cast<Button*>(_adminUI.adminUI->getChildByName("Button_Special"));

		bool onLine = 1 == type ? true : false;
		bool contorl = 2 == type ? true : false;
		bool special = 3 == type ? true : false;

		btnOnline->setBright(!onLine);
		btnControl->setBright(!contorl);
		btnSpecial->setBright(!special);
		btnOnline->setEnabled(!onLine);
		btnControl->setEnabled(!contorl);
		btnSpecial->setEnabled(!special);
		_adminUI.Image_OnLineBg->setVisible(onLine);
		_adminUI.Image_ControlBg->setVisible(contorl);
		_adminUI.Image_SpecialBg->setVisible(special);
	}

	//更新控制页面按钮和复选框状态
	void GameAdmin::updateControlPageButtonEnabled(bool enabled)
	{
		_CheckBox_Control->setSelected(enabled);

		//玩家ID输入框
		auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_UserID"));
		TextField_UserID->setTouchEnabled(enabled);
		//限制分数输入框
		auto TextField_Score = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_Score"));
		TextField_Score->setTouchEnabled(enabled);
		//特殊鱼输入框
		auto TextField_Chance = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_Chance"));
		TextField_Chance->setTouchEnabled(enabled);

		//输赢选择复选框
		auto CheckBox_Win = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Win"));
		if (!CheckBox_Win->isSelected()) CheckBox_Win->setTouchEnabled(enabled);
		CheckBox_Win->setBright(enabled);
		auto CheckBox_Lose = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Lose"));
		if (!CheckBox_Lose->isSelected()) CheckBox_Lose->setTouchEnabled(enabled);
		CheckBox_Lose->setBright(enabled);

		//增加减少按钮
		auto Button_Add = dynamic_cast<Button*>(_adminUI.Image_ControlBg->getChildByName("Button_Add"));
		Button_Add->setEnabled(enabled);
		Button_Add->setBright(enabled);
		auto Button_Cut = dynamic_cast<Button*>(_adminUI.Image_ControlBg->getChildByName("Button_Cut"));
		Button_Cut->setEnabled(enabled);
		Button_Cut->setBright(enabled);
	}

	//更新特殊鱼控制页面按钮和复选框状态
	void GameAdmin::updateSpecialPageButtonEnabled(bool enabled)
	{
		_CheckBox_Special->setSelected(enabled);

		//四种特殊鱼复选框
		auto CheckBox_Fish1 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish1"));
		CheckBox_Fish1->setTouchEnabled(enabled);
		CheckBox_Fish1->setBright(enabled);
		auto CheckBox_Fish2 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish2"));
		CheckBox_Fish2->setTouchEnabled(enabled);
		CheckBox_Fish2->setBright(enabled);
		auto CheckBox_Fish3 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish3"));
		CheckBox_Fish3->setTouchEnabled(enabled);
		CheckBox_Fish3->setBright(enabled);
		auto CheckBox_Fish4 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish4"));
		CheckBox_Fish4->setTouchEnabled(enabled);
		CheckBox_Fish4->setBright(enabled);

		//玩家ID输入框
		auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_UserID"));
		TextField_UserID->setTouchEnabled(enabled);
		//剩余条数输入框
		auto TextField_Num = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_Num"));
		TextField_Num->setTouchEnabled(enabled);
		//抓捕几率输入框
		auto TextField_Chance = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_Chance"));
		TextField_Chance->setTouchEnabled(enabled);

		//添加删除按钮
		auto Button_Append = dynamic_cast<Button*>(_adminUI.Image_SpecialBg->getChildByName("Button_Append"));
		Button_Append->setEnabled(enabled);
		Button_Append->setBright(enabled);
		auto Button_Delete = dynamic_cast<Button*>(_adminUI.Image_SpecialBg->getChildByName("Button_Delete"));
		Button_Delete->setEnabled(enabled);
		Button_Delete->setBright(enabled);
	}

	//创建在线玩家列表
	void GameAdmin::createOnlineList(std::vector<OnLineUserInfo> users)
	{
		_ListView_Online->removeAllItems();
		for (auto user : users)
		{
			addOnlineItem(user);
		}
	}

	//创建受控玩家列表
	void GameAdmin::createControlList(S_C_UPDATE_CONTROL_INFO* info)
	{
		_ListView_Control->removeAllItems();

		for (int i = 0; i < 40; i++)
		{
			if (0 != info->control_infos_[i].user_id_)
			{
				addControlItem(&info->control_infos_[i]);
			}
		}
	}

	//创建特殊鱼受控玩家列表
	void GameAdmin::createSprcialList(S_C_UPDATE_SPECIAL_INFO* info)
	{
		_ListView_UserID->removeAllItems();
		_userList.clear();

		for (int i = 0, n = 0; i < 20; i++)
		{
			if (0 != info->special_infos_[i].user_id_)
			{
				addSpecialItem(info->special_infos_[i], n);
				n++;
			}
		}
	}

	//添加在线玩家子列
	void GameAdmin::addOnlineItem(OnLineUserInfo info)
	{
		// 在线玩家子列表背景图
		auto listItem = ImageView::create(LIST_BG1);
		float itemWidth = listItem->getContentSize().width;
		float itemHeight = listItem->getContentSize().height;
		listItem->setTouchEnabled(true);

		char str[32];
		//玩家id
		sprintf(str, "%d", info.userID);
		auto userIdText = Text::create(str, "", 22);
		userIdText->setPosition(Vec2(itemWidth * 0.12f, itemHeight * 0.55f));
		listItem->addChild(userIdText);

		listItem->setTag(info.userID);

		//玩家昵称
		sprintf(str, "%s", info.nickName);
		auto nickNameText = Text::create(GBKToUtf8(str), "", 22);
		nickNameText->setPosition(Vec2(itemWidth * 0.35f, itemHeight * 0.55f));
		listItem->addChild(nickNameText);

		//玩家桌号
		sprintf(str, "%d", info.deskNo + 1);		
		auto deskNoText = Text::create(str, "", 22);
		deskNoText->setPosition(Vec2(itemWidth * 0.62f, itemHeight * 0.55f));
		listItem->addChild(deskNoText);
		if (255 == info.deskNo)
		{
			deskNoText->setString(GBKToUtf8("空闲"));
		}

		//玩家游戏币
		sprintf(str, "%d", info.i64UserMoney);
		auto uMoneyText = Text::create(str, "", 22);
		uMoneyText->setPosition(Vec2(itemWidth * 0.85f, itemHeight * 0.55f));
		listItem->addChild(uMoneyText);

		_ListView_Online->pushBackCustomItem(listItem);
	}

	//添加受控玩家子列
	void GameAdmin::addControlItem(ControlInfo* info)
	{
		// 受控玩家子列表背景图
		auto listItem = ImageView::create(LIST_BG1);
		float itemWidth = listItem->getContentSize().width;
		float itemHeight = listItem->getContentSize().height;
		listItem->setTouchEnabled(true);

		char str[32];
		//玩家id
		sprintf(str, "%d", info->user_id_);
		auto userIdText = Text::create(str, "", 22);
		userIdText->setPosition(Vec2(itemWidth * 0.12f, itemHeight * 0.55f));
		listItem->addChild(userIdText);

		listItem->setTag(info->user_id_);

		//输赢概率
		sprintf(str, "%d", info->catch_rate_);
		auto rateText = Text::create(str, "", 22);
		rateText->setPosition(Vec2(itemWidth * 0.35f, itemHeight * 0.55f));
		listItem->addChild(rateText);

		//限制分数
		sprintf(str, "%lld", info->limit_score_);
		auto limitText = Text::create(str, "", 22);
		limitText->setPosition(Vec2(itemWidth * 0.62f, itemHeight * 0.55f));
		listItem->addChild(limitText);

		//赢或者输
		sprintf(str, "%s", info->win_or_lose_ ? "赢" : "输");
		auto winOrloseText = Text::create(GBKToUtf8(str), "", 22);
		winOrloseText->setPosition(Vec2(itemWidth * 0.85f, itemHeight * 0.55f));
		listItem->addChild(winOrloseText);

		_ListView_Control->pushBackCustomItem(listItem);
	}

	//添加特殊鱼受控玩家子列
	void GameAdmin::addSpecialItem(SpecialUser user, int idx)
	{
		// 受控玩家子列表背景图
		auto listItem = ImageView::create(LIST_BG2);
		float itemWidth = listItem->getContentSize().width;
		float itemHeight = listItem->getContentSize().height;
		listItem->setTouchEnabled(true);
		_userList.push_back(user);
		listItem->setTag(user.user_id_);

		char str[32];
		//玩家id
		sprintf(str, "%d", user.user_id_);
		auto userIdText = Text::create(str, "", 22);
		userIdText->setPosition(Vec2(itemWidth / 2, itemHeight * 0.55f));
		listItem->addChild(userIdText);

		_ListView_UserID->pushBackCustomItem(listItem);
	}

	//添加特殊鱼受控子列
	void GameAdmin::addSpecialFishIten(SpecialFish fish)
	{
		// 受控玩家子列表背景图
		auto listItem = ImageView::create(LIST_BG3);
		float itemWidth = listItem->getContentSize().width;
		float itemHeight = listItem->getContentSize().height;
		
		char str[32];
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "Text_Fish%d", i + 1);
			auto text = dynamic_cast<Text*>(_adminUI.Image_SpecialBg->getChildByName(str));
			std::string temp = Utf8ToGB(text->getString().c_str());
			if (fish.fish_kind_ == text->getTag())
			{
				sprintf(str, "%s", temp.c_str());
				break;
			}
		}

		//鱼类型
		auto fishKindText = Text::create(GBKToUtf8(str), "", 22);
		fishKindText->setPosition(Vec2(itemWidth * 0.15f, itemHeight * 0.55f));
		listItem->addChild(fishKindText);

		//剩余数量
		sprintf(str, "%d", fish.left_num_);
		auto numText = Text::create(str, "", 22);
		numText->setPosition(Vec2(itemWidth / 2, itemHeight * 0.55f));
		listItem->addChild(numText);

		//抓捕概率
		sprintf(str, "%d", fish.catch_rate_);
		auto rateText = Text::create(str, "", 22);
		rateText->setPosition(Vec2(itemWidth * 0.8f, itemHeight * 0.55f));
		listItem->addChild(rateText);

		_ListView_Fish->pushBackCustomItem(listItem);
	}

	//在线玩家列表点击回调
	void GameAdmin::onLineUsersListEventCallBack(Ref* pSender, ui::ListView::EventType type)
	{
		if (ListView::EventType::ON_SELECTED_ITEM_END != type) return;
		ListView* listItems = dynamic_cast<ListView*>(pSender);
		UINT selected = listItems->getCurSelectedIndex();
		Widget* listItem = listItems->getItem(selected);	
		char str[12];
		sprintf(str, "%d", listItem->getTag());

		//点击在线玩家列表的时候如果控制开关复选框被勾选则把点击的玩家id填写到控制页面输入框中
		if (_CheckBox_Control->isSelected())
		{
			auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_UserID"));
			TextField_UserID->setString(str);
		}

		//点击在线玩家列表的时候如果控制开关复选框被勾选则把点击的玩家id填写到控制页面输入框中
		if (_CheckBox_Special->isSelected())
		{
			auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_UserID"));
			TextField_UserID->setString(str);
		}
	}

	//受控玩家列表点击回调
	void GameAdmin::controlUsersListEventCallBack(Ref* pSender, ui::ListView::EventType type)
	{
		if (ListView::EventType::ON_SELECTED_ITEM_END != type) return;
		if (!_CheckBox_Control->isSelected()) return;

		ListView* listItems = dynamic_cast<ListView*>(pSender);
		UINT selected = listItems->getCurSelectedIndex();
		Widget* listItem = listItems->getItem(selected);	
		char str[12];
		sprintf(str, "%d", listItem->getTag());
		auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_ControlBg->getChildByName("TextField_UserID"));
		TextField_UserID->setString(str);
	}

	//特殊鱼受控玩家列表点击回调
	void GameAdmin::specialFishUserListEventCallBack(Ref* pSender, ui::ListView::EventType type)
	{
		if (ListView::EventType::ON_SELECTED_ITEM_END != type) return;
		ListView* listItems = dynamic_cast<ListView*>(pSender);
		UINT selected = listItems->getCurSelectedIndex();
		Widget* listItem = listItems->getItem(selected);	
		SpecialUser user = (SpecialUser)_userList.at(selected);

		if (_CheckBox_Special->isSelected())
		{
			char str[12];
			sprintf(str, "%d", listItem->getTag());
			auto TextField_UserID = dynamic_cast<TextField*>(_adminUI.Image_SpecialBg->getChildByName("TextField_UserID"));
			TextField_UserID->setString(str);
		}

		_ListView_Fish->removeAllItems();

		for (int i = 0; i < 4; i++)
		{
			auto fish = user.special_fish_[i];
			if (fish.left_num_ == 0) continue;
			addSpecialFishIten(fish);
		}
	}

	// 复选框回调函数
	void GameAdmin::checkBoxCallback(Ref* pSender, CheckBox::EventType type)
	{
		auto checkBox = dynamic_cast<CheckBox*>(pSender);
		auto name = checkBox->getName();

		//输赢复选框
		auto CheckBox_Win = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Win"));
		auto CheckBox_Lose = dynamic_cast<ui::CheckBox*>(_adminUI.Image_ControlBg->getChildByName("CheckBox_Lose"));

		if (name.compare("CheckBox_Control") == 0)
		{
			bool select = checkBox->isSelected();
			updateControlPageButtonEnabled(select);
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_SWITCH_CONTROL_SIG, &select, sizeof(select));
		}

		if (name.compare("CheckBox_Special") == 0)
		{
			bool select = checkBox->isSelected();
			updateSpecialPageButtonEnabled(select);
			RoomLogic()->sendData(MDM_GM_GAME_NOTIFY, C_S_SWITCH_SPECIAL_SIG, &select, sizeof(select));
		}

		if (name.compare("CheckBox_Win") == 0)
		{
			CheckBox_Win->setTouchEnabled(false);
			CheckBox_Lose->setTouchEnabled(true);
			CheckBox_Lose->setSelected(false);
		}

		if (name.compare("CheckBox_Lose") == 0)
		{
			CheckBox_Lose->setTouchEnabled(false);
			CheckBox_Win->setTouchEnabled(true);
			CheckBox_Win->setSelected(false);
		}
	}

	//特殊鱼控制复选框回调
	void GameAdmin::specialCheckBoxCallBack(Ref* pSender, CheckBox::EventType type)
	{
		auto checkBox = dynamic_cast<CheckBox*>(pSender);
		auto name = checkBox->getName();

		//特殊鱼复选框
		auto CheckBox_Fish1 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish1"));
		auto CheckBox_Fish2 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish2"));
		auto CheckBox_Fish3 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish3"));
		auto CheckBox_Fish4 = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName("CheckBox_Fish4"));

		if (name.compare("CheckBox_Fish1") == 0)
		{
			CheckBox_Fish1->setTouchEnabled(false);
			CheckBox_Fish2->setTouchEnabled(true);
			CheckBox_Fish3->setTouchEnabled(true);
			CheckBox_Fish4->setTouchEnabled(true);
			CheckBox_Fish2->setSelected(false);
			CheckBox_Fish3->setSelected(false);
			CheckBox_Fish4->setSelected(false);
		}

		if (name.compare("CheckBox_Fish2") == 0)
		{
			CheckBox_Fish2->setTouchEnabled(false);
			CheckBox_Fish1->setTouchEnabled(true);
			CheckBox_Fish3->setTouchEnabled(true);
			CheckBox_Fish4->setTouchEnabled(true);
			CheckBox_Fish1->setSelected(false);
			CheckBox_Fish3->setSelected(false);
			CheckBox_Fish4->setSelected(false);
		}

		if (name.compare("CheckBox_Fish3") == 0)
		{
			CheckBox_Fish3->setTouchEnabled(false);
			CheckBox_Fish1->setTouchEnabled(true);
			CheckBox_Fish2->setTouchEnabled(true);
			CheckBox_Fish4->setTouchEnabled(true);
			CheckBox_Fish1->setSelected(false);
			CheckBox_Fish2->setSelected(false);
			CheckBox_Fish4->setSelected(false);
		}

		if (name.compare("CheckBox_Fish4") == 0)
		{
			CheckBox_Fish4->setTouchEnabled(false);
			CheckBox_Fish1->setTouchEnabled(true);
			CheckBox_Fish2->setTouchEnabled(true);
			CheckBox_Fish3->setTouchEnabled(true);
			CheckBox_Fish1->setSelected(false);
			CheckBox_Fish2->setSelected(false);
			CheckBox_Fish3->setSelected(false);
		}
	}

	//显示奖池操作结果
	void GameAdmin::showStockOperateResult(std::string msg)
	{
		auto text = dynamic_cast<Text*>(_adminUI.Image_OnLineBg->getChildByName("Text_Message"));
		text->setVisible(true);
		text->setString(GBKToUtf8(msg.c_str()));
	}

	//设置控制鱼的名字
	void GameAdmin::setSpecialFishName(S_C_FISH_NAME* pName)
	{
		char str[32];
		for (int i = 0; i < 4; i++)
		{
			sprintf(str, "Text_Fish%d", i + 1);
			auto text = dynamic_cast<Text*>(_adminUI.Image_SpecialBg->getChildByName(str));
			sprintf(str, "%s", pName->cFishName[i]);
			text->setString(GBKToUtf8(str));
			text->setTag(pName->byFishKind + i);

			sprintf(str, "CheckBox_Fish%d", i + 1);
			auto checkBox = dynamic_cast<ui::CheckBox*>(_adminUI.Image_SpecialBg->getChildByName(str));
			checkBox->setTag(pName->byFishKind + i);
		}
	}

	//更新复选框信息
	void GameAdmin::setCheckBoxState(S_C_SWITCH_INFO* pSwitch)
	{
		//更新控制页面按钮和复选框状态
		updateControlPageButtonEnabled(pSwitch->control_switch_);
		//更新特殊鱼控制页面按钮和复选框状态
		updateSpecialPageButtonEnabled(pSwitch->special_switch_);
	}
}
