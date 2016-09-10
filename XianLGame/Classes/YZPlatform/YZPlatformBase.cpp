#include "YZPlatformBase.h"
#include "YZPlatform/YZPlatformLogic.h"

namespace YZ {

YZPlatformBase::YZPlatformBase(void)
{
}

YZPlatformBase::~YZPlatformBase(void)
{
}

void YZPlatformBase::onEnter()
{
	YZScene::onEnter();
	PlatformLogic()->addObserver(this);
}

void YZPlatformBase::onExit()
{
	YZScene::onExit();
	PlatformLogic()->removeObserver(this);
}
}