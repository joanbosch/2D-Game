#ifndef _ALARM_INCLUDE
#define _ALARM_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Alarm
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	bool getBallColided();
	glm::vec2 getN();

	bool isOn() { return activated; };
	bool hasBeenActivated();

private:

	bool activated;
	bool ballColided;

	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _ALARM_INCLUDE




