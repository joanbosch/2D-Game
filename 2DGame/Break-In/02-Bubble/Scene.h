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
#include "Entities.h"
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
	Ball *ball;
	Entities *entities;
	Player *player;
	
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture backgroundImage;
	TexturedQuad *background;
	Texture topBarImage;
	TexturedQuad* topBar;

	Text text;

	void scroll(int direction);

	int money;
	int points;
	int lives;
	int bank;
	int room;

	float left, right, bottom, top;

	int prev_top, next_top;
	int scroll_delay;
	bool scrolling;

};


#endif // _SCENE_INCLUDE

