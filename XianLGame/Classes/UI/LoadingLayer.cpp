#include "LoadingLayer.h"
#include "Base/YZLayerColor.h"

namespace YZ 
{
	static const int LOADING_LAYER_TAG		= 99999999;
	static const int LOADING_LAYER_ZORDER	= 99999999;

	LoadingLayer* LoadingLayer::createLoading(Node* parent, const TTFConfig& ttfConfig, const std::string& text, const std::string& image)
	{
		auto loading = (LoadingLayer*)parent->getChildByTag(LOADING_LAYER_TAG);;
		if (nullptr == loading)
		{
			loading = LoadingLayer::create();
			loading->setPosition(Size::ZERO);
			loading->createDialog(ttfConfig, text, image);
			parent->addChild(loading, LOADING_LAYER_ZORDER, LOADING_LAYER_TAG);
		}
		loading->setText(text);
		return loading;
	}

	LoadingLayer* LoadingLayer::createLoading(const TTFConfig& ttfConfig, const std::string& text, const std::string& image, const std::string& imageBG)
	{
		auto loading = LoadingLayer::create();
		loading->createDialog(ttfConfig, text, image, imageBG);
		return loading;
	}

	void LoadingLayer::removeLoading(Node* parent)
	{
		CCAssert(nullptr != parent, "parent is null");
		parent->removeChildByTag(LOADING_LAYER_TAG);
	}

	//////////////////////////////////////////////////////////////////////////

	LoadingLayer::LoadingLayer()
	{

	}

	LoadingLayer::~LoadingLayer()
	{

	}

	void LoadingLayer::createDialog(const TTFConfig& ttfConfig, const std::string& text, const std::string& image)
	{
		// 转圈动画	
		auto _loading = Sprite::create(image);
		_loading->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height / 2 + _loading->getContentSize().width / 2));
		this->addChild(_loading, 2);	
		_loading->runAction(RepeatForever::create(RotateBy::create(0.4f, 90.0f)));

		// 提示字样
		_label = Label::createWithTTF(ttfConfig, text);
		_label->setColor(Color3B(255, 255, 255));
		_label->setPosition(Vec2(this->getContentSize().width / 2, _loading->getPositionY() - _loading->getContentSize().height / 2 - 30));
		this->addChild(_label, 2);

		quicklyShade(200);
	}

	void LoadingLayer::createDialog(const TTFConfig& ttfConfig, const std::string& text, const std::string& image, const std::string& imageBG)
	{
		Layer* colorLayer = YZLayerColor::create();
		colorLayer->setTag(10);
		addChild(colorLayer);

		auto visibleSize = Director::getInstance()->getWinSize();

		Sprite* loadingBox = Sprite::create(imageBG);
		loadingBox->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
		addChild(loadingBox, 1);

		// 转圈动画	
		auto _loading = Sprite::create(image);
		_loading->setPosition(Vec2(loadingBox->getContentSize().width/2, loadingBox->getContentSize().height*0.6));
		loadingBox->addChild(_loading);	
		_loading->runAction(RepeatForever::create(RotateBy::create(0.4f, 90.0f)));

		// 提示字样
		_label = Label::createWithTTF(ttfConfig, text);
		_label->setColor(Color3B(255, 215, 72));
		_label->setPosition(Vec2(loadingBox->getContentSize().width / 2, loadingBox->getContentSize().height*0.2));
		loadingBox->addChild(_label);

		schedule(schedule_selector(LoadingLayer::autoSetOpacity), 0.01f);
		startShade();
	}

	void LoadingLayer::autoSetOpacity(float dt)
	{
		_opacity = _opacity + 3;
		Layer* colorLayer = (Layer*)this->getChildByTag(10);
		colorLayer->setOpacity(_opacity);
		if (_opacity >= 200) unschedule(schedule_selector(LoadingLayer::autoSetOpacity));
	}

	void LoadingLayer::setText(const std::string& text)
	{
		_label->setString(text);
	}

	bool LoadingLayer::init()
	{
		if (!YZLayer::init())
		{
			return false;
		}
		_opacity = 0;
		// 监听点击事件
		auto MyListener = EventListenerTouchOneByOne::create();
		// 阻止触摸向下传递
		MyListener->setSwallowTouches(true);
		MyListener->onTouchBegan = [&](Touch* touch, Event* event)
		{
			auto target = static_cast<Sprite*>(event->getCurrentTarget());      
			Point locationInNode = target->convertToNodeSpace(touch->getLocation());
			Size s = Director::getInstance()->getWinSize();
			Rect rect = Rect(0, 0, s.width, s.height); 
			return rect.containsPoint(locationInNode);
		};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(MyListener, this);

		return true;
	}
}