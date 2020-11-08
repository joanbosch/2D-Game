#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

#define ESCALAT 2.f

#define SCREEN_X 32 * ESCALAT
#define SCREEN_Y 42 * ESCALAT

#define INIT_PLAYER_X_TILES 10
#define INIT_PLAYER_Y_TILES 20

#define SCROLL_VEL 6
#define TIME_TO_SRTART 3000
#define TIME_CHANGING_LEVEL 12000

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


void Scene::initVariables(int points, int coins, int lives)
{
	money = coins;
	this->points = points;
	this->lives = lives;
	bank = 1;
	room = 1;
}

void Scene::init(int lvl, int points, int coins, int lives, Audio* audio)
{
	initShaders();
	left = 0.f;
	right = float(SCREEN_WIDTH - 1);
	bottom = float((2.6 * SCREEN_HEIGHT) - 1);
	top = float((1.6 * SCREEN_HEIGHT) - 1);

	glm::vec2 geom[2] = { glm::vec2(left, top), glm::vec2(right, bottom) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };
	background = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	backgroundImage.loadFromFile("images/bkgLvl" + to_string(lvl) + ".png", TEXTURE_PIXEL_FORMAT_RGBA);

	topBarImage.loadFromFile("images/topBar.png", TEXTURE_PIXEL_FORMAT_RGBA);
	topBar = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1, 1), &topBarImage, &texProgram);
	topBar->setPosition(glm::vec2(0, top));

	map = TileMap::createTileMap("levels/lvl" + to_string(lvl) + ".txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	map->setActualLevel(lvl);
	map->setActualRoom(1);
	map->setPlayableArea(1 * map->getTileSize(), int(top) + 2 * map->getTileSize() - 2, float(20.5) * map->getTileSize(), int(top) + 20 * map->getTileSize());

	audioManager = audio;

	entities = new Entities();
	entities->init(glm::vec2(SCREEN_X, SCREEN_Y), texProgram, map, audioManager);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + INIT_PLAYER_Y_TILES * map->getTileSize()));

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, audioManager);
	ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 22, -8 + top + INIT_PLAYER_Y_TILES * map->getTileSize()));
	ball->setTileMap(map);
	ball->setGameStarted(false);

	// init camera position
	projection = glm::ortho(left, right, bottom, top);
	currentTime = 0.0f;
	markTime = 0.0f;
	startTime = TIME_TO_SRTART;

	if (!text.init("fonts/AnimalCrossing.ttf"))
		cout << "Could not load font!!!" << endl;

	initVariables(points, coins, lives);
	bank = lvl;

	godMode = false;
	lastGValue = false;
	scrollingUp = false;
	changingLevel = false;
	gameOver = false;
	playerDying = false;
	winScreen = false;
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

	if (currentTime >= startTime) ball->setGameStarted(true);

	// changing level
	if (changingLevel) {
		if (!audioManager->isPlaying(CHANGE_LEVEL_MUSIC)) {
			audioManager->stopAllSounds();
			audioManager->play(CHANGE_LEVEL_MUSIC, true);
		}
		thief->update(deltaTime);
		if (currentTime > win_time + TIME_CHANGING_LEVEL) {
			int next_level = map->getActualLevel() + 1;
			switch (next_level) {
			case 1:
				audioManager->stopAllSounds();
				audioManager->play(LEVEL1_MUSIC, true);
				break;
			case 2:
				audioManager->stopAllSounds();
				audioManager->play(LEVEL2_MUSIC, true);
				break;
			case 3:
				audioManager->stopAllSounds();
				audioManager->play(LEVEL3_MUSIC, true);
				break;
			default:
				audioManager->stopAllSounds();
				audioManager->play(LEVEL3_MUSIC, true);
				break;
			}
			if(next_level != 4) init(glm::min(next_level, 3), points, money, lives, audioManager);
			else {
				backgroundImage.loadFromFile("images/Win.png", TEXTURE_PIXEL_FORMAT_RGBA);
				winScreen = true;
			}
		}
	}
	else {
		// play screen
		ball->update(deltaTime);
		map->setBallPos(ball->getPosition());
		map->setPlayerPos(player->getPosition());
		map->setBallAngle(ball->getAngle());
		entities->update(deltaTime);
		player->update(deltaTime);
	}

	// SET ANIMATION TO PLAYER
	if (!playerDying) {
		player->updateAnimation(ball->getPosition(), entities->isStarMode());
	}

	// STAR MODE
	ball->setStarMode(entities->isStarMode());

	if (entities->ballHasColided()) {
		ball->treatCollision(entities->getN());
	}

	// check if player colided with police
	if (entities->playerHasColided() && !playerDying) {
		playerDying = true;
		playerDies();
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

	// UPDATE the money and points counter
	if(!changingLevel) money += entities->getNewCoins();
	points += entities->getNewPoints();

	// If no money entities remaining, go to next level.
	if (entities->getRemainingMoneyEntities() == 0 && !changingLevel) {
		changingLevel = true;
		win_time = currentTime;
		backgroundImage.loadFromFile("images/nextLvl.png", TEXTURE_PIXEL_FORMAT_RGBA);
		thief = new Thief();
		thief->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, map);
		thief->setPosition(glm::vec2(5*map->getTileSize(), 16*map->getTileSize()));
	}

	//check if ball is going to next/previous room & scroll
	glm::vec2 ballPos = ball->getPosition();
	int miny = map->getPlayableArea().miny;
	int maxy = map->getPlayableArea().maxy;

	if (!entities->ballHasColided()) {
		if (ballPos.y + 32 < miny) { // ball touched top border
			if (room <= 3) {
				changeRoom(-1, ballPos);
			}
		}
		if (ballPos.y > (maxy + 2.93 * map->getTileSize())) {   // ball touches bottom border
			if (room == 1 && !map->getScrolling()) {
				playerDying = true;
				playerDies();
			}
			else changeRoom(1, ballPos);
		}
	}

	// player dying animation is over
	if (markTime != NULL && currentTime >= markTime) {
		entities->setPlayerDead();
		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(), top + INIT_PLAYER_Y_TILES * map->getTileSize()));
		ball->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize() + 22, -8 + top + INIT_PLAYER_Y_TILES * map->getTileSize()));
		ball->setGameStarted(false);
		ball->setVelocity(prev_vel);
		player->setDead(false);
		startTime = currentTime + TIME_TO_SRTART;
		markTime = NULL;
		playerDying = false;
		entities->setPlayerColided(false);
	}

	if (room <= 3 && scrollingUp)
	{
		changeRoom(-1, ballPos);
		scrollingUp = map->getScrolling();
	}

	// KEYS TO CHANGE THE ROOM!
	if (Game::instance().getKey(119) && !map->getScrolling() && room<3) { // 'W' KEY: GO TO THE NEXT ROOM.
		changeRoom(-1, ballPos);
		scrollingUp = true;
	}

	// if on gameover screen and ENTER key pressed, restart game
	if (Game::instance().getKey(13)) {
		if (gameOver || winScreen) {
			audioManager->stopAllSounds();
			audioManager->play(LEVEL1_MUSIC, true);
			init(1, 0, 0, 4, audioManager);
		}
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

	background->render(backgroundImage);
	if (!((gameOver && !winScreen) ||(!gameOver && winScreen))) {
		if (!changingLevel) {
			map->render();
			entities->render();
			topBar->render();
			ball->render();
			player->render();
		}
		else thief->render();


		// Rendender text
		text.render("MONEY", glm::vec2(455 * ESCALAT, (42 + 30) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
		text.render(to_string_zeros(money, 8), glm::vec2(455 * ESCALAT, (42 + 2 * 33) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

		text.render("POINTS", glm::vec2(455 * ESCALAT, (42 + 3 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
		text.render(to_string_zeros(points, 8), glm::vec2(455 * ESCALAT, (42 + 4 * 38) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

		text.render("LIVES", glm::vec2(455 * ESCALAT, (42 + 5 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
		text.render(to_string_zeros(lives, 2), glm::vec2(455 * ESCALAT, (42 + 6 * 38) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

		text.render("BANK", glm::vec2(455 * ESCALAT, (42 + 7 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
		text.render(to_string_zeros(bank, 2), glm::vec2(455 * ESCALAT, (42 + 8 * 38.5) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

		text.render("ROOM", glm::vec2(455 * ESCALAT, (42 + 9 * 39.6) * ESCALAT), 40 * ESCALAT, glm::vec4(1, 0.81, 0.3, 1));
		text.render(to_string_zeros(room, 2), glm::vec2(455 * ESCALAT, (42 + 10 * 39) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 1, 1, 1));

		if (changingLevel) {
			text.render("PASSWORD   " + to_string(bank) + "   IS", glm::vec2(70, (42 + 10 * 39) * ESCALAT), 30 * ESCALAT, glm::vec4(1, 0.7, 0.5, 1));
			text.render(getPassword(bank), glm::vec2(500, (42 + 10 * 39) * ESCALAT), 30 * ESCALAT, glm::vec4(0.5, 1, 0, 1));
		}
		else {
			if (godMode) {
				text.render("GOD MODE is ENABLED", glm::vec2(70, (65 + 10 * 39) * ESCALAT), 15 * ESCALAT, glm::vec4(1, 0.7, 0.5, 1));
			}
		}
	}
}

string Scene::getPassword(int level) 
{
	switch (level) {
	case 1:
		return "NEWLEAF";
	case 2:
		return "TOMNOOK";
	case 3:
		return "ANTONI";
	default:
		return "LMAO";

	}

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
	topBar->setPosition(glm::vec2(0, top));
}

void Scene::changeRoom(int dir, glm::vec2 ballPos)
{
	if (!map->getScrolling()) {
		player->setVisibility(false);

		ball->setVisibility(false);
		prev_vel = ball->getVelocity();
		ball->setVelocity(0);

		map->setScrolling(true);

		next_margin = top + dir * 24 * map->getTileSize();
		room -= dir;
		map->setActualRoom(room);
	}
	else {
		if ( (dir == -1 && top > next_margin) || (dir == 1 && top < next_margin)) scroll(dir);
		else {
			map->setScrolling(false);
			map->setPlayableArea(1 * map->getTileSize(), int(top) + 2 * map->getTileSize() - 2, float(20.5) * map->getTileSize(), int(top) + 20 * map->getTileSize());
			
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

void Scene::playerDies() {
	prev_vel = ball->getVelocity();
	ball->setVelocity(0);
	if (lives == 0) {
		if (!audioManager->isPlaying(GAMEOVER_MUSIC)) {
			audioManager->stopAllSounds();
			audioManager->play(GAMEOVER_MUSIC, true);
			audioManager->play(NOOK_LAUGH, true);
		}
		backgroundImage.loadFromFile("images/GameOver.png", TEXTURE_PIXEL_FORMAT_RGBA);
		gameOver = true;
	}
	else {
		ball->setPosition(glm::vec2(ball->getPosition().x, ball->getPosition().y - 3));
		audioManager->play(PLAYER_DEAD_SOUND, false);
		if (!godMode) --lives;
		player->setDead(true);
		markTime = currentTime + 1300; // set timeout for animation to run
	}
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



