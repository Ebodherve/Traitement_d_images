#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

void inverse_pix(int **mat, dimension dim, int max_pix);


int main(int arc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];

	//image 
	entet_pgm entete_im;
	int ** matrice_pixels;

	//image d'origine
	construction_entete_pgm(&entete_im, nom_image_entre);
	matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	//Procedure d'inversion des pixels de l'image
	inverse_pix(matrice_pixels, entete_im.dim, entete_im.encodage_max);

	// Conservation du resultat
	strcpy(entete_im.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels, entete_im, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	return 0;
}


void inverse_pix(int **mat, dimension dim, int max_pix){

	for (int i=0; i<dim.dim1; i++){
		for (int j=0; j<dim.dim2; j++){
			if (mat[i][j]==0)
				mat[i][j] = max_pix;
			else
				mat[i][j] = 0;
		}
	}
}

