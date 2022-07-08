#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"


//Methode de filtrage pour la detection de contours
void filtrage_im(int ** origine, int ** convoluer, int typef, int seuil, dimension dim_o, int bordure);
//Methode de binarisation d'une matrice avec d'une matrice avec un seuil
void binarisation(int **mat, dimension dim, int seuil);
//calcule de la nouvelle d'un pixel avec le filtre de Robert
int filtre_derivation_robert(int **mat, int i, int j, int orientation);
//Methode de construction d'un filtre de Prewitt
void filtre_prewitt(int **origine, int **convoluer, int i, int j, int orientation);
//Methode de construction d'un filtre de Sobel
void filtre_sobel(int **origine, int **convoluer, int i, int j, int orientation);
//Methode de construction d'un filtre Laplacien
void filtre_laplacien(int **origine, int **convoluer, int i, int j, int orientation);


int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];
	char * type_f = argv[3];
	int val_bord = atoi(argv[4]);
	int seuil = atoi(argv[5]);
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

	/*Application de convolution*/
	seuil = valeur_absolue(seuil);
	if (strstr(type_f, "robert")){
		printf("Filtre de Robert \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 1, seuil, entete_im.dim, val_bord);
	}else if(strstr(type_f, "prewitt")){
		printf("Filtre de Prewitt \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 2, seuil, entete_im.dim, val_bord);
	}else if(strstr(type_f, "sobel")){
		printf("Filtre de Sobel \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 3, seuil, entete_im.dim, val_bord);
	}else if(strstr(type_f, "laplacien")){
		printf("Filtre laplacien \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 4, seuil, entete_im.dim, val_bord);
	}

	// Conservation du resultat
	strcpy(entete_res.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	//La nouvelle valeur du pixel max est 1
	entete_res.encodage_max = 1;
	conserve_image(matrice_pixels_resultat, entete_res, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	libert_image(matrice_pixels_resultat, entete_res.dim.dim1, entete_res.dim.dim2); /**/
	return 0;
}


void filtrage_im(int ** origine, int ** convoluer, int typef, int seuil, dimension dim_o, int bordure){
	int d1_o = dim_o.dim1;
	int d2_o = dim_o.dim2;
	int pixel_convolue;
	int variation_i;
	int variation_j;
	int bord = bordure;

	// construction du filtre en fonction de son type 1-robert; 2-prewitt; 3-sobel.
	if (typef == 1)
	{
		//Filtre de Robert
		for (int i=0; i<d1_o - 2; i++){
			for (int j=0; j<d2_o - 2; j++){
				// 1- orientation en x
				convoluer[i][j] = filtre_derivation_robert(origine, i, j, 1);
			}
		}
	}
	else if (typef == 2){
		//Filtre de Prewitt
		for (int i=1; i<d1_o - 2; i++){
			for (int j=1; j<d2_o - 2; j++){
				filtre_prewitt(origine, convoluer, i, j, 1);
			}
		}
	}else if (typef == 3){
		//Filtre de Sobel
		for (int i=1; i<d1_o - 2; i++){
			for (int j=1; j<d2_o - 2; j++){
				filtre_sobel(origine, convoluer, i, j, 1);
			}
		}
	}else if (typef == 4){
		//Filtre de Sobel
		for (int i=1; i<d1_o - 2; i++){
			for (int j=1; j<d2_o - 2; j++){
				filtre_laplacien(origine, convoluer, i, j, 1);
		}
	}
	}
	binarisation(convoluer, dim_o, seuil);
}


void binarisation(int **mat, dimension dim, int seuil){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			if (valeur_absolue(mat[i][j])>seuil)
				mat[i][j] = 1;
			else
				mat[i][j] = 0;
		}
	}
}


// calcule de la nouvelle d'un pixel avec le filtre de Robert
int filtre_derivation_robert(int **mat, int i, int j, int orientation){
	// Le orientations du filtre son 1- Pour x, 2- Pour y 
	if (orientation==1)
		return mat[i][j]- mat[i+1][j+1];
	else
		return mat[i][j+1]- mat[i][j+1];
}


//Construction d'un filtre de prewitt
void filtre_prewitt(int **origine, int **convoluer, int i, int j, int orientation){
	if (orientation==1)
		convoluer[i][j] = origine[i+1][j-1]+origine[i+1][j]+origine[i+1][j+1] - origine[i-1][j-1]-origine[i-1][j]-origine[i-1][j+1];
	else
		convoluer[i][j] = origine[i-1][j+1]+origine[i][j+1]+origine[i+1][j+1] - origine[i-1][j-1]+origine[i][j-1]+origine[i+1][j-1];
}


//Methode de construction d'un filtre de Sobel
void filtre_sobel(int **origine, int **convoluer, int i, int j, int orientation){
	if (orientation==1)
		convoluer[i][j] = origine[i+1][j-1]+2*origine[i+1][j]+origine[i+1][j+1] - origine[i-1][j-1]-2*origine[i-1][j]-origine[i-1][j+1];
	else
		convoluer[i][j] = origine[i-1][j+1]+2*origine[i][j+1]+origine[i+1][j+1] - origine[i-1][j-1]-2*origine[i][j-1]-origine[i+1][j-1];
}


//Construction d'un filtre de prewitt
void filtre_laplacien(int **origine, int **convoluer, int i, int j, int orientation){
	if (orientation==1)
		convoluer[i][j] = 4*origine[i][j] -origine[i+1][j-1]-origine[i+1][j+1] - origine[i-1][j-1]-origine[i-1][j+1];
	else
		convoluer[i][j] = 4*origine[i][j] - origine[i-1][j+1] - origine[i+1][j+1] - origine[i-1][j-1] - origine[i+1][j-1];
}

