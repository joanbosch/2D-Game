#include <cmath>
#include <iostream>
#include <ostream>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entities.h"

using namespace std;

#define SCREEN_X 32
#define SCREEN_Y 42

void Entities::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map)
{
	ballColided = false;

	int numEntities = map->getNEntities();

	woods = new vector<Wood*>();
	blocks = new vector<Block*>();
	single_coins = new vector<Coin*>();
	bags = new vector<Bag*>();
	/*
	multiple_coins = new vector<MultipleCoins>();
	diamonds = new vector<Diamonds>();
	*/


	for (int i = 0; i < numEntities; ++i) {
		int entityType = map->getEntity(i).type;
		glm::ivec2 tilemap = tileMapPos;

		if (entityType == WOOD) {
			Wood* aux = new Wood();
			aux->init(tilemap, shaderProgram);
			aux->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			aux->setTileMap(map);
			woods->push_back(aux);
		}
		else if (entityType == ORANGE_BLOCK) {
			Block* or_block = new Block();
			// TODO: init block's resistance
			or_block->init(tilemap, shaderProgram);
			or_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			or_block->setTileMap(map);
			blocks->push_back(or_block);
		}
		else if (entityType == GREEN_BLOCK) {
			Block* gr_block = new Block();
			// TODO: init block's resistance
			gr_block->init(tilemap, shaderProgram);
			gr_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			gr_block->setTileMap(map);
			blocks->push_back(gr_block);
		} 
		else if (entityType == BLUE_BLOCK) {
			Block* bl_block = new Block();
			// TODO: init block's resistance
			bl_block->init(tilemap, shaderProgram);
			bl_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			bl_block->setTileMap(map);
			blocks->push_back(bl_block);
		}
		else if (entityType == SINGLE_COIN) {
			Coin* coin = new Coin();
			coin->init(tilemap, shaderProgram);
			coin->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			coin->setTileMap(map);
			single_coins->push_back(coin);
		}
		else if (entityType == COINS_BAG) {
			Bag* bag = new Bag();
			bag->init(tilemap, shaderProgram);
			bag->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			bag->setTileMap(map);
			bags->push_back(bag);
		}
		/*else if (entityType == MULTIPLE_COINS) {
			MultipleCoins *coins = new MultipleCoins();
			coins->init(tilemap, shaderProgram);
			coins->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			coins->setTileMap(map);
			multiple_coins->push_back(coins);
		}
		else if (entityType == DIAMOND) {
			Diamond *diam = new Diamond();
			diam->init(tilemap, shaderProgram);
			diam->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			diam->setTileMap(map);
			diamonds->push_back(diam);
		}
		else if (entityType == ALARM) {
			alarm = new Alarm();
			alarm->init(tilemap, shaderProgram);
			alarm->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			alarm->setTileMap(map);
			tileMapDispl = tileMapPos;
		}
		*/
		else if (entityType == AXE) {
			axe = new Axe();
			axe->init(tilemap, shaderProgram);
			axe->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			axe->setTileMap(map);
			tileMapDispl = tileMapPos;
		}
	}
}

void Entities::update(int deltaTime)
{
	ballColided = false;

	for (int i = 0; i < blocks->size(); ++i) {
		if (!ballColided) {
			(*blocks)[i]->update(deltaTime);
			bool aux = (*blocks)[i]->getBallColidad();
			ballColided |= aux;
			if (aux) N = (*blocks)[i]->getN();
		}
		
	}
	for (int i = 0; i < woods->size(); ++i) {
		if (!axe->isVisible()) (*woods)[i]->setVisibility(false);
		if (!ballColided) {
			(*woods)[i]->update(deltaTime);
			bool aux = (*woods)[i]->getBallColidad();
			ballColided |= aux;
			if (aux) N = (*woods)[i]->getN();
		}
	}
	for (int i = 0; i < single_coins->size(); ++i) {
		(*single_coins)[i]->update(deltaTime);
	}
	for (int i = 0; i < bags->size(); ++i) {
		(*bags)[i]->update(deltaTime);
	}
	/*
	for (int i = 0; i < multiple_coins->size(); ++i) {
		(*multiple_coins)[i]->update(deltaTime);
	}
	for (int i = 0; i < diamonds->size(); ++i) {
		(*diamonds)[i]->update(deltaTime);
	}
	*/
	if (!ballColided) {
		axe->update(deltaTime);
		bool aux = axe->getBallColided();
		ballColided |= aux;
		if (aux) N = axe->getN();
	}
}

void Entities::render()
{
	for (int i = 0; i < blocks->size(); ++i) {
		(*blocks)[i]->render();
	}
	for (int i = 0; i < woods->size(); ++i) {
		(*woods)[i]->render();
	}
	for (int i = 0; i < single_coins->size(); ++i) {
		(*single_coins)[i]->render();
	}
	for (int i = 0; i < bags->size(); ++i) {
		(*bags)[i]->render();
	}
	/*
	for(int i = 0; i < multiple_coins->size(); ++i) {
		(*multiple_coins)[i]->render();
	}
	for(int i = 0; i < diamonds->size(); ++i) {
		(*diamonds)[i]->render();
	}
	alarm->render();
	*/
	axe->render();
}

bool Entities::ballHasColided()
{
	return ballColided;
}

glm::vec2 Entities::getN()
{
	return N;
}


