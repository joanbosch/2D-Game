#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "ShaderProgram.h"


// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

enum ENTITIES_TYPES {
	WOOD, ORANGE_BLOCK, GREEN_BLOCK, BLUE_BLOCK, SINGLE_COIN, COINS_BAG, MULTIPLE_COINS, DIAMOND, ALARM, AXE
};

struct infoEntities {
	int x;
	int y;
	ENTITIES_TYPES type;
};

struct playableArea {
	int minx;
	int miny;
	int maxx;
	int maxy;
};

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	playableArea getPlayableArea() { return playerArea; };
	void setPlayableArea(int minxcoord, int minycoord, int maxxcoord, int maxycoord);

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	
	bool ballCollision(const glm::ivec2& pos, const glm::ivec2& size) const;
	vector<bool> reviseCollisions(const glm::ivec2& pos, const glm::ivec2& size) const;
	glm::vec2 getNormalVector(const glm::ivec2& pos, const glm::ivec2& size, float angle) const;

	void setBallPos(glm::vec2 pos);
	glm::vec2 getBallPos();

	infoEntities getEntity(int i) { return (*entities)[i]; };
	int getNEntities() { return entities->size(); };

	void setActualRoom(int room);
	int getActualRoom() const { return sublvl; };

private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize, sublvl;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	glm::vec2 ballPos;

	vector<infoEntities> *entities;

	playableArea playerArea;

};


#endif // _TILE_MAP_INCLUDE


