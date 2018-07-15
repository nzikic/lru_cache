/*********************************************************************************
 *
 *  MIT License
 *
 *  Copyright (c) 2017 Nenad Zikic
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 *********************************************************************************/

#pragma once

#include <list>
#include <unordered_map>
#include <vector>
#include <optional>
#include <mutex>
#include <functional>
#include <ostream>

namespace lru_cache
{
    namespace detail
    {
        /* Dummy replacement for mutex which defaults to cache with no thread safety */
        class nolocking
        {
            public:
                void lock() {}
                void unlock() {}
                bool try_lock() { return true; }
        };
    }

    /* Associative Least Recently Used (LRU) cache */
    template <typename Key_t, typename Value_t, typename Lock_t = detail::nolocking>
    class LRU_cache
    {
            static_assert(
                    std::is_copy_constructible_v<Value_t>,
                    "Cache requires copy-constructible objects, otherwise data access with get member function would pop the data out of the cache, which renders cache pointless.");

        protected:
            using List_t = std::list<std::pair<Key_t, Value_t>>;
            using Map_t = std::unordered_map<Key_t, typename List_t::iterator>;
            using Lock = std::scoped_lock<Lock_t>;

            List_t  m_list;
            Map_t   m_map;
            const size_t  m_capacity;

            mutable Lock_t m_lock;

            std::function<void(Value_t)> m_onEvict;


            void evict()
            {
                auto lru = --m_list.end();

                if (m_onEvict)
                {
                    m_onEvict(lru->second);
                }

                m_map.erase(lru->first);
                m_list.pop_back();
            }


        public:

            explicit LRU_cache(size_t capacity = 8)
                : m_list()
                , m_map(capacity)
                , m_capacity(capacity)
            {
            }

            virtual ~LRU_cache() = default;

            LRU_cache(const LRU_cache&) = delete;
            LRU_cache& operator=(const LRU_cache&) = delete;

            LRU_cache(LRU_cache&&) = default;
            LRU_cache& operator=(LRU_cache&&) = default;

            template <typename Func>
            void setOnEvictNotifier(Func callback)
            {
                static_assert(std::is_invocable_v<Func, Value_t>, "Invalid type, expected function or callable object, or object callable to be called with argument of type Value_t");
                m_onEvict = callback;
            }

            /*
             * Inserts an element (value) with the given key (key).
             * If a value with the fiven key exists, function does nothing.
             */
            template <
                    typename Key_tt = Key_t,
                    typename Value_tt = Value_t,
                    class Key_tt_MustBe_Key_t = std::enable_if_t<std::is_same_v<std::remove_reference_t<Key_tt>, Key_t>>,
                    class Value_tt_MustBe_Value_t = std::enable_if_t<std::is_same_v<std::remove_reference_t<Value_tt>, Value_t>>>
            void insert(Key_tt&& key, Value_tt&& value)
            {
                Lock lock(m_lock);

                // lookup value in cache
                if (auto it = m_map.find(key); it == m_map.end())
                {
                    // insert item into cache (and check if cache is full)
                    if (m_list.size() >= m_capacity)
                    {
                        evict();
                    }

                    // insert element
                    m_list.emplace_front(std::make_pair(key, std::forward<Value_tt>(value)));
                    m_map[std::forward<Key_tt>(key)] = m_list.begin();
                }
            }

            /*
             * Retrieves the value by the given key (key).
             * If the given key isn't in cache, the function returns empty std::optional object.
             */
            template <typename Key_tt = Key_t,
                      class TypeMustBe_Key_t = std::enable_if_t<std::is_same_v<std::remove_reference_t<Key_tt>, Key_t>>>
            std::optional<Value_t> get(Key_tt&& key)
            {
                Lock lock(m_lock);

                // look in the map if element is present
                auto it = m_map.find(key);
                // if not return the empty optional
                if (it == m_map.end())
                {
                    return std::nullopt;
                }

                // element is present...
                auto list_iterator = it->second;

                if (list_iterator == m_list.begin())
                {
                    return { list_iterator->second };
                }
                else
                {
                    // move the accessed element to front of the list
                    m_list.splice(m_list.begin(), m_list, list_iterator);
                    // and update hash map
                    m_map[key] = m_list.begin();

                    return { m_list.begin()->second };
                }
            }

            /*
             * Purges the cache
             */
            virtual void purge()
            {
                Lock lock(m_lock);

                m_list.clear();
                m_map.clear();
            }

            /*
             * Releases all key/value pairs into a vector and purges the cache.
             */
            virtual std::vector<std::pair<Key_t, Value_t>> release()
            {
                Lock lock(m_lock);

                std::vector<std::pair<Key_t, Value_t>> v;

                for (auto& pair : m_list)
                {
                    v.emplace_back(std::move(pair));
                }

                purge();
                return v;
            }

            /*
             * Friend funtion for outputing key/value pairs.
             */
            friend std::ostream& operator<<(std::ostream& os, const LRU_cache& cache)
            {
                for (auto& pair : cache.m_list)
                {
                    os << "Key: " << pair.first << "\tValue: " << pair.second << std::endl;
                }

                return os;
            }
    };
}
