/*
 * Zevi Berlin - ZING 2026
 *
 * Implement the data compression-related functions for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/pym/pym.h
 */

#include "crypto/checksum.h"

#include <dirent.h>
#include <errno.h>
#include <fcntl.h> // open()
#include <stdbool.h>
#include <stdio.h> // fprintf()
#include <stdlib.h>
#include <string.h> // strerror()
#include <sys/stat.h>
#include <sys/types.h> // read()
#include <sys/uio.h> // read()
#include <unistd.h> // close()

#include "crypto/hashing.h"
#include "hashing.h"


bool checksum_compare(const sha256_Hash* hash1, const sha256_Hash* hash2) {
  for (size_t i = 0; i < hash1->len; i++) {
    if (hash1->digest[i] != hash2->digest[i]) return false;
  }

  return true;
}

sha256_Hash checksum_sha256_compute();

sha256_Hash checksum_sha256_read() {
  int cs = open(".zing/.cs", O_RDONLY);
  if (cs < 0) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Checksum Error.\n\tFailed to open the SHA256 checksum: %s\n", err);
    exit(1);
  }

  sha256_Hash hash;
  hash.len = 32;

  if (read(cs, hash.digest, hash.len) != hash.len) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Checksum Error.\n\tFailed to read the SHA256 checksum: %s\n\tPossible tampering.\n", err);
    exit(1);
  }

  close(cs);

  return hash;
}