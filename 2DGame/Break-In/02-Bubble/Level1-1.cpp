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
#define NUM_WOODS 30


#define SCREEN_X 32
#define SCREEN_Y 42

void Level11::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map)
{
	initDesplBlocks();
	initDesplWoods();
	initDesplCoins();
	initDesplBags();

	bJumping = false;
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
	axe->setPosition(glm::vec2(6*map->getTileSize(), 6*map->getTileSize()));
	axe->setTileMap(map);
	tileMapDispl = tileMapPos;

}

void Level11::update(int deltaTime)

{
	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks[i]->update(deltaTime);
	}
	for (int i = 0; i < NUM_WOODS; ++i) {
		woods[i]->update(deltaTime);
	}
	for (int i = 0; i < NUM_COINS; ++i) {
		coins[i]->update(deltaTime);
	}
	for (int i = 0; i < NUM_BAGS; ++i) {
		bags[i]->update(deltaTime);
	}
	axe->update(deltaTime);

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

void Level11::initDesplBlocks()
{
	blocks_desp.push_back(new std::pair<int, int>(2, 14));
	blocks_desp.push_back(new std::pair<int, int>(2, 15));
	blocks_desp.push_back(new std::pair<int, int>(4, 13));
	blocks_desp.push_back(new std::pair<int, int>(4, 14));
	blocks_desp.push_back(new std::pair<int, int>(6, 12));
	blocks_desp.push_back(new std::pair<int, int>(6, 13));
	blocks_desp.push_back(new std::pair<int, int>(8, 8));
	blocks_desp.push_back(new std::pair<int, int>(8, 9));
	blocks_desp.push_back(new std::pair<int, int>(10, 8));
	blocks_desp.push_back(new std::pair<int, int>(10, 9));
	blocks_desp.push_back(new std::pair<int, int>(12, 8));
	blocks_desp.push_back(new std::pair<int, int>(12, 9));
	blocks_desp.push_back(new std::pair<int, int>(14, 8));
	blocks_desp.push_back(new std::pair<int, int>(14, 9));
	blocks_desp.push_back(new std::pair<int, int>(20, 14));
	blocks_desp.push_back(new std::pair<int, int>(20, 15));
	blocks_desp.push_back(new std::pair<int, int>(18, 13));
	blocks_desp.push_back(new std::pair<int, int>(18, 14));
	blocks_desp.push_back(new std::pair<int, int>(16, 12));
	blocks_desp.push_back(new std::pair<int, int>(16, 13));
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

	// Wood level blocks
	wood_desp.push_back(new std::pair<int, int>(1,14));
	wood_desp.push_back(new std::pair<int, int>(2,13));
	wood_desp.push_back(new std::pair<int, int>(3,13));
	wood_desp.push_back(new std::pair<int, int>(4,12));
	wood_desp.push_back(new std::pair<int, int>(5,12));
	wood_desp.push_back(new std::pair<int, int>(6,11));
	wood_desp.push_back(new std::pair<int, int>(7,11));

	wood_desp.push_back(new std::pair<int, int>(4,4));
	wood_desp.push_back(new std::pair<int, int>(5,4));
	wood_desp.push_back(new std::pair<int, int>(6,5));
	wood_desp.push_back(new std::pair<int, int>(7,5));

	wood_desp.push_back(new std::pair<int, int>(16,5));
	wood_desp.push_back(new std::pair<int, int>(17,5));
	wood_desp.push_back(new std::pair<int, int>(18,4));
	wood_desp.push_back(new std::pair<int, int>(19,4));

	wood_desp.push_back(new std::pair<int, int>(16,11));
	wood_desp.push_back(new std::pair<int, int>(17,11));
	wood_desp.push_back(new std::pair<int, int>(18,12));
	wood_desp.push_back(new std::pair<int, int>(19,12));
	wood_desp.push_back(new std::pair<int, int>(20,13));
	wood_desp.push_back(new std::pair<int, int>(21,13));
	wood_desp.push_back(new std::pair<int, int>(22,14));
	
}

void Level11::initDesplCoins()
{
}

void Level11::initDesplBags()
{
}


