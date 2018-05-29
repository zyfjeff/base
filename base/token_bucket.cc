#include <algorithm>
#include <iostream>

#include "base/token_bucket.h"

namespace base {

TokenBucket::TokenBucket(uint64_t max_tokens, uint64_t consume_rate)
    : max_tokens_(max_tokens), tokens_(max_tokens), consume_rate_(consume_rate),
      last_fill_(SteadyClock::now()) { }

TokenBucket::~TokenBucket() { }

bool TokenBucket::Consume(uint64_t token) {
  if (tokens_ < max_tokens_) {
    auto now_time = SteadyClock::now();
    tokens_ = std::min(
        (std::chrono::duration_cast<std::chrono::seconds>(
        now_time - last_fill_).count() * consume_rate_) + tokens_, max_tokens_);
    last_fill_ = now_time;
  }

  if (tokens_ < token) {
    return false;
  }

  tokens_ -= token;
  return true;
}


}  // namespace base
