#include "profile.h"
#include <stdio.h>
#include <string.h>
#include <thread>
#include <sstream>
#include <string>
#if ENABLE_API_PROFILER

//#define CPU_MHZ (2200.122 * 1000000)


int get_cpufreq() {
  FILE *fp = popen("cat /proc/cpuinfo |grep \"cpu MHz\"|head -1|awk -F\":\" '{print $2}'|xargs -i echo {}","r");
  char buf[256] = {0};
  size_t sz = sizeof(buf) - 1;
  char *pbuf = buf;
  getline(&pbuf,&sz,fp);
  double mhz;
  std::string tt(buf,strlen(buf));
  std::stringstream ss(tt);
  ss.precision(3);
  ss >> mhz;
  fclose(fp);
  return mhz * 1000;
}


uint64_t rdtsc() {
  uint32_t lo,hi;
  __asm__ __volatile__
    (
     "rdtsc":"=a"(lo),"=d"(hi)
    );
  return (uint64_t)hi<<32|lo;
}

void QueryPerformanceCounter(uint64_t *start) {
  *start = rdtsc();
}



static const float APIProfiler_ReportIntervalSecs = 1.0f;
float APIProfiler::ooFrequency_ = 0;
int64_t APIProfiler::reportInterval_ = 0;

void APIProfiler::Flush(uint64_t end) {
  if (reportInterval_ == 0) {
    int cpu_freq = get_cpufreq();
    ooFrequency_ = 1.0f / cpu_freq;
    MemoryBarrier();
    reportInterval_ = (int64_t)(cpu_freq * APIProfiler_ReportIntervalSecs);
  }

  if (threadInfo_->lastReportTime == 0) {
    threadInfo_->lastReportTime = start_;
    return;
  }

  float interval = (end - threadInfo_->lastReportTime) * ooFrequency_;
  float measured = threadInfo_->accumulator * ooFrequency_;
  printf("TID 0x%x time spent in \"%s\": %.0f/%0.f ms %1.f%% %dx\n",
        std::this_thread::get_id(),
        threadInfo_->name,
        measured * 1000,
        interval * 1000,
        100.f * measured / interval,
        threadInfo_->hitCount);
  threadInfo_->lastReportTime = end;
  threadInfo_->accumulator = 0;
  threadInfo_->hitCount = 0;
}

#endif
