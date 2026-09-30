/*
 * Zevi Berlin - ZING 2026
 * 
 * Implement all functions relating to memory management for the ZING program.
 * 
 * For API documentation regarding aforementioned functions, refer to Source/mem/zmem.h
 */

#include "mem/zmem.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h> // fprintf()
#include <stdlib.h> // malloc(), free()
#include <string.h> // strerror()


void* z_alloc(uint32_t nbytes) {
  void* space = malloc(nbytes);
  if (space == NULL) {
    // the system malloc failed, first we will retry then we will use backup space
    space = malloc(nbytes);
    if (space == NULL) {
      // now, we must use emergency space because both malloc attempts failed.
      if (__G_EMERGENCY_SPACE_INDEX > __G_EMERGENCY_SPACE_SIZE - nbytes) {
        // there is no emergency space left, error out.
        const char* err = strerror(errno);
        fprintf(stderr, "ZING Memory Error.\n\tmalloc failed: %s\n\tbackup did not have enough space.\n", err);
      }
    
      // we will just allocate emergency space and return it
      space = __G_EMERGENCY_SPACE[__G_EMERGENCY_SPACE_INDEX];
      __G_EMERGENCY_SPACE_INDEX += nbytes + 1;
    }
  }

  return space;
}

void* z_realloc(void* ptr, uint32_t nbytes) {
  // check if the ptr exists within emergency space
  if ((uintptr_t)ptr > (uintptr_t)__G_EMERGENCY_SPACE && (uintptr_t)ptr < (uintptr_t)(__G_EMERGENCY_SPACE + __G_EMERGENCY_SPACE_SIZE)) {
    // it is in emergency space
    // next, check if we are increasing size or decreasing size and act accordingly
  } else {
    // it used system malloc
    // next, check if we are increasing size or decreasing size and act accordingly
    void* new_space = realloc(ptr, nbytes);
    if (new_space == NULL) {
      new_space = z_alloc(nbytes); // if this doesnt err out then we are good to go below
      memcpy(new_space, ptr, nbytes);
      return new_space;
    }
  }
}