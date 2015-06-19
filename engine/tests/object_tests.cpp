#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <object.hpp>
#include <object_pool.hpp>

class object_1 : public sigmafive::object {
    SIGMAFIVE_OBJECT()
};

class object_2_pool : public sigmafive::object_pool {
public:
    static std::unique_ptr<sigmafive::object_pool> create_pool() {
        return std::unique_ptr<sigmafive::object_pool>(new object_2_pool());
    }
};

class object_2 : public sigmafive::object {
    SIGMAFIVE_OBJECT()
public:
    typedef object_2_pool pool_type;
};

SIGMAFIVE_IMPLEMENT_OBJECT(object_1)
SIGMAFIVE_IMPLEMENT_OBJECT(object_2)

TEST(object_tests,CLASS) {
    EXPECT_EQ("object_1", object_1::CLASS);
}

TEST(object_tests,CLASS_ID) {
    EXPECT_EQ(compile_time_hash(object_1::CLASS), object_1::CLASS_ID);
}

TEST(object_tests,has_pool_false) {
    EXPECT_FALSE(sigmafive::object::has_pool(compile_time_hash("object_1")));
}

TEST(object_tests,has_pool_true) {
    EXPECT_TRUE(sigmafive::object::has_pool(compile_time_hash("object_2")));
}
