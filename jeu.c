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
	int i,j,h,l,c;

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
			fscanf(f,"%d", &c);
			jeu[i][j] = c;
		}
	}

	// On récupère les données du jeu de référence
	for (i = 0; i < h; i=i+1)
	{
		for (j = 0; j < l; j=j+1)
		{
			fscanf(f,"%d", &c);
			ref[i][j] = c;
		}
	}

	fclose(f);
	*H = h;
	*L = l;
	
}

int main(int argc, char const *argv[])
{
	int h = 3, l = 3;
	Jeu jeuRef,jeu;
	initTaquin(jeu,jeuRef,&h,&l,"jeu.init");
	afficheJeu(jeu,h,l);
	return 0;
}

