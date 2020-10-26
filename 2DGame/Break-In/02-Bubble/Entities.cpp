#include <cmath>
#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entities.h"

using namespace std;

#define SCREEN_X 32
#define SCREEN_Y 42

void Entities::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap)
{
	ballColided = false;

	map = tileMap;

	int numEntities = map->getNEntities();

	woods = new vector<Wood*>();
	blocks = new vector<Block*>();
	single_coins = new vector<Coin*>();
	bags = new vector<Bag*>();
	/*
	multiple_coins = new vector<MultipleCoins>();
	diamonds = new vector<Diamonds>();
	*/
	axes = new vector<Axe*>();


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
			Alarm* alarm = new Alarm();
			alarm->init(tilemap, shaderProgram);
			alarm->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			alarm->setTileMap(map);
			tileMapDispl = tileMapPos;
			alarms->push_back(alarm);
		}
		*/
		else if (entityType == AXE) {
			Axe* axe = new Axe();
			axe->init(tilemap, shaderProgram);
			axe->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			axe->setTileMap(map);
			tileMapDispl = tileMapPos;
			axes->push_back(axe);
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

	// AXES -> 2 habitacions nomes REPENSAR
	//int room = 3 - map->getActualRoom();
	//// update woods panels
	//int num_woods = 2;
	//switch (room)
	//{
	//case 0:		// actually room 3
	//	num_woods = 4;
	//	break;
	//case 1:		// actually room 2
	//	num_woods = 6;
	//	break;
	//case 2:		// actually room 1
	//	num_woods = 8;
	//	break;
	//}
	//for (int i = 0; i < num_woods; ++i) {
	//	int pos = room * num_woods + i;
	//	if (!(*axes)[room]->isVisible()) (*woods)[pos]->setVisibility(false);
	//	if (!ballColided) {
	//		(*woods)[pos]->update(deltaTime);
	//		bool aux = (*woods)[pos]->getBallColidad();
	//		ballColided |= aux;
	//		if (aux) N = (*woods)[pos]->getN();
	//	}
	//}

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
	for (int i = 0; i < axes->size(); ++i) {
		if (!ballColided) {
			(*axes)[i]->update(deltaTime);
			bool aux = (*axes)[i]->getBallColided();
			ballColided |= aux;
			if (aux) N = (*axes)[i]->getN();
		}
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
	for (int i = 0; i < multiple_coins->size(); ++i) {
		(*multiple_coins)[i]->render();
	}
	for (int i = 0; i < diamonds->size(); ++i) {
		(*diamonds)[i]->render();
	}
	for (int i = 0; i < alarms->size(); ++i) {
		alarms->render();
		}
	*/
	for (int i = 0; i < axes->size(); ++i) {
		(*axes)[i]->render();
	}
}

bool Entities::ballHasColided()
{
	return ballColided;
}

glm::vec2 Entities::getN()
{
	return N;
}


