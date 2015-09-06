#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/object.hpp>
#include <sigmafive/object.hpp>

class object_1 : public sigmafive::object {
    SIGMAFIVE_CLASS()
};

class object_2 : public sigmafive::object {
    SIGMAFIVE_CLASS()
public:
};

EXPORT_SIGMAFIVE_CLASS(object_1)
EXPORT_SIGMAFIVE_CLASS(object_2)

TEST(object_tests,CLASS_ID) {
    EXPECT_EQ(sigmafive::compile_time_hash("object_1"), object_1::CLASS_ID);
}
