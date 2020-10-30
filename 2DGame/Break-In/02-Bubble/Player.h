#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPlayer; };

	void setVisibility(bool vis);

private:
	glm::ivec2 tileMapDispl, posPlayer, playerSize;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool visible;
};


#endif // _PLAYER_INCLUDE