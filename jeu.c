#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char **Jeu;

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
	FILE *f;
	int i;
	char buffer[100];
	
	// On ouvre le fichier qui contient le jeu de référence et le jeu.
	f = fopen(nf, "rb");
	if(f == NULL) {
		fprintf(stderr,"erreur ouverture %s en lecture\n",nf);
		exit(1);
	}

	// On lit la première ligne
	fgets(buffer,100,f);

	// On cherche la valeur du jeu de référence
	if(strncmp(buffer, "Ref",2) != 0) {
		fprintf(stderr, "format de fichier non reconnu\n");
		exit(2);
	}
	else {
		fgets(buffer, 100, f);
		ref = (unsigned char **)calloc(*H,sizeof(unsigned char *));

		if(ref == NULL) {
		fprintf(stderr, "echec allocation ref\n");
		exit(3);
		}

		for (i = 0; i < *H; i=i+1)
		{
			ref[i] = (unsigned char *)calloc(*L,sizeof(unsigned char));
			if (ref[i] == NULL)
			{
				fprintf(stderr, "echec allocation ref[%d]\n",i);
				exit(4);
			}
		fread(ref[i],*L,sizeof(unsigned char),f);
		}

		fgets(buffer, 100, f);
		if(strncmp(buffer, "Jeu",3) != 0) {
			fprintf(stderr, "format de fichier 2 non reconnu\n");
			exit(2);
		}
		else {
			fgets(buffer, 100, f);
			jeu = (unsigned char **)calloc(*H,sizeof(unsigned char *));

			if(jeu == NULL) {
				fprintf(stderr, "echec allocation jeu\n");
				exit(3);
			}

			for (i = 0; i < *H; i=i+1)
			{
				jeu[i] = (unsigned char *)calloc(*L,sizeof(unsigned char));
				if (jeu[i] == NULL)
				{
					fprintf(stderr, "echec allocation jeu[%d]\n",i);
					exit(4);
				}
			fread(jeu[i],*L,sizeof(unsigned char),f);
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	int h = 3, l = 3;
	Jeu jeuRef = NULL, jeu = NULL;
	initTaquin(jeu,jeuRef,&h,&l,"jeu.init");
	afficheJeu(jeu,h,l);
	return 0;
}

