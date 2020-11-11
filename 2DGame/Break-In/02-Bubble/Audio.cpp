#include "Audio.h"
#include "Windows.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

Audio::Audio()
{
	soundEngine = createIrrKlangDevice();
	init();
	soundEngine->setSoundVolume(0.7f);
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
	sounds[LEVEL3_MUSIC] = soundEngine->addSoundSourceFromFile("music/lvl3Music.mp3");
	sounds[GAMEOVER_MUSIC] = soundEngine->addSoundSourceFromFile("music/gameoverMusic.mp3");
	sounds[PLAYER_DEAD_SOUND] = soundEngine->addSoundSourceFromFile("music/deadPlayerSound.mp3");
	sounds[POINTS_SOUND] = soundEngine->addSoundSourceFromFile("music/pointsSound.mp3");
	sounds[BAG_SOUND] = soundEngine->addSoundSourceFromFile("music/moneyBagSound.wav");
	sounds[COIN_SOUND] = soundEngine->addSoundSourceFromFile("music/coinSound.wav");
	sounds[DIAMOND_SOUND] = soundEngine->addSoundSourceFromFile("music/diamondSound.wav");
	sounds[STAR_MODE_MUSIC] = soundEngine->addSoundSourceFromFile("music/starModeMusic.mp3");
	sounds[STAR_SOUND] = soundEngine->addSoundSourceFromFile("music/starModeSound.wav");
	sounds[ALARM_SOUND] = soundEngine->addSoundSourceFromFile("music/alarmSound.flac");
	sounds[BOUNCE_SOUND] = soundEngine->addSoundSourceFromFile("music/bounceSound.wav");
	sounds[NOOK_LAUGH] = soundEngine->addSoundSourceFromFile("music/nookLaughSound.mp3");
	sounds[WIN_MUSIC] = soundEngine->addSoundSourceFromFile("music/winMusic.mp3");
	sounds[CHANGE_LEVEL_MUSIC] = soundEngine->addSoundSourceFromFile("music/changeLvlMusic.mp3");
}

void Audio::play(int i, bool loop)
{
	soundEngine->play2D(sounds[i], loop);
}

void Audio::stop(int i)
{
	soundEngine->stopAllSoundsOfSoundSource(sounds[i]);
}

void Audio::stopAllSounds()
{
	soundEngine->stopAllSounds();
}

bool Audio::isPlaying(int i)
{
	return soundEngine->isCurrentlyPlaying(sounds[i]);
}
