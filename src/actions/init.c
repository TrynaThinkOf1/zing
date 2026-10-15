/*
 * Zevi Berlin - ZING 2026
 *
 * Implement the `zing init` command's init function for the ZING program.
 *
 * For API documentation regarding aforementioned function, refer to src/actions/init.h
 */

#include "actions/init.h"

#include <dirent.h> // opendir()
#include <errno.h>
#include <fcntl.h> // open()
#include <stdio.h> // fnprintf()
#include <stdlib.h>
#include <string.h> // strerror()
#include <sys/stat.h> // mkdir(), permission bits
#include <unistd.h> // unlink(), rmdir(), close()

void init() {
  DIR* _dir = opendir(".zing");
  if (_dir) { // it exists
    fprintf(stderr, "ZING Init Error.\n\tA repository already exists here (.zing/).\n");
    exit(1);
  }

  // create .zing
  if (mkdir(".zing", PERMISSIONS) != 0) {
    const char* err = strerror(errno);
    fprintf(stderr, "ZING Init Error.\n\tFailed to create a repository (.zing/): %s\n", err);
    exit(1);
  }

  // create .zing/files
  if (mkdir(".zing/files", PERMISSIONS) != 0) {
    char* err = strerror(errno);
    fprintf(stderr, "ZING Init Error.\n\tFailed to create the `.zing/files` subdirectory in the repository (.zing/): %s\n", err);

    if (rmdir(".zing") != 0) {
      char* err = strerror(errno);
      fprintf(stderr, "ZING Init Error.\n\tFailed to clean unfinished state: %s\n\tRun `rm -rf .zing` before attempting `git init` again.", err);
    }
    
    exit(1); 
  }

  // create .zingignore
  int fd = open(".zingignore", O_WRONLY | O_TRUNC | O_CREAT, PERMISSIONS);
  if (fd < 0) {
    char* err = strerror(errno);
    fprintf(stderr, "ZING Init Error.\n\tFailed to create the .zingignore file for the repository (.zing/): %s\n", err);

    // removal order matters
    if (rmdir(".zing/files") != 0 || rmdir(".zing") != 0 || unlink(".zingignore") != 0) {
      char* err = strerror(errno);
      fprintf(stderr, "ZING Init Error.\n\tFailed to clean unfinished state: %s\n\tRun `rm -rf .zing` before attempting `git init` again.", err);
    }
    
    exit(1); 
  }

  // deal with the .zing/usr.json file -- FUTURE
}