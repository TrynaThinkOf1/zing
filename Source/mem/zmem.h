/*
 * Zevi Berlin - ZING 2026
 * 
 * Define all functions relating to memory management for the ZING program.
 * This includes:
 *    z_alloc() - equivalent to malloc in most scenarios, this WILL 0-out allocated memory
 *    z_realloc() - equivalent to realloc in most scenarios, this WILL 0-out newly allocated memory
 *    z_free() - equivalent to free in most scenarios
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/*
 * The whole emergency space system is designed very primitively
 * in hopes that it will only ever be used in dire scenarios.
 * 
 * Basically the plan for it is to have 16kb of empty space, which
 * is theoretically large enough to store a few code files and
 * miscellaneous internal objects, and then if system malloc fails
 * it will attempt to use the emergency space.
 * 
 * It keeps a running tab and simply returns a pointer to the next
 * available bytes of space in the array, relying on the caller to
 * use the given memory safely just like malloc.
 */
#define __G_EMERGENCY_SPACE_SIZE 1024 * 16 // 16kb of memory

static unsigned char __G_EMERGENCY_SPACE[__G_EMERGENCY_SPACE_SIZE];
static unsigned int __G_EMERGENCY_SPACE_INDEX = 0;

/*
 * This will serve as the allocation function for ZING. It will
 * first attempt to use system malloc and just return that, otherwise
 * it will attempt to allocate the space into the emergency space (see
 * above).
 * 
 * On the off change that both allocations should fail, the function
 * will print an error message and call exit(1).
 */
void* z_alloc(uint32_t nbytes);

/*
 * This reallocation function will first check if the block is inside
 * of the emergency space with a bounds check, if it is inside the
 * emergency space it will do one of three things:
 *    1. If there is enough empty space ahead of it, copy the ptr into
 *        the space ahead and then zero out the old spot, returning the
 *        pointer to the new space
 *    2. If there is not enough space ahead, it will:
 *      a: Go from 0 checking if there is enough freed contiguous space
 *      b: Print an error message and call exit(1)
 * 
 * Otherwise it will call system realloc. In the event that realloc
 * fails, it will call z_alloc and go from there. That would be super-duper
 * last-case end-the-world scenario.
 */
void* z_realloc(void* ptr, uint32_t nbytes);

/*
 * The free function is relatively simple, inheriting from the design of
 * the z_alloc and z_realloc functions.
 * 
 * If the given ptr is part of emergency space, if it is the head of the
 * space then zero out and decrement index pointer. If it is not the head,
 * it will simply zero out the memory.
 * 
 * If it is not part of emergency space, it just calls system free.
 */
void z_free(void* ptr);