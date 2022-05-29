#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

/* Ce programme permet d'effectuer des operations logique sur deux images 
exemple d'utilisation ./op_logique im1.pgm et im2.pgm resultat.pgm
*/


//Cette fonction prend en parametre deux couples de dimensions et retourne 
//un couple de dimensions dont les valeurs sont minimales
dimension ajuste_dim(dimension couple1, dimension couple2);
void ou_logique(int ** mat1, int ** mat2, int ** matR, dimension dim); 
void et_logique(int ** mat1, int ** mat2, int ** matR, dimension dim); 
void add_img(int ** mat1, int ** mat2, int ** matR, dimension dim); 
void sous_img(int ** mat1, int ** mat2, int ** matR, dimension dim); 
void multiplication_img(int ** mat, float coef, dimension dim);

int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre1 = argv[1];
	char * op_logique = argv[2];
	char * nom_image_entre2 = argv[3];
	char * nom_image_resultat = argv[4];
	/**/

	//image 1
	entet_pgm entete1;
	int ** matrice_pixels1;
	//image 2
	entet_pgm entete2;
	int ** matrice_pixels2;
	//image resultante
	entet_pgm enteteR;
	int ** matrice_pixelsR;
	dimension dim_resultat;

	/*Initialisation et construction des images 1, 2 et initialisation de l'image resultantes*/
	//image 1
	construction_entete_pgm(&entete1, nom_image_entre1);
	matrice_pixels1 = init_matrice(entete1.dim.dim1, entete1.dim.dim2);
	construction_matrice_pgm(matrice_pixels1, nom_image_entre1);
	//image 2
	construction_entete_pgm(&entete2, nom_image_entre2);
	matrice_pixels2 = init_matrice(entete2.dim.dim1, entete2.dim.dim2);
	construction_matrice_pgm(matrice_pixels2, nom_image_entre2);
	//image resultante
	dim_resultat = ajuste_dim(entete1.dim, entete2.dim);
	matrice_pixelsR = init_matrice(dim_resultat.dim1, dim_resultat.dim2);
	printf("Dimension resultat %d, %d \n", dim_resultat.dim1, dim_resultat.dim2);
	printf("operateur %s \n", op_logique);

	/* Application d'une operation logique*/
	if(strstr(op_logique, "ou"))
		ou_logique(matrice_pixels1, matrice_pixels2, matrice_pixelsR, dim_resultat);
	else if(strstr(op_logique, "et"))
		et_logique(matrice_pixels1, matrice_pixels2, matrice_pixelsR, dim_resultat);
	else if(strstr(op_logique, "+"))
		add_img(matrice_pixels1, matrice_pixels2, matrice_pixelsR, dim_resultat);
	else if(strstr(op_logique, "-"))
		sous_img(matrice_pixels1, matrice_pixels2, matrice_pixelsR, dim_resultat);
	else if (strstr(op_logique, "x")){
		float coef1 = atof(argv[5]);
		float coef2 = atof(argv[6]);
		multiplication_img(matrice_pixels1, coef1, entete1.dim);
		multiplication_img(matrice_pixels2, coef2, entete2.dim);
		add_img(matrice_pixels1, matrice_pixels2, matrice_pixelsR, dim_resultat);
	}
	else
		printf("Entrez un operateur logique valide (et/ou) \nExemple : ./op_logique im1.pgm et im2.pgm resultat.pgm\n");

	// Conservation du resultat
	enteteR.dim = dim_resultat;
	enteteR.encodage_max = entete2.encodage_max;
	strcpy(enteteR.nom_type, entete2.nom_type);
	strcpy(enteteR.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixelsR, enteteR , nom_image_resultat);

	libert_image(matrice_pixels1, entete1.dim.dim1, entete1.dim.dim2);
	libert_image(matrice_pixels2, entete2.dim.dim1, entete2.dim.dim2);
	libert_image(matrice_pixelsR, enteteR.dim.dim1, enteteR.dim.dim2);
	return 0;
}


dimension ajuste_dim(dimension couple1, dimension couple2){
	dimension dim;
	dim.dim1 = min(couple1.dim1, couple2.dim1);
	dim.dim2 = min(couple1.dim2, couple2.dim2);
	return dim;
}

void ou_logique(int ** mat1, int ** mat2, int ** matR, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			matR[i][j] = mat1[i][j] || mat2[i][j];
		}
	}
}

void et_logique(int ** mat1, int ** mat2, int ** matR, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			matR[i][j] = mat1[i][j] && mat2[i][j];
		}
	}
}


void add_img(int ** mat1, int ** mat2, int ** matR, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			matR[i][j] = min(mat1[i][j]+ mat2[i][j], 255);
		}
	}
}

void sous_img(int ** mat1, int ** mat2, int ** matR, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			matR[i][j] = vmax(mat1[i][j] - mat2[i][j], 0);
		}
	}
}


void multiplication_img(int ** mat, float coef, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			//mat[i][j] = min(mat[i][j]*coef, 255);
			mat[i][j] = correction_pixel(mat[i][j]*coef);
		}
	}
}


