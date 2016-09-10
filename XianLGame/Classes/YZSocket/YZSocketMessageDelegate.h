#ifndef __YZ_YZGameMessageDelegate_H__
#define __YZ_YZGameMessageDelegate_H__

#include "YZBaseType.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include "YZSocketMessage.h"

namespace YZ {

	class ISocketMessageDelegate
	{
	public:
		virtual ~ISocketMessageDelegate(void) {}
		virtual void onSocketMessage(YZSocketMessage* socketMessage) = 0;
		virtual void onConnected(bool connect, emSocketStatus status) = 0;
		virtual void onDisConnect() = 0;
	};

};

#endif	//__YZ_YZGameMessageDelegate_H__

