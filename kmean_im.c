#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

//Methode de segmentation par l'algorithme kmean
void segmentation_kmean(int **im_o, entet_pgm entete, int k, int n_max_iter, int min_pix, int max_pix);
//Procedure de choix des centres
void choix_centre(int centres[], int nb_centre, int max_pix);
//Fonction de découverte des centres
int argmin_pix(int centres[], int pixel, int n_centres);
//Fonction de calcule de distance entre deux pixel
int distance_pixel(int p1, int p2);
//Procédure de recalcule des centres
void recalcule_centres(int hist[], int classe_clusters[], int centres[], int max_pix, int n_centres);
//Procedure de seuillage avec les k centre
void seuillage_kmean(int **im_o, dimension dim, int seuils[], int k, int max_pix);


int main(int arc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];
	int k = atoi(argv[3]);

	//image 
	entet_pgm entete_im;
	int ** matrice_pixels;
	
	//image d'origine
	construction_entete_pgm(&entete_im, nom_image_entre);
	matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	//segmentation par l'algorithme k-mean
	segmentation_kmean(matrice_pixels, entete_im, k, 100, 0, entete_im.encodage_max);

	// Conservation du resultat
	strcpy(entete_im.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels, entete_im, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	return 0;
}


void segmentation_kmean(int **im_o, entet_pgm entete, int k, int n_max_iter, int min_pix, int max_pix){
	const int taille_hist = max_pix, taille_clusters=k;
	int hist[taille_hist], classe_clusters[taille_hist], centre_clusters[taille_clusters], centre_clusters1[taille_clusters];

	choix_centre(centre_clusters, k, max_pix);

	histogramme_im(hist, im_o, entete);

	for(int i=0; i<taille_hist; i++){
		classe_clusters[i] = argmin_pix(centre_clusters, i, k);
	}

	for (int i=0; i<n_max_iter; i++){
		//recalcule des centre
		recalcule_centres(hist, classe_clusters, centre_clusters, max_pix, k);

		//Affectation des clusters
		for(int i=0; i<taille_hist; i++)
			classe_clusters[i] = argmin_pix(centre_clusters, i, k);	
	}

	seuillage_kmean(im_o,entete.dim, centre_clusters, k, max_pix);
}


void choix_centre(int centres[], int nb_centre, int max_pix){
	const int taille_tab = max_pix;
	int tab[taille_tab];

	for (int i=0; i<taille_tab; i++)
		tab[i] = i;

	shuffle(tab, taille_tab);

	for(int i=0; i<nb_centre; i++)
		centres[i] = tab[i];
}


//Fonction de découverte des centres
int argmin_pix(int centres[], int pixel, int n_centres){
	int id_min=0;
	for (int i=0; i<n_centres; i++){
		if (distance_pixel(centres[id_min], pixel)>distance_pixel(centres[i], pixel))
			id_min = i;
	}
	return id_min;
}

int distance_pixel(int p1, int p2){
	return valeur_absolue(p1-p2);
}

//Procédure de recalcule des centres
void recalcule_centres(int hist[], int classe_clusters[], int centres[], int max_pix, int n_centres){
	int nv_centre, nb_classe;
	
	for (int i=0; i<n_centres; i++){
		nv_centre = 0;
		nb_classe = 0;
		for (int j=0; j<max_pix; j++){
			if (classe_clusters[j] ==i){
				nv_centre += j*hist[j];
				nb_classe += hist[j];
			}
		}
		if (nb_classe!=0)
			centres[i] = nv_centre/nb_classe;
		else
			centres[i] = nv_centre;
	}

}

//Procedure de seuillage avec les k centre
void seuillage_kmean(int **im_o, dimension dim, int seuils[], int k, int max_pix){
	int i_seuil = 0;

	tri_rapide_p(seuils, 0, k);

	for (int i=0; i<dim.dim1; i++){
		for (int j=0; j<dim.dim2; j++){
			i_seuil = 0;
			while (i_seuil<k && im_o[i][j]>seuils[i_seuil]){
				i_seuil += 1;
			}
			if (i_seuil>=k)
				im_o[i][j] = max_pix;
			else
				im_o[i][j] = seuils[i_seuil];
		}
	}
}


