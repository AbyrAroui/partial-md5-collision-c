#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include "floyd.h"
#include "md5_small.h"
#include "hash_utils.h"

#define _hash_handler(HASH, IMAGE, POSITION)							\
	free_hfile(POSITION);												\
	f(b_taille, (HASH), (IMAGE), &(POSITION));							\
	md5_small((HASH), b_taille, taille, (POSITION)->data, (POSITION)->len); \
	c.iter++;

collision floyd(size_t (*f)(size_t, hash *, h_file, h_file *), h_file entree, h_file sortie, size_t taille, hash *seed)
{
	unsigned int b_taille = ((taille - 1) / 8) + 1;

	h_file lievre, tortue, arbre;
	collision c = {NULL, NULL, 0};


	hash *h_lievre = (hash*) calloc(b_taille, sizeof(hash));
	hash *h_tortue = (hash*) calloc(b_taille, sizeof(hash));
	hash *h_arbre = (hash*) calloc(b_taille, sizeof(hash));
	hash *h_pierre = (hash*) calloc(b_taille, sizeof(hash));

	for (int i = 0; i < b_taille; ++i)
		h_tortue[i] = h_lievre[i] = seed[i];

#ifdef _DEBUG
	printf("PREMIÈRE PHASE\n");
#endif

	// c'est une itération de moins toute entière
	f(b_taille, h_tortue, entree, &tortue);
	md5_small(h_tortue, b_taille, taille, tortue->data, tortue->len);

	f(b_taille, h_tortue, entree, &lievre);
	md5_small(h_lievre, b_taille, taille, lievre->data, lievre->len);

	c.iter += 2;

#if _DEBUG > 1
	printh(h_tortue, b_taille);
	printf(" ");
	printh(h_lievre, b_taille);
	printf("\n");
#endif

	while(memcmp(h_tortue, h_lievre, b_taille)) {
		_hash_handler(h_tortue, entree, tortue);

		_hash_handler(h_lievre, entree, lievre);
		_hash_handler(h_lievre, entree, lievre);

#if _DEBUG > 1
		printh(h_tortue, b_taille);
		printf(" ");
		printh(h_lievre, b_taille);
		printf("\n");
#endif
	}

	// c'est plus facile.
	arbre = (h_file) malloc(sizeof(__h_file));
	arbre->data = malloc(0);

	for (int i = 0; i < b_taille; ++i)
		h_pierre[i] = h_arbre[i] = h_tortue[i];

#ifdef _DEBUG
	printf("DEUXIÈME PHASE\n");
#endif

	do {
		_hash_handler(h_arbre, entree, arbre);

		for (int i = 0; i < b_taille; ++i)
			h_tortue[i] = h_lievre[i] = h_arbre[i];

		do {
			_hash_handler(h_tortue, sortie, tortue);

			_hash_handler(h_lievre, sortie, lievre);
			_hash_handler(h_lievre, sortie, lievre);

#if _DEBUG > 2
			printh(h_arbre, b_taille);
			printf(" ");
			printh(h_tortue, b_taille);
			printf(" ");
			printh(h_lievre, b_taille);
			printf("\n");
#endif
		} while(memcmp(h_tortue, h_lievre, b_taille));

#if _DEBUG > 1
		printh(h_arbre, b_taille);
		printf(" ");
		printh(h_tortue, b_taille);
		printf("\n");
#endif

	} while(memcmp(h_arbre, h_tortue, b_taille) && memcmp(h_arbre, h_pierre, b_taille));

	if(!memcmp(h_arbre, h_pierre, b_taille)) {
		fprintf(stderr, "Mauvaise seed. Désolé.\n");
		free_hfile(lievre);
		free_hfile(tortue);
		free_hfile(arbre);

		free(h_lievre);
		free(h_tortue);
		free(h_arbre);
		free(h_pierre);

		exit(EX_SOFTWARE);
	}

	free_hfile(lievre);
	c.original = arbre;
	c.attaque = tortue;

	free(h_lievre);
	free(h_tortue);
	free(h_arbre);
	free(h_pierre);

	return c;
}
