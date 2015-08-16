#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/object.hpp>
#include <sigmafive/object.hpp>

class object_1 : public sigmafive::object {
    SIGMAFIVE_CLASS()
};

class object_2_pool : public sigmafive::object_pool {
public:
    static std::unique_ptr<sigmafive::object_pool> create_pool() {
        return std::unique_ptr<sigmafive::object_pool>(new object_2_pool());
    }
};

class object_2 : public sigmafive::object {
    SIGMAFIVE_CLASS()
public:
    typedef object_2_pool pool_type;
};

EXPORT_SIGMAFIVE_CLASS(object_1)
EXPORT_SIGMAFIVE_CLASS(object_2)

TEST(object_tests,CLASS_ID) {
    EXPECT_EQ(compile_time_hash("object_1"), object_1::CLASS_ID);
}
