#include "YZLayer.h"
#include "YZLayerColor.h"

namespace YZ 
{
	YZLayer::YZLayer(void) :_colorLayer(nullptr), _opacity(0)
	{
	}

	YZLayer::~YZLayer(void)
	{
	}

	bool YZLayer::init()
	{
		if (!Layer::init())
		{
			return false;
		}
		this->setAnchorPoint(Vec2(0.5f, 0.5f));

		return true;
	}

	void YZLayer::setBackGroundImage(const std::string& name)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		//背景
		auto loadingBG = Sprite::create(name.c_str());
		loadingBG->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		float _xScale = visibleSize.width / loadingBG->getContentSize().width;
		float _yScale = visibleSize.height/ loadingBG->getContentSize().height;
		loadingBG->setScaleX(_xScale);
		loadingBG->setScaleY(_yScale);
		this->addChild(loadingBG, -1);
	}

	float YZLayer::getRealScaleX(int designWidth)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		return visibleSize.width / designWidth;
	}

	float YZLayer::getRealScaleY(int designHeight)
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();
		return visibleSize.height / designHeight;
	}

	void YZLayer::enableKeyboard()
	{
		//对手机返回键的监听
		auto listener = EventListenerKeyboard::create();
		//和回调函数绑定
		listener->onKeyReleased = CC_CALLBACK_2(YZLayer::onKeyReleased, this);
		//添加到事件分发器中
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
	}

	bool YZLayer::switchToLayoutControl(Layout* layout, const std::string& controlName, Node* newControl)
	{
		auto tmp = (Text*)Helper::seekWidgetByName(layout, controlName);
		if (nullptr == tmp) return false;
		newControl->setPosition(tmp->getPosition());
		newControl->setTag(tmp->getTag());
		newControl->setLocalZOrder(tmp->getLocalZOrder());
		newControl->setScaleX(tmp->getScaleX());
		newControl->setScaleY(tmp->getScaleY());
		newControl->setAnchorPoint(tmp->getAnchorPoint());
		layout->addChild(newControl);
		return true;
	}

	void YZLayer::startShade(int opacity)
	{
		_opacity = opacity;
		if (nullptr == _colorLayer)
		{
			_colorLayer = YZLayerColor::create();
			addChild(_colorLayer, 1);
			schedule(schedule_selector(YZLayer::updateOpacity), 0.1f);
		}
	}

	void YZLayer::quicklyShade(int opacity)
	{
		_opacity = opacity;
		if (nullptr == _colorLayer)
		{
			_colorLayer = YZLayerColor::create();
			addChild(_colorLayer, 1);
			_colorLayer->setOpacity(_opacity);
		}
	}

	void YZLayer::updateOpacity(float dt)
	{
		static unsigned char i = _opacity;
		i = i + 10;

		_colorLayer->setOpacity(i);
		if (i >= _opacity)
		{
			i = 0;
			unschedule(schedule_selector(YZLayer::updateOpacity));
		}
	}
}