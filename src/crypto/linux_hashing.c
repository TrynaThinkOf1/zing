/*
 * Zevi Berlin - ZING 2026
 *
 * Implement all LINUX functions relating to hashing management for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/crypto/hashing.h
 */

#ifdef ZING_LINUX

  #include "crypto/hashing.h"

  #include <errno.h>
  #include <stdint.h>
  #include <stdio.h> // fprintf()
  #include <stdlib.h> // exit()
  #include <string.h> // strlen(), strerror()

  #include <linux/module.h>
  #include <crypto/hash.h>

  struct sdesc {
    struct shash_desc shash;
    char ctx[];
  };

  void __calc_hash(struct crypto_shash* alg, const char* data, uint32_t datalen, unsigned char* digest) {
    struct sdesc* sdesc;
    int size = sizeof(struct shash_desc) + crypto_shash_descsize(alg);
    sdesc = kmalloc(size, GFP_KERNEL);
    if (!sdesc) {
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Hashing Error.\n\tLinux kernel SHA256 allocation failed: %s\n", err);
      exit(1);
    }
    sdesc->shash.tfm = alg;

    crypto_shash_digest(&sdesc->shash, data, datalen, digest);
    
    kfree(sdesc);
  }

  sha256_Hash sha256_hash_string(const char* string) {
    sha256_Hash hash;
    hash.raw = string;

    struct crypto_shash* alg;
    char* hash_alg_name = "sha256";

    alg = crypto_alloc_shash(hash_alg_name, 0, 0);
    if (IS_ERR(alg)){
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Hashing Error.\n\tLinux kernel SHA256 failed: %s\n", err);
      exit(1);
    }

    unsigned int datalen = sizeof(data) - 1; // remove the null byte
    
    __calc_hash(alg, string, datalen, hash.digest);

    crypto_free_shash(alg);

    return hash;
  }

#endif /* ZING_LINUX */
