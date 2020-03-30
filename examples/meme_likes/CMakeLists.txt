# library
add_library(lib_example_meme_likes STATIC cache.cpp)

target_include_directories(lib_example_meme_likes PUBLIC ./)
target_link_libraries(lib_example_meme_likes PUBLIC lru_cache)
target_sources(lib_example_meme_likes PRIVATE meme.h cache.h cache.cpp)

# executable
add_executable(example_meme_likes main.cpp)

target_link_libraries(example_meme_likes lib_example_meme_likes)

install(TARGETS example_meme_likes DESTINATION ${CMAKE_INSTALL_PREFIX}/examples/meme_likes/)

# tests
if (LRU_CACHE_BUILD_UNIT_TESTS)

    add_subdirectory(test/)

endif(LRU_CACHE_BUILD_UNIT_TESTS)
