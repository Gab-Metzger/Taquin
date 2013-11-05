#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 4

typedef unsigned char Jeu[MAXSIZE][MAXSIZE];

typedef struct t_arbre
{
  Jeu jeu;
  int distance;
  int mt, nt;
  int marque;
  struct t_arbre *fils[4];
} Noeud, *Arbre;

typedef struct coord {
	int x;
	int y;
} Coordonnees;

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

Coordonnees rechercherValJeu(Jeu jeu, int h, int l, char val) {
	int i,j;
	Coordonnees res;

	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			if (jeu[i][j] == val)
			{
				res.x = i;
				res.y = j;
			}
		}
	}

	return res;

}

Coordonnees rechercherValJeuRef(Jeu ref, char val) {
	static char posRef[] = {0,0,0,1,0,2,1,0,1,1,1,2,2,0,2,1,2,2};
	Coordonnees res;

	res.x = posRef[2*(val-'A')];
	res.y = posRef[2*(val-'A')+1];

	return res;
}

int main(int argc, char const *argv[])
{
	int h = 3, l = 3;
	Coordonnees pos;
	Jeu jeuRef,jeu;
	initTaquin(jeu,jeuRef,&h,&l,"jeu.init");
	afficheJeu(jeu,h,l);
	pos = rechercherValJeuRef(jeuRef,'B');
	printf("x : %d - y : %d \n", pos.x, pos.y);
	return 0;
}

