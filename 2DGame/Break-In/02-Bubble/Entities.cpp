#include <cmath>
#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "Entities.h"

using namespace std;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#define SCREEN_X 32
#define SCREEN_Y 42

#define BLOCK_POINTS 100;
#define COIN_MONEY 100;
#define BAG_MONEY 200;
#define SOME_COINS_MONEY 400;
#define DIAMON_MONEY 1500;

#define TIME_STAR 8000;

void Entities::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, Audio* audio)
{
	ballColided = false;
	playerColided = false;

	map = tileMap;
	sP = shaderProgram;
	audioManager = audio;

	int numEntities = map->getNEntities();

	woods = new vector<Wood*>();
	blocks = new vector<Block*>();
	single_coins = new vector<Coin*>();
	bags = new vector<Bag*>();
	multiple_coins = new vector<MultipleCoins*>();
	diamonds = new vector<Diamond*>();
	alarms = new vector<Alarm*>();
	polices = new vector<Police*>();
	axes = new vector<Axe*>();
	stars = new vector<Star*>();
	moneyEntities = 0;
	starMode = false;
	starInitTime = 0;

	for (int i = 0; i < numEntities; ++i) {
		int entityType = map->getEntity(i).type;
		glm::ivec2 tilemap = tileMapPos;

		if (entityType == WOOD) {
			Wood* aux = new Wood();
			aux->init(tilemap, sP);
			aux->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			aux->setTileMap(map);
			woods->push_back(aux);
		}
		else if (entityType == ORANGE_BLOCK) {
			Block* or_block = new Block();
			// TODO: init block's resistance
			or_block->init(tilemap, sP, 0);
			or_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			or_block->setTileMap(map);
			blocks->push_back(or_block);
		}
		else if (entityType == GREEN_BLOCK) {
			Block* gr_block = new Block();
			// TODO: init block's resistance
			gr_block->init(tilemap, sP, 1);
			gr_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			gr_block->setTileMap(map);
			blocks->push_back(gr_block);
		} 
		else if (entityType == BLUE_BLOCK) {
			Block* bl_block = new Block();
			// TODO: init block's resistance
			bl_block->init(tilemap, sP, 2);
			bl_block->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			bl_block->setTileMap(map);
			blocks->push_back(bl_block);
		}
		else if (entityType == SINGLE_COIN) {
			Coin* coin = new Coin();
			coin->init(tilemap, sP);
			coin->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			coin->setTileMap(map);
			single_coins->push_back(coin);
			moneyEntities++;
		}
		else if (entityType == COINS_BAG) {
			Bag* bag = new Bag();
			bag->init(tilemap, sP);
			bag->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			bag->setTileMap(map);
			bags->push_back(bag);
			moneyEntities++;
		}
		else if (entityType == MULTIPLE_COINS) {
			MultipleCoins *coins = new MultipleCoins();
			coins->init(tilemap, sP);
			coins->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			coins->setTileMap(map);
			multiple_coins->push_back(coins);
			moneyEntities++;
		}
		else if (entityType == DIAMOND) {
			Diamond *diam = new Diamond();
			diam->init(tilemap, sP);
			diam->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			diam->setTileMap(map);
			diamonds->push_back(diam);
			moneyEntities++;
		}
		else if (entityType == ALARM) {
			Alarm* alarm = new Alarm();
			alarm->init(tilemap, sP);
			alarm->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			alarm->setTileMap(map);
			tileMapDispl = tileMapPos;
			alarms->push_back(alarm);
		}
		else if (entityType == AXE) {
			Axe* axe = new Axe();
			axe->init(tilemap, sP);
			axe->setPosition(glm::vec2(map->getEntity(i).x *map->getTileSize(), map->getEntity(i).y *map->getTileSize()));
			axe->setTileMap(map);
			tileMapDispl = tileMapPos;
			axes->push_back(axe);
		}
		else if (entityType == STAR) {
			Star* star = new Star();
			star->init(tilemap, sP);
			star->setPosition(glm::vec2(map->getEntity(i).x * map->getTileSize(), map->getEntity(i).y * map->getTileSize()));
			star->setTileMap(map);
			tileMapDispl = tileMapPos;
			stars->push_back(star);
		}
	}

}

