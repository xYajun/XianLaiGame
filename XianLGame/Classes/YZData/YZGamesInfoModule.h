#ifndef YZGamesInfoModule_h__
#define YZGamesInfoModule_h__


#include "YZNetProtocol/YZProtocolExport.h"
#include <vector>
#include <functional>

namespace YZ
{
	typedef std::function<void (ComNameInfo*, INT index)> TransformNameInfo;
	typedef std::function<void (ComKindInfo*, INT index)> TransformKindInfo;

	class YZGamesInfoModule
	{
		typedef std::vector<ComKindInfo*> GAME_KINDS;
		typedef std::vector<ComNameInfo*> GAME_NAMES;

	public:
		static YZGamesInfoModule* getInstance();

	public:
		ComNameInfo* selectedGame() const
		{
			return _selectedGame;
		}

		void selectedGame(ComNameInfo* game)
		{
			_selectedGame = game;
		}

		INT getGameNameCount() const;

		/*
		* 获取游戏信息
		*/
		ComNameInfo* getGameNameByKindID(INT kindID);

		/*
		* 获取游戏信息
		*/
		ComNameInfo* getGameNameByPos(INT pos);

		/*
		* 添加游戏类型
		*/
		void addGameKind(ComKindInfo* kindInfo);

		/*
		* 添加游戏名称
		*/
		void addGameName(ComNameInfo* nameInfo);

		/*
		* 查找游戏数据
		*/
		ComNameInfo* findGameName(INT gameID);

		/*
		* 查找游戏数据
		*/
		ComKindInfo* findGameKind(INT kindID);

		/*
		* 清空游戏数据
		*/
		void clear();
		
		/*
		* 遍历游戏数据
		*/
		void transform(const TransformNameInfo & fun);

		/*
		* 遍历游戏数据
		*/
		//void transform(const TransformKindInfo & fun);

	private:
		// 游戏类型列表
		GAME_KINDS*			_gameKinds;

		// 游戏名称列表
		GAME_NAMES*			_gameNames;

		// 当前选中的游戏
		ComNameInfo*		_selectedGame;

	public:
		YZGamesInfoModule();
		~YZGamesInfoModule();
	};

	extern YZGamesInfoModule gamesInfoModule;

	#define GamesInfoModule()	YZGamesInfoModule::getInstance()
}


#endif // YZGamesInfoModule_h__
