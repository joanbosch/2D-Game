#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"


Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
	shaderProgram->setUniformMatrix4f("modelview", modelview);
	shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
	glEnable(GL_TEXTURE_2D);
	texture->use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_TEXTURE_2D);
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

bool Sprite::ballCollision(glm::vec2 ballPos, glm::vec2 ballSize, glm::vec2 spritePosition, glm::vec2 spriteSize)
{
	int minx_ball, miny_ball, maxx_ball, maxy_ball;
	int minx_block, miny_block, maxx_block, maxy_block;

	minx_ball = ballPos.x;
	miny_ball = ballPos.y;
	maxx_ball = ballPos.x + ballSize.x;
	maxy_ball = ballPos.y + ballSize.y;

	minx_block = spritePosition.x;
	miny_block = spritePosition.y;
	maxx_block = spritePosition.x + spriteSize.x;
	maxy_block = spritePosition.y + spriteSize.y;

	return (minx_ball < maxx_block) && (minx_block < maxx_ball) && (miny_ball < maxy_block) && (miny_block < maxy_ball);
}

glm::vec2 Sprite::computeNormalVector(glm::vec2 ballPos, glm::vec2 ballSize, float ballAngle, glm::vec2 spritePosition, glm::vec2 spriteSize)
{
	int minx_ball, miny_ball, maxx_ball, maxy_ball;
	int minx_block, miny_block, maxx_block, maxy_block;

	minx_ball = ballPos.x;
	miny_ball = ballPos.y;
	maxx_ball = ballPos.x + ballSize.x;
	maxy_ball = ballPos.y + ballSize.y;

	minx_block = spritePosition.x;
	miny_block = spritePosition.y;
	maxx_block = spritePosition.x + spriteSize.x;
	maxy_block = spritePosition.y + spriteSize.y;

	if (minx_block <= maxx_ball && maxx_ball < maxx_block) 
	{
		if (miny_block <= miny_ball && miny_ball < maxy_block)
		{
			if (glm::abs((glm::min(maxy_ball, maxy_block)) - miny_ball) > glm::abs(glm::max(minx_block, minx_ball) - maxx_ball))
				if(ballAngle < 90) return glm::vec2(-1, -1);
				else if(ballAngle <= 180) return glm::vec2(0, -1);
				else return glm::vec2(-1, 0);
				
			else 
				return glm::vec2(0, -1);
		}
		else
		{
			if (glm::abs(maxx_ball - glm::max(minx_ball, minx_block)) > glm::abs(maxy_ball - miny_block))
				//return glm::vec2(0, 1);
				if (ballAngle <= 90) return glm::vec2(-1, 0);
				else /*if (ballAngle <= 240)*/ return glm::vec2(0, 1);
				//else return glm::vec2(-1, 1);
			else
				return glm::vec2(-1, 0); 
		}
	}
	else
	{
		if (miny_block <= miny_ball && miny_ball <= maxy_block)
		{
			if (glm::abs(maxy_block - miny_ball) > glm::abs(glm::min(maxx_block, maxx_ball) - minx_ball))
				if (ballAngle < 90) return glm::vec2(0, -1);
				else if (ballAngle <= 180) return glm::vec2(1, -1);
				else return glm::vec2(1, 0);
			else
				return glm::vec2(0, -1); 
		}
		else 
		{
			if (glm::abs(maxx_block - glm::max(minx_block, minx_ball)) > glm::abs(glm::min(maxy_block, maxy_ball) - max(miny_block, miny_ball)))
				//return glm::vec2(0, 1);
				if (ballAngle < 180) return glm::vec2(1, 0);
				/*else if (ballAngle <= 240) return glm::vec2(1, 1);*/
				else return glm::vec2(0, 1);
			else
				return glm::vec2(1, 0);
		}
	}
}



