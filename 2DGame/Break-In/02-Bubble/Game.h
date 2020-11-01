#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Instructions.h"
#include "Credits.h"
#include "Password.h"


#define SCREEN_WIDTH 640*2
#define SCREEN_HEIGHT 480*2


#define PLAY 0
#define INSTRUCTIONS 1
#define CREDITS 2
#define MENU 3
#define PASSWORD 4



// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	// Change the state of the game
	void setState(int s);
	void setLvl(int l);

private:
	bool bPlay;                       // Continue to play game?
	Menu menu;						  // Main Menu of the game
	Instructions instructions;		  // Instructions of the game
	Credits credits;				  // Credits of the game
	Scene scene;                      // Scene to render
	Password password;				  // Password screen
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

	int state; // Encode the current state of the game (Menu, Play, Instructions or Credits)
};


#endif // _GAME_INCLUDE


