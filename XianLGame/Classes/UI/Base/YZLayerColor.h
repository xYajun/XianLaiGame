#ifndef __YZ_YZLayerColor_H__
#define __YZ_YZLayerColor_H__

#include "cocos2d.h"
USING_NS_CC;

namespace YZ {

	class YZLayerColor : public LayerColor
	{
	public:
		YZLayerColor(void);
		virtual ~YZLayerColor(void);

		virtual bool init() override;
	};
};

#endif	//__YZ_YZLayerColor_H__
