/*
 * Zevi Berlin - ZING 2026
 *
 * Implement the data compression-related functions for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/pym/pym.h
 */

#include "crypto/checksum.h"

#include <dirent.h> // opendir(), readdir(), closedir()
#include <errno.h>
#include <fcntl.h> // open()
#include <stdbool.h> // qsort()
#include <stdio.h> // fprintf(), snprintf(), fopen(), fread(), fclose()
#include <stdlib.h>
#include <string.h> // strerror(), memcpy(), strcmp(), strlen()
#include <sys/stat.h> // lstat(), S_ISDIR() macro
#include <sys/types.h> // read()
#include <sys/uio.h> // read()
#include <unistd.h> // close(), read(), write()

#include "crypto/hashing.h"
#include "mem/zmem.h"


bool checksum_compare(const sha256_Hash* hash1, const sha256_Hash* hash2) {
  for (size_t i = 0; i < hash1->len; i++) {
    if (hash1->digest[i] != hash2->digest[i]) return false;
  }

  return true;
}

// honestly this code is pretty much taken from https://stackoverflow.com/a/44037947
// returns final length of array
size_t __recurse_add(char* d, char*** path_list, size_t path_list_len) {
  DIR* dir;
  struct dirent* entry;
  struct stat s;

  if (!(dir = opendir(d))) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Checksum Error.\n\tFailed to open %s directory or subdirectory: %s\n", d, err);
    exit(1);
  }

  while ((entry = readdir(dir))) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue; // if the directory is . or .. skip
      
      char path[4096]; // idk I saw this somewhere as the maximum path length
      int len = snprintf(path, sizeof(path) - 1, "%s/%s", d, entry->d_name); // makes pathname, nul terminates
      if (strcmp(path, ".cs") == 0) continue; // skip the checksum file
      // uncomment if my random constant is wrong
      // if (len < 0 || (size_t)len >= sizeof(path)) {
      //   fprintf(stderr, "ZING Checksum Error.\n\tPath too long: %s/%s\n", d, entry->d_name);
      //   exit(1);
      // }

      // TODO: Maybe move lstat out and check for error?
      if (lstat(path, &s) == 0 && S_ISDIR(s.st_mode)) { // if it's a directory
          path_list_len = __recurse_add(path, path_list, path_list_len);
      } else { // it's a file, so add the path name
        *path_list = z_realloc(*path_list, sizeof(char*) * (path_list_len + 1));
        char* new_path = z_alloc(len + 1);
        memcpy(new_path, path, len + 1);
        (*path_list)[path_list_len++] = new_path;
      }
  };
  
  closedir(dir);

  return path_list_len;
}

// this function receives POINTERS to the data, not the data themselves
int compar(const void* a, const void* b) { return strcmp(*(const char**)a, *(const char**)b); }

sha256_Hash checksum_sha256_compute() {
  char** filename_list = z_alloc(sizeof(char*));
  size_t len = __recurse_add(".zing", &filename_list, 0);
  
  // quicksort the paths lexicographically
  qsort(filename_list, len, sizeof(char*), compar);

  sha256_SM* state = sha256_sm_init();//(&state);

  for (size_t i = 0; i < len; i++) {
    struct stat st;
    if (stat(filename_list[i], &st) != 0) {
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Checksum Error.\n\tFailed to stat %s: %s\n", filename_list[i], err);
      exit(1);
    }
    
    FILE* file = fopen(filename_list[i], "r");
    if (file < 0) {
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Checksum Error.\n\tFailed to open %s: %s\n", filename_list[i], err);
      exit(1);
    }

    char data[st.st_size];

    if (fread(data, sizeof(unsigned char), st.st_size, file) < st.st_size) {
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Checksum Error.\n\tFailed to read %s: %s\n", filename_list[i], err);
      exit(1);
    }

    sha256_sm_update(state, data, st.st_size);

    fclose(file);
  }

  sha256_Hash hash = sha256_sm_final(state);
  
  return hash;
}

/* */

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

void checksum_sha256_write(const sha256_Hash* hash) {
  int cs = open(".zing/.cs", O_WRONLY | O_CREAT);
  if (cs < 0) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Checksum Error.\n\tFailed to open the SHA256 checksum: %s\n", err);
    exit(1);
  }
  
  if (write(cs, hash->digest, hash->len) != hash->len) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Checksum Error.\n\tFailed to write the SHA256 checksum: %s\n", err);
    exit(1);
  }

  close(cs);
}