#ifndef BALLE_H
#define BALLE_H
#include "Entity.h"
#include "Player.h"
#include "Brick.h"

typedef struct{
	Entity* entity;	
	Player* player;
	float radius;
	int glu;
	int id;
	GLuint textureId;
}Balle;

Balle* newBalle(Player * player, float x, float y, float speedX, float speedY, float radius, GLuint textureId);
void drawBalle(Balle * balle);
void moveBalle(Balle * balle);
void launchBalle(Balle * balle, float speedX, float speedY);
void freeBalle(Balle * balle);

#endif
