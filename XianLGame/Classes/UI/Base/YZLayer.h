#ifndef __YZ_YZLayer_H__
#define __YZ_YZLayer_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
//#include "cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;
//using namespace cocostudio;

namespace YZ {

	class YZLayer : public Layer
	{
	public:
		YZLayer(void);
		virtual ~YZLayer(void);

		virtual bool init() override;

	public:
		void setBackGroundImage(const std::string& name);

		void enableKeyboard();
		
		void startShade(int opacity = 100);

		void quicklyShade(int opacity = 100);

	public:
		float getRealScaleX(int designWidth);

		float getRealScaleY(int designHeight);

	public:
		bool switchToLayoutControl(Layout* layout, const std::string& controlName, Node* newControl);

	private:
		void updateOpacity(float dt);

	private:
		Layer*			_colorLayer;
		unsigned char	_opacity;
	};

};

#endif	//__YZ_YZLayer_H__
