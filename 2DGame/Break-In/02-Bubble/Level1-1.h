#ifndef _LVL1_1_INCLUDE
#define _LVL1_1_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Block.h"
#include "Axe.h"
#include "Wood.h"
#include "Coin.h"
#include "Bag.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Level11
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map);
	void update(int deltaTime);
	void render();

private:
	void initDesplBlocks();
	void initDesplWoods();
	void initDesplCoins();
	void initDesplBags();
	bool bJumping;
	glm::ivec2 tileMapDispl, posPlayer;
	int jumpAngle, startY;

	vector<Block*> blocks;
	vector<Wood*> woods;
	vector<Coin*> coins;
	vector<Bag*> bags;
	Axe* axe;

	TileMap* map;
	vector<pair<int,int>*> blocks_desp, wood_desp, coin_desp, bag_desp;
};


#endif // _LVL1_1_INCLUDE


