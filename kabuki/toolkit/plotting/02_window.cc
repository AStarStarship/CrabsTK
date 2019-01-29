#include "t_window.h"

#include <gtest/gtest.h>

namespace _ {

TEST(WindowTest, Init) { Window w; }

}  // namespace _

SIN main(SIN argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
