#include "lru_cache/lru_cache.hpp"
#include "test_parameters.h"
#include <gtest/gtest.h>
#include <gtest/gtest-param-test.h>
#include <gmock/gmock-matchers.h>


namespace lru_test::size_test
{

    struct Cache_parameters_size : public Cache_parameters
    {
        Cache_parameters_size(Input_content&& input_values, int expected_size)
            : Cache_parameters{std::move(input_values)}
            , expected_size{expected_size}
        {}

        int expected_size;

        void print(std::ostream& os) const override
        {
            Cache_parameters::print(os);
            os << ", expected: " << expected_size;
        }

        friend void PrintTo(const Cache_parameters_size& param, std::ostream* os)
        {
            *os << "{ "; param.print(*os); *os << " }";
        }
    };

    class Cache_size_tests : public ::testing::TestWithParam<Cache_parameters_size>
    {
        protected:
            lru_cache::LRU_cache<int, int> cache{4};
    };


    TEST_P(Cache_size_tests, inserting_number_of_elements_releases_not_more_than_capacity_elements)
    {
        auto row = GetParam();
        for (auto [k,v] : row.input)
        {
            cache.insert(k, v);
        }

        const auto content = cache.release();
        ASSERT_THAT(content, ::testing::SizeIs(row.expected_size));
    }

    INSTANTIATE_TEST_CASE_P(Inserting_content,
                            Cache_size_tests,
                            ::testing::Values(
                                Cache_parameters_size{Input_content{ }, 0},
                                Cache_parameters_size{Input_content{ {3,3}, {2,2}, {1,1} }, 3},
                                Cache_parameters_size{Input_content{ {3,3}, {2,2}, {1,1}, {0,0} }, 4},
                                Cache_parameters_size{Input_content{ {5,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0} }, 4})
                            );

    TEST_F(Cache_size_tests, inserting_6_elements_into_size_4_cache_containst_last_4_inserted_elements)
    {
        cache.insert(5, 5);
        cache.insert(4, 4);
        cache.insert(3, 3);
        cache.insert(2, 2);
        cache.insert(1, 1);
        cache.insert(0, 0);

        const std::vector<std::pair<int, int>> expected{ {0,0}, {1,1}, {2,2}, {3,3} };
        const auto content = cache.release();
        ASSERT_THAT(content, ::testing::ContainerEq(expected));
    }
}
