#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include <sstream>
#include <iomanip>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TexturedQuad.h"
#include "Text.h"
#include "Level1-1.h"
#include "Ball.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	string to_string_zeros(int number, int num_zeros);

private:
	TileMap *map;
	Ball *player;
	Level11* level11;
	
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture backgorundImage;
	TexturedQuad *background;

	Text text;

	int money;
	int points;
	int lives;
	int bank;
	int room;

};


#endif // _SCENE_INCLUDE

