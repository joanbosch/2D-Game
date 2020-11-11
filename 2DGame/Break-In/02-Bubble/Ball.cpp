#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define PLAYER_VEL 6

#define ESCALAT 2.f

enum PlayerAnims
{
	NORMAL, STAR_MODE
};


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, Audio *audio)
{
	starMode = false;
	vel = 4;
	angle = 90.f;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16 * ESCALAT, 16 * ESCALAT), glm::vec2(0.125, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 1);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f, 0.f));

	sprite->setAnimationSpeed(STAR_MODE, 21);
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 1.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 2.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 3.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 4.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 5.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 6.f, 0.f));
	sprite->addKeyframe(STAR_MODE, glm::vec2(0.125f * 7.f, 0.f));

	sprite->changeAnimation(NORMAL);
	visible = true;
	gameStarted = false;
	audioManager = audio;

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

	if(starMode && (sprite->animation() != STAR_MODE)) sprite->changeAnimation(STAR_MODE);
	else if(!starMode && (sprite->animation() == STAR_MODE)) sprite->changeAnimation(NORMAL);

	if (gameStarted) {
		float rel = cos(3.14159f * angle / 180.f) / sin(3.14159f * angle / 180.f);
		float x1 = vel * cos(3.14159f * angle / 180.f);
		float y1 = vel * sin(3.14159f * angle / 180.f);

		x_pos += x1;
		y_pos -= y1;

		posPlayer.x = int(x_pos);
		posPlayer.y = int(y_pos);
		if (map->ballCollision(posPlayer, glm::ivec2(16 * ESCALAT, 16 * ESCALAT))) {
			audioManager->play(BOUNCE_SOUND, false);
			treatCollision(map->getNormalVector(posPlayer, glm::ivec2(16 * ESCALAT, 16 * ESCALAT), angle));
			x_pos -= x1;
			y_pos += y1;
			posPlayer.x = int(x_pos);
			posPlayer.y = int(y_pos);
		}
	}
	else {

		int minx = map->getPlayableArea().minx;
		int miny = map->getPlayableArea().miny;
		int maxx = map->getPlayableArea().maxx;
		int maxy = map->getPlayableArea().maxy;
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
			gameStarted = true;
		}
		if (Game::instance().getSpecialKey(GLUT_KEY_DOWN))
		{
			posPlayer.y += PLAYER_VEL;
			if (posPlayer.y > maxy)
			{
				posPlayer.y -= PLAYER_VEL;
			}
		}
		x_pos = float(posPlayer.x);
		y_pos = float(posPlayer.y);
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	
}

void Ball::render()
{
	if (visible) sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posPlayer = pos;
	x_pos = float(posPlayer.x);
	y_pos = float(posPlayer.y);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Ball::getPosition()
{
	return posPlayer;
}

void Ball::treatCollision(glm::vec2 N)
{
	glm::vec2 v = glm::vec2(cos(3.14159f * angle / 180.f), sin(3.14159f * angle / 180.f));
	glm::vec2 initial = glm::normalize(glm::vec2(1.f, 0.f));
	float dn = 2.f * glm::dot(v, glm::normalize(N));
	glm::vec2 ref = v - N * dn;


	float ang_mid = atan2((initial.x * ref.y) - (ref.x * initial.y), (initial.x * ref.x) + (initial.y * ref.y));
	if (ang_mid < 0) angle = 360 + ((ang_mid) * 180.f / 3.14159f);
	else angle = (ang_mid) * 180.f / 3.14159f;
}

void Ball::setNewDirection(glm::vec2 ref)
{
	glm::vec2 initial = glm::normalize(glm::vec2(1.f, 0.f));

	float ang_mid = atan2((initial.x * ref.y) - (ref.x * initial.y), (initial.x * ref.x) + (initial.y * ref.y));
	if (ang_mid < 0) angle = 360 + ((ang_mid) * 180.f / 3.14159f);
	else angle = (ang_mid) * 180.f / 3.14159f;
}

void Ball::setVisibility(bool vis)
{
	visible = vis;
}

float Ball::getVelocity()
{
	return vel;
}

float Ball::getAngle()
{
	return angle;
}

void Ball::setVelocity(float v)
{
	vel = v;
}

void Ball::setGameStarted(bool s)
{
	this->gameStarted = s;
}

void Ball::setStarMode(bool s)
{
	starMode = s;
}



