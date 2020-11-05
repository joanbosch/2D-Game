#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define ESCALAT 2.f

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	vel = 4;
	angle = 60.f;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16 * ESCALAT, 16 * ESCALAT), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	visible = true;

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	float rel = cos(3.14159f * angle / 180.f) / sin(3.14159f * angle / 180.f);
	float x1 = vel * cos(3.14159f * angle / 180.f);
	float y1 = vel * sin(3.14159f * angle / 180.f);

	x_pos += x1;
	y_pos -= y1;

	posPlayer.x = int(x_pos);
	posPlayer.y = int(y_pos);
	if (map->ballCollision(posPlayer, glm::ivec2(16 * ESCALAT, 16 * ESCALAT))) {
		treatCollision(map->getNormalVector(posPlayer, glm::ivec2(16 * ESCALAT, 16 * ESCALAT), angle));
		x_pos -= x1;
		y_pos += y1;
		posPlayer.x = int(x_pos);
		posPlayer.y = int(y_pos);
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



