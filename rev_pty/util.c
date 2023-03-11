#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

static FILE *log = NULL;

int max(int a, int b) { return a > b ? a : b; }

void plog(const char *fmt, ...) {
  va_list arg;
  if (log == NULL) {
    log = fopen("log.log", "w");
  }
  va_start(arg, fmt);
  vfprintf(log, fmt, arg);
  fputc('\n', log);
  fflush(log);
  va_end(arg);
}

/// Get connected socket
int get_socket_connect(char *addr_, uint16_t port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr(addr_),
      .sin_port = htons(port),
  };
  int result = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
  if (result != 0) {
    return result;
  }
  return fd;
}

/// Get listened socket
int get_socket_listen(char *addr_, uint16_t port) {
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr = {
      .sin_family = AF_INET,
      .sin_addr.s_addr = inet_addr(addr_),
      .sin_port = htons(port),
  };
  bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
  listen(listen_fd, 5);
  return listen_fd;
}

/// Copy data from `read_fd` to `write_fd`
int copy_data(int read_fd, int write_fd) {
  char buf[4096];
  int read_bytes = 0;
  if ((read_bytes = read(read_fd, buf, 4096)) <= 0) {
    return read_bytes;
  }
  plog("recv %d bytes from %d", read_bytes, read_fd);
  write(write_fd, buf, read_bytes);
  return read_bytes;
}
