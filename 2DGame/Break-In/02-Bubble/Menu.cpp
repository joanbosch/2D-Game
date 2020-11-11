#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"
#include "Game.h"
#include <GL/glut.h>


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define ESCALAT 2.f

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

Menu::Menu()
{
}

Menu::~Menu()
{
}


void Menu::init(Audio *audio)
{
	initShaders();
	bopt = true;
	count = 0;
	audioManager = audio;
	audioManager->play(MENU_MUSIC, true);

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	backgorundImage.loadFromFile("images/menu-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	count++;
	if (count > 35) {
		count = 0;
		bopt = !bopt;
	}
	if (Game::instance().getKey(13)) {
		Game::instance().setLvl(1, 0, 0, 4);
		audioManager->stopAllSounds();
		audioManager->play(LEVEL1_MUSIC, true);
		Game::instance().setState(PLAY);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_F1)) {
		Game::instance().setState(INSTRUCTIONS);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_F2)) {
		Game::instance().setState(CREDITS);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_F3)) {
		Game::instance().setState(PASSWORD);
	}
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
	

	if(bopt) text.render("Press ENTER to START", glm::vec2(220* ESCALAT, 360* ESCALAT), 20* ESCALAT, glm::vec4(1, 1, 1, 1));
	text.render("Press F1 to see INSTRUCTIONS", glm::vec2(200* ESCALAT, 390* ESCALAT), 20* ESCALAT, glm::vec4(1, 1, 1, 1));
	text.render("Press F2 to see CREDITS", glm::vec2(220* ESCALAT, 420* ESCALAT), 20* ESCALAT, glm::vec4(1, 1, 1, 1));
	text.render("Press F3 to see PASSWORDS", glm::vec2(210* ESCALAT, 450* ESCALAT), 20* ESCALAT, glm::vec4(1, 1, 1, 1));
	
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