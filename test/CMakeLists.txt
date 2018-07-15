cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

project (lru_cache VERSION 1.0)

#
add_executable(lru_cache_test main.cpp)

target_include_directories (lru_cache_test PRIVATE ./)

target_link_libraries(lru_cache_test
    lru_cache
    GTest::GTest
)

target_sources(lru_cache_test
    PRIVATE
        main.cpp
)

install (TARGETS lru_cache_test DESTINATION ${CMAKE_INSTALL_PREFIX}/lru_tests/)
