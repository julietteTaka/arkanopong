#include <stdio.h>
#include "../../include/game/Theme.h"
#define TEXT_DIR "textures"

static int loadTexture(GLuint textureId, char * path){
	SDL_Surface* image =  IMG_Load(path);
	if (image == NULL){
		fprintf(stderr, "Impossible de charger l'image %s\n", path);
		return EXIT_FAILURE;
	}
	
	GLenum format;
	switch(image->format->BytesPerPixel) {
		case 1:
			format = GL_RED;
		break;
		case 3:
			format = GL_RGB;
		break;
		case 4:
			format = GL_RGBA;
		break;
		default:
			fprintf(stderr, "Format des pixels de l’image %s non pris en charge\n", path);
			return EXIT_FAILURE;
	}

	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA, 
		image->w, 
		image->h, 
		0, 
		format, 
		GL_UNSIGNED_BYTE,
		image->pixels);

	glBindTexture(GL_TEXTURE_2D, 0); // on debind la texture sur laquelle des operations ont été faites
	SDL_FreeSurface(image); //libération de la ram
	return 1;
}
int loadTheme(char * themePath, GLuint textureIds[]){
	DIR* dp = NULL;
    dp = opendir(themePath);
    if (NULL == dp){
		perror("opendir() error");
        return 0;
    }
   
	int i;
	char tmp[C_MAX];
	tmp[0] = '\0';

	/* creation du tableau de textures */
	glGenTextures(GAME_OVER_TEXT +1, textureIds);

  	for(i = 0; i < BRICK_MAX_STRENGTH; i++) {
	  	sprintf(tmp, "%s/%s/brick%d.png", themePath,TEXT_DIR, i+1);
		loadTexture(textureIds[i], tmp);
	}

  	sprintf(tmp, "%s/%s/balle.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BALL_TEXT_IDX], tmp);

	sprintf(tmp, "%s/%s/player.png", themePath,TEXT_DIR);
	loadTexture(textureIds[PLAYER_BOTTOM_TEXT_IDX], tmp);

	sprintf(tmp, "%s/%s/player.png", themePath,TEXT_DIR);
	loadTexture(textureIds[PLAYER_TOP_TEXT_IDX], tmp);

	sprintf(tmp, "%s/%s/background.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BACKGROUND_TEXT_IDX], tmp);

	sprintf(tmp, "%s/%s/life.png", themePath,TEXT_DIR);
	loadTexture(textureIds[LIFE_TEXT_IDX], tmp);

	sprintf(tmp, "%s/%s/speed.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BONUS_TEXT_ICO_SPEED_IDX], tmp);

	sprintf(tmp, "%s/%s/biger.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BONUS_TEXT_ICO_SIZE_IDX], tmp);

	sprintf(tmp, "%s/%s/lifesup.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BONUS_TEXT_ICO_LIFESUP_IDX], tmp);

	sprintf(tmp, "%s/%s/glu.png", themePath,TEXT_DIR);
	loadTexture(textureIds[BONUS_TEXT_ICO_GLU_IDX], tmp);

	sprintf(tmp, "%s/%s/gameOver.png", themePath,TEXT_DIR);
	loadTexture(textureIds[GAME_OVER_TEXT], tmp);

	free(dp);
    return 1;
}