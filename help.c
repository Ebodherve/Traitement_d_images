#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char ** argv){
	if (argc==1){
		printf("./help contraste : pour l'aide sur le contraste \n");
		printf("./help convo : pour l'aide sur la convolution \n");
		printf("./help dcontour : pour l'aide sur la detection des contours \n");
		printf("./help egalisation : pour l'aide sur l'egalisation des histogramme \n");
		printf("./help filtre_m : pour l'aide sur le filtrage des images\n");
		printf("./help interpolation : pour l'aide sur l'interpolation\n");
		printf("./help luminance : pour l'aide sur la luminance\n");
		printf("./help op : pour l'aide sur les operateur\n");
		printf("./help seuil : pour l'aide sur le seuillage\n");
		printf("./help inverse : pour l'inversion d'une image\n");
		printf("./help kmean_im : pour l'execution de kmean sur une image\n");
		printf("\n");
	}else{
		char * type_help = argv[1];
	if (strstr(type_help, "contraste")){
		//Aide sur le contraste
		printf("./contraste nom_imageO.pgm nom_imageR.pgm min max \n");
	}else if (strstr(type_help, "convo")){
		//Aide sur la convolution
		printf("./convo nom_imageO.pgm nom_masque.pgm nom_imageR.pgm valeur_bordure\n");
	}else if (strstr(type_help, "dcontour")){
		//Aide sur la detection des contours
		printf("./dcontour nom_imageO.pgm nom_imageR.pgm type_de_filtre_derivateur valeur_bordure seuil\n");
	}else if (strstr(type_help, "egalisation")){
		//Aide sur l'egalisation d'histogramme
		printf("./egalisation nom_imageO.pgm nom_imageR.pgm\n");
	}else if (strstr(type_help, "filtre_m")){
		//Aide sur le filtrage d'une image
		printf("./filtre_m nom_imageO.pgm nom_imageR.pgm type_de_filtre valeur_des_bord dimension_filtre\n");
	}else if (strstr(type_help, "interpolation")){
		//Aide sur l'interpolation
		printf("./interpolation nom_imageO.pgm nom_imageR.pgm type_dinterpolation \n");
	}else if (strstr(type_help, "luminance")){
		//Aide sur la luminance
		printf("./luminance nom_imageO.pgm nom_imageR.pgm valeur_daugmentation\n");
	}else if (strstr(type_help, "seuil")){
		//Aide sur contraste
		printf("Pour un seuillage binaire automatique avec determination du meilleur seuil (Avec l'algorithme d'Otsu)\n");
		printf("./seuil nom_imageO.pgm nom_imageR.pgm \n");
		printf("Pour un seuillage binaire simple par un seuil passé en parametre \n");
		printf("./seuil nom_imageO.pgm nom_imageR.pgm seuil\n");
		printf("Pour un seuillage binaire automatique avec determination du meilleur seuil \n");
		printf("./seuil nom_imageO.pgm nom_imageR.pgm seuil erreur\n");
	}else if (strstr(type_help, "inverse")){
		//Aide sur l'inversion des pixels d'une image
		printf("./inverse nom_imageO.pgm nom_imageR.pgm\n");
	}else if (strstr(type_help, "kmean_im")){
		//Aide sur l'algorithme kmean
		printf("./kmean_im nom_imageO.pgm nom_imageR.pgm k\n");
	}else if (strstr(type_help, "mo")){
		//----
	}else if (strstr(type_help, "mo")){
		//----
	}else if (strstr(type_help, "mo")){
		//----
	}
	}
	return 0;
}



