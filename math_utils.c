#include <stdio.h>
#include "math_utils.h"

float racine_carre(float nb){
	float nombre = nb;
	float racine;
	float erreur = 0.0000000001;
	if (nb==1){
		return 1;
	}
	else if (nb>1){
		racine = recherche_racine(1, nb/2, nb, erreur);
		return racine;
	}else{
		nb = 1/nb;
		racine = recherche_racine(1, nb/2, nb, erreur);
		return 1/racine;
	}
}


float recherche_racine(float debut, float fin, float nombre, float erreur){
	float d = debut;
	float f = fin;
	float racine = (d+f)/2;
	float nb = nombre;
	float eval = evalut_racine(racine, nombre);
	int max_iter = nombre*100;

	while (absolut(eval) > erreur && 0<max_iter){
		if (eval < 0){
			d = racine;
		}
		else{
			f = racine;
		}
		racine = (d+f)/2;
		eval = evalut_racine(racine, nombre);
		max_iter -=1;
	}

	return racine;
}


float absolut(float nombre){
	if (nombre >=0)
		return nombre;
	return -1*nombre;
}


float evalut_racine(float x, float nb){
	return x*x - nb;
}


// val exposent puis
float puissance(float val, int puis){
	float v = val;
	int p = puis;
	float resultat = 1;
	if (puis>=0){
		for (int i=0; i < puis; i++)
			resultat *= v; 
		return resultat;
	}
	else{
		return 1/puissance(val, -1*puis);
	}
}


//Generation de valeurs gaussiennes
int gausse(int x, int y, int ecart, int max){
	int puis = -1*(x*x+y*y)/(2*(ecart*ecart));
	float gauss = puissance(2.7, puis);
	gauss = gauss*max;
	return gauss;
}



