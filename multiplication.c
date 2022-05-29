#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

/* Ce programme permet de mumtiplier une image par un nombre
exemple d'utilisation ./mult im1.pgm 0.5
*/


//Cette fonction prend en parametre deux couples de dimensions et retourne 
//un couple de dimensions dont les valeurs sont minimales
void multiplication_img(int ** mat, float coef, dimension dim);

int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	float coef = atof(argv[2]);
	char * nom_image_resultat = argv[3];
	/**/

	//image 
	entet_pgm entete;
	int ** matrice_pixels;

	//image 
	construction_entete_pgm(&entete, nom_image_entre);
	matrice_pixels = init_matrice(entete.dim.dim1, entete.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	/*Application de l'operation de multîplication*/
	multiplication_img(matrice_pixels, coef, entete.dim);

	// Conservation du resultat
	strcpy(entete.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels, entete , nom_image_resultat);

	libert_image(matrice_pixels, entete.dim.dim1, entete.dim.dim2); /**/
	return 0;
}


void multiplication_img(int ** mat, float coef, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;
	int pixel;

	printf("coefficient : %f \n", coef);
	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			//mat[i][j] = min(mat[i][j]*coef, 255);
			pixel = correction_pixel(mat[i][j]*coef);
			mat[i][j] = pixel;
		}
	}
}


