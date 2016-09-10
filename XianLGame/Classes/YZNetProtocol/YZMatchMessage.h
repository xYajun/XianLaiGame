#ifndef _YZ_MatchMessage_H__
#define _YZ_MatchMessage_H__

#include "YZBaseType.h"
#include "YZCommon/YZCommonMarco.h"

//////////////////////////////////////////////////////////////////////////

// 报名结果
enum E_CONTEST_APPLY_RESULT
{
	rs_contest_apply_error,
	rs_contest_apply_ok,			//报名成功
	rs_contest_apply_nomoney,		//金币不够报名费用
	rs_contest_apply_already_apply,	//已经报名，无需再次报名
	rs_contest_apply_contest_begin,	//比赛已经开始无法报名
};

// 退赛结果
enum E_CONTEST_CANCEL_RESULT
{
	rs_contest_cancel_error,
	rs_contest_cancel_ok,			//退赛成功
	rs_contest_cancel_noapply,		//没有报名，无法执行退赛
	rs_contest_cancel_contest_begin,//比赛已经开始无法退赛
};

typedef struct NET_ROOM_CONTEST_APPLY
{
	UINT	dwUserID;			//用户ID
	INT		dwContestID;		//赛事ID(房间)
}NET_ROOM_CONTEST_APPLY;

typedef struct NET_ROOM_CONTEST_CANCEL
{
	UINT	dwUserID;		//用户ID
	INT		dwRoomId;		//房间ID
	INT		nEntryTax;		//退还费用
}NET_ROOM_CONTEST_CANCEL;

typedef struct NET_ROOM_CONTEST_CANCEL_RESULT
{
	UINT	dwUserID;
	int		nResult;
}NET_ROOM_CONTEST_CANCEL_RESULT;

//排队玩家坐桌成功结构体
typedef struct QUEUE_USER_SIT_RESULT
{
	UINT	dwUserID;		// 玩家ID
	BYTE	bDeskStation;	// 玩家座位
} GNUC_PACKED QUEUE_USER_SIT_RESULT;

// 房间中显示报名人数
typedef struct NET_ROOM_CONTEST_PEOPLE_RESULT
{
	BYTE	bSignedCount;	// 已报名数量
	BYTE	bNeedCount;		// 还需多少人报名才开始
	BYTE	bContestBegin;	// 比赛是否开始
} GNUC_PACKED NET_ROOM_CONTEST_PEOPLE_RESULT;

// 玩家报名结果
typedef struct NET_ROOM_CONTEST_APPLY_RESULT
{
	NET_ROOM_CONTEST_PEOPLE_RESULT	contestPeople;	//比赛报名人数情况
	UINT							dwUserID;		//玩家ID
	INT								nResult;
} GNUC_PACKED NET_ROOM_CONTEST_APPLY_RESULT;

// 发送每个人的排名
typedef struct NET_ROOM_CONTEST_CHANGE_RESULT
{
	LLONG	llContestScore;	// 比赛分数
	UINT	dwUserID;		// 玩家ID
	BYTE	bRemainPeople;	// 剩余人数
	BYTE	bRankNum;		// 当前排名
	BYTE	bPlayCount;		// 本场比赛第几局
} GNUC_PACKED NET_ROOM_CONTEST_CHANGE_RESULT;

// 参赛纪录，用于房间中显示个人记录
typedef struct NET_ROOM_CONTEST_RECORD_RESULT
{
	INT		iChanpionTimes;	// 夺冠次数
	INT		iContestTimes;	// 参赛次数
	BYTE	bBestRank;		// 最佳排名
} GNUC_PACKED NET_ROOM_CONTEST_RECORD_RESULT;


// 比赛奖励
typedef struct NET_ROOM_CONTEST_AWARD_RESULT
{
	UINT						uRankNum;	//排名
	UINT						uLottery;	//奖券数量
	UINT						uMoney;		//金币数量
} GNUC_PACKED NET_ROOM_CONTEST_AWARD_RESULT;

#endif	//_YZ_MatchMessage_H__
