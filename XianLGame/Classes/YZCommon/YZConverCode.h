#ifndef _YZ_ConverCode_h__
#define _YZ_ConverCode_h__

#include "cocos2d.h"
USING_NS_CC;
#include <string>
namespace YZ {
	class YZConverCode
	{
	public:
		static std::string GB2312ToUtf8(const char *gb2313);

		static std::string Utf8ToGB2312(const char *utf8);
	};
}

#define GBKToUtf8(object) YZConverCode::GB2312ToUtf8(object).c_str()

#define Utf8ToGB(object) YZConverCode::Utf8ToGB2312(object).c_str()

#endif // _YZ_ConverCode_h__
