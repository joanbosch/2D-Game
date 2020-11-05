#ifndef _POLICE_INCLUDE
#define _POLICE_INCLUDE


#include "Sprite.h"
#include "TileMap.h"

class Police
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() { return posPolice; };

	void setVisibility(bool vis);

private:
	glm::ivec2 tileMapDispl, posPolice;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool visible;
	bool ballColided;

	glm::vec2 N;
};


#endif // _PLAYER_INCLUDE

