#ifndef __YZ_YZSocketMessageDelegate_H__
#define __YZ_YZSocketMessageDelegate_H__

#include "YZBaseType.h"
#include "YZNetProtocol/YZProtocolExport.h"
#include "YZSocketMessage.h"

namespace YZ {

class ISocketEventDelegate
{
public:
	virtual ~ISocketEventDelegate() {}
	//virtual void onReadSocketData(YZSocketMessage* socketMessage) = 0;
	virtual void onReadComplete() = 0;
	//virtual void onConnected(bool connect, emSocketStatus status) = 0;
	//virtual void onDisConnect() = 0;
};

}

#endif	//__YZ_YZSocketMessageDelegate_H__
