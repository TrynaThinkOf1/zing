/*
 * Zevi Berlin - ZING 2026
 *
 * Implement all MACOS functions relating to hashing management for the ZING
 * program.
 *
 * For API documentation regarding aforementioned functions, refer to src/crypto/hashing.h
 */

/*
 * See the way this works is that this file is compiled into the program as a
 * translation unit no matter what, but if the OS isnt MacOS it simply doesnt
 * add any of the code locked inside a defined guard
 */
#ifdef ZING_MACOS

  #include "crypto/hashing.h"

  #include <string.h> // strlen()

  #include <CommonCrypto/CommonDigest.h>

  sha256_Hash sha256_hash_string(const char* string) {
    sha256_Hash hash;
    hash.raw = string;

    CC_SHA256(string, strlen(string), hash.digest);

    hash.digest[CC_SHA256_DIGEST_LENGTH] = 0;

    return hash;
  }

#endif /* ZING_MACOS */
