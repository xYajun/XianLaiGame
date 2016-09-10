#ifndef __YZ_YZGameUIBase_H__
#define __YZ_YZGameUIBase_H__

#include "YZUIExport.h"
#include "YZBaseType.h"

namespace YZ
{
	class YZGameUIBase: public YZLayer
	{
	public:
		YZGameUIBase();
		virtual ~YZGameUIBase();

	public:
		void sendData(UINT MainID, UINT AssistantID, void* object = nullptr, INT objectSize = 0);

	};
}


#endif