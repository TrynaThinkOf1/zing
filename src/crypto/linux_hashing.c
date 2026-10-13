/*
 * Zevi Berlin - ZING 2026
 *
 * Implement all LINUX functions relating to hashing management for the ZING program.
 *
 * For API documentation regarding aforementioned functions, refer to src/crypto/hashing.h
 */

#include "hashing.h"
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

  #include "mem/zmem.h"

  sha256_Hash sha256_hash_string(const char* string, size_t len) {
    sha256_SM* state = sha256_sm_init();
    sha256_sm_update(state, string, len);
    return sha256_sm_final(state);
  }

  /* */
  
  struct sha256_SM {
    int sock_fd;
    int alg_fd;
  };
  
  sha256_SM* sha256_sm_init(void) {
    struct sockaddr_alg sa_alg = {
      .salg_family = AF_ALG,
      .salg_type = "hash",
      .salg_name = "sha256"
    };

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

    sha256_SM* state = z_alloc(sizeof(*state));
    state->sock_fd = sock_fd;
    state->alg_fd = fd;
    
    return state;
  }
  
  void sha256_sm_update(sha256_SM* state, const char* string, size_t len) {
    if (send(state->alg_fd), string, len, MSG_MORE) != len) {
      const char* err = strerror(errno);
      fprintf(stderr, "ZING Hashing Error.\n\tFailed to update hasher with new data: %s\n", err);
      exit(1);
    }
  }
  sha256_Hash sha256_sm_final(sha256_SM* state) {
    sha256_Hash hash;
    hash.len = SHA256_DIGEST_LENGTH;

    read(state->alg_fd, hash.digest, hash.len);

    close(state->alg_fd);
    close(state->sock_fd);

    z_free(state);

    return hash;
  }

#endif /* ZING_LINUX */
