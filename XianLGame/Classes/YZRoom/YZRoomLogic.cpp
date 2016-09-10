#include "YZRoomLogic.h"
#include "YZRoomMessageDelegate.h"
#include "YZData/YZRoomInfoModule.h"
#include "YZData/YZUserInfoModule.h"
#include "YZPlatform/YZPlatformExport.h"
#include "YZSocket/YZSocketProtocolData.h"
#include "YZSocket/YZSocketLogic.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include "YZGame/YZGameMessageDelegate.h"
#include "YZCommon/YZConverCode.h"
#include "GamePrompt.h"
#include "GlobalUnits.h"
#include "Util/JzLog.h"

namespace YZ
{
	static YZRoomLogic* sYZRoomLogic = nullptr;

	YZRoomLogic* YZRoomLogic::getInstance()
	{
		if (nullptr == sYZRoomLogic)
		{
			sYZRoomLogic = new YZRoomLogic();
			sYZRoomLogic->init();
		}
		return sYZRoomLogic;
	}

	YZRoomLogic::YZRoomLogic(void)
		: _gameRoomRule(0)
		, _serverPort(-1)
		, _roomConnected(false)
		, _roomLogin(false)
		, _selectedRoom(nullptr)
	{
        _CheckTimes = 0;
        memset(&loginResult, 0x00, sizeof(loginResult));
		_socketLogic = new YZSocketLogic(this, "game");
		_gameNotifyQueue = new YZUIDelegateQueue<IGameMessageDelegate*>();
		_roomNotifyQueue = new YZUIDelegateQueue<IRoomMessageDelegate*>();
	}

	YZRoomLogic::~YZRoomLogic(void)
	{
		YZ_SAFE_DELETE(_gameNotifyQueue);
		YZ_SAFE_DELETE(_roomNotifyQueue);
		YZ_SAFE_DELETE(_socketLogic);
	}

	bool YZRoomLogic::init() 
	{
		return true;
	}

	bool YZRoomLogic::connectRoom(const CHAR* ip, INT port)
	{
        Director::getInstance()->getScheduler()->schedule(schedule_selector(YZRoomLogic::update), this, 16.0f, kRepeatForever, 0.0f, false);

		_serverIp = ip;
		_serverPort = port;
		return _socketLogic->openWithIp(_serverIp.c_str(), _serverPort);
	}

	bool YZRoomLogic::closeRoom()
	{
		_roomConnected =  false;
		return _socketLogic->close();
	}

	bool YZRoomLogic::connected() const 
	{ 
		return _roomConnected && _socketLogic->connected(); 
	} 

	bool YZRoomLogic::login() const
	{
		return _roomLogin;
	}

	INT YZRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{	
		return _socketLogic->send(MainID, AssistantID, YZSocketProtocolData::GameCheckCode, object, objectSize);
	}

