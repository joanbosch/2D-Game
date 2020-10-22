#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 42

#define INIT_PLAYER_X_TILES 12
#define INIT_PLAYER_Y_TILES 22


Scene::Scene()
{
	map = NULL;
	ball = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(ball != NULL)
		delete ball;
}


void Scene::init()
{
	initShaders();
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	backgorundImage.loadFromFile("images/bkgLvl1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	map = TileMap::createTileMap("levels/level1_1_new.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);

	level11 = new Level11();
	level11->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		//if(!text.init("fonts/OpenSans-Bold.ttf"))
		//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;

	money = 0;
	points = 0;
	lives = 4;
	bank = 0;
	room = 1;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	ball->update(deltaTime);
	map->setBallPos(ball->getPosition());
	level11->update(deltaTime);
	if (level11->ballHasColided()) {
		ball->treatCollision(level11->getN());
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render(backgorundImage);
	map->render();
	ball->render();
	level11->render();
	

	// Rendender text
	text.render("MONEY", glm::vec2(455, 42+30), 40, glm::vec4(1, 0.81, 0.3, 1));
	text.render( to_string_zeros(money, 8), glm::vec2(455, 42 + 2 * 33), 30, glm::vec4(1, 1, 1, 1));

	text.render("POINTS", glm::vec2(455, 42 + 3*39.6), 40, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(points, 8), glm::vec2(455 , 42 + 4 * 38), 30, glm::vec4(1, 1, 1, 1));

	text.render("LIVES", glm::vec2(455, 42 + 5 * 39.6), 40, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(lives, 2), glm::vec2(455, 42 + 6 * 38), 30, glm::vec4(1, 1, 1, 1));

	text.render("BANK", glm::vec2(455, 42 + 7 * 39.6), 40, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(bank,2), glm::vec2(455, 42 + 8 * 38.5), 30, glm::vec4(1, 1, 1, 1));

	text.render("ROOM", glm::vec2(455, 42 + 9 * 39.6), 40, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(room,2), glm::vec2(455, 42 + 10 * 39), 30, glm::vec4(1, 1, 1, 1));
	
}

string Scene::to_string_zeros(int number, int num_zeros)
{
	std::stringstream ss;
	ss << std::setw(num_zeros) << std::setfill('0') << number;
	std::string s = ss.str();
	return s;
}
void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



