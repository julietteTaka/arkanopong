#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "../../include/objects/Object.h"

Object* newObject(float x, float y){
	Object* o = malloc(sizeof(Object));
	if(NULL == o) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
	}
	o->x = x;
	o->y = y;
	return o;
}



