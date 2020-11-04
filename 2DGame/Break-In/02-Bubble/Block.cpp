#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Block.h"
#include "Game.h"

#define ESCALAT 2.f

enum PlayerAnims
{
	VISIBLE, INVISIBLE
};


void Block::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	visible = true;
	ballColided = false;
	level = 2;
	switch (level)
	{
	case 0:
		spritesheet.loadFromFile("images/Orange_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case 1:
		spritesheet.loadFromFile("images/Green_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	case 2:
		spritesheet.loadFromFile("images/Blue_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
		break;
	}
	sprite = Sprite::createSprite(glm::ivec2(32 * ESCALAT, 16 * ESCALAT), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Block::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPlayer, glm::vec2(32 * ESCALAT, 16 * ESCALAT));
	ballColided &= visible;
	if (ballColided) {
		--level;
		switch (level)
		{
		case 0:
			spritesheet.loadFromFile("images/Orange_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 1:
			spritesheet.loadFromFile("images/Green_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		case 2:
			spritesheet.loadFromFile("images/Blue_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
			break;
		}
		if (level < 0) {
			visible = false;
		}
	}
	sprite->update(deltaTime);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Block::render()
{
	if(visible) sprite->render();
}

void Block::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Block::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Block::getBallColidad()
{
	return ballColided;
}

glm::vec2 Block::getN()
{
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posPlayer, glm::vec2(32 * ESCALAT, 16 * ESCALAT));
}



