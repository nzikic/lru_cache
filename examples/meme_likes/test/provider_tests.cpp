#include "cache.h"

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

namespace cache::tests
{
    using namespace ::testing;

    class MemeProviderTests : public ::testing::Test
    {
        protected:
            MemeProvider provider{new MemeDataBase, 2};
    };

    TEST_F(MemeProviderTests, getting_meme_not_in_database_returns_empty_pointer)
    {
        auto troll = provider.get("Trollface");
        ASSERT_THAT(troll, IsNull());
    }

    TEST_F(MemeProviderTests, popularity_is_preserved_in_cache_without_eviciton)
    {
        auto cate = provider.get("Woman Yelling at a Cat");
        cate->like();
        cate->like();

        auto popularity = cate->popularity();
        cate.reset();

        auto cate_again = provider.get("Woman Yelling at a Cat");
        auto popularity_again = cate_again->popularity();
        ASSERT_EQ(popularity, popularity_again);
    }

    TEST_F(MemeProviderTests, popularity_is_preserved_in_when_meme_is_evicted)
    {
        auto cate = provider.get("Woman Yelling at a Cat");
        cate->like();
        cate->like();
        cate->like();

        auto tenguy = provider.get("10guy");
        tenguy->like();

        auto wat = provider.get("Wat");

        auto cate_again = provider.get("Woman Yelling at a Cat");
        ASSERT_EQ(cate_again->popularity(), 3);
    }
}
