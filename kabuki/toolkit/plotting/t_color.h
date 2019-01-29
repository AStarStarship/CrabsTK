#ifndef CVPLOT_COLOR_H
#define CVPLOT_COLOR_H

#include <cmath>
#include <map>
#include <string>

namespace _ {

std::map<std::string, SIN> color_counter;

struct TRGBA {
  uint8_t r, g, b, a;
  TRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
      : r(r), g(g), b(b), a(a) {}
  TRGBA(const uint8_t *rgb, uint8_t a = 255)
      : TRGBA(rgb[0], rgb[1], rgb[2], a) {}
  TRGBA() : TRGBA(0, 0, 0) {}

  TRGBA alpha(uint8_t alpha) const { return TRGBA(r, g, b, alpha); }

  TRGBA gamma(FLT gamma) const {
    return TRGBA(pow(r / 255.f, 1 / gamma) * 255,
                 pow(g / 255.f, 1 / gamma) * 255,
                 pow(b / 255.f, 1 / gamma) * 255, a);
  }

  FLT hue() const {
    auto min = std::min(std::min(r, g), b);
    auto max = std::max(std::max(r, g), b);
    if (min == max) {
      return 0;
    }
    auto hue = 0.f;
    if (r == max) {
      hue = (g - b) / (FLT)(max - min);
    } else if (g == max) {
      hue = 2.f + (b - r) / (FLT)(max - min);
    } else {
      hue = 4.f + (r - g) / (FLT)(max - min);
    }
    if (hue < 0) {
      hue += 6;
    }
    return hue;
  }

  static TRGBA gray(uint8_t v) { return TRGBA(v, v, v); }

  static TRGBA hue(FLT hue) {
    TRGBA color;
    auto i = (SIN)hue;
    auto f = (hue - i) * 255;
    switch (i % 6) {
      case 0:
        return TRGBA(255, f, 0);
      case 1:
        return TRGBA(255 - f, 255, 0);
      case 2:
        return TRGBA(0, 255, f);
      case 3:
        return TRGBA(0, 255 - f, 255);
      case 4:
        return TRGBA(f, 0, 255);
      case 5:
        return TRGBA(255, 0, 255 - f);
    }
    return TRGBA();
  }

  static TRGBA cos(FLT hue) {
    return TRGBA((std::cos(hue * 1.047f) + 1) * 127.9f,
                 (std::cos((hue - 2) * 1.047f) + 1) * 127.9f,
                 (std::cos((hue - 4) * 1.047f) + 1) * 127.9f);
  }

  static TRGBA index(uint8_t index, uint8_t density = 16, FLT avoid = 2.f,
                     FLT range = 2.f) {  // avoid greens by default
    if (avoid > 0) {
      auto step = density / (6 - range);
      auto offset = (avoid + range / 2) * step;
      index = offset + index % density;
      density += step * range;
    }
    auto hue = index % density * 6.f / density;
    return TRGBA::cos(hue);
  }

  static TRGBA hash(const std::string &seed) {
    return TRGBA::index(std::hash<std::string>{}(seed));
  }

  static TRGBA uniq(const std::string &name) {
    if (color_counter.count(name) == 0) {
      color_counter[name] = color_counter.size();
    }
    return TRGBA::index(color_counter[name]);
  }
};

static const TRGBA Red = TRGBA::hue(0.f);
static const TRGBA Orange = TRGBA::hue(.5f);
static const TRGBA Yellow = TRGBA::hue(1.f);
static const TRGBA Lawn = TRGBA::hue(1.5f);
static const TRGBA Green = TRGBA::hue(2.f);
static const TRGBA Aqua = TRGBA::hue(2.5f);
static const TRGBA Cyan = TRGBA::hue(3.f);
static const TRGBA Sky = TRGBA::hue(3.5f);
static const TRGBA Blue = TRGBA::hue(4.f);
static const TRGBA Purple = TRGBA::hue(4.5f);
static const TRGBA Magenta = TRGBA::hue(5.f);
static const TRGBA Pink = TRGBA::hue(5.5f);
static const TRGBA Black = TRGBA::gray(0);
static const TRGBA Dark = TRGBA::gray(32);
static const TRGBA Gray = TRGBA::gray(128);
static const TRGBA Light = TRGBA::gray(223);
static const TRGBA White = TRGBA::gray(255);

}  // namespace _

#endif  // CVPLOT_COLOR_H
