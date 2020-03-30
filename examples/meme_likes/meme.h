#pragma once

#include <string>
#include <ostream>
#include <initializer_list>
#include <unordered_map>
#include <stdexcept>

namespace cache
{

    class Meme
    {
        public:
            explicit Meme(std::string name, std::string url)
                : m_name{name}, m_url{url}, m_popularity{0}
            {}

            Meme (const Meme&) = default;
            Meme& operator=(const Meme&) = default;


            std::string name() const { return m_name; }
            std::string url() const { return m_url; }

            void like() { ++m_popularity; }
            size_t popularity() const { return m_popularity; }

            friend bool operator==(const Meme& lhs, const Meme& rhs) {
                return lhs.name() == rhs.name() &&
                       lhs.url() == rhs.url() &&
                       lhs.popularity() == rhs.popularity();
            }

            friend std::ostream& operator<<(std::ostream& os, const Meme& meme) {
                os << "{ name : [" << meme.m_name << "] | url : [" << meme.m_url << "] | popularity : [" << meme.m_popularity << "] }";
                return os;
            }

        private:
            std::string m_name;
            std::string m_url;
            size_t m_popularity;
    };

    /* fake database, which might be expensive to access */
    class IMemeDatabaseBase
    {
        public:
            virtual ~IMemeDatabaseBase() = default;

            void initialize(std::initializer_list<Meme> il)
            {
                for(auto m : il)
                {
                    memes.insert(std::make_pair(m.name(), m));
                }
            }

            virtual Meme get_by_name(const std::string& name) = 0;
            virtual void update_meme(const Meme& meme) = 0;

        protected:
            std::unordered_map<std::string, Meme> memes;
    };

    class MemeDataBase : public IMemeDatabaseBase
    {
        public:
            explicit MemeDataBase() {};

            // IMemeDatabase interface
            /** Get meme from database
             * @param name - meme name
             * @returns Meme - meme with the given name
             * @exception std::out_of_range if no meme with the name is present
            **/
            Meme get_by_name(const std::string& name) override
            {
                if (auto it = memes.find(name);
                    it != memes.end())
                {
                    return it->second;
                }

                throw std::out_of_range{name + " not found in db"};
            }

            /** Updates database for the meme record */
            void update_meme(const Meme& meme) override
            {
                if (auto it = memes.find(meme.name());
                    it != memes.end())
                {
                    it->second = meme;
                }
            }
    };
}
