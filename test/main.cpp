#include <gtest/gtest.h>


int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

/* *************
 * TODO testing:
 * *************
 *  scalar cache
 *    insert/get variations
 *
 *  class cache
 *    lvalue refs
 *    rvalue refs
 *
 *    insert/get variations
 *
 */
