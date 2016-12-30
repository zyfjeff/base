#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#define NET_PROC_FILE  "/proc/net/tcp"

/*
 *  判断某个端口是否监听，目前仅支持tcp4
 */

bool is_listened(int32_t port, int64_t* inode) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  FILE *fp = fopen(NET_PROC_FILE, "r");
  if (fp == NULL) {
    // log error
    return false;
  }
  int listen_port;
  int count = 0;
  while((read = getline(&line, &len, fp)) != -1) {
    ++count;
    if (count == 1)
      continue;
      if (sscanf(line,"%*d:%*x:%x%*x:%*x%*x%*x:%*x%*x:%*x%*d%*d%*d%d", &listen_port, inode) != 0 && port == listen_port)
        return true;
  }
  free(line);
  return false;
}

/*
int main() {
  int64_t inode = 0;
  std::cout << is_listened(6379, &inode) << "\n";
  std::cout << "inode " << inode << "\n";
  return 0;
}
*/
