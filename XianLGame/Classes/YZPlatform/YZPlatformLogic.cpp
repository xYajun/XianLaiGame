#include "YZPlatformLogic.h"
#include "YZPlatformNotifyDelegate.h"
#include "YZSocket/YZSocketExport.h"
#include "YZData/YZUserInfoModule.h"
#include "YZData/YZRoomInfoModule.h"
#include "YZData/YZGamesInfoModule.h"
#include "YZRoom/YZRoomExport.h"
#include "YZCommon/YZConverCode.h"
#include "YZCommon/YZLog.h"
#include "GlobalUnits.h"
#include "GamePrompt.h"

#include "HallLayer.h"

namespace YZ
{
	static YZPlatformLogic* sYZPlatformLogic = nullptr;

	YZPlatformLogic* YZPlatformLogic::getInstance()
	{
		if (nullptr == sYZPlatformLogic)
		{
			sYZPlatformLogic = new YZPlatformLogic();
			sYZPlatformLogic->init();
		}
		return sYZPlatformLogic;
	}

	YZPlatformLogic::YZPlatformLogic(void) 
		: _loginResult(false)
		, _heartBeatFrequency(0)
		, _platformConnected(false)
		, _platformPort(INVALID_VALUE)
	{
        memset(&loginResult, 0x00, sizeof(loginResult));
		_socketLogic = new YZSocketLogic(this, "platform");
		_notifyQueue = new YZUIDelegateQueue<IPlatformNotifyDelegate*>();

		//Director::getInstance()->getScheduler()->schedule(schedule_selector(YZPlatformLogic::heartBeat), this, 5.0f, kRepeatForever, 0.0f, false);
	}

	YZPlatformLogic::~YZPlatformLogic(void)
	{
		//Director::getInstance()->getScheduler()->unschedule(schedule_selector(YZPlatformLogic::heartBeat), this);
		YZ_SAFE_DELETE(_notifyQueue);
		YZ_SAFE_DELETE(_socketLogic);
	}

	bool YZPlatformLogic::init() 
	{
		_platformConterServer = false;
		//_platformAddress = PLATFORM_SERVER_ADDRESS;
		_platformAddress = g_GlobalUnits.serverip;
		_platformPort = PLATFORM_SERVER_PORT;
		_socketLogic->openWithIp(_platformAddress.c_str(), _platformPort);
		return connected();
	}

	bool YZPlatformLogic::connectPlatform()
	{
		RoomInfoModule()->clear();
		GamesInfoModule()->clear();
		return _socketLogic->openWithIp(_platformAddress.c_str(), _platformPort);
	}

	bool YZPlatformLogic::closePlatform()
	{
		_platformConnected = false;
		return _socketLogic->close();
	}

	bool YZPlatformLogic::connected() const 
	{ 
		return _socketLogic->connected() && _platformConnected; 
	} 

	bool YZPlatformLogic::login() const
	{
		return _platformLogin;
	}

	bool YZPlatformLogic::loginSuccess() const
	{
		return _loginResult;
	}

	void YZPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{	
		_socketLogic->send(MainID, AssistantID, YZSocketProtocolData::PlatformCheckCode, object, objectSize);
	}

	void YZPlatformLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		_socketLogic->send(MainID, AssistantID, YZSocketProtocolData::PlatformCheckCode, object, objectSize);
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void YZPlatformLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void YZPlatformLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void YZPlatformLogic::addObserver(IPlatformNotifyDelegate* delegate)
	{
		_notifyQueue->addObserver(delegate);
	}

	void YZPlatformLogic::removeObserver(IPlatformNotifyDelegate* delegate)
	{
		_notifyQueue->removeObserver(delegate);
	}

	void YZPlatformLogic::heartBeat(float dt)
	{
		if (connected())
		{
			_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST, YZSocketProtocolData::PlatformCheckCode);
			_heartBeatFrequency++;
		}

