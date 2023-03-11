#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

int max(int a, int b);
void plog(const char *fmt, ...);
int copy_data(int read_fd, int write_fd);
int get_socket_connect(char *addr_, uint16_t port);
int get_socket_listen(char *addr_, uint16_t port);

#endif