#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmputils.h"
#include "hash_utils.h"
#include "md5_small.h"
#include "brent.h"

#define MAX_ITER 32

int main(int argc, char *argv[])
{
	int i = 0;
	int rd = open("/dev/urandom", O_RDONLY);
	h_file file;
	h_file out;
	collision c;

	if(argc != 6) {
		fprintf(stderr, "Usage: %s <entree.bmp> <sortie.bmp> <original.bmp> <attaque.bmp> <taille>\n", argv[0]);
		exit(1);
	}

	size_t t = (unsigned long)atol(argv[5]);

	if(t > 128 || t == 0) {
		fprintf(stderr, "La taille doit être comprise entre 1 et 128\n");
		exit(1);
	}

	hash *seed = calloc((t - 1) / 8 + 1, sizeof(hash));
	hash *h_seed = calloc((t - 1) / 8 + 1, sizeof(hash));

	read_hfile(argv[1], &file);
	read_hfile(argv[2], &out);

	do {
		// Générer la seed (sous forme de hash de n bits)
		if(!read(rd, seed, (t - 1) / 8 + 1)) {
			fprintf(stderr, "random read error\n");
			exit(1);
		}
		md5_small(h_seed, (t - 1) / 8 + 1, t, seed, (t - 1) / 8 + 1);

		c = brent((*hash_to_bitmap), file, out, t, h_seed);
		i++;
	} while ((!c.original || !c.attaque) && i < MAX_ITER);  // la pédagogie, c'est l'art de la répétition

	close(rd);

	free_hfile(file);
	free_hfile(out);

	free(seed);
	free(h_seed);

	if(c.original == NULL || c.attaque == NULL) {
		fprintf(stderr, "Seed introuvable.\n");
	}

	write_hfile(argv[3], c.original);
	write_hfile(argv[4], c.attaque);
	printf("Itérations: %lu\n", c.iter);

	free_hfile(c.original);
	free_hfile(c.attaque);
    return 0;
}
