#include "weak_ptr.hpp"
#include <gtest/gtest.h>

// Instantiate the template class for testing
template class my::weak_ptr<int>;

TEST(WeakPtrTest, DefaultConstructor) {
    my::weak_ptr<int> wp;
    EXPECT_TRUE(wp.expired());
    EXPECT_EQ(wp.use_count(), 0);
}

TEST(WeakPtrTest, UseCount) {
    my::shared_ptr<int> sp(new int(10));
    my::weak_ptr<int> wp(sp);
    EXPECT_EQ(wp.use_count(), 1);
}

TEST(WeakPtrTest, Expired) {
    my::shared_ptr<int> sp(new int(20));
    my::weak_ptr<int> wp(sp);
    EXPECT_FALSE(wp.expired());
    sp.reset();
    EXPECT_TRUE(wp.expired());
}

TEST(WeakPtrTest, Reset) {
    my::shared_ptr<int> sp(new int(40));
    my::weak_ptr<int> wp(sp);
    wp.reset();
    EXPECT_TRUE(wp.expired());
}

TEST(WeakPtrTest, CopyConstructor) {
    my::shared_ptr<int> sp(new int(50));
    my::weak_ptr<int> wp1(sp);
    my::weak_ptr<int> wp2(wp1);
    EXPECT_EQ(wp1.use_count(), 1);
    EXPECT_EQ(wp2.use_count(), 1);
}

TEST(WeakPtrTest, MoveConstructor) {
    my::shared_ptr<int> sp(new int(60));
    my::weak_ptr<int> wp1(sp);
    my::weak_ptr<int> wp2(std::move(wp1));
    EXPECT_EQ(wp2.use_count(), 1);
    EXPECT_TRUE(wp1.expired());
}