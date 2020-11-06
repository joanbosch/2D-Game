#ifndef _POLICE_INCLUDE
#define _POLICE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

#define SEARCHING 0
#define MOVING 1

class Police
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int r);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPolice; };

	void setVelocity(float v);

	int getRoom() { return room; };

	void setSearching();

private:
	glm::ivec2 tileMapDispl, posPolice, policeSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool ballColided, isOnScreen;
	int policeState;

	int room;
	float vel;

	glm::vec2 N;
};


#endif // _PLAYER_INCLUDE

