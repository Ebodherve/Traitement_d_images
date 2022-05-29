#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "rw_im.h"
#include "math_utils.h"


//Moyenne d'une matrice
float moyenne_mat(int **mat, int d1, int d2);

// Calcule du contraste d'une image 
float contraste_image(int ** image_p, dimension dim);

/*Fonctions d'amélioration du contraste*/
void trans_lineaire(int ** contenu, int min, int max, dimension dim);


int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_fichier_destination = argv[2];
	int min = atoi(argv[3]);
	int max = atoi(argv[4]);
	entet_pgm entete;
	int ** matrice_pixels;
	float contraste;
	float new_contraste;

	//construction de l'entete de l'image 
	construction_entete_pgm(&entete, nom_image_entre);
	printf("Dimensions %d , %d \n", entete.dim.dim1, entete.dim.dim2);
	
	// Initialisation et construction d'une matrice des pixels de l'image
	matrice_pixels = init_matrice(entete.dim.dim1, entete.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	// Calcule du contraste
	contraste = contraste_image(matrice_pixels, entete.dim);

	// Transformation linéraire
	trans_lineaire(matrice_pixels, min, max, entete.dim);

	// Calcule du nouveau contraste
	new_contraste = contraste_image(matrice_pixels, entete.dim);

	// Conservation de l'image construit 
	conserve_image(matrice_pixels, entete , nom_fichier_destination);

	// Liberation en memoire de la matrice de pixels
	libert_image(matrice_pixels, entete.dim.dim1, entete.dim.dim2);
	
	printf("Le contraste de cette de cette image est : %f \n", contraste);
	printf("Le contraste de la nouvelle image est : %f \n", new_contraste);

	return 0;
}


float moyenne_mat(int **mat, int d1, int d2){
	int taille_mat = d1*d2;
	float somme = 0;
	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			somme += mat[i][j];
		}
	}
	return somme/taille_mat;
}


float contraste_image(int ** image_p, dimension dim){
	
	int d1 = dim.dim1;
	int d2 = dim.dim2;
	int ** contenu = image_p;
	int taille_mat = d1*d2;
	float diff;
	float somme = 0;

	float moyenne_pixels = moyenne_mat(contenu, d1, d2);

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			diff = contenu[i][j] - moyenne_pixels;
			somme += puissance(diff, 2);
		}
	}
	
	//return racine_carre(somme/taille_mat);	
	return somme/(taille_mat*10);
}


void trans_lineaire(int ** contenu, int min, int max, dimension dim){
	int d1 = dim.dim1;
	int d2 = dim.dim2;

	for (int i=0; i < d1; i++){
		for (int j=0; j < d2; j++){
			contenu[i][j] = correction_pixel(255*(contenu[i][j]-min)/(max - min));
		}
	}
}



