#ifndef __GuessCar_Game_Table_Logic_H__
#define __GuessCar_Game_Table_Logic_H__

#include "YZNetExport.h"
#include "ErMahjongMessageHead.h"
#include "ErMahjongGameLogic.h"

namespace ErMahjong
{
	class GameTableUICallback;
	class GameTableLogic;

	class GameTableLogic: public YZ::YZGameLogicBase
	{
	public:
		GameTableLogic(GameTableUICallback* uiCallback, BYTE deskNo, bool bAutoCreate);
		virtual ~GameTableLogic();	
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
	public:
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree)override;
		virtual void dealGameStartResp(BYTE bDeskNO)override;
	    virtual void dealGameEndResp(BYTE deskNo)override;
	    virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)override;
	    virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& user)override;
	    virtual void dealUserUpResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)override;
	    virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo)override;
	    virtual void dealGameStationResp(void* object, INT objectSize)override;
	    virtual void onGamePointMessage(void* object, INT objectSize)override;
        
        virtual void dealUserChatMessage(void* object, INT objectSize)override;
        
        virtual void onDisConnect() override;
        
        void sendDisbandRoomResp(bool isDisband);
        void sendAgreeDisbandRoom(bool isDisband);
        
        
        bool getUserIsAgree(BYTE seatNo);
        
        void testForGameAction();
        
        UserInfoStruct* getGameUserByDeskStation(BYTE vSeatNo);
        BYTE getLogicViewSeatNoByLocal(BYTE localSeatNo);
	protected:
		void dealGameShaiZiResp(void * object , INT objectSize);
		void dealGameBeginResp(void* object, INT objectSize);
		void dealGameOutCardResp(void* object, INT objectSize);
		void dealGameGetCardResp(void* object, INT objectSize);
		void dealGameUserOutCardsResp(void* object, INT objectSize);
		void dealGameActNotifyResp(void* object, INT objectSize);
        void dealGameActNotifyResultResp(void* object, INT objectSize);        

		void dealGameAutoOutCardResp(void* object, INT objectSize);
		void dealGameQuitResp(void* object, INT objectSize);
        
        void dealThisGameEndResp(void *object , INT objectSize);
        
        void dealGameTrusteeResp(void *object , INT objectSize);
        
        void dealGoOnGameResp(void * object ,INT objectSize);
		
        void dealListenerCardResp(void * object ,INT objectSize);
        
        void dealAsk2DisbandRoomResp(void * object , INT objectSize);  //请求解散房间消息

      
    protected:
        virtual void initParams();
        virtual void refreshParams();
	public:
		void enterGame();
	    void loadUsers();

		virtual void sendUserUp() override;
		virtual void sendForceQuit() override;
		virtual void clearDesk() override;

		void sendShowStart();
		void sendMakeDI(bool iBanker,bool isMake);
		void sendQiPaiFinish();
		void sendAutoOutCard(bool iAuto);
		void sendOutCard(BYTE val);
		void sendActRequest(BYTE type,BYTE card);
		void sendThinkRequest(int type);
        void sendChatTalkWithId(int id , std::string msg);

		void autoOutCheck(BYTE vSeatNo, const std::vector<BYTE>& cards);
		void GameOutCardOverTime();
        
        void setCurrentPlaySeatNo(BYTE seatNo);
        
        BYTE getLeftPlayerSeatNo();
        
        bool checkUserIsDeskOwner();
	protected:
		void setSeatOffset(BYTE seatNo);
        BYTE getNextPlayerSeatNo();
        
        std::map<BYTE , UserInfoStruct*> _gameUsers;
        
	private:
		GameTableUICallback*	_uiCallback;
		GameLogic        _gameLogic;
		bool _playerSitted[PLAYER_COUNT];
		bool _userReady[PLAYER_COUNT];
		bool _iAutoOutCard;
		int _userCardCount[PLAYER_COUNT];
	    BYTE _userCardList[PLAYER_COUNT][MJ_MAX_HANDCARD];
		int _actionTypeCount;
		BYTE _currentOutCardPeople;
		int _iCaiShenCard;
		BYTE _byUserOutCard;									//ÕÊº“¥Ú≥ˆµƒ≈∆
		BYTE _currentBanker;
        BYTE _currentSeatNo;
		int _iOutTimeCount;
		bool _bOutCardLocker;
		int _currentActCount;
	public:
		bool _waiting;
        bool _isLastGameEnd;
        bool _isLastGame;
    };
}


#endif