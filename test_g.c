#include <stdio.h>
#include <stdlib.h>
#include "math_utils.h"


int main(int argc, char **argv){
	int n = atoi(argv[3])/2;
	int ecart = atoi(argv[2]);
	int max = atoi(argv[1]);
	int g;
	printf("Taille %d \n", n);
	printf("Ecart %d\n", ecart);
	printf("Max %d\n\n", max);

	for(int i= 1-n; i<n; i++){
		for (int j=-n; j<n; j++){
			g = gausse(i, j, ecart, max);
			printf("Pour i =%d et j = %d gausse =%d \n",i,j,g);
		}
	}
	return 0;
}




