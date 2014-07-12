#include "../../include/objects/Brick.h"
#include "../../include/objects/Object.h"

Brick* newBrick(float x, float y, float width, float height, int strength, GLuint textureId ){
	Brick* brick = (Brick*) malloc(sizeof(Brick));
	if(NULL == brick) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	Object* obj;
	obj = newObject(x, y);
	
	brick->obj = obj;
	brick->width = width;
	brick->height = height;
	brick->strength = strength;
	brick->textureId = textureId;
	
	return brick;
}


void drawBrick(Brick * brick){
	if(NULL == brick || brick->strength < 0){
		return;
	}
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, brick->textureId);
   
   glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
		
		glTranslatef(brick->obj->x,brick->obj->y, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(0,0);
			glTexCoord2f(1,0);
			glVertex2f(brick->width,0);
			glTexCoord2f(1,1);
			glVertex2f(brick->width,-brick->height);
			glTexCoord2f(0,1);
			glVertex2f(0,-brick->height);
		glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

void freeBrick(Brick * brick){
	free(brick->obj);
	free(brick);
}
