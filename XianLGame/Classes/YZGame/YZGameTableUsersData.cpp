#include "YZGameTableUsersData.h"

namespace YZ
{


YZGameTableUsersData::YZGameTableUsersData(BYTE deskNo) : _deskNo(deskNo)
{
	UserInfoModule()->addObserver(this);
	update();
}

void YZGameTableUsersData::update()
{
	//UserInfoModule()->findDeskUsers(_deskNo, _users);
	_users = UserInfoModule()->findDeskUsers(_deskNo);
}

void YZGameTableUsersData::update(BYTE deskNo)
{
	_deskNo = deskNo;
	//UserInfoModule()->findDeskUsers(_deskNo, _users);
	_users = UserInfoModule()->findDeskUsers(_deskNo);
}

YZGameTableUsersData::~YZGameTableUsersData(void)
{
	UserInfoModule()->removeObserver(this);
}

UserInfoStruct* YZGameTableUsersData::getUserByDeskStation(BYTE bDeskStation)
{
	UserInfoStruct* user = nullptr;
	std::vector<UserInfoStruct*>::iterator iter = std::find_if(_users.begin(), _users.end(), 
		[&bDeskStation](UserInfoStruct* user)
		{
			return user->bDeskStation == bDeskStation;
		});
	
	if (iter != _users.end())
	{
		user = (*iter);
	}

	return user;
}

// 获取玩家信息
UserInfoStruct* YZGameTableUsersData::getUserByUserID(INT userID)
{
	UserInfoStruct* user = nullptr;
	std::vector<UserInfoStruct*>::iterator iter = std::find_if(_users.begin(), _users.end(), 
		[&userID](UserInfoStruct* user) 
		{
			return user->dwUserID == userID;
		});
	
	if (iter != _users.end())
	{
		user = (*iter);
	}

	return user;
}

UserInfoStruct* YZGameTableUsersData::getUserByIndex(BYTE index)
{
	if (index >= _users.size())
	{
		return nullptr;
	}
	return _users[index];
}

void YZGameTableUsersData::findLooker(std::vector<UserInfoStruct*>& users)
{
	{
		std::vector<UserInfoStruct*>().swap(users);
	}

	std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
		{
			return user->bUserState == USER_WATCH_GAME;
		});
}

// 获取坐桌玩家
void YZGameTableUsersData::findSitUsers(std::vector<UserInfoStruct*>& users)
{
	{
		std::vector<UserInfoStruct*>().swap(users);
	}

	std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
		{
			return user->bUserState == USER_SITTING;
		});
}

// 获取正在游戏玩家
void YZGameTableUsersData::findGameUsers(std::vector<UserInfoStruct*>& users)
{
	{
		std::vector<UserInfoStruct*>().swap(users);
	}

	std::copy_if(_users.begin(), _users.end(), std::back_inserter(users), [](UserInfoStruct* user) -> bool
		{
			return user->bUserState == USER_PLAY_GAME;
		});
}

void YZGameTableUsersData::clear()
{
	_users.clear();
}

// 遍历用户数据
void YZGameTableUsersData::transform(const TransformUserFunc& func)
{
	INT index = 0;
	for (auto iter = _users.begin(); iter != _users.end(); ++iter, ++index)
	{
		func(*iter, index);
	}
}

void YZGameTableUsersData::onChanged(UserInfoStruct* user, COMMAND command)
{
	switch (command)
	{
	case YZ::IUserInfoChangedEvent::UNKNOW:
		break;
	case YZ::IUserInfoChangedEvent::ADD:
		_users.push_back(user);
		break;
	case YZ::IUserInfoChangedEvent::REMOVE:
	{
		auto ptr = std::find(_users.begin(), _users.end(), user);
		if (ptr != _users.end())
		{
			_users.erase(std::remove(_users.begin(), _users.end(), user));
		}
	}
		break;
	case YZ::IUserInfoChangedEvent::UPDATE:
		{
			auto iter = _users.begin();
			for (; iter != _users.end(); ++iter)
			{
				if (*iter == user)
				{
					break;
				}
			}

			if(user->bDeskNO == _deskNo)
			{
				if(iter == _users.end())
				{
					_users.push_back(user);
				}
				else
				{
					**iter = *user;
				}
			}
			else
			{
				if(iter != _users.end())
				{
					_users.erase(iter);
				}
			}
		}
		break;
	case YZ::IUserInfoChangedEvent::CLEAR:
		_users.clear();
		break;
	default:
		break;
	}
}

}
