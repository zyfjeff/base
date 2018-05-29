#include <thread>
#include <chrono>

#include "base/token_bucket.h"

#include "gtest/gtest.h"

namespace base {

TEST(TokenBucketTest, basic) {
  TokenBucket bucket(100, 1);
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(bucket.Consume(1));
  }
  EXPECT_FALSE(bucket.Consume(1));
}

TEST(TokenBucketTest, time_reset) {
  TokenBucket bucket(100, 1);
  for(int i = 0; i < 100; i++) {
    EXPECT_TRUE(bucket.Consume(1));
  }
  std::this_thread::sleep_for(std::chrono::seconds(1));
  EXPECT_TRUE(bucket.Consume(1));
}

}  // namespace base