void Entities::update(int deltaTime)
{
	ballColided = false;
	coins = 0;
	points = 0;

	if (starMode) {
		if (starInitTime > 8000) {
			starMode = false;
			starInitTime = 0;
		}
		starInitTime += deltaTime;
	}

	for (int i = 0; i < blocks->size(); ++i) {
		if (!ballColided) {
			(*blocks)[i]->update(deltaTime, starMode);
			bool aux = (*blocks)[i]->getBallColidad();
			ballColided |= (aux && !starMode);
			if (aux) {
				audioManager->play(POINTS_SOUND, false);
				if (!(*blocks)[i]->isVisible()) {
					points += BLOCK_POINTS;
				}
				N = (*blocks)[i]->getN();
			}
		}
		
	}

	// AXES -> 2 habitacions nomes REPENSAR
	int level = 3 - map->getActualLevel();
	int room = 3 - map->getActualRoom();
	// update woods panels
	int num_woods = 2;
	switch (level)
	{
	case 0:		// actually level 3
		num_woods = 4;
		break;
	case 1:		// actually level 2
		num_woods = 6;
		break;
	case 2:		// actually level 1
		num_woods = 8;
		break;
	}

	if (room > 0 && room <3){
		for (int i = 0; i < num_woods; ++i) {
			int pos = (room-1) * num_woods + i;
			if (!(*axes)[room - 1]->isVisible()) {
				(*woods)[pos]->setVisibility(false);
			} 
			if (!ballColided && (*woods)[pos]->isVisible()) {
				(*woods)[pos]->update(deltaTime);
				bool aux = (*woods)[pos]->getBallColidad();
				ballColided |= aux;
				if (aux) {
					audioManager->play(BOUNCE_SOUND, false);
					N = (*woods)[pos]->getN();
				} 
			}
		}
	}

	for (int i = 0; i < single_coins->size(); ++i) {
		if (!ballColided) {
			(*single_coins)[i]->update(deltaTime);
			bool aux = (*single_coins)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				if (!(*single_coins)[i]->isVisible()) {
					audioManager->play(COIN_SOUND, false);
					coins += COIN_MONEY;
					moneyEntities--;
				}
				N = (*single_coins)[i]->getN();
			}
		}
	}
	for (int i = 0; i < bags->size(); ++i) {
		if (!ballColided) {
			(*bags)[i]->update(deltaTime);
			bool aux = (*bags)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				if (!(*bags)[i]->isVisible()) {
					audioManager->play(BAG_SOUND, false);
					coins += BAG_MONEY;
					moneyEntities--;
				}
				N = (*bags)[i]->getN();
			}
		}
	}
	for (int i = 0; i < multiple_coins->size(); ++i) {
		if (!ballColided) {
			(*multiple_coins)[i]->update(deltaTime);
			bool aux = (*multiple_coins)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				if (!(*multiple_coins)[i]->isVisible()) {
					audioManager->play(COIN_SOUND, false);
					audioManager->play(COIN_SOUND, false);
					coins += SOME_COINS_MONEY;
					moneyEntities--;
				}
				N = (*multiple_coins)[i]->getN();
			}
		}
	}
	for (int i = 0; i < diamonds->size(); ++i) {
		if (!ballColided) {
			(*diamonds)[i]->update(deltaTime);
			bool aux = (*diamonds)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				if (!(*diamonds)[i]->isVisible()) {
					audioManager->play(DIAMOND_SOUND, false);
					coins += DIAMON_MONEY;
					moneyEntities--;
				}
				N = (*diamonds)[i]->getN();
			}
		}
	}

	for (int i = 0; i < alarms->size(); ++i) {
		if (!ballColided) {
			bool aux_activ = (*alarms)[i]->isOn();
			(*alarms)[i]->update(deltaTime);
			// if actual alarm is off and alarm sound is playing, turn off
			bool b = audioManager->isPlaying(ALARM_SOUND);
			if (room == i) {
				if (audioManager->isPlaying(ALARM_SOUND) && !(*alarms)[i]->isOn()) audioManager->stop(ALARM_SOUND);
				else if (!audioManager->isPlaying(ALARM_SOUND) && (*alarms)[i]->isOn()) audioManager->play(ALARM_SOUND, true);
			}

			bool aux = (*alarms)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				if (aux_activ != (*alarms)[i]->isOn()) {
					if ((*alarms)[i]->isOn() && (room == i)) audioManager->play(ALARM_SOUND, true);
					Police *pol_aux = new Police();
					pol_aux->init(glm::vec2(64, 84 ), sP, map, 3-room);
					pol_aux->setPosition(glm::vec2(2.f * map->getTileSize(), map->getPlayableArea().maxy));
					pol_aux->setSearching();
					polices->push_back(pol_aux);
				}
				N = (*alarms)[i]->getN();
			}
		}
	}

	for (int i = 0; i < polices->size(); ++i) {
		(*polices)[i]->update(deltaTime, starMode);
		if ((*polices)[i]->getCollisionPlayer()) {
			(*polices)[i]->setVelocity(0);
			playerColided = true;
		}
		else {
			if (map->getScrolling()) {
				(*polices)[i]->setVelocity(0);
			}
			else {
				(*polices)[i]->setVelocity(1.5);
			}
		}
	}

	for (int i = 0; i < axes->size(); ++i) {
		if (!ballColided) {
			(*axes)[i]->update(deltaTime);
			bool aux = (*axes)[i]->getBallColided();
			ballColided |= aux;
			if (aux) N = (*axes)[i]->getN();
		}
	}

	for (int i = 0; i < stars->size(); ++i) {
		if (!ballColided) {
			(*stars)[i]->update(deltaTime);
			bool aux = (*stars)[i]->getBallColided();
			ballColided |= aux;
			if (aux) {
				audioManager->play(STAR_SOUND, false);
				N = (*stars)[i]->getN();
				starMode = true;
				audioManager->play(STAR_MODE_MUSIC, false);
			}
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
	for (int i = 0; i < multiple_coins->size(); ++i) {
		(*multiple_coins)[i]->render();
	}
	for (int i = 0; i < diamonds->size(); ++i) {
		(*diamonds)[i]->render();
	}
	for (int i = 0; i < axes->size(); ++i) {
		(*axes)[i]->render();
	}
	for (int i = 0; i < alarms->size(); ++i) {
		(*alarms)[i]->render();
	}
	for (int i = 0; i < stars->size(); ++i) {
		(*stars)[i]->render();
	}
	for (int i = 0; i < polices->size(); ++i) {
		(*polices)[i]->render();
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

int Entities::getNewCoins()
{
	return coins;
}

int Entities::getNewPoints()
{
	return points;
}

int Entities::getRemainingMoneyEntities()
{
	return moneyEntities;
}

bool Entities::isStarMode()
{
	return starMode;
}

void Entities::setPlayerDead() {
	for (int i = 0; i < polices->size(); ++i) {
		(*polices)[i]->setVelocity(0);
	}
	for (int i = 0; i < alarms->size(); ++i) {
		(*alarms)[i]->deactivate();
		audioManager->stop(ALARM_SOUND);
	}
	polices = new vector<Police*>();
}

void Entities::setPlayerColided(bool b) 
{
	playerColided = b;
}
