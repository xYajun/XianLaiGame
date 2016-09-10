#include "YZGameLogicBase.h"
#include "YZCommon/YZLog.h"
#include "YZGameTableUsersData.h"

namespace YZ
{
	YZGameLogicBase::YZGameLogicBase(BYTE deskNo, INT maxPlayers, bool autoCreate)
		: _deskNo(deskNo)
		, _maxPlayers(maxPlayers)
		, _autoCreate(autoCreate)
	{
		initParams();

		_deskUserList = new YZGameTableUsersData(deskNo);
		RoomLogic()->addObserver(this);
	}

	YZGameLogicBase::~YZGameLogicBase()
	{
		RoomLogic()->removeObserver(this);
		YZ_SAFE_DELETE(_deskUserList);
	}
    //////////////////////////////
   

	//////////////////////////////////////////////////////////////////////////
	void YZGameLogicBase::onUserAgreeMessage(MSG_GR_R_UserAgree* agree)
	{
		if (agree->bDeskNO != _deskNo) return;
			
		dealUserAgreeResp(agree);		
	}

	void YZGameLogicBase::onGameStartMessage(BYTE bDeskNO)
	{
		if (bDeskNO != _deskNo) return;

		dealGameStartResp(bDeskNO);	
	}

	void YZGameLogicBase::onGameChatMessage(void * object, INT objectSize)
	{
		//dealUserChatMessage(object, objectSize);
	}

	void YZGameLogicBase::onGameEndMessage(BYTE bDeskNO)
	{
		if (bDeskNO != _deskNo) return;

		dealGameEndResp(bDeskNO);

		if ((RoomLogic()->getRoomRule() & GRR_CONTEST) || (RoomLogic()->getRoomRule() & GRR_QUEUE_GAME))
		{
			_deskUserList->clear();
		}
	}

	void YZGameLogicBase::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (nullptr == userSit) return;
		if (nullptr == user) return;

		if (userSit->bDeskIndex != _deskNo) return;

