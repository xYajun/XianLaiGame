#ifndef _YZ_AUDIO_ENGINE_
#define _YZ_AUDIO_ENGINE_

#include "cocos2d.h"
USING_NS_CC;

namespace YZ
{

	class YZAudioEngine
	{
	public:
		static YZAudioEngine* getInstance();

	public:
		YZAudioEngine();

		//播放音乐
		void playBackgroundMusic(const char* pszFilePath, bool bLoop = false);

		//停止播放音乐
		void stopBackgroundMusic(bool bReleaseData = false);

		//暂停播放
		void pauseBackgroundMusic();

		//继续播放
		void resumeBackgroundMusic();

		//播放音效
		unsigned int playEffect(const char* pszFilePath, bool bLoop = false,float pitch = 1.0f, float pan = 0.0f, float gain = 1.0f);

		//设置音量
		void setEffectsVolume(float volume);
		void setBackgroundMusicVolume(float volume);

		//获得音量大小
		float getEffectsVolume() const;
		float getBackgroundMusicVolume() const;

		//设置声音开关
		void setSwitchOfMusic(bool isOpen);
		void setSwitcjofEffect(bool isOpen);

		//获取声音开发与否
		bool getSwitchOfMusic() const;
		bool getSwitcjofEffect() const;

	private:
		bool			_switchOfMusic;						//true设置音乐为开

		bool			_switchOfEffect;                    //ture设置音效为开

		std::string		_currentBackgroundName;				//当前背景音乐名称
	};

}
#endif 		//_YZ_AUDIO_ENGINE_


