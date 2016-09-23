#include "profile.h"
#include <stdio.h>
#include <unistd.h>

DEFINE_API_PROFILER(test);
void test() {
  API_PROFILER(test);
  int count = 100000000;
  while(count--);
  printf("Hello World\n");
}

int main() {
  while(1) {
    test();
    sleep(1);
  }
}
