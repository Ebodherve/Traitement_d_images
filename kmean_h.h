//Methode de segmentation par l'algorithme kmean
void segmentation_kmean(int **im_o, entet_pgm entete, int k, int n_max_iter, int min_pix, int max_pix);
//Procedure de choix des centres
void choix_centre(int centres[], int nb_centre, int max_pix);
//Fonction de découverte des centres
int argmin_pix(int centres[], int pixel, int n_centres);
//Fonction de calcule de distance entre deux pixel
int distance_pixel(int p1, int p2);
//Procédure de recalcule des centres
void recalcule_centres(int hist[], int classe_clusters[], int centres[], int max_pix, int n_centres);
//Procedure de seuillage avec les k centre
void seuillage_kmean(int **im_o, dimension dim, int seuils[], int k, int max_pix);





