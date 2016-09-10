#include "YZRoomEventLayer.h"
#include "YZRoom/YZRoomLogic.h"
#include "YZCommon/YZLog.h"

namespace YZ {

	YZRoomEventLayer::YZRoomEventLayer(void)
	{
	}

	YZRoomEventLayer::~YZRoomEventLayer(void)
	{
	}
	 
	void YZRoomEventLayer::onEnter()
	{
		YZLayer::onEnter();
		RoomLogic()->addRoomObserver(this);
	}

	void YZRoomEventLayer::onExit()
	{
		YZLayer::onExit();
		RoomLogic()->removeRoomObserver(this);
	}

	void YZRoomEventLayer::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		YZRoomLogic::getInstance()->sendData(MainID, AssistantID, object, objectSize);
	}

	void YZRoomEventLayer::sendUserSit(void* object, INT objectSize)
	{
		sendData(MDM_GR_USER_ACTION, ASS_GR_USER_SIT, object, objectSize);
	}

	void YZRoomEventLayer::sendUserUp()
	{
		sendData(MDM_GR_USER_ACTION, ASS_GR_USER_UP);
	}

	//////////////////////////////////////////////////////////////////////////
	// 网络断开
	void YZRoomEventLayer::onDisConnect()
	{
		YZLOG_DEBUG("onDisConnect");
	}

	// 房间连接成功
	void YZRoomEventLayer::onConnectMessage(bool result)
	{
		YZLOG_DEBUG("onConnectMessage");
	}

	// 房间登录成功
	void YZRoomEventLayer::onLoginMessage(bool result, UINT dwErrorCode)
	{
		YZLOG_DEBUG("onLoginMessage");
	}

	// 房间登录完成
	void YZRoomEventLayer::onLoginFinishMessage()
	{
		YZLOG_DEBUG("onLoginFinishMessage");
	}

	// 用户进入房间
	void YZRoomEventLayer::onUserComeMessage(UserInfoStruct* user)
	{
		YZLOG_DEBUG("onUserComeMessage");
	}

	// 用户离开房间
	void YZRoomEventLayer::onUserLeftMessage(UserInfoStruct* user)
	{
		YZLOG_DEBUG("onUserLeftMessage");
	}

	// 排队用户坐下
	void YZRoomEventLayer::onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users)
	{
		YZLOG_DEBUG("onQueueUserSitMessage");
	}

	// 用户坐下
	void YZRoomEventLayer::onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user)
	{
		YZLOG_DEBUG("onUserSitMessage");
	}

	// 用户站起
	void YZRoomEventLayer::onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user)
	{
		YZLOG_DEBUG("onUserUpMessage");
	}

	// 用户断线
	void YZRoomEventLayer::onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE bDeskStation)
	{
		YZLOG_DEBUG("onUserCutMessage");
	}

	// 房间其他消息
	bool YZRoomEventLayer::onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize)
	{
		YZLOG_DEBUG("onRoomFrameMessage");
		return false;
	}

	// 金币结算消息
	void YZRoomEventLayer::onGamePointMessage(void * object, INT objectSize)
	{
		YZLOG_DEBUG("onGamePointMessage");
	}

	// 比赛奖励
	void YZRoomEventLayer::onGameContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards)
	{
		YZLOG_DEBUG("onGameContestAwards");
	}

	// 报名数量
	void YZRoomEventLayer::onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople)
	{
		YZLOG_DEBUG("onGameContestPeople");
	}

	// 个人参赛纪录
	void YZRoomEventLayer::onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord)
	{
		YZLOG_DEBUG("onGameContestRecord");
	}

	//////////////////////////////////////////////////////////////////////////
	// 聊天消息
	void YZRoomEventLayer::onUserChatMessage(void* object, INT objectSize)
	{
		YZLOG_DEBUG("onUserChatMessage");
	}
	void YZRoomEventLayer::onGameChatMessage(void* object, INT objectSize)
	{
		YZLOG_DEBUG("onGameChatMessage");
	}
    
    void YZRoomEventLayer::onBackRoomMessage(MSG_GR_BACKROOM_STATUS *data){
        YZLOG_DEBUG("onBackRoomMessage");
    }

    //使用房卡消息
    void YZRoomEventLayer::onUseRoomCardMessage(MSG_PROP_S_OP_ROOMCARD * data){ YZLOG_DEBUG("onUseRoomCardMessage");}
    //创建房间消息
    void YZRoomEventLayer::onCreateBackRoomMessage(MSG_BACKROOM_INFO * data){ YZLOG_DEBUG("onCreateBackRoomMessage");}
    //查询房间消息
    void YZRoomEventLayer::onQueryBakcRoomMessage(MSG_QueryBACKROOM_RSP_INFO * data , BYTE code){ YZLOG_DEBUG("onQueryBakcRoomMessage");}
    //查询房卡信息
    void YZRoomEventLayer::onQueryPropRoomCardInfo(MSG_PROP_S_QUERY_ROOMCARD * data , BYTE bHandleCode) {YZLOG_DEBUG("onQueryPropRoomCardInfo");};
    void YZRoomEventLayer::onQueryPropKindMessage(MSG_PROP_S_KIND_ROOMCARD * data , BYTE bHandleCode ) {YZLOG_DEBUG("onQueryPropKindMessage");}

    void YZRoomEventLayer::onBuyRoomCardMessage(MSG_PROP_S_BUY_ROOMCARD * data){YZLOG_DEBUG("onBuyRoomCardMessage");};

	//void YZRoomEventLayer::onGameDeskMessage(MSG_GR_DeskStation * desk)
	//{
	//	YZLOG_DEBUG("onGameDeskMessage");
	//}

}