#ifndef PlatformConfig_h__
#define PlatformConfig_h__

#include "YZBaseType.h"
#include <string>

#include "cocos2d.h"
USING_NS_CC;

class PlatformConfig
{
public:
	enum ACCOUNT_TYPE
	{
		UNKNOWN = 0,
		GUEST,
		NORMAL
	};
public:
	static PlatformConfig* getInstance();

	void setGameLogo(const std::string& logoGame);
	std::string getGameLogo() const;

	void setSplashLogo(const std::string& splashLogo);
	std::string getSplashLogo() const;

	void setGameDesignSize(Size designSize);
	Size getGameDesignSize() const;

	void setPlatformDesignSize(Size designSize);
	Size getPlatformDesignSize() const;

	void setAccountType(ACCOUNT_TYPE type);
	ACCOUNT_TYPE getAccountType() const;

private:
	PlatformConfig();
	~PlatformConfig();

private:
	std::string		_gameLogo;
	std::string		_splashLogo;
	Size			_gameDesignSize;
	Size			_platformDesignSize;
	ACCOUNT_TYPE	_accountType;
};

#endif // PlatformConfig_h__
