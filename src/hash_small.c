#include <stdio.h>
#include <stdlib.h>
#include "md5_small.h"
#include "hash_utils.h"

int main(int argc, char *argv[])
{
	unsigned int hash_size, hash_bytes;
	h_file buf = malloc(sizeof(h_file*));

	if(argc != 3) {
		fprintf(stderr, "Usage: %s <fichier> <taille>\n", argv[0]);
		exit(1);
	}

	hash_size = (unsigned int)atoi(argv[2]);
	hash_bytes = ((hash_size - 1) / 8) + 1;

	read_hfile(argv[1], &buf);
	hash *hash = calloc(hash_bytes, sizeof(unsigned char));

	if(md5_small(hash, hash_bytes, hash_size, buf->data, buf->len) != 0) {
		exit(1);
	}

	printh(hash, hash_bytes);
	printf("\n");
	
	free_hfile(buf);
	free(hash);
	return 0;
}
