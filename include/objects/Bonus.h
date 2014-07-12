#ifndef BONUS_H
#define BONUS_H
#include <SDL/SDL_image.h>
#include "Player.h"

typedef struct{
	Entity* entity;	
	Player* player;
	float radius;
	int id;
	GLuint textureId;
}Bonus;

Bonus* newBonus(float x, float y, float speedY, float radius, int id, GLuint textureId);
void drawBonus(Bonus * bonus);
void moveBonus(Bonus* bonus);
void freeBonus(Bonus * bonus);
#endif