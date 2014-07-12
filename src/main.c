#define _POSIX_SOURCE
	#include <unistd.h>
#undef _POSIX_SOURCE
// pour listing des dossiers
#include <dirent.h>
#ifndef WIN32
    #include <sys/types.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/game/Game.h"
#define C_MAX 100
#define THEMES_DIR "/usr/share/games/arkanopong/themes/"

int main(int argc, char** argv) {
	if(argc < 2){
		fprintf(stderr, "Nombre d'arguments incorrects. Vous devez inclure un fichier de configuration.");
		return 0;
	}

	/* debug, pour savoir dans quel fichier on est. ici, dans ./ARKANOPONG.
	char cwd[256];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
      perror("getcwd() error");
    else
      printf("ŵd :  %s\n _____ \n", cwd);

	end debug */
	
	
	int nbPlayer;
	int isIa;
	char * configFile;
	
	char theme[C_MAX];
	char themePath[C_MAX];
	configFile = argv[1];
	
	/* le menu */
	printf("---------------------------- \n");
	printf("Bienvenue dans Arkanopong !\n");
	printf("---------------------------- \n");
	
	printf("Vous jouez avec le fichier de niveau : %s\n", configFile);
	
	
	printf("Souhaitez vous jouer seul [1] ou à deux [2] ? \n");
	if(!scanf("%d", &nbPlayer) || nbPlayer > 2 || nbPlayer < 0){
		fprintf(stderr, "Saisie incorrecte\n");
		return 0;
	}
	isIa = (nbPlayer == 1) ? 1 : 0;

	// listing et choix du thème à utiliser 
	
	DIR * dp = NULL;
	struct dirent * readFile = NULL;
    dp = opendir(THEMES_DIR);
    if (NULL == dp){
        fprintf(stderr, "Erreur lors de l'ouverture du repertoires de thèmes.\n");
		return 0;
    }
	printf("--- THEMES DISPONIBLES ---\n");
	while (NULL != (readFile  = readdir(dp))){
		if(strcmp (readFile->d_name, ".") && strcmp (readFile->d_name, "..")){
			printf("\t%s\n", readFile->d_name);
		}
	}
	printf("---------------------------- \n");
	
	printf("Quel thème souhaitez-vous utiliser ? \n");
	if(scanf("%s", theme)){
		//sprintf(theme, "default"); // a suppr
		strcpy (themePath, THEMES_DIR);
		strcat(themePath, theme);
		
		// lancement du jeu
		launchGame(configFile, themePath, isIa);
	}
	free(readFile);
	free(dp);
    return EXIT_SUCCESS;
}
