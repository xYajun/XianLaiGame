#include "YZSocketMessage.h"
#include "YZCommon/YZLog.h"
#include "YZCommon/YZMemoryPool.h"

namespace YZ {
	//////////////////////////////////////////////////////////////////////////
	YZMemPool gMemPool(sizeof(YZSocketMessage), 32, 1024);

	YZSocketMessage* YZSocketMessage::getMessage()
	{
		return new YZSocketMessage();
	}

	void YZSocketMessage::releaseMessage(YZSocketMessage* message)
	{
		YZ_SAFE_DELETE(message);
	}

	YZSocketMessage::YZSocketMessage() : objectSize(0), socketStatus(SocketStatus_UNKNOW)
	{
		::memset(object, 0x0, sizeof(object));
		::memset(&messageHead, 0x0, sizeof(object));
	}

	YZSocketMessage::~YZSocketMessage()
	{

	}

	void *YZSocketMessage::operator new(std::size_t ObjectSize)
	{
		return gMemPool.get();
	}

	void YZSocketMessage::operator delete(void *ptrObject)
	{
		gMemPool.release(ptrObject);
	}

	void YZSocketMessage::setMessage(const NetMessageHead* head, CHAR* obj, INT Size)
	{
		messageHead = *head;
		objectSize = Size;
		memcpy(object, obj, objectSize);
		CHAR buf[16] = {0};
		sprintf(buf, "%u_%u", messageHead.bMainID, messageHead.bAssistantID);
		strKey.assign(buf);
	}
}
