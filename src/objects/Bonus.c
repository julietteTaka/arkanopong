#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include "../../include/objects/Entity.h"
#include "../../include/objects/Bonus.h"
#include "../../include/objects/Brick.h"
#include "../../include/objects/Player.h"


Bonus* newBonus(float x, float y, float speedY, float radius, int id, GLuint textureId){
	Bonus* bonus = malloc(sizeof(Bonus));
	if(NULL == bonus) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	
	Entity* entity = newEntity(x, y, 0, speedY);
	if(NULL == entity) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	bonus->entity = entity;
	bonus->radius = radius;
	bonus->id = id;
	bonus->textureId = textureId;
	return bonus;
}

void drawBonus(Bonus * bonus){
	if(NULL == bonus || bonus->id == -1){
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bonus->textureId);
	
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
		 	
		 	x = xCos * bonus->radius + bonus->entity->obj->x;
		 	y = ySin * bonus->radius + bonus->entity->obj->y;

			glTexCoord2f(xCos * 0.5 + 0.5, -ySin * 0.5 + 0.5);
			glVertex2f(x,y);
		 }
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void moveBonus(Bonus* bonus){
	bonus->entity->obj->y += bonus->entity->speedY;
}

void freeBonus(Bonus* bonus){
	free(bonus->entity->obj);
	free(bonus->entity);
	free(bonus);
}


