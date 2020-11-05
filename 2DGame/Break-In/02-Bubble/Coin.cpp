#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Coin.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ESCALAT 2.f

enum PlayerAnims
{
	VISIBLE, INVISIBLE
};


void Coin::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	visible = true;
	ballColided = false;
	spritesheet.loadFromFile("images/Coin.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 32 * ESCALAT), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Coin::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPlayer, glm::vec2(32 * ESCALAT, 32 * ESCALAT));
	ballColided &= visible;
	if(ballColided) visible = false;
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Coin::render()
{
	if(visible) sprite->render();
}

void Coin::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Coin::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Coin::getBallColided()
{
	return ballColided;
}

glm::vec2 Coin::getN()
{
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), map->getBallAngle(), posPlayer, glm::vec2(32 * ESCALAT, 32 * ESCALAT));
}

bool Coin::isVisible()
{
	return visible;
}


