#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL/glut.h>
#include "Password.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define ESCALAT 2.f

Password::Password()
{
}

Password::~Password()
{
}


void Password::init()
{
	initShaders();
	bopt = true;
	count = 0;
	slashPos = 700;
	pwd = "";

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(SCREEN_WIDTH, SCREEN_HEIGHT) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	backgorundImage.loadFromFile("images/menu-background.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Password::update(int deltaTime)
{
	currentTime += deltaTime;
	count++;
	if (count > 5) {
		count = 0;
		bopt = !bopt;
	}

	//detect letter pressed
	if (pwd.size() < 8) {
		for (int i = 'A'; i <= 'Z'; ++i) {
			if (Game::instance().getKey(i)) {
				Game::instance().keyReleased(i);
				pwd += (char)i;
				addSlashSpace(i);
			}
		}
		for (int i = 'a'; i <= 'z'; ++i) {
			if (Game::instance().getKey(i)) {
				Game::instance().keyReleased(i);
				pwd += (char)i - 32;
				addSlashSpace(i);
			}
		}
	}
	else {
		if (Game::instance().getKey(13)) {
			pwd = "";
			slashPos = 700;
		}
	}
	//look if password is valid, if valid switch case and load selected lvl and change game state, else pws = ""
	if (Game::instance().getKey(13)) {
		slashPos = 700;
		if (pwd == "EASY") {
			pwd = "";
			Game::instance().setLvl(1);
			Game::instance().setState(PLAY);
		}
		else if (pwd == "TOMNOOK") {
			pwd = "";
			Game::instance().setLvl(2);
			Game::instance().setState(PLAY);
		}
		else if (pwd == "ANTONI") {
			pwd = "";
			Game::instance().setLvl(3);
			Game::instance().setState(PLAY);
		}
	}
}

void Password::render()
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
	text.render("ENTER  PASSWORD   :  ", glm::vec2(260, 380 * ESCALAT), 25 * ESCALAT, glm::vec4(1, 1, 1, 1));
	text.render(pwd, glm::vec2(700, 380 * ESCALAT), 25 * ESCALAT, glm::vec4(1, 1, 1, 1));
	if (bopt && (pwd.size() != 8)) text.render("_", glm::vec2(slashPos , 380 * ESCALAT), 25 * ESCALAT, glm::vec4(1, 1, 1, 1));

}

void Password::addSlashSpace(int key)
{
	if (key == 'i' || key == 'I') {
		slashPos += 16;
	}
	else if (key == 's' || key == 'S') {
		slashPos += 20;
	}
	else if (key == 'j' || key == 'J') {
		slashPos += 22;
	}
	else if (key == 'z' || key == 'Z') {
		slashPos += 23;
	}
	else if (key == 'w' || key == 'W') {
		slashPos += 35;
	}
	else if (key == 'q' || key == 'Q') {
		slashPos += 31;
	}
	else if (key == 'm' || key == 'M') {
		slashPos += 32;
	}
	else if (key == 'h' || key == 'H' || key == 'o' || key == 'O') {
		slashPos += 29;
	}
	else if (key == 'g' || key == 'G' || key == 'n' || key == 'N' || key == 'u' || key == 'U') {
		slashPos += 28;
	}
	else if (key == 'b' || key == 'B' || key == 'f' || key == 'F' || key == 'l' || key == 'L') {
		slashPos += 24;
	}
	else if (key == 'd' || key == 'D' || key == 't' || key == 'T' || key == 'y' || key == 'Y') {
		slashPos += 28;
	}
	else if (key == 'a' || key == 'A' || key == 'k' || key == 'K' || key == 'v' || key == 'V' || key == 'x' || key == 'X') {
		slashPos += 27;
	}
	else {
		slashPos += 25;
	}
}

void Password::initShaders()
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