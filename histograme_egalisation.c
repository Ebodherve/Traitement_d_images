#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

/*Ensemble d'opérations nécessaires pour l'égalisation de l'histogramme*/
//Densite de probabilité normalisé
float densite_proba(int i, int hist[], int nbp);
//Methode qui fait l'égalisation des histogrammes
void transformation(int **contenu, entet_pgm entete);


int main(int argc, char **argv){
	/*Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_fichier_destination = argv[2];

	//image 
	entet_pgm entete;
	int ** matrice_pixels;

	//construction de l'entete de l'image 
	construction_entete_pgm(&entete, nom_image_entre);

	// Initialisation et construction d'une matrice des pixels de l'image
	matrice_pixels = init_matrice(entete.dim.dim1, entete.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	// Egalisation des histogrammes
	printf("Debut de l'egalisation \n");
	transformation(matrice_pixels, entete);

	// Conservation de l'image construit 
	conserve_image(matrice_pixels, entete , nom_fichier_destination);

	// Liberation en memoire de la matrice de pixels
	libert_image(matrice_pixels, entete.dim.dim1, entete.dim.dim2);

}


//Densite de probabilité normalisé
float densite_proba(int i, int hist[], int nbp){
	int c=0, pixel;

	for (int j=0; j<=i; j++){
		c += hist[j];
	}
	return (float)c/nbp;
}


//Methode qui fait l'égalisation des histogrammes
void transformation(int **contenu, entet_pgm entete){
	const int taille_hist = entete.encodage_max+1;
	int hist[taille_hist], nbp = entete.dim.dim1*entete.dim.dim2, max_p=entete.encodage_max, pixel;
	
	//Construction de l'histogramme
	histogramme_im(hist, contenu, entete);
	printf("Affichage des valeurs de l'histogramme de l'image \n");
	affiche_tab(hist, taille_hist);
	
	for (int i=0; i<entete.dim.dim1; i++){
		for (int j=0; j<entete.dim.dim2; j++){
			pixel = densite_proba(contenu[i][j], hist, nbp)*max_p;
			contenu[i][j] = correction_pixel(pixel);
		}
	}
	
}



