#include "FishKingGameAudio.h"
#include "YZUIExport.h"
#include "SimpleAudioEngine.h"

namespace FishKing
{
	const char*		GAME_AUDIO_ROOT	 =	"fishKing/sounds/effect/";

	void FishKingGameAudio::playBackgroundMusic()
	{
		char str[12];
		std::string filename("fishKing/sounds/bgm/");
		int num = rand()%6 + 1;
		sprintf(str, "bgm%d.mp3", num);
		filename.append(str);
		YZAudioEngine::getInstance()->playBackgroundMusic(filename.c_str(), true);
	}

	void FishKingGameAudio::stopBackgroundMusic()
	{
		YZAudioEngine::getInstance()->stopBackgroundMusic();
	}

	void FishKingGameAudio::pauseBackgroundMusic()
	{
		YZAudioEngine::getInstance()->pauseBackgroundMusic();
	}

	void FishKingGameAudio::resumeBackgroundMusic()
	{
		YZAudioEngine::getInstance()->resumeBackgroundMusic();
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void FishKingGameAudio::playSmallFishDiead(FishKind kind)
	{
		std::string filename(GAME_AUDIO_ROOT);

		int num = rand()%2 + 1;
		char str[24];
		sprintf(str, "fish%d_%d.mp3", kind + 1, num);
		filename.append(str);
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playFireEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("fire.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playNetEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("bgm_net.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playIonFireEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("ion_fire.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playBingoEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("bingo.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playGoldEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("gold.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playWaveEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("wave.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playCatchEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("catch.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

	void FishKingGameAudio::playBombEffect()
	{
		std::string filename(GAME_AUDIO_ROOT);
		filename.append("superarm.mp3");
		YZAudioEngine::getInstance()->playEffect(filename.c_str());
	}

}
