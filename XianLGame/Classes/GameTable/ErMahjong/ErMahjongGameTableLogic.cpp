#include "ErMahjongGameTableLogic.h"
#include "YZNetExport.h"
#include "GamePrompt.h"
#include "ErMahjongGameTableUICallback.h"
#include "Util/JzLog.h"
using namespace YZ;

namespace ErMahjong
{
	GameTableLogic::GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate)
		: _uiCallback(uiCallback)
		, YZGameLogicBase(deskNo, PLAYER_COUNT, bAutoCreate)
	{
	    initParams();
	}

	GameTableLogic::~GameTableLogic()
	{
    }

	void GameTableLogic::dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		if(MDM_GM_GAME_NOTIFY != messageHead->bMainID)
		{
			YZLOG("onGameMessage MainID is error.");
			return;
		}
		switch (messageHead->bAssistantID)
		{
		case SUB_S_GAME_START:
		    dealGameBeginResp(object, objectSize);
			break;
		case SUB_S_OUT_CARD:
			dealGameOutCardResp(object, objectSize);
			break;
		case SUB_S_SEND_CARD:
			dealGameGetCardResp(object, objectSize);
			break;
		case SUB_S_LISTEN_CARD:
            break;
		case SUB_S_OPERATE_NOTIFY:
			dealGameActNotifyResp(object, objectSize);
			break;
		case SUB_S_OPERATE_RESULT :
            dealGameActNotifyResultResp(object, objectSize);
			break;
		case SUB_S_GAME_END:
                
                if (_isLastGame) {
                    _isLastGameEnd = true;
                }
            _gameStatus=GS_GAMEEND;
            dealThisGameEndResp(object, objectSize);
			break;
		case SUB_S_TRUSTEE:
            dealGameTrusteeResp(object , objectSize);
			break;
		case SUB_S_GAME_YAZI:
			break;
        case SUB_S_LISTEN_CARD_NOTICE:
            dealListenerCardResp(object, objectSize);
            break;
		case SUB_S_SEZI:
            dealGameShaiZiResp(object, objectSize);
			break;
                
            case ASS_GR_DISMISS_BACKROOM_REQ:
                dealAsk2DisbandRoomResp(object, objectSize);
                break;
                
            case ASS_GR_DISMISS_BACKROOM :
            {
                if (!_isLastGameEnd) {
                    this->sendUserUp();
                }
                //this->sendGetAllGameRecord();
            }
                break;
                
        break;
		default:
			break;
		}
	}
    
    //======================解散房间=========================//
    void GameTableLogic::dealAsk2DisbandRoomResp(void *object, INT objectSize){
        
        MSG_GR_C_BACKROOM_REQ_DISMISS* pData = (MSG_GR_C_BACKROOM_REQ_DISMISS*)object;
        
        UserInfoStruct pUserInfoStruct= RoomLogic()->loginResult.pUserInfoStruct;
        
        int myRoleId = pUserInfoStruct.dwUserID;
        
        _uiCallback->showUserAskDisbandRoom(true);
    }
    
    void GameTableLogic::dealListenerCardResp(void * object ,INT objectSize){
        CHECK_SOCKET_DATA(CMD_S_ListenCard1, objectSize, "CMD_S_ListenCard1 size of error!");
        CMD_S_ListenCard1 *pData = (CMD_S_ListenCard1 *)object;
        
        if (pData->wListenUser == _mySeatNo) {
            
            std::vector<BYTE> listenerCards;
            listenerCards.clear();
            for (int i=0; i< 28; i++) {
                if (pData->cbListenCardData[i] != 0x00) {
                    listenerCards.push_back(pData->cbListenCardData[i]);
                }
            }
            
            _uiCallback->showListenerCard(true, listenerCards);
        }
    }


	void GameTableLogic::dealGameShaiZiResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_GameSeZi, objectSize, "ShaiZi size of error!");
		_gameStatus=GS_PLAYING;

		CMD_S_GameSeZi *pData= (CMD_S_GameSeZi*)object;
		_uiCallback->showGameShaiZi(pData->cbSiceCount);
	}

	void GameTableLogic::dealUserAgreeResp(MSG_GR_R_UserAgree* agree)
    {
		_userReady[agree->bDeskStation] = (agree->bAgreeGame == 1);
		if (GS_WAIT_ARGEE == _gameStatus)
		{
			_gameStatus = GS_WAIT_ARGEE;
		}
        
        
        BYTE seatNo = INVALID_DESKNO;
        for(int i = 0; i < PLAYER_COUNT; i++)
        {
            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
            if(_existPlayer[i] && pUser != nullptr)
            {
                _playerSitted[i] = true;
                seatNo = logicToViewSeatNo(i);
                
                if (pUser->bUserState == USER_ARGEE)
                {
                    _userReady[i] = true;
                    _uiCallback->showUserReady(seatNo, true);
                }
            }
        }
     
    }
    
    bool GameTableLogic::getUserIsAgree(BYTE seatNo){
        BYTE sNo = viewToLogicSeatNo(seatNo);
        return _userReady[sNo];
    }
    

	void GameTableLogic::dealGameStartResp(BYTE bDeskNO)
	{
	}

	void GameTableLogic::dealGameEndResp(BYTE deskNo)
	{
        _uiCallback->showGameEnd();
		YZGameLogicBase::dealGameEndResp(deskNo);
	}

	void GameTableLogic::dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		YZGameLogicBase::dealUserSitResp(userSit, user);
		bool isMe = (user->dwUserID == RoomLogic()->loginResult.pUserInfoStruct.dwUserID);
		if(isMe)
		{
			setSeatOffset(userSit->bDeskStation);
			loadUsers();
		}
		else
		{
			if (_playerSitted[userSit->bDeskStation]) return;

			_playerSitted[userSit->bDeskStation] = true;
			BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);
			_uiCallback->addUser(seatNo, userSit->bDeskStation == _mySeatNo);
			_uiCallback->setUserName(seatNo, user->nickName);
			_uiCallback->setUserMoney(seatNo, 0);
			_uiCallback->showUserMoney(seatNo, true);
		}	
	}

	void GameTableLogic::dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user)
	{
		YZGameLogicBase::dealQueueUserSitMessage(deskNo, user);
		if(_mySeatNo != INVALID_DESKSTATION)
		{
			setSeatOffset(_mySeatNo);
			loadUsers();
			sendGameInfo();
		}
	}

	void GameTableLogic::dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		YZGameLogicBase::dealUserUpResp(userSit, user);
		if(!_playerSitted[userSit->bDeskStation]) return;
		_playerSitted[userSit->bDeskStation] = false;

		BYTE seatNo = logicToViewSeatNo(userSit->bDeskStation);

		_uiCallback->setUserName(seatNo, "");
		_uiCallback->showUserMoney(seatNo, false);
		_uiCallback->setUserMoney(seatNo, 0);
		_uiCallback->removeUser(logicToViewSeatNo(userSit->bDeskStation), userSit->bDeskStation == _mySeatNo);
		if (INVALID_DESKSTATION == _mySeatNo)
		{
			_uiCallback->leaveDesk();
		}
	}

	void GameTableLogic::dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)
	{
	}
    
    void GameTableLogic::dealUserChatMessage(void* object, INT objectSize){
        
        MSG_GR_RS_NormalTalk* pData = (MSG_GR_RS_NormalTalk*)object;
        
        BYTE t_seat = 0;
        UserInfoStruct* LogonResult = getUserByUserID(pData->dwSendID);
        if (nullptr != LogonResult)
        {
            auto seat = LogonResult->bDeskStation;
            t_seat = logicToViewSeatNo(seat);
        }
        
        if (pData->nDefaultIndex == 0)
        {
            _uiCallback->showUserChatTalkMsg(t_seat, pData->szMessage);
        }
        else if (pData->nDefaultIndex == 1)
        {
            int faceid = atoi(pData->szMessage);
            
            _uiCallback->showUserChatFaceAnimate(t_seat,faceid);
            log("this is the face !!!");
        }
        else if (pData->nDefaultIndex == 2)
        {
            //录音消息
            
            std::string msg = pData->szMessage;
            
            CCLOG("收到的语音聊天消息 : %s " ,msg.c_str());
            
            const char * split = "-";
            char * p;
            p = strtok ( pData->szMessage,split);
            std::string arr[2];
            int index = 0;
            while(p) {
                arr[index] = p;
                index++;
                p = strtok(NULL, split);
            }
            
            _uiCallback->showUserVoice(t_seat, arr[0], std::atoi(arr[1].c_str()));
        }
    }
    
    
    void GameTableLogic::dealGameStationResp(void* object, INT objectSize)
	{
		YZLOG("dealGameStationResp");

		switch(_gameStatus)
	    {
	
			case GS_WAIT_ARGEE:
				{
                    CMD_S_StatusFree* pData= (CMD_S_StatusFree*)object;
                    
					_gameStatus=GS_WAIT_ARGEE;
					_waiting=true;
                    
                    _isLastGame = pData->m_iPlayGameRounds == pData->m_iTotalGameRounds;
                    
                    _uiCallback->isLastGame(pData->m_iPlayGameRounds , pData->m_iTotalGameRounds);
                }
                break;
                
            case GS_MJ_PLAY:
                dealGoOnGameResp(object,objectSize);
                
                break;
                
                
			case GS_WAIT_NEXT:
				{
                    CMD_S_StatusFree* pData= (CMD_S_StatusFree*)object;
                    
                    _gameStatus=GS_WAIT_ARGEE;
                    _waiting=true;
                    
                    _isLastGame = pData->m_iPlayGameRounds == pData->m_iTotalGameRounds;
                    
                    _uiCallback->isLastGame(pData->m_iPlayGameRounds , pData->m_iTotalGameRounds);
                }
				
				break;

			
		}
	}
    
