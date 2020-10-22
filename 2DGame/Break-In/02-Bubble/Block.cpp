#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Block.h"
#include "Game.h"


enum PlayerAnims
{
	VISIBLE, INVISIBLE
};


void Block::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	visible = true;
	ballColided = false;
	spritesheet.loadFromFile("images/Orange_Block.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 16), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Block::update(int deltaTime)
{
	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(32, 16));
	ballColided &= visible;
	visible = visible && !ballColided;
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
	return sprite->computeNormalVector(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(16, 16));
}



