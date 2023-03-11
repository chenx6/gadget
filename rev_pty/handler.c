#include <arpa/inet.h>
#include <fcntl.h>
#include <pty.h>
#include <sys/select.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>

#include "util.h"

static int copy_loop(int accepted_fd) {
  for (;;) {
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    FD_SET(accepted_fd, &set);
    select(max(STDIN_FILENO, accepted_fd) + 1, &set, NULL, NULL, NULL);
    int read_bytes = 0;
    if (FD_ISSET(STDIN_FILENO, &set)) {
      read_bytes = copy_data(STDIN_FILENO, accepted_fd);
    } else if (FD_ISSET(accepted_fd, &set)) {
      read_bytes = copy_data(accepted_fd, STDOUT_FILENO);
    }
    if (read_bytes < 0) {
      return read_bytes;
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s [PORT]\n", argv[0]);
    return -1;
  }
  // PTY
  int pty_fd = open("/proc/self/fd/0", O_RDWR);
  struct termios terminal, origin_terminal;
  tcgetattr(pty_fd, &terminal);
  origin_terminal = terminal;
  // turn off echo, uncanonical mode
  terminal.c_lflag &= ~ECHO;
  terminal.c_lflag &= ~ICANON;
  // Don't handle ^C / ^Z / ^\.
  terminal.c_cc[VINTR] = 0;
  terminal.c_cc[VQUIT] = 0;
  terminal.c_cc[VSUSP] = 0;
  tcsetattr(pty_fd, TCSANOW, &terminal);

  // Socket
  int listen_fd = get_socket_listen("0.0.0.0", atoi(argv[1]));
  socklen_t s;
  struct sockaddr accepted_addr;
  int accepted_fd = accept(listen_fd, &accepted_addr, &s);
  copy_loop(accepted_fd);
  tcsetattr(pty_fd, TCSANOW, &origin_terminal);
  return 0;
}