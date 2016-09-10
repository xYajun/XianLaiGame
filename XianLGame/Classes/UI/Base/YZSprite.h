#ifndef __YZ_YZSprite_H__
#define __YZ_YZSprite_H__

#include "cocos2d.h"
USING_NS_CC;

namespace YZ {

	class YZSprite : public Sprite
	{
	public:
		YZSprite(void);
		static YZSprite* create(const std::string& filename);
		virtual ~YZSprite(void);
	};

};

#endif	//__YZ_YZSprite_H__