#pragma mark - 断线重连
    void GameTableLogic::dealGoOnGameResp(void* object, INT objectSize)
    {
        CHECK_SOCKET_DATA(CMD_S_StatusPlay, objectSize, "CMD_S_StatusPlay size of error!");
        CMD_S_StatusPlay *pData = (CMD_S_StatusPlay *)object;

        JzLog::shareLog()->log(kLogLevelDebug, "=================断线重连开始================");
        
        BYTE wBankerUser = logicToViewSeatNo(pData->wBankerUser);
        BYTE wCurrentUser = logicToViewSeatNo(pData->wCurrentUser);
        BYTE wOutCardUser = logicToViewSeatNo(pData->wOutCardUser);
        
        BackGameData data;
        
        data.wBankerUser = wBankerUser;
        data.cbActionCard = pData->cbActionCard;
        data.cbActionMask = pData->cbActionMask;
        data.wCurrentUser = wCurrentUser;
        data.wOutCardUser = wOutCardUser;
        data.cbOutCardData = pData->cbOutCardData;
        data.cbSendCardData = pData->cbSendCardData;
        
        JzLog::shareLog()->log(kLogLevelDebug, "庄家游戏座位号为 ： %d   本地座位号为 ： %d" , pData->wBankerUser , wBankerUser);
        JzLog::shareLog()->log(kLogLevelDebug, "当前玩家游戏座位号为 ： %d   本地座位号为 ： %d" , pData->wCurrentUser , wCurrentUser);
        JzLog::shareLog()->log(kLogLevelDebug, "出牌玩家游戏座位号为 ： %d   本地座位号为 ： %d" , pData->wOutCardUser , wOutCardUser);
        JzLog::shareLog()->log(kLogLevelDebug, "出牌玩家牌值为 ： %d" , pData->cbOutCardData );

        
        std::map<BYTE , std::vector<BYTE>> cbDusCardMaps;                           //所有玩家打出的牌
        std::map<BYTE , std::vector<CMD_WeaveItem>> cbWeaveItemMaps;
        std::map<BYTE , std::vector<BYTE>> cbHandCardsMaps;                         //手牌列表
        
        JzLog::shareLog()->log(kLogLevelDebug, "-------恢复牌值开始----------");
        
        //转换服务器发送过来的出牌数据为本地数据
        for (int i = 0; i < PLAYER_COUNT; i++) {
            
            BYTE localSeatNo = logicToViewSeatNo(i);

            JzLog::shareLog()->log(kLogLevelDebug, "~~~~~~服务器座位号 %d   本地座位号~~~~~~~", i);
            
            std::vector<BYTE> outCards;
            outCards.clear();
            BYTE outCount = pData->cbDiscardCount[i];
            
            JzLog::shareLog()->log(kLogLevelDebug, "恢复座位号为：%d 玩家的出牌列表 ， 手牌数量为 ： %d" , localSeatNo ,outCount);
            //出牌数据
            for (int j = 0; j < outCount; j++) {
                
                BYTE cbColor=(pData->cbDiscardCard[i][j]&MASK_COLOR)>>4;
                BYTE cbValue=(pData->cbDiscardCard[i][j]&MASK_VALUE);
                
                JzLog::shareLog()->log(kLogLevelDebug, "索引 : %d  牌色 : %d  牌值 : %d" , j ,cbColor ,cbValue );
                
                outCards.push_back(pData->cbDiscardCard[i][j]);
            }
            
            cbDusCardMaps.insert(std::make_pair(localSeatNo, outCards));
                        
            //动作牌数据
            std::vector<CMD_WeaveItem> weaveItems;
            outCards.clear();
            
            BYTE actionCount = pData->cbWeaveCount[i];
            
            JzLog::shareLog()->log(kLogLevelDebug, "恢复玩家的动作牌列表 ， 动作数量为 ： %d" ,actionCount);
            
            for (int j = 0; j < actionCount; j++) {
                BYTE cbColor=(pData->WeaveItemArray[i][j].cbPublicCard&MASK_COLOR)>>4;
                BYTE cbValue=(pData->WeaveItemArray[i][j].cbPublicCard&MASK_VALUE);
                
                JzLog::shareLog()->log(kLogLevelDebug, "索引 : %d  牌色 : %d  牌值 : %d" , j ,cbColor ,cbValue );
                weaveItems.push_back(pData->WeaveItemArray[i][j]);
            }
            cbWeaveItemMaps.insert(std::make_pair(localSeatNo, weaveItems));
            
            data.bTrustee[localSeatNo] = pData->bTrustee[i];
            
            if (localSeatNo != 0) {
                std::vector<BYTE>               handCards;                                //当前玩家手牌数据
                handCards.clear();
                int maxCount = MAX_COUNT - 1;
                int handCardCount = maxCount - actionCount * 3;
                JzLog::shareLog()->log(kLogLevelDebug, "恢复座位号为：%d 玩家的手牌列表 ， 手牌数量为 ： %d" , localSeatNo ,handCardCount);
                for (int k = 0; k < handCardCount; k++) {
                    handCards.push_back(0x01);
                }
                cbHandCardsMaps.insert(make_pair(localSeatNo, handCards));
            }
            
            
            JzLog::shareLog()->log(kLogLevelDebug, "~~~~~~~~~~~~~~~~ End ~~~~~~~~~~~~~~~~");
        }
        
        data.cbDusCardMaps = cbDusCardMaps;
        data.cbWeaveItemMaps = cbWeaveItemMaps;
        
        JzLog::shareLog()->log(kLogLevelDebug, "恢复自己的手牌列表 ， 麻将数量为 ： %d" , pData->cbCardCount);
        
        std::vector<BYTE>               cbCardDatas;                                //当前玩家手牌数据
        cbCardDatas.clear();
        for (int i = 0; i< pData->cbCardCount ; i++) {
            BYTE cbColor=(pData->cbCardData[i]&MASK_COLOR)>>4;
            BYTE cbValue=(pData->cbCardData[i]&MASK_VALUE);
            
            
            if (pData->cbCardData[i] == 0x53) {
                cbCardDatas.insert(cbCardDatas.begin(), pData->cbCardData[i]);
            }else{
                cbCardDatas.push_back(pData->cbCardData[i]);
            }
            JzLog::shareLog()->log(kLogLevelDebug, "索引 : %d  牌色 : %d  牌值 : %d" , i ,cbColor ,cbValue );
        }
        
        if (pData->wCurrentUser == _mySeatNo) {
            for (int i = 0; i < cbCardDatas.size(); i++) {
                if (cbCardDatas.at(i) == pData->cbSendCardData) {
                    cbCardDatas.erase(cbCardDatas.begin() + i);
                    break;
                }
            }
        }
        
        cbHandCardsMaps.insert(make_pair(0, cbCardDatas));
        
        data.cbHandCardsMaps = cbHandCardsMaps;
        
        JzLog::shareLog()->log(kLogLevelDebug, "-------恢复牌值结束----------");
        
        _uiCallback->setGameZMCount(pData->m_cbZaCodeNum);
        //玩家回到游戏
        _uiCallback->playerBackGame(data);
        
        JzLog::shareLog()->log(kLogLevelDebug, "=================断线重连结束================");
       
        _uiCallback->setGamePlayerPoint(_mySeatNo);
        
        _gameUsers.clear();
        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
            _gameUsers.insert(make_pair(i, pUser));
        }

        //显示财神
        _isLastGame = pData->m_iPlayGameRounds == pData->m_iTotalGameRounds;
        _uiCallback->isLastGame(pData->m_iPlayGameRounds , pData->m_iTotalGameRounds);
        _uiCallback->setTureCaiShenCard(pData->cbDaCardData);
        _uiCallback->showMahjongLayout(pData->cbFanDaCardData , true);

        //如果有动作恢复
        if (pData->cbActionMask != ActionType::WIK_NULL) {
            if ((pData->cbActionMask & ActionType::WIK_CHI_HU) != 0) {
                this->sendActRequest(ActionType::WIK_CHI_HU,pData->cbActionCard);
            }else{
                _uiCallback->showActionBtn(wCurrentUser ,pData->cbActionMask , pData->cbActionCard);
            }
        }
    }

	void GameTableLogic::onGamePointMessage(void* object, INT objectSize)
	{
	}


	void GameTableLogic::dealGameBeginResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_GameStart, objectSize, "GameStartResp size of error!");
		_gameStatus=GS_PLAYING;
		CMD_S_GameStart *pData = (CMD_S_GameStart *)object;

		BYTE mySeatNo=logicToViewSeatNo(_mySeatNo);

		std::vector<BYTE> cards;
        
        cards.clear();
		
        BYTE wBankerUser = logicToViewSeatNo(pData->wBankerUser);
        
        for (int i = 0; i < MAX_COUNT - 1; i++)
		{
            BYTE card = pData->cbCardData[i];
            
            if (pData->cbDaCardData == card) {
                cards.insert(cards.begin(), card);
            }else{
                cards.push_back(card);
            }
		}
        
    
        CCLOG("values size : %lu " , cards.size());

        CCLOG("庄家是 : %d" , wBankerUser);
        
        _uiCallback->setGameZMCount(pData->m_cbZaCodeNum);
        
        _isLastGame = pData->m_iPlayGameRounds == pData->m_iTotalGameRounds;
        
        _uiCallback->isLastGame(pData->m_iPlayGameRounds , pData->m_iTotalGameRounds);
        
        _uiCallback->setTureCaiShenCard(pData->cbDaCardData);
        
        _uiCallback->setGamePlayerPoint(_mySeatNo);
        
        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            
            BYTE seatNo = logicToViewSeatNo(i);
            _uiCallback->showUserHandCard(seatNo,cards);
            _uiCallback->showUserHandCardOut(seatNo);
        }
       _uiCallback->showAllUserCard(wBankerUser , cards  , pData->cbCardData[MAX_COUNT - 1]);
        
		CCLOG("真财神  : %d" , pData->cbDaCardData);
		CCLOG("显示财神  : %d" , pData->cbFanDaCardData);

		_waiting=false;
        
        if ((pData->cbUserAction & ActionType::WIK_CHI_HU) != 0) {
            this->sendActRequest(ActionType::WIK_CHI_HU,pData->cbCardData[MAX_COUNT - 1]);
        }
        
        _gameUsers.clear();
        for (int i = 0; i < PLAYER_COUNT; i++)
        {
            UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
            _gameUsers.insert(make_pair(i, pUser));
        }
        
        //_uiCallback->setWaitTime(0, 30, true, 1);
	}

    
