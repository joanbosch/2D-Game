#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Alarm.h"
#include "Game.h"

#define ESCALAT 2.f

enum AlarmAnims
{
	VISIBLE, INVISIBLE, ALARMON
};


void Alarm::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	visible = true;
	ballColided = false;
	spritesheet.loadFromFile("images/alarm.png", TEXTURE_PIXEL_FORMAT_RGBA);

	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(0.5, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(ALARMON, 4);
	sprite->addKeyframe(ALARMON, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(ALARMON, glm::vec2(1.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Alarm::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPlayer, glm::vec2(32 * ESCALAT, 16 * ESCALAT));
	// activar animación alarmon
	if (ballColided) {
		sprite->changeAnimation(1);
		// hacer aparecer policia
	}
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Alarm::render()
{
	if (visible) sprite->render();
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

bool Alarm::getBallColidad()
{
	return ballColided;
}

glm::vec2 Alarm::getN()
{
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), map->getBallAngle(), posPlayer, glm::vec2(32 * ESCALAT, 16 * ESCALAT));
}



