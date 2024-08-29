#include <gtest/gtest.h>
#include "unique_ptr.hpp"

template class my::unique_ptr<int>;

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(UniquePtrTest, ConstructorDereference){
    my::unique_ptr<int> ptr(new int(5));
    EXPECT_EQ(*ptr,5);
}

TEST(UniquePtrTest, MoveConstructor){
    my::unique_ptr<int> ptr1(new int(5));
    my::unique_ptr<int> ptr2(std::move(ptr1));
    EXPECT_EQ(*ptr2, 5);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST(UniquePtrTest, MoveAssignmentOperator){
    my::unique_ptr<int> ptr1(new int(5));
    my::unique_ptr<int> ptr2(new int(15));
    ptr2 = std::move(ptr1);

    EXPECT_EQ(ptr1.get(), nullptr);
    EXPECT_EQ(*ptr2, 5);
}

TEST(UniquePtrTest, ArrowOperator){
    struct Foo
{
    int give5() { return 5; }
};
    my::unique_ptr<Foo> ptr(new Foo);
    EXPECT_EQ(ptr->give5(), 5);
}

TEST(UniquePtrTest, GetTest){
    my::unique_ptr<int> ptr1(new int(10));
    int* ptr1_int = ptr1.get();

    EXPECT_EQ(*ptr1_int, 10);
}

TEST(UniquePtrTest, ReleaseTest){
    my::unique_ptr<int> ptr1(new int(10));
    int* ptr1_int = ptr1.release();

    EXPECT_EQ(*ptr1_int, 10);
    EXPECT_EQ(ptr1.get(), nullptr);
}

TEST(UniquePtrTest, ResetTest){
    my::unique_ptr<int> ptr1(new int(10));
    EXPECT_EQ(*ptr1, 10);

    int* ptrX_int = new int(20);

    ptr1.reset(ptrX_int);
    EXPECT_EQ(*ptr1, 20);
    
}