#pragma mark 服务器发送的指令 ---- 玩家出牌
	void GameTableLogic::dealGameOutCardResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_OutCard, objectSize, "CMD_S_OutCard size of error!");
        
        CMD_S_OutCard* pData= (CMD_S_OutCard*)object;
		BYTE seatNo = logicToViewSeatNo(pData->wOutCardUser);

        CCLOG("服务器收到玩家出牌的消息... 服务器发送座位号为 ：%d   我的座位为： %d  出牌结果 :%d" , pData->wOutCardUser,_mySeatNo, pData->cbRspcode);
        
        if (pData->cbRspcode == 1) {
            _uiCallback->showOutCard(seatNo, pData->cbOutCardData);
        }else{
            CCLOG("出牌错误  错误码  : %d " , pData->cbRspcode);
            
            if (seatNo == 0) {
                _uiCallback->outCardResultError(seatNo);
            }
        }
        
        /*
        if (pData->wOutCardUser==_mySeatNo)
		{
            if (pData->cbRspcode == 1) {
            }
		}
		else
		{
            _uiCallback->showOutCard(seatNo, pData->cbOutCardData);
		}*/
	}

#pragma mark 服务器发送的指令 ---- 摸牌
	void GameTableLogic::dealGameGetCardResp(void* object, INT objectSize)
	{
        CHECK_SOCKET_DATA(CMD_S_SendCard, objectSize, "CMD_S_SendCard size of error!");
		CMD_S_SendCard* pData= (CMD_S_SendCard*)object;
        BYTE seatNo = logicToViewSeatNo(pData->wCurrentUser);
        CCLOG("摸牌....  %d 号玩家拿到一张牌  牌值为 : %d" , seatNo , pData->cbCardData);

        _uiCallback->showPlayerGetCard(seatNo, pData->cbCardData);
        
        if (pData->cbActionMask != ActionType::WIK_NULL) {
            if (seatNo == 0) {
                if ((pData->cbActionMask & ActionType::WIK_GANG) != 0) {
                    
                    BYTE cardValue = _uiCallback->getPlayerGuangCard(seatNo);
                    
                    if (cardValue != 0x00) {
                        _uiCallback->showActionBtn(seatNo ,pData->cbActionMask , cardValue);
                    }else{
                        jzError("服务器发送杠牌动作， 但是客户端没有找到可以杠的牌...");
                    }
                }else{
                    if ((pData->cbActionMask & ActionType::WIK_CHI_HU) != 0) {
                        this->sendActRequest(ActionType::WIK_CHI_HU,pData->cbCardData);
                    }
                }
             }
            _uiCallback->setWaitTime(seatNo, ErMahjong_Game_Time::ACTION_TIME, true, 1);
        } else{
            _uiCallback->setWaitTime(seatNo, ErMahjong_Game_Time::OUTCARD_TIME, true, 1);
        }
	}

    
