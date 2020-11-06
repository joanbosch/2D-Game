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

	void init(int lvl, int points, int coins, int lives);
	void update(int deltaTime);
	void render();
	void initVariables(int points, int coins, int lives);

private:
	void initShaders();
	string to_string_zeros(int number, int num_zeros);
	void changeRoom(int direction, glm::vec2 ballPos);
	void playerDies();

private:
	TileMap *map;
	Ball *ball;
	Entities *entities;
	Player *player;
	
	ShaderProgram texProgram;
	float currentTime, markTime, startTime;
	glm::mat4 projection;

	Texture backgroundImage, topBarImage;
	TexturedQuad *background, *topBar;

	Text text;

	void scroll(int direction);

	int money;
	int points;
	int lives;
	int bank;
	int room;

	float left, right, bottom, top;

	float next_margin, prev_vel;
	bool scrolling, scrollingUp;
	bool godMode;
	bool lastGValue, lastRPValue;
};


#endif // _SCENE_INCLUDE

