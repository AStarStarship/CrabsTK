#include <pch.h>

#include "t_color.h"

namespace _ {

TEST(TRGBATest, Init) {
  TRGBA c;
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 0);
  EXPECT_EQ(c.b, 0);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Init1) {
  uint8_t v[3] = {1, 2, 3};
  TRGBA c(v, 4);
  EXPECT_EQ(c.r, 1);
  EXPECT_EQ(c.g, 2);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 4);
  TRGBA d(v);
  EXPECT_EQ(d.a, 255);
}

TEST(TRGBATest, Init2) {
  TRGBA c(1, 2, 3, 4);
  EXPECT_EQ(c.r, 1);
  EXPECT_EQ(c.g, 2);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 4);
  TRGBA d(1, 2, 3);
  EXPECT_EQ(d.a, 255);
}

TEST(TRGBATest, Alpha) {
  TRGBA c(1, 2, 3, 4);
  EXPECT_EQ(c.alpha(5).a, 5);
}

TEST(TRGBATest, Gray) {
  TRGBA c = TRGBA::gray(3);
  EXPECT_EQ(c.r, 3);
  EXPECT_EQ(c.g, 3);
  EXPECT_EQ(c.b, 3);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Hue) {
  TRGBA c = TRGBA::hue(3);
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 255);
  EXPECT_EQ(c.b, 255);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Cos) {
  TRGBA c = TRGBA::cos(3);
  EXPECT_EQ(c.r, 0);
  EXPECT_EQ(c.g, 191);
  EXPECT_EQ(c.b, 191);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Index) {
  TRGBA c = TRGBA::index(3);
  EXPECT_EQ(c.r, 37);
  EXPECT_EQ(c.g, 94);
  EXPECT_EQ(c.b, 251);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Hash) {
  TRGBA c = TRGBA::hash("test");
  EXPECT_EQ(c.r, 160);
  EXPECT_EQ(c.g, 4);
  EXPECT_EQ(c.b, 218);
  EXPECT_EQ(c.a, 255);
}

TEST(TRGBATest, Red) {
  EXPECT_EQ(Red.r, 255);
  EXPECT_EQ(Red.g, 0);
  EXPECT_EQ(Red.b, 0);
  EXPECT_EQ(Red.a, 255);
}

TEST(TRGBATest, Green) {
  EXPECT_EQ(Green.r, 0);
  EXPECT_EQ(Green.g, 255);
  EXPECT_EQ(Green.b, 0);
  EXPECT_EQ(Green.a, 255);
}

TEST(TRGBATest, Blue) {
  EXPECT_EQ(Blue.r, 0);
  EXPECT_EQ(Blue.g, 0);
  EXPECT_EQ(Blue.b, 255);
  EXPECT_EQ(Blue.a, 255);
}

}  // namespace _
