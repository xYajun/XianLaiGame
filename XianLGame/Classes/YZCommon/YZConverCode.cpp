#include "YZConverCode.h"
#include "YZSingleAutoPtr.h"

#include <errno.h>
#include <assert.h>

#include "cocos2d.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "win32-specific/icon/include/iconv.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../../../../libiconv/include/iconv.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#endif


namespace YZ {

int code_convert(const char *from_charset, const char *to_charset,
                const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    memset(outbuf, 0, outlen);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	 const char *_inbuf = (char*)inbuf;
#else
	char *_inbuf = (char*)inbuf;
#endif // CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
   
    size_t outbytes = outlen;
    
	size_t ret = -1;
	iconv_t ic = NULL;
	ic = iconv_open (to_charset, from_charset);
	if (ic == (iconv_t *) - 1)
	{
		CCLOG ("iconv_open failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
		return -1;
	}

	while (inlen > 0)
	{
		ret = iconv (ic, &_inbuf, &inlen, &outbuf, &outbytes);
		
		if (ret == -1)
		{
			CCLOG ("iconv failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
			return -1;
		}
	}

	ret = iconv_close (ic);
	if (ret == -1)
	{
		CCLOG ("iconv_close failed: from: %s, to: %s: %s", from_charset, to_charset, strerror(errno));
		return -1;
	}
    
	return 0;
}
    
    int IsTextUTF8(const char* str,long length)
    {
        int i;
        int nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
        unsigned char chr;
        bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
        for(i=0;i<length;i++)
        {
            chr= *(str+i);
            if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
                bAllAscii= false;
            if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
            {
                if(chr>=0x80)
                {
                    if(chr>=0xFC&&chr<=0xFD)
                        nBytes=6;
                    else if(chr>=0xF8)
                        nBytes=5;
                    else if(chr>=0xF0)
                        nBytes=4;
                    else if(chr>=0xE0)
                        nBytes=3;
                    else if(chr>=0xC0)
                        nBytes=2;
                    else
                    {
                        return false;
                    }
                    nBytes--;
                }
            }
            else //多字节符的非首字节,应为 10xxxxxx
            {
                if( (chr&0xC0) != 0x80 )
                {
                    return false;
                }
                nBytes--;
            }
        }
        
        if( nBytes > 0 ) //违返规则
        {
            return false;
        }
        
        if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
        {
            return false;
        }
        return true;
    }


//UTF8->GB2312
std::string YZConverCode::Utf8ToGB2312(const char *gb2313)
{
    if (!IsTextUTF8(gb2313 , strlen(gb2313))) {
        return gb2313;
    }
    
	size_t inlen = strlen(gb2313);
	if (0 == inlen) return "";

    size_t outlen = inlen * 2 + 2;
    YZPointerArray<char> outbuf(new char[outlen]);
    std::string strRet("");
    if(code_convert("utf-8", "gbk", gb2313, inlen, outbuf, outlen) == 0)
    {
        strRet = outbuf;
    }
    return strRet;
}

//GB2312->UTF8
std::string YZConverCode::GB2312ToUtf8(const char *utf8)
{
    if (IsTextUTF8(utf8 , strlen(utf8))) {
        return utf8;
    }
    
    size_t inlen = strlen(utf8);
    if (0 == inlen) return "";
    
    size_t outlen = inlen * 2 + 2;
    YZPointerArray<char> outbuf(new char[outlen]);
    std::string strRet(utf8);
    if(code_convert("gbk", "utf-8", utf8, inlen, outbuf, outlen) == 0)
    {
        strRet = outbuf;
    }
    return strRet;
}

}