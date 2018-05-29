#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* get_program_name(char *buf,int count, pid_t pid)
{
    char procpath[256] = {0};
    sprintf(procpath, "/proc/%d/exe", pid);

    if (access(procpath, F_OK) == -1) {
      return NULL;
    }

    int retval = readlink(procpath, buf,count-1);
    if((retval < 0 || retval >= count - 1)) {
        return NULL;
    }
    buf[retval] = '\0';
    // 获取指向最后一次出现'/'字符的指针
    // 并没有处理 delete的情况
    char* process_name = strrchr(buf, '/');
    if (process_name) {
      return strrchr(buf,'/') + 1;
    } else {
      return NULL;
    }
}


int main() {
  char buf[256] = {0};
  printf("%s\n",get_program_name(buf, 256 -1, 1));
  return 0;
}
