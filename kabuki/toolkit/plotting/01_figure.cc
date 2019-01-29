#include "t_figure.h"

#include <gtest/gtest.h>

namespace _ {

TEST(FigureTest, Init) {
  Window w;
  View v(w);
  Figure f(v);
}

}  // namespace _

SIN main(SIN argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
