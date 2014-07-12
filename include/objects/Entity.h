#ifndef ENTITY_H
#define ENTITY_H
#include "Object.h"
typedef struct{
	Object* obj;
	float speedX;
	float speedY;
}Entity;

Entity* newEntity (float x, float y, float speedX, float speedY);

#endif
