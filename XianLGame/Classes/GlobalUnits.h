#ifndef GLOBAL_UNITS_HEAD_FILE
#define GLOBAL_UNITS_HEAD_FILE

#pragma once
#include <stack>
#include <string>
#include <cctype>
#include <queue>


#define SaveStringToXML CCUserDefault::getInstance()->setStringForKey
#define SaveIntegerToXML CCUserDefault::getInstance()->setIntegerForKey
#define SaveBooleanToXML CCUserDefault::getInstance()->setBoolForKey
#define SaveFloatToXML CCUserDefault::getInstance()->setFloatForKey
#define SaveXmlFlush CCUserDefault::getInstance()->flush

#define LoadStringFromXML CCUserDefault::getInstance()->getStringForKey
#define LoadIntegerFromXML CCUserDefault::getInstance()->getIntegerForKey
#define LoadBooleanFromXML CCUserDefault::getInstance()->getBoolForKey
#define loadFloatToXML CCUserDefault::getInstance()->getFloatForKey


/* 私聊信息 */
class Talk_Item
{
public:
	int  talker;			//聊天者 1.自己 2.好友
	char time1[128];		//时间 年月日;
	char time2[128];		//时间 时分秒;
	char nickName[30];		//昵称;
	char talkMsg[512];		//消息;
};


/* 好友私聊表 */
class Friend_Talks_Tab
{
public:
	int f_id;							//好友ID;
	std::vector<Talk_Item> f_talks;		//聊天记录
};


//全局信息类
class CGlobalUnits
{
	//信息变量
protected:
	//tagGlobalUserData					m_GloblaUserData;				//用户信息

public:
	//构造函数
	CGlobalUnits(void);
	//析构函数
	virtual ~CGlobalUnits(void);

	//功能函数
	std::string getRoleTitleStr(long long _i64Diamond);				// 获取玩家头衔

public:
	//初始化函数
	bool InitGlobalUnits();
	std::string serverip;
    bool isOpenRoom;    //是否是开房
public:
	//聊天相关
	Talk_Item				m_f_talk;
	Friend_Talks_Tab		m_f_talk_tab;
	std::vector<Talk_Item>	mf_talks_tab;
	std::vector<Friend_Talks_Tab> m_friend_talks_tab;

public:
	bool bISFirstEnterGame() {return m_bFirstEnterGame; }
	void setISFirstEnterGame(const bool bFirstEnterGame){ m_bFirstEnterGame = bFirstEnterGame; }
private:
	bool m_bFirstEnterGame;

};

//////////////////////////////////////////////////////////////////////////

//全局信息
extern	CGlobalUnits  g_GlobalUnits;					//信息组件


//////////////////////////////////////////////////////////////////////////

#endif
