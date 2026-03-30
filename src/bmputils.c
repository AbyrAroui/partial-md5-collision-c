#include <stdlib.h>
#include <unistd.h>
#include "bmputils.h"
#include "hash_utils.h"


size_t hash_to_bitmap(size_t taille, hash *hash, h_file image, h_file *out) {
	int i;

	copy_hfile(out, image); // optimiser ?

	for (i = 0; i < taille * 8; i++) {
		(*out)->data[(*out)->len - i - 1] ^= (hash[i/8] >> (i%8)) & 1;
	}

	return (*out)->len;
}

