#include "YZLayerColor.h"

namespace YZ {

YZLayerColor::YZLayerColor(void)
{
}

YZLayerColor::~YZLayerColor(void)
{
}

bool YZLayerColor::init()
{
	if (!LayerColor::init())
	{
		return false;
	}
	this->setAnchorPoint(Vec2(0.5f, 0.5f));
	return true;
}


}