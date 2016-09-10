#ifndef TextSprite_h__
#define TextSprite_h__

#include "Base/YZSprite.h"

namespace YZ {

	class TextSprite : public YZSprite
	{
	public:
		enum TextLayout
		{
			LEFT = 0,
			CENTER,
			RIGHT
		};
	public:
		TextSprite();
		virtual ~TextSprite();

	public:
		static TextSprite* create(const std::string& filename);
		static TextSprite* createWithTTF(const TTFConfig& ttfConfig, const std::string& filename);

	public:
		bool initWithTTF(const TTFConfig& ttfConfig, const std::string& filename);

		void setText (const std::string& text, const Color4B &color = Color4B::WHITE);

		void setText (const TTFConfig& ttfConfig, const std::string& text, const Color4B &color = Color4B::WHITE);

		void setTextLayout(TextLayout layout);

		void runTextAction();

		void setIndent(float indent);

	private:
		void createLabel(const TTFConfig& ttfConfig, const std::string& text, const Color4B &color = Color4B::WHITE);

	private:
		Label*			_text;
		TTFConfig		_ttfConfig;
		float	_indent;
	};

}

#endif // TextSprite_h__
