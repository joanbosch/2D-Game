#include "Audio.h"
#include "Windows.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

Audio::Audio()
{
	soundEngine = createIrrKlangDevice();
	init();
}


Audio::~Audio()
{
	for (int i = 0; i < NUM_SOUNDS; ++i) {
		sounds[i]->drop();
		sounds[i] = 0;
	}
	soundEngine->drop();
}

void Audio::init()
{
	sounds[MENU_MUSIC] = soundEngine->addSoundSourceFromFile("music/menuMusic.mp3");
	sounds[LEVEL1_MUSIC] = soundEngine->addSoundSourceFromFile("music/lvl1Music.mp3");
	sounds[LEVEL2_MUSIC] = soundEngine->addSoundSourceFromFile("music/lvl2Music.mp3");
	/*sounds[LEVEL3_MUSIC] = soundEngine->addSoundSourceFromFile("music/lvl3Music.mp3");
	sounds[GAMEOVER_MUSIC] = soundEngine->addSoundSourceFromFile("music/gameoverMusic.mp3");
	sounds[PLAYER_DEAD_MUSIC] = soundEngine->addSoundSourceFromFile("music/playerdeadSound.mp3");*/
	sounds[POINTS_SOUND] = soundEngine->addSoundSourceFromFile("music/points.mp3");
	/*sounds[MONEY_SOUND] = soundEngine->addSoundSourceFromFile("music/money.mp3");
	sounds[STAR_MODE_MUSIC] = soundEngine->addSoundSourceFromFile("music/starmodeMusic.mp3");
	sounds[STAR_SOUND] = soundEngine->addSoundSourceFromFile("music/starSound.mp3");*/
}

void Audio::play(int i, bool loop) {
	soundEngine->play2D(sounds[i], loop);
}

void Audio::stopAllSounds() {
	soundEngine->stopAllSounds();
}
