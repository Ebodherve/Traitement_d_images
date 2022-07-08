#include <stdlib.h>
#include <stdio.h>

#define TAILLE_MAX 63


int pixel_valide(int pixel, int max){
	if(pixel<=max && 0<=pixel)
		return 1;
	return 0;
}


int icremente_pixel(int pixel, int ajout, int max){
	int nvpixel = pixel + ajout;
	if (pixel_valide(nvpixel, max))
		{
			return nvpixel;
	}
	else if(nvpixel > max)
		{
			return max;
	}
	else
	{
		return 0;
	}
}


int main(int argc, char **argv)
{
	FILE* im_source = NULL;
	FILE* im_result = NULL;
	char chaine[TAILLE_MAX];

	int taille_en_tete = 3;
	int luminance = atoi(argv[3]);
	unsigned int couleur = 0;

	im_source = fopen(argv[1], "r+");
	im_result = fopen(argv[2], "w+");
	if (im_source != NULL)
	{
		while (fgets(chaine, TAILLE_MAX, im_source) != NULL && taille_en_tete!=0)
		{
			fprintf(im_result, "%s", chaine);
			taille_en_tete-=1;
			}
			fprintf(im_result, "%s", chaine);
		while (fgets(chaine, TAILLE_MAX, im_source) != NULL)
		{
			couleur =  atoi(chaine);
			couleur = icremente_pixel(couleur, luminance, 255);
			fprintf(im_result, "%d\n", couleur);
			}
		fclose(im_result);
		fclose(im_source);
	}
	else{
		printf("Le fichier passé en parametre n'est pas valide.\n");
	}
	return 0;
}


