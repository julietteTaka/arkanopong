#ifndef THEME_H
#define THEME_H
#include <dirent.h>
#ifndef WIN32
    #include <sys/types.h>
#endif
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define C_MAX 100
#define BRICK_MAX_STRENGTH 4
#define BALL_TEXT_IDX BRICK_MAX_STRENGTH
#define PLAYER_BOTTOM_TEXT_IDX BRICK_MAX_STRENGTH + 1
#define PLAYER_TOP_TEXT_IDX BRICK_MAX_STRENGTH + 2
#define BACKGROUND_TEXT_IDX BRICK_MAX_STRENGTH + 3
#define LIFE_TEXT_IDX BRICK_MAX_STRENGTH + 4

#define BONUS_TEXT_ICO_SIZE_IDX BRICK_MAX_STRENGTH + 5
#define BONUS_TEXT_ICO_SPEED_IDX BRICK_MAX_STRENGTH + 6
#define BONUS_TEXT_ICO_LIFESUP_IDX BRICK_MAX_STRENGTH + 7
#define BONUS_TEXT_ICO_GLU_IDX BRICK_MAX_STRENGTH + 8

#define GAME_OVER_TEXT BRICK_MAX_STRENGTH + 9

int loadTheme(char * themePath, GLuint textureIds[]);

#endif
