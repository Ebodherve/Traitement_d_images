
#define TAILLE_MAX 255

struct dimension
{
	int dim1;
	int dim2;
};
typedef struct dimension dimension;

struct image_pgm
{
	//Entete
	char * nom_type;
	char * createur;
	dimension dim;
	int encodage_max;

	//Contenu de l'image
	int ** contenu;
};
typedef struct image_pgm image_pgm;

struct entet_pgm
{
	//Entete
	char nom_type[TAILLE_MAX];
	char createur[TAILLE_MAX];
	dimension dim;
	int encodage_max;

};
typedef struct entet_pgm entet_pgm;

// Cette fonction extrait deux nombres séparés par des espaces dans une chaine de caractère
dimension dim_img(char * chaine_dim);

//Cette fonction lit l'entete d'une image pgm et la stocke dans une stucture entete_pgm
void construction_entete_pgm(entet_pgm * entet_p, char * nom_image); 

//Cette fonction etrait les pixels d'une image puis les stockent dans une matrice
void construction_matrice_pgm(int ** image, char* nom_image);

// liberation de l'espace memoire
void libert_image(int ** image_p, int d1, int d2);

// conservation de l'image dans un nouveau fichier 
void conserve_image(int ** image, entet_pgm entete , char * nom_fichier);

//Initialisation d'une matrice de dimension d1 et d2
int ** init_matrice(int d1, int d2);

//Correction de la valeur d'un pixel
int correction_pixel(int pixel);

//minimum entre deux entiers
int min(int a, int b);

//maximum entre deux entiers
int vmax(int a, int b);

//Fonction qui teste si un indice est valide
int indice_valide(int indice, int max);

//Fonction qui calcule la somme des entiers d'une matrice
int somme_pixels(int ** image, int d1, int d2);

//Tri de pixels dans un tableau
void tri_pixels(int t_pixel[], int n);

//Affichage des pixels d'une image 
void affiche(int ** image, entet_pgm entete);

//Procedure de cremplissage des bordures par une valeur constante
void remplit_bordures(int ** mat, int bord, int taille_bord, dimension dim_im);

//Fonction permettant de tester si des coordonnes 'i', 'j' sont des bordures d'une matrice de dimension 'dim'
// Sachant qu'on y passe un filtre de dimension d1
int est_bordure(int i, int j, dimension dim, int d1);

//Cette procedure compare deux matrice de meme taille 
void compare_mat(int ** mat1, int ** mat2, dimension dim);

//Tri rapide des pixels
void tri_rapide_p(int t_pixel[], int p, int r);

/*Fonctions necessaires pour la realisation d'un tri rapide*/
void permutation(int t[], int i, int j);
int partition(int t[], int p, int r);

//Fonction qui rend un nombre à la fois paire et positif
int pair_posi(int nbr);




