#include "cache.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <gmock/gmock-matchers.h>

namespace cache::tests
{
using namespace ::testing;

    class MockMemeDataBase : public IMemeDatabaseBase
    {
        public:
            MockMemeDataBase() = default;
            MOCK_METHOD(Meme, get_by_name, (const std::string&), (override));
            MOCK_METHOD(void, update_meme, (const Meme&), (override));
    };

    class MemeDatabaseTests : public ::testing::Test
    {
        public:
            void SetUp() override
            {
                ON_CALL(*db, get_by_name("Woman Yelling at a Cat")).WillByDefault(Return(Meme{"Woman Yelling at a Cat", "https://knowyourmeme.com/memes/woman-yelling-at-a-cat"}));
                ON_CALL(*db, get_by_name("Doge")).WillByDefault(Return(Meme{"Doge", "https://knowyourmeme.com/memes/doge"}));
                ON_CALL(*db, get_by_name("Good guy")).WillByDefault(Return(Meme{"Good guy", "https://knowyourmeme.com/memes/good-guy-greg"}));
                ON_CALL(*db, get_by_name("Travolta")).WillByDefault(Return(Meme{"Travolta", "https://knowyourmeme.com/memes/confused-travolta"}));
                ON_CALL(*db, get_by_name("10guy")).WillByDefault(Return(Meme{"10guy", "https://knowyourmeme.com/memes/10-guy"}));
                ON_CALL(*db, get_by_name("Wat")).WillByDefault(Return(Meme{"Wat", "https://knowyourmeme.com/memes/wat"}));
                ON_CALL(*db, get_by_name("Sweet Brown")).WillByDefault(Return(Meme{"Sweet Brown", "https://knowyourmeme.com/memes/sweet-brown-aint-nobody-got-time-for-that"}));
                ON_CALL(*db, get_by_name("Derpina")).WillByDefault(Return(Meme{"Derpina", "https://knowyourmeme.com/memes/derpina"}));
                ON_CALL(*db, get_by_name("Girlfriend")).WillByDefault(Return(Meme{"Girlfriend", "https://knowyourmeme.com/memes/overly-attached-girlfriend"}));
                ON_CALL(*db, get_by_name("Forever Alone")).WillByDefault(Return(Meme{"Forever Alone", "https://knowyourmeme.com/memes/forever-alone"}));
            }

       protected:
           MockMemeDataBase* db = new ::testing::NiceMock<MockMemeDataBase>;
           MemeProvider provider{db, 2};
    };

    TEST_F(MemeDatabaseTests, getting_meme_when_not_in_cache_calls_database)
    {
        EXPECT_CALL(*db, get_by_name("Doge")).Times(1);
        EXPECT_CALL(*db, get_by_name("10guy")).Times(1);
        EXPECT_CALL(*db, get_by_name("Wat")).Times(1);

        provider.get("Doge");
        provider.get("10guy");
        provider.get("Wat");
    }

    TEST_F(MemeDatabaseTests, getting_meme_when_in_cache_doesnt_call_database)
    {
        EXPECT_CALL(*db, get_by_name("Doge")).Times(1);
        EXPECT_CALL(*db, get_by_name("Wat")).Times(1);

        provider.get("Doge");
        provider.get("Wat");
        provider.get("Doge");
        provider.get("Doge");
    }

    TEST_F(MemeDatabaseTests, ecivtion_not_called_when_cache_not_full_yet)
    {
        EXPECT_CALL(*db, update_meme(_)).Times(0);

        provider.get("Doge");
        provider.get("10guy");
    }

    TEST_F(MemeDatabaseTests, eviction_from_cache_updated_database)
    {
        InSequence s;
        EXPECT_CALL(*db, update_meme(Meme{"Doge", "https://knowyourmeme.com/memes/doge"})).Times(1);
        EXPECT_CALL(*db, update_meme(Meme{"10guy", "https://knowyourmeme.com/memes/10-guy"})).Times(1);
        EXPECT_CALL(*db, update_meme(Meme{"Wat", "https://knowyourmeme.com/memes/wat"})).Times(1);

        provider.get("Doge");
        provider.get("10guy");
        provider.get("Wat");
        provider.get("Derpina");
        provider.get("Travolta");
    }

    TEST_F(MemeDatabaseTests, eviction_happened_correctly_for_least_recently_used)
    {
        InSequence s;
        EXPECT_CALL(*db, update_meme(Meme{"10guy", "https://knowyourmeme.com/memes/10-guy"})).Times(1);
        EXPECT_CALL(*db, update_meme(Meme{"Wat", "https://knowyourmeme.com/memes/wat"})).Times(1);
        EXPECT_CALL(*db, update_meme(Meme{"Doge", "https://knowyourmeme.com/memes/doge"})).Times(1);

        provider.get("Doge");
        provider.get("10guy");
        provider.get("Doge");
        provider.get("Wat");
        provider.get("Doge");
        provider.get("Derpina");
        provider.get("Sweet Brown");
    }

}
