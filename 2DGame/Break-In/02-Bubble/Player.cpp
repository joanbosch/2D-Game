#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define ESCALAT 2.f

enum PlayerAnims
{
	LOOK_FRONT, LOOK_RIGHT, LOOK_LEFT, LOOK_TOP, LOOK_BOTTOM, LOOK_TOPRIGHT, LOOK_TOPLEFT, LOOK_BOTRIGHT, LOOK_BOTLEFT, DEAD
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap)
{
	playerSize = glm::ivec2(38 * ESCALAT, 64 * ESCALAT);
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64 * ESCALAT, 64 * ESCALAT), glm::vec2(0.2, 0.33), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(10);

	sprite->setAnimationSpeed(LOOK_FRONT, 8);
	sprite->addKeyframe(LOOK_FRONT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(LOOK_RIGHT, 8);
	sprite->addKeyframe(LOOK_RIGHT, glm::vec2(0.f, 0.33f));

	sprite->setAnimationSpeed(LOOK_LEFT, 8);
	sprite->addKeyframe(LOOK_LEFT, glm::vec2(0.2f, 0.33f));

	sprite->setAnimationSpeed(LOOK_TOP, 8);
	sprite->addKeyframe(LOOK_TOP, glm::vec2(0.4f, 0.33f));

	sprite->setAnimationSpeed(LOOK_BOTTOM, 8);
	sprite->addKeyframe(LOOK_BOTTOM, glm::vec2(0.6f, 0.33f));

	sprite->setAnimationSpeed(LOOK_TOPRIGHT, 8);
	sprite->addKeyframe(LOOK_TOPRIGHT, glm::vec2(0.8f, 0.33f));

	sprite->setAnimationSpeed(LOOK_TOPLEFT, 8);
	sprite->addKeyframe(LOOK_TOPLEFT, glm::vec2(0.f, 0.66f));

	sprite->setAnimationSpeed(LOOK_BOTRIGHT, 8);
	sprite->addKeyframe(LOOK_BOTRIGHT, glm::vec2(0.2f, 0.66f));

	sprite->setAnimationSpeed(LOOK_BOTLEFT, 8);
	sprite->addKeyframe(LOOK_BOTLEFT, glm::vec2(0.4f, 0.66f));

	sprite->setAnimationSpeed(DEAD, 4);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.8f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	map = tileMap;
	visible = true;
	ballColided = false;
}

void Player::update(int deltaTime)
{
	int minx = map->getPlayableArea().minx;
	int miny = map->getPlayableArea().miny;
	int maxx = map->getPlayableArea().maxx;
	int maxy = map->getPlayableArea().maxy;

	glm::vec2 posBar = posPlayer;
	posBar.y += 28;

	ballColided = sprite->ballCollision(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posBar, glm::ivec2(38 * ESCALAT, 10 * ESCALAT));
	ballColided &= visible;

	sprite->update(deltaTime);
	if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		/*if(sprite->animation() != DEAD)
			sprite->changeAnimation(DEAD);*/
		posPlayer.x -= 4;
		if (posPlayer.x < minx)
		{
			posPlayer.x += 4;
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		/*if(sprite->animation() != LOOK_RIGHT)
			sprite->changeAnimation(LOOK_RIGHT);*/
		posPlayer.x += 4;
		if (posPlayer.x > maxx)
		{
			posPlayer.x -= 4;
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		/*if (sprite->animation() != LOOK_TOP)
			sprite->changeAnimation(LOOK_TOP);*/
		posPlayer.y -= 4;
		if (posPlayer.y < miny)
		{
			posPlayer.y += 4;
		}
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
	{
		/*if (sprite->animation() != LOOK_BOTTOM)
			sprite->changeAnimation(LOOK_BOTTOM);*/
		posPlayer.y += 4;
		if (posPlayer.y > maxy)
		{
			posPlayer.y -= 4;
		}
	}

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	if (visible) sprite->render();
}

void Player::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

bool Player::getBallColided()
{
	return ballColided;
}

glm::vec2 Player::getN()
{
	glm::vec2 posBar = posPlayer;
	posBar.y += 28;
	return computeNormalVector(map->getBallPos(), glm::vec2(16 * ESCALAT, 16 * ESCALAT), posBar, glm::ivec2(38 * ESCALAT, 10 * ESCALAT));
}


glm::vec2 Player::computeNormalVector(glm::vec2 ballPos, glm::vec2 ballSize, glm::vec2 spritePosition, glm::vec2 spriteSize)
{
	int minx_ball, miny_ball, maxx_ball, maxy_ball;
	int minx_block, miny_block, maxx_block, maxy_block;

	minx_ball = ballPos.x;
	miny_ball = ballPos.y;
	maxx_ball = ballPos.x + ballSize.x;
	maxy_ball = ballPos.y + ballSize.y;

	int midx_ball = (maxx_ball - minx_ball) / 2 + minx_ball;

	minx_block = spritePosition.x;
	miny_block = spritePosition.y;
	maxx_block = spritePosition.x + spriteSize.x;
	maxy_block = spritePosition.y + spriteSize.y;

	int midx_block = (maxx_block - minx_block) / 2 + minx_block;

	int dist;
	float alpha;

	if (minx_block <= maxx_ball && maxx_ball < maxx_block)
	{
		if (miny_block <= miny_ball && miny_ball < maxy_block)
		{
			if (glm::abs((glm::min(maxy_ball, maxy_block)) - miny_ball) > glm::abs(glm::max(minx_block, minx_ball) - maxx_ball))
				return glm::vec2(-1, 0);
			else
				// NO POT ARRIBAR MAI AQUI
				return glm::vec2(0, -1);
		}
		else
		{
			if (glm::abs(maxx_ball - glm::max(minx_ball, minx_block)) > glm::abs(maxy_ball - miny_block))
				// TODO
			{
				dist = midx_ball - minx_block;
			    alpha = (dist * 3.141592f) / 72.f + 3.141592f / 2.f;
				return glm::vec2(- glm::sin(alpha), 1);
			}
			else
				return glm::vec2(-1, 0);
		}
	}
	else
	{
		if (miny_block <= miny_ball && miny_ball <= maxy_block)
		{
			if (glm::abs(maxy_block - miny_ball) > glm::abs(glm::min(maxx_block, maxx_ball) - minx_ball))
				return glm::vec2(1, 0);
			else
				// NO POT ARRIBAR MAI AQUI
				return glm::vec2(0, -1);
		}
		else
		{
			if (glm::abs(maxx_block - glm::max(minx_block, minx_ball)) > glm::abs(glm::min(maxy_block, maxy_ball) - max(miny_block, miny_ball)))
			{
				// TODO
				dist = minx_ball - minx_block;
			    alpha = (dist * 3.141592f) / 72.f + 3.141592f / 2.f;
				return glm::vec2(- glm::sin(alpha), 1);
			}
			else
				return glm::vec2(1, 0);
		}
	}
}
void Player::setVisibility(bool vis)
{
	visible = vis;
}
