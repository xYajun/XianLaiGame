#include "YZSprite.h"

namespace YZ {

	YZSprite* YZSprite::create(const std::string& filename)
	{
		YZSprite *sprite = new (std::nothrow) YZSprite();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	YZSprite::YZSprite(void)
	{

	}

	YZSprite::~YZSprite(void)
	{

	}

}