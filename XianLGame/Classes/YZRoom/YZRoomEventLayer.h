#ifndef __YZ_YZRoomEventLayer_H__
#define __YZ_YZRoomEventLayer_H__

#include "YZRoomMessageDelegate.h"
#include "YZUIExport.h"

namespace YZ {

	class YZRoomEventLayer : public YZLayer, IRoomMessageDelegate
	{
	public:
		YZRoomEventLayer(void);
		virtual ~YZRoomEventLayer(void);

	public:
		virtual void onEnter() override;

		virtual void onExit() override;
        
	public:
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

		void sendUserSit(void* object, INT objectSize);

		void sendUserUp();

	public:
		// 网络断开
		virtual void onDisConnect() override;
		// 房间连接成功
		virtual void onConnectMessage(bool result) override;
		// 房间登录成功
		virtual void onLoginMessage(bool result, UINT dwErrorCode) override;
		// 房间登录完成
		virtual void onLoginFinishMessage() override;
		// 用户进入房间
		virtual void onUserComeMessage(UserInfoStruct* user) override;
		// 用户离开房间
		virtual void onUserLeftMessage(UserInfoStruct* user) override;
		// 房间其他消息
		virtual bool onRoomFrameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
		// 结算消息
		virtual void onGamePointMessage(void * object, INT objectSize) override;

		/*
		* user aciton intertace
		*/
		// 排队用户坐下
		virtual void onQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users) override;
		// 用户坐下
		virtual void onUserSitMessage(MSG_GR_R_UserSit * userSit, UserInfoStruct* user) override;
		// 用户站起
		virtual void onUserUpMessage(MSG_GR_R_UserUp * userUp, UserInfoStruct* user) override;
		// 用户断线
		virtual void onUserCutMessage(INT dwUserID, BYTE bDeskNO, BYTE	bDeskStation) override;

		//////////////////////////////////////////////////////////////////////////
		// 比赛接口
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 转发到游戏
		//////////////////////////////////////////////////////////////////////////
		// 比赛初始化
		virtual void onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange) override {}
		// 用户比赛信息
		virtual void onGameUserContset(MSG_GR_ContestChange* contestChange) override {}
		// 比赛结束
		virtual void onGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward) override {}
		// 比赛淘汰
		virtual void onGameContestKick() override {}
		// 等待比赛结束
		virtual void onGameContestWaitOver() override {}

		//////////////////////////////////////////////////////////////////////////
		// 转发到房间
		//////////////////////////////////////////////////////////////////////////
		// 比赛奖励
		virtual void onGameContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards) override;
		// 报名数量
		virtual void onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople) override;
		// 个人参赛纪录
		virtual void onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord) override;

		//////////////////////////////////////////////////////////////////////////
		// 聊天接口
		//////////////////////////////////////////////////////////////////////////
		// 聊天消息
		virtual void onUserChatMessage(void* object, INT objectSize) override;
		virtual void onGameChatMessage(void* object, INT objectSize) override;

        
        virtual void onBackRoomMessage(MSG_GR_BACKROOM_STATUS* data) override;
        //使用房卡消息
        virtual void onUseRoomCardMessage(MSG_PROP_S_OP_ROOMCARD * data) override;
        //创建房间消息
        virtual void onCreateBackRoomMessage(MSG_BACKROOM_INFO * data) override;
        //查询房间消息
        virtual void onQueryBakcRoomMessage(MSG_QueryBACKROOM_RSP_INFO * data , BYTE code) override;

        virtual void onQueryPropKindMessage(MSG_PROP_S_KIND_ROOMCARD * data , BYTE bHandleCode ) override;
        
        virtual void onQueryPropRoomCardInfo(MSG_PROP_S_QUERY_ROOMCARD * data , BYTE bHandleCode) override;
        
        virtual void onBuyRoomCardMessage(MSG_PROP_S_BUY_ROOMCARD * data)override;
		//virtual void onGameDeskMessage(MSG_GR_DeskStation * desk)override;
	};
};

#endif	//__YZ_YZRoomEventLayer_H__
