/*
 * Zevi Berlin - ZING 2026
 *
 * Implement the data decompression-related functions for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/pym/pym.h
 */

#include "pym/pym.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "zlib.h"

#include "mem/zmem.h"


zlib_Data zlib_decompress_string(unsigned char* string, size_t length) {
  z_stream      stream;     // state of deflation
  unsigned char out[CHUNK]; // where it will put output data

  stream.zalloc = Z_NULL; // z_alloc; // add custom memory allocation
  stream.zfree = Z_NULL; // z_free; // add custom memory allocation
  stream.opaque = Z_NULL; // idk what this does
  if (inflateInit(&stream) != Z_OK) {
    fprintf(stderr, "ZING Decompression Error.\n\tZLIB initialization failed: %s\n", stream.msg);
    exit(1);
  }
  
  unsigned char* out_str = z_alloc(length); // this is probably not enough
  unsigned int out_len = length;
  unsigned int out_written = 0;

  stream.avail_in = length; // let zlib deal with all of the chunking
  stream.next_in = (Bytef*)string;

  do {
    stream.avail_out = CHUNK;
    stream.next_out = out;

    inflate(&stream, Z_FINISH); // everything in one go hopefully

    unsigned int have = CHUNK - stream.avail_out;
    if (out_len - out_written < have) { // not enough space in output string
      out_str = z_realloc(out_str, out_written + have);
      out_len = out_written + have;
    }

    memcpy(out_str + out_written, out, have);
    out_written += have;
  } while (stream.avail_out == 0);

  inflateEnd(&stream);

  zlib_Data data = {
    .len = out_written,
    .data = out_str
  };

  return data;
}
