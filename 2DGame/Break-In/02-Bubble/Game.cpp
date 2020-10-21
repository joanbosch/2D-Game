#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	state = MENU;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	scene.init();
	instructions.init();
	credits.init();
	menu.init();

}

bool Game::update(int deltaTime)
{
	switch (state) {
		case PLAY:
			scene.update(deltaTime);
			break;

		case INSTRUCTIONS:
			instructions.update(deltaTime);
			break;

		case CREDITS:
			credits.update(deltaTime);
			break;

		case MENU:
			menu.update(deltaTime);
			break;
	}

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	switch (state) {
	case PLAY:
		scene.render();
		break;

	case INSTRUCTIONS:
		instructions.render();
		break;

	case CREDITS:
		credits.render();
		break;

	case MENU:
		menu.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		//bPlay = false;
		state = MENU;
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::setState(int s)
{
	state = s;
}






