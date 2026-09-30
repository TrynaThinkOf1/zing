/*
 * Zevi Berlin - ZING 2026
 * 
 * Entry File
 */

#include <stdio.h>

int main(int argc, char** argv) {
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      printf("Argument #%i: %s\n", i, argv[i]);
    }
    return 0;
  } else {
    fprintf(stderr, "No arguments given!\n");
    return 1;
  }
}