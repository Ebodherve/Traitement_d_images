#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rw_im.h"


/* Cette fonction extrait deux nombres séparés 
par des espaces dans une chaine de caractère */
dimension dim_img(char * chaine_dim){
	int i = 0;
	int compte = 1;
	dimension dim;

	const int t_max = strlen(chaine_dim);

	char chaine_dim1[t_max];
	char chaine_dim2[t_max];
	char c1 = chaine_dim[0];

	while(c1 != ' '){
		chaine_dim1[i] = c1;
		c1 = chaine_dim[compte];
		compte += 1;
		i += 1;
	}
	
	dim.dim1 = atoi(chaine_dim1);
	i = 0;

	while( compte < t_max){
		c1 = chaine_dim[compte];
		chaine_dim2[i] = c1;
		compte += 1;
		i += 1;
	}
	dim.dim2 = atoi(chaine_dim2);
	compte = dim.dim1;
	dim.dim1 = dim.dim2;
	dim.dim2 = compte;
	return dim;
}


/* Cette fonction lit les informations du fichier de nom 'nom_image'
Puis elle stocke les pixels de cette image dans une matrice */
void construction_matrice_pgm(int ** image, char* nom_image){
	FILE * im_source = NULL;
	im_source = fopen(nom_image, "r+");
	char chaine[TAILLE_MAX];

	int entete = 0;
	int d1;
	int d2;

	// structure de donnée devant contenir les dimensions de limage
	dimension dim;

	if(im_source != NULL){
		//Lecture de l'entete et extration des dimensions de l'image
		while (fgets(chaine, TAILLE_MAX, im_source) != NULL && entete!=3)
		{
		 if(entete == 2){
				// Lecture des dimension de l'image
				dim = dim_img(chaine);
				d1 = dim.dim1;
				d2 = dim.dim2;
				}
				entete += 1;
		}
		// Boucle de lecture de la valeur des pixels de l'image et stockage
		
		for (int i=0; i<d1; i++){
			for (int j=0; j<d2; j++){
				if (fgets(chaine, TAILLE_MAX, im_source) != NULL)
					image[i][j] = atoi(chaine);
				else
					image[i][j] = 1;
			}
		}
	}
	fclose(im_source);
}


//Extraction de l'entete d'une image pgm
void construction_entete_pgm(entet_pgm * entet_p, char * nom_image){
	FILE * im_source = fopen(nom_image, "r+");
	char chaine[TAILLE_MAX];

	if (im_source != NULL){
		//Type de l'image;
		fgets(entet_p->nom_type, TAILLE_MAX, im_source);
	
		//Createur de l'image;
		fgets(entet_p->createur, TAILLE_MAX, im_source);
	
		//dimension de l'image
		fgets(chaine, TAILLE_MAX, im_source);
		entet_p->dim = dim_img(chaine);
	
		//encodage maximale
		fgets(chaine, TAILLE_MAX, im_source);
		entet_p->encodage_max = atoi(chaine);
	}
}


//Fonction qui libert l'image construit en mémoire
void libert_image(int ** image_p, int d1, int d2){

	for (int i=0; i<d1; i++){
		free(image_p[i]);
	}
}


/*Cette fonction conserve une image construit dans un fichier*/
void conserve_image(int ** image, entet_pgm entete , char * nom_fichier)
{

	FILE * im_destination = fopen(nom_fichier, "w+");
	//image_pgm struct_image = image;
	int d1 = entete.dim.dim1;
	int d2 = entete.dim.dim2;

	/*Ecriture dans l'entete de l'image*/

	// Type de l'image
	fprintf(im_destination, "%s", entete.nom_type);

	// createur de l'image
	fprintf(im_destination, "%s", entete.createur);

	// dimension de l'image
	fprintf(im_destination, "%d %d\n", d2, d1);

	// Valeur maximale d'un pixel dans l'image
	fprintf(im_destination, "%d\n", entete.encodage_max);

	/* Remplissage du contenue de l'image */
	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			fprintf(im_destination, "%d\n",image[i][j]);
		}
	}
	fclose(im_destination);
}


// Fonction qui initialise une image vide 
int ** init_matrice(int d1, int d2){

	int ** image = (int **)malloc(sizeof(int*[d1]));
	for (unsigned i=0; i<d1; i++)
		image[i] = malloc(sizeof(int[d2]));
	return image;
}


