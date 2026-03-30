#ifndef _FLOYD_H
#define _FLOYD_H

#include "collision.h"

collision floyd(size_t (*f)(size_t, hash *, h_file, h_file *), h_file entree, h_file sortie, size_t taille, hash *seed);

#endif // _FLOYD_H
