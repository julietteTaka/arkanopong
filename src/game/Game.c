#include <math.h>
#include "../../include/game/Game.h"
#include "../../include/game/Theme.h"
#include "../../include/objects/Object.h"
#include "../../include/objects/Entity.h"
#include "../../include/objects/Player.h"
#include "../../include/objects/Brick.h"
#include "../../include/objects/Balle.h"
#include "../../include/objects/Bonus.h"
#define MUSICS_DIR "musics"
#define BONUS_SIZE 1
#define BONUS_SPEED 2
#define BONUS_LIFE 4
#define BONUS_GLU 8

static const Uint32 FRAMERATE_MILLISECONDS = 200 / 60;
static const int WINDOW_WIDTH = 600;
static const int WINDOW_HEIGHT = 600;
static const unsigned int BIT_PER_PIXEL = 32;
static const int LIFEPOINTS = 6;
static GLuint textureIds[GAME_OVER_TEXT + 1];
static int widthBoardBrick, heightBoardBrick;
static float step;
static int nbBonus;
static unsigned int bonusReleased = 0;
static unsigned int gameIsOver = 0;

static void movePlayer(Player* player) {
	if(player->isPressedLeft && player->entity->obj->x >= -WINDOW_WIDTH/2){
		player->entity->obj->x -= step * player->entity->speedX;
	}
	if(player->isPressedRight && player->entity->obj->x <= WINDOW_WIDTH/2 - player->width){
		player->entity->obj->x += step * player->entity->speedX;
	}
}

static void drawBonusIco(float x, float y, int width, int height, GLuint textureId, float alpha){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);


	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glColor4f(1.0f,1.0f,1.0f, alpha);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
		glTranslatef(x, y, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(0,0);
			glTexCoord2f(1,0);
			glVertex2f(width,0);
			glTexCoord2f(1,1);
			glVertex2f(width,-height);
			glTexCoord2f(0,1);
			glVertex2f(0,-height);
			
		glEnd();
	glPopMatrix();
	glColor4f(1.0f,1.0f,1.0f, 1);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

}

static int isBonusActivated(char activatedBonus, int bonusId) {
	return (activatedBonus & bonusId);
}

static float setAlpha(char activatedBonus, int active) {
	return isBonusActivated(activatedBonus, active) ? 1 : 0.2;
}

/*
  0,0 __________  1,0
     |          |
	 |          |
	 |          |
	 |          |
  	 ------------
  0,1			1,1
  */

static void drawAllBonus(Player * player) {
	int width = 20;
	int height = 20;
	float alpha = 0.5;
	float x;
	float y;

	int test = player->entity->obj->y < 0 ? 1 : -1;

	x = WINDOW_WIDTH/2 - (WINDOW_WIDTH/2) * 0.08;

	y = player->entity->obj->y + test * (height * 1.5);
	alpha = setAlpha(player->activatedBonus, BONUS_SIZE);
	drawBonusIco(x, y, width, height, textureIds[BONUS_TEXT_ICO_SIZE_IDX], alpha);

	y = player->entity->obj->y + test * (height * 2 * 1.5);
	alpha = setAlpha(player->activatedBonus, BONUS_SPEED);
	drawBonusIco(x, y, width, height, textureIds[BONUS_TEXT_ICO_SPEED_IDX], alpha);
	
	y = player->entity->obj->y + test * (height * 3 * 1.5 );
	alpha = setAlpha(player->activatedBonus, BONUS_GLU);
	drawBonusIco(x, y, width, height, textureIds[BONUS_TEXT_ICO_GLU_IDX], alpha);
	
}

static void setBonus(int bonusId, char * activatedBonus) {
	*activatedBonus |= bonusId;
}

static void removeBonus(int bonusId, char * activatedBonus ) {
	*activatedBonus &= (~bonusId);
}

static void removeAllBonus(char * activatedBonus) {
	*activatedBonus =  0;
}

