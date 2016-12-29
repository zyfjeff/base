#include <unistd.h>
#include <sys/types.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <exception>
#include <string>
#include <iostream>
#define MAX_PATH_SIZE 256

extern bool is_listened(int32_t port, int64_t *inode);  //from is_listened.cc

bool is_digit(const char* inStr, size_t size) {
  for(int i = 0; i < size; ++i) {
    if(isdigit(inStr[i]) == 0)
      return false;
  }

  return true;
}

bool is_user_process (const char* inStr){
  // 1 ~ 299 is daemon process,not normal process
  try {

    unsigned int pid = std::stoul(inStr);
    if (pid > 299)
      return true;
    else
      return false;

  } catch(std::exception& e) {
    // log error
    return false;
  }
}

bool is_pid_listend(const char* path, int64_t inode ) {
  if (access(path, F_OK) != -1) {
    DIR *procfdDir = opendir(path);
    if (procfdDir != NULL) {
      struct dirent *end_flag;
      while(true) {
        // Setp1: Directory traversal
        struct dirent entry;
        int ret = readdir_r(procfdDir, &entry, &end_flag);
        if (end_flag != NULL) {
          if (strncmp(entry.d_name, ".", strlen(entry.d_name)) == 0 ||
              strncmp(entry.d_name, "..", strlen(entry.d_name)) == 0)
              continue;

        // Setp2: To determine whether fd is socket fd and listening on the specified port
          char path_buf[MAX_PATH_SIZE] = {0};
          char link_buf[256] = {0};
          int64_t anony_inode;
          sprintf(path_buf,"%s/%s",path, entry.d_name);

          if (readlink(path_buf, link_buf, 255) != -1) {
            if (sscanf(link_buf, "socket:[%d]", &anony_inode) != 0) {
              printf("[1]anony_inode:%d  inode:%d result: %d\n", anony_inode, inode, anony_inode - inode);
              if (anony_inode == inode) {
                printf("[2]anony_inode:%d  inode:%d\n", anony_inode, inode);
                return true;
              }
            }
          }

          // continue
        } else {
          break;  // directory traversal end
        }
      }
    } else {  // if (procDir != NULL)
      return false;
    }

  } else {  // if (accesspath, F_OK) != -1)
    return false;
  }
}

/*
 *  通过指定端口，获取监听这个端口的pid
 *
 */
pid_t get_pid_from_port(int32_t port) {

  // Setp1: 如果有进程监听在这个端口，那么继续，否则直接返回0
  // Setp2: 遍历/proc/PID/fd 找到监听在这个端口的进程pid


  int64_t  listenfd_inode = 1;
  if (!is_listened(port, &listenfd_inode))
    return 0;

  DIR *procDir = opendir("/proc/");
  if (procDir != NULL) {
    struct dirent *end_flag;
    while(true) {
      struct dirent entry;
      int ret = readdir_r(procDir, &entry, &end_flag);
      if (end_flag != NULL) {
        if (strncmp(entry.d_name, ".", strlen(entry.d_name)) == 0 ||
            strncmp(entry.d_name, "..", strlen(entry.d_name)) == 0)
            continue;

        if (!is_digit(entry.d_name, strlen(entry.d_name))) {
            continue;
        }

        if (!is_user_process(entry.d_name)) {
            continue;
        }

        // Starting deal with.......
        char path_buf[MAX_PATH_SIZE] = {0};
        sprintf(path_buf,"/proc/%s/fd/", entry.d_name);
        if (!is_pid_listend(path_buf, listenfd_inode)) {
          continue;
        }

        // std::stoul always success
        return static_cast<pid_t>(std::stoul(entry.d_name));
      } else {
        break;  // directory traversal end
      }
    }

  } else {
    //log error
  }

  return 0; // failure
}


int main() {
 if ( get_pid_from_port(6379) == 0) {
  std::cout << "error, not find" << "\n";
 } else {
  std::cout << "yes find it" << "\n";

 }

 return 0;
}
