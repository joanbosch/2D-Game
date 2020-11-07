#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define ESCALAT 2.f

#define PLAYER_VEL 6

enum PlayerAnims
{
	LOOK_FRONT, LOOK_RIGHT, LOOK_LEFT, LOOK_TOP, LOOK_BOTTOM, LOOK_TOPRIGHT, LOOK_TOPLEFT, LOOK_BOTRIGHT, LOOK_BOTLEFT, DEAD, LOOK_RIGHT_STAR, LOOK_LEFT_STAR, LOOK_TOP_STAR, LOOK_BOTTOM_STAR, LOOK_TOPRIGHT_STAR, LOOK_TOPLEFT_STAR, LOOK_BOTRIGHT_STAR, LOOK_BOTLEFT_STAR
};


void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* tileMap)
{
	playerSize = glm::ivec2(38 * ESCALAT, 64 * ESCALAT);
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(64 * ESCALAT, 64 * ESCALAT), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(18);

	sprite->setAnimationSpeed(LOOK_FRONT, 8);
	sprite->addKeyframe(LOOK_FRONT, glm::vec2(0.f, 0.f));



	sprite->setAnimationSpeed(LOOK_RIGHT, 8);
	sprite->addKeyframe(LOOK_RIGHT, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(LOOK_LEFT, 8);
	sprite->addKeyframe(LOOK_LEFT, glm::vec2(0.2f, 0.2f));

	sprite->setAnimationSpeed(LOOK_TOP, 8);
	sprite->addKeyframe(LOOK_TOP, glm::vec2(0.4f, 0.2f));

	sprite->setAnimationSpeed(LOOK_BOTTOM, 8);
	sprite->addKeyframe(LOOK_BOTTOM, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(LOOK_TOPRIGHT, 8);
	sprite->addKeyframe(LOOK_TOPRIGHT, glm::vec2(0.8f, 0.2f));

	sprite->setAnimationSpeed(LOOK_TOPLEFT, 8);
	sprite->addKeyframe(LOOK_TOPLEFT, glm::vec2(0.f, 0.4f));

	sprite->setAnimationSpeed(LOOK_BOTRIGHT, 8);
	sprite->addKeyframe(LOOK_BOTRIGHT, glm::vec2(0.2f, 0.4f));

	sprite->setAnimationSpeed(LOOK_BOTLEFT, 8);
	sprite->addKeyframe(LOOK_BOTLEFT, glm::vec2(0.4f, 0.4f));



	sprite->setAnimationSpeed(LOOK_RIGHT_STAR, 8);
	sprite->addKeyframe(LOOK_RIGHT_STAR, glm::vec2(0.6f, 0.4f));

	sprite->setAnimationSpeed(LOOK_LEFT_STAR, 8);
	sprite->addKeyframe(LOOK_LEFT_STAR, glm::vec2(0.8f, 0.4f));

	sprite->setAnimationSpeed(LOOK_TOP_STAR, 8);
	sprite->addKeyframe(LOOK_TOP_STAR, glm::vec2(0.f, 0.6f));

	sprite->setAnimationSpeed(LOOK_BOTTOM_STAR, 8);
	sprite->addKeyframe(LOOK_BOTTOM_STAR, glm::vec2(0.2f, 0.6f));

	sprite->setAnimationSpeed(LOOK_TOPRIGHT_STAR, 8);
	sprite->addKeyframe(LOOK_TOPRIGHT_STAR, glm::vec2(0.4f, 0.6f));

	sprite->setAnimationSpeed(LOOK_TOPLEFT_STAR, 8);
	sprite->addKeyframe(LOOK_TOPLEFT_STAR, glm::vec2(0.6f, 0.6f));

	sprite->setAnimationSpeed(LOOK_BOTRIGHT_STAR, 8);
	sprite->addKeyframe(LOOK_BOTRIGHT_STAR, glm::vec2(0.8f, 0.6f));

	sprite->setAnimationSpeed(LOOK_BOTLEFT_STAR, 8);
	sprite->addKeyframe(LOOK_BOTLEFT_STAR, glm::vec2(0.f, 0.8f));



	sprite->setAnimationSpeed(DEAD, 4);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.8f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.8f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	map = tileMap;
	visible = true;
	ballColided = false;
	newBallVelocity = 4.f;
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
	if (!isDead) {
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= PLAYER_VEL;
			if (posPlayer.x < minx)
			{
				posPlayer.x += PLAYER_VEL;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
		{
			posPlayer.x += PLAYER_VEL;
			if (posPlayer.x > maxx)
			{
				posPlayer.x -= PLAYER_VEL;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_UP))
		{
			posPlayer.y -= PLAYER_VEL;
			if (posPlayer.y < miny)
			{
				posPlayer.y += PLAYER_VEL;
			}
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			posPlayer.y += PLAYER_VEL;
			if (posPlayer.y > maxy)
			{
				posPlayer.y -= PLAYER_VEL;
			}
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

float Player::getNewBallVelocity()
{
	return newBallVelocity;
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

float Player::newVel(int dist)
{
	if (dist <= 14 || dist >= 58) return 8.f;
	else if (dist <= 30 || dist >= 42) return 6.f;
	else return 4.f;
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
				return glm::vec2(0, 0);
			else
				return glm::vec2(0, -1);
		}
		else
		{
			if (glm::abs(maxx_ball - glm::max(minx_ball, minx_block)) > glm::abs(maxy_ball - miny_block))
			{
				dist = midx_ball - minx_block;
				newBallVelocity = newVel(dist);

			    alpha = (dist * 3.141592f) / 72.f + 3.141592f / 2.f;
				return glm::vec2(- glm::sin(alpha), 1);
			}
			else
				return glm::vec2(0, 0);
		}
	}
	else
	{
		if (miny_block <= miny_ball && miny_ball <= maxy_block)
		{
			if (glm::abs(maxy_block - miny_ball) > glm::abs(glm::min(maxx_block, maxx_ball) - minx_ball))
				return glm::vec2(0, 0);
			else
				return glm::vec2(0, -1);
		}
		else
		{
			if (glm::abs(maxx_block - glm::max(minx_block, minx_ball)) > glm::abs(glm::min(maxy_block, maxy_ball) - max(miny_block, miny_ball)))
			{
				dist = minx_ball - minx_block;
			    alpha = (dist * 3.141592f) / 72.f + 3.141592f / 2.f;
				return glm::vec2(- glm::sin(alpha), 1);
			}
			else
				return glm::vec2(0, 0);
		}
	}
}
float Player::computeAngle(glm::vec2 ref)
{
	glm::normalize(ref);
	glm::vec2 initial = glm::normalize(glm::vec2(1.f, 0.f));

	float ang_mid = atan2((initial.x * ref.y) - (ref.x * initial.y), (initial.x * ref.x) + (initial.y * ref.y));
	if (ang_mid < 0) return 360 + ((ang_mid) * 180.f / 3.14159f);
	else return (ang_mid) * 180.f / 3.14159f;
}
void Player::updateAnimation(glm::vec2 ballPos, bool starMode)
{
	glm::vec2 v = glm::vec2(ballPos.x - posPlayer.x, posPlayer.y - ballPos.y);
	float angle = computeAngle(v);
	if (!starMode) {
		if (angle > 337.5f && angle <= 22.5f) sprite->changeAnimation(LOOK_RIGHT);
		else if (angle > 22.5f && angle <= 67.5f) sprite->changeAnimation(LOOK_TOPRIGHT);
		else if (angle > 67.5f && angle <= 112.5f) sprite->changeAnimation(LOOK_TOP);
		else if (angle > 112.5f && angle <= 157.5) sprite->changeAnimation(LOOK_TOPLEFT);
		else if (angle > 157.5 && angle <= 202.5f) sprite->changeAnimation(LOOK_LEFT);
		else if (angle > 202.5f && angle <= 247.5f) sprite->changeAnimation(LOOK_BOTLEFT);
		else if (angle > 247.5f && angle <= 292.5f) sprite->changeAnimation(LOOK_BOTTOM);
		else if (angle > 292.5f && angle <= 337.5f) sprite->changeAnimation(LOOK_BOTRIGHT);
	}
	else {
		if (angle > 337.5f && angle <= 22.5f) sprite->changeAnimation(LOOK_RIGHT_STAR);
		else if (angle > 22.5f && angle <= 67.5f) sprite->changeAnimation(LOOK_TOPRIGHT_STAR);
		else if (angle > 67.5f && angle <= 112.5f) sprite->changeAnimation(LOOK_TOP_STAR);
		else if (angle > 112.5f && angle <= 157.5) sprite->changeAnimation(LOOK_TOPLEFT_STAR);
		else if (angle > 157.5 && angle <= 202.5f) sprite->changeAnimation(LOOK_LEFT_STAR);
		else if (angle > 202.5f && angle <= 247.5f) sprite->changeAnimation(LOOK_BOTLEFT_STAR);
		else if (angle > 247.5f && angle <= 292.5f) sprite->changeAnimation(LOOK_BOTTOM_STAR);
		else if (angle > 292.5f && angle <= 337.5f) sprite->changeAnimation(LOOK_BOTRIGHT_STAR);
	}

}
void Player::setVisibility(bool vis)
{
	visible = vis;
}

void Player::setDead(bool b)
{
	isDead = b;
	if (b) {
		if (sprite->animation() != DEAD)
			sprite->changeAnimation(DEAD);
	}
	else {
		if (sprite->animation() != LOOK_FRONT)
			sprite->changeAnimation(LOOK_FRONT);
	}
	
}