static void drawLife(Player* player, GLuint textureId){

	float size = WINDOW_WIDTH * 0.01;
	float y = player->entity->obj->y;
	float x = -WINDOW_WIDTH/2 + (WINDOW_WIDTH/2) * 0.08;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int i;
	for (i = 0; i < player->lifePoints; i++){
		glPushMatrix();
			glTranslatef(i*size*3 + x, y ,0);
			glBegin(GL_QUADS);
				glTexCoord2f(0,0);
				glVertex2f(-size, size);
				glTexCoord2f(1,0);
				glVertex2f(size,size);
				glTexCoord2f(1,1);
				glVertex2f(size, -size);
				glTexCoord2f(0,1);
				glVertex2f(-size, -size);
				
			glEnd();
		glPopMatrix();
	}
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

/*
  0,0 __________  1,0
     |          |
	 |          |
	 |          |
	 |          |
  	 ------------
  0,1			1,1
  */

static void drawBackground(GLuint textureId){
	float width = WINDOW_WIDTH/2;
	float height = WINDOW_HEIGHT/2;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glPushMatrix();
		glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			glVertex2f(-width, height);
			glTexCoord2f(1,0);
			glVertex2f(width,height);
			glTexCoord2f(1, 1);
			glVertex2f(width, -height);
			glTexCoord2f(0,1);
			glVertex2f(-width, -height);
		glEnd();
	glPopMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

static void resetPlayerConfig(Player * player){
	player->width = WINDOW_WIDTH / 10;
	player->entity->speedX = WINDOW_WIDTH / 5;
}

static void testCollisionBonus(Bonus * bonus, Player * player, Balle * balle, Mix_Chunk * sound){

	if(bonus->id == -1){
		return;
	}

	if((bonus->entity->obj->x > player->entity->obj->x - bonus->radius)
	&& (bonus->entity->obj->x < player->entity->obj->x + player->width + bonus->radius)
	&& (bonus->entity->obj->y > player->entity->obj->y - player->height - bonus->radius) 
	&& (bonus->entity->obj->y < player->entity->obj->y + bonus->radius)) {
		

		switch (bonus->id){
			case BONUS_SIZE :
				if(player->width > WINDOW_WIDTH / 10){
					player->width = WINDOW_WIDTH / 10;
					removeBonus(BONUS_SIZE, &(player->activatedBonus));
				}else{
					player->width = (WINDOW_WIDTH / 10) * 2;
					setBonus(BONUS_SIZE, &(player->activatedBonus) );
				}
				Mix_PlayChannel(1, sound, 0);
				
			break;
			case BONUS_SPEED :
				if(player->entity->speedX > (WINDOW_WIDTH / 5) * 2){
					removeBonus(BONUS_SPEED, &(player->activatedBonus));
					player->entity->speedX = WINDOW_WIDTH / 5; 
				}else{
					player->entity->speedX *= 2;
					setBonus(BONUS_SPEED, &(player->activatedBonus));
				}

				Mix_PlayChannel(1, sound, 0);
			break;
			case BONUS_LIFE:
				player->lifePoints --;
				Mix_PlayChannel(1, sound, 0);
				setBonus(BONUS_LIFE,&(player->activatedBonus));
			break;
			case BONUS_GLU :
				if(balle->player == player){
					balle->glu = 1;
					Mix_PlayChannel(1, sound, 0);
					setBonus(BONUS_GLU, &(player->activatedBonus));
				}
			break;

		}
		bonus->id = -1;
	}
}

static void moveIA(Balle * balle1, Balle * balle2, Player * player,  Mix_Chunk * sound, float step){
	if(balle1 == NULL || balle2 == NULL || player == NULL){
		return;
	}

	Balle * balle = (balle1->entity->obj->y < balle2->entity->obj->y) ? balle2 : balle1;
	if((balle->entity->speedX == 0) && (balle->entity->speedY == 0)){
		launchBalle(balle,  step*30,  step*30);
	}
	
	float deltaX = balle->entity->obj->x - player->entity->obj->x;

	if(
		(deltaX < 0 ) 
	||  (deltaX - player->width > 0))

	{
		if(deltaX < 0){
			player->isPressedLeft = 1;
			player->isPressedRight = 0;
		}else{
			player->isPressedRight = 1;
			player->isPressedLeft = 0;
		}
	}else{
		player->isPressedRight = 0;
		player->isPressedLeft = 0;
	}

	movePlayer(player);
}

static double norme(float Xa, float Xb, float Ya, float Yb){
	return sqrt ((Xb - Xa) * (Xb - Xa) + (Yb - Ya) *(Yb - Ya));
}


static void testCollisionPlayer(Balle * balle, Player * player, Mix_Chunk * sound){

	if((!balle->entity->speedX && !balle->entity->speedY) || balle == NULL){
		return;
	}

	//il faut calculer la distance entre le centre de la balle et chaque segment de brique.
	// si distance < rayonBalle alors COLLISION

	float playerTopLeftX = player->entity->obj->x;
	float playerTopLeftY = player->entity->obj->y;

	float playerTopRightX = player->entity->obj->x + player->width;
	float playerTopRightY = player->entity->obj->y;

	float playerBottomRightX = player->entity->obj->x;
	float playerBottomRightY = player->entity->obj->y - player->height;
	
	float playerBottomLeftX = player->entity->obj->x + player->width;
	float playerBottomLeftY = player->entity->obj->y - player->height;

	float balleX = balle->entity->obj->x;
	float balleY = balle->entity->obj->y;

	int deltaLeft = balle->entity->obj->x - player->entity->obj->x;
	int deltaRight = balle->entity->obj->x - player->entity->obj->x - player->width;

	int deltaTop = balle->entity->obj->y - player->entity->obj->y;
	int deltaBottom = balle->entity->obj->y - player->entity->obj->y + player->height;

	double deltaCornerTopLeft = norme (balleX, playerTopLeftX, balleY, playerTopLeftY );
	double deltaCornerTopRight = norme (balleX, playerTopRightX, balleY, playerTopRightY );
	double deltaCornerBottomRight = norme (balleX, playerBottomRightX, balleY, playerBottomRightY );
	double deltaCornerBottomLeft = norme (balleX, playerBottomLeftX, balleY, playerBottomLeftY );

	if((abs(deltaTop) <= balle->radius || abs(deltaBottom) <= balle->radius) && (deltaLeft >= 0 && deltaRight  <= 0)){
		Mix_PlayChannel(1, sound, 0);

		if(!balle->glu) {
			balle->entity->speedY = -balle->entity->speedY;
		}else{
			balle->entity->speedX = 0;
			balle->entity->speedY = 0;
			balle->entity->obj->x = balle->player->entity->obj->x + balle->player->width/2;
			balle->entity->obj->y = balle->player->entity->obj->y + balle->radius;
		}

		return;
	}

	if((abs(deltaLeft) <= balle->radius || abs(deltaRight) <= balle->radius) && (deltaTop <= 0 && deltaBottom  >= 0)){
		Mix_PlayChannel(1, sound, 0);
		balle->entity->speedX = -balle->entity->speedX;
		return;
	}

	if(deltaCornerTopLeft <= balle->radius || deltaCornerBottomRight <= balle->radius || deltaCornerBottomLeft <= balle->radius || deltaCornerTopRight <= balle->radius ){
		balle->entity->speedX = -balle->entity->speedX;
		balle->entity->speedY = -balle->entity->speedY;
		return;
	}
}


static void testCollisionBrick(Balle * balle, Brick * brick, Bonus ** arrayBonus, Mix_Chunk * sounds[]){
	int bonusId[] = {1,2,4,8};
	int textIndex = 0;
	int indexBonusId = 0;

	if(balle == NULL || brick == NULL || arrayBonus == NULL || brick->strength < 0){
		return;
	}

	float brickTopLeftX = brick->obj->x;
	float brickTopLeftY = brick->obj->y;

	float brickTopRightX = brick->obj->x + brick->width;
	float brickTopRightY = brick->obj->y;

	float brickBottomRightX = brick->obj->x;
	float brickBottomRightY = brick->obj->y - brick->height;
	
	float brickBottomLeftX = brick->obj->x + brick->width;
	float brickBottomLeftY = brick->obj->y - brick->height;

	float balleX = balle->entity->obj->x;
	float balleY = balle->entity->obj->y;

	int deltaLeft = balle->entity->obj->x - brick->obj->x;
	int deltaRight = balle->entity->obj->x - brick->obj->x - brick->width;

	int deltaTop = balle->entity->obj->y - brick->obj->y;
	int deltaBottom = balle->entity->obj->y - brick->obj->y + brick->height;

	double deltaCornerTopLeft = norme (balleX, brickTopLeftX, balleY, brickTopLeftY );
	double deltaCornerTopRight = norme (balleX, brickTopRightX, balleY, brickTopRightY );
	double deltaCornerBottomRight = norme (balleX, brickBottomRightX, balleY, brickBottomRightY );
	double deltaCornerBottomLeft = norme (balleX, brickBottomLeftX, balleY, brickBottomLeftY );


	if((abs(deltaTop) <= balle->radius || abs(deltaBottom) <= balle->radius) && (deltaLeft >= 0 && deltaRight  <= 0)){
		balle->entity->speedY = -balle->entity->speedY;
		brick->strength --;
		brick->textureId = textureIds[brick->strength];
		
		if(brick->strength != -1 ){
			Mix_PlayChannel(2, sounds[1], 0);
		}
			Mix_PlayChannel(1, sounds[0], 0);
	}

	else if((abs(deltaLeft) <= balle->radius || abs(deltaRight) <= balle->radius) && (deltaTop <= 0 && deltaBottom  >= 0)){
		balle->entity->speedX = -balle->entity->speedX;
		brick->strength --;
		brick->textureId = textureIds[brick->strength];

		if(brick->strength != -1 ){
			Mix_PlayChannel(2, sounds[1], 0);
		}
			Mix_PlayChannel(1, sounds[0], 0);

	}

	else if(deltaCornerTopLeft <= balle->radius || deltaCornerBottomRight <= balle->radius || deltaCornerBottomLeft <= balle->radius || deltaCornerTopRight <= balle->radius ){
		balle->entity->speedX = -balle->entity->speedX;
		balle->entity->speedY = -balle->entity->speedY;
		brick->strength --;
		brick->textureId = textureIds[brick->strength];

		if(brick->strength != -1 ){
			Mix_PlayChannel(2, sounds[1], 0);
		}
			Mix_PlayChannel(1, sounds[0], 0);

		printf("collision coins\n");
	}

	if(brick->strength == -1){
		Mix_PlayChannel(3, sounds[2], 0);
		int haveBonus = rand()%10 > 5;

		if(haveBonus && (bonusReleased < nbBonus)){
			indexBonusId = rand()%3;
			switch (bonusId[indexBonusId]){
				case 1 :
					textIndex = textureIds[BONUS_TEXT_ICO_SIZE_IDX];
				break;
				case 2 :
					textIndex = textureIds[BONUS_TEXT_ICO_SPEED_IDX];
				break;
				case 4 :
					textIndex = textureIds[BONUS_TEXT_ICO_LIFESUP_IDX];
				break;
				case 8 :
					textIndex = textureIds[BONUS_TEXT_ICO_GLU_IDX];
				break;
			}

			arrayBonus[bonusReleased] = newBonus (brick->obj->x + brick->width/2, 
								brick->obj->y + brick->height/2, 
								0,
								WINDOW_WIDTH * 0.02, 
								bonusId[indexBonusId],
								textIndex
								);
			if(balle->player->entity->obj->y > 0){
				arrayBonus[bonusReleased]->entity->speedY = 0.30;
			}else{
				arrayBonus[bonusReleased]->entity->speedY = -0.30;
			}
			bonusReleased ++;
		}
	}}


static void testBonusOut(Bonus * bonus, int winH){
	if(bonus == NULL){
		return;
	}

	if(bonus->entity->obj->y > winH/2 || bonus->entity->obj->y < -winH/2){
		bonus->id = -1;
		bonus->entity->speedY = 0;
	}
}

static void testBalleOut(Balle * balle, Player * playerBottom, Player * playerTop,  int winW, int winH, Mix_Chunk * sounds[], int isIa){
	if(balle == NULL || playerBottom == NULL || playerTop == NULL ){
		return;
	}

	int test = 0;
	test = balle->entity->obj->y < -winH/2 ? 1 : 0;
	test = balle->entity->obj->y > winW/2 ? 2 : test;

	if(test > 0){
		balle->entity->speedX = 0;
		balle->entity->speedY = 0;
		balle->entity->obj->x = balle->player->entity->obj->x + balle->player->width/2;
		balle->entity->obj->y = balle->player->entity->obj->y;

		if(balle->player == playerBottom) {
			balle->entity->obj->y += balle->player->height + balle->radius/2;
			resetPlayerConfig(playerBottom);
			removeAllBonus(&(playerBottom->activatedBonus));
			balle->glu = 0;
		}else {
			balle->entity->obj->y -= balle->player->height + balle->radius;
			resetPlayerConfig(playerTop);
			removeAllBonus(&(playerTop->activatedBonus));
			balle->glu = 0;
			if(isIa){
				playerTop->entity->obj->x = 0;
			}

		}
		((test == 1) ? playerBottom : playerTop) -> lifePoints --;
		Mix_PlayChannel(1, sounds[3], 0);

	}

	if(balle->entity->obj->x >= winW/2 - balle->radius || balle->entity->obj->x <= -winW/2 + balle->radius){
		balle->entity->speedX = -balle->entity->speedX;
		Mix_PlayChannel(1, sounds[0], 0);
		return;
	}
}

static Brick ** createBoardGame(char * configFile){
	int i,j,strength;

	j = 0;
	FILE * file = NULL;
	file = fopen(configFile, "r");
	if (NULL == file){
		fprintf(stderr, "impossible d'ouvrir le fichier de configuration\n");
		return 0;
	}

	if(2 != fscanf(file,"%d %d",&widthBoardBrick, &heightBoardBrick)) {
		fprintf(stderr, "Invalid file!\n");
		exit(EXIT_FAILURE);	
	}
	
    Brick ** arrayBrick = (Brick**)calloc (widthBoardBrick * heightBoardBrick, sizeof (Brick*));
    if(NULL == arrayBrick) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
    }
	float widthBrick = WINDOW_WIDTH / widthBoardBrick;
    float heightBrick =  (WINDOW_HEIGHT / heightBoardBrick) * 0.4;
    float centerX = -WINDOW_WIDTH / 2;
	float centerY = WINDOW_HEIGHT * 0.2;
	
    
    for(i = 0; i < widthBoardBrick * heightBoardBrick; i++){
		if(! i % heightBoardBrick){ j++; }
		int x = i % widthBoardBrick;
		int y = i / widthBoardBrick;
		
		if(1 != fscanf(file,"%d",&strength)) {
			fprintf(stderr, "Invalid grid!\n");
			exit(EXIT_FAILURE);
		}
		
		arrayBrick[i] = newBrick(	centerX + x * widthBrick,
									centerY - y * heightBrick,
									widthBrick,
									heightBrick,
									strength,
									textureIds[strength]
									);
	}

	fclose(file);
	return arrayBrick;
}

static Bonus ** createBonusArray(Brick ** arrayBrick, float radius){
    Bonus ** arrayBonus = (Bonus**)calloc (nbBonus, sizeof (Bonus*));
    if(NULL == arrayBonus) {
		fprintf(stderr, "Not enough memory!\n");
		exit(EXIT_FAILURE);
    }
	return arrayBonus;
}


		 

static void destroyObjects(Brick ** arrayBrick, Bonus ** arrayBonus, Player * playertop, Player * playerBottom, Balle * balle1, Balle * balle2){
	int i;

	for(i = 0; i < widthBoardBrick * heightBoardBrick; i++){
		freeBrick(arrayBrick[i]);
	}

	free(arrayBrick);

	for(i = 0; i < bonusReleased; i++){
		freeBonus(arrayBonus[i]);
	}

	free(arrayBonus);

	freeBalle(balle1);
	freeBalle(balle2);

}


int launchGame(char * configFile, char * themePath, int isIa){
	srand(time(NULL));
	/* load le fichier de thèmes pour les textures */
	int i;
	char backMusicPath[100];
	char balleBumpPath[100];
	char brickTouchedPath[100];
	char brickExplodePath[100];
	char lostPath[100];
	
	/* lancement de la fenetre et le jeu*/
    if(-1 == SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
        return EXIT_FAILURE;
    }

	 /* Ouverture d'une fenêtre et création d'un contexte OpenGL */
	if(NULL == SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE)) {
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		return EXIT_FAILURE;
	}

	if(!loadTheme(themePath , textureIds)) {
		return 0;
	}
	SDL_WM_SetCaption("Arkanopong", NULL);

	if(-1 == Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024)) //Initialisation de l'API Mixer
	{
	   printf("%s", Mix_GetError());
	}


	/* audio */
	Mix_AllocateChannels(6);
	int nbSounds = 4;
	Mix_Music * backMusic = NULL;
	Mix_Chunk * mixChunks[nbSounds];

	mixChunks[0] = NULL;
	mixChunks[1] = NULL;
	mixChunks[2] = NULL;
	mixChunks[3] = NULL;


	sprintf(backMusicPath, "%s/%s/backMusic.mp3", themePath, MUSICS_DIR);
	backMusic = Mix_LoadMUS(backMusicPath);

	sprintf(balleBumpPath, "%s/%s/balle_bump.wav", themePath, MUSICS_DIR);
	mixChunks[0] = Mix_LoadWAV(balleBumpPath);
 	Mix_VolumeChunk(mixChunks[0], MIX_MAX_VOLUME);

	sprintf(brickTouchedPath, "%s/%s/brick_str.wav", themePath, MUSICS_DIR);
	mixChunks[1] = Mix_LoadWAV(brickTouchedPath);
 	Mix_VolumeChunk(mixChunks[1], MIX_MAX_VOLUME);

 	sprintf(brickExplodePath, "%s/%s/brick_explode.wav", themePath, MUSICS_DIR);
	mixChunks[2] = Mix_LoadWAV(brickExplodePath);
 	Mix_VolumeChunk(mixChunks[2], MIX_MAX_VOLUME);

 	sprintf(lostPath, "%s/%s/lost.wav", themePath, MUSICS_DIR);
	mixChunks[3] = Mix_LoadWAV(lostPath);
 	Mix_VolumeChunk(mixChunks[3], MIX_MAX_VOLUME);
 	
 	Mix_PlayMusic(backMusic, -1);

	/* de combien de pixel notre objet bouge entre chaque frame */
	step = (float)WINDOW_WIDTH/40000; // a calculer en fonction de la largeur de fenetre

	
	/* creation du niveau */
	float speed = WINDOW_WIDTH / 6; 
	float width =  WINDOW_WIDTH / 10;
	float y = WINDOW_HEIGHT / 2.15;
	float balleRadius = width * 0.2; 
	
	
	// BRIQUES : creation du tableau de briques depuis le fichier de conf
	Brick ** arrayBrick = createBoardGame(configFile);

	// BONUS
	nbBonus = widthBoardBrick * heightBoardBrick * 0.4;
	Bonus ** arrayBonus = createBonusArray(arrayBrick, balleRadius);

	Player* playerBottom = newPlayer(-width/2,-y,speed,1,width,10,LIFEPOINTS, textureIds[PLAYER_BOTTOM_TEXT_IDX]);

	Player* playerTop = newPlayer(-width/2,y,speed,1,width,10,LIFEPOINTS, textureIds[PLAYER_TOP_TEXT_IDX]);
	
	// BALLE
	Balle* balle1 = newBalle(	playerBottom, 
						playerBottom->entity->obj->x + playerBottom->width/2,
						playerBottom->entity->obj->y + playerBottom->height + balleRadius/2,
						0,
						0,
						balleRadius,
						textureIds[BALL_TEXT_IDX]
						);
	
	Balle* balle2  = newBalle(	playerTop, 
						playerTop->entity->obj->x + playerTop->width/2, 
						playerTop->entity->obj->y - playerTop->height - balleRadius * 1.5,
						0,
						0,
						balleRadius,
						textureIds[BALL_TEXT_IDX]);

	reshape(600,600);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();


    int loop = 1;
    while(loop) {
    	Uint32 startTime = SDL_GetTicks();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Calcul du temps écoulé */
	    Uint32 elapsedTime = SDL_GetTicks() - startTime;
	    /* Si trop peu de temps s'est écoulé, on met en pause le programme */
	    if(elapsedTime < FRAMERATE_MILLISECONDS) {
	      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
	    }


		if(!gameIsOver){
		/* détection des collisisons */
		// collisions balle / bords fenetre
			testBalleOut(balle1, playerBottom, playerTop, WINDOW_WIDTH, WINDOW_HEIGHT, mixChunks, isIa);
			testBalleOut(balle2, playerBottom, playerTop, WINDOW_WIDTH, WINDOW_HEIGHT, mixChunks, isIa);



			// collisions balle / joueur
			testCollisionPlayer(balle1, playerBottom, mixChunks[0]);
			testCollisionPlayer(balle1, playerTop, mixChunks[0]);
			testCollisionPlayer(balle2, playerBottom, mixChunks[0]);
			testCollisionPlayer(balle2, playerTop, mixChunks[0]);


			// collisions balle / brique
			for(i = 0; i < widthBoardBrick * heightBoardBrick; i++){
				testCollisionBrick(balle1, arrayBrick[i], arrayBonus, mixChunks);
				testCollisionBrick(balle2, arrayBrick[i], arrayBonus, mixChunks);
			}

			for(i = 0; i < bonusReleased; i++){
				testCollisionBonus(arrayBonus[i], playerTop,balle1, mixChunks[0]);
				testCollisionBonus(arrayBonus[i], playerBottom, balle1, mixChunks[0]);
				testCollisionBonus(arrayBonus[i], playerTop, balle2, mixChunks[0]);
				testCollisionBonus(arrayBonus[i], playerBottom, balle2, mixChunks[0]);
				testBonusOut(arrayBonus[i], WINDOW_HEIGHT);
			}

			// collisions joueur / bords fenetre	+ déplacement du joueur				
			movePlayer(playerBottom);
			if(!isIa){
				movePlayer(playerTop);
			}
			else{
				moveIA(balle1, balle2, playerTop, mixChunks[0], step);
			}
			moveBalle(balle1);
			moveBalle(balle2);

			/* test fin de jeu*/
			if(!playerBottom->lifePoints || !playerTop->lifePoints ){
				gameIsOver = 1;
			}

			drawBackground(textureIds[BACKGROUND_TEXT_IDX]);
			drawPlayer(playerBottom);
			drawPlayer(playerTop);

			drawLife(playerBottom, textureIds[LIFE_TEXT_IDX]);
			drawLife(playerTop, textureIds[LIFE_TEXT_IDX]);

			drawAllBonus(playerBottom);
			drawAllBonus(playerTop);
			
			// il s'agit d'un tableau à une dimension
			for(i = 0; i < widthBoardBrick * heightBoardBrick; i++){
				drawBrick(arrayBrick[i]);
			}

			drawBalle(balle1);
			drawBalle(balle2);

			for(i = 0; i < bonusReleased; i++){
				drawBonus(arrayBonus[i]);
				if(arrayBonus[i]->id != -1){
					moveBonus(arrayBonus[i]);
				}
			}
		}else{
			drawBackground(textureIds[GAME_OVER_TEXT]);
		}

		
		/* Echange du front et du back buffer : mise à jour de la fenêtre */
		SDL_GL_SwapBuffers();
			SDL_Event e;
			while(SDL_PollEvent(&e)){
				if(e.type == SDL_QUIT) {
					loop = 0;
					break;
				}

				switch(e.type) {
					case SDL_KEYDOWN:
						switch (e.key.keysym.sym)
						{
							case 113:
								loop = 0;
							 break;
						//player 1 joue avec <- et ->
							case 276:
								playerBottom->isPressedLeft = 1;
							 break;
							 case 275:
								 playerBottom->isPressedRight = 1;
							 break;
						// player 2 joue avec A et Z
							case 97:
								if(!isIa){
									playerTop->isPressedLeft = 1;
								}
							 break;
							 case 122:
								if(!isIa){
									playerTop->isPressedRight = 1;
								}
							 break;
						//lancement balle joueur 2
							  case 32:
								  if(!isIa){
								  	if(balle2->entity->speedX == 0 && balle2->entity->speedY == 0){
										launchBalle(balle2,  step*30,  -step*30);
								  	}
								  }
							 break;
						//lancement balle joueur 1
							  case 273:
								if(balle1->entity->speedX == 0 && balle1->entity->speedY == 0){
									if(isIa && balle2->entity->speedX == 0 && balle2->entity->speedY == 0){
										launchBalle(balle2,  step*30,  -step*30);
									}
									launchBalle(balle1,  step*30,  step*30);
							  	}
							 break;
							default:
							break;
						}
					break;
					case SDL_KEYUP:
						switch (e.key.keysym.sym)
							{
						//player 1
							case 276:
								playerBottom->isPressedLeft = 0;
							 break;
							 case 275:
								 playerBottom->isPressedRight = 0;
							 break;
						// player 2
							case 97:
								playerTop->isPressedLeft = 0;
							 break;
							 case 122:
								playerTop->isPressedRight = 0;
							 break;
							default:
							break;
						}
					break;
					case SDL_VIDEORESIZE:
						reshape(e.resize.w, e.resize.h);
					default:
					break;
				}
			}
    }
    // Liberation RAM
    glDeleteTextures(GAME_OVER_TEXT, textureIds);
    Mix_FreeMusic(backMusic);
    for (i = 0; i < nbSounds; i++){
    	Mix_FreeChunk(mixChunks[i]);
    }
  	Mix_CloseAudio();
  	destroyObjects(arrayBrick, arrayBonus, playerTop, playerBottom, balle1, balle2);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}



void reshape (int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  SDL_SetVideoMode(w, h, BIT_PER_PIXEL, SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_RESIZABLE);
  glLoadIdentity();
  gluOrtho2D(-300, 300, -300, 300);
  glMatrixMode(GL_MODELVIEW);
}
