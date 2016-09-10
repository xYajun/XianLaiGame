#include "Tools.h"
#include <regex>

namespace YZ
{
	bool Tools::verifyEmailAddress(const std::string& email)
	{
		std::regex pattern("([0-9A-Za-z\\-_\\.]+)@([0-9a-z]+\\.[a-z]{2,3}(\\.[a-z]{2})?)");
		return std::regex_match(email, pattern);
	}

	bool Tools::verifyPhone(const std::string& phone)
	{
		return true;
	}

	bool Tools::verifyChinese(const std::string& word)
	{
		std::regex pattern("^[\u4e00-\u9fa5]+$");
		return std::regex_match(word, pattern);
	}

	bool Tools::verifyNumber(const std::string& word)
	{
		std::regex pattern("^[0-9]*$");
		return std::regex_match(word, pattern);
	}

	bool Tools::verifyNumberAndEnglish(const std::string& word)
	{
		std::regex pattern("^[A-Za-z0-9]+$");
		return std::regex_match(word, pattern);
	}

	std::string Tools::base64urlencode(const std::string &str)
	{
		std::string encode = str;
		std::string::size_type pos(0);
		while(( pos = encode.find("+") ) != std::string::npos)
		{
			encode.replace(pos,1,"%2B");
		}
		pos = 0;
		while(( pos = encode.find("/") ) != std::string::npos)
		{
			encode.replace(pos,1,"%2F");
		}
		return encode;
	}
}