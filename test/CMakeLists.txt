cmake_minimum_required (VERSION 3.8 FATAL_ERROR)

#
add_executable(lru_cache_test)

target_include_directories (lru_cache_test PRIVATE ./)

target_link_libraries(lru_cache_test
    lru_cache
    GTest::GTest
)

target_sources(lru_cache_test
    PRIVATE
        main.cpp
        test_cache_size.cpp
        test_cache_evict.cpp
        test_parameters.h
)

add_test(Testing lru_cache_test)

install (TARGETS lru_cache_test DESTINATION ${CMAKE_INSTALL_PREFIX}/lru_tests/)
