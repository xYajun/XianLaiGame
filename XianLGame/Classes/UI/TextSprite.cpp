#include "TextSprite.h"

namespace YZ {

	TextSprite* TextSprite::create(const std::string& filename)
	{
		TextSprite *sprite = new (std::nothrow) TextSprite();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	TextSprite* TextSprite::createWithTTF(const TTFConfig& ttfConfig, const std::string& filename)
	{
		TextSprite *sprite = new (std::nothrow) TextSprite();
		if (sprite && sprite->initWithTTF(ttfConfig, filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	TextSprite::TextSprite() : _text(nullptr), _indent(0.25f)
	{

	}

	TextSprite::~TextSprite()
	{

	}

	bool TextSprite::initWithTTF(const TTFConfig& ttfConfig, const std::string& filename)
	{
		_ttfConfig = ttfConfig;
		return initWithFile(filename);
	}

	void TextSprite::setText (const std::string& text, const Color4B &color)
	{
		createLabel(_ttfConfig, text, color);
	}

	void TextSprite::setText (const TTFConfig& ttfConfig, const std::string& text, const Color4B &color)
	{
		_ttfConfig = ttfConfig;
		createLabel(_ttfConfig, text, color);
	}

	void TextSprite::createLabel(const TTFConfig& ttfConfig, const std::string& text, const Color4B &color)
	{
		if (nullptr == _text)
		{
			_text = Label::create();
			_text->setAnchorPoint(Vec2(0.5f, 0.5f));
			_text->setPosition(Vec2(this->getContentSize().width / 2 + _indent, this->getContentSize().height / 2));
			addChild(_text, 2);
		}
		_text->setTTFConfig(ttfConfig);
		_text->setTextColor(color);
		_text->setString(text);
	}

	void TextSprite::setTextLayout(TextLayout layout)
	{
		if (nullptr == _text) return;
		if (CENTER == layout)
		{
			_text->setAnchorPoint(Vec2(0.5f, 0.5f));
		}
		else if (RIGHT == layout)
		{
			_text->setAnchorPoint(Vec2(0.5f, 0.0f));
		}
		else
		{
			_text->setAnchorPoint(Vec2(0.0f, 0.5f));
		}
	}

	void TextSprite::runTextAction()
	{
		if (nullptr != _text && !_text->getString().empty())
		{
			_text->runAction(Sequence::create(ScaleTo::create(0.5f, 1.2f), ScaleTo::create(0.5f, 1.0f), nullptr));
		}
	}

	void TextSprite::setIndent(float indent)
	{
		_indent = indent;
		if (nullptr != _text)
		{
			_text->setPosition(Vec2(this->getContentSize().width / 2 + _indent, this->getContentSize().height / 2));
		}
	}


}