#pragma once

#include <sstream>
#include <initializer_list>

namespace lru_test
{
    struct ICache_parameters
    {
        virtual ~ICache_parameters() = default;

        virtual void print(std::ostream& os) const = 0;
    };

    struct Input_content
    {
        Input_content(std::initializer_list<std::pair<int, int>> il) : content{il} {};
        std::vector<std::pair<int, int>> content;
    };

    struct Expected_content
    {
        Expected_content(std::initializer_list<std::pair<int, int>> il) : content{il} {};
        std::vector<std::pair<int, int>> content;
    };

    struct Cache_parameters : public ICache_parameters
    {
        Cache_parameters(Input_content&& input_values)
            : input{std::move(input_values.content)}
        {}

        std::vector<std::pair<int, int>> input;

        void print(std::ostream& os) const override
        {
            std::string separator{" "};

            os << "input: [";
            for (auto& [k,v] : input) {
                os << separator << "{" << k << "," << v << "}";
                separator = ", ";
            }
            os << " ]";
        }

        friend void PrintTo(const Cache_parameters& param, std::ostream* os)
        {
            *os << "{ "; param.print(*os); *os << " }";
        }
    };

}
