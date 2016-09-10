#ifndef __YZ_SocketLogic_H__
#define __YZ_SocketLogic_H__

#include "YZCommon/YZCommonMarco.h"
#include "YZCommon/YZUIDelegateQueue.h"
#include "YZSocketMessage.h"
#include "YZSocketEventDelegate.h"
#include "YZSocketSelector.h"

#include <list>

#include "cocos2d.h"
USING_NS_CC;

namespace YZ
{
	class YZSocketThread;
	class ISocketMessageDelegate;
	class YZSocketSelectorItem;

	class YZSocketLogic : public Ref, ISocketEventDelegate 
	{
		typedef YZSocketSelector<std::string> YZSocketMessageSelectorHandler;

	public:
		YZSocketLogic(ISocketMessageDelegate* delegate, const std::string& tag);
		virtual ~YZSocketLogic(void);

	public:
		bool init();

		bool connected() const;

	public:
		// connect server
		bool openWithIp(const CHAR* ip, INT port);

		// connect server
		bool openWithHost(const CHAR* host, INT port);

		// close socket
		bool close();

		// send data 
		INT send(UINT MainID, UINT AssistantID, UINT CheckCode, void* object, INT objectSize);

		// set handler callback event
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// remove handler callback event
		void removeEventSelector(UINT MainID, UINT AssistantID);

		// send a heartbeat packet
		void heartBeat(UINT MainID, UINT AssistantID, UINT CheckCode);
		
		void resetData();

	public:
		//
		virtual void onReadComplete() override;

		// reading network data delegate
		//virtual void onReadSocketData(YZSocketMessage* socketMessage) override;
		
		// the server connection success delegate
		//virtual void onConnected(bool connect, emSocketStatus status) override;
		
		// disconnect the network delegate
		//virtual void onDisConnect() override;

	private:
		/*
		* 网络消息通知处理函数 
		*/
		void socketDataDispatch(float dt);

	private:
		YZSocketThread*					_socketThread;

		YZSocketMessageSelectorHandler*	_selectorQueue;

		ISocketMessageDelegate*			_delegate;
	};
}

#endif	//__YZ_SocketLogic_H__

