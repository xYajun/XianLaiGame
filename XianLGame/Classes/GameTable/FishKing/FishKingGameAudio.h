#ifndef __HN_FishKing_GAME_AUDIO_H__
#define __HN_FishKing_GAME_AUDIO_H__

#include <string>
#include "FishKingMessageHead.h"

namespace FishKing
{

	class FishKingGameAudio
	{
	public:
		static void playBackgroundMusic();
		static void stopBackgroundMusic();
		static void pauseBackgroundMusic();
		static void resumeBackgroundMusic();

		static void playSmallFishDiead(FishKind kind);
		static void playFireEffect();
		static void playNetEffect();
		static void playIonFireEffect();
		static void playBingoEffect();
		static void playGoldEffect();
		static void playWaveEffect();
		static void playCatchEffect();
		static void playBombEffect();

	};
}


#endif