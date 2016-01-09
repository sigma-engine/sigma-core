#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <cppbr/meta/object.hpp>
#include <sigmafive/resource/resource.hpp>
#include <sigmafive/resource/resource_cache.hpp>

using namespace sigmafive::resource;

using ::testing::_;
using ::testing::Return;

struct r1 {
};

template<class T>
class mock_cache : public resource_cache<T> {
public:
    /*MOCK_METHOD1(add_resource, void(resource));*/
    /*MOCK_METHOD1_T(get_resource, resource<T>(identifier));*/
    MOCK_METHOD1_T(acquire, T*(identifier));
    MOCK_CONST_METHOD1_T(acquire, const T*(identifier));
    /*MOCK_METHOD1(remove_resource, void(resource));*/
};

TEST(resource_tests,default_construct_should_be_nullptr) { // TODO should may be assert cache is not nullptr??
    resource<r1> r;

    EXPECT_TRUE(r == nullptr);
    EXPECT_FALSE(r != nullptr);
    EXPECT_TRUE(nullptr == r);
    EXPECT_FALSE(nullptr != r);
}

TEST(resource_tests,nullptr_cache_valid_identifier_construct_should_be_nullptr) { // TODO should may be assert cache is not nullptr?
    resource<r1> r{identifier{"test"},nullptr};

    EXPECT_TRUE(r == nullptr);
    EXPECT_FALSE(r != nullptr);
    EXPECT_TRUE(nullptr == r);
    EXPECT_FALSE(nullptr != r);
}

TEST(resource_tests,non_nullptr_cache_and_invalid_identifier_construct_should_be_nullptr) {
    mock_cache<r1> cache;

    identifier id;
    resource<r1> r{id,&cache};

    EXPECT_TRUE(r == nullptr);
    EXPECT_FALSE(r != nullptr);
    EXPECT_TRUE(nullptr == r);
    EXPECT_FALSE(nullptr != r);
}

TEST(resource_tests,non_nullptr_cache_and_valid_identifier_construct_should_not_be_nullptr) {
    mock_cache<r1> cache;
    r1 res;

    identifier id("test");
    resource<r1> r{id,&cache};

    EXPECT_CALL(cache,acquire(id)).Times(4).WillRepeatedly(Return(&res));

    EXPECT_FALSE(r == nullptr);
    EXPECT_TRUE(r != nullptr);
    EXPECT_FALSE(nullptr == r);
    EXPECT_TRUE(nullptr != r);
}

TEST(resource_tests,should_equal_nullptr_if_cache_returns_nullptr) {
    mock_cache<r1> cache;

    identifier id{"test"};
    resource<r1> r{id,&cache};

    EXPECT_CALL(cache,acquire(id)).Times(4).WillRepeatedly(Return(nullptr));

    EXPECT_TRUE(r == nullptr);
    EXPECT_FALSE(r != nullptr);

    EXPECT_TRUE(nullptr == r);
    EXPECT_FALSE(nullptr != r);
}

TEST(resource_tests,two_resources_should_not_be_equal_if_they_have_different_caches_but_same_identifier) {
    mock_cache<r1> cache1,cache2;

    identifier id{"name"};
    resource<r1> res1{id,&cache1};
    resource<r1> res2{id,&cache2};

    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
}

TEST(resource_tests,two_resources_should_not_be_equal_if_they_have_same_caches_but_different_identifier) {
    mock_cache<r1> cache;

    identifier id1{"name1"};
    resource<r1> res1{id1,&cache};

    identifier id2{"name2"};
    resource<r1> res2{id2,&cache};

    EXPECT_FALSE(res1 == res2);
    EXPECT_TRUE(res1 != res2);
}

TEST(resource_tests,two_resources_should_be_equal_if_they_have_same_caches_and_identifier) {
    mock_cache<r1> cache;

    identifier id{"name"};
    resource<r1> res1{id,&cache};
    resource<r1> res2{id,&cache};

    EXPECT_TRUE(res1 == res2);
    EXPECT_FALSE(res1 != res2);
}

TEST(resource_tests,cache_does_not_have_resource_get_should_return_nullptr) {
    mock_cache<r1> cache;

    identifier id{"name"};
    resource<r1> c{id,&cache};

    EXPECT_CALL(cache,acquire(id)).Times(1).WillOnce(Return(nullptr));

    EXPECT_TRUE(c == nullptr);
}

TEST(resource_tests,cache_has_resource_get_should_return_the_resource) {
    mock_cache<r1> cache;

    identifier id{"name"};
    resource<r1> c{id,&cache};

    r1 res;
    EXPECT_CALL(cache,acquire(id)).Times(1).WillOnce(Return(&res));

    EXPECT_TRUE(c.get() == &res);
}