		if (_heartBeatFrequency > 5)
		{
			_heartBeatFrequency = 0;
			closePlatform();
		}
	}

    //SET Version
	void YZPlatformLogic::platformLogin(const std::string& name, const std::string& password, UINT uNameID)
	{
		MSG_GP_S_LogonByNameStruct logonByName;
		logonByName.uRoomVer = 4;
		strcpy(logonByName.TML_SN, "EQ4gG6vEUL06ajaGn4EAuXDa662vaeeqL6UdoOQatxuujAlnqovO6VndvXT4Tv0l4a28XGoDxqde4El6XUAXLXe66lg2o6gQN4tlOgeAoV6gulE2jTNneUulE");

		strcpy(logonByName.szName,name.c_str());

		strcpy(logonByName.szMD5Pass, password.c_str());
		logonByName.gsqPs = 5471;
		strcpy(logonByName.szMathineCode, "000266-703028-103FA5-C05024-006BBE-007F50");
		strcpy(logonByName.szCPUID, "612826255");
		strcpy(logonByName.szHardID, "2222222");
		strcpy(logonByName.szIDcardNo, "123456789");
		strcpy(logonByName.szMathineCode, "123456789");
		if (uNameID > 0)
		{
			logonByName.iUserID = uNameID;
		}
		else
		{
			logonByName.iUserID = -1;
		}
		
		strcpy(logonByName.szIDcardNo, "*");
		strcpy(logonByName.szMobileVCode, "*");

		sendData(MDM_GP_LOGON, ASS_GP_LOGON_BY_NAME, &logonByName, sizeof(MSG_GP_S_LogonByNameStruct));
	} 


	void YZPlatformLogic::platformRegister(bool isFastRegister, UINT uid, const std::string& name, const std::string& password, const std::string& nickname, int sex, const std::string& oldpawd,int _headImgId, const std::string szUID)
		{
			if (szUID.length() > 0)
			{
				// 微信注册
				MSG_GP_S_UserRegister userRegister = {0};
				strcpy(userRegister.szAccount, name.c_str());
				strcpy(userRegister.szMD5Pass, password.c_str());
				strcpy(userRegister.szNick, nickname.c_str());
				strcpy(userRegister.szPass, oldpawd.c_str());
				userRegister.szSex = sex;
				strcpy(userRegister.szUID, szUID.c_str());
				userRegister.iHeadID = _headImgId;
				PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_USER_REGISTER, &userRegister, sizeof(MSG_GP_S_UserRegister));
			}
			else if (isFastRegister)
			{
				// 不存在，则进行游客快速注册
				MSG_GP_S_VistorRegister vistorRegister;
				strcpy(vistorRegister.szMD5Pass, password.c_str());
                strcpy(vistorRegister.szUID, szUID.c_str());
                
				PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_VISITOR_REGISTER, &vistorRegister, sizeof(MSG_GP_S_VistorRegister));
			}
			else if (uid > 0)
			{
				//游客绑定
				MSG_GP_S_VisitorBind visitorBind = {0};
				visitorBind.dwUserID = uid;
				strcpy(visitorBind.szAccount, name.c_str());
				strcpy(visitorBind.szOldPass, oldpawd.c_str());
				strcpy(visitorBind.szNewPass, password.c_str());
				strcpy(visitorBind.szNick, nickname.c_str());
				visitorBind.szSex = sex;
				PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_VISITOR_BIND, &visitorBind, sizeof(MSG_GP_S_VisitorBind));
			}
			else
			{
				// 用户注册
				MSG_GP_S_UserRegister userRegister = {0};
				strcpy(userRegister.szAccount, name.c_str());
				strcpy(userRegister.szMD5Pass, password.c_str());
				strcpy(userRegister.szNick, nickname.c_str());
				strcpy(userRegister.szPass, oldpawd.c_str());
				userRegister.szSex = sex;
				userRegister.iHeadID = _headImgId;
				PlatformLogic()->sendData(MDM_GP_REGISTER, ASS_GP_USER_REGISTER, &userRegister, sizeof(MSG_GP_S_UserRegister));
			}

		}

	void YZPlatformLogic::requestGameListInfo()
	{
		GamesInfoModule()->clear();
		sendData(MDM_GP_LIST, ASS_GP_LIST_KIND, 0, 0);
	}

	void YZPlatformLogic::requestRoomListInfo(UINT uKindID, UINT uNameID)
	{
		RoomInfoModule()->clear();
		MSG_GP_SR_GetRoomStruct GetRoomStruct;
		GetRoomStruct.uKindID = uKindID;
		GetRoomStruct.uNameID = uNameID;
		sendData(MDM_GP_LIST, ASS_GP_LIST_ROOM, &GetRoomStruct, sizeof(GetRoomStruct));
	}
	
	void YZPlatformLogic::onConnected(bool connect, emSocketStatus status)
	{
		dispatchMessage([status](IPlatformNotifyDelegate* delegate) -> bool
		{
			delegate->onHandleConnectMessage(SocketStatus_STCCESS == status);
			return false;
		});	
	}

	void YZPlatformLogic::onDisConnect()
	{
		_platformConnected = false;
		dispatchMessage([](IPlatformNotifyDelegate* delegate) -> bool
		{
			delegate->onHandleDisConnect();
			return false;
		});
	}

	void YZPlatformLogic::onSocketMessage(YZSocketMessage* socketMessage)
	{
		// send heartBeat to server
		if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST == socketMessage->messageHead.bAssistantID)
		{
			_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST, YZSocketProtocolData::PlatformCheckCode);
			return;
		}
		
		// main id
		switch (socketMessage->messageHead.bMainID)
		{
		// platform conter server connect
		case MDM_GP_REQURE_GAME_PARA: onConnectConterMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform connect
		case MDM_GP_CONNECT: onConnectMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform login
		case MDM_GP_LOGON: onLoginMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform rooms list
		case MDM_GP_LIST: onRoomListMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform all user count
		case MDM_GP_LOGONUSERS: onUpdateOnLineUserMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform register
		case MDM_GP_REGISTER: onUserRegisterMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform logo upload
		case MDM_GR_USER_LOGO: onUserUploadMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform friend inform
		case MDM_GP_IM: onFriendInformMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;
		// platform broadcast inform
		case MDM_GP_PROP: onBoardcastMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;

		case MDM_GP_NOTICE_MSG: onTransferMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); break;

		default:
			break;
		}
	}

	// 中心服务器连接返回
	void YZPlatformLogic::onConnectConterMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 效验数据
		CHECK_SOCKET_DATA(CenterServerMsg, objectSize, "CenterServerMsg size is error.");

		CenterServerMsg centerServerMsg;
		memcpy(&centerServerMsg, (char*)object, sizeof(CenterServerMsg));

		_socketLogic->close();
		_platformConterServer = true;
		_platformAddress = centerServerMsg.m_strMainserverIPAddr;
		_platformPort = centerServerMsg.m_iMainserverPort;
	}

	// 大厅连接处理
	void YZPlatformLogic::onConnectMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 效验数据
		CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize, "MSG_S_ConnectSuccess size is error.");

		MSG_S_ConnectSuccess* pConnectSuccess = (MSG_S_ConnectSuccess*)object;

		YZSocketProtocolData::PlatformCheckCode = ((UINT)pConnectSuccess->i64CheckCode - SECRET_KEY) / 23;
		YZLOG_WARNING("PlatformCheckCode >> [%d]", YZSocketProtocolData::PlatformCheckCode);

		// 中心服务器数据返回
		if (_platformConterServer == false)
		{
			_socketLogic->send(MDM_GP_REQURE_GAME_PARA, 0, YZSocketProtocolData::PlatformCheckCode, 0, 0);
		}
		else
		{
			_platformConnected = true;

			dispatchMessage([&](IPlatformNotifyDelegate* delegate) -> bool
			{
				delegate->onHandleConnectMessage(_platformConnected);
				return false;
			});	
		}	
	}

	void YZPlatformLogic::onLoginMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch (messageHead->bAssistantID)
		{
		case ASS_GP_LOGON_SUCCESS:
		case ASS_GP_LOGON_ERROR:
			{
				if (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode)
				{
					// 效验数据
					CHECK_SOCKET_DATA(MSG_GP_R_LogonResult, objectSize, "MSG_GP_R_LogonResult size is error.");

					MSG_GP_R_LogonResult* pLogonResult = (MSG_GP_R_LogonResult*)object;
					loginResult = *pLogonResult;
				}

				_platformLogin = (ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode);

                dispatchMessage([&](IPlatformNotifyDelegate* delegate) -> bool
                {
                    delegate->onHandleLoginMessage(_platformLogin, messageHead->bHandleCode);
                    return false;
                });
			}
			break;
		default:
			break;
		}
	}

	void YZPlatformLogic::onRoomListMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (ASS_GP_LIST_KIND == messageHead->bAssistantID) // 游戏类型列表
		{
			ComKindInfo* kingInfo = (ComKindInfo*)object;
			INT Count = objectSize / sizeof(ComKindInfo);
			Count = Count < 0 ? 0 : Count;
			GamesInfoModule()->clear();
			while (Count-- > 0)
			{
				ComKindInfo* tmp = kingInfo++;
				GamesInfoModule()->addGameKind(tmp);
			}
		}
		else if (ASS_GP_LIST_NAME == messageHead->bAssistantID) // 游戏名称列表
		{
			ComNameInfo* nameInfo = (ComNameInfo*)object;
			INT Count = objectSize / sizeof(ComNameInfo);
			Count = Count < 0 ? 0 : Count;

            CCLOG("Count Count : %d" , Count);
            
			while (Count-- > 0)
			{
				ComNameInfo* tmp = nameInfo++;
				GamesInfoModule()->addGameName(tmp);
			}

			if (ERR_GP_LIST_FINISH == messageHead->bHandleCode)
			{
				dispatchMessage([](IPlatformNotifyDelegate* delegate) -> bool
				{
					delegate->onHandleGameListMessage();
					return false;
				});
			}
		}
		else if (ASS_GP_LIST_COUNT == messageHead->bAssistantID)	//大厅房间人数
		{
			if (objectSize < sizeof(DL_GP_RoomListPeoCountStruct)) return;
				
			if (messageHead->bHandleCode == 0) // 大厅房间人数
			{
				DL_GP_RoomListPeoCountStruct* PeoCountStruct = (DL_GP_RoomListPeoCountStruct*)object;
				INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
				Count = Count < 0 ? 0 : Count;
				while (Count-- > 0)
				{
					DL_GP_RoomListPeoCountStruct* tmp = PeoCountStruct++;
					ComRoomInfo* pRoom = RoomInfoModule()->getByRoomID(tmp->uID);
					if (nullptr != pRoom)
					{
						pRoom->uPeopleCount = tmp->uOnLineCount;
						pRoom->uVirtualUser = tmp->uVirtualUser;
					}
				}
			}
			else // 游戏名称人数
			{
				DL_GP_RoomListPeoCountStruct* PeoCountStruct = (DL_GP_RoomListPeoCountStruct*)object;
				INT Count = objectSize / sizeof(DL_GP_RoomListPeoCountStruct);
				Count = Count < 0 ? 0 : Count;
				while (Count-- > 0)
				{
					DL_GP_RoomListPeoCountStruct* tmp = PeoCountStruct++;
					dispatchMessage([&PeoCountStruct](IPlatformNotifyDelegate* delegate) -> bool
					{
						delegate->onHandleGameUserCountMessage(PeoCountStruct);
						return false;
					});
				}
			}
		}
		else if (ASS_GP_LIST_ROOM == messageHead->bAssistantID)
		{
			if (objectSize >= sizeof(MSG_GP_SR_GetRoomStruct))
			{
				MSG_GP_SR_GetRoomStruct* pGetRoomStruct = (MSG_GP_SR_GetRoomStruct*)object;

				// 跳过房间ComRoomInfo数据前包含MSG_GP_SR_GetRoomStruct结构体，要跳过MSG_GP_SR_GetRoomStruct头。
				CHAR* pData = ((CHAR*)object) + sizeof(MSG_GP_SR_GetRoomStruct);
				INT userCount = (objectSize - sizeof(MSG_GP_SR_GetRoomStruct)) / sizeof(ComRoomInfo);
				if (userCount > 0)
				{
					ComRoomInfo* pComRoomInfo = (ComRoomInfo*)(pData);
					INT i = 0;
					while (i++ < userCount)
					{
						RoomInfoModule()->addRoom(pComRoomInfo++);
					}
				}

				if (ERR_GP_LIST_FINISH == messageHead->bHandleCode)
				{
					dispatchMessage([](IPlatformNotifyDelegate* delegate) -> bool
					{
						delegate->onHandleRoomListMessage();
						return false;
					});
				}
			}
		}
	}

	void YZPlatformLogic::onUpdateOnLineUserMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (messageHead->bAssistantID == ASS_GP_LOGONUSERS_COUNT)
		{
			// 效验数据
			CHECK_SOCKET_DATA(ONLINEUSERSCOUNT, objectSize, "ONLINEUSERSCOUNT size is error.");

			ONLINEUSERSCOUNT* ponlineCount = (ONLINEUSERSCOUNT*)object;

			RoomInfoModule()->setOnlineAllCount(ponlineCount->uiLogonPeopCount);

			dispatchMessage([&ponlineCount](IPlatformNotifyDelegate* delegate) -> bool
			{
				delegate->onHandlePlatformUserCountMessage(ponlineCount->uiLogonPeopCount);
				return false;
			});
		}
	}

	void YZPlatformLogic::onUserRegisterMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 效验数据
		//CHECK_SOCKET_DATA(MSG_GP_S_Register, objectSize, "MSG_GP_S_Register size is error.");
		//MSG_GP_S_Register* userRegister = (MSG_GP_S_Register*)object;
		dispatchMessage([&object, &messageHead](IPlatformNotifyDelegate* delegate) -> bool
		{
			delegate->onHandleUserRegisterMessage(object, messageHead->bAssistantID);
			return false;
		});
	}

	
	void YZPlatformLogic::onUserUploadMessage(NetMessageHead* messageHead, void* object, UINT objectSize)
	{
		// 效验数据
		//CHECK_SOCKET_DATA(MSG_GP_S_Register, objectSize, "MSG_GP_S_Register size is error.");
		//MSG_GP_S_Register* userRegister = (MSG_GP_S_Register*)object;
		dispatchMessage([&messageHead, &object,&objectSize](IPlatformNotifyDelegate* delegate) -> bool
		{
			delegate->onHandleLogoUploadMessage(messageHead, object, objectSize);
			return false;
		});
	}

	void YZPlatformLogic::onFriendInformMessage(NetMessageHead* messageHead, void* object, UINT objectSize)
	{

		switch (messageHead->bAssistantID)
		{
		case ASS_IMS_ADDREQUEST:
			{
				MSG_IM_S_ADDREQUEST* data = (MSG_IM_S_ADDREQUEST*)object;
				log("get server Friend inform date !!!");

				auto q_name = data->sUserName;
				_requestUserID = data->dwUserID;
				auto scene = Director::getInstance()->getRunningScene();

				Size winSize = Director::getInstance()->getWinSize();
				f_prompt = static_cast<Layout*>(GUIReader::getInstance()->widgetFromJsonFile("prompt/PromptBoxUI_2.json"));
				f_prompt->setAnchorPoint(Vec2(0.5f, 0.5f));
				f_prompt->setPosition(Vec2(winSize.width / 2, winSize.height / 2));
				scene->addChild(f_prompt);

				auto label_content = static_cast<Text*>(Helper::seekWidgetByName(f_prompt, "Label_Prompt"));
				auto content_str = String::createWithFormat("玩家%s请求添加您为好友！是否同意",q_name)->_string;
				label_content->setString(GBKToUtf8(content_str.c_str()));

				auto btn_confirm = static_cast<Button*>(Helper::seekWidgetByName(f_prompt, "Button_OK"));
				btn_confirm->addTouchEventListener(CC_CALLBACK_2(YZPlatformLogic::onBtnClicked, this));
				btn_confirm->setTag(FRIEND_REQUST_ADD);

				auto btn_cancel = static_cast<Button*>(Helper::seekWidgetByName(f_prompt, "Button_Cancel"));
				btn_cancel->addTouchEventListener(CC_CALLBACK_2(YZPlatformLogic::onBtnClicked, this));
				btn_cancel->setTag(FRIEND_REQUST_CANCEL);
			}
			break;
		case ASS_IMS_ADDREQUESTRESPONSE:
			{
				MSG_IM_S_ADDRESPONSE* data = (MSG_IM_S_ADDRESPONSE*)object;
				auto q_name = data->sRequestedUserName;
				auto content_str = String::createWithFormat("玩家%s已添加您为好友！",q_name)->_string;
				
				if (data->nResponse)
				{
					GamePromptLayer::create()->showPrompt(GBKToUtf8(content_str.c_str()));
				}
				else
				{
					content_str = String::createWithFormat("玩家%s拒绝添加您为好友！",q_name)->_string;
					GamePromptLayer::create()->showPrompt(GBKToUtf8(content_str.c_str()));
				}
				
			}
		case ASS_IMS_MSG:
			{
				MSG_IM_C_MSG* data = (MSG_IM_C_MSG*)object;

				int nYear = 0;
				int nMonth = 0;
				int nDay = 0;
				int nHour=0;  
				int nMinute=0; 
				int nSecond=0;
				#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
								struct timeval now;
								struct tm* time;
								gettimeofday(&now, NULL);
								time = localtime(&now.tv_sec);
								nYear = time->tm_year + 1900;
								nMonth = time->tm_mon + 1;
								nDay = time->tm_mday;
								nHour=time->tm_hour;
								nMinute=time->tm_min;
								nSecond=time->tm_sec;
				#endif
				#if ( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
								struct tm *tm;  
								time_t timep;  
								time(&timep);  
								tm = localtime(&timep);  
								nYear = tm->tm_year + 1900;  
								nMonth = tm->tm_mon + 1;  
								nDay = tm->tm_mday;  
								nHour=tm->tm_hour;  
								nMinute=tm->tm_min;  
								nSecond=tm->tm_sec;
				#endif	

				g_GlobalUnits.m_f_talk_tab.f_id = 0;
				g_GlobalUnits.m_f_talk_tab.f_talks.clear();
				memset(&g_GlobalUnits.m_f_talk,0,sizeof(Talk_Item));

				g_GlobalUnits.m_f_talk.talker = 2;

				//时间1
				char buffer[128];
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "%d%s%d%s%d",nYear,"-",nMonth,"-",nDay);
				memcpy(&g_GlobalUnits.m_f_talk.time1, &buffer, sizeof(buffer));
				
	
				//时间2
				memset(buffer, 0, sizeof(buffer));
				sprintf(buffer, "%d%s%d%s%d",nHour,":",nMinute,":",nSecond);
				memcpy(&g_GlobalUnits.m_f_talk.time2, &buffer, sizeof(buffer));
				
				memcpy(&g_GlobalUnits.m_f_talk.nickName, &data->szUserName, sizeof(data->szUserName));

				log("the content word is %s",data->szMsg);
				memcpy(&g_GlobalUnits.m_f_talk.talkMsg, data->szMsg, sizeof(data->szMsg));
				

				g_GlobalUnits.mf_talks_tab.push_back(g_GlobalUnits.m_f_talk);

				g_GlobalUnits.m_f_talk_tab.f_id = data->dwUserID;
				for (auto& e : g_GlobalUnits.mf_talks_tab)
				{
					g_GlobalUnits.m_f_talk_tab.f_talks.push_back(e);
				}
				
				g_GlobalUnits.m_friend_talks_tab.push_back(g_GlobalUnits.m_f_talk_tab);


				///////////////////////////
				auto scene = Director::getInstance()->getRunningScene();
				if (scene->getChildByTag(110011))
				{
					/*if (scene->getChildByTag(110011)->getChildByTag(110012))
					{
						FriendLayer* f_layer = (FriendLayer*)scene->getChildByTag(110011)->getChildByTag(110012);
						if (f_layer->getChildByTag(3007))
						{
							auto fw_layer = f_layer->getChildByTag(3007);
							f_layer->loadFriendWhisperList();
						}
					}*/
					
				}

			}
		
			break;
		default:
			break;
		}
	}


	void YZPlatformLogic::onBoardcastMessage(NetMessageHead* messageHead, void* object, UINT objectSize)
	{
		switch (messageHead->bAssistantID)
		{
		case ASS_PROP_BIG_BOARDCASE:
			{
				_TAG_BOARDCAST* data = (_TAG_BOARDCAST*)object;
				log("the big trumpet  information  !~!!!!!!!!!!");
				auto scene = Director::getInstance()->getRunningScene();
				if (scene->getChildByTag(110011))
				{
					auto layer = (HallLayer*)scene->getChildByTag(110011);
					char msgStr[256];
					strcpy(msgStr,data->szMessage);
					(layer->label_up_msg)->setString(GBKToUtf8(msgStr));
					layer->setMsgRollAction();
				}
			}

			break;
		default:
			break;
		}
	}


	void YZPlatformLogic::onTransferMessage(NetMessageHead* messageHead, void* object, UINT objectSize)
	{
		switch (messageHead->bAssistantID)
		{
		case ASS_GR_TRANSFER_MONEY:
			{
				TMSG_GP_BankTransfer * transferInfo = (TMSG_GP_BankTransfer*)(object);
				auto q_name = transferInfo->szNickName;
			    auto content_str = String::createWithFormat("玩家%s转给你%lld金币！",q_name,transferInfo->i64Money)->_string;
				GamePromptLayer::create()->showPrompt(GBKToUtf8(content_str.c_str())); break;
			}

			break;
		default:
			break;
		}
    }

	// btn call back 

	void YZPlatformLogic::onBtnClicked(Ref* pSender, Widget::TouchEventType type)
	{
		Button* btn = (Button*)pSender;

		if (type == Widget::TouchEventType::ENDED && btn)
		{
			log("tag = %d", btn->getTag());
			switch (btn->getTag())
			{
			case FRIEND_REQUST_ADD:
				{

					MSG_IM_S_ADDRESPONSE answer_request;
					memset(&answer_request,0,sizeof(MSG_IM_S_ADDRESPONSE));
					answer_request.dwUserID =  _requestUserID;
					answer_request.dwRequestedUserID =  PlatformLogic()->loginResult.dwUserID;
					answer_request.nResponse = 6;
					PlatformLogic()->sendData(MDM_GP_IM, ASS_IMC_ADDREQUESTRESPONSE, &answer_request, sizeof(answer_request));

					f_prompt->removeFromParentAndCleanup(true);
				}
				break;
			case FRIEND_REQUST_CANCEL:
				{
					MSG_IM_S_ADDRESPONSE answer_request;
					memset(&answer_request,0,sizeof(MSG_IM_S_ADDRESPONSE));
					answer_request.dwUserID =  _requestUserID;
					answer_request.dwRequestedUserID =  PlatformLogic()->loginResult.dwUserID;
					answer_request.nResponse = 7;
					PlatformLogic()->sendData(MDM_GP_IM, ASS_IMC_ADDREQUESTRESPONSE, &answer_request, sizeof(answer_request));

					f_prompt->removeFromParentAndCleanup(true);
				}
				break;
			default:
				break;
			}
		}
	}

}




