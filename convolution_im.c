#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"

/* Ce programme permet ce programe prend en entrée une image 
source et un masque puis convolut l'image source par le masque
exemple d'utilisation ./convo im1.pgm masque.pgm r.pgm 0
*/

// Cette fonction prend une image d'origine, un masque et fait la convolution de l'image par le masque et stocke le 
// Le resultat dans une nouvelle image convolué 
void convolut_im(int ** origine, int ** masque, int ** convoluer, dimension dim_o, dimension dim_m);
//Convolution avec remplacement de bords par une valeur constante
void convolut_im_bord(int ** origine, int ** masque, int ** convoluer, dimension dim_o, dimension dim_m, int bord);
//fonction qui reajuste les dimension pour une convolution
int reajuste_dim_masque(dimension dim);
//determination du centre d'un masque
dimension centre_masque(dimension dim);


int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_masque = argv[2];
	char * nom_image_resultat = argv[3];
	int val_bord = atoi(argv[4]);
	/**/

	//image 
	entet_pgm entete_im;
	int ** matrice_pixels;
	//masque
	entet_pgm entete_mas;
	int ** matrice_pixels_masque;
	//resultat;
	entet_pgm entete_res;
	int ** matrice_pixels_resultat;


	//image d'origine
	construction_entete_pgm(&entete_im, nom_image_entre);
	matrice_pixels = init_matrice(entete_im.dim.dim1, entete_im.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	//masque
	construction_entete_pgm(&entete_mas, nom_image_masque);
	matrice_pixels_masque = init_matrice(entete_mas.dim.dim1, entete_mas.dim.dim2);
	construction_matrice_pgm(matrice_pixels_masque, nom_image_masque);

	//resultat
	entete_res = entete_im;
	matrice_pixels_resultat = init_matrice(entete_res.dim.dim1, entete_res.dim.dim2);

	/*Application de convolution*/
	if (val_bord < 0)
		convolut_im(matrice_pixels, matrice_pixels_masque, matrice_pixels_resultat, entete_im.dim, entete_mas.dim);
	else
		convolut_im_bord(matrice_pixels, matrice_pixels_masque, matrice_pixels_resultat, entete_im.dim, entete_mas.dim, val_bord);

	// Conservation du resultat
	strcpy(entete_res.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels_resultat, entete_res, nom_image_resultat);

	libert_image(matrice_pixels, entete_im.dim.dim1, entete_im.dim.dim2); /**/
	libert_image(matrice_pixels_masque, entete_mas.dim.dim1, entete_mas.dim.dim2); /**/
	libert_image(matrice_pixels_resultat, entete_res.dim.dim1, entete_res.dim.dim2); /**/
	return 0;
}


void convolut_im(int ** origine, int ** masque, int ** convoluer, dimension dim_o, dimension dim_m){
	int d1_o = dim_o.dim1;
	int d2_o = dim_o.dim2;
	int d_m = reajuste_dim_masque(dim_m);
	int centre_masquex;
	int centre_masquey;
	int pixel_convolue;
	int variation_i;
	int variation_j;
	int somme_pixel_masque = somme_pixels(masque, d_m, d_m);

	dimension centre_m = centre_masque(dim_m);
	centre_masquex = centre_m.dim1;
	centre_masquey = centre_m.dim2;

	for (int i=0; i<d1_o; i++){
		for (int j=0; j<d2_o; j++){
			pixel_convolue = 0;
			for (int k=0; k<d_m; k++){
				for (int l=0; l<d_m; l++){
					variation_i = k - centre_masquex + i;
					variation_j = l - centre_masquey + j;
					if (indice_valide(variation_i, d1_o) && indice_valide(variation_j, d2_o))
						pixel_convolue += origine[variation_i][variation_j]*masque[k][l];
				}
			}
			convoluer[i][j] = pixel_convolue/somme_pixel_masque;
		}
	}
}


int reajuste_dim_masque(dimension dim){
	int d1 = min(dim.dim1, dim.dim2);

	if (d1%2 == 0){
		d1 -= 1;
	}
	
	return d1;
}


dimension centre_masque(dimension dim){
	int demi_distance = reajuste_dim_masque(dim);
	dimension d;
	d.dim1 = demi_distance/2;
	d.dim2 = d.dim1;
	return d;
}


void convolut_im_bord(int ** origine, int ** masque, int ** convoluer, dimension dim_o, dimension dim_m, int bord)
{
	int d1_o = dim_o.dim1;
	int d2_o = dim_o.dim2;
	int d_m = reajuste_dim_masque(dim_m);
	int centre_masquex;
	int centre_masquey;
	int pixel_convolue;
	int variation_i;
	int variation_j;
	int somme_pixel_masque = somme_pixels(masque, d_m, d_m);

	dimension centre_m = centre_masque(dim_m);
	centre_masquex = centre_m.dim1;
	centre_masquey = centre_m.dim2;

	//Calcule des valeurs de pixels dans une image à l'interieur de bordures
	for (int i=0; i < d1_o -d_m; i++){
		for (int j=0; j< d2_o -d_m; j++){
			pixel_convolue = 0;
			for (int k=0; k<d_m; k++){
				for (int l=0; l<d_m; l++){
					pixel_convolue += origine[i+k][j+l]*masque[k][l];
				}
			}
			convoluer[i+centre_masquex][j+centre_masquey] = pixel_convolue/somme_pixel_masque;
		}
	}
	//Remplissage des bordures
	variation_i = d1_o - d_m/2;
	for (int i=0; i < centre_masquex; i++){
		for(int j=0; j<d2_o; j++){
			convoluer[i][j] = bord;
			convoluer[i+variation_i][j] = bord;
		}
	}
	variation_j = d2_o - d_m/2;
	for (int i=0; i < d1_o; i++){
		for(int j=0; j<centre_masquey; j++){
			convoluer[i][j] = bord;
			convoluer[i][j+variation_j] = bord;
		}
	}
}



