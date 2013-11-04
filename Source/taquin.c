#include <stdio.h> 
#include <stdlib.h>

#define FOPEN(f,nf,mode)\
  f = fopen(nf,mode);\
  if (f==NULL)\
        {\
          fprintf(stderr,"erreur ouverture %s en lecture\n",nf);\
        }

#define MALLOC(buf,type)\
  buf = (type *)malloc(sizeof(type));\
  if (buf==NULL)\
  {\
         fprintf(stderr,"erreur allocation MALLOC (%s)\n",#buf);\
         exit(-1);\
  }

#define MAXSIZE 4
/* convention 0=TROU */
#define TROU    0
#define MAXINT  MAXSIZE*MAXSIZE*MAXSIZE*MAXSIZE

typedef unsigned char Jeu[MAXSIZE][MAXSIZE];

typedef struct t_arbre
{
	Jeu jeu;
	int distance;
	int mt, nt;
	int marque;
	struct t_arbre *fils[4];
} Noeud, *Arbre;

int nbcomb=0;

void afficheJeu(Jeu jeu, int h, int l)
{
	int i, j;
	
	for(i=0;i<h;i++)
    {
		for(j=0;j<l;j++)
		{
			printf("%2d ",jeu[i][j]);
		}
		printf("\n");
    }
	printf("\n");
}

void initTaquin(Jeu jeu, Jeu ref, int *H, int *L, char *nf)
{
	int h,l,i,j,val;
	FILE *f=NULL;
	
	FOPEN(f, nf, "r");
	fscanf(f,"%d %d",&h, &l);
	for(i=0;i<h;i++)
		for(j=0;j<l;j++)
		{
			fscanf(f,"%d",&val);
			jeu[i][j] = (unsigned char)val;
		}
		
		for(i=0;i<h;i++)
			for(j=0;j<l;j++)
			{
				fscanf(f,"%d",&val);
				ref[i][j] = (unsigned char)val;
			}
			
			fclose(f);
			*H=h;
			*L=l;
}

void chercher(unsigned char val, Jeu ref, int h, int l, int *m, int *n)
{
	int i,j;
	
	for(i=0;i<h;i++)
    {
		for(j=0;j<l;j++)
		{
			if (ref[i][j] == val) 
			{
				*m=i;
				*n=j;
				return;
			}
		}
    }
	return;
}

int valJeu(Jeu jeu, Jeu ref, int h, int l)
{
	int i,j,m,n,sommedist=0,dist;
	
	for(i=0;i<h;i++)
	{
		for(j=0;j<l;j++)
		{
			if (jeu[i][j] != TROU)
			{
				if (jeu[i][j] != ref[i][j])
				{
					chercher(jeu[i][j], ref, h, l, &m, &n);
					dist = abs(m-i) + abs(n-j);
					//fprintf(stdout,"dist=%d\n",dist);
					sommedist += dist;
				}
			}
		}
	}
	//afficheJeu(jeu,h,l);
	//   fprintf(stdout,"val=%d\n",sommedist);
	return sommedist;
}

Noeud *meilleureConfig(Arbre arbreJeu)
{
	int i, aDesFils=0, distmin=MAXINT;
	Noeud *n[4], *res=NULL;
	
	for(i=0;i<4;i++)
		if (arbreJeu->fils[i] != NULL) aDesFils = 1;
		if (aDesFils)
		{
			for(i=0;i<4;i++)
			{
				if (arbreJeu->fils[i] != NULL)
				{
					n[i] = meilleureConfig(arbreJeu->fils[i]);
					if (n[i]!=NULL)
					{
						if (n[i]->distance < distmin)
						{
							distmin = n[i]->distance;
							res = n[i];
						}
					}
				}
			}
		}
		else
		{
			if (arbreJeu->marque == 0) res = arbreJeu;
		}
		return res;
}

int pasDansArbre(Jeu jeu, Arbre arbreJeu, int h, int l)
{
	int i, res=1;
	
	if (arbreJeu == NULL) res = 1;
	else if (valJeu(jeu, arbreJeu->jeu, h, l) == 0) res = 0;
	else
    {
		for(i=0;i<4;i++)
		{
			if (arbreJeu->fils[i] != NULL)
			{
				if (res != 0)
					if (pasDansArbre(jeu, arbreJeu->fils[i], h, l) == 0)
						res = 0;
			}
		}
    }
	return res;
}

void echange(unsigned char *a, unsigned char *b)
{
	unsigned char tmp;
	
	tmp = *a;
	*a = *b;
	*b = tmp;
}

Arbre ajouterFils(Jeu jeu, Jeu ref, Arbre arbre, int numFils, int h, int l)
{
	Noeud *noeud;
	int i, j;
	
	MALLOC(noeud, Noeud);
	for(i=0;i<h;i++)
		for(j=0;j<l;j++)
			noeud->jeu[i][j] = jeu[i][j];
		noeud->distance = valJeu(jeu, ref, h, l);
		noeud->marque=0;
		for(i=0;i<4;i++)
			noeud->fils[i]=NULL;
		if (arbre != NULL)
			arbre->fils[numFils]=noeud;
		else arbre = noeud;
		
		return arbre;
}


void jouer(Jeu ref, int h, int l, Arbre arbreJeu)
{
	Jeu jeu;
	Noeud *noeud, *n, m;
	int i, j, mt, nt, dist;
	
	/* chercher la meilleure configuration */
	noeud = meilleureConfig(arbreJeu);
	if (noeud == NULL) {
		printf("blocage\n");
		fprintf(stdout,"nbcomb = %d\n",nbcomb);
		exit(0);
    	}
	
	noeud->marque = 1;
	nbcomb++;
	for(i=0;i<h;i++)
		for(j=0;j<l;j++)
			jeu[i][j] = noeud->jeu[i][j];
		
	dist = noeud->distance;
	if (nbcomb%100 == 0)fprintf(stderr,"(%d) %d\r",dist,nbcomb);
	/*  afficheJeu(jeu, h, l);*/
		
	if (dist == 0) {
		fprintf(stdout,"(%d) %d\n",dist,nbcomb);
		afficheJeu(jeu, h, l);
		exit (0);
	}
	/* chercher la position du trou */
	chercher(TROU, jeu, h, l, &mt, &nt);
		
	/* tester toutes les possibilités d'action */
	if (mt > 0)    {
		/* on echange le trou et l'élément au dessus */
		echange(&jeu[mt][nt], &jeu[mt-1][nt]);
		if (pasDansArbre(jeu, arbreJeu, h, l))
			n = ajouterFils(jeu, ref, noeud, 0, h, l);
		echange(&jeu[mt][nt], &jeu[mt-1][nt]);
	}
	if (mt < h-1)    {
		/* on echange le trou et l'élément au dessus */
		echange(&jeu[mt][nt], &jeu[mt+1][nt]);
		if (pasDansArbre(jeu, arbreJeu, h, l)) 
			n = ajouterFils(jeu, ref, noeud, 1, h, l);
		echange(&jeu[mt][nt], &jeu[mt+1][nt]);
	}
	if (nt > 0)    {
		/* on echange le trou et l'élément au dessus */
		echange(&jeu[mt][nt], &jeu[mt][nt-1]);
		if (pasDansArbre(jeu, arbreJeu, h, l))
			n = ajouterFils(jeu, ref, noeud, 2, h, l);
		echange(&jeu[mt][nt], &jeu[mt][nt-1]);
	}
	if (nt < l-1)     {
		/* on echange le trou et l'élément au dessus */
		echange(&jeu[mt][nt], &jeu[mt][nt+1]);
		if (pasDansArbre(jeu, arbreJeu, h, l))
			n = ajouterFils(jeu, ref, noeud, 3, h, l);
		echange(&jeu[mt][nt], &jeu[mt][nt+1]);
	}
	jouer(ref, h, l, arbreJeu);
}

int main(int argc, char **argv)
{
	Jeu jeu, ref;
	Arbre arbre;
	int h, l;
	
	if (argc<2) {
		printf("usage = %s <fichier.test>\n",argv[0]);
		exit(1);
	}

	initTaquin(jeu, ref, &h, &l, argv[1]);
	afficheJeu(jeu, h, l);
	afficheJeu(ref, h, l);
	
	arbre = ajouterFils(jeu, ref, NULL, 0, h, l);
	jouer(ref, h, l, arbre);
	
	exit(0);
}







