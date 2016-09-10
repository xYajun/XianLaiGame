#ifndef __YZ_YZGameLogicBase_H__
#define __YZ_YZGameLogicBase_H__

#include "YZGameMessageDelegate.h"

namespace YZ
{
	class YZGameTableUsersData;
	class YZGameLogicBase;

	class YZGameLogicBase: public IGameMessageDelegate
	{

	protected:
		// user list who sit in the table
		YZGameTableUsersData*		_deskUserList;
		// desk no.
		BYTE						_deskNo;
		// self seat no.
		BYTE						_mySeatNo;		
		BYTE						_watchOther;	// 允许旁观
		BYTE						_gameStatus;	// 游戏状态
		// + is clockwise, - is opposite.
		int							_seatOffset;	// 座位偏移量
		// before auto action, player's action time.
		BYTE						_waitTime;		// 等待时间
		// max player count in the table.
		INT							_maxPlayers;
		// if the table is auto created
		bool						_autoCreate;

		std::vector<bool>			_existPlayer;

		std::vector<bool>			_playing;

	public:
		YZGameLogicBase(BYTE deskNo, INT maxPlayers, bool autoCreate);
		virtual ~YZGameLogicBase();

	public:
		/*
		 * @func: interface for others. 
		 * @info: they will support table ui to get information, or send some message to server.
		 */
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);
		void sendGameInfo();
		virtual void sendUserUp();
		virtual void sendUserSit(BYTE viewSeatNo);
		virtual void sendForceQuit();
		virtual void sendAgreeGame();
	
	// 功能接口
	public:
		// 获取用户ID
		INT getUserId(BYTE vSeatNo);
		INT getUserLogoId(BYTE vSeatNo);
		// 获取自己座位号
		BYTE getMySeatNo() const;
		// 获取玩家性别
		bool getUserIsBoy(BYTE vSeatNo);
		// 获取玩家信息
		UserInfoStruct* getUserByUserID(INT userID);
		// 获取玩家信息
		UserInfoStruct* getUserByDeskStation(BYTE vSeatNo);

	protected:
		virtual void loadDeskUsers();
		virtual void clearDesk();

	protected:
		/*
		 * framework message deal function.
		 */
		virtual void dealUserAgreeResp(MSG_GR_R_UserAgree* agree);
		virtual void dealGameStartResp(BYTE bDeskNO);
		virtual void dealGameEndResp(BYTE bDeskNO);
		virtual void dealUserSitResp(MSG_GR_R_UserSit * userSit, UserInfoStruct* user);
		virtual void dealQueueUserSitMessage(BYTE deskNo, const std::vector<QUEUE_USER_SIT_RESULT*>& users);
		virtual void dealUserUpResp(MSG_GR_R_UserUp * userSit, UserInfoStruct* user);
		virtual void dealGameInfoResp(MSG_GM_S_GameInfo* pGameInfo);
		virtual void dealGameStationResp(void* object, INT objectSize) = 0;
		virtual void dealGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) = 0;
		virtual void dealUserCutMessageResp(INT userId, BYTE seatNo);
		virtual void dealGamePointResp(void * object, INT objectSize);
                
		//////////////////////////////////////////////////////////////////////////
		// 比赛接口
		//////////////////////////////////////////////////////////////////////////
		// 转发到游戏
		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 比赛初始化
		virtual void dealGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange);
		// 用户比赛信息
		virtual void dealGameUserContset(MSG_GR_ContestChange* contestChange);
		// 比赛结束
		virtual void dealGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward);
		// 比赛淘汰
		virtual void dealGameContestKick() ;
		// 等待比赛结束
		virtual void dealGameContestWaitOver() ;
		//////////////////////////////////////////////////////////////////////////
		// 转发到房间
		//////////////////////////////////////////////////////////////////////////
		// 比赛奖励
		virtual void dealGameContestAwards(NET_ROOM_CONTEST_AWARD_RESULT* contestAward);
		// 报名数量
		virtual void dealGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople);
		// 个人参赛纪录
		virtual void dealGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* ContestRecord);

		//////////////////////////////////////////////////////////////////////////
		// 聊天接口
		//////////////////////////////////////////////////////////////////////////
		// 聊天消息
		virtual void dealUserChatMessage(void* object, INT objectSize);

	protected:
		/* 
		 * init member only once.
		 */
		virtual void initParams();
		/*
		 * refresh member before every game start.
		 */
		virtual void refreshParams();
		/*
		 * get next and last user seat no.
		 * @param:  seatNo find seat no.
		 * @return: return seat no if success, or return INVALID_DESKSTATION
		 */
		virtual BYTE getNextUserSeatNo(BYTE lSeatNo);
		virtual BYTE getLastUserSeatNo(BYTE lSeatNo);
	
		/*
		 * between view seat no and logic seat no translate.
		 */
		virtual BYTE viewToLogicSeatNo(BYTE vSeatNO);
		virtual BYTE logicToViewSeatNo(BYTE lSeatNO);
		
	private:
		/*
		 * they will be called when framework message come.
		 */
		virtual void onUserAgreeMessage(MSG_GR_R_UserAgree* agree) override;
		virtual void onGameStartMessage(BYTE bDeskNO) override;
		virtual void onGameEndMessage(BYTE bDeskNO) override;
		virtual void onGameInfoMessage(MSG_GM_S_GameInfo* pGameInfo) override;
		virtual void onGameStationMessage(void* object, INT objectSize) override;
		virtual bool onGameMessage(NetMessageHead* messageHead, void* object, INT objectSize) override;
		virtual void onGamePointMessage(void * object, INT objectSize) override;
		virtual void onGameChatMessage(void * object, INT objectSize) override;
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
		// 转发到游戏
		//////////////////////////////////////////////////////////////////////////
		// 比赛初始化
		virtual void onGameContestInit(NET_ROOM_CONTEST_CHANGE_RESULT* contestChange) override;
		// 用户比赛信息
		virtual void onGameUserContset(MSG_GR_ContestChange* contestChange) override;
		// 比赛结束
		virtual void onGameContestOver(NET_ROOM_CONTEST_AWARD_RESULT* contestAward) override;
		// 比赛淘汰
		virtual void onGameContestKick() override;
		// 等待比赛结束
		virtual void onGameContestWaitOver() override;
		//////////////////////////////////////////////////////////////////////////
		// 转发到房间
		//////////////////////////////////////////////////////////////////////////
		// 比赛奖励
		virtual void onGameContestAwards(std::vector<NET_ROOM_CONTEST_AWARD_RESULT*>* awards) override {}
		// 报名数量
		virtual void onGameContestPeople(NET_ROOM_CONTEST_PEOPLE_RESULT* contestPeople) override {}
		// 个人参赛纪录
		virtual void onGameContestRecord(NET_ROOM_CONTEST_RECORD_RESULT* contestRecord) override {}

		//////////////////////////////////////////////////////////////////////////
		// 聊天接口
		//////////////////////////////////////////////////////////////////////////
		// 聊天消息
		virtual void onUserChatMessage(void* object, INT objectSize) override;

	};
}


#endif