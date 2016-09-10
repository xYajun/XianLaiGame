#ifndef __YZ_YZRoomLogic_H__
#define __YZ_YZRoomLogic_H__

#include "YZCommon/YZCommonMarco.h"
#include "YZCommon/YZUIDelegateQueue.h"
#include "YZSocket/YZSocketMessageDelegate.h"
#include "YZSocket/YZSocketMessage.h"
#include "YZNetProtocol/YZProtocolExport.h"

#include "cocos2d.h"

USING_NS_CC;

namespace YZ
{
	class IGameMessageDelegate;
	class IRoomMessageDelegate;
	class YZSocketLogic;
	
	// room message distribution statement a callback function
	typedef std::function<bool (IRoomMessageDelegate*)> RoomFrameMessageFun;

	// game message distribution statement a callback function
	typedef std::function<bool (IGameMessageDelegate*)> GameMessageFun;

	// the room network processing logic class
	class YZRoomLogic : public Ref, ISocketMessageDelegate 
	{
	public:
		static YZRoomLogic* getInstance();

	public:
		// the initialization data
		bool init();

		// connecting room
		bool connectRoom(const CHAR* ip, INT port);
		
		// close room
		bool closeRoom();

		bool connected() const;

		bool login() const;

		ComRoomInfo* getSelectedRoom() const
		{
			return _selectedRoom;
		}

		void setSelectedRoom(ComRoomInfo* room)
		{
			_selectedRoom = room;
		}

		UINT getRoomRule() const
		{
			return _gameRoomRule;
		}

		void setRoomRule(UINT roomRule)
		{
			_gameRoomRule = roomRule;
		}
        
        void setInRoomNum(std::string roomNum){
            _inRoomNum = roomNum;
        }
        
        std::string getInRoomNum()const
        {
            return _inRoomNum;
        }
        
        int getRoomZMCount() const
        {
            return _roomZMCount;
        }
        
        void setRoomZMCount(int roomZm)
        {
            _roomZMCount = roomZm;
        }
        
        void setRoomGameRoundInfo(int maxCount , int playCount)
        {
            _gameRounds = maxCount;
            _playGameRound = playCount;
        }
        
        int getGameMaxRound() const
        {
            return _gameRounds;
        }
        
        int getGamePlayRound() const
        {
            return _playGameRound;
        }
        
        void addDeskOwnerInfo(int deskId , int userId);
        void removeDeskOwnerInfo(int deskId);
        int getDeskOwner(int deskId);
	public:
		INT sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);
		// send data
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);
		// set handler callback event
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// remove handler callback event
		void removeEventSelector(UINT MainID, UINT AssistantID);
	public:
		// the logon server room
		void roomLogin(UINT uGameID);

	public:
		// add game event viewer
		void addObserver(IGameMessageDelegate* delegate);

		// remove game event viewer
		void removeObserver(IGameMessageDelegate* delegate);

		// add room event viewer
		void addRoomObserver(IRoomMessageDelegate* delegate);

		//  remove room event viewer
		void removeRoomObserver(IRoomMessageDelegate* delegate);


	public:
		virtual void onSocketMessage(YZSocketMessage* socketMessage) override;

		virtual void onConnected(bool connect, emSocketStatus status) override;

		virtual void onDisConnect() override;

	private:
		// connection room  processing
		void onConnectMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// login room treatment
		void onLoginMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// user list processing
		void onUserListMessage(NetMessageHead* messageHead, void* object, INT objectSize);
		
		// user action processing 
		void onUserActionMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// room message processing
		void onRoomMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// room message processing
		void onMatchMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		void onGameServerFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		void printfSitError(NetMessageHead* messageHead);

        void update(float dt);
	private:
		// distribution of the framework of the game news
		bool dispatchFrameMessage(const RoomFrameMessageFun& func)
		{
			bool ret = false;
			for (auto iter = _roomNotifyQueue->begin(); iter != _roomNotifyQueue->end(); ++iter)
			{
				ret = func(iter->second);
				if (ret)
				{
					return true;
				}
			}
			return false;
		}

		// distributed game news
		bool dispatchGameMessage(const GameMessageFun& func)
		{
			for (auto iter = _gameNotifyQueue->begin(); iter != _gameNotifyQueue->end(); ++iter)
			{
				bool ret = func(iter->second);
				if (ret)
				{
					return true;
				}
			}
			return false;
		}

	public:
		// table state
		MSG_GR_DeskStation	deskStation;
		// the room log results
		MSG_GR_R_LogonResult loginResult;

	private:
		// 当前选中的房间
		ComRoomInfo*	_selectedRoom;
		// the logic of the network processing
		YZSocketLogic*	_socketLogic;
		// ip
		std::string		_serverIp;
		// port
		INT				_serverPort;
		//
		bool			_roomConnected;
		//
		bool			_roomLogin;
        
        int             _roomZMCount;
        int				_gameRounds;
        int				_playGameRound;
		UINT			_gameRoomRule;	// 游戏房间规则
        
        int             _CheckTimes;
        
        std::string     _inRoomNum; //进入的房间号
        
		// game notification message queue
		YZUIDelegateQueue<IGameMessageDelegate*> *	_gameNotifyQueue;
		// room notification message queue
		YZUIDelegateQueue<IRoomMessageDelegate*> *	_roomNotifyQueue;

        //key 桌号 ， Value ： 桌主用户ID
        std::map<int , int > _roomDeskOwner;        //房间中，房间的拥有者信息
        
	private:
		YZRoomLogic(void);
		virtual ~YZRoomLogic(void);
	};

#define RoomLogic() YZRoomLogic::getInstance()
}

#endif	//__YZ_YZPlatformLogic_H__