		if (userSit->bUserState != USER_WATCH_GAME)
		{
			if (_maxPlayers > 0)
			{
				_existPlayer[userSit->bDeskStation] = true;
			}
			
			if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				_deskNo = userSit->bDeskIndex;
				_mySeatNo = userSit->bDeskStation;
			}
			dealUserSitResp(userSit, user);
		}
	}

	// 排队用户坐下
	void YZGameLogicBase::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
	{
		BYTE mySeatNo = INVALID_DESKNO;

		for (auto iter = users.begin(); iter != users.end(); ++iter)
		{
			if((*iter)->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				mySeatNo = (*iter)->bDeskStation;
				break;
			}
		}

		if (INVALID_DESKNO == mySeatNo) return;
        
        _deskNo = deskNo;
		_mySeatNo = mySeatNo;

		_seatOffset = -_mySeatNo;

		_deskUserList->update(_deskNo);
		dealQueueUserSitMessage(deskNo, users);
	}

	void YZGameLogicBase::onUserUpMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		if (userSit->bDeskIndex != _deskNo) return;
		if (userSit->bUserState != USER_WATCH_GAME)
		{
			if (_maxPlayers > 0)
			{
				_existPlayer[userSit->bDeskStation] = false;
			}
			
			if (userSit->dwUserID == PlatformLogic()->loginResult.dwUserID)
			{
				_mySeatNo = INVALID_DESKSTATION;
				_deskNo = INVALID_DESKNO;
			}

			dealUserUpResp(userSit, user);
		}
	}

	void YZGameLogicBase::onGameInfoMessage(MSG_GM_S_GameInfo* pGameInfo)
	{
		_gameStatus = pGameInfo->bGameStation;
		_waitTime   = pGameInfo->bWaitTime;
		_watchOther = pGameInfo->bWatchOther;

		dealGameInfoResp(pGameInfo);
	}

	void YZGameLogicBase::onGameStationMessage(void* object, INT objectSize)
	{
		dealGameStationResp(object, objectSize);
	}

	bool YZGameLogicBase::onGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		dealGameMessage(messageHead, object, objectSize);
		return true;
	}

	void YZGameLogicBase::onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation)
	{
		if (bDeskNO != _deskNo) return;

		dealUserCutMessageResp(dwUserID, bDeskStation);
	}


	// 结算消息
	void YZGameLogicBase::onGamePointMessage(void * object, INT objectSize)
	{
		dealGamePointResp(object, objectSize);
	}

	//////////////////////////////////////////////////////////////////////////
	// 比赛初始化
	void YZGameLogicBase::onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange)
	{
		dealGameContestInit(contestChange);
	}

	// 用户比赛信息
	void YZGameLogicBase::onGameUserContset(MSG_GR_ContestChange* contestChange)
	{
		dealGameUserContset(contestChange);
	}

	// 比赛结束
	void YZGameLogicBase::onGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward)
	{
		dealGameContestOver(contestAward);
	}

	// 比赛淘汰
	void YZGameLogicBase::onGameContestKick()
	{
		dealGameContestKick();
	}

	// 等待比赛结束
	void YZGameLogicBase::onGameContestWaitOver() 
	{
		dealGameContestWaitOver();
	}

	//////////////////////////////////////////////////////////////////////////
	// 聊天消息
	void YZGameLogicBase::onUserChatMessage(void* object, INT objectSize)
	{
		dealUserChatMessage(object, objectSize);
	}



	//////////////////////////////////////////////////////////////////////////
	void YZGameLogicBase::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
	{
		YZLOG("dealUserAgreeResp");	
	}

	void YZGameLogicBase::dealGameStartResp(BYTE bDeskNO)
	{
		YZLOG("dealGameStartResp");
	}

	void YZGameLogicBase::dealGameEndResp(BYTE bDeskNO)
	{
		YZLOG("dealGameEndResp");
		for(int i = 0; i < _maxPlayers; i++)
		{
			if(_mySeatNo == i)
			{
				if (PlatformLogic()->loginResult.i64Money < RoomLogic()->loginResult.uLessPoint)
				{
					sendData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
				}
			}
		};
		
	}

	void YZGameLogicBase::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		YZLOG("dealUserSitResp");
	}

	void YZGameLogicBase::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
	{
		YZLOG("dealQueueUserSitMessage");
	}

	void YZGameLogicBase::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		YZLOG("dealUserUpResp");
	}

	void YZGameLogicBase::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
		YZLOG("dealGameInfoResp");
	}

	void YZGameLogicBase::dealUserCutMessageResp(INT userId, BYTE seatNo)
	{
		YZLOG("dealUserCutMessageResp");
	}
	
	void YZGameLogicBase::dealGamePointResp(void * object, INT objectSize)
	{
		YZLOG("dealGamePointResp");
	}

	//////////////////////////////////////////////////////////////////////////
	// 比赛接口
	//////////////////////////////////////////////////////////////////////////
	// 比赛初始化
	void YZGameLogicBase::dealGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange)
	{
		YZLOG("dealGameContestInit");
		CCAssert(false, "subclasses must override");
	}

	// 用户比赛信息
	void YZGameLogicBase::dealGameUserContset(MSG_GR_ContestChange* contestChange)
	{
		YZLOG("dealGameUserContset");
		CCAssert(false, "subclasses must override");
	}

	// 比赛结束
	void YZGameLogicBase::dealGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward)
	{
		YZLOG("dealGameContestOver");
		CCAssert(false, "subclasses must override");
	}

	// 比赛淘汰
	void YZGameLogicBase::dealGameContestKick()
	{
		YZLOG("dealGameContestKick");
		CCAssert(false, "subclasses must override");
	}

	// 等待比赛结束
	void YZGameLogicBase::dealGameContestWaitOver()
	{
		YZLOG("dealGameContestWaitOver");
		CCAssert(false, "subclasses must override");
	}

	// 比赛奖励
	void YZGameLogicBase::dealGameContestAwards(NET_ROOM_CONTEST_AWARD_RESULT* contestAward)
	{
		YZLOG("dealGameContestAwards");
	}

	// 报名数量
	void YZGameLogicBase::dealGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople)
	{
		YZLOG("dealGameContestPeople");
	}

	// 个人参赛纪录
	void YZGameLogicBase::dealGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* ContestRecord)
	{
		YZLOG("dealGameContestRecord");
	}

	//////////////////////////////////////////////////////////////////////////
	// 聊天接口
	//////////////////////////////////////////////////////////////////////////
	// 聊天消息
	void YZGameLogicBase::dealUserChatMessage(void* object, INT objectSize)
	{
		YZLOG("dealGameChatMessage");
	}

	//////////////////////////////////////////////////////////////////////////
	void YZGameLogicBase::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		RoomLogic()->sendData(MainID, AssistantID, object, objectSize);
	}

	void YZGameLogicBase::sendGameInfo()
	{
		YZLOG("sendGameInfo");
		MSG_GM_S_ClientInfo ClientInfo;
		ClientInfo.bEnableWatch = 0;
		sendData(MDM_GM_GAME_FRAME, ASS_GM_GAME_INFO, &ClientInfo, sizeof(ClientInfo));
	}

	void YZGameLogicBase::sendUserUp()
	{
		sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
	}

	void YZGameLogicBase::sendUserSit(BYTE viewSeatNo)
	{
		BYTE logicSeatNo = viewToLogicSeatNo(viewSeatNo);

		UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(logicSeatNo);
		if (nullptr != userInfo)
		{
			BYTE tmpSeatNo = logicSeatNo;
			do 
			{
				tmpSeatNo = (tmpSeatNo + 1) % _maxPlayers;
				if(_deskUserList->getUserByDeskStation(logicSeatNo) == nullptr)
				{
					break;
				}
			} while (tmpSeatNo != logicSeatNo);
			if(tmpSeatNo == logicSeatNo)
			{
				YZLOG("no empty sit.");
				return;
			}
			else
			{
				logicSeatNo = tmpSeatNo;
			}
		}

		MSG_GR_S_UserSit UserSit;
		UserSit.bDeskIndex = _deskNo;
		UserSit.bDeskStation = logicSeatNo;
		sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, &UserSit, sizeof(UserSit));
	}

	void YZGameLogicBase::sendForceQuit()
	{
		sendData(MDM_GM_GAME_FRAME, ASS_GM_FORCE_QUIT);
	}

	void YZGameLogicBase::sendAgreeGame()
	{
		sendData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME);
	}

	//////////////////////////////////////////////////////////////////////////
	
	void YZGameLogicBase::loadDeskUsers()
	{
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < _maxPlayers; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(nullptr != pUser)
			{
				if(pUser->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID)
				{
					_mySeatNo = i;
				}
				if(_maxPlayers > 0)
				{
					_existPlayer[i] = true;
				}
				
			}
		}
	}

	void YZGameLogicBase::clearDesk()
	{
		
	}

	//////////////////////////////////////////////////////////////////////////
	INT YZGameLogicBase::getUserId(BYTE vSeatNo)
	{
		BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);
		UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
		if(userInfo != nullptr)
		{
			return userInfo->dwUserID;
		}
		return INVALID_USER_ID;
	}

	INT YZGameLogicBase::getUserLogoId(BYTE vSeatNo)
	{
		BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);
		UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
		if(userInfo != nullptr)
		{
			return userInfo->bLogoID;
		}
		return INVALID_USER_ID;
	}
	

	UserInfoStruct* YZGameLogicBase::getUserByUserID(INT userID)
	{
		return _deskUserList->getUserByUserID(userID);
	}

	UserInfoStruct* YZGameLogicBase::getUserByDeskStation(BYTE vSeatNo)
	{
		BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);
		return _deskUserList->getUserByDeskStation(lSeatNo);
	}

	bool YZGameLogicBase::getUserIsBoy(BYTE vSeatNo)
	{
		BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);
		UserInfoStruct* userInfo = _deskUserList->getUserByDeskStation(lSeatNo);
		if(userInfo != nullptr)
		{
			return userInfo->bBoy;
		}
		return true;
	}

	BYTE YZGameLogicBase::getMySeatNo() const
	{
		return _mySeatNo;
	}
	//////////////////////////////////////////////////////////////////////////

	void YZGameLogicBase::initParams()
	{
		_gameStatus = 0;
		_mySeatNo = INVALID_DESKSTATION;
		_waitTime = 0;
		_seatOffset = 0;

		if(_maxPlayers > 0)
		{
			_existPlayer.resize(_maxPlayers, false);
			_playing.resize(_maxPlayers, false);
		}
	}

	void YZGameLogicBase::refreshParams()
	{

	}

	BYTE YZGameLogicBase::getNextUserSeatNo(BYTE lSeatNo)
	{
		if(_maxPlayers <= 0)
		{
			return INVALID_DESKSTATION;
		}

		BYTE nextSeatNo = lSeatNo;
		int count = 0;
		while(count < _maxPlayers)
		{
			nextSeatNo = (nextSeatNo + 1 + _maxPlayers) % _maxPlayers;
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(nextSeatNo);
			if(pUser != nullptr && _playing[nextSeatNo])
			{
				return nextSeatNo;
			}
			count++;
		}
		return INVALID_DESKSTATION;
	}

	BYTE YZGameLogicBase::getLastUserSeatNo(BYTE lSeatNo)
	{
		if(_maxPlayers <= 0)
		{
			return INVALID_DESKSTATION;
		}

		BYTE lastSeatNo = lSeatNo;
		int count = 0;
		while(count < _maxPlayers)
		{
			lastSeatNo = (lastSeatNo - 1 + _maxPlayers) % _maxPlayers;
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(lastSeatNo);
			if(pUser != nullptr && _playing[lastSeatNo])
			{
				return lastSeatNo;
			}
			count++;
		}
		return INVALID_DESKSTATION;
	}


	BYTE YZGameLogicBase::viewToLogicSeatNo(BYTE vSeatNO)
	{
		return (vSeatNO - _seatOffset + _maxPlayers) % _maxPlayers;
	}

	BYTE YZGameLogicBase::logicToViewSeatNo(BYTE lSeatNO)
	{
		return (lSeatNO + _seatOffset + _maxPlayers) % _maxPlayers;
	}
}

