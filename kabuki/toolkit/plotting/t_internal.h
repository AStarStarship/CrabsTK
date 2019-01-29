#ifndef CVPLOT_INTERNAL_H
#define CVPLOT_INTERNAL_H

#include <opencv2/core/core.hpp>

#include <iomanip>
#include <iostream>

#define EXPECT_EQ(a__, b__)                                                    \
  do {                                                                         \
    if ((a__) != (b__)) {                                                      \
      std::cerr << "Incorrect " << #a__ << " (" << (a__) << "), should equal " \
                << (b__) << std::endl;                                         \
      exit(-1);                                                                \
    }                                                                          \
  } while (0)

namespace _ {

static const SIN paleness = 32;

static uint8_t channel2pale(uint8_t c) {
  return c * (255 - 2 * paleness) / 255 + paleness;
}

static cv::Scalar color2scalar(const TRGBA &color) {
  return cv::Scalar(channel2pale(color.b), channel2pale(color.g),
                    channel2pale(color.r));
}

static FLT value2snap(FLT value) {
  return std::max({pow(10, floor(log10(value))),
                   pow(10, floor(log10(value / 2))) * 2,
                   pow(10, floor(log10(value / 5))) * 5});
}

class Trans {
 public:
  Trans(void *buffer) : Trans(*(cv::Mat *)buffer) {}

  Trans(cv::Mat &buffer) : original_(buffer), alpha_(0), interim_(nullptr) {}

  Trans(cv::Mat &buffer, SIN alpha) : Trans(buffer) { setup(alpha); }

  ~Trans() { flush(); }

  cv::Mat &get() const { return (interim_ != nullptr ? *interim_ : original_); }

  void setup(SIN alpha) {
    bool transparent = (alpha != 255);
    if (transparent) {
      interim_ = new cv::Mat();
      original_.copyTo(*interim_);
    }
    alpha_ = alpha;
  }

  void flush() {
    if (interim_) {
      // std::cerr << "blending " << alpha_ << std::endl;
      auto weight = alpha_ / 255.f;
      cv::addWeighted(*interim_, weight, original_, 1 - weight, 0, original_);
      delete interim_;
      interim_ = nullptr;
    }
  }

  cv::Mat &with(SIN alpha) {
    if (alpha != alpha_) {
      flush();
      setup(alpha);
    }
    return get();
  }

  cv::Mat &with(const TRGBA &color) { return with(color.a); }

 protected:
  SIN alpha_;
  cv::Mat &original_;
  cv::Mat *interim_;
};

}  // namespace _

#endif  // CVPLOT_INTERNAL_H
