#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "../../include/objects/Player.h"
#include "../../include/objects/Entity.h"

Player* newPlayer(float x, float y, float speedX, float speedY, float width, float height, int lifePoints, GLuint textureId){
	Player* player = malloc(sizeof(Player));
	if(NULL == player) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}

	Entity* entity = newEntity(x, y, speedX, speedY);
	if(NULL == entity) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	
	player->entity = entity;
	player->lifePoints = lifePoints;
	player->width = width;
	player->height = height;
	
	player->isPressedLeft = 0;
	player->isPressedRight = 0;
	player->textureId = textureId;
	player->activatedBonus = 0;
	return player;
}

void drawPlayer(Player * player){
	if(NULL == player){
		return;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, player->textureId);


	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		glTranslatef(player->entity->obj->x,player->entity->obj->y,0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(0,0);
			glTexCoord2f(1,0);
			glVertex2f(player->width,0);
			glTexCoord2f(1,1);
			glVertex2f(player->width,-player->height);
			glTexCoord2f(0,1);
			glVertex2f(0,-player->height);
			
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void freePlayer(Player * player){
	free(player->entity->obj);
	free(player->entity);
	free(player);
}
