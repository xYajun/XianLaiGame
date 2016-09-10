#ifndef __YZ_RoomNotifyDelegate_H__
#define __YZ_RoomNotifyDelegate_H__

#include "YZBaseType.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include <vector>

namespace YZ
{
	class IRoomMessageDelegate	: public IGameMatchMessageDelegate
								, public IGameChartMessageDelegate
								, public IUserActionMessageDelegate
	{
	public:
		virtual ~IRoomMessageDelegate() {}
		// 网络断开
		virtual void onDisConnect() = 0;
		// 房间连接成功
		virtual void onConnectMessage(bool result) = 0;
		// 房间登录成功
		virtual void onLoginMessage(bool result, UINT dwErrorCode) = 0;
		// 房间登录完成
		virtual void onLoginFinishMessage() = 0;
		// 用户进入房间
		virtual void onUserComeMessage(UserInfoStruct* user) = 0;
		// 用户离开房间
		virtual void onUserLeftMessage(UserInfoStruct* user) = 0;
		// 房间其他消息
		virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) = 0;
		// 结算消息
		virtual void onGamePointMessage(void * object, INT objectSize) = 0;

        //返回房间消息
        virtual void onBackRoomMessage(MSG_GR_BACKROOM_STATUS* data) = 0;
        //使用房卡消息
        virtual void onUseRoomCardMessage(MSG_PROP_S_OP_ROOMCARD * data) = 0;
        //创建房间消息
        virtual void onCreateBackRoomMessage(MSG_BACKROOM_INFO * data) = 0;
        //查询房间消息
        virtual void onQueryBakcRoomMessage(MSG_QueryBACKROOM_RSP_INFO * data , BYTE code) = 0;
        
        virtual void onQueryPropKindMessage(MSG_PROP_S_KIND_ROOMCARD * data , BYTE bHandleCode ) = 0;
        
        //查询房卡信息
        virtual void onQueryPropRoomCardInfo(MSG_PROP_S_QUERY_ROOMCARD * data , BYTE bHandleCode) = 0;

        //购买房卡
        virtual void onBuyRoomCardMessage(MSG_PROP_S_BUY_ROOMCARD * data) = 0;
        
		//virtual void onGameDeskMessage(MSG_GR_DeskStation * desk) =0;
	};
}

#endif	//__YZ_RoomNotifyDelegate_H__

