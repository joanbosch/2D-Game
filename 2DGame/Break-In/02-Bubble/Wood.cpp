#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Wood.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	VISIBLE, INVISIBLE
};


void Wood::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	visible = true;
	bJumping = false;
	spritesheet.loadFromFile("images/Wood.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(VISIBLE, 8);
	sprite->addKeyframe(VISIBLE, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Wood::update(int deltaTime)
{
	bool colide = sprite->ballCollision(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(16, 16));
	ballColided = !colide;
	if (colide) {
		glm::vec2 N = sprite->computeNormalVector(map->getBallPos(), glm::vec2(16, 16), posPlayer, glm::vec2(16, 16));
	}
		
	sprite->update(deltaTime);	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Wood::render()
{
	if(visible) sprite->render();
}

void Wood::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Wood::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Wood::getBallColidad()
{
	return ballColided;
}




