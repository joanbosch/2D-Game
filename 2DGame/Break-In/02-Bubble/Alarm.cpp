#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Alarm.h"
#include "Game.h"

#define ESCALAT 2.f

enum AlarmAnims
{
	VISIBLE, ALARMON
};


void Alarm::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	activated = false;
	ballColided = false;
	spritesheet.loadFromFile("images/alarm.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(ALARMON, 16);
	sprite->addKeyframe(ALARMON, glm::vec2(0.0f, 0.f));
	sprite->addKeyframe(ALARMON, glm::vec2(0.5f, 0.f));

	sprite->changeAnimation(VISIBLE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Alarm::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPlayer, glm::vec2(32 * ESCALAT, 32 * ESCALAT));
	if (ballColided) {
		activated = true;
		if (sprite->animation() != ALARMON)
			sprite->changeAnimation(ALARMON);
	}
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Alarm::render()
{
	sprite->render();
}

void Alarm::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Alarm::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Alarm::getBallColided()
{
	if (activated) return false;
	else return ballColided;
}

glm::vec2 Alarm::getN()
{
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), map->getBallAngle(), posPlayer, glm::vec2(32 * ESCALAT, 16 * ESCALAT));
}

bool Alarm::hasBeenActivated() {
	if (activated) return false;
	else return true;
}



