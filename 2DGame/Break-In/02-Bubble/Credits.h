#ifndef _CREDITS_INCLUDE
#define _CREDITS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Text.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Credits
{

public:
	Credits();
	~Credits();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture backgorundImage;
	TexturedQuad* background;

	int option_selected;

	Text text;
};


#endif // _CREDITS_INCLUDE
