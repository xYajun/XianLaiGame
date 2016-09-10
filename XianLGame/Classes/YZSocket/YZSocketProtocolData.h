#ifndef __YZ_YZSocketProtocolData_H__
#define __YZ_YZSocketProtocolData_H__

#include "YZBaseType.h"
#include "YZCommon/YZCommonMarco.h"
#include "YZCommon/YZLog.h"
#include "YZNetProtocol/YZProtocolExport.h"

namespace YZ {

	class YZSocketProtocolData
	{
	public:
		YZSocketProtocolData() : _objectSize(0)
		{
			::memset(_object, 0x0, sizeof(_object));
			::memset(&_messageHead, 0x0, sizeof(NetMessageHead));
		}

		bool createPackage(UINT MainID, UINT AssistantID, UINT CheckCode, void* object = nullptr, INT objectSize = 0)
		{
			assert(INVALID_VALUE != CheckCode);
			if (INVALID_VALUE == CheckCode) return false;
			
			UINT messageHeadSize = sizeof(NetMessageHead);

			_objectSize = messageHeadSize + objectSize; 

			YZ_ASSERT(_objectSize < CACHE_BUFFER_SIZE, "data size > cache buffer.");

			bool ret = false;
			do
			{
				if (_objectSize <= CACHE_BUFFER_SIZE)
				{
					_messageHead.uMessageSize = _objectSize;
					_messageHead.bMainID = MainID;
					_messageHead.bAssistantID = AssistantID;
					_messageHead.bReserve = CheckCode;
					::memcpy(_object, &_messageHead, messageHeadSize);
					if (nullptr != object)
					{
						::memcpy(_object + messageHeadSize, (void*)object, _objectSize);
					}
					//Description();
					ret = true;
				}
			} while (0);

			return ret;
		}

		CHAR* getPackage()
		{
			return _object;
		}

		INT getPackageSize() const
		{
			return _objectSize;
		}

		void Description() 
		{
# if defined(_DEBUG) || defined(DEBUG)
			CHAR buf[512] = {0};
			sprintf(buf, "NetMessageHead = {uMessageSize = %d bMainID = %d bAssistantID = %d bHandleCode = %d bReserve = %d object = %s objectSize = %d}", 
				_messageHead.uMessageSize, 
				_messageHead.bMainID,
				_messageHead.bAssistantID,
				_messageHead.bHandleCode,
				_messageHead.bReserve, 
				_object,
				_objectSize);
			YZLOG_DEBUG("%s", buf);
#endif	//_DEBUG || DEBUG
		}

	public:
		const static UINT CACHE_BUFFER_SIZE = 1024*10;
		static UINT PlatformCheckCode;
		static UINT GameCheckCode;

	private:
		NetMessageHead	_messageHead;
		CHAR			_object[CACHE_BUFFER_SIZE];
		INT				_objectSize;
	};
}

#endif	//__YZ_YZSocketProtocolData_H__

