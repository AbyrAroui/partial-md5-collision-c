#ifndef _BRENT_H
#define _BRENT_H

#include "collision.h"

linked table_lookup(linked *array_hash, hash* h, size_t t, unsigned int b_t);
collision brent(size_t (*f)(size_t, hash *, h_file, h_file *), h_file original, h_file attaque, size_t taille, hash *seed);

#endif // _BRENT_H