// fonction permettant de corriger un pixel qui n'est pas valide 
int correction_pixel(int pixel){
	if (pixel <0)
		return 0;
	if (pixel > 255)
		return 255;
	return pixel;
}


//minimum entre deux entiers
int min(int a, int b){
	if (a<b)
		return a;
	return b;
}


//maximum entre deux entiers
int vmax(int a, int b)
{
	if (a<b)
		return b;
	return a;
}


//Fonction qui teste si un indice est valide
int indice_valide(int indice, int max){
	return (indice >= 0)&&(indice<max);
}


//Fonction qui calcule la somme des pixels d'une image stocké dans une matrice
int somme_pixels(int ** image, int d1, int d2){
	int somme = 0;
	for (int i=0; i < d1; i++){
		for (int j=0; j < d2; j++)
			somme += image[i][j];
	}
	return somme;
}


//Tri de pixels dans un tableau
void tri_pixels(int t_pixel[], int n){
	int cle;
	int j;
	for(int i =1; i < n; i++){
		cle = t_pixel[i];
		j = i-1;

		while(j>-1 && t_pixel[j]>cle){
			t_pixel[j+1] = t_pixel[j];
			j--;
		}
		t_pixel[j+1] = cle;
	}
}


//Affichage des pixels d'une image 
void affiche(int ** image, entet_pgm entete){
	int d1 = entete.dim.dim1;
	int d2 = entete.dim.dim2;

	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			printf(" pixel (%d, %d) = %d \n", i, j,image[i][j]);
		}
	}
}


//Procedure de remplissage des bords d'une image
void remplit_bordures(int ** mat, int bord, int taille_bord, dimension dim_im){
	int t1 = dim_im.dim1;
	int t2 = dim_im.dim2;
	int ecart = t1-taille_bord;
	for (int i=0; i<taille_bord; i++){
		for (int j=0; j<t2; j++){
			mat[i][j] = bord;
			mat[i+ecart][j] = bord;
		}
	}
	ecart = t2-taille_bord;
	for (int i=0; i<t1; i++){
		for (int j=0; j<taille_bord; j++){
			mat[i][j] = bord;
			mat[i][j+ecart] = bord;
		}
	}
}



//Cette procedure compare deux matrice de meme taille 
void compare_mat(int ** mat1, int ** mat2, dimension dim){
	int d1 = dim.dim1, d2 = dim.dim2;
	
	for (int i=0; i<d1; i++){
		for (int j=0; j<d2; j++){
			if (mat1[i][j]!=mat2[i][j]){
				printf("cellule %d %d = %d de mat1 # cellule %d %d = %d de mat2 \n", i, j, mat1[i][j], i, j, mat2[i][j]);
			}
		}
	}
}


//Fonction permettant de tester si des coordonnes 'i', 'j' sont des bordures d'une matrice de dimension 'dim'
// Sachant qu'on y passe un filtre de dimension d1
int est_bordure(int i, int j, dimension dim, int d1){
	int est_bord = 0;
	int dim_f = d1/2;
	int dim1 = dim.dim1;
	int dim2 = dim.dim2;

	est_bord = (dim1-dim_f < i)||(dim2-dim_f < j);
	//(i<=d1)||(j<=d1);
	//||(dim1-dim_f < i)||(dim2-dim_f < j);

	return est_bord;
}

//Tri rapide des nombres d'un tableau
void tri_rapide_p(int t_pixel[], int p, int r){
	if(p<r){
		int q = partition(t_pixel, p, r);
		tri_rapide_p(t_pixel, p, q-1);
		tri_rapide_p(t_pixel, q+1, r);
	}
}

/*Fonctions necessaires pour la realisation d'un tri rapide*/
void permutation(int t[], int i, int j){
	int c = t[i];
	t[i] = t[j];
	t[j] = c;
}

int partition(int t[], int p, int r){
	int x = t[r];
	int i = p-1;

	for (int j=p; j<r; j++){
		if (t[j] < x){
			i += 1;
			permutation(t, i, j);
		}
	}
	permutation(t, i+1, r);

	return i+1;
}

//Fonction qui rend un nombre à la fois paire et strictement positif
int pair_posi(int nbr){
	if (nbr<2){
		return 3; 
	}
	else if(nbr%2 != 0){
		return nbr -1;
	}
	else{
		return nbr;
	}
}


