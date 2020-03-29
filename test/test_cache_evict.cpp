#include "lru_cache/lru_cache.hpp"
#include "test_parameters.h"

#include <gtest/gtest.h>
#include <gtest/gtest-param-test.h>
#include <gmock/gmock-matchers.h>


namespace lru_test::evict_test
{
    struct Evicted
    {
        Evicted(std::initializer_list<int> il) : evicted{il} {}
        std::vector<int> evicted;
    };

    struct Cache_parameters_evict : public Cache_parameters_content
    {
        Cache_parameters_evict(Input_content&& input_values,
                               Expected_content&& expected_content,
                               Evicted evicted)
            : Cache_parameters_content{std::move(input_values), std::move(expected_content)}
            , evicted{std::move(evicted.evicted)}
        {}

        std::vector<int> evicted;

        void print(std::ostream& os) const override
        {
            std::string separator = " ";
            Cache_parameters::print(os);
            os << ", evicted: [";
            for (auto& e : evicted)
            {
                os << separator << e;
                separator = ", ";
            }
            os << " ]";
        }

        friend void PrintTo(const Cache_parameters_evict& param, std::ostream* os)
        {
            *os << "{ "; param.print(*os); *os << " }";
        }
    };

    class Cache_tests_evict : public ::testing::TestWithParam<Cache_parameters_evict>
    {
        protected:
            lru_cache::LRU_cache<int, int> cache{4};
            std::vector<int> evicted;
    };

    TEST_P(Cache_tests_evict, element_evicted_when_cache_is_full)
    {
        auto row = GetParam();
        cache.setOnEvictNotifier([this](int e) { evicted.push_back(e); });

        for (auto& [k, v] : row.input)
            cache.insert(k, v);

        const auto content = cache.release();

        EXPECT_THAT(evicted, ::testing::ContainerEq(row.evicted));
        EXPECT_THAT(content, ::testing::ContainerEq(row.content));
    }

    INSTANTIATE_TEST_CASE_P(Evict_content,
                            Cache_tests_evict,
                            ::testing::Values(
                                Cache_parameters_evict{Input_content{ },                                          Expected_content{ },                            Evicted{ }},
                                Cache_parameters_evict{Input_content{ {3,3}, {2,2}, {1,1} },                      Expected_content{ {1,1}, {2,2}, {3,3} },        Evicted{ }},
                                Cache_parameters_evict{Input_content{ {3,3}, {2,2}, {1,1}, {0,0} },               Expected_content{ {0,0}, {1,1}, {2,2}, {3,3} }, Evicted{ }},
                                Cache_parameters_evict{Input_content{ {4,4}, {3,3}, {2,2}, {1,1}, {0,0} },        Expected_content{ {0,0}, {1,1}, {2,2}, {3,3} }, Evicted{ 4 }},
                                Cache_parameters_evict{Input_content{ {5,5}, {4,4}, {3,3}, {2,2}, {1,1}, {0,0} }, Expected_content{ {0,0}, {1,1}, {2,2}, {3,3} }, Evicted{ 5, 4 }}
                            ));
}