	void YZRoomLogic::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector)
	{
		_socketLogic->send(MainID, AssistantID, YZSocketProtocolData::GameCheckCode, object, objectSize);
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void YZRoomLogic::addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector)
	{
		_socketLogic->addEventSelector(MainID, AssistantID, selector);
	}

	void YZRoomLogic::removeEventSelector(UINT MainID, UINT AssistantID)
	{
		_socketLogic->removeEventSelector(MainID, AssistantID);
	}

	void YZRoomLogic::addObserver(IGameMessageDelegate* delegate)
	{
		_gameNotifyQueue->addObserver(delegate);
	}

	void YZRoomLogic::removeObserver(IGameMessageDelegate* delegate)
	{
		_gameNotifyQueue->removeObserver(delegate);
	}

	void YZRoomLogic::addRoomObserver(IRoomMessageDelegate* delegate)
	{
		_roomNotifyQueue->addObserver(delegate);
	}

	void YZRoomLogic::removeRoomObserver(IRoomMessageDelegate* delegate)
	{
		_roomNotifyQueue->removeObserver(delegate);
	}

	void YZRoomLogic::roomLogin(UINT uGameID)
	{
		MSG_GR_S_RoomLogon RoomLogon = {0};
		RoomLogon.uNameID = uGameID;
		RoomLogon.dwUserID = PlatformLogic()->loginResult.dwUserID;
		strcpy(RoomLogon.szMD5Pass, PlatformLogic()->loginResult.szMD5Pass);
		sendData(MDM_GR_LOGON, ASS_GR_LOGON_BY_ID, &RoomLogon, sizeof(RoomLogon));
	}

	void YZRoomLogic::onConnected(bool connect, emSocketStatus status)
	{
		_roomConnected = connect;
		// 分发事件
		dispatchFrameMessage([status](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->onConnectMessage(SocketStatus_STCCESS == status);
			return false;
		});
	}

	void YZRoomLogic::onDisConnect()
	{
		_roomLogin = false;

		_roomConnected = false;

		// 分发消息到房间
		dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->onDisConnect();
			return false;
		});

		// 分发消息到游戏
		dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool
		{
			delegate->onDisConnect();
			return false;
		});
	}
    
    void YZRoomLogic::update(float dt){
        _CheckTimes++;
        if (_CheckTimes >= 3) {
            Director::getInstance()->getScheduler()->unschedule(schedule_selector(YZRoomLogic::update), this);
            onDisConnect();
        }
    }

	void YZRoomLogic::onSocketMessage(YZSocketMessage* socketMessage)
	{
        _CheckTimes = 0;
		// 心跳
		if (MDM_CONNECT == socketMessage->messageHead.bMainID && ASS_NET_TEST == socketMessage->messageHead.bAssistantID)
		{
			_socketLogic->heartBeat(MDM_CONNECT, ASS_NET_TEST, YZSocketProtocolData::GameCheckCode);
			return;
		}

		switch (socketMessage->messageHead.bMainID)
		{
			// 连接成功
		case MDM_GR_CONNECT: { onConnectMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
			// 登录消息
		case MDM_GR_LOGON: { onLoginMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
			// 用户列表
		case MDM_GR_USER_LIST: { onUserListMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
			// 用户动作 
		case MDM_GR_USER_ACTION: { onUserActionMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
			// 房间消息
		case MDM_GR_ROOM: { onRoomMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;	
			// 比赛消息
		case MDM_GR_MATCH_INFO: { onMatchMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;		
			// 框架消息
		case MDM_GM_GAME_FRAME:	{ onGameServerFrameMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
			// 游戏消息
		case MDM_GM_GAME_NOTIFY:	
			{
				dispatchGameMessage([&socketMessage](IGameMessageDelegate* delegate) -> bool 
				{
					delegate->onGameMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize);
					return true;
				});
			}
			break;
        case MDM_GR_PROP: { onRoomMessage(&socketMessage->messageHead, socketMessage->object, socketMessage->objectSize); } break;
                
		default:
			break;
		}
	}

	void YZRoomLogic::onGameServerFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		//发送游戏信息(1)
		if (ASS_GM_GAME_INFO == messageHead->bAssistantID)
		{
			// 效验数据
			// CHECK_SOCKET_DATA(MSG_GM_S_GameInfo, objectSize, "MSG_GM_S_GameInfo size is error.");

			MSG_GM_S_GameInfo* pGameInfo = (MSG_GM_S_GameInfo*)object;

			// 显示信息
			if (messageHead->uMessageSize > (sizeof(MSG_GM_S_GameInfo) - sizeof(pGameInfo->szMessage)))
			{
				dispatchGameMessage([&pGameInfo](IGameMessageDelegate* delegate) -> bool 
				{
					delegate->onGameInfoMessage(pGameInfo);
					return true;
				});
			}
		}
		// 发送游戏状态(2)
		else if (ASS_GM_GAME_STATION == messageHead->bAssistantID)
		{
			dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameStationMessage(object, objectSize);
				return true;
			});
		}
		// 普通聊天
		else if (ASS_GM_NORMAL_TALK == messageHead->bAssistantID)	
		{
			dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
			{
				(dynamic_cast<IGameChartMessageDelegate*>(delegate))->onUserChatMessage(object, objectSize);
				return true;
			});
		}
	}

	void YZRoomLogic::onMatchMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 积分
		if (ASS_GR_CUR_SCORE == messageHead->bAssistantID)
		{
			USERCURSCOREREP *pData = (USERCURSCOREREP*)object;

			MSG_GM_S_GameInfo* pGameInfo = (MSG_GM_S_GameInfo*)object;

		}
	}

	//房间消息处理
	void YZRoomLogic::onRoomMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if (ASS_GR_GAME_BEGIN == messageHead->bAssistantID)			// 游戏开始
		{
			BYTE deskNo = (BYTE)messageHead->bHandleCode;
			// 更新用户状态
			UserInfoModule()->transform(deskNo, [](UserInfoStruct* user, INT index) 
			{
				user->bUserState = USER_PLAY_GAME;
			});

			dispatchGameMessage([&messageHead](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameStartMessage((BYTE)messageHead->bHandleCode);
				return false;
			});
		}
		else if (ASS_GR_NORMAL_TALK == messageHead->bAssistantID)
		{
			log("size = %d", sizeof(MSG_GR_RS_NormalTalk));
			MSG_GR_RS_NormalTalk* normalTalk = (MSG_GR_RS_NormalTalk *)object;
			log("size = %d", normalTalk->iLength);

			dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
			{
				(dynamic_cast<IGameChartMessageDelegate*>(delegate))->onGameChatMessage(object, objectSize);
				return false;
			});

		}
		else if (ASS_GR_USER_POINT == messageHead->bAssistantID)	 // 游戏结算
		{
			if (10 == messageHead->bHandleCode)			// 同步金币
			{
				// 效验数据
				CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize, "MSG_GR_S_RefalshMoney size is error.");

				MSG_GR_S_RefalshMoney * pReflashMoney = (MSG_GR_S_RefalshMoney*)object;
                
				// 处理数据
				UserInfoStruct * pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
				if (pUserItem== nullptr) return;
                
                pUserItem->i64Money = pReflashMoney->i64Money;
                
                // 更新自己数据
                if (pReflashMoney->dwUserID == PlatformLogic()->loginResult.dwUserID)
                {
                    RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
                    PlatformLogic()->loginResult.i64Money += pReflashMoney->i64Money;
                }
			}
			else if (11 == messageHead->bHandleCode)	// 同步经验值
			{
				// 效验数据
				CHECK_SOCKET_DATA(MSG_GR_S_RefalshMoney, objectSize, "MSG_GR_S_RefalshMoney size is error.");

				MSG_GR_S_RefalshMoney * pReflashMoney = (MSG_GR_S_RefalshMoney*)object;

				// 处理数据
				UserInfoStruct * pUserItem = UserInfoModule()->findUser(pReflashMoney->dwUserID);
				if (pUserItem== nullptr) return;

				pUserItem->dwPoint = pReflashMoney->i64Money;

				// 更新自己数据
				if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
				{
					RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
				}

			}
			else if (0 == messageHead->bHandleCode)		// 同步经验值
			{
				// 效验数据
				CHECK_SOCKET_DATA(MSG_GR_R_UserPoint, objectSize, "MSG_GR_R_UserPoint size is error.");

				MSG_GR_R_UserPoint * pUserPoint = (MSG_GR_R_UserPoint*)object;
			
				UserInfoStruct * pUserItem = UserInfoModule()->findUser(pUserPoint->dwUserID);
				if (pUserItem== nullptr) return;

				// 更新用户信息
				pUserItem->dwPoint += pUserPoint->dwPoint;							//用户经验值
				pUserItem->i64Money += pUserPoint->dwMoney;							//用户金币
				//pUserItem->dwSend += pUserPoint->dwSend;							//赠送
				pUserItem->uWinCount += pUserPoint->bWinCount;						//胜局
				pUserItem->uLostCount += pUserPoint->bLostCount;					//输局
				pUserItem->uMidCount += pUserPoint->bMidCount;						//平局
				pUserItem->uCutCount += pUserPoint->bCutCount;						//逃局

				//pUserItem->strAutoSendMoney += pUserPoint->strAutoSendMoney;      //添加自动赠送 2013-01-29 wangzhitu
				
                // 更新自己数据
				if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
				{
					RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
					PlatformLogic()->loginResult.i64Money += pUserPoint->dwMoney;
				}
			}
			dispatchFrameMessage([&object, objectSize](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->onGamePointMessage(object, objectSize);
				return false;
			});
			
			dispatchGameMessage([&object, objectSize](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGamePointMessage(object, objectSize);
				return false;
			});
		}
		else if (ASS_GR_GAME_END == messageHead->bAssistantID)		 // 游戏结束
		{	
			BYTE deskIndex = (BYTE)messageHead->bHandleCode;
			
			dispatchGameMessage([deskIndex](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameEndMessage(deskIndex);
				return false;
			});

			if ((_gameRoomRule & GRR_CONTEST) || (_gameRoomRule & GRR_QUEUE_GAME))
			{
				std::vector<UserInfoStruct*> users;
				UserInfoModule()->findDeskUsers(deskIndex, users);
				std::for_each(users.begin(), users.end(), [](UserInfoStruct* user)
				{
					user->bDeskStation = INVALID_DESKSTATION;
					user->bDeskNO = INVALID_DESKNO;
					user->bUserState = USER_LOOK_STATE;
				});
			}
		}
		else if (ASS_GR_USER_AGREE == messageHead->bAssistantID)     // 用户同意
		{
			// 效验数据
			CHECK_SOCKET_DATA(MSG_GR_R_UserAgree, objectSize, "MSG_GR_R_UserAgree size is error.");

			MSG_GR_R_UserAgree * pUserAgree = (MSG_GR_R_UserAgree*)object;

			UserInfoStruct * pUserItem = UserInfoModule()->findUser(pUserAgree->bDeskNO, pUserAgree->bDeskStation);
			if (nullptr != pUserItem)
			{
				if (pUserAgree->bAgreeGame == 1)
				{
					pUserItem->bUserState = USER_ARGEE;
				}
				
			}

			dispatchGameMessage([&pUserAgree](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onUserAgreeMessage(pUserAgree);
				return false;
			});
		} 
		else if (ASS_GR_INSTANT_UPDATE == messageHead->bAssistantID)	 // 即时更新分数金币
		{
			// 效验数据
			CHECK_SOCKET_DATA(MSG_GR_R_InstantUpdate, objectSize, "MSG_GR_R_InstantUpdate size is error.");

			MSG_GR_R_InstantUpdate * pInstantUpdate = (MSG_GR_R_InstantUpdate*)object;
			
			// 更新数据
			UserInfoStruct * pUserItem = UserInfoModule()->findUser(pInstantUpdate->dwUserID);
			if (nullptr != pUserItem)
			{
				pUserItem->i64Money += pInstantUpdate->dwMoney;
				pUserItem->dwPoint += pInstantUpdate->dwPoint;
			}

			// 更新自己数据
			if (pUserItem->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				RoomLogic()->loginResult.pUserInfoStruct = *pUserItem;
				PlatformLogic()->loginResult.i64Money += pInstantUpdate->dwMoney;
			}
		}
		else if (ASS_GR_USER_CONTEST == messageHead->bAssistantID)	 // 用户比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，排名有更改
		{
			CHECK_SOCKET_DATA(MSG_GR_ContestChange, objectSize, "MSG_GR_ContestChange size of error!");
			MSG_GR_ContestChange* contestChange = (MSG_GR_ContestChange*)object;
			
			UserInfoStruct* user = UserInfoModule()->findUser(contestChange->dwUserID);
			if (nullptr != user)
			{
				user->iRankNum = contestChange->iRankNum;			// 排行名次
				user->i64ContestScore = contestChange->i64ContestScore;
				//g_GlobalUnits.matchRank = contestChange->iRankNum;
			//	user->iRemainPeople = contestChange->bRemainPeople;	// 比赛中还剩下的人数
			}

			dispatchGameMessage([contestChange](IGameMessageDelegate* delegate) -> bool
			{
				delegate->onGameUserContset(contestChange);
				return false;
			});
		}
		else if (ASS_GR_CONTEST_GAMEOVER == messageHead->bAssistantID) // 比赛结束
		{
			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_AWARD_RESULT, objectSize, "NET_ROOM_CONTEST_AWARD_RESULT size of error!");
			NET_ROOM_CONTEST_AWARD_RESULT* contestAward = (NET_ROOM_CONTEST_AWARD_RESULT*)object;

			dispatchGameMessage([contestAward](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestOver(contestAward);
				return false;
			});	
		}
		else if (ASS_GR_CONTEST_KICK == messageHead->bAssistantID)	 // 用户被淘汰
		{
			dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestKick();
				return false;
			});	
		}
		else if (ASS_GR_CONTEST_WAIT_GAMEOVER == messageHead->bAssistantID)	 // 比赛结束，但是有用户还在打最后一局，通知已打完的用户等待排名
		{
			dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestWaitOver();
				return false;
			});	
		}
		else if (ASS_GR_INIT_CONTEST == messageHead->bAssistantID)		// 比赛开始，初始化比赛信息，发送NET_ROOM_CONTEST_CHANGE_RESULT，初始化排名信息
		{
			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_CHANGE_RESULT, objectSize, "NET_ROOM_CONTEST_CHANGE_RESULT size of error!");

			NET_ROOM_CONTEST_CHANGE_RESULT* contestChange = (NET_ROOM_CONTEST_CHANGE_RESULT*)object;

			dispatchGameMessage([contestChange](IGameMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestInit(contestChange);
				return false;
			});	
		}
		else if (ASS_GR_CONTEST_PEOPLE == messageHead->bAssistantID)	 // 获取报名数量，登陆完成后服务端主动发送NET_ROOM_CONTEST_PEOPLE_RESUL
		{
			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_PEOPLE_RESULT, objectSize, "NET_ROOM_CONTEST_PEOPLE_RESULT size of error!");
			NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople = (NET_ROOM_CONTEST_PEOPLE_RESULT*)object;

			// 分发房间数据
			dispatchFrameMessage([contestPeople](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestPeople(contestPeople);
				return false;
			});
		}
		else if (ASS_GR_CONTEST_RECORD == messageHead->bAssistantID)	 // 个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
		{
			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_RECORD_RESULT, objectSize, "NET_ROOM_CONTEST_RECORD_RESULT size of error!");
			NET_ROOM_CONTEST_RECORD_RESULT* contestRecord = (NET_ROOM_CONTEST_RECORD_RESULT*)object;

			// 分发房间数据
			dispatchFrameMessage([contestRecord](IRoomMessageDelegate* delegate) -> bool 
			{
				delegate->onGameContestRecord(contestRecord);
				return false;
			});
		}
		else if (ASS_GR_CONTEST_AWARDS == messageHead->bAssistantID)	 // 个人参赛纪录，登陆完成后服务端主动发送NET_ROOM_CONTEST_RECORD_RESULT
		{
			CHECK_SOCKET_DATA(NET_ROOM_CONTEST_AWARD_RESULT, objectSize, "NET_ROOM_CONTEST_RECORD_RESULT size of error!");

			UINT count = objectSize / sizeof(NET_ROOM_CONTEST_AWARD_RESULT);
			if (count > 0)
			{
				NET_ROOM_CONTEST_AWARD_RESULT* contestRecord = (NET_ROOM_CONTEST_AWARD_RESULT*)object;

				std::vector<NET_ROOM_CONTEST_AWARD_RESULT*> results(count);
				while (count > 0)
				{
					NET_ROOM_CONTEST_AWARD_RESULT* tmp = contestRecord++;
					results[count - 1] = tmp;
					count--;
				}

				// 分发房间数据
				dispatchFrameMessage([&results](IRoomMessageDelegate* delegate) -> bool 
				{
					delegate->onGameContestAwards(&results);
					return false;
				});
			}
        }else if (ASS_GR_OPERAT_STATUS == messageHead->bAssistantID)	 // 获取可操作状态
        {
            CHECK_SOCKET_DATA(MSG_GR_BACKROOM_STATUS, objectSize, "MSG_GR_BACKROOM_STATUS size of error!");
            
            UINT count = objectSize / sizeof(MSG_GR_BACKROOM_STATUS);
            if (count > 0)
            {
                MSG_GR_BACKROOM_STATUS* data = (MSG_GR_BACKROOM_STATUS*)object;
                
                dispatchFrameMessage([data](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onBackRoomMessage(data);
                                         return false;
                                     });
            }
        }else if (ASS_PROP_ROOMCARD_USE == messageHead->bAssistantID)	 // 使用房卡
        {
            CHECK_SOCKET_DATA(MSG_PROP_S_OP_ROOMCARD, objectSize, "MSG_PROP_S_OP_ROOMCARD size of error!");
            
            MSG_PROP_S_OP_ROOMCARD* data = (MSG_PROP_S_OP_ROOMCARD*)object;
            
            dispatchFrameMessage([data](IRoomMessageDelegate* delegate) -> bool
                                 {
                                     delegate->onUseRoomCardMessage(data);
                                     return false;
                                 });

        }else if (ASS_GR_CREATE_BACKROOM == messageHead->bAssistantID)	 // 创建房间
        {
            CHECK_SOCKET_DATA(MSG_BACKROOM_INFO, objectSize, "MSG_BACKROOM_INFO size of error!");
            
            UINT count = objectSize / sizeof(MSG_BACKROOM_INFO);
            BYTE code = (BYTE)messageHead->bHandleCode;
            YZLOG("创建密室结果 : %d " , code);
            if (count > 0 && code == 0)
            {
                MSG_BACKROOM_INFO* data = (MSG_BACKROOM_INFO*)object;
                
                dispatchFrameMessage([data](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onCreateBackRoomMessage(data);
                                         return false;
                                     });
            }else{
                CHAR buf[512];
                sprintf(buf, ("创建房间失败 [ %d ] 请稍后再试..."), code);
                YZLOG_ERROR("%s", GBKToUtf8(buf));
                GamePromptLayer::create()->showPrompt(GBKToUtf8(buf));
            }
        }else if (ASS_GR_QUERY_BACKROOM == messageHead->bAssistantID)	 // 查询房间信息
        {
            CHECK_SOCKET_DATA(MSG_QueryBACKROOM_RSP_INFO, objectSize, "MSG_BACKROOM_INFO size of error!");
            
            UINT count = objectSize / sizeof(MSG_QueryBACKROOM_RSP_INFO);
            
            BYTE code = (BYTE)messageHead->bHandleCode;
            YZLOG("创建密室结果 : %d " , code);
            if (count > 0)
            {
                MSG_QueryBACKROOM_RSP_INFO* data = (MSG_QueryBACKROOM_RSP_INFO*)object;
                
                dispatchFrameMessage([data , code](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onQueryBakcRoomMessage(data , code);
                                         return false;
                                     });
            }else{
                CHAR buf[512];
                sprintf(buf, ("加入房间失败 [ %d ] 请稍后再试..."), code);
                YZLOG_ERROR("%s", GBKToUtf8(buf));
                GamePromptLayer::create()->showPrompt(GBKToUtf8(buf));
            }
            
        }else if (ASS_PROP_ROOMCARD_TYPEINFO == messageHead->bAssistantID)	 // 查询房卡类型
        {
            CHECK_SOCKET_DATA(MSG_PROP_S_KIND_ROOMCARD, objectSize, "MSG_PROP_S_QUERY_ROOMCARD size of error!");
            
            BYTE code = (BYTE)messageHead->bHandleCode;
            
            UINT count = objectSize / sizeof(MSG_PROP_S_KIND_ROOMCARD);
            if (count > 0)
            {
                MSG_PROP_S_KIND_ROOMCARD* data = (MSG_PROP_S_KIND_ROOMCARD*)object;
                
                dispatchFrameMessage([data,code](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onQueryPropKindMessage(data, code);
                                         return false;
                                     });
            }
        }else if (ASS_PROP_ROOMCARD_GETINFO == messageHead->bAssistantID)	 // 查询房卡
        {
            CHECK_SOCKET_DATA(MSG_PROP_S_QUERY_ROOMCARD, objectSize, "MSG_PROP_S_QUERY_ROOMCARD size of error!");
            
            BYTE code = (BYTE)messageHead->bHandleCode;
            
            UINT count = objectSize / sizeof(MSG_PROP_S_QUERY_ROOMCARD);
            if (count > 0)
            {
                MSG_PROP_S_QUERY_ROOMCARD* data = (MSG_PROP_S_QUERY_ROOMCARD*)object;
                
                dispatchFrameMessage([data,code](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onQueryPropRoomCardInfo(data, code);
                                         return false;
                                     });
            }
        }else if (ASS_PROP_ROOMCARD_BUY == messageHead->bAssistantID)	 // 购买房卡
        {
            CHECK_SOCKET_DATA(MSG_PROP_S_BUY_ROOMCARD, objectSize, "MSG_PROP_S_BUY_ROOMCARD size of error!");
            
            UINT count = objectSize / sizeof(MSG_PROP_S_BUY_ROOMCARD);
            if (count > 0)
            {
                MSG_PROP_S_BUY_ROOMCARD* data = (MSG_PROP_S_BUY_ROOMCARD*)object;
                
                dispatchFrameMessage([data](IRoomMessageDelegate* delegate) -> bool
                                     {
                                         delegate->onBuyRoomCardMessage(data);
                                         return false;
                                     });
            }
        }
    }
    
    // 房间连接处理
    void YZRoomLogic::onConnectMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		// 参数校验
		if (messageHead->bAssistantID != ASS_GR_CONNECT_SUCCESS) return;

		CHECK_SOCKET_DATA(MSG_S_ConnectSuccess, objectSize, "MSG_S_ConnectSuccess size is error.");
		
		MSG_S_ConnectSuccess* pConnectSuccess = (MSG_S_ConnectSuccess*)object;

		YZSocketProtocolData::GameCheckCode = (UINT)(pConnectSuccess->i64CheckCode - SECRET_KEY) / 23;

		_roomConnected = true;

		// 分发事件
		dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
		{
			delegate->onConnectMessage(true);
			return false;
		});
	}

	// 房间登录处理
	void YZRoomLogic::onLoginMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch (messageHead->bAssistantID)
		{	
		case ASS_GP_LOGON_ALLO_PART: // 异地登录
			{
				
			}
			break;
		case ASS_GP_LOGON_LOCK_VALID: // 设备锁定
			{
			}
			break;	
		case ASS_GR_LOGON_SUCCESS:	// 房间登录成功  (1)
			{
				// 数据校验
				CHECK_SOCKET_DATA(MSG_GR_R_LogonResult, objectSize, "MSG_GR_R_LogonResult size is error.");

				MSG_GR_R_LogonResult * pLogonResult = (MSG_GR_R_LogonResult*)object;

				loginResult = *pLogonResult;
				_roomLogin = ERR_GP_LOGON_SUCCESS == messageHead->bHandleCode;

				// 分发事件
				dispatchFrameMessage([&messageHead](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onLoginMessage(true, messageHead->bHandleCode);
					return false;
				});
			}
			break;
		case ASS_GR_LOGON_ERROR:	// 登录失败
			{
				// 分发事件
				dispatchFrameMessage([&messageHead](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onLoginMessage(false, messageHead->bHandleCode);
					return false;
				});
			}
			break;
		case ASS_GR_SEND_FINISH:	// 房间登录完成(6)
			{
				// 分发事件
				dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onLoginFinishMessage();

					return false;
				});	
			}
			break;
                
		default:
			break;
		}
	}

	// 用户列表处理
	void YZRoomLogic::onUserListMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch (messageHead->bAssistantID)
		{
		case ASS_GR_ONLINE_USER:	// 在线用户信息(2)
		case ASS_GR_NETCUT_USER:	// 断线用户信息(3)
			{
				UINT userCount = objectSize / sizeof(UserInfoStruct);
				UserInfoStruct * pUserInfo = (UserInfoStruct *)object;
				while (userCount > 0)
				{
					UserInfoModule()->updateUser(pUserInfo);
					pUserInfo++;
					userCount--;
				}
			}
			break;
		case ASS_GR_DESK_STATION:	// 桌子信息(4)
			{
				// 数据校验
				CHECK_SOCKET_DATA(MSG_GR_DeskStation, objectSize, "MSG_GR_DeskStation size is error.");
				//messageHead->bHandleCode.
				MSG_GR_DeskStation * pDeskStation = (MSG_GR_DeskStation *)object;
				deskStation = *pDeskStation;

				
				// 分发房间消息
				dispatchFrameMessage([messageHead,object,objectSize](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onRoomFrameMessage(messageHead,object,objectSize);
					return false;
				});	

				/*		dispatchFrameMessage([&pDeskStation](IRoomMessageDelegate* delegate) -> bool
						{
						delegate->onGameDeskMessage(pDeskStation);
						return false;
						});*/
			}
			break;
		default:
			break;
		}	
	}

	// 用户动作处理
	void YZRoomLogic::onUserActionMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		switch (messageHead->bAssistantID)
		{		
		case ASS_GR_QUEUE_USER_SIT:
			{
				int* pData = (int*)object;

				int _DeskIndex = *pData;

				UINT count  = (objectSize - 4) / sizeof(int);

				std::vector<QUEUE_USER_SIT_RESULT *> queueUsers;

				QUEUE_USER_SIT_RESULT temp[4];
				int i = 0;

				while (count > 0)
				{
					count--;
					pData++;
					temp[count].bDeskStation = _DeskIndex;
					temp[count].dwUserID = *pData;				
					UserInfoStruct* userInfo = UserInfoModule()->findUser(*pData);
					if (nullptr != userInfo)
					{
						userInfo->bDeskNO = _DeskIndex;
						userInfo->bDeskStation = i;
						userInfo->bUserState = USER_ARGEE;
					}
					i++;
					queueUsers.push_back(&temp[count]);
				}

				// 分发游戏消息
				dispatchGameMessage([&_DeskIndex, &queueUsers](IGameMessageDelegate* delegate) -> bool
				{
					delegate->onQueueUserSitMessage(_DeskIndex, queueUsers);
					return false;
				});

				// 分发房间消息
				dispatchFrameMessage([&_DeskIndex, &queueUsers](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onQueueUserSitMessage(_DeskIndex, queueUsers);
					return false;
				});
			} break;
		case ASS_GR_USER_COME:	//用户进入房间(5)
			{
				// 数据校验
				CHECK_SOCKET_DATA(MSG_GR_R_UserCome, objectSize, "MSG_GR_R_UserCome size is error.");

				MSG_GR_R_UserCome* pUserCome = (MSG_GR_R_UserCome*)object;

				UserInfoModule()->updateUser(&pUserCome->pUserInfoStruct);
				// 分发房间消息
				dispatchFrameMessage([&pUserCome](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onUserComeMessage(&pUserCome->pUserInfoStruct);
					return false;
				});	
			}
			break;
		case ASS_GR_USER_LEFT:	//用户离开房间
			{
				// 数据校验
				CHECK_SOCKET_DATA(MSG_GR_R_UserLeft, objectSize, "MSG_GR_R_UserLeft size is error.");

				MSG_GR_R_UserLeft * pUserLeft = (MSG_GR_R_UserLeft*)object;
				
				UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserLeft->dwUserID);
				
				// 分发房间消息
				dispatchFrameMessage([&userInfo](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onUserLeftMessage(userInfo);
					return false;
				});
				UserInfoModule()->removeUser(pUserLeft->dwUserID);
			}
			break;
		case ASS_GR_USER_CUT:	//用户断线
			{
				// 数据校验
				CHECK_SOCKET_DATA(MSG_GR_R_UserCut, objectSize, "MSG_GR_R_UserCut size is error.");

				MSG_GR_R_UserCut* pUserCut = (MSG_GR_R_UserCut*)object;

				UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserCut->dwUserID);
				if (nullptr != userInfo)
				{
					//设置数据
					userInfo->bUserState = USER_CUT_GAME;
				}

				// 分发游戏消息
				dispatchGameMessage([&pUserCut, &userInfo](IGameMessageDelegate* delegate) -> bool
				{
					delegate->onUserCutMessage(pUserCut->dwUserID, pUserCut->bDeskNO, pUserCut->bDeskStation);
					return false;
				});

				// 分发房间消息
				dispatchFrameMessage([&pUserCut](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onUserCutMessage(pUserCut->dwUserID, pUserCut->bDeskNO, pUserCut->bDeskStation);
					return false;
				});
			}
			break;
		case ASS_GR_USER_UP:	// 用户站起
		case ASS_GR_WATCH_UP:	// 旁观站起
			{
				if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
				{
					// 数据校验
					CHECK_SOCKET_DATA(MSG_GR_R_UserUp, objectSize, "MSG_GR_R_UserUp size is error.");

					MSG_GR_R_UserUp* pUserUp = (MSG_GR_R_UserUp*)object;

					// 更新自己信息
					if (pUserUp->dwUserID == loginResult.pUserInfoStruct.dwUserID)
					{
						loginResult.pUserInfoStruct.bDeskNO = pUserUp->bDeskIndex;
						loginResult.pUserInfoStruct.bDeskStation = pUserUp->bDeskStation;
						loginResult.pUserInfoStruct.bUserState = pUserUp->bUserState;
					}

					UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserUp->dwUserID);
					// 更新用户数据
					if (nullptr != userInfo)
					{
						userInfo->bDeskNO = INVALID_DESKNO;
						userInfo->bDeskStation = INVALID_DESKSTATION;
						userInfo->bUserState = USER_LOOK_STATE;
					}
					// 分发游戏消息
					dispatchGameMessage([&pUserUp, &userInfo](IGameMessageDelegate* delegate) -> bool
					{
						delegate->onUserUpMessage(pUserUp, userInfo);
						return false;
					});

					// 分发房间消息
					dispatchFrameMessage([&pUserUp, &userInfo](IRoomMessageDelegate* delegate) -> bool
					{
						delegate->onUserUpMessage(pUserUp, userInfo);
						return false;
					});
				}
				else
				{

					printfSitError(messageHead);
				}
			}
			break;
		case ASS_GR_SIT_ERROR:
			{
				// 分发游戏消息
				dispatchGameMessage([](IGameMessageDelegate* delegate) -> bool
				{
					delegate->onUserSitMessage(nullptr, nullptr);
					return false;
				});	

				// 分发房间消息
				dispatchFrameMessage([](IRoomMessageDelegate* delegate) -> bool
				{
					delegate->onUserSitMessage(nullptr, nullptr);
					return false;
				});
				printfSitError(messageHead);
			}
			break;
		case ASS_GR_USER_SIT:	// 用户坐下
		case ASS_GR_WATCH_SIT:	// 旁观坐下
			{
				if (ERR_GR_SIT_SUCCESS == messageHead->bHandleCode)
				{
					// 数据校验
					CHECK_SOCKET_DATA(MSG_GR_R_UserSit, objectSize, "MSG_GR_R_UserSit size is error.");

					MSG_GR_R_UserSit * pUserSit = (MSG_GR_R_UserSit*)object;

					// 更新自己信息
					if (pUserSit->dwUserID == loginResult.pUserInfoStruct.dwUserID)
					{
						loginResult.pUserInfoStruct.bDeskNO = pUserSit->bDeskIndex;
						loginResult.pUserInfoStruct.bDeskStation = pUserSit->bDeskStation;
						loginResult.pUserInfoStruct.bUserState = pUserSit->bUserState;
					}
					
					UserInfoStruct* userInfo = UserInfoModule()->findUser(pUserSit->dwUserID);
					// 更新用户数据
					if (nullptr != userInfo)
					{
						userInfo->bDeskNO = pUserSit->bDeskIndex;
						userInfo->bDeskStation = pUserSit->bDeskStation;
						userInfo->bUserState = pUserSit->bUserState;
						UserInfoModule()->updateUser(userInfo);
					}

					// 分发游戏消息
					dispatchGameMessage([&pUserSit, &userInfo](IGameMessageDelegate* delegate) -> bool
					{
						delegate->onUserSitMessage(pUserSit, userInfo);
						return false;
					});	

					// 分发房间消息
					dispatchFrameMessage([&pUserSit, &userInfo](IRoomMessageDelegate* delegate) -> bool
					{
						delegate->onUserSitMessage(pUserSit, userInfo);
						return false;
					});
				}
			}
			break;
		default:
			break;
		}
	}

	// 错误输出
	void YZRoomLogic::printfSitError(NetMessageHead* messageHead)
	{
		//显示错误信息
		std::string strMessage;
		switch (messageHead->bHandleCode)
		{
		case ERR_GR_BEGIN_GAME:
			{
				strMessage = "游戏已经开始了!";
				break;
			}
		case ERR_GR_ALREAD_USER:
			{
				strMessage = "坐下此位置失败,下次动作快一点喔!";
				break;
			}
		case ERR_GR_PASS_ERROR:
			{
				strMessage = "游戏桌密码错误,请在游戏设置中重新设置您的携带密码!";
				break;
			}
		case ERR_GR_IP_SAME:
			{
				strMessage = "同桌玩家不允许有相同 IP 地址的玩家一起进行游戏!";
				break;
			}
		case ERR_GR_IP_SAME_3:
			{
				strMessage = "同桌玩家不允许有 IP 地址前３位相同的玩家一起进行游戏!";
				break;
			}
		case ERR_GR_IP_SAME_4:
			{
				strMessage = "同桌玩家不允许有IP 地址前４位相同的玩家一起进行游戏!";
				break;
			}
		case ERR_GR_CUT_HIGH:
			{
				strMessage = "同桌玩家认为您的逃跑率太高,不愿意和您游戏!";
				break;
			}
		case ERR_GR_POINT_LOW:
			{
				strMessage = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
				break;
			}
		case ERR_GR_POINT_HIGH:		
			{
				strMessage = "本桌玩家设置的进入条件，您不符合本桌进入条件!";
				break;
			}
		case ERR_GR_NO_FRIEND:
			{
				strMessage = "此桌有您不欢迎的玩家!";
				break;
			}
		case ERR_GR_POINT_LIMIT:
			{
				char buf[128];
				sprintf(buf, "此游戏桌需要至少 %u 的游戏积分,您的积分不够,不能游戏!", loginResult.uLessPoint);
				strMessage = buf;
				break;
			}
		case ERR_GR_CAN_NOT_LEFT:
			{
				strMessage = "您正在游戏中...";
				break;
			}
		case ERR_GR_NOT_FIX_STATION:
			{
				strMessage = "您不能加入此游戏桌游戏!";
				break;
			}
		case ERR_GR_MATCH_FINISH:
			{
				strMessage = "您的比赛已经结束了,不能继续参加比赛!";
				break;
			}
		case ERR_GR_MATCH_WAIT://比赛场排队中
			{
				break;
			}
		case ERR_GR_UNENABLE_WATCH:
			{
				strMessage = "不允许旁观游戏!";
				break;
			}
		default:
			strMessage = "抱歉,服务器网络繁忙,请重新进入游戏房间后选择游戏桌位置!";
			break;
		}

		CHAR buf[512];
		sprintf(buf, ("加入游戏失败 >> %s"), strMessage.c_str());
		YZLOG_ERROR("%s", GBKToUtf8(buf));
		GamePromptLayer::create()->showPrompt(GBKToUtf8(buf));
	}
    
    
    void YZRoomLogic::addDeskOwnerInfo(int deskId , int userId){
        JzDebug("添加桌主信息 ---- 桌号 : %d  所有者 : %d " ,deskId , userId);
        _roomDeskOwner.insert(std::make_pair(deskId , userId));
    }
    
    int YZRoomLogic::getDeskOwner(int deskId){
        std::map<int , int >::iterator findInfo;
        findInfo = _roomDeskOwner.find(deskId);
        if (findInfo!= _roomDeskOwner.end()) {
            return _roomDeskOwner.at(deskId);
        }
        return -1;
    }
    
    
    void YZRoomLogic::removeDeskOwnerInfo(int deskId){
        std::map<int , int >::iterator findInfo;
        findInfo = _roomDeskOwner.find(deskId);
        
        if (findInfo!= _roomDeskOwner.end()) {
            _roomDeskOwner.erase(findInfo);
        }
    }

}
