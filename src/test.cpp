#include <gtest/gtest.h>

#include "archiver.h"

TEST(test, encoding) {
  Archiver A{};
  ASSERT_NO_THROW(A.Encode("huffman_tree.cpp", "./", "./"));
}

TEST(test, decoding) {
  Archiver A{};
  ASSERT_NO_THROW(A.Decode("huffman_tree.cpp_encoded", "./", "huffman.cfg"));
  ASSERT_EQ(system("cmp -s huffman_tree.cpp huffman_tree.cpp_decoded"), 0);
  system("rm huffman_tree.cpp_* huffman.cfg");
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}