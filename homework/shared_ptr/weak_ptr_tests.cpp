#include <gtest/gtest.h>
#include "weak_ptr.hpp"

template class my::weak_ptr<int>;

TEST(WeakPtrTest, DefaultConstructor) {
    my::weak_ptr<int> wp;
    EXPECT_TRUE(wp.expired());
    EXPECT_EQ(wp.use_count(), 0);
}

TEST(WeakPtrTest, ConstructFromSharedPtr) {
    my::shared_ptr<int> sp(new int(42));
    my::weak_ptr<int> wp(sp);

    EXPECT_FALSE(wp.expired());
    EXPECT_EQ(wp.use_count(), 1);
}

TEST(WeakPtrTest, LockMethod) {
    my::shared_ptr<int> sp(new int(42));
    my::weak_ptr<int> wp(sp);

    auto locked = wp.lock();
    EXPECT_TRUE(locked);
    EXPECT_EQ(*locked, 42);
    EXPECT_EQ(locked.use_count(), 2);
}

TEST(WeakPtrTest, ExpiredCheck) {
    my::weak_ptr<int> wp;
    {
        my::shared_ptr<int> sp(new int(42));
        wp = sp;
        EXPECT_FALSE(wp.expired());
    }
    EXPECT_TRUE(wp.expired());
}

TEST(WeakPtrTest, CopyConstructor) {
    my::shared_ptr<int> sp(new int(42));
    my::weak_ptr<int> wp1(sp);
    my::weak_ptr<int> wp2(wp1);

    EXPECT_EQ(wp1.use_count(), 1);
    EXPECT_EQ(wp2.use_count(), 1);
}

TEST(WeakPtrTest, MoveConstructor) {
    my::shared_ptr<int> sp(new int(42));
    my::weak_ptr<int> wp1(sp);
    my::weak_ptr<int> wp2(std::move(wp1));

    EXPECT_TRUE(wp1.expired());
    EXPECT_FALSE(wp2.expired());
    EXPECT_EQ(wp2.use_count(), 1);
}

TEST(WeakPtrTest, Reset) {
    my::shared_ptr<int> sp(new int(42));
    my::weak_ptr<int> wp(sp);

    wp.reset();
    EXPECT_TRUE(wp.expired());
    EXPECT_EQ(wp.use_count(), 0);
}