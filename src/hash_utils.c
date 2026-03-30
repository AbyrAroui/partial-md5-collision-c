#include "hash_utils.h"
#include <stdlib.h>
#include <stdio.h>

size_t btos(hash *b) {
	return b[0];
}

void empile(h_file f, linked *pile) {
	linked tmp = malloc(sizeof(__link));
	tmp->rec = f;
	tmp->nxt = *pile;
	*pile = tmp;
}

h_file depile(linked *pile) {
	linked tmp = *pile;
	h_file val = tmp->rec;
	*pile = tmp->nxt;
	free(tmp);
	return val;
}

// Read an entire file in memory
void read_hfile(char *fpath, h_file *buf) {
	FILE *f;
	if ((f = fopen(fpath, "rb")) == NULL) {
		fprintf(stderr, "Le fichier %s n'existe pas\n", fpath);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	*buf = malloc(sizeof(__h_file));
	(*buf)->data = malloc(fsize + 1);
	if(!fread((*buf)->data, fsize, 1, f)) {
		fprintf(stderr, "read error for %s\n", fpath);
		exit(1);
	}
	fclose(f);

	(*buf)->data[fsize] = 0;
	(*buf)->len = fsize;
}

void write_hfile(char *fpath, h_file buf) {
	FILE *fp = fopen(fpath, "w");
	fwrite(buf->data, sizeof(hash), buf->len, fp);
	fclose(fp);
}

void printh(hash *hash, size_t t) {
	for (int i = 0; i < t; ++i) {
		printf("%02x", hash[i]);
	}
}

h_file copy_hfile(h_file *out, h_file in) {
	*out = (h_file)malloc(sizeof(__h_file));
	(*out)->len = in->len;
	(*out)->data = (hash*)calloc((*out)->len, sizeof(hash));

	for (int i = 0; i < in->len; ++i) {
		(*out)->data[i] = in->data[i];
	}

	return *out;
}

void free_hfile(h_file f) {
	free(f->data);
	free(f);
}

void free_pile(linked *pile) {
	linked tmp;
	while(*pile != NULL) {
		tmp = *pile;
		*pile = tmp->nxt;
		free_hfile(tmp->rec);
		free(tmp);
	}
}

void print_pile(linked pile) {
	linked *p = &pile;
	while(*p != NULL) {
		printf("%p -> ", (*p)->rec);
		*p = (*p)->nxt;
	}
	printf("NULL\n");
}

void print_hash_table(linked *array_table) {
	linked *table;
	for (int i = 0; i < 0x100; ++i) {
		table = array_table + i;
		if(*table) {
			printf("0x%x\t| ", i);
			print_pile(*table);
		}
	}
}
