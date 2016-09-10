#include "YZGameUIBase.h"
#include "YZRoom/YZRoomLogic.h"

namespace YZ
{
	YZGameUIBase::YZGameUIBase()
	{

	}

	YZGameUIBase::~YZGameUIBase()
	{

	}

	void YZGameUIBase::sendData(UINT MainID, UINT AssistantID, void* object, INT objectSize)
	{
		RoomLogic()->sendData(MainID, AssistantID, object, objectSize);
	}
}