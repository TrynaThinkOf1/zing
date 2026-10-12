/*
 * Zevi Berlin - ZING 2026
 * 
 * Defines the functions necessary for calculating a checksum for the state of the program:
 *    checksum_compare - compare two different sha256 sums
 *    checksum_sha256_compute - walk .zing directory and calculate the sha256 hash
 *    checksum_sha256_read - open the .cs file to read the checksum
 *    checksum_sha256_write - open the .cs file to write a new checksum
 */

#ifndef CRYPTO_CHECKSUM_H
#define CRYPTO_CHECKSUM_H

#include <stdbool.h>

#include "crypto/hashing.h"


/*
 * Literally just goes through like strcmp()
 */
bool checksum_compare(const sha256_Hash* hash1, const sha256_Hash* hash2);

/* 
 * Gets a list of all file paths, goes through each file adding the
 * file's data to an unsigned char* and then hashes that, returning the hash
 * 
 * If this fails, it will print the error and exit
 */
sha256_Hash checksum_sha256_compute();

/* */

/*
 * Reads the .cs file and returns the contained hash
 * 
 * If this fails, it will print the error and exit
 */
sha256_Hash checksum_sha256_read();

/*
 * Writes a sha256 hash to a file to store a checksum
 * 
 * if this fails, it will print the error and exit
 */
void checksum_sha256_write(const sha256_Hash* hash);

#endif /* CRYPTO_CHECKSUM_H */