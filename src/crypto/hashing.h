/*
 * Zevi Berlin - ZING 2026
 *
 * Define all functions and types relating to hashing for the ZING program.
 * This includes:
 *    struct sha256_Hash - contains the pair of the raw data and the hashed digest
 *    sha256_hash_string - hash a raw string, returns the correspodning struct sha256_Hash
 *
 * This API is designed such that this header exposes the definitions for the
 * functions, then based on a compile-time macro `ZING_LINUX` or `ZING_MACOS`
 * (and maybe `ZING_WINDOWS` in the future) the source files
 * src/crypto/linux_hashing or src/crypto/macos_hashing (respectively) will
 * actually come alive and implement those functions for the corresponding OS.
 *    src/main.c makes sure that either `ZING_LINUX` or `ZING_MACOS` is set.
 */

#ifndef CRYPTO_HASHING_H
#define CRYPTO_HASHING_H

#include <stdlib.h>

#define SHA256_DIGEST_LENGTH 32

typedef struct {
  size_t len;
  unsigned char digest[SHA256_DIGEST_LENGTH]; // digest length is 32 (256-bit)
} sha256_Hash;

/*
 * Very basic function, simply takes a string and hashes it, then
 * it loads the data and the digest into the struct and returns it.
 */
sha256_Hash sha256_hash_string(const char* string, size_t len);

#endif /* CRYPTO_HASHING_H */
