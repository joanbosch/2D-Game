#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Block.h"
#include "Game.h"
#include "Level1-1.h"

#define NUM_BLOCKS 20
#define NUM_COINS 0
#define NUM_BAGS 0
#define NUM_WOODS 8


#define SCREEN_X 32
#define SCREEN_Y 42

void Level11::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map)
{
	initDesplBlocks();
	initDesplWoods();
	initDesplCoins();
	initDesplBags();

	bJumping = false;
	ballColided = false;

	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks.push_back(new Block());
		blocks[i]->init(glm::ivec2(tileMapPos[0], tileMapPos[1]), shaderProgram);
		blocks[i]->setPosition(glm::vec2(blocks_desp[i]->first * map->getTileSize(), blocks_desp[i]->second * map->getTileSize()));
		blocks[i]->setTileMap(map);
	}

	for (int i = 0; i < NUM_WOODS; ++i) {
		woods.push_back(new Wood());
		woods[i]->init(glm::ivec2(tileMapPos[0], tileMapPos[1]), shaderProgram);
		woods[i]->setPosition(glm::vec2(wood_desp[i]->first * map->getTileSize(), wood_desp[i]->second * map->getTileSize()));
		woods[i]->setTileMap(map);
	}

	for (int i = 0; i < NUM_COINS; ++i) {
		coins.push_back(new Coin());
		coins[i]->init(glm::ivec2(tileMapPos[0], tileMapPos[1]), shaderProgram);
		coins[i]->setPosition(glm::vec2(coin_desp[i]->first * map->getTileSize(), coin_desp[i]->second * map->getTileSize()));
		coins[i]->setTileMap(map);
	}

	for (int i = 0; i < NUM_BAGS; ++i) {
		bags.push_back(new Bag());
		bags[i]->init(glm::ivec2(tileMapPos[0], tileMapPos[1]), shaderProgram);
		bags[i]->setPosition(glm::vec2(bag_desp[i]->first * map->getTileSize(), bag_desp[i]->second * map->getTileSize()));
		bags[i]->setTileMap(map);
	}

	axe = new Axe();
	axe->init(glm::ivec2(tileMapPos[0], tileMapPos[1]), shaderProgram);
	axe->setPosition(glm::vec2(6*map->getTileSize(), 5*map->getTileSize()));
	axe->setTileMap(map);
	tileMapDispl = tileMapPos;

}

void Level11::update(int deltaTime)
{
	ballColided = false;

	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks[i]->update(deltaTime);
		bool aux = blocks[i]->getBallColidad();
		ballColided |= aux;
		if (aux) N = blocks[i]->getN();
	}
	for (int i = 0; i < NUM_WOODS; ++i) {
		if (!axe->isVisible()) woods[i]->setVisibility(false);
		woods[i]->update(deltaTime);
		bool aux = woods[i]->getBallColidad();
		ballColided |= aux;
		if (aux) N = woods[i]->getN();
	}
	for (int i = 0; i < NUM_COINS; ++i) {
		coins[i]->update(deltaTime);
	}
	for (int i = 0; i < NUM_BAGS; ++i) {
		bags[i]->update(deltaTime);
	}
	axe->update(deltaTime);
	bool aux = axe->getBallColided();
	ballColided |= aux;
	if (aux) N = axe->getN();

}

void Level11::render()
{
	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks[i]->render();
	}
	for (int i = 0; i < NUM_WOODS; ++i) {
		woods[i]->render();
	}
	for (int i = 0; i < NUM_COINS; ++i) {
		coins[i]->render();
	}
	for (int i = 0; i < NUM_BAGS; ++i) {
		bags[i]->render();
	}
	axe->render();
}

bool Level11::ballHasColided()
{
	return ballColided;
}



glm::vec2 Level11::getN()
{
	return N;
}

void Level11::initDesplBlocks()
{
	blocks_desp.push_back(new std::pair<int, int>(2, 13));
	blocks_desp.push_back(new std::pair<int, int>(2, 14));
	blocks_desp.push_back(new std::pair<int, int>(4, 12));
	blocks_desp.push_back(new std::pair<int, int>(4, 13));
	blocks_desp.push_back(new std::pair<int, int>(6, 11));
	blocks_desp.push_back(new std::pair<int, int>(6, 12));
	blocks_desp.push_back(new std::pair<int, int>(8, 7));
	blocks_desp.push_back(new std::pair<int, int>(8, 8));
	blocks_desp.push_back(new std::pair<int, int>(10, 7));
	blocks_desp.push_back(new std::pair<int, int>(10, 8));
	blocks_desp.push_back(new std::pair<int, int>(12, 7));
	blocks_desp.push_back(new std::pair<int, int>(12, 8));
	blocks_desp.push_back(new std::pair<int, int>(14, 7));
	blocks_desp.push_back(new std::pair<int, int>(14, 8));
	blocks_desp.push_back(new std::pair<int, int>(20, 13));
	blocks_desp.push_back(new std::pair<int, int>(20, 14));
	blocks_desp.push_back(new std::pair<int, int>(18, 12));
	blocks_desp.push_back(new std::pair<int, int>(18, 13));
	blocks_desp.push_back(new std::pair<int, int>(16, 11));
	blocks_desp.push_back(new std::pair<int, int>(16, 12));
}

void Level11::initDesplWoods()
{
	// The first 8 sprites are to hide the arrows to change of level.
	wood_desp.push_back(new std::pair<int, int>(8,0));
	wood_desp.push_back(new std::pair<int, int>(9,0));
	wood_desp.push_back(new std::pair<int, int>(10,0));
	wood_desp.push_back(new std::pair<int, int>(11,0));
	wood_desp.push_back(new std::pair<int, int>(12,0));
	wood_desp.push_back(new std::pair<int, int>(13,0));
	wood_desp.push_back(new std::pair<int, int>(14,0));
	wood_desp.push_back(new std::pair<int, int>(15,0));
}

void Level11::initDesplCoins()
{
}

void Level11::initDesplBags()
{
}


