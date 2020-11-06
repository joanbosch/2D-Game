#ifndef ENTITIES_INCLUDE
#define ENTITIES_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "Block.h"
#include "Axe.h"
#include "Wood.h"
#include "Coin.h"
#include "Bag.h"
#include "MultipleCoins.h"
#include "Diamond.h"
#include "Alarm.h"
#include "Police.h"
#include <vector>

class Entities
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap);
	void update(int deltaTime);
	void render();

	bool ballHasColided();
	glm::vec2 getN();

	int getNewCoins();
	int getNewPoints();
	int getRemainingMoneyEntities();
private:
	glm::ivec2 tileMapDispl, posPlayer;

	bool ballColided;
	int coins, points, moneyEntities;
	glm::vec2 N;

	vector<Block*> *blocks;
	vector<Wood*> *woods;
	vector<Coin*> *single_coins;
	vector<Bag*> *bags;
	vector<MultipleCoins*> *multiple_coins;
	vector<Diamond*> *diamonds;
	vector<Alarm*> *alarms;
	vector<Police*> *polices;
	vector<Axe*> *axes;

	TileMap *map;
};


#endif // ENTITIES_INCLUDE


