Project IPI C - Taquin
======================

Résolution de jeu du Taquin  
Créer par Gabriel METZGER  
A rendre le 15 Novembre 2013  

Fonctions de base
=================

##Fonction d'initialisation##

- initTaquin :
	- Paramètres : Jeu (Jeu), Ref (Jeu), hauteur (ptr sur integer), largeur (ptr sur integer), nomFichier (string).
	- Résultat : Ne retourne rien (void).

##Fonction d'affichage##

- afficheJeu
	- Paramètres : Jeu (jeu), hauteur (integer), largeur (integer).
	- Résultat : Ne retourne rien (void).

##Fonctions de gestion de l'arbre##

- ajoutFilsArbre : 
	- Paramètres: arbre (Arbre), noeud (ptr sur Noeud), jeu (Jeu), ref (Jeu), numeroCaseFils (integer), hauteur (integer), largeur (integer).
	- Résultat : arbre (Arbre).

- pasDansArbre :
	- Paramètres : arbre (Arbre), jeu (Jeu), hauteur (integer), largeur (integer).
	- Résultat : booléen (0 si le jeu existe dans l'arbre et 1 sinon).

- inverserValeur : 
	- Paramètres : a (ptr sur unsigned char), b (ptr sur unsigned char).
	- Résultat : Ne retourne rien (void).

##Fonctions de déroulement du jeu##

- rechercherValJeu : 
	- Paramètres : val (unsigned char), ref (Jeu), hauteur (integer), largeur (integer), x (ptr sur integer), y (ptr sur integer).
	- Résultat : Ne retourne rien.

- distance :
	- Paramètres : jeu (Jeu), ref (Jeu), hauteur (integer), largeur (integer).
	- Résultat : Retourne un entier qui correspond à la somme des distances.

- rechercheMeilleurConfig : 
	- Paramètres : arbre (Arbre).
	- Résultat : noeud (ptr sur Noeud).

- jouer : 
	- Paramètres : ref (Jeu), hauteur (integer), largeur (integer), arbre (Arbre).
	- Résultat : Ne retourne rien.


Utilisation
===========

- On commence tout d'abord par écrire le Taquin dans le fichier jeu.init.
Exemple : 

3 3   
A B C  
D F E  
G 0 H  

A B C  
D E F  
G H 0  

Où le caractère 0 correspond au trou.

- Pour résoudre le jeu, il suffit de compiler le programme puis de l'executer grâce aux commandes suivantes : 

Compilation du programme : 
--------------------------
	gcc -Wall jeu.c -o Jeu
--------------------------

Execution du programme : 
---------
	./Jeu
---------

