#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Police.h"
#include "Game.h"

#define ESCALAT 2.f

#define VEL 4

enum PoliceAnims
{
	LOOK_FRONT, LOOK_RIGHT, LOOK_LEFT, DEAD, SEARCH
};


void Police::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap, int r)
{
	policeSize = glm::vec2(32 * ESCALAT, 16 * ESCALAT);
	spritesheet.loadFromFile("images/police.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(0.33, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(LOOK_FRONT, 8);
	sprite->addKeyframe(LOOK_FRONT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(LOOK_RIGHT, 8);
	sprite->addKeyframe(LOOK_RIGHT, glm::vec2(0.33f, 0.f));

	sprite->setAnimationSpeed(LOOK_LEFT, 8);
	sprite->addKeyframe(LOOK_LEFT, glm::vec2(0.66f, 0.f));

	sprite->setAnimationSpeed(SEARCH, 8);
	sprite->addKeyframe(SEARCH, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.33f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(SEARCH, glm::vec2(0.66f, 0.f));

	sprite->setAnimationSpeed(DEAD, 3);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.33f, 0.5f));
	sprite->addKeyframe(DEAD, glm::vec2(0.33f, 0.5f));

	sprite->changeAnimation(LOOK_FRONT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));

	map = tileMap;
	room = r;
	policeState = NULL;
	vel = 0;
}

void Police::update(int deltaTime)
{
	int minx = map->getPlayableArea().minx;
	int miny = map->getPlayableArea().miny;
	int maxx = map->getPlayableArea().maxx;
	int maxy = map->getPlayableArea().maxy;

	isOnScreen = (room == map->getActualRoom());

	if (isOnScreen) {
		// search player and move to their position
		vel = VEL;
		
	}
	else {
		vel = 0;
	}

	/*glm::vec2 posBar = posPolice;
	posBar.y += 28;

	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posBar, glm::ivec2(64 * ESCALAT, 32 * ESCALAT));*/

	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
}

void Police::render()
{
	sprite->render();
}

void Police::setPosition(const glm::vec2& pos)
{
	posPolice = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x), float(tileMapDispl.y + posPolice.y)));
}

void Police::setSearching() {
	if (sprite->animation() != SEARCH) sprite->changeAnimation(SEARCH);
}

void Police::setVelocity(float v) {
	vel = v;
}
