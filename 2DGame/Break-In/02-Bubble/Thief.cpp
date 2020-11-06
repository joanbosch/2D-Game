#include "Thief.h"

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

#define ESCALAT 2.f

enum PlayerAnims
{
	WALK
};


void Thief::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap)
{
	playerSize = glm::ivec2(38 * ESCALAT, 64 * ESCALAT);
	spritesheet.loadFromFile("images/thief.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64 * ESCALAT, 72 * ESCALAT), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(WALK, 4);
	sprite->addKeyframe(WALK, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(WALK, glm::vec2(0.75f, 0.f));

	sprite->changeAnimation(WALK);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	map = tileMap;
}

void Thief::update(int deltaTime)
{

	sprite->update(deltaTime);
	posPlayerx += 0.4;
	posPlayer.x = int(posPlayerx);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Thief::render()
{
	sprite->render();
}

void Thief::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	posPlayerx = float(tileMapDispl.x + posPlayer.x);
}
