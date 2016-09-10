#ifndef __YZ_PlatformNotifyDelegate_H__
#define __YZ_PlatformNotifyDelegate_H__

#include "YZBaseType.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include <string>

namespace YZ
{
	class IPlatformNotifyDelegate
	{
	public:
		virtual ~IPlatformNotifyDelegate() {}
		// 网络断线
		virtual void onHandleDisConnect() {}
		// 房间列表处理消息
 		virtual void onHandleRoomListMessage() {}
		// 游戏列表处理消息
		virtual void onHandleGameListMessage() {}
 		// 平台连接处理消息
		virtual void onHandleConnectMessage(bool result) {}
 		// 平台登录处理消息
		virtual void onHandleLoginMessage(bool result, UINT dwErrorCode) {}
		// 平台游戏通知
		virtual void onHandlePlatformNotifyMessage(const std::string& message) {}
		// 平台在线总人数更新消息处理
 		virtual void onHandlePlatformUserCountMessage(UINT uOnLineCount) {}
		// 平台游戏在线人数更新消息处理
		virtual void onHandleGameUserCountMessage(DL_GP_RoomListPeoCountStruct* peopleCountStruct) {}
		// 平台注册
		virtual void onHandleUserRegisterMessage(void* registerStruct, UINT ErrorCode) {}
		// 平台上传图片
		virtual void onHandleLogoUploadMessage(NetMessageHead* pNetHead, void* pNetData, UINT uDataSize) {}
	};
}

#endif	//__YZ_PlatformNotifyDelegate_H__

