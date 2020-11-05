#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#define ESCALAT 2.f

#define SCREEN_X 32 * ESCALAT
#define SCREEN_Y 42 * ESCALAT

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 20

#define SCROLL_VEL 6
#define TIME_TO_SRTART 3000

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

	topBar = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	topBarImage.loadFromFile("images/topBar.png", TEXTURE_PIXEL_FORMAT_RGBA);

	loadLvl(1);

	// init camera position
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		cout << "Could not load font!!!" << endl;

	initVariables();

	scrolling = false;
	godMode = false;
	lastGValue = false;
	lastRPValue = false;
	scrollingUp = false;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	// GOD MODE
	if (Game::instance().getKey(103)) { // G key to enable and disable the god mode!!
		if (!lastGValue) {
			godMode = !godMode;
			lastGValue = true;
		}
	}
	else lastGValue = false;

	if (currentTime >= TIME_TO_SRTART) ball->setGameStarted(true);

	ball->update(deltaTime);
	map->setBallPos(ball->getPosition());
	map->setBallAngle(ball->getAngle());
	entities->update(deltaTime);
	player->update(deltaTime);

	

	if (entities->ballHasColided()) {
		ball->treatCollision(entities->getN());
	}
	if (player->getBallColided() && ball->getAngle() >= 180.f) {
		glm::vec2 dir;
		float vel;

		if (godMode) {
			dir = glm::vec2(0, 1);
			vel = 8.f;
			ball->setNewDirection(dir);
			ball->setVelocity(vel);
		} 
		else {
			dir = player->getN();
			vel = player->getNewBallVelocity();
			if (dir.x != 0) {
				ball->setNewDirection(dir);
				ball->setVelocity(vel);
			}
		} 

		
	}
	//check if ball is going to next/previous room & scroll
	glm::vec2 ballPos = ball->getPosition();
	int miny = map->getPlayableArea().miny;
	int maxy = map->getPlayableArea().maxy;

	if (ballPos.y + 32 < miny) { // ball touched top border
		if (room <= 3) {
			changeRoom(-1, ballPos);
		}
	}
	if (ballPos.y > (maxy + 2.7 * map->getTileSize()) ) {   // ball touches bottom border
		if (room == 1 && !scrolling) {
			ball->setVelocity(0);
			// TODO: set dead animation in player
			if (lives == 0) {
				// TODO: gameover
			}
			else {
				ball->setPosition(glm::vec2(ballPos.x, ballPos.y - 4));
				if(!godMode) --lives;
			}
		}
		else changeRoom(1, ballPos);
	}

	if (room <= 3 && scrollingUp)
	{
		changeRoom(-1, ballPos);
		scrollingUp = scrolling;
	}

	// KEYS TO CHANGE THE ROOM!
	if (Game::instance().getKey(119)) { // 'W' KEY: GO TO THE NEXT ROOM.
		if (!lastRPValue && !scrollingUp) {
			scrollingUp = true;
			lastRPValue = true;
		}
	}
	else lastRPValue = false;

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
	text.render("MONEY", glm::vec2(455*ESCALAT, (42+30)*ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
	text.render( to_string_zeros(money, 8), glm::vec2(455*ESCALAT, (42 + 2 * 33) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

	text.render("POINTS", glm::vec2(455 * ESCALAT, (42 + 3*39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(points, 8), glm::vec2(455 * ESCALAT, (42 + 4 * 38) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

	text.render("LIVES", glm::vec2(455 * ESCALAT, (42 + 5 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(lives, 2), glm::vec2(455 * ESCALAT, (42 + 6 * 38) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

	text.render("BANK", glm::vec2(455 * ESCALAT, (42 + 7 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(bank,2), glm::vec2(455 * ESCALAT, (42 + 8 * 38.5) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

	text.render("ROOM", glm::vec2(455 * ESCALAT, (42 + 9 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
	text.render(to_string_zeros(room,2), glm::vec2(455 * ESCALAT, (42 + 10 * 39) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));
	
}

string Scene::to_string_zeros(int number, int num_zeros)
{
	std::stringstream ss;
	ss << std::setw(num_zeros) << std::setfill('0') << number;
	std::string s = ss.str();
	return s;
}

void Scene::scroll(int direction)
{
	top += direction * SCROLL_VEL;
	bottom += direction * SCROLL_VEL;
	projection = glm::ortho(left, right, bottom, top);

	//scroll background image too
	glm::vec2 geom[2] = { glm::vec2(left, top), glm::vec2(right, bottom) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background->setPosition(geom, texCoords, texProgram);
	topBar->setPosition(geom, texCoords, texProgram);
}

void Scene::changeRoom(int dir, glm::vec2 ballPos)
{
	if (!scrolling) {
		player->setVisibility(false);

		ball->setVisibility(false);
		prev_vel = ball->getVelocity();
		ball->setVelocity(0);

		scrolling = true;
		next_margin = top + dir * 24 * map->getTileSize();
		room -= dir;
		map->setActualRoom(room);
	}
	else {
		if ( (dir == -1 && top > next_margin) || (dir == 1 && top < next_margin)) scroll(dir);
		else {
			scrolling = false;
			map->setPlayableArea(1 * map->getTileSize(), int(top) + 2 * map->getTileSize(), float(20.5) * map->getTileSize(), int(top) + 20 * map->getTileSize());
			
			glm::vec2 playerPos = player->getPosition();
			player->setPosition(glm::vec2(playerPos.x, playerPos.y + dir * 24 * map->getTileSize()));
			if (scrollingUp) player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + INIT_PLAYER_Y_TILES * map->getTileSize()));
			player->setVisibility(true);

			ball->setPosition(glm::vec2(ballPos.x, ballPos.y + dir * 5 * map->getTileSize()));
			if (scrollingUp) ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 22, -8 + top + INIT_PLAYER_Y_TILES * map->getTileSize()));
			ball->setVelocity(prev_vel);
			ball->setVisibility(true);
		}
	}
}

void Scene::loadLvl(int lvl)
{
	bank = lvl;
	map = TileMap::createTileMap("levels/lvl"+to_string(lvl)+".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setActualRoom(1);
	map->setPlayableArea(1 * map->getTileSize(), int(top) + 2 * map->getTileSize(), float(20.5) * map->getTileSize(), int(top) + 20 * map->getTileSize());

	entities = new Entities();
	entities->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + INIT_PLAYER_Y_TILES * map->getTileSize()));

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 22, - 8 + top + INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setGameStarted(false);
	currentTime = 0.f;
}

void Scene::initVariables()
{
	money = 0;
	points = 0;
	lives = 4;
	bank = 1;
	room = 1;
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



