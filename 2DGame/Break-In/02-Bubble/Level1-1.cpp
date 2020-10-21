#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Block.h"
#include "Game.h"
#include "Level1-1.h"

#define NUM_BLOCKS 20
#define SCREEN_X 32
#define SCREEN_Y 42

void Level11::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, TileMap* map)
{
	bJumping = false;
	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks.push_back(new Block());
		blocks[i]->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram);
		blocks[i]->setPosition(glm::vec2(i * map->getTileSize(), i * map->getTileSize()));
		blocks[i]->setTileMap(map);
	}
	tileMapDispl = tileMapPos;

}

void Level11::update(int deltaTime)

{
	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks[i]->update(deltaTime);
	}
}

void Level11::render()
{
	for (int i = 0; i < NUM_BLOCKS; ++i) {
		blocks[i]->render();
	}
}




