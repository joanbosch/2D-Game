#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include "irrKlang.h"

using namespace irrklang;

#define NUM_SOUNDS 14

enum AUDIOS {
	MENU_MUSIC, LEVEL1_MUSIC, LEVEL2_MUSIC, LEVEL3_MUSIC, GAMEOVER_MUSIC, PLAYER_DEAD_MUSIC, 
	POINTS_SOUND, BAG_SOUND, COIN_SOUND, DIAMOND_SOUND, STAR_MODE_MUSIC, STAR_SOUND, 
	ALARM_SOUND, BOUNCE_SOUND
};

class Audio
{
	public:
		Audio();
		~Audio();

		void init();
		void play(int i, bool loop);
		void stop(int i);
		void stopAllSounds();
		bool isPlaying(int i);

	private:
		ISoundEngine *soundEngine;
		ISoundSource *sounds[NUM_SOUNDS];
};

#endif // _AUDIO_INCLUDE