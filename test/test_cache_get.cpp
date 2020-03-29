#include "lru_cache/lru_cache.hpp"
#include "test_parameters.h"

#include <gtest/gtest.h>
#include <gtest/gtest-param-test.h>
#include <gmock/gmock-matchers.h>

namespace lru_test::get_tests
{
    class Elem;
    using ElemVtr = std::vector<Elem>;

    class Elem
    {
            std::string m_val;

        public:
            Elem(const std::string& value) : m_val{value} {}

            Elem(const Elem& other) = default;
            Elem& operator=(const Elem& other) = default;

            std::string value() const { return m_val; }

            friend bool operator==(const Elem& lhs, const Elem& rhs) { return lhs.m_val == rhs.m_val; };
            friend std::ostream& operator<<(std::ostream& os, Elem const& e) { os << "[" << e.m_val << "]"; return os; }
    };
}


namespace std
{
    template<> struct hash<lru_test::get_tests::Elem> {
        size_t operator()(lru_test::get_tests::Elem const& e) {
            return std::hash<std::string>{}(e.value());
        }
    };
}

namespace lru_test::get_tests
{

    std::ostream& operator<<(std::ostream& os, ElemVtr const& v) {
        std::string separator = " ";
        os << "{ ";
        for(auto& e : v) {
            os << separator << e;
            separator = ", ";
        }
        os << " }";

        return os;
    }

    class Cache_get_tests : public ::testing::Test
    {
        public:
            size_t key(Elem const& e) { return std::hash<lru_test::get_tests::Elem>{}(e); }
        protected:
            std::vector<Elem> v = { Elem{"Cate"}, Elem{"Doggo"}, Elem{"Good guy Craig"}, Elem{"Travolta"}, Elem{"Derp"}, Elem{"Derpina"}, Elem{"Overly Attached Girlfriend"}, Elem{"Forever Alone"}};
            lru_cache::LRU_cache<size_t, Elem> cache{4};
    };

    TEST_F(Cache_get_tests, get_from_empty_cache_returns_empty_optional)
    {
        auto e = cache.get(key(v[0]));

        ASSERT_EQ(e.has_value(), false);
    }

    TEST_F(Cache_get_tests, get_from_missing_value_from_cache_returns_empty_optional)
    {
        cache.insert(key(v[0]), v[0]);
        cache.insert(key(v[1]), v[1]);
        cache.insert(key(v[5]), v[5]);

        auto e = cache.get(key(v[7]));

        ASSERT_EQ(e.has_value(), false);
    }

    TEST_F(Cache_get_tests, get_from_existing_value_from_cache_returns_valid_optional)
    {
        cache.insert(key(v[0]), v[0]);
        cache.insert(key(v[1]), v[1]);
        cache.insert(key(v[5]), v[5]);

        auto e = cache.get(key(v[0]));

        EXPECT_EQ(e.has_value(), true);
        EXPECT_EQ(*e, v[0]);
    }
}
