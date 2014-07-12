#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "../../include/objects/Entity.h"
#include "../../include/objects/Object.h"


Entity* newEntity (float x, float y, float speedX, float speedY){
	Entity* entity = malloc(sizeof(Entity));
	if(NULL == entity) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}

	Object* obj = newObject(x, y);
	entity->obj = obj;
	
	entity->speedX = speedX;
	entity->speedY = speedY;
	return entity;
}
