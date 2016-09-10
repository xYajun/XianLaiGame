#ifndef __YZ_YZPlatformLogic_H__
#define __YZ_YZPlatformLogic_H__

#include "YZCommon/YZCommonMarco.h"
#include "YZCommon/YZUIDelegateQueue.h"
#include "YZSocket/YZSocketMessageDelegate.h"
#include "YZSocket/YZSocketMessage.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include <vector>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio;

#define FRIEND_REQUST_ADD		1001
#define FRIEND_REQUST_CANCEL	1002


namespace YZ
{
	class IPlatformNotifyDelegate;
	class YZSocketLogic;

	typedef std::function<bool (IPlatformNotifyDelegate *)> PlatformNotifyFun;

	class YZPlatformLogic : public Ref, ISocketMessageDelegate 
	{

	public:
		// returns the login information platform
		MSG_GP_R_LogonResult		loginResult;

	public:

	public:
		static YZPlatformLogic* getInstance();

	public:
		bool init();

	public:
		// connect platform function
		bool connectPlatform();

		// close platform function
		bool closePlatform();
		
		// platform connecting success function
		bool connected() const;

		// platform login success function
		bool login() const;

		// platform login is success function
		bool loginSuccess() const;
		
		// send data
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

		// send data
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);

		// set handler callback event
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// remove handler callback event
		void removeEventSelector(UINT MainID, UINT AssistantID);

	public:
		// add a network event viewer function
		void addObserver(IPlatformNotifyDelegate* delegate);

		// delete a network event viewer function
		void removeObserver(IPlatformNotifyDelegate* delegate);

	public:

		void requestGameListInfo();
		// get the room list function
		void requestRoomListInfo(UINT uKindID, UINT uNameID);

		// platform login function
		void platformLogin(const std::string& name, const std::string& password, UINT uNameID);

		// platform register
		void platformRegister(bool isFastRegister, UINT uid, const std::string& name, const std::string& password, const std::string& nickname, int sex, const std::string& oldpawd ,int _headImgId, const std::string szUID="");

		void onBtnClicked(Ref* pSender, Widget::TouchEventType type);

	public:
		virtual void onConnected(bool connect, emSocketStatus status) override;
		
		virtual void onDisConnect() override;
		
		virtual void onSocketMessage(YZSocketMessage* socketMessage) override;

	private:
		void onConnectConterMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// platform connecting processing delegate
		void onConnectMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// platform login process delegate
		void onLoginMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// the room list processing
		void onRoomListMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// update the online user information
		void onUpdateOnLineUserMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// platform register
		void onUserRegisterMessage(NetMessageHead* messageHead, void* object, INT objectSize);

		// platform logo upload
		void onUserUploadMessage(NetMessageHead* messageHead, void* object, UINT objectSize);

		// platform friend inform
		void onFriendInformMessage(NetMessageHead* messageHead, void* object, UINT objectSize);
		
		// platform boardcast inform
		void onBoardcastMessage(NetMessageHead* messageHead, void* object, UINT objectSize);

		void onTransferMessage(NetMessageHead* messageHead, void* object, UINT objectSize);

		// the distribution of the platform news
		bool dispatchMessage(const PlatformNotifyFun& func)
		{
			bool ret = false;
			for (auto iter = _notifyQueue->begin(); iter != _notifyQueue->end(); ++iter)
			{
				ret = func(iter->second);
				if (ret)
				{
					return true;
				}
			}
			return false;
		}

	private:
		void heartBeat(float dt);

	private:
		YZUIDelegateQueue<IPlatformNotifyDelegate*>* _notifyQueue;

		YZSocketLogic*	_socketLogic;

		bool			_loginResult;

		bool			_platformConnected;

		//
		bool			_platformLogin;

		INT				_heartBeatFrequency;

		std::string		_platformAddress;

		INT				_platformPort;

		bool			_platformConterServer;

		Layout*			f_prompt;
		int				_requestUserID;
	private:
		YZPlatformLogic(void);
		virtual ~YZPlatformLogic(void);

	};

	#define  PlatformLogic()	YZPlatformLogic::getInstance()
}

#endif	//__YZ_YZPlatformLogic_H__

