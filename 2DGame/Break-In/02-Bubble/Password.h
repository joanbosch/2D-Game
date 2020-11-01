#ifndef _PASSWORD_INCLUDE
#define _PASSWORD_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TexturedQuad.h"
#include "Text.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Password
{

public:
	Password();
	~Password();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void addUnderscoreSpace(int key);

private:
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Texture backgorundImage;
	TexturedQuad* background;

	int count, underscorePos;
	bool bopt;
	string pwd;
	Text text;
};


#endif // _PASSWORD_INCLUDE

