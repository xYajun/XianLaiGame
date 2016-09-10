#ifndef __HS_YZDialogBase_H__
#define __HS_YZDialogBase_H__

#include "Base/YZLayer.h"
#include "cocos2d.h"

namespace YZ
{
	class YZDialogBase: public YZLayer
	{
	public:
		virtual void show();
		virtual void hide();
		virtual void setShowPoint(const cocos2d::Vec2& showPoint){ _showPoint = showPoint;}
		virtual void setHidePoint(const cocos2d::Vec2& hidePoint){ _hidePoint = hidePoint;}
		virtual cocos2d::Vec2 getHidePoint(){ return _hidePoint;}
		virtual void locateMid(cocos2d::Node* pNode);
	public:
		YZDialogBase();
		virtual ~YZDialogBase();
		bool init(cocos2d::Node* parent);
	private:
		cocos2d::Node* _parent;
		cocos2d::Vec2 _showPoint;
		cocos2d::Vec2 _hidePoint;
	};
}

#endif