#include "lru_cache/lru_cache.hpp"
#include "test_parameters.h"

#include <gtest/gtest.h>
#include <gtest/gtest-param-test.h>
#include <gmock/gmock-matchers.h>

namespace lru_test::content_test
{

    class Cache_tests_content : public ::testing::TestWithParam<Cache_parameters_content>
    {
        public:
            Cache_tests_content() : params{GetParam()}, cache{4}
            {
                for (auto [k,v] : params.input)
                {
                    cache.insert(k,v);
                }
            }

        protected:
            const ParamType& params;
            lru_cache::LRU_cache<int, int> cache;
    };

    TEST_P(Cache_tests_content, inserting_elements_into_cache_and_check_content)
    {
        const auto actual_content = cache.release();
        EXPECT_THAT(actual_content, ::testing::SizeIs(params.content.size()));
        ASSERT_THAT(actual_content, ::testing::ContainerEq(params.content));
    }

    TEST_P(Cache_tests_content, purge_leaves_empty_cache)
    {
        cache.purge();
        const auto actual_content = cache.release();
        ASSERT_THAT(actual_content, ::testing::SizeIs(0));
    }

    TEST_P(Cache_tests_content, release_purges_cache)
    {
        cache.release();
        const auto actual_content = cache.release();
        ASSERT_THAT(actual_content, ::testing::SizeIs(0));
    }

    INSTANTIATE_TEST_CASE_P(Check_content_when_inserting,
                            Cache_tests_content,
                            ::testing::Values(
                                Cache_parameters_content {Input_content{}, Expected_content{}},
                                Cache_parameters_content {Input_content{{0,0}}, Expected_content{{0,0}}},
                                Cache_parameters_content {Input_content{{5,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0}}, Expected_content{{0,0}, {1,1}, {2,2}, {3,3}}},
                                Cache_parameters_content {Input_content{{5,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0}, {5,5}, {0,0}, {1,1}, {2,2}, {1,1}, {6,6}}, Expected_content{{6,6}, {5,5}, {0,0}, {1,1}}},
                                Cache_parameters_content {Input_content{{9,9}, {9,9}, {9,9}, {9,9}, {9,9}, {9,9}}, Expected_content{{9,9}}}
                            ));
}
