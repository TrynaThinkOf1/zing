/*
 * Zevi Berlin - ZING 2026
 *
 * Implement the data struct to store an array and coerresponding length for the ZING program.
 */

#ifndef LENARR_H
#define LENARR_H

#include <stddef.h>

typedef struct {
  size_t len;
  void* arr;
} LenArr;

#endif /* LENARR_H */