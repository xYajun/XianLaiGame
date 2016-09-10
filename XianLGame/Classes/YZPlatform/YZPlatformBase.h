#ifndef __YZ_YZPlatformBase_H__
#define __YZ_YZPlatformBase_H__

#include "YZPlatform/YZPlatformNotifyDelegate.h"
#include "YZSocket/YZSocketMessage.h"
#include "YZUIExport.h"

namespace YZ {

	class YZPlatformBase : public YZScene, IPlatformNotifyDelegate
	{
	public:
		YZPlatformBase(void);
		virtual ~YZPlatformBase(void);

	public:
		virtual void onEnter() override;

		virtual void onExit() override;

	};
};

#endif	//__YZ_YZPlatformScene_H__
