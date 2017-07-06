#ifndef _LOGAGENT_SINGLE_INSTANCE_H_
#define _LOGAGENT_SINGLE_INSTANCE_H_

#include <stdio.h>
#include <unistd.h>
#include <boost/noncopyable.hpp>

namespace logagent {

class SingleInstance : public boost::noncopyable
{
 public:
    SingleInstance(const char* pidfile) {
        fd_ = open(pidfile,O_RDWR|O_TRUNC|O_CREAT|O_EXCL);
        if(fd_ == -1) {
           //file also exsits
           fd_ = open(pidfile,O_RDWR|O_TRUNC);
        }

        int ret = LockOrUnlock(fd_,true);
        if (ret == -1) {
            close(fd_);
            fprintf(stderr,"Cannot start multiple logagent instances\n");
            exit(-1);
        }

        char buf[6] = {0};
        sprintf(buf,"%ld",static_cast<long>(getpid()));
        //TODO check return value
        write(fd_,buf,strlen(buf));
    }

    long get_lockfile_fd() {
        return fd_;
    }
    //C++11 delegation constructor
    SingleInstance() : SingleInstance("/var/run/logagent.pid") {}
    ~SingleInstance() {
        //ignore return value
        LockOrUnlock(fd_,false);
        close(fd_);

    }
 private:
    int fd_;
};

}

#endif //end of _LOGAGENT_SINGLE_INSTANCE_H_
