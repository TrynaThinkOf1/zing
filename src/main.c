/*
 * Zevi Berlin - ZING 2026
 * 
 * Entry File
 */

#include <stdio.h>
#include <stdlib.h>

#include "mem/zmem.h"

#define malloc(n) (NULL) // force malloc to fail - early-stage testing.

int main(int argc, char** argv) {
  if (argc < 2) return 1;

  void* space = z_alloc(atoi(argv[1]));

  printf("space: %p\nbackup: [%p, %p]\nbackup index: %d\nspace in backup: %s\n",
    space,
    __G_EMERGENCY_SPACE,
    __G_EMERGENCY_SPACE + __G_EMERGENCY_SPACE_SIZE,
    __G_EMERGENCY_SPACE_INDEX,
    ((uintptr_t)space > (uintptr_t)__G_EMERGENCY_SPACE && (uintptr_t)space < (uintptr_t)(__G_EMERGENCY_SPACE + __G_EMERGENCY_SPACE_SIZE)) ? "true" : "false"
  );

  z_free(space);

  printf("space: %p\n", space);
  
  return 0;
}