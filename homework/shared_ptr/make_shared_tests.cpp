#include <gtest/gtest.h>
#include "make_shared.hpp"
#include "weak_ptr.hpp"
#include <string>

struct TestStruct {
    int x;
    std::string str;
    
    TestStruct(int x_, std::string str_) 
        : x(x_), str(std::move(str_)) {}
};

TEST(MakeSharedTest, BasicType) {
    auto sp = my::make_shared<int>(42);
    EXPECT_EQ(*sp, 42);
    EXPECT_EQ(sp.use_count(), 1);
}

TEST(MakeSharedTest, CustomType) {
    auto sp = my::make_shared<TestStruct>(42, "test");
    EXPECT_EQ(sp->x, 42);
    EXPECT_EQ(sp->str, "test");
    EXPECT_EQ(sp.use_count(), 1);
}

TEST(MakeSharedTest, WeakPtr) {
    auto sp = my::make_shared<int>(42);
    my::weak_ptr<int> wp(sp);
    
    EXPECT_EQ(wp.use_count(), 1);
    EXPECT_FALSE(wp.expired());
    
    auto locked = wp.lock();
    EXPECT_EQ(*locked, 42);
}