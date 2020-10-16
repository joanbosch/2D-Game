#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "TexturedQuad.h"
#include "Text.h"

#define PLAY 0
#define INSTRUCTIONS 1
#define CREDITS 2

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Menu
{

public:
	Menu();
	~Menu();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	//TileMap* map;
	//Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture backgorundImage;
	TexturedQuad *background;

	int option_selected;

	Text text;
};


#endif // _MENU_INCLUDE
