#include "cache.h"
#include "meme.h"

#include <functional>


namespace cache
{

    MemeProvider::MemeProvider(IMemeDatabaseBase* db, size_t cache_size)
        : db{db}
        , cache{cache_size}
    {
        db->initialize(
            {
                Meme{"Woman Yelling at a Cat", "https://knowyourmeme.com/memes/woman-yelling-at-a-cat"},
                Meme{"Doge", "https://knowyourmeme.com/memes/doge"},
                Meme{"Good guy", "https://knowyourmeme.com/memes/good-guy-greg"},
                Meme{"Travolta", "https://knowyourmeme.com/memes/confused-travolta"},
                Meme{"10guy", "https://knowyourmeme.com/memes/10-guy"},
                Meme{"Wat", "https://knowyourmeme.com/memes/wat"},
                Meme{"Sweet Brown", "https://knowyourmeme.com/memes/sweet-brown-aint-nobody-got-time-for-that"},
                Meme{"Derpina", "https://knowyourmeme.com/memes/derpina"},
                Meme{"Girlfriend", "https://knowyourmeme.com/memes/overly-attached-girlfriend"},
                Meme{"Forever Alone", "https://knowyourmeme.com/memes/forever-alone"}
            });

        cache.setOnEvictNotifier([this](auto meme) { this->onMemeEvicted(meme); });
    }

    std::shared_ptr<Meme> MemeProvider::get(const std::string& name)
    {
        if (auto m = cache.get(name);
            m.has_value())
        {
            return m.value();
        }
        else
        {
            try
            {
                auto meme = std::make_shared<Meme>(db->get_by_name(name));
                cache.insert(name, meme);
                return meme;
            }
            catch (...)
            {
                return nullptr;
            }
        }
    }

    void MemeProvider::onMemeEvicted(std::shared_ptr<Meme> m)
    {
        db->update_meme(*m);
    }
}
