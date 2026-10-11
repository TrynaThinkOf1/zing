/*
 * Zevi Berlin - ZING 2026
 * 
 * Defines the functions necessary for data compression and decompression:
 *    compress - deflate data using zlib level 4
 *    decompress - reinflate data
 */

#ifndef PYM_PYM_H
#define PYM_PYM_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

#include "mem/zmem.h"
#define CHUNK 16384

typedef struct {
  size_t len;
  unsigned char* data;
} zlib_Data;

/*
 * Takes in a string given by the user and compresses it using ZLIB level 4.
 * 
 * the function will allocate a new string for the compressed data and set
 * return the string and length !!! WHICH THE CALLER MUST FREE USING `z_free` !!!
 * 
 * Should this fail, it prints the error and exits.
 */
zlib_Data zlib_compress_string(char* string);

/*
 * This takes in a string of compressed data, and much like the compress
 * function, it returns the new string and length, again !!! WHICH THE
 * CALLER MUST FREE USING `z_free` !!!
 * 
 * Should this fail, even for a user reason, it will print the error and exit.
 */
zlib_Data zlib_decompress_string(unsigned char* string, size_t len);

#endif /* PYM_PYM_H */
