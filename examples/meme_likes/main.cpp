#include "cache.h"
#include <iostream>

int main()
{
    using namespace cache;
    MemeProvider provider{new MemeDataBase, 4};

    auto doge = provider.get("Doge");
    auto girlfriend = provider.get("Girlfriend");
    auto derpina = provider.get("Derpina");

    //
    std::cout << "I like doggos\n";
    doge->like();

    //
    std::cout << "I prefer Derpina over overly attached girlfriends...";
    girlfriend->like();
    derpina->like();
    derpina->like();

    //
    std::cout << "But cats are my favourites!\n";
    auto cate = provider.get("Woman Yelling at a Cat");
    cate->like();

    //
    auto trollface = provider.get("Trollface");
    if (trollface)
        trollface->like();
    else
        std::cout << "They don't have trollface :/\n";

    std::cout << "\n\n"
                 "***********************************\n"
                 " It's not 9gag but it's still fun!\n"
                 "***********************************\n";
    return 0;
}
