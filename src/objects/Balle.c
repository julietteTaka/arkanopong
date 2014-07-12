#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include "../../include/objects/Entity.h"
#include "../../include/objects/Balle.h"
#include "../../include/objects/Brick.h"
#include "../../include/objects/Player.h"


Balle* newBalle(Player * player, float x, float y, float speedX, float speedY, float radius, GLuint textureId){
	Balle * balle = (Balle *)malloc(sizeof(Balle));
	if(NULL == balle) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	
	Entity* entity = newEntity(x, y, speedX, speedY);
	if(NULL == entity) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	balle->entity = entity;
	balle->radius = radius;
	balle->player = player;
	balle->glu = 0;
	balle->textureId = textureId;
	return balle;
}

void drawBalle(Balle * balle){

	if(NULL == balle){
		return;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, balle->textureId);
	
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glBegin(GL_POLYGON);
		 float radian, xCos, ySin, x, y;
		 float angle = 0;
		 for(angle = 0 ; angle < 360.0 ; angle += 2.0){
		 	radian = angle * (M_PI/180.0f);
		 	xCos = (float)cos(radian);
		 	ySin = (float)sin(radian);
		 	
		 	x = xCos * balle->radius + balle->entity->obj->x;
		 	y = ySin * balle->radius + balle->entity->obj->y;

		
			glTexCoord2f(xCos * 0.5 + 0.5, ySin * 0.5 + 0.5);
			glVertex2f(x,y);
		 }
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void moveBalle(Balle * balle){
	if(balle->entity->speedX == 0 &&  balle->entity->speedY == 0){
		balle->entity->obj->x = balle->player->entity->obj->x + balle->player->width/2;
	}else{
		balle->entity->obj->x += balle->entity->speedX;
		balle->entity->obj->y += balle->entity->speedY;
	}
}

void launchBalle(Balle * balle, float speedX, float speedY){
	balle->entity->speedX = speedX;
	balle->entity->speedY = speedY;
}

void freeBalle(Balle * balle){
	freePlayer(balle->player);
	free(balle->entity->obj);
	free(balle->entity);
	free(balle);
}