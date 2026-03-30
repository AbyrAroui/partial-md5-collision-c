#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include "brent.h"
#include "md5_small.h"
#include "hash_utils.h"

#define _hash_handler(IMAGE, POSITION)									\
	f(b_taille, (h_ ## POSITION), (IMAGE), &(POSITION));				\
	md5_small((h_ ## POSITION), b_taille, taille, (POSITION)->data, (POSITION)->len); \
	empile((POSITION), &(pile_mem_ ## IMAGE));							\
	c.iter++

linked table_lookup(linked *array_hash, hash* h, size_t t, unsigned int b_t) {
	hash index = h[b_t - 1];
	linked p = array_hash[index];

	if(p == NULL)
		return 0;

	hash *res = (hash*) calloc(b_t, sizeof(hash));

	while(p != NULL) {
		md5_small(res, b_t, t, p->rec->data, p->rec->len);
		if(!memcmp(res, h, b_t))
			break;
		p = p->nxt;
	}

#ifdef _DEBUG
	if(p != NULL){
		printh(h, b_t);
		printf(" ");
		printh(res, b_t);
		printf(" table match! :)\n");
	}
#endif

	free(res);

	return p;
}

collision brent(size_t (*f)(size_t, hash *, h_file, h_file *), h_file original, h_file attaque, size_t taille, hash *seed)
{
	int i;
	unsigned int b_taille = ((taille - 1) / 8) + 1;
	h_file lievre, arbre;
	int power, lam, iter, mu;
	collision c = {NULL, NULL, 0};

	linked array_hash[0x100] = { NULL };
	linked pile_mem_original = NULL;
	linked pile_mem_attaque = NULL;
	linked lookup = NULL;

	hash *h_lievre = (hash*) calloc(b_taille, sizeof(hash));
	hash *h_tortue = (hash*) calloc(b_taille, sizeof(hash));
	hash *h_arbre = (hash*) calloc(b_taille, sizeof(hash));

#ifdef _DEBUG
	printf("PREMIÈRE PHASE\n");
#endif

	for (i = 0; i < b_taille; ++i)
		h_tortue[i] = h_lievre[i] = seed[i];

	_hash_handler(original, lievre);
	empile(lievre, array_hash + h_lievre[b_taille - 1]);

	power = lam = 1;
	while(memcmp(h_tortue, h_lievre, b_taille)) {
		if (power == lam) {
			memcpy(h_tortue, h_lievre, b_taille);
			power *= 2;
			lam = 0;
		}
		_hash_handler(original, lievre);
		empile(lievre, array_hash + h_lievre[b_taille - 1]);
		lam++;
#if _DEBUG > 1
		printf("%d %d\t| ", power, lam);
		printh(h_tortue, b_taille);
		printf(" ");
		printh(h_lievre, b_taille);
		printf("\n");
#endif

	}
#ifdef _DEBUG
	printf("DEUXIÈME PHASE\n");
#endif

	for (i = 0; i < b_taille; ++i)
		h_arbre[i] = h_lievre[i];

	for (i = 0; i < lam; ++i) {
		_hash_handler(original, lievre);
	}

	mu = 0;
	while(memcmp(h_arbre, h_lievre, b_taille)) {
		_hash_handler(original, arbre);
		_hash_handler(original, lievre);
		mu++;
#if _DEBUG > 1
		printf("%d\t", mu);
		printh(h_arbre, b_taille);
		printf(" ");
		printh(h_lievre, b_taille);
		printf("\n");
#endif
	}

#ifdef _DEBUG
	printf("%d %d\n", lam, mu);
#endif

	memcpy(h_arbre, h_lievre, b_taille);
	iter = lam;
	lookup = NULL;

#ifdef _DEBUG
	printf("TROISIÈME PHASE\n");
#endif

	do {
		free_pile(&pile_mem_attaque);
		_hash_handler(original, arbre);

		for (i = 0; i < b_taille; ++i)
			h_lievre[i] = h_tortue[i] = h_arbre[i];

		_hash_handler(attaque, lievre);
		power = lam = 1;

		while(memcmp(h_tortue, h_lievre, b_taille) && !(lookup = table_lookup(array_hash, h_lievre, taille, b_taille))) {
			if (power == lam) {
				memcpy(h_tortue, h_lievre, b_taille);
				power *= 2;
				lam = 0;
			}
			_hash_handler(attaque, lievre);
			lam++;
#if _DEBUG > 2
			printf("%p %d\t| %d %d\t| ", lookup, iter, power, lam);
			printh(h_arbre, b_taille);
			printf(" ");
			printh(h_tortue, b_taille);
			printf(" ");
			printh(h_lievre, b_taille);
			printf("\n");
#endif
		}

#if _DEBUG > 1
		if(!lookup) {
			printf("%d | ", iter);
			printh(h_arbre, b_taille);
			printf(" ");
			printh(h_lievre, b_taille);
			printf("\n");
		}
#endif
		iter--;
	} while(iter > 0 && !lookup);

	if(iter != 0) {
		copy_hfile(&c.original, lookup ? lookup->rec : arbre);
		copy_hfile(&c.attaque, lievre);
	}

	free(h_lievre);
	free(h_tortue);
	free(h_arbre);

#if _DEBUG > 2
	print_hash_table(array_hash);
#endif

	for (i = 0; i < 0x100; ++i)
		while(array_hash[i] != NULL) {
			linked tmp = array_hash[i]->nxt;
			free(array_hash[i]);
			array_hash[i] = tmp;
		}

	free_pile(&pile_mem_original);
	free_pile(&pile_mem_attaque);

	return c;
}