#pragma 托管
    
    void GameTableLogic::dealGameTrusteeResp(void *object, INT objectSize){
        CHECK_SOCKET_DATA(CMD_S_Trustee, objectSize, "CMD_S_Trustee size of error!");
        
        CMD_S_Trustee *pData = (CMD_S_Trustee *)object;
        
        CCLOG("收到服务器发送的托管回调..操作者为: %d   类型为 : %d" , pData->wChairID , pData->bTrustee);
        
        BYTE seatNo = logicToViewSeatNo(pData->wChairID);
        _uiCallback->showUserTrustee(seatNo, pData->bTrustee);
    }
    
    
    
    void GameTableLogic::dealThisGameEndResp(void *object, INT objectSize){
                
        CCLOG("CMD_S_GameEnd size of .  %lu ， 长度 : %d"  ,sizeof(CMD_S_GameEnd)  , objectSize);
        
        CHECK_SOCKET_DATA(CMD_S_GameEnd, objectSize, "CMD_S_GameEnd size of error!");
        CMD_S_GameEnd* pData= (CMD_S_GameEnd*)object;
        
        CCLOG("收到服务器发送的游戏结束回调...  %d ， 长度 : %d"  ,pData->dwWinTimes  , objectSize);
        
        MJGameEndData data;
        data.cbContinueNT = pData->cbContinueNT;
        data.iLongCount = pData->dwWinTimes;
        data.cbProvideCard = pData->cbProvideCard;
        data.bZiMo = false;

        //Key 座位号 , Value 牌信息
        std::map<BYTE , std::vector<BYTE>> cardInfoMap;
        std::map<BYTE , std::vector<BYTE>> actionCardMaps;
        cardInfoMap.clear();
        actionCardMaps.clear();
        
        std::vector<int> recvGangSeats;     //接杠消息
        std::vector<int> giveGangSeats;     //放杠消息
        std::vector<int> lightGangSeats;     //明杠消息
        std::vector<int> darkGangSeats;     //明杠消息
        
        for (int i = 0; i < GAME_PLAYER; i++) {
            
            BYTE localSeatNo = logicToViewSeatNo(i);
            
            data.dwChiHuKind[localSeatNo] = pData->dwChiHuKind[i];
            data.dwChiHuRight[localSeatNo] = pData->dwChiHuRight[i];
            
            if (pData->dwChiHuKind[i] != CHK_NULL) {
                data.winUser = localSeatNo;
            }
            
            data.iChangeMoney[localSeatNo] = pData->iChangeMoney[i];
            
            JzDebug("%d 号玩家 手牌数量 : %d" ,i, pData->cbCardCount[i]);
            JzDebug("%d 号玩家 得分: %d" ,i, pData->iChangeMoney[i]);
            
            
            std::vector<BYTE> cardInfos;
            cardInfos.clear();
            for (int k = 0; k < pData->cbCardCount[i]; k++) {
                BYTE cardInfo = pData->cbCardData[i][k];
                cardInfos.push_back(cardInfo);
            }
            
            cardInfoMap.insert(std::make_pair(localSeatNo, cardInfos));
            
            //动作牌数据
            std::vector<BYTE> actionCards;
            actionCards.clear();
            BYTE actionCount = pData->cbWeaveCount[i];
            JzDebug("%d 号玩家 动作数量 : %d" ,i, actionCount);
            for (int j = 0; j < actionCount; j++) {
                JzLog::shareLog()->log(kLogLevelDebug, "索引 : %d  牌值 : %d " , j , pData->WeaveItemArray[i][j].cbPublicCard);
                CMD_WeaveItem item = pData->WeaveItemArray[i][j];
                if (item.cbWeaveKind == ActionType::WIK_GANG) {
                    JzDebug("%d 号玩家 杠动作..." ,localSeatNo);
                    for (int k = 0; k < 4; k++) {
                        actionCards.push_back(item.cbCenterCard);
                    }
                }else{
                    if (item.cbWeaveKind == ActionType::WIK_PENG) {
                        JzDebug("%d 号玩家 碰动作..." ,localSeatNo);
                        for (int k = 0; k < 3; k++) {
                            actionCards.push_back(item.cbCenterCard);
                        }
                    }
                }
            }
            JzDebug("%d 号玩家 生成动作牌数量 : %lu" ,i, actionCards.size());
            actionCardMaps.insert(make_pair(localSeatNo, actionCards));
        }
        
        std::vector<BYTE> leftCards;
        leftCards.clear();
        for (int i= 0; i < pData->cbLeftCardCount; i++) {
            leftCards.push_back(pData->cbLeftCardData[i]);
        }

        
        data.lastCardVec = leftCards;
        
        std::vector<BYTE> cbZaCardData;
        cbZaCardData.clear();
        for (int i = 0; i< MAX_ZMCOUNT ; i++) {
            if (pData->cbZaCardData[i] != 0x00) {
                cbZaCardData.push_back(pData->cbZaCardData[i]);
            }
        }
        data.cbZaCardData = cbZaCardData;
        
        JzLog::shareLog()->log(kLogLevelDebug,"=======================================");
        
        _uiCallback->doGameEndResp(data,actionCardMaps,cardInfoMap);
    }

	void GameTableLogic::dealGameUserOutCardsResp(void* object, INT objectSize)
	{
		/*
		CHECK_SOCKET_DATA(TOutCard, objectSize, "TOutCard size of error!");
		_gameStatus=GS_PLAYING;
		TOutCard *pData= (TOutCard*)object;
		_currentOutCardPeople=pData->byUser;
		_byUserOutCard=pData->byCard;
		if (pData->byUser==_mySeatNo)
		{
			_userCardCount=pData->iHandNums;
			std::copy(pData->byCards, pData->byCards +pData->iHandNums, _userCardList);
			_gameLogic.sort(_userCardList,_userCardCount,false);
			BYTE seatNo = logicToViewSeatNo(_mySeatNo);
			std::vector<BYTE> cards(_userCardCount);
			std::copy(_userCardList, _userCardList + _userCardCount, cards.begin());
			_uiCallback->showUserHandCard(seatNo, cards, false, true,false);
			_uiCallback->setWaitTime(seatNo,0,false,0);
			std::vector<BYTE> card(1);
			card[0]=pData->byCard;
			_uiCallback->showUserHandCardOut(seatNo,card);
		}
		else
		{
			BYTE seatNo = logicToViewSeatNo(pData->byUser);
			std::vector<BYTE> card(1);
			card[0]=pData->byCard;
			_uiCallback->showUserHandCardOut(seatNo,card);
		}
		*/
	}

