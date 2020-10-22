#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	bJumping = false;
	vel = 4;
	angle = 32.f;
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	int rel = cos(3.14159f * angle / 180.f) / sin(3.14159f * angle / 180.f);
	float x1 = vel * cos(3.14159f * angle / 180.f);
	float y1 = vel * sin(3.14159f * angle / 180.f);

	x_pos += x1;
	y_pos -= y1;

	posPlayer.x = int(x_pos);
	posPlayer.y = int(y_pos);
	if (map->ballCollision(posPlayer, glm::ivec2(16, 16))) {
		treatCollision(map->getNormalVector(posPlayer, glm::ivec2(16, 16), angle));
		x_pos -= x1;
		y_pos += y1;
		posPlayer.x = int(x_pos);
		posPlayer.y = int(y_pos);
	}
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Ball::render()
{
	sprite->render();
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
	glm::vec2 initial = glm::normalize(glm::vec2(1, 0));
	float dn = 2 * glm::dot(v, glm::normalize(N));
	glm::vec2 ref = v - N * dn;


	float ang_mid = atan2((initial.x * ref.y) - (ref.x * initial.y), (initial.x * ref.x) + (initial.y * ref.y));
	if (ang_mid < 0) angle = 360 + ((ang_mid) * 180.f / 3.14159f);
	else angle = (ang_mid) * 180.f / 3.14159f;
}



