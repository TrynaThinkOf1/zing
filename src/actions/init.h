/*
 * Zevi Berlin - ZING 2026
 * 
 * Defines the function for the `zing init` command which creates a ZING repository
 *    init - create a ZING repo
 */

#ifndef ACTIONS_INIT_H
#define ACTIONS_INIT_H

#include <sys/stat.h>

#define PERMISSIONS S_IXUSR | S_IRUSR | S_IWUSR // owner execute/read/write - execute is needed to add files/subdirs to directory

/*
 * This init function creates the following:
 *    .zing/         - store all of the zing internal stuff
 *    .zingignore    - file to ignore stuff
 *    .zing/usr.json - store the user's info for commits -- NOT RIGHT NOW, IN FUTURE!!
 *    .zing/files/   - hold all of the hashed/compressed commits
 * 
 * If it fails it prints the error and exits
 */
void init();

#endif /* ACTIONS_INIT_H */