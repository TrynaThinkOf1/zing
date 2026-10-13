/*
 * Zevi Berlin - ZING 2026
 *
 * Define all functions and types relating to hashing for the ZING program.
 * This includes:
 *    struct sha256_Hash - contains the pair of the raw data and the hashed digest
 *    sha256_hash_string - hash a raw string, returns the correspodning struct sha256_Hash
 *  streaming api:
 *    sha256_sm_init - initiate hash state machine
 *    sha256_sm_update - update hash state machine
 *    sha256_sm_final - finish and retrieve the hash
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

// opaque struct for type safety rather than use void*, changes internals based on machine
// for linux its just a file descriptor, macos has its own state struct
typedef struct sha256_SM sha256_SM;

/*
 * Very basic function, simply takes a string and hashes it, then
 * it loads the data and the digest into the struct and returns it.
 */
sha256_Hash sha256_hash_string(const char* string, size_t len);

/*
 * Internally just calls whatever funciton the crypto api uses
 * 
 * easy for macos, harder for linux.
 */
sha256_SM* sha256_sm_init(void);
void sha256_sm_update(sha256_SM* state, const char* string, size_t len);
sha256_Hash sha256_sm_final(sha256_SM* state);

#endif /* CRYPTO_HASHING_H */
