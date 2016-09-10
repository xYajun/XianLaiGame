#include "YZUserInfoModule.h"
#include "YZCommon/YZCommonMarco.h"
#include <algorithm>
#include <iterator>

namespace YZ
{
	YZUserInfoModule* YZUserInfoModule::getInstance()
	{
		static YZUserInfoModule sUserModule;
		return &sUserModule;
	}

	YZUserInfoModule::YZUserInfoModule(void)
	{
		_onlineUsers = new USERLIST();
		_dataChangedQueue = new std::vector<IUserInfoChangedEvent*>();
	}

	YZUserInfoModule::~YZUserInfoModule(void)
	{
		clear();
		YZ_SAFE_DELETE(_onlineUsers);
		YZ_SAFE_DELETE(_dataChangedQueue);
	}

	void YZUserInfoModule::addObserver(IUserInfoChangedEvent* delegate)
	{
		_dataChangedQueue->push_back(delegate);
	}

	void YZUserInfoModule::removeObserver(IUserInfoChangedEvent* delegate)
	{
		_dataChangedQueue->erase(std::remove(_dataChangedQueue->begin(), _dataChangedQueue->end(), delegate));
	}

	void YZUserInfoModule::clear()
	{
		for (auto iter = _onlineUsers->begin(); iter != _onlineUsers->end(); ++iter)
		{
			YZ_SAFE_DELETE(*iter);
		}
		_onlineUsers->clear();
		onNotify(nullptr, IUserInfoChangedEvent::CLEAR);
	}

	UserInfoStruct* YZUserInfoModule::addUser(UserInfoStruct* user)
	{
		assert(nullptr != user);

		UserInfoStruct * newUser = new UserInfoStruct;
		*newUser = *user;
		_onlineUsers->push_back(newUser);
		onNotify(newUser, IUserInfoChangedEvent::ADD);
		return newUser;
	}

	void YZUserInfoModule::removeUser(INT userID)
	{
		for (auto iter = _onlineUsers->begin(); iter != _onlineUsers->end(); ++iter)
		{
			if (userID == (*iter)->dwUserID)
			{
				onNotify(*iter, IUserInfoChangedEvent::REMOVE);
				YZ_SAFE_DELETE(*iter);
				_onlineUsers->erase(iter);
				break;
			}
		}
	}

	UserInfoStruct* YZUserInfoModule::findUser(INT userID)
	{
		UserInfoStruct* user = nullptr;
		for (auto iter = _onlineUsers->begin(); iter != _onlineUsers->end(); ++iter)
		{
			if (userID == (*iter)->dwUserID)
			{
				user = *iter;
				break;
			}
		}
		return user;
	}

	UserInfoStruct* YZUserInfoModule::findUser(BYTE deskNo, BYTE station)
	{
		std::vector<UserInfoStruct*> tmpUsers;
		findDeskUsers(deskNo, tmpUsers);
		UserInfoStruct* user = nullptr;
		for (auto iter = tmpUsers.begin(); iter != tmpUsers.end(); ++iter)
		{
			if (station == (*iter)->bDeskStation)
			{
				user = *iter;
				break;
			}
		}
		return user;
	}

	void YZUserInfoModule::updateUser(UserInfoStruct* user)
	{
		UserInfoStruct* oldUser = findUser(user->dwUserID);
		if (nullptr != oldUser)
		{
			*oldUser = *user;
			onNotify(oldUser, IUserInfoChangedEvent::UPDATE);
		}
		else
		{
			addUser(user);
		}
	}

	void YZUserInfoModule::findDeskUsers(BYTE bDeskNO, std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}

		std::copy_if(_onlineUsers->begin(), _onlineUsers->end(), std::inserter(users, users.begin()), 
			[&bDeskNO](UserInfoStruct* user) -> bool
		{
			return (bDeskNO == user->bDeskNO) && (user->bUserState != USER_WATCH_GAME) && (user->bUserState != USER_LOOK_STATE);
		});
	}

	std::vector<UserInfoStruct*> YZUserInfoModule::findDeskUsers(BYTE bDeskNO)
	{
		std::vector<UserInfoStruct*> users;
		std::copy_if(_onlineUsers->begin(), _onlineUsers->end(), std::inserter(users, users.begin()), 
			[&bDeskNO](UserInfoStruct* user) -> bool
		{
			return (bDeskNO == user->bDeskNO) && (user->bUserState != USER_WATCH_GAME) && (user->bUserState != USER_LOOK_STATE);
		});
		return users;
	}

	void YZUserInfoModule::findLooker(BYTE deskNO, std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}
		
		std::copy_if(_onlineUsers->begin(), _onlineUsers->end(), std::back_inserter(users), 
			[deskNO](UserInfoStruct* user) -> bool
			{
				return deskNO == user->bDeskNO && user->bUserState == USER_WATCH_GAME;
			});
	}

	// 获取游戏玩家
	void YZUserInfoModule::findGameUsers(BYTE deskNO, std::vector<UserInfoStruct*>& users)
	{
		{
			std::vector<UserInfoStruct*>().swap(users);
		}

		std::copy_if(_onlineUsers->begin(), _onlineUsers->end(), std::back_inserter(users), 
			[deskNO](UserInfoStruct* user) -> bool
			{
				return (
						(deskNO == user->bDeskNO) 
						&& ((user->bUserState == USER_SITTING) || (user->bUserState == USER_PLAY_GAME) || (user->bUserState == USER_ARGEE)));
			});
	}

	void YZUserInfoModule::onNotify(UserInfoStruct* user, IUserInfoChangedEvent::COMMAND command)
	{
		for (auto iter = _dataChangedQueue->begin(); iter != _dataChangedQueue->end(); ++iter)
		{
			(*iter)->onChanged(user, command);
		}
	}

	void YZUserInfoModule::transform(const TransformUserFunc& func)
	{
		INT index = 0;
		for (auto iter = _onlineUsers->begin(); iter != _onlineUsers->end(); ++iter, ++index)
		{
			func(*iter, index);
		}
	}

	void YZUserInfoModule::transform(BYTE deskNO, const TransformUserFunc& func)
	{
		INT index = 0;
		for (auto iter = _onlineUsers->begin(); iter != _onlineUsers->end(); ++iter, ++index)
		{
			if (deskNO == (*iter)->bDeskNO)
			{
				func(*iter, index);
			}
		}
	}

}
