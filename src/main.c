/*
 * Zevi Berlin - ZING 2026
 * 
 * Entry File
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem/zmem.h"

// #define malloc(unsigned long) (NULL) // force malloc to fail - early-stage testing.

int main(int argc, char** argv) {
  if (argc < 2) return 1;

  void* space = z_alloc(atoi(argv[1]));

  z_free(space);
  
  return 0;
}