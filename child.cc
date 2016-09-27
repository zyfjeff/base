#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/prctl.h>

void parent_exit(int signum)
{
    printf("parent exit,so child procress will immediately exit");
    exit(0);
}

int main()
{
    prctl(PR_SET_PDEATHSIG, SIGHUP);
    signal(SIGHUP,parent_exit);
    while(1);
}
