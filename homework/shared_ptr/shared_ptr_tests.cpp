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

TEST(SharedPtrTest, ArrowOperator) {
    struct TestStruct {
        int value;
        TestStruct(int v)
            : value(v) {}
    };

    my::shared_ptr<TestStruct> sp(new TestStruct(42));
    EXPECT_EQ(sp->value, 42);
    sp->value = 24;
    EXPECT_EQ(sp->value, 24);
}

TEST(SharedPtrTest, GetMethod) {
    int* raw_ptr = new int(10);
    my::shared_ptr<int> sp(raw_ptr);

    EXPECT_EQ(sp.get(), raw_ptr);
    EXPECT_EQ(*sp.get(), 10);

    EXPECT_EQ(sp.use_count(), 1);
}

TEST(SharedPtrTest, ResetMethod) {
    my::shared_ptr<int> sp(new int(5));
    EXPECT_EQ(*sp, 5);
    EXPECT_EQ(sp.use_count(), 1);

    sp.reset(new int(10));
    EXPECT_EQ(*sp, 10);
    EXPECT_EQ(sp.use_count(), 1);

    sp.reset();
    EXPECT_EQ(sp.get(), nullptr);
    EXPECT_EQ(sp.use_count(), 0);
}

TEST(SharedPtrTest, ResetWithMultipleReferences) {
    my::shared_ptr<int> sp1(new int(5));
    my::shared_ptr<int> sp2 = sp1;

    EXPECT_EQ(sp1.use_count(), 2);
    EXPECT_EQ(sp2.use_count(), 2);

    sp1.reset(new int(10));
    EXPECT_EQ(*sp1, 10);
    EXPECT_EQ(sp1.use_count(), 1);
    EXPECT_EQ(*sp2, 5);
    EXPECT_EQ(sp2.use_count(), 1);
}
TEST(SharedPtrTest, CopyConstructor) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2(sp1);
    EXPECT_EQ(*sp1, 42);
    EXPECT_EQ(*sp2, 42);
    EXPECT_EQ(sp1.use_count(), 2);
    EXPECT_EQ(sp2.use_count(), 2);
}

TEST(SharedPtrTest, CopyAssignment) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2;
    sp2 = sp1;
    EXPECT_EQ(*sp1, 42);
    EXPECT_EQ(*sp2, 42);
    EXPECT_EQ(sp1.use_count(), 2);
    EXPECT_EQ(sp2.use_count(), 2);
}

TEST(SharedPtrTest, MoveConstructor) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2(std::move(sp1));
    EXPECT_EQ(*sp2, 42);
    EXPECT_EQ(sp2.use_count(), 1);
    EXPECT_FALSE(sp1);
}

TEST(SharedPtrTest, MoveAssignment) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2;
    sp2 = std::move(sp1);
    EXPECT_EQ(*sp2, 42);
    EXPECT_EQ(sp2.use_count(), 1);
    EXPECT_FALSE(sp1);
}

TEST(SharedPtrTest, BoolOperator) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2;
    EXPECT_TRUE(sp1);
    EXPECT_FALSE(sp2);
}

TEST(SharedPtrTest, Swap) {
    my::shared_ptr<int> sp1(new int(42));
    my::shared_ptr<int> sp2(new int(24));
    sp1.swap(sp2);
    EXPECT_EQ(*sp1, 24);
    EXPECT_EQ(*sp2, 42);
}

TEST(SharedPtrTest, MultipleSharedPtrs) {
    my::shared_ptr<int> sp1(new int(42));
    {
        my::shared_ptr<int> sp2 = sp1;
        my::shared_ptr<int> sp3 = sp1;
        EXPECT_EQ(sp1.use_count(), 3);
        EXPECT_EQ(sp2.use_count(), 3);
        EXPECT_EQ(sp3.use_count(), 3);
    }
    EXPECT_EQ(sp1.use_count(), 1);
}