#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <game/entity.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

TEST(entity_tests,default_constructor) {
    sigmafive::game::entity e;
    EXPECT_EQ(std::uint32_t(-1),e.index);
}

TEST(entity_tests,constructor_index_and_version) {
    sigmafive::game::entity e{1,2};
    EXPECT_EQ(1,e.index);
    EXPECT_EQ(2,e.version);
}

TEST(entity_tests,is_valid_false) {
    sigmafive::game::entity e;
    EXPECT_FALSE(e.is_valid());
}

TEST(entity_tests,is_valid_true) {
    sigmafive::game::entity e{1,2};
    EXPECT_TRUE(e.is_valid());
}

TEST(entity_tests,equal_false) {
    sigmafive::game::entity e1{1,2};
    sigmafive::game::entity e2{0,0};
    EXPECT_FALSE(e1==e2);
}


TEST(entity_tests,equal_true) {
    sigmafive::game::entity e1{1,2};
    sigmafive::game::entity e2{1,2};
    EXPECT_TRUE(e1==e2);
}

TEST(entity_tests,not_equal_false) {
    sigmafive::game::entity e1{1,2};
    sigmafive::game::entity e2{1,2};
    EXPECT_FALSE(e1!=e2);
}

TEST(entity_tests,not_equal_true) {
    sigmafive::game::entity e1{1,2};
    sigmafive::game::entity e2{0,0};
    EXPECT_TRUE(e1!=e2);
}

TEST(entity_tests,serialize) {
    sigmafive::game::entity e1{1,2};
    sigmafive::game::entity e2;
    std::stringstream ss;
    {
        boost::archive::text_oarchive oar(ss);
        oar << boost::serialization::make_nvp("e",e1);
    }
    {
        boost::archive::text_iarchive iar(ss);
        iar >> boost::serialization::make_nvp("e",e2);
    }

    EXPECT_EQ(e1,e2);
}