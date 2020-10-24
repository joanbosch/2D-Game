#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"


#define SCREEN_X 32
#define SCREEN_Y 42

#define INIT_PLAYER_X_TILES 11
#define INIT_PLAYER_Y_TILES 20

#define SCROLL_VEL 16

Scene::Scene()
{
	map = NULL;
	ball = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
	if(ball != NULL)
		delete ball;
	if (player != NULL)
		delete player;
}


void Scene::init()
{
	initShaders();
	left = 0.f;
	right = float(SCREEN_WIDTH - 1);
	bottom = float((2.6 * SCREEN_HEIGHT) - 1);
	top = float((1.6 * SCREEN_HEIGHT) - 1);

	glm::vec2 geom[2] = { glm::vec2(left, top), glm::vec2(right, bottom) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	backgroundImage.loadFromFile("images/bkgLvl1.png", TEXTURE_PIXEL_FORMAT_RGBA);

	glm::vec2 geom2[2] = { glm::vec2(left, top), glm::vec2(right, bottom) };
	// glm::vec2 texCoords2[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	topBar = TexturedQuad::createTexturedQuad(geom2, texCoords, texProgram);
	topBarImage.loadFromFile("images/topBar.png", TEXTURE_PIXEL_FORMAT_RGBA);

	map = TileMap::createTileMap("levels/lvl1.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setPlayableArea(1 * map->getTileSize(), top + 2 * map->getTileSize(), 20.5 * map->getTileSize(), top + 20 * map->getTileSize());

	entities = new Entities();
	entities->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + INIT_PLAYER_Y_TILES * map->getTileSize()));

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + 1.5f + INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);

	// level11 = new Level11();
	// level11->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);

	// init camera position
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		cout << "Could not load font!!!" << endl;

	money = 0;
	points = 0;
	lives = 4;
	bank = 1;
	room = 1;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	ball->update(deltaTime);
	map->setBallPos(ball->getPosition());
	entities->update(deltaTime);
	player->update(deltaTime);
	if (entities->ballHasColided()) {
		ball->treatCollision(entities->getN());
	}

	//check if ball is out of room
	glm::vec2 ballPos = map->getBallPos();

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
	background->render(backgroundImage);
	map->render();
	topBar->render(topBarImage);
	entities->render();
	ball->render();
	player->render();
	
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



