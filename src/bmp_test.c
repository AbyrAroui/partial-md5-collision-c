#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "bmputils.h"
#include "hash_utils.h"

int main(int argc, char *argv[])
{
	int rd = open("/dev/urandom", O_RDONLY);
	h_file file, sortie;

	if(argc != 4) {
		fprintf(stderr, "Usage: %s <entree.bmp> <sortie.bmp> <taille>\n", argv[0]);
		exit(1);
	}

	size_t t = (unsigned long)atol(argv[3]);

	hash *seed = calloc((t - 1) / 8 + 1, sizeof(hash));

	if(!read(rd, seed, (t - 1) / 8 + 1)) {
		fprintf(stderr, "read error\n");
		exit(1);
	}
	close(rd);

	read_hfile(argv[1], &file);

	unsigned int b_taille = ((t - 1) / 8) + 1;
	hash_to_bitmap(b_taille, seed, file, &sortie);
	write_hfile(argv[2], sortie);

	free_hfile(file);
	free_hfile(sortie);
	free(seed);

    return 0;
}
