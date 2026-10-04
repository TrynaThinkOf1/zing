/*
 * Zevi Berlin - ZING 2026
 *
 * Implement all LINUX functions relating to hashing management for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/crypto/hashing.h
 */

#ifdef ZING_LINUX

  #include "crypto/hashing.h"

  #include <errno.h>
  #include <stdint.h>
  #include <stdio.h> // fprintf()
  #include <stdlib.h> // exit()
  #include <string.h> // strlen(), strerror()
  #include <unistd.h> // read(), write()

  #include <sys/socket.h>
  #include <linux/if_alg.h>
  #include <linux/socket.h>

  sha256_Hash sha256_hash_string(const char* string) {
    sha256_Hash hash;
    hash.raw = string;

    struct sockaddr_alg sa_alg;
    sa_alg.salg_family = AF_ALG;
    sa_alg.salg_type = {'h', 'a', 's', 'h'}; // dont ask, idk why it makes me do this.
    sa_alg.salg_name = {'s', 'h', 'a', '2', '5', '6'}; // ^

    int sock_fd = socket(AF_ALG, SOCK_SEQPACKET, 0); // create a connection to the kernel's crypto system
    if (sock_fd < 0) {
      // the socket could not be allocated
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Hashing Error.\n\tFailed to allocate Linux kernel crypto: %s\n", err);
      exit(1);
    }

    if (bind(sock_fd, (struct sockaddr *)&sa_alg, sizeof(sa_alg))) {
      // the socket could not be connected, algorithm may not be supported
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Hashing Error.\n\tFailed to allocate Linux kernel crypto: %s\n\tThe SHA256 algorithm may not be supported by your OS\n", err);
      exit(1);
    }

    int fd = accept(sock_fd, NULL, 0); // actually connect to the algorithm

    write(fd, string, strlen(string)); // send the plaintext to the hasher
    read(fd, hash.digest, 32); // read the hashed digest
    hash.digest[32] = 0;

    // clean up resources
    close(fd);
    close(sock_fd);

    return hash;
  }

#endif /* ZING_LINUX */
