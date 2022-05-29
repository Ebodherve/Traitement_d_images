#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "rw_im.h"
#include "math_utils.h"

/* code d'interpolation d'une image :
Formalisme : 
./programme <image d'origine> <image d'arrivé> <type d'interpolation> <valeur d'interpolation>

Exemple d'exécution : ./interpolation im_source.pgm im_result.pgm ppv 2 

*/


/* Interpolation par plus pproche voisin */
void interpolation_ppv(int ** im_o, int ** im_r, dimension dim_o, dimension dim_r, int mult);

/* Interpolation bilinéaire */
void interpolation_bil(int ** im_o, int *** im_r, dimension dim_o, dimension dim_r, int mult);

/* Interpolation bilineaire à l'ordre 1 (cette interpolation bilineaire permet de faire une multiplier l'image simplement par 2)*/
int ** interpolation_bil1(int ** im_o, dimension dim_o);

/* Interpolation bicubique */
void interpolation_bic(int ** im_o, int ** im_r, dimension dim_o, dimension dim_r, int mult);


int main(int argc, char **argv){
	/* Recuperation des valeur prises en ligne de commandes */
	char * nom_image_entre = argv[1];
	char * nom_image_resultat = argv[2];
	char * type_int = argv[3];
	int mult = atoi(argv[4]);

	//image
	entet_pgm entete;
	int ** matrice_pixels;	
	//resultat;
	entet_pgm entete_res;
	int ** matrice_pixels_resultat;

	//image d'origine
	construction_entete_pgm(&entete, nom_image_entre);
	matrice_pixels = init_matrice(entete.dim.dim1, entete.dim.dim2);
	construction_matrice_pgm(matrice_pixels, nom_image_entre);

	if (strstr(type_int, "ppv")){
		//Matrice du resultat
		entete_res = entete;
		entete_res.dim.dim1 = entete.dim.dim1*mult;
		entete_res.dim.dim2 = entete.dim.dim2*mult;
		matrice_pixels_resultat = init_matrice(entete_res.dim.dim1, entete_res.dim.dim2);
		printf("interpolation plus proches voisins \n");
		// interpolation de l'image
		interpolation_ppv(matrice_pixels, matrice_pixels_resultat, entete.dim, entete_res.dim, mult);
	}else if(strstr(type_int, "ibil")){
		// Matrice du resultat
		entete_res = entete;
		entete_res.dim.dim1 = entete.dim.dim1*puissance(2, mult);
		entete_res.dim.dim2 = entete.dim.dim2*puissance(2, mult);
		printf("interpolation bilineaire (L'image est multiplié par 2 à la puissance %d)\n", mult);
		// interpolation de l'image
		interpolation_bil(matrice_pixels, &matrice_pixels_resultat, entete.dim, entete_res.dim, mult);
	}else if(strstr(type_int, "ibic")){
		//interpolation_bic(matrice_pixels, matrice_pixels_resultat, entete.dim, entete_res.dim, mult);
		printf("Interpolation bicubique \n");
	}

	//stockage dans une image resultante
	// Conservation du resultat
	strcpy(entete_res.createur, "# CREATE BY TSANGA EBODE Ulrich Hervé\n");
	conserve_image(matrice_pixels_resultat, entete_res, nom_image_resultat);

	libert_image(matrice_pixels, entete.dim.dim1, entete.dim.dim2); /**/
	libert_image(matrice_pixels_resultat, entete_res.dim.dim1, entete_res.dim.dim2); /**/

	return 0;
}


/* Interpolation par plus pproche voisin */
void interpolation_ppv(int ** im_o, int ** im_r, dimension dim_o, dimension dim_r, int mult){
	int ecart = mult;
	int do1 = dim_o.dim1, do2 = dim_o.dim2, dr1 = dim_r.dim1, dr2 = dim_r.dim2;

	for (int i1=0; i1<do1; i1++){
		for (int j1=0; j1<do2; j1++){
			for (int i2=(i1*ecart); i2<((i1+1)*ecart); i2++){
				for (int j2=(j1*ecart); j2<((j1+1)*ecart); j2++){
					im_r[i2][j2] = im_o[i1][j1];
				} 
			}
		}
	}
}


