#pragma once

#include "meme.h"
#include "lru_cache/lru_cache.hpp"
#include <memory>


namespace cache
{
    class MemeProvider
    {
        public:
            explicit MemeProvider(IMemeDatabaseBase* db, size_t cache_size);

            /** Checks cache first.
             * If meme is not in cache,
             * it gets the meme from database
             * caches it
             *
             * @param name - meme name
             * @returns Meme - shared pointer to meme with the given name, empty pointer if no meme with the name is present
            **/
            std::shared_ptr<Meme> get(const std::string& name);

            /** Callback registered with the cache when meme is evicted from cache
             * Evicted meme is updated implicitly and each request updates the cache */
            void onMemeEvicted(std::shared_ptr<Meme> m);

        protected:
            std::unique_ptr<IMemeDatabaseBase> db;
            lru_cache::LRU_cache<std::string, std::shared_ptr<Meme>> cache;
    };
}
