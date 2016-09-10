#ifndef __YZ_YZSocketMessage_H__
#define __YZ_YZSocketMessage_H__

#include "YZBaseType.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include <functional>
#include <string>

namespace YZ {

	#define SELECTER_KEY_FORMAT	"%u_%u"

	class YZSocketMessage;

	typedef std::function<bool(YZSocketMessage* socketMessage)> SEL_SocketMessage;

	#define YZ_SOCKET_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, ##__VA_ARGS__)


	class YZSocketMessage 
	{
		public:
			static YZSocketMessage* getMessage();
			static void releaseMessage(YZSocketMessage* message);

		public:
			void setMessage(const NetMessageHead* head, CHAR* obj, INT Size);

		private:
			YZSocketMessage();
			~YZSocketMessage();

 			void *operator new(std::size_t ObjectSize);
 			void operator delete(void *ptrObject);

		public:
			NetMessageHead messageHead;
			UINT objectSize;			// message size
			CHAR object[4096];
			emSocketStatus socketStatus;
			std::string strKey;
			
	};

	class YZSocketSelectorItem
	{
	public:
		YZSocketSelectorItem(SEL_SocketMessage selector) : _selector(selector)
		{
		}

	public:
		bool doCallSelector(YZSocketMessage* socketMessage)
		{ 
			return (_selector)(socketMessage);
		}

	private:
		SEL_SocketMessage	_selector;
	};
};

#endif	//__YZ_YZSocketMessage_H__
