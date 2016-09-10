#include "PlatformConfig.h"

static PlatformConfig* sPlatformConfig = nullptr;

PlatformConfig* PlatformConfig::getInstance()
{
	if (nullptr == sPlatformConfig)
	{
		sPlatformConfig = new PlatformConfig();
	}
	return sPlatformConfig;
}

void PlatformConfig::setGameLogo(const std::string& logoGame)
{
	_gameLogo = logoGame;
}

std::string PlatformConfig::getGameLogo() const
{
	return _gameLogo;
}

void PlatformConfig::setSplashLogo(const std::string& splashLogo)
{
	_splashLogo = splashLogo;
}

std::string PlatformConfig::getSplashLogo() const
{
	return _splashLogo;
}

void PlatformConfig::setGameDesignSize(Size designSize)
{
	_gameDesignSize = designSize;
}

Size PlatformConfig::getGameDesignSize() const
{
	return _gameDesignSize;
}

void PlatformConfig::setPlatformDesignSize(Size designSize)
{
	_platformDesignSize = designSize;
}

Size PlatformConfig::getPlatformDesignSize() const
{
	return _platformDesignSize;
}

void PlatformConfig::setAccountType(PlatformConfig::ACCOUNT_TYPE type)
{
	_accountType = type;
}

PlatformConfig::ACCOUNT_TYPE PlatformConfig::getAccountType() const
{
	return _accountType;
}

PlatformConfig::PlatformConfig()
	: _gameDesignSize(1136, 640)
	, _platformDesignSize(1136, 640)
	, _accountType(UNKNOWN)
{
}

PlatformConfig::~PlatformConfig()
{
}        