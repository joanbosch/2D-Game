#ifndef _BLOCK_INCLUDE
#define _BLOCK_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Block
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int level);
	void update(int deltaTime, bool starMode);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool getBallColidad();
	glm::vec2 getN();
	bool isVisible();

private:

	bool visible;
	bool ballColided;

	int level;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

};


#endif // _BLOCK_INCLUDE


