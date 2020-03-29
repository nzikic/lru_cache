#pragma once

#include <sstream>
#include <initializer_list>
#include <vector>

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

    struct Cache_parameters_content : public Cache_parameters
    {
        Cache_parameters_content(Input_content&& input_values,
                                 Expected_content&& expected_content)
            : Cache_parameters{std::move(input_values)}
            , content{std::move(expected_content.content)}
        {}

        std::vector<std::pair<int, int>> content;

        void print(std::ostream &os) const override
        {
            std::string separator{" "};
            Cache_parameters::print(os);
            os << ", content: [";
            for (auto& [k,v] : content)
            {
                os << separator << "{" << k << "," << v << "}";
                separator = ", ";
            }
            os << "]";
        }

        friend void PrintTo(const Cache_parameters_content& param, std::ostream* os)
        {
            *os << "{ "; param.print(*os); *os << " }";
        }
    };

}
