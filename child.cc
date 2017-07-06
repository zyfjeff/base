#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>

/*
 *  父进程退出，子进程也退出，需要考虑这个信号是否已经有别的用处。
 *
 */
void parent_exit(int signum)
{
    printf("parent exit,so child procress will immediately exit");
    exit(0);
}

class NotifyChild {
 public:
  static bool register_callback(std::function<void(int)>& call) {
    assert(prctl(PR_SET_PDEATHSIG, SIGUSR1) == 0);
    assert(signal(SIGUSR1, NotifyChild::func_) == 0);
  }
 private:
  static std::function<void(int)> func_;
  static void wrapper_sighandler(int num) {
    func_(num);
  }
}
