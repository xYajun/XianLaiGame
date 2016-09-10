#include "YZRoomInfoModule.h"
#include "YZCommon/YZCommonMarco.h"

namespace YZ
{
	YZRoomInfoModule* YZRoomInfoModule::getInstance()
	{
		static YZRoomInfoModule sRoomInfoModule;
		return &sRoomInfoModule;
	}

	YZRoomInfoModule::YZRoomInfoModule(void) : _onlineAllCount(0)
	{
		_rooms = new ROOMLIST();
	}

	YZRoomInfoModule::~YZRoomInfoModule(void)
	{
		clear();
		YZ_SAFE_DELETE(_rooms);
	}

	void YZRoomInfoModule::clear()
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			YZ_SAFE_DELETE(*iter);
		}
		_rooms->clear();
	}

	ComRoomInfo* YZRoomInfoModule::getByRoomID(INT roomID)
	{
		ComRoomInfo* room = nullptr;
		do 
		{
			for (auto iter =_rooms->begin(); iter != _rooms->end(); ++iter)
			{
				if ((*iter)->uRoomID == roomID)
				{
					room = *iter; 
					break;
				}
			}
		} while (0);
		return room;
	}

	ComRoomInfo* YZRoomInfoModule::getRoom(INT Postion)
	{
		if (Postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return _rooms->at(Postion);
		}

		return nullptr;
	}

	ComRoomInfo* YZRoomInfoModule::operator [](INT postion)
	{
		if (postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return (*_rooms)[postion];
		}

		return nullptr;
	}

	const ComRoomInfo* YZRoomInfoModule::operator [](INT postion) const
	{
		if (postion >= _rooms->size())
		{
			return nullptr;
		}

		if (!_rooms->empty())
		{
			return (*_rooms)[postion];
		}

		return nullptr;
	}

	void YZRoomInfoModule::addRoom(ComRoomInfo* room)
	{
		assert(room != nullptr);
		ComRoomInfo* rm = new ComRoomInfo;
		*rm = *room;
		_rooms->push_back(rm);
	}

	void YZRoomInfoModule::removeRoom(INT roomID)
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if ((*iter)->uRoomID == roomID) 
			{
				YZ_SAFE_DELETE(*iter);
				_rooms->erase(iter);
				break;
			}
		}
	}

	ComRoomInfo* YZRoomInfoModule::findRoom(INT roomID)
	{
		ComRoomInfo* room = nullptr;
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if ((*iter)->uRoomID == roomID) 
			{
				room = *iter;
				break;
			}
		}
		return room;
	}

	void YZRoomInfoModule::updateRoom(ComRoomInfo* room)
	{
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter)
		{
			if (*iter == room) 
			{
				*(*iter) = *room;
				break;
			}
		}
	}

	void YZRoomInfoModule::transform(const TransformRoomFunc& fun)
	{
		INT index = 0;
		for (auto iter = _rooms->begin(); iter != _rooms->end(); ++iter, ++index)
		{
			fun(*iter, index);
		}
	}
}
