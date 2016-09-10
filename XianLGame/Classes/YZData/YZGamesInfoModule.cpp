#include "YZGamesInfoModule.h"
#include "YZCommon/YZCommonMarco.h"

namespace YZ
{
	YZGamesInfoModule* YZGamesInfoModule::getInstance()
	{
		static YZGamesInfoModule gamesInfoModule;
		return &gamesInfoModule;
	}

	YZGamesInfoModule::YZGamesInfoModule(void)
		: _gameKinds(new GAME_KINDS())
		, _gameNames(new GAME_NAMES())
		, _selectedGame(nullptr)
	{
	}

	YZGamesInfoModule::~YZGamesInfoModule(void)
	{
		clear();
		YZ_SAFE_DELETE(_gameKinds);
		YZ_SAFE_DELETE(_gameNames);
	}

	void YZGamesInfoModule::clear()
	{
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			YZ_SAFE_DELETE(*iter);
		}
		_gameNames->clear();

		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter)
		{
			YZ_SAFE_DELETE(*iter);
		}
		_gameKinds->clear();
	}

	int YZGamesInfoModule::getGameNameCount() const
	{
		return _gameNames->size();
	}

	ComNameInfo* YZGamesInfoModule::getGameNameByKindID(INT kindID)
	{
		ComNameInfo* game = nullptr;
		for (auto iter =_gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			if ((*iter)->uKindID == kindID) 
			{
				game = *iter; 
				break;
			}
		}
		return game;
	}

	ComNameInfo* YZGamesInfoModule::getGameNameByPos(INT pos)
	{
		if (pos >= _gameNames->size())
		{
			return nullptr;
		}

		if (!_gameNames->empty())
		{
			return _gameNames->at(pos);
		}

		return nullptr;
	}

	void YZGamesInfoModule::addGameKind(ComKindInfo* kindInfo)
	{
		assert(kindInfo != nullptr);
		if (nullptr != kindInfo)
		{
			ComKindInfo* game = findGameKind(kindInfo->uKindID);
			if (nullptr != game)
			{
				*game = *kindInfo;
			}
			else
			{
				ComKindInfo* game = new ComKindInfo();
				*game = *kindInfo;
				_gameKinds->push_back(game);
			}
		}
	}

	void YZGamesInfoModule::addGameName(ComNameInfo* nameInfo)
	{
		assert(nameInfo != nullptr);
		if (nullptr != nameInfo)
		{
			ComNameInfo* game = findGameName(nameInfo->uNameID);
			if (nullptr != game)
			{
				*game = *nameInfo;
			}
			else
			{
				ComNameInfo* game = new ComNameInfo();
				*game = *nameInfo;
				_gameNames->push_back(game);
			}
		}
	}

	ComKindInfo* YZGamesInfoModule::findGameKind(int kindID)
	{
		ComKindInfo* game = nullptr;
		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter)
		{
			if ((*iter)->uKindID == kindID) 
			{
				game = *iter;
				break;
			}
		}
		return game;
	}

	/*
	* 查找游戏数据
	*/
	ComNameInfo* YZGamesInfoModule::findGameName(INT gameID)
	{
		ComNameInfo* game = nullptr;
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter)
		{
			if ((*iter)->uNameID == gameID) 
			{
				game = *iter;
				break;
			}
		}
		return game;
	}

	void YZGamesInfoModule::transform(const TransformNameInfo& fun)
	{
		INT index = 0;
		for (auto iter = _gameNames->begin(); iter != _gameNames->end(); ++iter, ++index)
		{
			fun(*iter, index);
		}
	}

// 	void YZGamesInfoModule::transform(const TransformKindInfo& fun)
// 	{
// 		INT index = 0;
// 		for (auto iter = _gameKinds->begin(); iter != _gameKinds->end(); ++iter, ++index)
// 		{
// 			fun(*iter, index);
// 		}
// 	}

}