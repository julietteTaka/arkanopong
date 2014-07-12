#ifndef PLAYER_H
#define PLAYER_H
#include <SDL/SDL_image.h>
#include "Entity.h"

typedef struct{
	Entity* entity;
	float width;
	float height;
	int lifePoints;
	int isPressedLeft;
	int isPressedRight;
	GLuint textureId;
	char activatedBonus;
} Player;

Player* newPlayer(float x, float y, float speedX, float speedY, float width, float height, int lifePoints, GLuint textureId);
void drawPlayer(Player * player);
void freePlayer(Player * player);
#endif
