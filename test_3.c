#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_resultat = argv[1];
	// const int d1 = atoi(argv[2]);
	// const int d2 = atoi(argv[3]);
	/**/

	FILE * im_result = fopen(nom_image_resultat, "w+");

	//int image[10][10];

	fprintf(im_result, "%s\n", "P2");	
	fprintf(im_result, "%s\n", "# CREER PAR TSANGA");	
	fprintf(im_result, "%s\n", "10 10");	
	fprintf(im_result, "%s\n", "255");	
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");
	fprintf(im_result, "%s\n", "0 0 0 0 0 0 0 0 0 0");
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");	
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");	
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");	
	fprintf(im_result, "%s\n", "128 128 128 128 128 128 128 128 128 100");
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");	
	fprintf(im_result, "%s\n", "255 255 255 255 255 255 255 255 255 100");	
	fprintf(im_result, "%s\n", "255 255 255 255 32 32 255 255 255 100");	

	fclose(im_result);

	return 0;
}







