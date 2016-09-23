#ifndef __LIB_PROFILER_H_
#define __LIB_PROFILER_H_

#include <thread>
#include <stdint.h>
#include <stdio.h>

#define ENABLE_API_PROFILER 1
#if ENABLE_API_PROFILER

#define MemoryBarrier() asm volatile("mfence" ::: "memory")

uint64_t rdtsc();
void QueryPerformanceCounter(uint64_t *start);

class APIProfiler {
 public:
  struct ThreadInfo {
    int64_t lastReportTime;
    int64_t accumulator;
    int64_t hitCount;
    const char* name;
  };
 private:
  uint64_t start_;
  ThreadInfo *threadInfo_;
  static float ooFrequency_;
  static int64_t reportInterval_;
  void Flush(uint64_t end);

 public:
  inline APIProfiler(ThreadInfo *threadInfo) {
    uint64_t start;
    QueryPerformanceCounter(&start);
    start_ = start;
    threadInfo_ = threadInfo;
  }

  inline ~APIProfiler() {
    uint64_t end;
    QueryPerformanceCounter(&end);
    threadInfo_->accumulator += (end - start_);
    threadInfo_->hitCount++;
    if ( (end - threadInfo_->lastReportTime) > reportInterval_) {
      Flush(end);
    }
  }
};

#define DECLARE_API_PROFILER(name)  \
  extern thread_local  APIProfiler::ThreadInfo __APIProfiler_##name;

#define DEFINE_API_PROFILER(name)   \
  thread_local APIProfiler::ThreadInfo __APIProfiler_##name = { 0, 0, 0, #name};

#define TOKENPASTE2(x,y) x ## y

#define TOKENPASTE(x,y) TOKENPASTE2(x,y)

#define API_PROFILER(name)           \
  APIProfiler TOKENPASTE(__APIProfiler_##name,__LINE__)(&__APIProfiler_##name)

#else

#define DECLARE_API_PROFILER(name)
#define DEFINE_API_PROFILER(name)
#define API_PROFILER(name)

#endif

#endif  //end __LIB_PROFILER_H_

