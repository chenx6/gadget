#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plt_hook.h"

int my_atoi(const char *b) {
  return 114514;
}

int main() {
  plthook_info info;
  plthook_status s = plt_hook_init(&info, NULL);
  plt_hook_debug(&info);
  plt_hook_replace(&info, "atoi", my_atoi);
  char password[] = "123456";
  int r = atoi(password);
  printf("%d\n", r);
}