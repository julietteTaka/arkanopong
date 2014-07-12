#ifndef GAME_H
#define GAME_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <time.h>
#include <SDL/SDL_mixer.h>

#include "../../include/objects/Brick.h"
#include "../../include/objects/Balle.h"

int launchGame(char * configFile, char * themePath, int isIa);
void reshape();

#endif
