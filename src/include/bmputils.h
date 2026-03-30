#ifndef _BMPUTILS_H
#define _BMPUTILS_H

#include <unistd.h>
#include "hash_utils.h"

size_t hash_to_bitmap(size_t taille, hash *hash, h_file image, h_file *out);

#endif // _BMPUTILS_H
