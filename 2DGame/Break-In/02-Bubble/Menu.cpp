#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25


Menu::Menu()
{
}

Menu::~Menu()
{
}


void Menu::init()
{
	initShaders();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	option_selected = PLAY;

	backgorundImage.loadFromFile("images/menu-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	// Select which font you want to use
	if (!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Menu::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(backgorundImage);
	
	// Select the correct option that the user has selected.
	if (option_selected == PLAY) {
		text.render("PLAY", glm::vec2(260, 380), 20, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));
	}
	else if (option_selected == INSTRUCTIONS) {
		text.render("PLAY", glm::vec2(260, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(220, 420), 20, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(250, 460), 14, glm::vec4(1, 1, 1, 1));
	}
	else if (option_selected == CREDITS) {
		text.render("PLAY", glm::vec2(260, 380), 14, glm::vec4(1, 1, 1, 1));
		text.render("INSTRUCTIONS", glm::vec2(220, 420), 14, glm::vec4(1, 1, 1, 1));
		text.render("CREDITS", glm::vec2(250, 460), 20, glm::vec4(1, 1, 1, 1));

	}
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}