#include <stdio.h>
#include <stdlib.h>

#include "rw_im.h"


int main(int argc, char **argv){
	int n;
	printf("Taille du tableau : ");
	scanf("%d", &n);
	const int taille = n;
	int tab[taille];

	for (int i=0; i<n; i++){
		printf("Elément d'indice %d : ",i+1);
		scanf("%d", &tab[i]);
	}

	tri_pixels(tab, n);
	printf("\nAffichage dans l'ordre croissant \n\n");
	for (int i=0; i<n; i++)
		printf("Elément d'indice %d : %d\n", i, tab[i]);

	n /= 2;

	printf("\nMediane %d \n", tab[n]);

	return 0;
}



