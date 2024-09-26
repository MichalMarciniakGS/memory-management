#include <gtest/gtest.h>
#include "shared_ptr.hpp"

// Instantiate the template class for testing
template class my::shared_ptr<int>;

TEST(SharedPtrTest, DefaultConstructor) {
    my::shared_ptr<int> sp;
    EXPECT_EQ(sp.use_count(), 1);
    EXPECT_FALSE(sp);
}

TEST(SharedPtrTest, ConstructorWithPointer) {
    my::shared_ptr<int> sp(new int(42));
    EXPECT_EQ(sp.use_count(), 1);
    EXPECT_TRUE(sp);
    EXPECT_EQ(*sp, 42);
}