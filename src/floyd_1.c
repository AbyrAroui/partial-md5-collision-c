#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "floyd.h"
#include "hash_utils.h"
#include "md5_small.h"

size_t identite(size_t taille, hash *h, h_file v, h_file *out){
	*out = (h_file) malloc(sizeof(__h_file));
	(*out)->len = taille;
	(*out)->data = (hash*) calloc(taille, sizeof(hash));

	for (int i = 0; i < taille; i++) {
		(*out)->data[i] = h[i];
	}
	return taille;
}

int main(int argc, char *argv[])
{
	int rd = open("/dev/urandom", O_RDONLY);

	if(argc != 2) {
		fprintf(stderr, "Usage: %s <taille>\n", argv[0]);
		exit(1);
	}

	size_t t = (unsigned long)atol(argv[1]);

	if(t > 128 || t == 0) {
		fprintf(stderr, "La taille doit être comprise entre 1 et 128\n");
		exit(1);
	}

	hash *seed = calloc((t - 1) / 8 + 1, sizeof(hash));
	hash *h_seed = calloc((t - 1) / 8 + 1, sizeof(hash));

	if(!read(rd, seed, (t - 1) / 8 + 1)) {
		fprintf(stderr, "random read error\n");
		exit(1);
	}
	close(rd);

	md5_small(h_seed, (t - 1) / 8 + 1, t, seed, (t - 1) / 8 + 1);

	collision c = floyd((*identite), NULL, NULL, t, h_seed);
	printh(c.original->data, t/8);
	printf("\n");
	printh(c.attaque->data, t/8);
	printf("\n");
	printf("Itérations: %lu\n", c.iter);

	free_hfile(c.original);
	free_hfile(c.attaque);

	free(seed);
	free(h_seed);
    return 0;
}
