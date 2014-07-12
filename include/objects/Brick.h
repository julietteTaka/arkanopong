#ifndef BRICK_H
#define BRICK_H
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include "Object.h"

typedef struct{
	Object * obj;
	float width;
	float height;
	int strength;
	GLuint textureId;
}Brick;

Brick* newBrick(float x, float y, float width, float height, int strength, GLuint textureId);
void drawBrick(Brick * brick);
void freeBrick(Brick * brick);
#endif