#pragma mark 服务器发送的指令 ---- 动作

    void GameTableLogic::testForGameAction(){
        this->dealGameActNotifyResp(nullptr,1);
    }
    
    void GameTableLogic::dealGameActNotifyResp(void* object, INT objectSize)
	{
		CHECK_SOCKET_DATA(CMD_S_OperateNotify, objectSize, "CMD_S_OperateNotify size of error!");
		_gameStatus=GS_PLAYING;
		CMD_S_OperateNotify *pData = (CMD_S_OperateNotify *)object;
     
        BYTE seatNo = logicToViewSeatNo(pData->wResumeUser);
        
        CCLOG("收到服务器发送的动作指令...   我的状态  : %d , wResumeUser : %d" , pData->cbActionMask , pData->wResumeUser);
        //_uiCallback->showActionBtn(seatNo ,pData->cbActionMask , pData->cbActionCard);
        //_uiCallback->setWaitTime(getNextPlayerSeatNo(), ACTION_TIME, true, 1);
        
        //如果是自己的动作
        if (pData->cbActionMask != ActionType::WIK_NULL) {
            
            if ((pData->cbActionMask & ActionType::WIK_CHI_HU) != 0) {
                this->sendActRequest(ActionType::WIK_CHI_HU,pData->cbActionCard);
            }else{
                _uiCallback->showActionBtn(seatNo ,pData->cbActionMask , pData->cbActionCard);
            }
        }else{
            _uiCallback->setWaitTime(seatNo, ACTION_TIME, true, 1);
        }
	}
    
    
    void GameTableLogic::dealGameActNotifyResultResp(void *object, INT objectSize){

       // CHECK_SOCKET_DATA(CMD_S_OperateResult, objectSize, "CMD_S_OperateResult size of error!");
        CMD_S_OperateResult *pData = (CMD_S_OperateResult *)object;
        
        CCLOG("收到服务器发送的动作指令回调...操作者为: %d   牌为 : %d" , pData->cbOperateCode , pData->cbOperateCard);
        
        BYTE seatNo = logicToViewSeatNo(pData->wOperateUser);
        BYTE outSeatNo = logicToViewSeatNo(pData->wProvideUser);

        MJActionData data;
        data.acSeatNo = seatNo;
        data.outSeatNo = outSeatNo;
        data.card = pData->cbOperateCard;
        data.bDarkGuang =pData->bDarkGuang;
        
        _uiCallback->showActionCard(data, (ActionType)pData->cbOperateCode);
        _uiCallback->setWaitTime(seatNo, OUTCARD_TIME, true, 0);
    }

	void GameTableLogic::dealGameAutoOutCardResp(void* object, INT objectSize)
	{
		/*
		CHECK_SOCKET_DATA(TuoGuan, objectSize, "TuoGuan size of error!");
		_gameStatus=GS_PLAYING;
		TuoGuan *pData = (TuoGuan *)object;
		if (pData->byDeskStation==_mySeatNo)
		{
			if (pData->info==TuoGuan::ActInfo::AF_START_TUOGUAN)
			{
				_iAutoOutCard=true;
				_uiCallback->setAutoOutCardBtn(false);
				_uiCallback->showCAutoOutCardBtn(true);
				if (_bOutCardLocker)
				{
					GameOutCardOverTime();
				}
				else
				{
					sendActRequest(MjActFlag::usGuo,0);
				}

			}
			else
			{
				_iAutoOutCard=false;
				_uiCallback->setAutoOutCardBtn(true);
				_uiCallback->showCAutoOutCardBtn(false);
			}
		}
		*/
	}


	//≥¨ ± ¥Ú≥ˆ≈∆
	void GameTableLogic::GameOutCardOverTime()
	{
		if (_bOutCardLocker)
		{

			if (!_iAutoOutCard)
			{
				_iOutTimeCount++ ;
				if (3 == _iOutTimeCount)
				{
					sendAutoOutCard(true);
					//_uiCallback->showCAutoOutCardBtn(true);
				}

		    }

			BYTE byOutCard = 255;
			//”–∑Á◊÷≈∆ ¥Ú≥ˆµ•∏ˆ∑Á
			/*if (_gameLogic.WzFengZiLocked(_mySeatNo, _userCardList,_userCardCount,_iCaiShenCard))
			{
				for (int i = 0; i < _userCardCount; i ++)
				{
					BYTE byTempCard = _userCardList[i];
					if (byTempCard>=31 && byTempCard<=37 && byTempCard != _iCaiShenCard)
					{
						byOutCard = byTempCard;
						break;
					}
				}
				
				for (int i = 0; i < _userCardCount; i ++)
				{
					BYTE byTempCard = _userCardList[i];
					if (byTempCard!=37 && byTempCard>=31 && byTempCard<=37 && byTempCard != _iCaiShenCard)
					{
						byOutCard = byTempCard;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < _userCardCount; i ++) {
					BYTE byTempCard = _userCardList[i];
					if (byTempCard != _iCaiShenCard) {
						byOutCard = byTempCard;
						break;
					}
				}
			}
			
			if ( _byUserOutCard>=31 && _byUserOutCard<=37 && _byUserOutCard != _iCaiShenCard) {
				for (int i = 0; i< _userCardCount; i ++) {
					BYTE byTempCard = _userCardList[i];
					if (byTempCard == _byUserOutCard) {
						byOutCard = _byUserOutCard;
						break;
					}
				}
			}
		
			if (byOutCard == 255) {
				int numCards = _userCardCount;
				byOutCard =_userCardList[numCards-1];
			}
			_bOutCardLocker=false;
			sendOutCard(byOutCard);*/
		}
	}

	void GameTableLogic::initParams()
	{
		memset(_playerSitted, 0, sizeof(_playerSitted));
		_iAutoOutCard=false;
		_waiting=false;
		_actionTypeCount=0;
		_byUserOutCard = 255;		//ÕÊº“¥Ú≥ˆµƒ≈∆
		_iOutTimeCount=0;
		_bOutCardLocker=false;
		_currentActCount=0;
        _isLastGame = false;
        _isLastGameEnd =false;
        
        for(int i = 0; i < PLAYER_COUNT; i++){
            _userReady[i] = false;
        }
	}

	void GameTableLogic::refreshParams()
	{
		_iAutoOutCard=false;
		_waiting=false;
		_actionTypeCount=0;
		_byUserOutCard = 255;
		_iOutTimeCount=0;
		_bOutCardLocker=false;
		_currentActCount=0;
        _isLastGame = false;
        _isLastGameEnd =false;
        
        for(int i = 0; i < PLAYER_COUNT; i++){
            _userReady[i] = false;
        }
    }

	void GameTableLogic::enterGame()
	{
		loadDeskUsers();

		if ((INVALID_DESKSTATION == _mySeatNo) && !_autoCreate)
		{
			for(int i = 0; i < PLAYER_COUNT; i++)
			{
				if(!_existPlayer[i])
				{
					sendUserSit(logicToViewSeatNo(i));
					break;
				}
			}
		}
		else
		{
			setSeatOffset(_mySeatNo);
			loadUsers();
				
			if ((INVALID_DESKSTATION != _mySeatNo) && _autoCreate)
			{
				sendGameInfo();
			}
		}
	}

	void GameTableLogic::loadUsers()
	{
		BYTE seatNo = INVALID_DESKNO;
		for(int i = 0; i < PLAYER_COUNT; i++)
		{
			UserInfoStruct* pUser = _deskUserList->getUserByDeskStation(i);
			if(_existPlayer[i] && pUser != nullptr)
			{
				_playerSitted[i] = true;
				seatNo = logicToViewSeatNo(i);
				_uiCallback->addUser(seatNo, i == _mySeatNo);
				_uiCallback->setUserName(seatNo, pUser->nickName);
				_uiCallback->setUserMoney(seatNo, 0);
				_uiCallback->showUserMoney(seatNo, true);
				if (pUser->bUserState == USER_ARGEE)
				{
					_userReady[i] = true;
					_uiCallback->showUserReady(seatNo, true);
                }else{
                    _userReady[i] = false;
                }
			}
		}
        
	}

	void GameTableLogic::sendUserUp()
	{
		if (RoomLogic()->connected())
		{
			YZGameLogicBase::sendUserUp();
		}
		else
		{
			_uiCallback->leaveDesk();
		}
	}

	void GameTableLogic::sendForceQuit()
	{
		if (RoomLogic()->connected())
		{
			YZGameLogicBase::sendForceQuit();
		}
		_uiCallback->leaveDesk();
	}

	void GameTableLogic::clearDesk()
	{
		YZGameLogicBase::clearDesk();

		_uiCallback->clearDesk();
		 _uiCallback->hideBtnType();
		 //_uiCallback->showCaiShenCard(0,false);
		 //_uiCallback->showActionResult(MjActFlag::usWaitting,0,0,false);
		 if (_iAutoOutCard)
		 {
			 sendAutoOutCard(false);
		 }
		 refreshParams();
	}

	void GameTableLogic::sendShowStart()
	{
		if(INVALID_DESKSTATION == _mySeatNo) return;

		TAgree data;
		data.bAgree=true;
		data.byUser=_mySeatNo;
		sendData(MDM_GM_GAME_NOTIFY, ASS_GM_AGREE_GAME, (void*)&data, sizeof(data));
	}

	void GameTableLogic::sendMakeDI(bool iBanker,bool isMake)
	{/*
		if(INVALID_DESKSTATION == _mySeatNo) return;

		tagMaiDiTong data;
		data.byUser=_mySeatNo;
		data.bNt=iBanker;
		if (iBanker)
		{
			
			if (isMake)
			{
				sendData(MDM_GM_GAME_NOTIFY, ASS_MAKE_MAI, (void*)&data, sizeof(data));
			}
			else
			{
				sendData(MDM_GM_GAME_NOTIFY, ASS_QUIT_MAI_DING, (void*)&data, sizeof(data));
			}
			
		}
		else
		{
			
			if (isMake)
			{
				sendData(MDM_GM_GAME_NOTIFY, ASS_MAKE_DING, (void*)&data, sizeof(data));
			}
			else
			{
				sendData(MDM_GM_GAME_NOTIFY, ASS_QUIT_MAI_DING, (void*)&data, sizeof(data));
			}
			
		}
		*/
	}


	void GameTableLogic::sendQiPaiFinish()
	{
		/*
		sendData(MDM_GM_GAME_NOTIFY, ASS_FETCH_METHOD_FINISH,NULL ,0);  
		*/
	}

	void GameTableLogic::sendAutoOutCard(bool iAuto)
	{

		if(INVALID_DESKSTATION == _mySeatNo) return;

		CMD_C_Trustee data;
		data.bTrustee=iAuto;
	
		
		sendData(MDM_GM_GAME_NOTIFY, SUB_C_TRUSTEE, (void*)&data, sizeof(data));
	}

	void GameTableLogic::sendOutCard(BYTE val)
	{
        
        BYTE cbColor=(val&MASK_COLOR)>>4;
        BYTE cbValue=(val&MASK_VALUE);
        
        CCLOG("向服务器发送一个出牌请求  花色: %d  值 : %d" ,cbColor,cbValue);
        
		if(INVALID_DESKSTATION == _mySeatNo) return;
		CMD_C_OutCard  data;
        data.cbCardData = val;
		sendData(MDM_GM_GAME_NOTIFY, SUB_C_OUT_CARD, (void*)&data, sizeof(data));
		
        std::vector<BYTE> cards;
        cards.clear();
        _uiCallback->showListenerCard(false, cards);
	}

	void GameTableLogic::sendActRequest(BYTE type,BYTE card)
	{
		if(INVALID_DESKSTATION == _mySeatNo) return;
		CMD_C_OperateCard  data;
	    data.cbOperateCode = type;
        data.cbOperateCard = card;
		
		sendData(MDM_GM_GAME_NOTIFY, SUB_C_OPERATE_CARD, (void*)&data, sizeof(data));
	}

	void GameTableLogic::sendThinkRequest(int type)
	{
		/*
		if(INVALID_DESKSTATION == _mySeatNo) return;
		ThinkSelectStruct data;
		data.bThinkS = type;
		sendData(MDM_GM_GAME_NOTIFY, ASS_THINK_SELECT, (void*)&data, sizeof(data));	
		*/
	}
    
    void GameTableLogic::sendChatTalkWithId(int id ,std::string msg)
    {
        MSG_GR_RS_NormalTalk data;
        memset(&data,0,sizeof(MSG_GR_RS_NormalTalk));
        
        data.nDefaultIndex = id;
        data.dwSendID = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
        strcpy(data.szMessage,msg.c_str());
        data.iLength = msg.length();
        sendData(MDM_GM_GAME_FRAME, ASS_GM_NORMAL_TALK, &data, sizeof(data)-sizeof(data.szMessage)+data.iLength+1);
    }
    
    void GameTableLogic::onDisConnect()
    {
        _uiCallback->onGameDisConnected();
    }
    
    void GameTableLogic::setCurrentPlaySeatNo(BYTE seatNo){
        _currentSeatNo = seatNo;
    }

    BYTE GameTableLogic::getNextPlayerSeatNo(){
        BYTE next = _currentSeatNo + 1;
        if (next > 3) {
            next = 0;
        }
        return next;
    }
    
    BYTE GameTableLogic::getLeftPlayerSeatNo(){
        BYTE left = _currentSeatNo - 1;
        if (left < 0) {
            left = 3;
        }
        return left;
    }
    
    
    bool GameTableLogic::checkUserIsDeskOwner(){
        //return RoomLogic()->getDeskOwner(_deskNo) == getUserId(_mySeatNo);
        return _mySeatNo == 0;
    }
    
    
    void GameTableLogic::sendDisbandRoomResp(bool isDisband){
        MSG_GR_C_BACKROOM_REQ_DISMISS msg;
        memset(&msg, 0, sizeof(msg));
        msg.dwInitiator = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
        msg.uDeskIndex = RoomLogic()->loginResult.pUserInfoStruct.bDeskNO;
        msg.uRoomID = RoomLogic()->getSelectedRoom()->uRoomID;
        sendData(MDM_GM_GAME_NOTIFY, ASS_GR_DISMISS_BACKROOM_REQ, &msg, sizeof(msg));
    }
    
    
    void GameTableLogic::sendAgreeDisbandRoom(bool isDisband){
        MSG_GR_C_BACKROOM_RSP_DISMISS msg;
        memset(&msg, 0, sizeof(msg));
        msg.dwRespondent = RoomLogic()->loginResult.pUserInfoStruct.dwUserID;
        msg.uDeskIndex = RoomLogic()->loginResult.pUserInfoStruct.bDeskNO;
        msg.uRoomID = RoomLogic()->getSelectedRoom()->uRoomID;
        msg.cbAgree = isDisband;
        sendData(MDM_GM_GAME_NOTIFY, ASS_GR_DISMISS_BACKROOM_AGREE, &msg, sizeof(msg));
    }
    
	void GameTableLogic::autoOutCheck(BYTE vSeatNo, const std::vector<BYTE>& cards)
	{

		_uiCallback->setUpCardList(vSeatNo, cards);
		
	}
    
    BYTE GameTableLogic::getLogicViewSeatNoByLocal(BYTE localSeatNo){
        return viewToLogicSeatNo(localSeatNo);
    }
    

	void GameTableLogic::setSeatOffset(BYTE seatNo)
	{
		_seatOffset = -seatNo;
	}
    
    UserInfoStruct* GameTableLogic::getGameUserByDeskStation(BYTE vSeatNo)
    {
        if (_gameUsers.size() != 4) {
            jzError("Game User size not 4...");
            return NULL;
        }
        BYTE lSeatNo = viewToLogicSeatNo(vSeatNo);
        return _gameUsers.at(lSeatNo);
    }
}
