# partial-md5-collision-c
# Partial MD5 Collision Search on BMP Images in C

Academic C project exploring **partial MD5 collisions** using **Floyd** and **Brent** cycle-detection algorithms, with applications to **BMP image transformation**.

## Overview

This project studies how partial hash collisions can be found by combining:

- truncated MD5 hashing
- Floyd’s cycle-finding algorithm
- Brent’s cycle-finding algorithm
- bitmap transformation driven by hash bits

The work starts with collision search on hash values, then extends the approach to bitmap images by modifying low-significance bits in image data. The goal is to generate different image variants that share the same **first `t` bits** of an MD5 hash.

## Project goals

- implement truncated MD5 hashing in C
- use Floyd’s algorithm to detect cycles and search for partial collisions
- adapt the search process from raw hash values to bitmap images
- extend the method to collisions between **two different images**
- improve performance with an optimized Brent-based version

## Main files

- `md5_small.c`  
  Computes and manipulates truncated MD5 hashes.

- `hash_small.c`  
  Prints the first `t` bits of the MD5 hash of a given file.

- `floyd.c`  
  Core Floyd-based collision search implementation.

- `floyd_1.c`  
  Basic collision search on hash values only.

- `floyd_2.c`  
  Applies the collision search process to transformed versions of a single BMP image.

- `floyd_3.c`  
  Extends the method to search for a partial collision between two different BMP images.

- `bmputils.c`  
  Contains the bitmap transformation logic used to convert a hash into a visually close image variant.

- `brent.c`  
  Optimized collision search implementation based on Brent’s algorithm.

- `faussaire.c`  
  Final driver program using the Brent-based optimized approach to generate two output images with the same partial hash prefix.

- `Rapport_Aroui.pdf`  
  Full project report with methodology, diagrams, analysis, and benchmark results.

## How it works

The central idea is to search for two different inputs that produce the same first `t` bits of an MD5 hash.

For bitmap images, the program transforms an image by modifying the least significant bits of the last bytes of the file according to a hash value. This produces visually similar image variants while allowing iterative collision search.

The project evolves in three stages:

1. **Hash-only collision search**  
   Search for partial MD5 collisions on truncated hash outputs.

2. **Single-image adaptation**  
   Transform one BMP image iteratively and search for collisions among its generated variants.

3. **Two-image collision search**  
   Adapt the method so that two different images can lead to the same partial MD5 hash prefix.

A final optimized version replaces the naïve approach with **Brent’s algorithm** and lookup-based improvements.

## Example usage

### 1. Print the first `t` bits of a file hash

```bash
./hash_small <file> <taille>
