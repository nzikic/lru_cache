add_executable(lru_cache_test)

target_include_directories (lru_cache_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(lru_cache_test
    PRIVATE
        lru_cache
        GMock::main
)

target_sources(lru_cache_test
    PRIVATE
        test_parameters.h
        test_cache_size.cpp
        test_cache_evict.cpp
        test_cache_content.cpp
        test_cache_get.cpp
)

add_test(Testing lru_cache_test)

install (TARGETS lru_cache_test DESTINATION ${CMAKE_INSTALL_PREFIX}/lru_tests/)
