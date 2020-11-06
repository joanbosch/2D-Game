#ifndef _THIEF_INCLUDE
#define _THIEF_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Thief
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPlayer; };
private:
	glm::ivec2 tileMapDispl, posPlayer, playerSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	float posPlayerx;
};


#endif // _THIEF_INCLUDE

