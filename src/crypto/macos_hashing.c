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

  #include "mem/zmem.h"

  sha256_Hash sha256_hash_string(const char* string, size_t len) {
    sha256_Hash hash;
    hash.len = SHA256_DIGEST_LENGTH;

    CC_SHA256(string, len, hash.digest);

    // hash.digest[CC_SHA256_DIGEST_LENGTH] = 0; // no need because we store length

    return hash;
  }

  /* */

  struct sha256_SM { CC_SHA256_CTX inner; };
  
  sha256_SM* sha256_sm_init(void) {
      sha256_SM* state = z_alloc(sizeof(*state));
      CC_SHA256_Init(&state->inner);
      return state;
  }
  
  void sha256_sm_update(sha256_SM* state, const char* string, size_t len) {
    CC_SHA256_Update(&state->inner, string, (CC_LONG)len);
  }
  
  sha256_Hash sha256_sm_final(sha256_SM* state) {
    sha256_Hash hash;
    hash.len = SHA256_DIGEST_LENGTH;
    
    CC_SHA256_Final(hash.digest, &state->inner);

    z_free(state);

    return hash;
  }

#endif /* ZING_MACOS */
