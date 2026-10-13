/*
 * Zevi Berlin - ZING 2026
 * 
 * Entry File
 */

#include <stdio.h>
#include <stdlib.h>

#include "crypto/hashing.h"
#include "crypto/checksum.h"
#include "pym/pym.h"
#include "mem/zmem.h"

int main(int argc, char** argv) {
  
  sha256_Hash hash = checksum_sha256_compute();

  for (int i = 0; i < hash.len; i++) {
    printf("%x", hash.digest[i]);
  }
  
  return 0;
}