#ifndef _AUDIO_INCLUDE
#define _AUDIO_INCLUDE

#include "irrKlang.h"

using namespace irrklang;

#define NUM_SOUNDS 3

enum AUDIOS {
	MENU_MUSIC, LEVEL1_MUSIC, LEVEL2_MUSIC, LEVEL3_MUSIC, GAMEOVER_MUSIC, PLAYER_DEAD_MUSIC, 
	POINTS_SOUND, MONEY_SOUND, STAR_MODE_MUSIC, STAR_SOUND
};

class Audio
{
	public:
		Audio();
		~Audio();

		void init();
		void bindAudio(const char* file, int i);
		void play(int i, bool loop);
		void stopAllSounds();

	private:
		ISoundEngine *soundEngine;
		ISoundSource *sounds[NUM_SOUNDS];
};

#endif // _AUDIO_INCLUDE