/* Interpolation bilinéaire */
int ** interpolation_bil1(int ** im_o, dimension dim_o){
	int N = dim_o.dim1, M = dim_o.dim2, ni, nj, nval1, nval2;
	int ** im_r = init_matrice(2*N, 2*M);
	
	//Remplissage des lignes d'indices paires
	for (int i=0; i<2*N-1; i+=2){
		ni = i/2;
		for (int j=0; j<2*M-1; j+=2){
			nj = j/2;
			im_r[i][j] = correction_pixel(im_o[ni][nj]);
		}
		for (int j=1; j<2*M-2; j+=2){
			nj = j/2;
			im_r[i][j] = correction_pixel((im_o[ni][nj]+im_o[ni][nj+1])/2);
		}
		int j = 2*M -1;
		nj = j/2;
		im_r[i][j] = correction_pixel(2*im_o[ni][nj] - im_o[ni][nj-1]);
	}

	//Remplissage des lignes d'indices impaires
	for (int i=1; i<2*N-2; i+=2){
		ni = i/2;
		for (int j=0; j<2*M-1; j+=2){
			nj = j/2;
			nval1 = (im_o[ni][nj]+im_o[ni+1][nj])/2;
			im_r[i][j] = correction_pixel(nval1);
		}
		for (int j=1; j<2*M-2; j+=2){
			nj = j/2;
			nval1 = (im_o[ni][nj]+im_o[ni+1][nj])/2;
			nval2 = (im_o[ni][nj+1]+im_o[ni+1][nj+1])/2;
			im_r[i][j] = correction_pixel((nval1+nval2)/2);
		}
		int j = 2*M -1;
		nj = j/2;
		nval1 = (im_o[ni][nj-1]+im_o[ni+1][nj-1])/2;
		nval2 = (im_o[ni][nj]+im_o[ni+1][nj])/2;
		im_r[i][j] = correction_pixel(2*nval2 - nval1);
	}

	//Remplissage de la derniere ligne
	int i = 2*N-1;
	ni = i/2;
	for (int j=0; j<2*M-1; j+=2){
		nj = j/2;
		nval1 = 2*im_o[ni][nj]+im_o[ni-1][nj];
		im_r[i][j] = correction_pixel(nval1);
	}
	for (int j=1; j<2*M-2; j+=2){
		nj = j/2;
		nval1 = 2*im_o[ni][nj]-im_o[ni-1][nj];
		nval2 = 2*im_o[ni][nj+1]-im_o[ni-1][nj-1];
		im_r[i][j] = correction_pixel((nval1+nval2)/2);
	}
	int j = 2*M -1;
	nj = j/2;
	nval1 = 2*im_o[ni][nj-1]-im_o[ni-1][nj-1];
	nval2 = 2*im_o[ni][nj]-im_o[ni-1][nj]/2;
	im_r[i][j] = correction_pixel(2*nval2 - nval1);

	return im_r;
}

/* Interpolation bilinéaire */
void interpolation_bil(int ** im_o, int *** im_r, dimension dim_o, dimension dim_r, int mult){
	int **im1 , **im2 = interpolation_bil1(im_o, dim_o);
	dimension dim = {dim_o.dim1*2, dim_o.dim2*2};

	for (int i=1; i<mult; i++){
		im1 = im2;
		im1 = interpolation_bil1(im1, dim);
		libert_image(im2, dim.dim1, dim.dim2);
		dim.dim1 = dim.dim1*2; 
		dim.dim2 = dim.dim2*2;		
		im2 = im1;
	}

	*im_r = im2;
}



