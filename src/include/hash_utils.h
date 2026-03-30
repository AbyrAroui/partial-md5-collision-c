#ifndef _HASH_UTILS_H
#define _HASH_UTILS_H
#include <sys/types.h>
#include <stdlib.h>

typedef unsigned char hash;

typedef struct {
	size_t len;
	hash *data;
} __h_file, *h_file;

typedef struct _link_ {
	h_file rec;
	struct _link_ *nxt;
} __link, *linked;

void empile(h_file f, linked *pile);
h_file depile(linked *pile);
void free_pile(linked *pile);
void print_pile(linked pile);

void printh(hash *hash, size_t t);
size_t identite(size_t taille, hash *hash, h_file f, h_file *out);
void read_hfile(char *fpath, h_file *buf);
void write_hfile(char *fpath, h_file buf);
h_file copy_hfile(h_file *out, h_file in);
void free_hfile(h_file f);
h_file buf_hfile(h_file *out, size_t taille, hash b);
void print_hash_table(linked *array_table);

#endif //_HASH_UTILS_H
