#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"


int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];
	int  bord = atoi(argv[3]);
	int  dim_f = atoi(argv[4]);
	/**/

	//image 
	entet_pgm entete_im;
	int ** matrice_pixels;
	//resultat;
	entet_pgm entete_res;
	int ** matrice_pixels_resultat;


	//image d'origine
	construction_entete_pgm(&entete_im, nom_image_entre);
	matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	//resultat
	entete_res = entete_im;
	matrice_pixels_resultat = init_matrice(entete_res.dim.dim1, entete_res.dim.dim2);
	
	int d1 = entete_res.dim.dim1;
	int d2 = entete_res.dim.dim2;
	printf("dim1 : %d, dim2 : %d \n", d1, d2);
	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			if(est_bordure(i,j,entete_res.dim, dim_f)){
				matrice_pixels_resultat[i][j] = bord;
				printf("Nous sommes sur la bordure %d %d \n", i,j);
			}
			else{
				matrice_pixels_resultat[i][j] = matrice_pixels[i][j];
			}
		}
	}
	strcpy(entete_res.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels_resultat, entete_res, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	libert_image(matrice_pixels_resultat, entete_res.dim.dim1, entete_res.dim.dim2);

	return 0;
}







