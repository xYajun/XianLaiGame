#include "YZAudioEngine.h"
#include <SimpleAudioEngine.h>

namespace YZ
{
	static YZAudioEngine* instant = nullptr;

	YZAudioEngine::YZAudioEngine()
	{
		_switchOfMusic = true;
		_switchOfEffect = true;
	}

	YZAudioEngine* YZAudioEngine::getInstance()
	{
		if (nullptr == instant)
		{
			instant = new YZAudioEngine();
		}
		return instant;
	}

	void YZAudioEngine::setSwitchOfMusic(bool isOpen)
	{
		_switchOfMusic = isOpen;
	}

	bool YZAudioEngine::getSwitchOfMusic() const
	{
		return _switchOfMusic;
	}

	void YZAudioEngine::setSwitcjofEffect(bool isOpen)
	{
		_switchOfEffect = isOpen;
	}

	bool YZAudioEngine::getSwitcjofEffect() const
	{
		return _switchOfEffect;
	}

	void YZAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
	{
        _switchOfMusic = false;
		if (_switchOfMusic)
		{
			if (nullptr != pszFilePath)
			{
				if (0 != _currentBackgroundName.compare(pszFilePath))
				{
					_currentBackgroundName = pszFilePath;
					CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(pszFilePath, bLoop);
				}
			}
		}
	}

	void YZAudioEngine::stopBackgroundMusic(bool bReleaseData)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic(bReleaseData);
	}

	void YZAudioEngine::pauseBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void YZAudioEngine::resumeBackgroundMusic()
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	unsigned int YZAudioEngine::playEffect(const char* pszFilePath, bool bLoop, float pitch, float pan, float gain)
	{
        _switchOfEffect = false;
        
		if (_switchOfEffect)
		{
			return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(pszFilePath, bLoop, pitch, pan, gain);
		}
		return 0;
	}

	void YZAudioEngine::setEffectsVolume(float volume)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(volume);
	}

	float YZAudioEngine::getEffectsVolume() const
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getEffectsVolume();
	}

	void YZAudioEngine::setBackgroundMusicVolume(float volume)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
	}

	float YZAudioEngine::getBackgroundMusicVolume() const
	{
		return CocosDenshion::SimpleAudioEngine::getInstance()->getBackgroundMusicVolume();
	}
}
