#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "math_utils.h"


int main(int argc, char **argv){
	float nbr = 4;

	/*printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	nbr = 45;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	nbr = 100;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	nbr = 99;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	nbr = 81;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	nbr = 25;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr)); 
	nbr = 5888;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr)); 
	nbr = 9999;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr)); 
	nbr = 99999999;
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr)); */
	nbr = atof(argv[1]);
	printf("Racine carre de %f = %f \n", nbr, racine_carre(nbr));
	return 0;
}




