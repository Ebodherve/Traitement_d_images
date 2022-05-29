#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"
#include "math_utils.h"

/* Ce programme permet ce programe prend en entrée une image 
source et un masque puis convolut l'image source par le masque
exemple d'utilisation ./convo im1.pgm masque.pgm r.pgm 00

*/


//Filtrage d'une image avec un type de filtre : 1-moyenneur; 2-gaussien; 3-median.
void filtrage_im(int ** origine, int ** convoluer, int typef, int dim_f, dimension dim_o, float ecartype, int max_g, int bordure);
//Filtrage d'une image avec remplacement des valeur des bordures par une valeur par une consatante 
void filtre_im_bord(int ** origine, int ** masque, int ** convoluer, dimension dim_o, dimension dim_m, int bord);
//Construction d'un filtre moyenneur
int moyenneur(int ** origine, int taille, int x, int y);
//Construction d'un filtre gaussien
int gaussien(int ** gauss, int taille, float ecartype, int max);
//Construction d'un filtre median
int median(int ** origine, int taille, int i,int j);
//filtre normal
int normal(int ** origine, int taille, int i,int j);
//Ajustement de la taille d'un filtre
int ajuste_taille(int nombre);
//calcule d'un pixel avec un filtre
void pixel_filtre(int ** origine, int ** filtre, int ** resultat, int taille_f, int x, int y, int somme);



int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];
	char * type_f = argv[3];
	int val_bord = atoi(argv[4]);
	int dim_f = atoi(argv[5]);
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
	dim_f = ajuste_taille(dim_f);
	if (strstr(type_f, "mo")){
		printf("Filtre moyenneur \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 1, dim_f, entete_im.dim, 1, 1, val_bord);
	}else if(strstr(type_f, "me")){
		printf("Filtre median \n");
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 3, dim_f, entete_im.dim, 1, 1, val_bord);
	}else if(strstr(type_f, "ga")){
		int max;
		float ecart;
		printf("Entrez l'ecart type : \n");
		scanf("%f",&ecart);
		printf("Entrez l'élément maximale : \n");
		scanf("%d",&max);
		filtrage_im(matrice_pixels, matrice_pixels_resultat, 2, dim_f, entete_im.dim, ecart, max, val_bord);
	}

	// Test d'affichage des pixel
	// affiche(matrice_pixels_resultat, entete_res);

	// Conservation du resultat
	strcpy(entete_res.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels_resultat, entete_res, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	libert_image(matrice_pixels_resultat, entete_res.dim.dim1, entete_res.dim.dim2); /**/
	return 0;
}


void filtrage_im(int ** origine, int ** convoluer, int typef, int dim_f, dimension dim_o, float ecartype, int max_g, int bordure){
	int d1_o = dim_o.dim1;
	int d2_o = dim_o.dim2;
	int d_m = dim_f;
	int centre_masquex;
	int centre_masquey;
	int pixel_convolue;
	int variation_i;
	int variation_j;
	int bord = bordure;

	centre_masquex = d_m/2;
	centre_masquey = centre_masquex;

	// construction du filtre en fonction de son type 1-moyenneur; 2-gaussien; 3-median; 4-normal.
	if (typef == 1){
		for (int i=centre_masquex; i<d1_o - centre_masquex; i++){
			for (int j=centre_masquey; j<d2_o - centre_masquey; j++){
				convoluer[i][j] = moyenneur(origine, d_m, i, j);
			}
		}
		remplit_bordures(convoluer, bord, centre_masquex, dim_o);
	}
	else if (typef == 2)
	{
		int ** filtre = init_matrice(d_m, d_m);
		int somme_fg = gaussien(filtre, d_m, ecartype, max_g);
		printf("Somme des pixels : %d \n", somme_fg);
		for (int i=centre_masquex; i<d1_o - centre_masquex; i++){
			for (int j=centre_masquey; j<d2_o - centre_masquey; j++){
				pixel_filtre(origine, filtre, convoluer, d_m, i, j, somme_fg);
				//printf("Annulation du filtre \n");
			}
		}
		remplit_bordures(convoluer, bord, centre_masquex, dim_o);
		free(filtre);
	}
	else if (typef == 3){
		for (int i=centre_masquex; i<d1_o - centre_masquex; i++){
			for (int j=centre_masquey; j<d2_o - centre_masquey; j++){
				convoluer[i][j] = median(origine, d_m, i, j);
			}
		}
		remplit_bordures(convoluer, bord, centre_masquex, dim_o);
	}
}


//ajustement de la taille d'un filtre
int ajuste_taille(int nombre){
	if (nombre<0){
		return ajuste_taille(nombre*-1);
	}
	else if(nombre > 2){
		if (nombre%2==0)
			return nombre-1;
		return nombre;
	}else{
		return 3;
	}
}


//Calcule d'un pixel avec un filte moyenneur
int moyenneur(int ** origine, int taille, int x, int y){
	int t = taille*taille;
	int ecart = taille/2;
	int pixel = 0;
	int xo = x-ecart;
	int yo = y-ecart;

	for(int i=0; i<taille; i++){
		for (int j=0; j<taille; j++){
			pixel += origine[i+xo][j+yo];
		}
	}
	return pixel/t;
}


//Construction d'un filtre gaussien
int gaussien(int ** gauss, int taille, float ecartype, int max){
	int dep = taille/2;
	int val_gausse;
	int somme = 0;
	printf("Debut filtre gaussien \n");
	for (int i=-dep; i<=dep; i++){
		for (int j=-dep; j<=dep; j++){
			val_gausse = gausse(i, j, ecartype, max);
			gauss[i+dep][j+dep] = val_gausse;
			somme += val_gausse;
			printf("Gausse %d %d = %d \n", i+dep, j+dep, val_gausse);
		}
	}
	return somme;
}


// calcule d'un pixel median
int median(int ** origine, int taille, int i,int j){
	const int t = taille*taille;
	int med[t];
	int i_med=0;
	int ecart = taille/2;
	int xo = i - ecart;
	int yo = j - ecart;

	for (int x=0; x<taille; x++){
		for (int y=0; y<taille; y++){
			med[i_med] = origine[x+xo][y+yo];
			i_med++;
		}
	}
	tri_rapide_p(med, 0, t-1);
	i_med = t/2;
	return med[i_med];
}


//Calcule d'un pixel avec un filtre sur une image
void pixel_filtre(int ** origine, int ** filtre, int ** resultat, int taille_f, int x, int y, int somme){
	int pixel = 0;
	int ecart = taille_f/2;
	int xo = x-ecart;
	int yo = y-ecart;
	for (int i=0; i<taille_f; i++){
		for (int j=0; j<taille_f; j++){
			pixel += filtre[i][j]*origine[xo+i][yo+j];
		}
	}
	pixel = pixel/somme;
	resultat[x][y] = pixel;
}


//filtre normal
int normal(int ** origine, int taille, int i,int j){
	return origine[i][j];
}



