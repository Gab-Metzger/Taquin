#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXSIZE 4
#define TROU 0

typedef unsigned char Jeu[MAXSIZE][MAXSIZE];

typedef struct t_arbre
{
  Jeu jeu;
  int distance;
  int mt, nt;
  int marque;
  struct t_arbre *fils[4];
} Noeud, *Arbre;


// Fonction d'affichage d'un jeu du Taquin.
// Le 0 correspond à la case vide.

void afficheJeu(Jeu jeu, int h, int l) {
	int i,j;
	for (i = 0; i < h; i=i+1)
	{
		printf("| ");
		for (j = 0; j < l; j=j+1)
		{
			printf("%c | ", jeu[i][j]);
		}
		printf("\n");
	}
}

void initTaquin(Jeu jeu, Jeu ref, int *H, int *L, char *nf) {
	FILE *f = NULL;
	int i,j,h,l;
	char c[3];

	f = fopen(nf,"r");

	if (f == NULL)
	{
		fprintf(stderr, "Erreur ouverture fichier : %s\n", nf);
	}

	fscanf(f,"%d %d",&h,&l);

	// On récupère les données du jeu 
	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			fscanf(f,"%s", c);
			jeu[i][j] = c[0];
		}
	}

	// On récupère les données du jeu de référence
	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			fscanf(f,"%s", c);
			ref[i][j] = c[0];
		}
	}

	fclose(f);
	*H = h;
	*L = l;
	
}

// On inverse le contenu de deux variables en utilisant une variable temporaire. (Passage par adresse)
void inverserValeur(unsigned char *a, unsigned char *b)
{
	unsigned char tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

// Recherche les coordonnées de la lettre val dans le jeu de référance.
void rechercherValJeu(unsigned char val, Jeu ref, int h, int l, int *x, int *y) {
	int i,j;

	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			if (ref[i][j] == val)
			{
				*x = i;
				*y = j;
			}
		}
	}
}

// Calcul la somme des distances des caractères entre le jeu et le jeu de référance.
int distance(Jeu jeu, Jeu ref, int h, int l) {

	int i,j,res,sommeDistance = 0,x,y;

	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			if (jeu[i][j] != TROU)
			{
				if (jeu[i][j] != ref[i][j])
				{
					rechercherValJeu(jeu[i][j],ref,h,l,&x,&y);
					res = abs(x-i) + abs(y-j);
					sommeDistance = sommeDistance + res;
				}
			}
		}
	}
	return sommeDistance;
}

// Change la place du trou en fonction du mouvement possible.
void actionJeu(Arbre a, Noeud noeud, Jeu jeu, Jeu ref, int h, int l) {
	int x,y;
	Noeud *n;

	// On commence par detecter la position du trou (x et y).
	rechercherValJeu(TROU,jeu,h,l,&x,&y);

	// En fonction des coordonnées du trou on effectue les mouvements possibles.
	if (x > 0) {
		// On échange le trou avec le caractère du dessus.
		inverserValeur(&jeu[x][y], &jeu[x-1][y]);
		if (!estDansArbre(a,jeu,h,l)) {
			n = ajoutFilsArbre(noeud,jeu,ref,0,h,l);
		}
		inverserValeur(&jeu[x][y], &jeu[x-1][y]);
	}
	if (x < h-1) {
		// On échange le trou avec le caractère du dessous.
		inverserValeur(&jeu[x][y], &jeu[x+1][y]);
		if (!estDansArbre(a,jeu,h,l)) {
			n = ajoutFilsArbre(noeud,jeu,ref,1,h,l);
		}
		inverserValeur(&jeu[x][y], &jeu[x+1][y]);		
	}
	if (y > 0) {
		// On échange le trou avec le caractère de gauche.
		inverserValeur(&jeu[x][y], &jeu[x][y-1]);
		if (!estDansArbre(a,jeu,h,l)) {
			n = ajoutFilsArbre(noeud,jeu,ref,2,h,l);
		}
		inverserValeur(&jeu[x][y], &jeu[x][y-1]);		
	}
	if (y < l-1) {
		// On échange le trou avec le caractère de droite.
		inverserValeur(&jeu[x][y], &jeu[x][y+1]);
		if (!estDansArbre(a,jeu,h,l)) {
			n = ajoutFilsArbre(noeud,jeu,ref,3,h,l);
		}
		inverserValeur(&jeu[x][y], &jeu[x][y+1]);		
	}
}

////////////////////////
// Gestion de l'arbre //
////////////////////////


Arbre ajoutFilsArbre(Arbre a, Jeu jeu, Jeu ref, int numeroCaseFils, int h, int l) {
	Noeud *newNoeud;
	newNoeud = (Noeud*)malloc(sizeof(Noeud));
	int i,j;

	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			newNoeud->jeu[i][j] = jeu[i][j];
		}
	}
	newNoeud->distance = distance(jeu,ref,h,l);
	newNoeud->marque = 0;

	// Recherche position du TROU.
	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			if (jeu[i][j] == TROU)
			{
				newNoeud->mt = i;
				newNoeud->nt = j;
			}
		}
	}

	// Création des fils NULL
	for (i = 0; i < 4; i=i+1)
	{
		newNoeud->fils[i] = NULL;
	}

	// Ajout du Noeud dans l'arbre à la position du Fils défini et si arbre vide alors création d'un nouveau.
	if (a != NULL)
	{
		a->fils[numeroCaseFils] = newNoeud;	
	}
	else {
		a = newNoeud;
	}	

	return a;
}

// Si le jeu est déjà dans l'arbre alors res = 1, sinon res = 0;
int estDansArbre(Arbre a, Jeu jeu, int h, int l) {
	int res = 0,i;

	if (a == NULL) {
		res = 0;
	}
	else if (distance(jeu,a->jeu,h,l) == 0) {
		res = 1;
	}
	else {
		for (i = 0; i < 4; i=i+1) {
			if (a->fils[i] != NULL) {
				if (res != 1) {
					if (estDansArbre(a->fils[i],jeu,h,l) == 1) {
						res = 1;
					}
				}
			}
		}
	}
	return res;
}

// Recherche de la meilleur config, càd la somme des distances la plus courte en parcourant tout l'arbre.
Noeud *rechercheMeilleurConfig(Arbre a) {
	
}

int main(int argc, char const *argv[])
{
	int h = 3, l = 3;
	Jeu jeuRef,jeu;
	initTaquin(jeu,jeuRef,&h,&l,"jeu.init");
	afficheJeu(jeu,h,l);
	printf("Distance de A : %d\n", distance(jeu,h,l,'A'));
	return 0;
}

