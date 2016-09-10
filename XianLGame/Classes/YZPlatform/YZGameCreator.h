#ifndef YZGame_Creator_h__
#define YZGame_Creator_h__

#include "cocos2d.h"
#include "YZBaseType.h"
#include "YZUIExport.h"
#include "YZNetExport.h"

#include <unordered_map>

#define Apple_PlatForm (Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_IPAD || \
	Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_IPHONE || \
	Application::getInstance()->getTargetPlatform() == ApplicationProtocol::Platform::OS_MAC)


typedef std::function<YZGameUIBase* (UINT uDeskId, bool autoCreate)> GAME_CREATE_SELECTOR;

typedef std::function<YZLayer* (Node* parent)> ROOM_CREATE_SELECTOR;

class YZGameCreator
{
public:
	enum GAMETYPE
	{
		UNKNOWN = 0,
		NORMAL,
		BR
	};
	static const BYTE INVALID_PRIORITY = 0xFF;

private:
	struct ItemCreator
	{
		BYTE priority;
		GAMETYPE type;
		UINT uNameId;
		UINT uKindId;
		GAME_CREATE_SELECTOR	gameSelector;
		ROOM_CREATE_SELECTOR	roomSelector;

		ItemCreator() : uNameId(0), uKindId(0), type(UNKNOWN), gameSelector(nullptr), roomSelector(nullptr)
		{

		}

		bool valid()
		{
			return (0 != uNameId) && (0 != uKindId) && (UNKNOWN != type) && (nullptr != gameSelector);
		}
	};

public:
	static YZGameCreator* getInstance();

public:
	// 注册游戏
	void addGame(UINT uNameId, UINT uKindId, GAMETYPE type, 
									GAME_CREATE_SELECTOR createGameSelector, ROOM_CREATE_SELECTOR createRoomSelector = nullptr);
	// 获取有效的游戏
	bool getValidGames(std::vector<ComNameInfo*>* validGames);
	// 获取游戏优先级
	BYTE getGamePriority(UINT uNameId);
	// 设置当前选择的游戏
	void setCurrentGame(UINT uNameId);
	// 当前游戏ID
	UINT getCurrentGameNameID() const;
	// 当前游戏类型
	UINT getCurrentGameKindID() const;
	// 获取游戏类型
	GAMETYPE getCurrentGameType() const;

public:
	// 启动游戏客户端
	bool startGameClient(UINT uNameId, BYTE bDeskIndex, bool bAutoCreate);
	// 启动游戏房间
	bool startGameRoom(UINT uNameId, Node* parent);

protected:
	// 校验游戏
	BYTE validGame(UINT uNameId);

private:
	YZGameCreator();
	~YZGameCreator();

private:
	std::unordered_map<UINT, ItemCreator> _creators;
	ItemCreator* _currentCreator;

	LLONG	_basePriority;
};

#define GameCreator()	YZGameCreator::getInstance()

#endif // YZGame_Creator_h__
