#ifndef __YZ_YZPlatformEventLayer_H__
#define __YZ_YZPlatformEventLayer_H__

#include "YZPlatform/YZPlatformNotifyDelegate.h"
#include "YZSocket/YZSocketMessage.h"
#include "YZUIExport.h"

namespace YZ {

	class YZPlatformEventLayer : public YZLayer, IPlatformNotifyDelegate
	{
	public:
		YZPlatformEventLayer(void);
		virtual ~YZPlatformEventLayer(void);

	public:
		virtual void onEnter() override;

		virtual void onExit() override;

	public:
		// send data
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

		// send data
		void sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize, SEL_SocketMessage selector);

		// set handler callback event
		void addEventSelector(UINT MainID, UINT AssistantID, SEL_SocketMessage selector);

		// remove handler callback event
		void removeEventSelector(UINT MainID, UINT AssistantID);

	protected:
		// get the room list function
		void requestRoomListInfo(UINT uKindID, UINT uNameID);

		// platform login function
        void platformLogin(const std::string& name, const std::string& password, UINT uNameID,const std::string szUID , const std::string hearUrl);

		void platformCheckLogin(const std::string& name);

		// platform register
		void platformRegister(bool isFastRegister, UINT uid, const std::string& name, const std::string& password, const std::string& nickname, const std::string& oldpawd = " ");

	};
};

#endif	//__YZ_YZPlatformEventLayer_H__
