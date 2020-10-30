#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Axe.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ESCALAT 2.f

enum PlayerAnims
{
	VISIBLE, INVISIBLE
};


void Axe::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	ballColided = false;
	visible = true;
	spritesheet.loadFromFile("images/Axe.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Axe::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(32, 32));
	ballColided &= visible;
	visible = visible && !ballColided;
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Axe::render()
{
	if(visible) sprite->render();
}

void Axe::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Axe::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Axe::getBallColided()
{
	return ballColided;
}

glm::vec2 Axe::getN()
{
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(16, 16));
}

bool Axe::isVisible()
{
	return visible;
}



