#ifndef BASE_TOKEN_BUCKET_H_
#define BASE_TOKEN_BUCKET_H_

#include <stdint.h>
#include <chrono>

namespace base {
using SteadyClock = std::chrono::steady_clock;

class TokenBucket {
 public:
  TokenBucket(uint64_t max_tokens, uint64_t consume_rate);
  ~TokenBucket();
  bool Consume(uint64_t token);

 private:
  const uint64_t max_tokens_;
  uint64_t tokens_;
  const uint64_t consume_rate_;
  SteadyClock::time_point last_fill_;
};
}  // namespace base

#endif  // BASE_TOKEN_BUCKET_H_
