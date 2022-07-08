#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"
#include "math_utils.h"

//Procedure de seuillage automatique
void seuillage_automatique(int **mat, entet_pgm entete, int seuil, int erreur, int min_pix, int max_pix);
//Fonction de calcule de la moyenne d'un groupe dans un histogramme d'intensités
int moyenne_groupe(int hist[], int debut, int fin);
//Methode de seuillage par l'algorithme d'Otsu
void seuillage_otsu(int **mat, entet_pgm entete, int min_pix, int max_pix);
/*Fonctions nécessaires pour l'algorithme Otsu*/
double probabilite_classe_otsu(int hist[], int taille_im, int debut, int fin);
double probabilite_otsu(int r, int hist[], int taille_im);
double moyenne_otsu(int hist[], int taille_im, int debut, int fin);
double variance_otsu(int hist[], int taille_im, int debut, int fin);

int main(int argc, char **argv){
	if(argc==3){
		/*Lorsque trois informations sont passés en parametre le seuil est déterminé automatiquement 
		par l'algorithme d'Otsu*/
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

		/*Application du seuillage de l'image*/

		//seuillage de l'image
		seuillage_otsu(matrice_pixels, entete_im, 0, 255);

		// Conservation du resultat
		strcpy(entete_im.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
		//La nouvelle valeur du pixel max est 1
		conserve_image(matrice_pixels, entete_im, nom_image_resultat);

		libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/

	}else if(argc==4){
		//Lorsque quatre informations sont passés en parametre il doit s'agir d'un seuillage 
		//dont le seuil est passé en parametre
		/* Recuperation des valeur prises en ligne de commandes */
		char * nom_image_entre = argv[1];
		char * nom_image_resultat = argv[2];
		int seuil = atoi(argv[3]);

		//image 
		entet_pgm entete_im;
		int ** matrice_pixels;

		//image d'origine
		construction_entete_pgm(&entete_im, nom_image_entre);
		matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
		construction_matrice_pgm(matrice_pixels, nom_image_entre);

		/*Application du seuillage de l'image*/
		seuil = valeur_absolue(seuil);

		//seuillage de l'image
		seuillage_binaire(matrice_pixels, entete_im.dim, seuil, 0, 255);

		// Conservation du resultat
		strcpy(entete_im.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
		//La nouvelle valeur du pixel max est 1
		conserve_image(matrice_pixels, entete_im, nom_image_resultat);

		libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
		
	}else if (argc==5){
		//Lorsque cinq informations sont passés en parametre il doit s'agir d'un seuillage automatique
		/* Recuperation des valeur prises en ligne de commandes */
		char * nom_image_entre = argv[1];
		char * nom_image_resultat = argv[2];
		int seuil_depart = atoi(argv[3]);
		int erreur = atoi(argv[4]);

		//image 
		entet_pgm entete_im;
		int ** matrice_pixels;

		//image d'origine
		construction_entete_pgm(&entete_im, nom_image_entre);
		matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
		construction_matrice_pgm(matrice_pixels, nom_image_entre);

		/*Application du seuillage de l'image*/
		seuil_depart = valeur_absolue(seuil_depart);

		//seuillage de l'image
		seuillage_automatique(matrice_pixels, entete_im, seuil_depart, erreur, 0, 255);

		// Conservation du resultat
		strcpy(entete_im.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
		//La nouvelle valeur du pixel max est 1
		conserve_image(matrice_pixels, entete_im, nom_image_resultat);

		libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	}

	return 0;
}


void seuillage_automatique(int **mat, entet_pgm entete, int seuil, int erreur, int min_pix, int max_pix){
	const int taille_hist = max_pix;
	int hist[taille_hist], m_groupe1 = 0, m_groupe2 = 0, seuil1, seuil2;

	//Construction de l'histogramme
	histogramme_im(hist, mat, entete);

	if (seuil < min_pix || seuil > max_pix)
		seuil = (min_pix+max_pix)/2;
	seuil2 = seuil;
	//Calcule du seuill minimale
	do{
		seuil1 = seuil2;
		m_groupe1 = moyenne_groupe(hist, min_pix, seuil1);
		m_groupe1 = moyenne_groupe(hist, seuil1, max_pix);
		seuil2 = (m_groupe1+m_groupe2)/2;
	}while(valeur_absolue(seuil1-seuil2)>erreur);

	//Application du nouveau seuil
	seuil = seuil2;
	printf("Nouveau seuil %d \n", seuil);
	seuillage_binaire(mat, entete.dim, seuil, min_pix, max_pix);
}

int moyenne_groupe(int hist[], int debut, int fin){
	int nombre_total = 0, somme=0;

	for (int i=debut; i<fin; i++){
		somme += i*hist[i];
		nombre_total += hist[i];
	}
	if (nombre_total!=0)
		return somme/nombre_total;
	return 0;
}

//Methode de seuillage par l'algorithme d'Otsu
void seuillage_otsu(int **mat, entet_pgm entete, int min_pix, int max_pix){
	const int taille_hist = max_pix;
	int hist[taille_hist], taille_im = entete.dim.dim1*entete.dim.dim2;
	int seuil;
	double variance_intra[taille_hist];

	//Construction de l'histogramme
	histogramme_im(hist, mat, entete);
	//Calcule des variances de chaque seuil potentiel
	for (int i=min_pix; i<max_pix; i++){
		variance_intra[i] = (double)probabilite_classe_otsu(hist, taille_im, min_pix, i)*variance_otsu(hist, taille_im, min_pix, i);
		variance_intra[i]+= (double)probabilite_classe_otsu(hist, taille_im, i, max_pix)*variance_otsu(hist, taille_im, i, max_pix);
		//printf("Variance %d = %f \n", i, variance_intra[i]);
	}
	seuil = argmin(variance_intra, taille_hist);
	printf("Le seuil meilleur est : %d \n", seuil);
	seuillage_binaire(mat, entete.dim, seuil, min_pix, max_pix);
}

double probabilite_classe_otsu(int hist[], int taille_im, int debut, int fin){
	double prob = 0.0;
	
	for (int i=debut; i<fin; i++)
		prob += (double)hist[i];
	
	return (double)(prob/taille_im);
}

double probabilite_otsu(int r, int hist[], int taille_im){
	return (double)hist[r]/taille_im;
}

double moyenne_otsu(int hist[], int taille_im, int debut, int fin){
	double moyenne = 0.0;
	double prob_classe = probabilite_classe_otsu(hist, taille_im, debut, fin);	

	for (int i=debut; i<fin; i++)
		moyenne += (double)i*probabilite_otsu(i, hist, taille_im);

	if (prob_classe != 0)
		return (double)moyenne/prob_classe;
	return 0.0;
}

double variance_otsu(int hist[], int taille_im, int debut, int fin){
	double moyenne = moyenne_otsu(hist, taille_im, debut, fin), variance = 0;
	double prob_classe = probabilite_classe_otsu(hist, taille_im, debut, fin);

	for (int i=debut; i<fin; i++)
		variance += puissance(i-moyenne, 2)*probabilite_otsu(i, hist, taille_im);

	if (prob_classe != 0)
		return (double)variance/prob_classe;
	return 0.0;
}


